<?php
/**
 * @file
 * fe_user.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_user_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function fe_user_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}

/**
 * Implements hook_flag_default_flags().
 */
function fe_user_flag_default_flags() {
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
    'module' => 'fe_user',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  return $flags;
}
