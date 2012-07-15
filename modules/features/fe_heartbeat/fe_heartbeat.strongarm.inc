<?php
/**
 * @file
 * fe_heartbeat.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fe_heartbeat_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_profile_message_templates';
  $strongarm->value = array(
    'oi_add_comment' => 'oi_add_comment',
    'oi_add_node' => 'oi_add_node',
    'oi_vote_on_node' => 'oi_vote_on_node',
    'oi_comment_on_my_content' => 'oi_comment_on_my_content',
    'oi_vote_on_my_content' => 'oi_vote_on_my_content',
  );
  $export['heartbeat_profile_message_templates'] = $strongarm;

  return $export;
}
