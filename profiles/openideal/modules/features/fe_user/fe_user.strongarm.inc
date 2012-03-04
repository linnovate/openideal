<?php
/**
 * @file
 * fe_user.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fe_user_strongarm() {
  $export = array();

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'default_heartbeat_messages';
  $strongarm->value = array(
    'heartbeat_edit_node' => TRUE,
    'heartbeat_add_node' => TRUE,
    'heartbeat_add_comment' => TRUE,
    'heartbeat_edit_account' => TRUE,
  );
  $export['default_heartbeat_messages'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'default_heartbeat_streams';
  $strongarm->value = array(
    'relationsactivity' => FALSE,
  );
  $export['default_heartbeat_streams'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_activity_grouping_how_many';
  $strongarm->value = '5';
  $export['heartbeat_activity_grouping_how_many'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_activity_grouping_seconds';
  $strongarm->value = '7200';
  $export['heartbeat_activity_grouping_seconds'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_activity_log_cron_delete';
  $strongarm->value = '2678400';
  $export['heartbeat_activity_log_cron_delete'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_debug';
  $strongarm->value = 0;
  $export['heartbeat_debug'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_enabled';
  $strongarm->value = 0;
  $export['heartbeat_enabled'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_profile_message_templates';
  $strongarm->value = array(
    'heartbeat_add_node_oi' => 'heartbeat_add_node_oi',
  );
  $export['heartbeat_profile_message_templates'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_show_message_times';
  $strongarm->value = 1;
  $export['heartbeat_show_message_times'] = $strongarm;

  $strongarm = new stdClass;
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'heartbeat_show_time_grouped_items';
  $strongarm->value = 0;
  $export['heartbeat_show_time_grouped_items'] = $strongarm;

  return $export;
}
