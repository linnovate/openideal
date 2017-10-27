<?php
/**
 * @file
 * fe_idea.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_idea_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "quicktabs" && $api == "quicktabs") {
    return array("version" => "1");
  }
  if ($module == "radioactivity" && $api == "radioactivity_decay_profile") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function fe_idea_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_flag_default_flags().
 */
function fe_idea_flag_default_flags() {
  $flags = array();
  // Exported flag: "Blast Idea".
  $flags['blast_idea'] = array(
    'entity_type' => 'node',
    'title' => 'Blast Idea',
    'global' => 1,
    'types' => array(
      0 => 'idea',
    ),
    'flag_short' => 'flag as blast idea',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as blast idea',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'teaser' => TRUE,
      'full' => TRUE,
    ),
    'show_as_field' => FALSE,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'module' => 'fe_idea',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Buzz Idea".
  $flags['buzz_idea'] = array(
    'entity_type' => 'node',
    'title' => 'Buzz Idea',
    'global' => 1,
    'types' => array(
      0 => 'idea',
    ),
    'flag_short' => 'flag as buzz idea',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as buzz idea',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'teaser' => TRUE,
      'full' => TRUE,
    ),
    'show_as_field' => FALSE,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'module' => 'fe_idea',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  // Exported flag: "Groove Idea".
  $flags['groove_idea'] = array(
    'entity_type' => 'node',
    'title' => 'Groove Idea',
    'global' => 1,
    'types' => array(
      0 => 'idea',
    ),
    'flag_short' => 'flag as groove idea',
    'flag_long' => '',
    'flag_message' => '',
    'unflag_short' => 'unflag as groove idea',
    'unflag_long' => '',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'toggle',
    'weight' => 0,
    'show_in_links' => array(
      'teaser' => TRUE,
      'full' => TRUE,
    ),
    'show_as_field' => FALSE,
    'show_on_form' => 0,
    'access_author' => '',
    'show_contextual_link' => FALSE,
    'i18n' => 0,
    'module' => 'fe_idea',
    'locked' => array(
      0 => 'name',
    ),
    'api_version' => 3,
  );
  return $flags;

}

/**
 * Implements hook_image_default_styles().
 */
function fe_idea_image_default_styles() {
  $styles = array();

  // Exported image style: scale_cropped.
  $styles['scale_cropped'] = array(
    'label' => 'Scale&Cropped 220*220',
    'effects' => array(
      1 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 220,
          'height' => 220,
        ),
        'weight' => 1,
      ),
    ),
  );

  return $styles;
}

/**
 * Implements hook_node_info().
 */
function fe_idea_node_info() {
  $items = array(
    'idea' => array(
      'name' => t('Idea'),
      'base' => 'node_content',
      'description' => t('Idea content type'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
