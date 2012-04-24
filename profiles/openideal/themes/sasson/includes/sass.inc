<?php

/**
 * @file
 * Handles compiling of .sass / .scss files.
 */

/**
 *  Helper function for sass integration. 
 */
function sasson_sass_render() {
  $element = array(
    '#items' => drupal_add_css(), 
  );
  sasson_pre_render($element);
}

/**
 * Implementation of hook_element_info_alter().
 */
function sasson_element_info_alter(&$type) {
  array_unshift($type['styles']['#pre_render'], 'sasson_pre_render');
  // Show development messages once an hour.
  if (theme_get_setting('sasson_clear_registry') && user_access('administer site configuration') && flood_is_allowed('sasson_registry_warning', 1)) {
    flood_register_event('sasson_registry_warning');
    drupal_set_message(t('Your theme registry is being rebuilt on every page request. Don\'t forget to <a href="!link">disable this feature</a> before going public.', array('!link' => url('admin/appearance/settings/' . variable_get('theme_default','none')))), 'warning');
  }
  if (theme_get_setting('sasson_devel') && user_access('administer site configuration') && flood_is_allowed('sasson_devel_warning', 1)) {
    flood_register_event('sasson_devel_warning');
    drupal_set_message(t('Your SASS / SCSS files are being recompiled on every page request. Don\'t forget to <a href="!link">disable this feature</a> before going public.', array('!link' => url('admin/appearance/settings/' . variable_get('theme_default','none')))), 'warning');
  }
  if (theme_get_setting('sasson_devel') && variable_get('preprocess_css') && user_access('administer site configuration')) {
    drupal_set_message(t('Your SASS / SCSS files are being recompiled on every page request while your CSS compression is turned on. This makes no sense. You should <a href="!link">disable this feature</a>.', array('!link' => url('admin/appearance/settings/' . variable_get('theme_default','none')))), 'error');
  }
}

/**
 * Builds the SASS cache. Should only be invoked by drupal_render().
 *
 * @param $elements
 *   A render array containing:
 *   '#items': The CSS items as returned by drupal_add_css() and altered by
 *   drupal_get_css().
 *   '#group_callback': A function to call to group #items to enable the use of
 *   fewer tags by aggregating files and/or using multiple @import statements
 *   within a single tag.
 *   '#aggregate_callback': A function to call to aggregate the items within the
 *   groups arranged by the #group_callback function.
 *
 * @return $elements
 *   The modified (pre-rendered) $elements parameter.
 */
function sasson_pre_render($elements) {
  $map = $original = variable_get('sasson_cache', array());
  $files = sasson_pick_files($elements['#items']);
  $devel = theme_get_setting('sasson_devel');
  // We can bail out here if there are no SCSS files anyways.
  if (function_exists('drupal_get_path')) {
    $inc = DRUPAL_ROOT . '/' . drupal_get_path('theme', 'sasson') . "/phamlp/sass/SassParser.php";
    if (is_file($inc)) {
      require_once $inc;
    }
    else {
      return;
    }
  }
  if (empty($files)) {
    // Remove the files from the array of stylesheets.
    $elements['#items'] = array_diff_key($elements['#items'], $files);
    return $elements;
  }
  foreach ($files as $key => $file) {
    // Create a unique identifier for the file.
    if ($file['recompile'] !== TRUE) {
      $hash = hash('sha256', serialize($file));
      $path = isset($map[$hash]) ? $map[$hash] : NULL;
    }
    $parts = explode("/", $elements['#items'][$key]['data']);
    $last = end($parts);
    $parts = explode(".", $last);
    $name = reset($parts);
    $directory = 'public://css';
    $path = $directory . '/' . $name . '.css';
    
    // We recompile this file if the file doesn't exist, if recompile equals true or if the file have changed since it was last compiled AND if we are in development mode.
    if (!file_exists($path) || $file['recompile'] || filemtime($path) < filemtime($file['data']) && $devel) {
      $data = sasson_load_stylesheet($file['data']);
      if (!$output = sasson_parse($data, $file, $file['syntax'])) {
        unset($elements['#items'][$key]);
        continue;
      }
      // Create the CSS file.
      file_prepare_directory($directory, FILE_CREATE_DIRECTORY);
      if (!file_unmanaged_save_data($output, $path, FILE_EXISTS_REPLACE)) {
        unset($elements['#items'][$key]);
        continue;
      }
    }
    // Update the item in the stylesheets array.
    $elements['#items'][$key] = $file;
    $elements['#items'][$key]['data'] = $path;
    if ($file['recompile'] !== TRUE) {
      // Don't cache this item if it is set to recompile on every page load.
      $map[$hash] = $path;
    }
  }
  // If $map and $original don't match anymore that means we need to update the
  // CSS cache.
  if ($original !== $map) {
    variable_set('sasson_cache', $map);
  }
  
  return $elements;
}

/**
 * Picks all SCSS and SASS files from an array of stylesheets.
 *
 * @param $items
 *   An array of stylesheets.
 *
 * @return
 *   The extracted files as an array.
 */
function sasson_pick_files($items) {
  $files = array();
  foreach ($items as $key => $file) {
    if ($file['type'] == 'file') {
      $extension = drupal_substr($file['data'], -5);
      if (in_array($extension, array('.scss', '.sass'))) {
        $file['syntax'] = $extension == '.scss' ? 'scss' : 'sass';
        $file['recompile'] = isset($file['recompile']) ? $file['recompile'] : FALSE;
        // If the file is set to recompile on every page load then we don't want
        // it to be aggregated.
        $file['preprocess'] = !empty($file['recompile']) ? FALSE : $file['preprocess'];
        $files[$key] = $file;
      }
    }
  }
  return $files;
}

/**
 * Loads a stylesheet and writes the base path to all url declarations.
 *
 * @param $file
 *   A filepath or an array representing a stylesheet.
 *
 * @return
 *   A string that represents the processed contents of the stylesheet.
 */
function sasson_load_stylesheet($file) {
  $file = is_array($file) ? $file['data'] : $file;
  //$data = drupal_load_stylesheet($file); // this removes @import statements
  $data =  @file_get_contents($file);
  // Build the base URL of this CSS file. Start with the full URL.
  $base = file_create_url($file);
  // Move to the parent.
  $base = substr($base, 0, strrpos($base, '/'));
  // Simplify to a relative URL if the stylesheet URL starts with the base URL
  // of the website.
  if (substr($base, 0, strlen($GLOBALS['base_root'])) == $GLOBALS['base_root']) {
    $base = substr($base, strlen($GLOBALS['base_root']));
  }
  _drupal_build_css_path(NULL, $base . '/');
  // Anchor all paths in the CSS with its base URL, ignoring external and
  // absolute paths.
  $data = preg_replace_callback('/url\(\s*[\'"]?(?![a-z]+:|\/+)([^\'")]+)[\'"]?\s*\)/i', '_drupal_build_css_path', $data);
  $data = preg_replace("/url\(([^'\")]+)\)/i", "url('$1')", $data);
  return $data;
}

/**
 * Parse a SCSS string and transform it into CSS.
 *
 * @params $data
 *   A SCSS string.
 * @param $file
 *   The SASS or SCSS file that $data belongs to described by an array.
 * @param $syntax
 *   The syntax (SASS or SCSS) of the file contents. This information is needed
 *   by the parser.
 *
 * @return
 *   The transformed CSS as a string.
 */
function sasson_parse($data, $file, $syntax) {
  try {
    $data = sasson_get_libraries($syntax) . "\n\n" . $data;
    // Quote all url(..) to ensure PhamlP doesn't strip them.
    $data = preg_replace("/url\(([^'\")]+)\)/i", "url('$1')", $data);
    // Are we in development mode ?
    $devel = theme_get_setting('sasson_devel');
    $style = $devel ? 'expanded' : 'compressed';
    // Watch the theme dynasty for partials
    $dynasty = sasson_theme_dynasty();
    $paths = array();
    foreach ($dynasty as $theme) {
      $dir = drupal_get_path('theme', $theme->name) . '/styles/';
      if (is_dir($dir)) {
        $paths[] = $dir;
      }
    }
    // Prepare vendor prefixes array
    // @see hook_prefixes_alter().
    $pref = array();
    drupal_alter('prefixes', $pref);

    // Execute the compiler.
    $options = array(
      'style' => $style,
      'cache' => FALSE,
      'syntax' => $syntax,
      'filename' => $file['data'],  
      'load_paths' => $paths, // fix by http://code.google.com/p/phamlp/issues/detail?id=108
      'debug_info' => $devel,
      'vendor_properties'=> $pref,
      'extensions' => array('compass' => array()),
    );
    $parser = new SassParser($options);
    
    // Alter SASS just before it is sent to the parser
    // @see hook_sasson_alter().
    drupal_alter('sasson', $data);
    
    // parsing the data content of the file.
    $data = $parser->toCss($data, FALSE); 
    
    // Alter SASS right after it comes back from the parser
    // @see hook_sasson_post_parse().
    drupal_alter('sasson_post_parse', $data);

    return $data;
  }
  catch (Exception $e) {
    watchdog_exception('sasson', $e);
    if (user_access('administer site configuration')) {
      drupal_set_message(t('An error occured while processing !stylesheet. Please consult your !watchdog for a detailed error description.', array('!stylesheet' => l(basename($file['data']), $file['data']), '!watchdog' => l('log messages', 'admin/reports/dblog'))), 'error');
    }
  }
}

/**
 * Fetches, caches and returns all SASS / SCSS libraries from all enabled
 * modules and the theme trail.
 *
 * @param $syntax
 *   The syntax (SASS or SCSS) of the file contents. This information is needed
 *   by the parser.
 *
 * @return
 *   A string representing the cummulated content of all library files for
 *   the given syntax.
 */
function sasson_get_libraries($syntax) {
  $libraries = &drupal_static(__FUNCTION__);
  if (!isset($libraries)) {
    if (!theme_get_setting('sasson_devel') && $cache = cache_get('sasson_libraries:' . $GLOBALS['theme_key'] . '')) {
      $libraries = $cache->data;
    }
    else {
      $libraries = array('sass' => array(), 'scss' => array());
      // Load libraries from all enabled modules.
      foreach (system_get_info('module') as $module => $info) {
        if (!empty($info['sasson'])) {
          foreach ($info['sasson'] as $library) {
            $extension = drupal_substr($library, -5);
            if (in_array($extension, array('.scss', '.sass'))) {
              $current = $extension == '.scss' ? 'scss' : 'sass';
              $path = drupal_get_path('module', $module) . '/' . $library;
              $libraries[$current][$path] = $path;
            }
          }
        }
      }
      // Load libraries from the theme trail.
      foreach (array_merge($GLOBALS['base_theme_info'], array($GLOBALS['theme_info'])) as $info) {
        if (!empty($info->info['sasson'])) {
          foreach ($info->info['sasson'] as $library) {
            $extension = drupal_substr($library, -5);
            if (in_array($extension, array('.scss', '.sass'))) {
              $current = $extension == '.scss' ? 'scss' : 'sass';
              $path = drupal_get_path('theme', $info->name) . '/' . $library;
              $libraries[$current][$path] = $path;
            }
          }
        }
      }
      drupal_alter('sasson_libraries_scss', $libraries['scss']);
      drupal_alter('sasson_libraries_sass', $libraries['sass']);
      $libraries['scss'] = implode("\n\n", array_map('sasson_load_stylesheet', $libraries['scss']));
      $libraries['sass'] = implode("\n\n", array_map('sasson_load_stylesheet', $libraries['sass']));
      cache_set('sasson_libraries:' . $GLOBALS['theme_key'], $libraries);
    }
  }
  if (isset($libraries[$syntax])) {
    return $libraries[$syntax];
  }
}
