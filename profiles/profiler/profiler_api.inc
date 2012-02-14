<?php

/**
 * @see hook_profile_modules().
 *
 * @param $config
 *   The config array for an Install Profile.
 */
function profiler_profile_modules($config) {
  // Retrieve install profile dependencies.
  $modules = isset($config['dependencies']) ? profiler_config_reduce($config['dependencies']) : array();

  // Include code for building the module dependency tree.
  require_once('profiler_module.inc');
  $files = profiler_module_rebuild_cache();

  // Always install required modules first. Respect the dependencies between
  // the modules.
  $required = array();
  $non_required = array();

  // Add modules that other modules depend on.
  foreach ($modules as $module) {
    if ($files[$module]->requires) {
      $modules = array_merge($modules, array_keys($files[$module]->requires));
    }
  }
  $modules = array_unique($modules);
  foreach ($modules as $module) {
    if (!empty($files[$module]->info['required'])) {
      $required[$module] = $files[$module]->sort;
    }
    else {
      $non_required[$module] = $files[$module]->sort;
    }
  }
  arsort($required);
  arsort($non_required);
  return array_unique(array_keys(array_merge($required, $non_required)));
}

/**
 * @see hook_profile_task_list().
 *
 * @param $config
 *   The config array for an Install Profile.
 */
function profiler_profile_task_list($config) {
  return array();
}

/**
 * @see hook_profile_tasks().
 *
 * @param $config
 *   The config array for an Install Profile.
 * @param $task
 *   The current $task of the install system. When hook_profile_tasks() is first
 *   called, this is 'profile'.
 * @param $url
 *   Complete URL to be used for a link or form action on a custom page, if
 *   providing any, to allow the user to proceed with the installation.
 */
function profiler_profile_tasks($config, &$task, $url) {
  if ($task == 'profile') {
    // Disable all DB blocks
    db_query("UPDATE {blocks} SET status = 0, region = ''");

    // We flush caches at the beginning to ensure that any necessary module
    // tasks, such as Features component rebuilding, is complete before the
    // remaining tasks are run.
    drupal_flush_all_caches();

    // Run any component installations
    profiler_install_components($config);

    // Run hook_install() for the install profile.
    global $profile;
    $function = "{$profile}_install";
    $file = "./profiles/{$profile}/{$profile}.install";
    if (file_exists($file)) {
      require_once($file);
    }
    if (function_exists($function)) {
      call_user_func($function);
    }

    // Rebuild key tables/caches
    menu_rebuild();
    module_rebuild_cache(); // Detects the newly added bootstrap modules
    node_access_rebuild();
    drupal_get_schema('system', TRUE); // Clear schema DB cache
    drupal_flush_all_caches();

    // Finish
    profiler_install_configure($config);
    $task = 'profile-finished';
  }
  return '';
}

/**
 * Helper to implementation of hook_form_alter() for Install profiles.
 *
 * Allows the profile to alter the site-configuration form. This is
 * called through custom invocation, so $form_state is not populated.
 *
 * @param $config
 *   The config array for an Install Profile.
 */
function profiler_form_alter($config, &$form, $form_state, $form_id) {
  // If site_name and site_mail are in the config, we can possibly bypass a step
  // during the installation.
  if ($form_id == 'install_configure' &&
    isset($config['variables']['site_name']) &&
    isset($config['variables']['site_mail']) &&
    profiler_config_load_superuser($config)
  ) {
    variable_set('site_name', $config['variables']['site_name']);
    variable_set('site_mail', $config['variables']['site_mail']);
  }
}

/**
 * Reduce an array of components with mixed keys to its final reduced set of
 * values allowing for later entries to override former entries. Example:
 *
 *   profiler_config_reduce(array('foo', 'bar', 'foo' => 0));
 *
 * Will return the following:
 *
 *   array('bar');
 */
function profiler_config_reduce($data) {
  $reduced = array();
  foreach ($data as $key => $value) {
    if (is_int($key)) {
      $reduced[$value] = TRUE;
    }
    elseif (is_string($key)) {
      $reduced[$key] = (bool) $value;
    }
  }
  return array_keys(array_filter($reduced));
}

/**
 * Take an array of arrays and return a single array merging the child arrays.
 * The resulting array can optionally be reduced & made unique.
 *
 * @param array $data
 *   An array of arrays.
 * @param array $excluded_keys
 *   An array of keys to exclude. By default, no keys are excluded.
 * @param boolean $process
 *   Optional. Reduce and make the merged array unique.
 * @return array
 *   A single merged array.
 */
function profiler_config_merge($data, $excluded_keys = array(), $process = TRUE) {
  $merged = array();
  foreach ($data as $key => $value) {
    if (is_array($value) && !in_array($key, $excluded_keys, TRUE)) {
      $merged = array_merge($merged, $process ? profiler_config_reduce($value) : $value);
    }
  }
  return $process ? array_unique($merged) : $merged;
}

/**
 * Returns user 1 for a config, if it exists.
 */
function profiler_config_load_superuser($config) {
  if (isset($config['users'])) {
    foreach ($config['users'] as $account) {
      if ($account['uid'] == 1) {
        return $account;
      }
    }
  }
  return FALSE;
}

/**
 * Install components.
 */
function profiler_install_components($config) {
  if (is_array($config)) {
    $components = profiler_components();
    $identifiers = array();
    foreach ($config as $name => $data) {
      if (isset($components[$name], $components[$name]['callback'])) {
        if (isset($components[$name]['file'])) {
          require_once($components[$name]['file']);
        }
        if (function_exists($components[$name]['callback']) && !empty($data)) {
          $components[$name]['callback']($data, $config, $identifiers);
        }
      }
    }
  }
}

/**
 * This function mimics a lot of the functionality of install_configure_form_submit() inside install.php
 */
function profiler_install_configure($config) {
  if ($array = profiler_config_load_superuser($config)) {
    // We need to mimic being user 1 in order to bypass administeruserbyrole.
    global $user;
    $user->uid = 1;

    // Save user 1
    profiler_install_users(array($array));
    user_authenticate($array);

    // The user is now logged in, but has no session ID yet, which
    // would be required later in the request, so remember it.
    $user->sid = session_id();

    // Force clean URLs... (why?)
    variable_set('clean_url', TRUE);

    // Record when this install ran.
    variable_set('install_time', time());
  }
}

/**
 * Invoke module hooks to retrieve component info.
 */
function profiler_components() {
  $components = module_invoke_all('profiler_components');
  drupal_alter('profiler_components', $components);
  uasort($components, 'profiler_components_sort');
  return $components;
}

/**
 * Sort callback for ordering components.
 */
function profiler_components_sort($a, $b) {
  $a = (array)$a + array('weight' => 0);
  $b = (array)$b + array('weight' => 0);
  return $a['weight'] < $b['weight'] ? -1 : 1;
}

/**
 * Implementation of hook_profiler_components() on behalf of system.
 */
function system_profiler_components() {
  return array(
    'theme' => array('callback' => 'profiler_install_theme'),
    'variables' => array('callback' => 'profiler_install_variables'),
  );
}

/**
 * Implementation of hook_profiler_components() on behalf of node.
 */
function node_profiler_components() {
  return array(
    'nodes' => array(
      'callback' => 'profiler_install_nodes',
      'weight' => 10,
    ),
  );
}

/**
 * Implementation of hook_profiler_components() on behalf of taxonomy.
 */
function taxonomy_profiler_components() {
  return array(
    'terms' => array(
      'callback' => 'profiler_install_terms',
      'weight' => 10,
    ),
  );
}

/**
 * Implementation of hook_profiler_components() on behalf of user.
 */
function user_profiler_components() {
  return array(
    'users' => array(
      'callback' => 'profiler_install_users',
      'weight' => -10,
    ),
  );
}

/**
 * Component install callback for 'theme'.
 */
function profiler_install_theme($key, $config, &$identifiers) {
  system_theme_data();
  variable_set('theme_default', $key);
  list_themes(TRUE);
  system_initialize_theme_blocks($key);
  db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name = '%s'", $key);
  list_themes(TRUE);
  menu_rebuild();
  drupal_rebuild_theme_registry();
}

/**
 * Component install callback for 'variables'.
 */
function profiler_install_variables($vars, $config, &$identifiers) {
  foreach($vars as $key => $value) {
    variable_set($key, $value);
  }
}

/**
 * Component install callback for 'nodes'.
 */
function profiler_install_nodes($nodes, $config, &$identifiers) {
  foreach (array_filter($nodes) as $nid => $properties) {
    if (isset($properties['name'])) {
      $account = user_load(array('name' => $properties['name']));
      if (!$account) {
        drupal_set_message(t('Failed to load Drupal user %user -- node %title not saved.', array('%name' => $properties['name'], '%title' => $properties['title'])), 'error', FALSE);
        continue;
      }
    }
    $default = array(
      'nid' => NULL,
      'title' => '',
      'body' => '',
      'type' => 'page',
      'teaser' => '',
      'log' => '',
      'created' => '',
      'format' => FILTER_FORMAT_DEFAULT,
      'uid' => !empty($account) ? $account->uid : 0,
    );
    $node = (object) array_merge($default, $properties);
    node_save($node);
  }
}

/**
 * Component install callback for 'terms'.
 */
function profiler_install_terms($terms, $config, &$identifiers) {
  static $vocabs, $vocabs_by_module;
  if (!isset($vocabs)) {
    $vocabs = taxonomy_get_vocabularies();
    foreach ($vocabs as $vid => $vocab) {
      $vocabs_by_module[$vocab->module][$vid] = $vocab;
    }
  }
  $weight = 0;
  foreach ($terms as $term) {
    // Support the 'vocab_module' key for referring to a term's vocabulary by
    // its module.
    if (isset($term['vocab_module'], $vocabs_by_module[$term['vocab_module']]) && $vid = key($vocabs_by_module[$term['vocab_module']])) {
      $term['vid'] = $vid;
    }
    // Sanity checks before creating.
    if (!empty($term['name']) && !empty($term['vid']) && isset($vocabs[$term['vid']])) {
      $term['weight'] = empty($term['weight']) ? $weight++ : $term['weight'];
      taxonomy_save_term($term);
    }
  }
}

/**
 * Component install callback for 'users'.
 */
function profiler_install_users($users) {
  foreach (array_filter($users) as $array) {
    // For some reason db_last_insert_id() doesn't get the uid of the newly created
    // user, so we need to separate out roles first, create the account, then save
    // again to properly assign roles to this user.
    $roles = isset($array['roles']) ? $array['roles'] : array();
    unset($array['roles']);

    // Use random password.
    $array['pass'] = user_password();

    // Load and update the account or create a new one.
    $account = isset($array['uid']) ? user_load($array['uid']) : user_load(array('name' => $array['name']));
    $account = $account ? $account : new stdClass();
    user_save($account, $array);

    // Load the saved account.
    $account = user_load(array('name' => $array['name']));

    // Set the roles.
    $formatted = array();
    $roles = explode(',', $roles);
    foreach ($roles as $name) {
      if ($rid = db_result(db_query("SELECT rid FROM {role} WHERE name = '%s'", trim($name)))) {
        $formatted[$rid] = TRUE;
      }
    }
    user_save($account, array('roles' => $formatted));
  }
}
