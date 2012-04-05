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
  // Exported flag: "Bronze User".
  $flags['bronze_user'] = array(
    'content_type' => 'user',
    'title' => 'Bronze User',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'flag as Bronze user',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as Bronze user',
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
  // Exported flag: "Gold User".
  $flags['gold_user'] = array(
    'content_type' => 'user',
    'title' => 'Gold User',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'flag as Gold user',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as Gold user',
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
  // Exported flag: "Silver User".
  $flags['silver_user'] = array(
    'content_type' => 'user',
    'title' => 'Silver User',
    'global' => '1',
    'types' => array(),
    'flag_short' => 'flag as Silver user',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as Silver user',
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

/**
 * Implements hook_image_default_styles().
 */
function fe_user_image_default_styles() {
  $styles = array();

  // Exported image style: aventar_45_45
  $styles['aventar_45_45'] = array(
    'name' => 'aventar_45_45',
    'effects' => array(
      1 => array(
        'label' => 'Scale and crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => '45',
          'height' => '45',
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}
