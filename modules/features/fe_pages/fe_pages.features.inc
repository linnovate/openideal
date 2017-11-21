<?php
/**
 * @file
 * fe_pages.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_pages_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  if ($module == "panels" && $api == "layouts") {
    return array("version" => "1");
  }
  if ($module == "panels_mini" && $api == "panels_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function fe_pages_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function fe_pages_image_default_styles() {
  $styles = array();

  // Exported image style: cropped_220_220.
  $styles['cropped_220_220'] = array(
    'label' => 'Cropped 220*220',
    'effects' => array(
      1 => array(
        'name' => 'image_crop',
        'data' => array(
          'width' => 220,
          'height' => 220,
          'anchor' => 'center-center',
        ),
        'weight' => 1,
      ),
    ),
  );

  // Exported image style: scale_cropped_220_220.
  $styles['scale_cropped_220_220'] = array(
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
