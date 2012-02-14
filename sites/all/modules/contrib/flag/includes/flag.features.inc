<?php
// $Id: flag.features.inc,v 1.1.2.3 2010/01/08 04:59:06 quicksketch Exp $

/**
 * @file
 * Features integration for Flag module.
 */

/**
 * Implementation of hook_features_export().
 */
function flag_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  // Add flag module as a dependency.
  $export['dependencies']['features'] = 'flag';

  // Ensure the modules that provide the flag are included as dependencies.
  $modules = flag_features_providing_module();
  foreach ($data as $key => $flag) {
    // Get the module that provided the flag definition.
    if ($flag = flag_get_flag($flag)) {
      $module = $modules[$flag->content_type];
      $export['dependencies'][$module] = $module;
      $export['features']['flag'][$flag->name] = $flag->name;
    }
  }

  return $pipe;
}

/**
 * Implementation of hook_features_export_options().
 */
function flag_features_export_options() {
  $options = array();
  // Get all flags.
  foreach (flag_get_flags() as $name => $flag) {
    $options[$name] = ucfirst(check_plain($flag->content_type)) .': '. check_plain($flag->title);
  }
  return $options;
}

/**
 * Implementation of hook_features_export_render().
 */
function flag_features_export_render($module = 'foo', $data, $features_export) {
  module_load_include('inc', 'flag', '/includes/flag.export');
  $code = flag_export_flags($data, $features_export['project']);
  return array('flag_default_flags' => $code);
}

/**
 * Implementation of hook_features_revert().
 *
 * @param $module
 *   The name of module for which to revert content.
 */
function flag_features_revert($module = NULL) {
  // Get default flags from features.
  if (module_hook($module, 'flag_default_flags')) {
    module_load_include('inc', 'flag', '/includes/flag.admin');
    $default_flags = module_invoke($module, 'flag_default_flags');

    // Delete flags that are defined in code.
    foreach ($default_flags as $default_flag) {
      $current_flag = flag_get_flag($default_flag['name']);
      $current_flag->delete();
    }
    _flag_clear_cache();
  }
  else {
    drupal_set_message(t('Could not load default flag.'), 'error');
    return FALSE;
  }
  return TRUE;
}

/**
 * Helper function; Retrieve the providing modules defining the flags.
 */
function flag_features_providing_module() {
  $modules = array();
  $hook = 'flag_definitions';
  foreach (module_implements($hook) as $module) {
    foreach (module_invoke($module, $hook) as $key => $value) {
      $modules[$key] = $module;
    }
  }
  return $modules;
}
