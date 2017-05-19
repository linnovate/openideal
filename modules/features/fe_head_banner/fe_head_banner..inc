<?php
/**
 * @file
 * fe_head_banner..inc
 */

/**
 * Implements hook_default_block_machine_name_boxes().
 */
function fe_head_banner_default_block_machine_name_boxes() {
  $export = array();

  $block_machine_name_boxes = new stdClass();
  $block_machine_name_boxes->machine_name = 'banner_header_confucius';
  $export['banner_header_confucius'] = $block_machine_name_boxes;

  return $export;
}
