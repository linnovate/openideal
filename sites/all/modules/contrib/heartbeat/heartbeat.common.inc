<?php
// $Id: heartbeat.common.inc,v 1.1.2.2.2.23 2010/10/16 09:16:35 stalski Exp $

/**
 * @file
 *   Commonly functions used in heartbeat
 */

/**
 * Include heartbeat .inc class files
 */
function heartbeat_include($file, $module = 'heartbeat', $_file_path = '') {

  static $loaded = array();
  $file = drupal_strtolower($file);
  $path = drupal_get_path('module', $module);
  $_file_path = empty($_file_path) ? $file . '.inc' : $_file_path;

  if (!isset($loaded[$file])) {

    if ($file == 'heartbeatmessagebuilder' && variable_get('heartbeat_debug', 0)) {
      require $path . '/includes/'. $file . 'debug.inc';
      $loaded[$file] = TRUE;
      return $loaded[$file];
    }

    $file_path = $path . '/includes/'. $_file_path;
    if (is_file($file_path)) {
      require $file_path;
      $loaded[$file] = TRUE;
      return $loaded[$file];
    }

    $file_path = $path . '/'. $_file_path;
    if (is_file($file_path)) {
      require $file_path;
      $loaded[$file] = TRUE;
    }
  }

  return $loaded[$file];
}

/**
 * Decode heartbeat message variables
 */
function heartbeat_decode_message_variables($string, $object = FALSE) {

  if (!is_string($string)) {
    return array();
  }

  // Variable string need to be cleared from spaces to decode properly
  $array = explode("-|-", $string);
  $variables = array();
  if (!empty($array)) {
    foreach ($array as $varvalue) {
      $parts = explode("=|=", $varvalue);
      if (isset($parts[0]) && !empty($parts[0])) {
        if (eregi("\*\*\*", $parts[1])) {
          $parts[1] = explode("***", $parts[1]);
        }
        $variables[$parts[0]] = !is_array($parts[1]) ? (string)$parts[1] : $parts[1];
      }
    }
  }

  return $object ? (object) $variables : (array) $variables;
}

/**
 * Encode heartbeat message variables
 */
function heartbeat_encode_message_variables($array) {
  $string = '';
  foreach ($array as $key => $value) {
    if (is_array($value)) {
      $value = implode('***', $value);
    }
    $string .= $key .'=|='. $value .'-|-';
  }
  //$string = serialize((object)$array);
  return $string;
}

/**
 * Helper function to map a array to dropdown
 *   with a field and value for the options
 *
 * @param array $options
 * @param string target $field
 * @param sring target $value
 * @return array mapped for options dropdown
 */
function _heartbeat_map_assoc($options, $field, $value) {
  $mapped = array();
  foreach ($options as $heartbeat_activity) {
    $mapped[$heartbeat_activity->{$field}] = $heartbeat_activity->{$value};
  }
  return $mapped;
}

/**
 * Helper function to get the fields
 */
function _heartbeat_get_fields($table) {
  $fields = array();
  $schema = drupal_get_schema($table, TRUE);
  // Go through our schema
  foreach ($schema['fields'] as $field => $info) {
    $fields[$field] = !empty($info['alias']) ? $info['alias'] : $field;
  }
  return $fields;
}

/**
 * Helper function to check if a user has access to edit/delete buttons
 */
function _heartbeat_activity_stream_delete_access() {
  if (variable_get('heartbeat_activity_stream_delete', 1)) {
    return user_access('delete heartbeat activity logs');
  }
  return FALSE;
}

/**
 * Helper function to check if a user has access to delete a message
 */
function _heartbeat_message_delete_access($uaid) {
  if (_heartbeat_activity_stream_delete_access()) {
    return TRUE;
  }

  if (variable_get('heartbeat_activity_stream_actor_delete', 1)) {
    global $user;
    $uid = db_result(db_query("SELECT uid FROM {heartbeat_activity} WHERE uaid = %d", $uaid));
    return $uid == $user->uid;
  }
  return FALSE;
}

/**
 * Helper function to get the options for access types
 *
 * @return array of access types
 */
function _heartbeat_access_type_options() {
  $return = array();
  $types = variable_get('heartbeat_access_types', array());
  foreach ($types as $type) {
    $return[$type['class']] = $type['name'];
  }
  return $return;
}

/**
 * Function to get the allowed tags for heartbeat strings
 */
function heartbeat_allowed_tags() {
  $tags = str_replace(' ', '', variable_get('heartbeat_allowed_tags', ''));
  return explode(',', $tags);
}

/**
 * Helper function to get the stream default properties
 */
function _heartbeat_stream_defaults($values) {
  $default = array(
    'name' => '',
    'class' => '',
    'path' => '',
    'module' => '',
    'access' => 1,
    'status' => 'default',
    'messages_denied' => array(),
    'display_filters' => 0,
    'display_block_filters' => 0,
    'filters' => array(),
    'filters_cumul' => 0,
    'settings' => array(
      'block_items_max' => 10,
      'block_show_pager' => 2,
      'page_items_max' => 15,
      'page_show_pager' => 1,
      'page_pager_ajax' => 1,
      'num_load_max' => 30,
      'page_disabled' => 0
    ),
    'poll_messages' => 0,
  );

  // if same string keys exist, the used configuration will
  // override the default one
  $update = array_merge($default, $values);

  return $update;
}

/**
 * Helper function for a more link on streams (older messages)
 */
function heartbeat_stream_prev_link($access_type, $offset_time = 0) {
  $path = 'heartbeat/'. $access_type . ($offset_time == 0 ? '' : '/' . $offset_time);
  return '<div class="more previous-archive">' . l(t('Go to previous messages'), $path) . '</div>';
}

/**
 *
 * @return unknown_type
 */
function _theme_time_ago($time) {
  return t('@time ago', array('@time' => format_interval(($_SERVER['REQUEST_TIME'] - $time), 1)));
}

/**
 * Function to get the corresponding tuaid
 * @param Integer $uaid
 * @return Integer $tuaid
 */
function heartbeat_get_tuaid($uaid) {
  $result = db_fetch_object(db_query("SELECT tuaid FROM {heartbeat_translations} WHERE uaid=%d", $uaid));
  return $result ? $result->tuaid : $uaid;
}

/**
 * Function to load users within heartbeat cached system.
 *
 * @param Integer $uid
 *   The ID of the user
 * @return Object $user
 *   A full user object
 */
function heartbeat_user_load($uid) {
  static $users;
  if (!isset($users[$uid])) {
    $users[$uid] = user_load($uid);
  }

  return $users[$uid];
}
