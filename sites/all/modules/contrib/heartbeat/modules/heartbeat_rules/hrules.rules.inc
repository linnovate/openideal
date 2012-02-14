<?php
// $Id: hrules.rules.inc,v 1.1.2.13 2010/09/26 21:03:17 stalski Exp $

/**
 * @file
 *  Heartbeat rules definitions.
 */

# DATA TYPES

/**
 * Rules heartbeat activity message data type.
 */
class rules_data_type_heartbeat_message extends rules_data_type {
  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value) {
    $info += array('long' => FALSE, 'required' => TRUE, 'description' => '');
    return array(
      '#type' => $info['long'] ? 'textarea' : 'textfield',
          '#title' => $info['label'],
          '#description' => $info['description'],
          '#required' => $info['required'],
          '#default_value' => $value,
    );
  }
}

/**
 * Rules heartbeat activity message data type.
 */
class rules_data_type_heartbeat_message_id extends rules_data_type_heartbeat_message {
  function check_value($info, $value) {
    return (float)$value;
  }
}

/**
 * Rules heartbeat activity message data type.
 */
class rules_data_type_heartbeat_access extends rules_data_type_heartbeat_message {
  function check_value($info, $value) {
    return (float)$value;
  }
}

/**
 * Implementation of hook_rules_data_type_info().
 */
function heartbeat_rules_data_type_info() {
  return array(
    'heartbeat_message' => array(
      'label' => t('logged heartbeat message in the most basic form'),
      'class' => 'rules_data_type_heartbeat_message',
      'identifiable' => FALSE,
      'eval input' => TRUE,
    ),
    'heartbeat_message_id' => array(
      'label' => t('heartbeat message id'),
      'class' => 'rules_data_type_heartbeat_message_id',
      'identifiable' => FALSE,
      'eval input' => TRUE,
    ),
    'heartbeat_access' => array(
      'label' => t('heartbeat message access'),
      'class' => 'rules_data_type_heartbeat_access',
      'identifiable' => FALSE,
      'eval input' => TRUE,
    ),
  );
}

# ACTIONS

/**
 * Implementation of hook_rules_action_info().
 * @ingroup rules
 */
function hrules_rules_action_info() {

  $info = array(

    // Base action to execute as default user activity logs
    'heartbeat_rules_default_action' => array(
      'label' => t('Logs default activity'),
      'eval input' => array(
        'uid_param',
        'uid_target_param',
        'nid_param',
        'nid_target_param',
        'message_id_param',
        'variables_param'
      ),
      'help' => 'Create a relation between a user and something else(node, user, comment, etc ...).',
      'module' => 'heartbeat',
    ),

    // Simplified action to execute user-to-user activity logs
    'heartbeat_rules_users_action' => array(
      'label' => t('Logs activity to indicate a relation between users'),
      'eval input' => array(
        'uid_param',
        'uid_target_param',
        'message_id_param',
        'variables_param'
      ),
      'help' => 'Create a relation between a user and another user. This is only helpfull for relationships like friends, collegues, ... but not when a user only addresses his content to another user.',
      'module' => 'heartbeat',
    ),
  );
  return $info;
}

/**
 * Action drupal message configuration form.
 */
function heartbeat_rules_default_action_form($settings, &$form, $form_state) {

  drupal_add_js(drupal_get_path('module', 'hrules') . '/hrules.js');

  // Add the default empty values to the settings hash
  $settings += array(
    'uid_param' => '',
    'uid_target_param' => '',
    'nid_param' => '',
    'nid_target_param' => '',
    'message_id_param' => '',
    'variables_param' => ''
  );

  // The parameters needed.
  $form['settings']['uid_param'] = array(
    '#type' => 'textfield',
    '#size' => '15',
    '#maxsize' => '25',
    '#title' => t('User Id'),
    '#default_value' => $settings['uid_param'],
    '#description' => t('The user doing the activity. Think well about who is the acting user for content related events. Is it the currently logged-in user or the author of the content.'),
    '#weight' => -5,
  );
  $form['settings']['uid_target_param'] = array(
    '#type' => 'textfield',
    '#size' => '15',
    '#maxsize' => '25',
    '#title' => t('User target Id'),
    '#default_value' => $settings['uid_target_param'],
    '#description' => t('The user target of the activity'),
    '#weight' => -4,
  );
  $form['settings']['nid_param'] = array(
    '#type' => 'textfield',
    '#size' => '15',
    '#maxsize' => '25',
    '#title' => t('Node context Id'),
    '#default_value' => $settings['nid_param'],
    '#description' => t('The node for activity context'),
    '#weight' => -3,
  );
  $form['settings']['nid_target_param'] = array(
    '#type' => 'textfield',
    '#size' => '15',
    '#maxsize' => '25',
    '#title' => t('Node target Id'),
    '#default_value' => $settings['nid_target_param'],
    '#description' => t('The node target of the activity. Only set one if the node is used! It is only handy to merge nodes within node targets, and thus separating the node target contexts (E.g.: Groups, references, ...'),
    '#weight' => -3,
  );

  // The message that needs to be linked to it
  $messages = heartbeat_messages('all', FALSE, FALSE);
  $options = array(0 => t('No message selected'));
  foreach ($messages as $message) {
    $options[$message['message_id']] = !empty($message['description']) ? $message['description'] : str_replace('_', ' ', $message['message_id']);
  }
  // Choose a message and addin ahah behavior to show variables
  $message_id = empty($settings['message_id_param']) ? 0 : $settings['message_id_param'];
  $form['settings']['message_id_param'] = array(
    '#type' => 'select',
    '#title' => t('Choose a message'),
    '#default_value' => $message_id,
    '#options' => $options,
    '#weight' => -2,
    '#description' => t('The message'),
    '#attributes' => array(
      'onchange' => 'javascript:heartbeat_rule_get_variables(this); return false;',
    ),
  );

  // Show variables if message_id_param is set
  $heartbeat_message_id = empty($form_state['values']['settings']['message_id_param']) ? $message_id : $form_state['values']['settings']['message_id_param'];

  // Show default information on how variables will appear
  $form['settings']['variables_info'] = array(
    '#type' => 'markup',
    '#value' => t('Variables in the chosen message will appear here. Assign them to a token,
      available for this type of event. Variables are supported to build heartbeat messages
       at all time from a log.<br />
      The syntax you must use goes like this. You give your variable you want to use,
      prefixed by @. (or # is for variables that need to be pretranslated)
      <br />Assign your variable to a token available from the token replacement lists above.
      <br /><strong>Note</strong> One variable assignment per line and no colons as assignment because
      colons are in the tokens as well.
      <br /><strong>Synthax</strong><pre>
@username=[author:user-name-url]
@node_type=[node:type]
@node_title=[node:title-link]
@types=pages</pre>'),
  );
  // ajax wrapper must always exist
  $default_values = heartbeat_rule_action_get_variables($heartbeat_message_id, $settings['variables_param']);
  $template_message = '';
  if ($heartbeat_message_id) {
    $message = heartbeat_message_load($heartbeat_message_id, 'message_id');
    $template_message = $message->message;
    $template_message_concat = $message->message_concat;
  }
  $display = empty($settings['variables_param']) ? 'none' : 'block';
  $template = '<blockquote>';
  if (!empty($template_message)) {
    $template .= t('<strong>Message</strong>: <div id="message-message">!message</div>', array('!message' => $template_message));
  }
  if (!empty($template_message_concat)) {
    $template .= t('<strong>Grouping message</strong>: <div id="message-message">!message</div>', array('!message' => $template_message_concat));
  }
  $template .= '</blockquote>';
  $form['settings']['variables_param'] = array(
    '#type' => 'textarea',
    '#title' => t('Assign a token to each of these variables used in the selected message'),
    '#prefix' => '<div class="clear-block" id="heartbeat-wrapper">'
    . $template . '<div id="message-variables" style="display: '. $display . '">',
    '#suffix' => '</div></div>',
    '#default_value' => $default_values,
    '#cols' => 60,
    '#rows' => 5
  );
}

/**
 * Action drupal message configuration form.
 *
 * @desc Update heartbeat_messages to synchronize,
 * and avoid separate versions of messages and message_concats
 */
function heartbeat_rules_default_action_submit(&$settings, $form, $form_state) {

  $params = $form_state['values']['settings'];
  $vars = $params['variables_param'];
  $vars = eregi_replace("\r\n", "-|-", $vars);
  $vars = eregi_replace("=", "=|=", $vars);
  $settings['variables_param'] = $vars;

  //db_query("UPDATE {heartbeat_messages} SET variables = '%s' WHERE message_id = '%s'", $vars, $params['message_id_param']);
}

/**
 * heartbeat_rules_default_action
 *   Action Implementation: Log an activity.
 *   Default action for most common heartbeat message activities.
 * @ToDo  figure out what params mean / can do
 *
 * @param array $settings
 * @param array form
 * @param array with two arrays (set_info and variables)
 */
function heartbeat_rules_default_action($settings, $form = array(), $event_info = array()) {

  if (is_array($settings)) {
    heartbeat_api_log($settings['message_id_param'], $settings['uid_param'], $settings['uid_target_param'],
      $settings['nid_param'], $settings['nid_target_param'], $settings['variables_param']);
  }

  return TRUE;
}

/**
 * User action drupal message configuration form.
 */
function heartbeat_rules_users_action_form($settings, &$form, $form_state) {

  heartbeat_rules_default_action_form($settings, $form, $form_state);

  $form['settings']['nid_param'] = array(
    '#type' => 'hidden',
    '#value' => 0,
  );
  $form['settings']['nid_target_param'] = array(
    '#type' => 'hidden',
    '#value' => 0,
  );
}

/**
 * Action drupal message configuration form.
 *
 * @desc Update heartbeat_messages to synchronize,
 * and avoid separate versions of messages and message_concats
 */
function heartbeat_rules_users_action_submit(&$settings, $form, $form_state) {
  heartbeat_rules_default_action_submit($settings, $form, $form_state);
}

/**
 * Action Implementation: Log an activity  between two users.
 *
 * @param array $settings
 * @param array form
 * @param array with two arrays (set_info and variables)
 * @ToDo  figure out what params mean / can do
 */
function heartbeat_rules_users_action($settings, $form = array(), $event_info = array()) {

  if (is_array($settings)) {
    heartbeat_api_log($settings['message_id_param'], $settings['uid_param'], $settings['uid_target_param'],
      0, 0, $settings['variables_param'] . '-|-duplicate=|=1-|-');
  }
  return TRUE;
}
