<?php
// $Id: heartbeat.pages.inc,v 1.1.2.21 2010/10/16 09:16:35 stalski Exp $

/**
 * @file
 *   Separate codefile for page displays
 */

/**
 * Page callback function to load an activity stream page
 *
 * @param array $access_type
 * @return string Themed list of messages
 */
function heartbeat_messages_page($access_type, $offset_time = 0, $account = NULL) {

  if (!user_access('view heartbeat messages') || !variable_get('heartbeat_enabled', 1)) {
    drupal_access_denied();
    exit();
  }

  $output = '';

  if ($offset_time == 0) {
    $offset_time = $_SERVER['REQUEST_TIME'];
  }

  // Messages have to loaded by ajax if the url contains an offset
  // variable AND a post variable ajax is defined.
  $ajax = !empty($_POST['ajax']);
  $page = empty($_POST['block']);

  // Normal page request with a offset time need a previous link
  if (!$ajax && $offset_time > 0 && $offset_time != $_SERVER['REQUEST_TIME']) {
    $output .= heartbeat_stream_prev_link($access_type);
  }

  // For block updates by ajax displayed on the user
  // profile page, we want to show the account requested.
  if (!$page && is_numeric($account) && $account > 0 && variable_get('heartbeat_show_user_profile_messages_'. $access_type, 0)) {
    $account = heartbeat_user_load($account);
  }

  // Message streams for each access type
  $context = heartbeat_stream_view($access_type, $page, $offset_time, $ajax, $account);

  if (!isset($context)) {
    return $ajax ? drupal_json(array('status' => TRUE, 'data' => t('No messages found.'))) : t('No messages found.');
  }

  // Get the messages
  $messages = $context->execute();

  $heartbeataccess = $context->getState();
  $link = '';
  if ($context->hasMoreMessages()) {
    $last_message = end($messages);
    $link = heartbeat_stream_more_link($heartbeataccess, $last_message->timestamp, $page);
  }
  $theme_function = $ajax ? 'heartbeat_messages' : 'heartbeat_list';
  $output .= theme($theme_function, $messages, $heartbeataccess, $link);

  if ($ajax) {
    return drupal_json(array('status' => TRUE, 'data' => $output));
  }

  return $output;
}

/**
 * Page callback for one activity message
 */
function heartbeat_message_activity($uaid) {

  $output = '';

  // Message streams for each access type
  $context = heartbeat_stream_view('singleheartbeat', TRUE);
  if (!isset($context)) {
    return t('No message found.');
  }
  $heartbeataccess = $context->getState();
  $heartbeataccess->stream->latest_uaid = $uaid;
  $context->updateState($heartbeataccess);

  // Get the message
  $messages = $context->execute();
  $heartbeataccess = $context->getState();

  if (count($messages) == 0) {
  	drupal_access_denied();
  	exit();
  }

  $output = theme('heartbeat_list', $messages, $heartbeataccess, '');

  return $output;
}

/**
 * Callback to poll for newer messages
 */
function heartbeat_activity_poll() {

  // if no stream is defined or the user does not have access
  if (!isset($_REQUEST['stream']) || !user_access('view heartbeat messages')) {
    return drupal_json(array('status' => FALSE, 'data' => ''));
  }

  $latest_uaid = $_REQUEST['latestUaid'];
  $access_type = $_REQUEST['stream'];

  $context = heartbeat_stream_view($access_type, TRUE, 0, TRUE);

  if (!isset($context)) {
    return drupal_json(array('status' => TRUE, 'data' => ''));
  }

  $heartbeataccess = $context->getState();
  $heartbeataccess->stream->latest_uaid = $latest_uaid;

  // change the language with the post value
  if (isset($_REQUEST['language'])) {
    $heartbeataccess->stream->language = $_REQUEST['language'];
  }

  $context->updateState($heartbeataccess);

  // Get the messages
  $messages = $context->execute();

  $json = array(
    'status' => TRUE,
    'stream' => $heartbeataccess->stream->name,
    'data' => theme('heartbeat_messages', $messages, $context->getState()),
    'time_updates' => array());

  // Get the times to update to the current time
  if (!empty($_REQUEST['uaids'])) {
    $uaids = explode(',', $_REQUEST['uaids']);
    $placeholders = implode(',', array_fill(0, count($uaids), "%d"));
    $result = db_query("SELECT uaid, timestamp FROM {heartbeat_activity} WHERE uaid IN($placeholders) ", $uaids);
    while ($row = db_fetch_object($result)) {
      $json['time_updates'][$row->uaid] = _theme_time_ago($row->timestamp);
    }
  }

  return drupal_json($json);
}

/**
 * Menu callback: confirm deleting of logs.
 */
function heartbeat_delete_log_confirm($form_state, $uaid) {
  $form = array(
    'uaid' => array(
      '#type' => 'hidden',
      '#value' => $uaid
    ),
    'redirect_path' => array(
      '#type' => 'hidden',
      '#value' => isset($_GET['destination']) ? $_GET['destination'] : $_SERVER['HTTP_REFERER']
    ),
  );
  return confirm_form($form,
    t('Are you sure you want to delete this message?'),
    $_GET['destination'],
    t('This action can not be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Handler for wipe confirmation
 */
function heartbeat_delete_log_confirm_submit($form, &$form_state) {

  if (!is_numeric($form_state['values']['uaid'])) {
    return;
  }

  _heartbeat_activity_delete($form_state['values']['uaid']);

  $form_state['redirect'] = isset($_GET['destination']) ? $_GET['destination'] : $form_state['values']['redirect_path'];
  drupal_set_message(t('Message deleted'));
}
