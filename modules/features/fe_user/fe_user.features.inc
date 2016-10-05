<?php
/**
 * @file
 * fe_user.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_user_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "quicktabs" && $api == "quicktabs") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function fe_user_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
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
    'global' => 1,
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
        0 => 3,
        1 => 5,
      ),
      'unflag' => array(
        0 => 3,
        1 => 5,
      ),
    ),
    'weight' => 0,
    'show_on_entity' => FALSE,
    'show_on_form' => FALSE,
    'access_author' => '',
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
    'global' => 1,
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
        0 => 3,
        1 => 5,
      ),
      'unflag' => array(
        0 => 3,
        1 => 5,
      ),
    ),
    'weight' => 0,
    'show_on_entity' => FALSE,
    'show_on_form' => FALSE,
    'access_author' => '',
    'show_on_profile' => 1,
    'access_uid' => 'others',
    'module' => 'fe_user',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 2,
  );
  // Exported flag: "Has Launched Idea".
  $flags['has_launched_idea'] = array(
    'content_type' => 'user',
    'title' => 'Has Launched Idea',
    'global' => 1,
    'types' => array(),
    'flag_short' => 'flag as has launched idea',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as has launched idea',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'roles' => array(
      'flag' => array(
        0 => 3,
      ),
      'unflag' => array(
        0 => 3,
      ),
    ),
    'weight' => 0,
    'show_on_entity' => FALSE,
    'show_on_form' => FALSE,
    'access_author' => '',
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
    'global' => 1,
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
        0 => 3,
        1 => 5,
      ),
      'unflag' => array(
        0 => 3,
        1 => 5,
      ),
    ),
    'weight' => 0,
    'show_on_entity' => FALSE,
    'show_on_form' => FALSE,
    'access_author' => '',
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

  // Exported image style: avatar_45_45.
  $styles['avatar_45_45'] = array(
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 45,
          'height' => 45,
        ),
        'weight' => 1,
      ),
    ),
    'label' => 'avatar_45_45',
  );

  return $styles;
}
