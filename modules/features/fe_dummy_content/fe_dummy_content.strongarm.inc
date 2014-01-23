<?php
/**
 * @file
 * fe_dummy_content.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fe_dummy_content_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'nodequeue_links';
  $strongarm->value = TRUE;
  $export['nodequeue_links'] = $strongarm;

  return $export;
}
