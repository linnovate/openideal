<?php
/**
 * @file
 * fe_head_banner.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_head_banner_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "block_machine_name" && $api == "") {
    return array("version" => "");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_node_info().
 */
function fe_head_banner_node_info() {
  $items = array(
    'header_site' => array(
      'name' => t('header site'),
      'base' => 'node_content',
      'description' => t('Content type for the header banner'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
  );
  drupal_alter('node_info', $items);
  return $items;
}
