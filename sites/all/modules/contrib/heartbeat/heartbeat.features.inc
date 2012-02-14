<?php
// $Id: heartbeat.features.inc,v 1.1.2.2 2010/08/16 20:39:52 stalski Exp $

/**
 * @file
 * Features support.
 */

/**
 * Return API information for features.
 */
function _heartbeat_features_api() {
  return array(
    'heartbeat' => array(
      'name' => t('Heartbeat'),
      'default_hook' => 'heartbeat_message_info',
      'default_file' => FEATURES_DEFAULTS_INCLUDED,
      'features_source' => TRUE,
      'file' => drupal_get_path('module', 'heartbeat') .'/heartbeat.features.inc',
    ),
  );
}

/**
 * Implementation of hook_features_export_options().
 */
function heartbeat_features_export_options() {
  return _heartbeat_features_get_types();
}

/**
 * Get types for features.
 */
function _heartbeat_features_get_types() {
  $types = array();
  $templates = heartbeat_messages('all', TRUE);
  foreach ($templates as $template) {
    $types[$template->message_id] = empty($template->description) ? $template->message_id : $template->description;
  }
  return $types;
}

/**
 * Implementation of hook_features_export().
 */
function heartbeat_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $export['dependencies']['heartbeat'] = 'heartbeat';
  foreach ($data as $module) {
    $export['features']['heartbeat'][$module] = $module;
  }

  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 */
function heartbeat_features_export_render($module = 'foo', $data) {
  $code = array();
  $info = array();

  // Begin code
  $code[] = '  $messages = array();' . "\n";

  // Gather all messages
  $messages = heartbeat_messages('all', TRUE, FALSE);

  foreach ($messages as $message) {
    $message = (object) $message;
    if (!array_search($message->message_id, $data)) {
      continue; // Leave if not selected
    }

    // Label the current message
    $code[] = '  // Exported heartbeat message: ' . $message->message_id;

    // Build message object
    $concat_args = heartbeat_decode_message_variables($message->concat_args);
    $variables = heartbeat_decode_message_variables($message->variables);
    $message = array(
      'message' => $message->message,
      'message_concat' => $message->message_concat,
      'message_id' => $message->message_id,
      'concat_args' => $concat_args,
      'description' => $message->description,
      'perms' => $message->perms,
      'custom' => HEARTBEAT_MESSAGE_DEFAULT,
      'variables' => $variables
    );

    // Add to code
    $code[] = '  $messages[] = ' . str_replace("\n", "\n  ", var_export($message, 1)) . ";\n";
  }

  // End code
  $code[] = "\n" . '  return $messages;';

  // Put code into a string
  $code = implode($code, "\n");

  return array('heartbeat_message_info' => $code);
}

/**
 * Implementation of hook_features_revert().
 */
function heartbeat_features_revert($module = NULL) {
  
  // Load heartbeat.admin.inc
  require_once(drupal_get_path('module', 'heartbeat') . '/heartbeat.admin.inc');
  
  // Get default heartbeats from feature
  if (module_hook($module, 'heartbeat_message_info')) {
    $heartbeats = module_invoke($module, 'heartbeat_message_info');

    foreach ($heartbeats as $heartbeat) {
      heartbeat_messages_revert($heartbeat['message_id']);
    }
  }
  
}
