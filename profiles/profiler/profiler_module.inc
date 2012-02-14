<?php

/**
 * @file
 * Contains backports of Drupal 7 install profile and graph builder functions.
 */

/**
 * Generate a cache of module files with proper dependency tree relationships
 * fully built. Modeled after module_rebuild_cache() but without requiring a
 * database to be present.
 *
 * @return
 *   The array of filesystem objects used to rebuild the cache.
 */
function profiler_module_rebuild_cache() {
  // Get current list of modules
  $files = drupal_system_listing('\.module$', 'modules', 'name', 0);
  ksort($files);

  // Set defaults for module info
  $defaults = array(
    'dependencies' => array(),
    'dependents' => array(),
    'description' => '',
    'version' => NULL,
    'php' => DRUPAL_MINIMUM_PHP,
  );

  foreach ($files as $filename => $file) {
    // Look for the info file.
    $file->info = drupal_parse_info_file(dirname($file->filename) .'/'. $file->name .'.info');

    // Skip modules that don't provide info.
    if (empty($file->info)) {
      unset($files[$filename]);
      continue;
    }
    // Merge in defaults and save.
    $files[$filename]->info = $file->info + $defaults;
  }
  $files = _profiler_module_build_dependencies($files);
  return $files;
}

/**
 * Find dependencies any level deep and fill in required by information too.
 *
 * @param $files
 *   The array of filesystem objects used to rebuild the cache.
 *
 * @return
 *   The same array with the new keys for each module:
 *   - requires: An array with the keys being the modules that this module
 *     requires.
 *   - required_by: An array with the keys being the modules that will not work
 *     without this module.
 */
function _profiler_module_build_dependencies($files) {
  // require_once DRUPAL_ROOT . '/includes/graph.inc';
  foreach ($files as $filename => $file) {
    $graph[$file->name]['edges'] = array();
    if (isset($file->info['dependencies']) && is_array($file->info['dependencies'])) {
      foreach ($file->info['dependencies'] as $dependency) {
        $dependency_data = profiler_drupal_parse_dependency($dependency);
        $graph[$file->name]['edges'][$dependency_data['name']] = $dependency_data;
      }
    }
  }
  profiler_drupal_depth_first_search($graph);
  foreach ($graph as $module => $data) {
    $files[$module]->required_by = isset($data['reverse_paths']) ? $data['reverse_paths'] : array();
    $files[$module]->requires = isset($data['paths']) ? $data['paths'] : array();
    $files[$module]->sort = $data['weight'];
  }
  return $files;
}

/**
 * Parse a dependency for comparison by drupal_check_incompatibility().
 *
 * @param $dependency
 *   A dependency string, for example 'foo (>=7.x-4.5-beta5, 3.x)'.
 * @return
 *   An associative array with three keys:
 *   - 'name' includes the name of the thing to depend on (e.g. 'foo').
 *   - 'original_version' contains the original version string (which can be
 *     used in the UI for reporting incompatibilities).
 *   - 'versions' is a list of associative arrays, each containing the keys
 *     'op' and 'version'. 'op' can be one of: '=', '==', '!=', '<>', '<',
 *     '<=', '>', or '>='. 'version' is one piece like '4.5-beta3'.
 *   Callers should pass this structure to drupal_check_incompatibility().
 *
 * @see drupal_check_incompatibility()
 */
function profiler_drupal_parse_dependency($dependency) {
  // We use named subpatterns and support every op that version_compare
  // supports. Also, op is optional and defaults to equals.
  $p_op = '(?P<operation>!=|==|=|<|<=|>|>=|<>)?';
  // Core version is always optional: 7.x-2.x and 2.x is treated the same.
  $p_core = '(?:' . preg_quote(DRUPAL_CORE_COMPATIBILITY) . '-)?';
  $p_major = '(?P<major>\d+)';
  // By setting the minor version to x, branches can be matched.
  $p_minor = '(?P<minor>(?:\d+|x)(?:-[A-Za-z]+\d+)?)';
  $value = array();
  $parts = explode('(', $dependency, 2);
  $value['name'] = trim($parts[0]);
  if (isset($parts[1])) {
    $value['original_version'] = ' (' . $parts[1];
    foreach (explode(',', $parts[1]) as $version) {
      if (preg_match("/^\s*$p_op\s*$p_core$p_major\.$p_minor/", $version, $matches)) {
        $op = !empty($matches['operation']) ? $matches['operation'] : '=';
        if ($matches['minor'] == 'x') {
          // Drupal considers "2.x" to mean any version that begins with
          // "2" (e.g. 2.0, 2.9 are all "2.x"). PHP's version_compare(),
          // on the other hand, treats "x" as a string; so to
          // version_compare(), "2.x" is considered less than 2.0. This
          // means that >=2.x and <2.x are handled by version_compare()
          // as we need, but > and <= are not.
          if ($op == '>' || $op == '<=') {
            $matches['major']++;
          }
          // Equivalence can be checked by adding two restrictions.
          if ($op == '=' || $op == '==') {
            $value['versions'][] = array('op' => '<', 'version' => ($matches['major'] + 1) . '.x');
            $op = '>=';
          }
        }
        $value['versions'][] = array('op' => $op, 'version' => $matches['major'] . '.' . $matches['minor']);
      }
    }
  }
  return $value;
}

/**
 * Perform a depth first sort on a directed acyclic graph.
 *
 * @param $graph
 *   A three dimensional associated array, with the first keys being the names
 *   of the vertices, these can be strings or numbers. The second key is
 *   'edges' and the third one are again vertices, each such key representing
 *   an edge. Values of array elements are copied over.
 *
 *   Example:
 *   @code
 *     $graph[1]['edges'][2] = 1;
 *     $graph[2]['edges'][3] = 1;
 *     $graph[2]['edges'][4] = 1;
 *     $graph[3]['edges'][4] = 1;
 *   @endcode
 *
 *   On return you will also have:
 *   @code
 *     $graph[1]['paths'][2] = 1;
 *     $graph[1]['paths'][3] = 1;
 *     $graph[2]['reverse_paths'][1] = 1;
 *     $graph[3]['reverse_paths'][1] = 1;
 *   @endcode
 *
 * @return
 *   The passed in $graph with more secondary keys filled in:
 *   - 'paths': Contains a list of vertices than can be reached on a path from
 *     this vertex.
 *   - 'reverse_paths': Contains a list of vertices that has a path from them
 *     to this vertex.
 *   - 'weight': If there is a path from a vertex to another then the weight of
 *     the latter is higher.
 *   - 'component': Vertices in the same component have the same component
 *     identifier.
 *
 * @see _drupal_depth_first_search()
 */
function profiler_drupal_depth_first_search(&$graph) {
  $state = array(
    // The order of last visit of the depth first search. This is the reverse
    // of the topological order if the graph is acyclic.
    'last_visit_order' => array(),
    // The components of the graph.
    'components' => array(),
  );
  // Perform the actual sort.
  foreach ($graph as $start => $data) {
    _profiler_drupal_depth_first_search($graph, $state, $start);
  }

  // We do such a numbering that every component starts with 0. This is useful
  // for module installs as we can install every 0 weighted module in one
  // request, and then every 1 weighted etc.
  $component_weights = array();

  foreach ($state['last_visit_order'] as $vertex) {
    $component = $graph[$vertex]['component'];
    if (!isset($component_weights[$component])) {
      $component_weights[$component] = 0;
    }
    $graph[$vertex]['weight'] = $component_weights[$component]--;
  }
}

/**
 * Helper function to perform a depth first sort.
 *
 * @param &$graph
 *   A three dimensional associated graph array.
 * @param &$state
 *   An associative array. The key 'last_visit_order' stores a list of the
 *   vertices visited. The key components stores list of vertices belonging
 *   to the same the component.
 * @param $start
 *   An arbitrary vertex where we started traversing the graph.
 * @param &$component
 *   The component of the last vertex.
 *
 * @see drupal_depth_first_search()
 */
function _profiler_drupal_depth_first_search(&$graph, &$state, $start, &$component = NULL) {
  // Assign new component for each new vertex, i.e. when not called recursively.
  if (!isset($component)) {
    $component = $start;
  }
  // Nothing to do, if we already visited this vertex.
  if (isset($graph[$start]['paths'])) {
    return;
  }
  // Mark $start as visited.
  $graph[$start]['paths'] = array();

  // Assign $start to the current component.
  $graph[$start]['component'] = $component;
  $state['components'][$component][] = $start;

  // Visit edges of $start.
  if (isset($graph[$start]['edges'])) {
    foreach ($graph[$start]['edges'] as $end => $v) {
      // Mark that $start can reach $end.
      $graph[$start]['paths'][$end] = $v;

      if (isset($graph[$end]['component']) && $component != $graph[$end]['component']) {
        // This vertex already has a component, use that from now on and
        // reassign all the previously explored vertices.
        $new_component = $graph[$end]['component'];
        foreach ($state['components'][$component] as $vertex) {
          $graph[$vertex]['component'] = $new_component;
          $state['components'][$new_component][] = $vertex;
        }
        unset($state['components'][$component]);
        $component = $new_component;
      }
      // Only visit existing vertices.
      if (isset($graph[$end])) {
        // Visit the connected vertex.
        _profiler_drupal_depth_first_search($graph, $state, $end, $component);

        // All vertices reachable by $end are also reachable by $start.
        $graph[$start]['paths'] += $graph[$end]['paths'];
      }
    }
  }

  // Now that any other subgraph has been explored, add $start to all reverse
  // paths.
  foreach ($graph[$start]['paths'] as $end => $v) {
    if (isset($graph[$end])) {
      $graph[$end]['reverse_paths'][$start] = $v;
    }
  }

  // Record the order of the last visit. This is the reverse of the
  // topological order if the graph is acyclic.
  $state['last_visit_order'][] = $start;
}
