<?php
/**
 * @file
 * fe_challenge.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_challenge_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function fe_challenge_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function fe_challenge_image_default_styles() {
  $styles = array();

  // Exported image style: challenge_hp__500x325_.
  $styles['challenge_hp__500x325_'] = array(
    'label' => 'Challenge HP (500X325)',
    'effects' => array(
      1 => array(
        'name' => 'image_resize',
        'data' => array(
          'width' => 498,
          'height' => 325,
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: singe_challenge.
  $styles['singe_challenge'] = array(
    'label' => 'singe_challenge',
    'effects' => array(
      9 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 1040,
          'height' => 321,
        ),
        'weight' => 2,
      ),
    ),
  );

  // Exported image style: singe_challenge_mobile.
  $styles['singe_challenge_mobile'] = array(
    'label' => 'singe challenge mobile',
    'effects' => array(
      2 => array(
        'name' => 'image_scale_and_crop',
        'data' => array(
          'width' => 334,
          'height' => 316,
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
function fe_challenge_node_info() {
  $items = array(
    'challenge' => array(
      'name' => t('Challenge'),
      'base' => 'node_content',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
