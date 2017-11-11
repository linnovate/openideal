<?php
/**
 * @file
 * fe_pages.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_pages_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "panels" && $api == "layouts") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "panels_mini" && $api == "panels_default") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function fe_pages_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_image_default_styles().
 */
function fe_pages_image_default_styles() {
  $styles = array();

  // Exported image style: sacle_and_crop_220_220.
  $styles['sacle_and_crop_220_220'] = array(
    'name' => 'image_scale_and_crop',
    'label' => 'Scale and crop 220*220',
    'effects' => array(
      1 => array(
        'label' => 'Crop',
        'help' => 'Scale and crop will maintain the aspect-ratio of the original image, then crop the larger dimension. This is most useful for creating perfectly square thumbnails without stretching the image.',
        'effect callback' => 'image_scale_and_crop_effect',
        'dimensions callback' => 'image_resize_dimensions',
        'form callback' => 'image_resize_form',
        'summary theme' => 'image_resize_summary',
        'module' => 'image',
        'name' => 'image_crop',
        'data' => array(
          'width' => '220',
          'height' => '220',
          'anchor' => 'center-center',
        ),
        'weight' => '1',
      ),
    ),
  );

  return $styles;
}
