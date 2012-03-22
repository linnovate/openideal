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
  list($module, $api) = func_get_args();
  if ($module == "heartbeat" && $api == "heartbeat") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "quicktabs" && $api == "quicktabs") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
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
  // Exported flag: "A Class User".
  $flags['a_class_user'] = array(
    'content_type' => 'user',
    'title' => 'A Class User',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'flag as A class user',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as A class user',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '3',
        1 => '5',
      ),
      'unflag' => array(
        0 => '3',
        1 => '5',
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
  // Exported flag: "B Class User".
  $flags['b_class_user'] = array(
    'content_type' => 'user',
    'title' => 'B Class User',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'flag as B class user',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as B class user',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '3',
        1 => '5',
      ),
      'unflag' => array(
        0 => '3',
        1 => '5',
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
  // Exported flag: "C Class User".
  $flags['c_class_user'] = array(
    'content_type' => 'user',
    'title' => 'C Class User',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'flag as C class user',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as C class user',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => '3',
        1 => '5',
      ),
      'unflag' => array(
        0 => '3',
        1 => '5',
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
