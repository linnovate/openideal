<?php
/**
 * @file
 * fe_default.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_default_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function fe_default_flag_default_flags() {
  $flags = array();
  // Exported flag: "Extreme User".
  $flags['extreme_user'] = array(
    'content_type' => 'user',
    'title' => 'Extreme User',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'Flag as extreme user',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'Unflagas extreme user',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '3',
      ),
      'unflag' => array(
        0 => '3',
      ),
    ),
    'show_on_profile' => 1,
    'access_uid' => 'others',
    'api_version' => 2,
    'module' => 'fe_default',
    'locked' => array(
      0 => 'name',
    ),
  );
  return $flags;
}
