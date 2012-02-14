<?php

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

  // Messages have to loaded by ajax if the url contains an offset
  // variable AND a post variable ajax is defined.
  $ajax = !empty($_GET['ajax']);
  $page = empty($_GET['block']);

  // For block updates by ajax displayed on the user
  // profile page, we want to show the account requested.
  if (!$page && is_numeric($account) && $account > 0 && variable_get('heartbeat_show_user_profile_messages_' . $access_type, 0)) {
    $account = user_load($account);
  }
  // For blocks calling this page in general.
  elseif (arg(0) == 'user' && is_numeric(arg(1))) {
    $account = user_load(arg(1));
  }

  if ($heartbeatStream = heartbeat_stream($access_type, $page, $account)) {

    // Changes at runtime.
    $heartbeatStream->setOffsetTime($offset_time);
    $heartbeatStream->setIsPage($page);

    heartbeat_stream_build($heartbeatStream);
    if ($heartbeatStream->hasErrors()) {
      if (user_access('access devel information')) {
        dsm($heartbeatStream->getErrors());
      }
      else {
        drupal_set_message(implode('<br />', $heartbeatStream->getErrors()));
      }
    }

    $build = heartbeat_stream_view($heartbeatStream);
  }
  else {
    $build = t('You are not allowed to see this activity stream.');
  }

  // Ajax calls may directly render the content. No extra wrapping needed.
  if ($ajax) {
    return drupal_json_output(array('status' => TRUE, 'data' => drupal_render($build)));
  }

  $content = array();
  $content['#theme'] = 'heartbeat_list';
  $content['#stream'] = $heartbeatStream;
  $content['#content'] = $build;

  return $content;

}

/**
 * Page callback for one activity message.
 */
function heartbeat_message_activity($heartbeatActivity) {

  // Set the page title.
  $title = '';
  if ($heartbeatActivity->actor->uid) {
    $title = t('Activity of @username', array('@username' => $heartbeatActivity->actor->name));
  }
  drupal_set_title($title);

  $heartbeatStream = heartbeat_stream('singleactivity', 1);
  // Configuration at runtime.
  $heartbeatStream->setLatestActivityId($heartbeatActivity->uaid);
  heartbeat_stream_build($heartbeatStream);
  $build = heartbeat_stream_view($heartbeatStream);

  $content = array();
  $content['#theme'] = 'heartbeat_list';
  $content['#stream'] = $heartbeatStream;
  $content['#content'] = $build;

  return $content;

}

/**
 * Callback to poll for newer messages
 */
function heartbeat_activity_poll() {

  // if no stream is defined or the user does not have access
  if (!isset($_REQUEST['stream']) || !user_access('view heartbeat messages')) {
    return drupal_json_output(array('status' => FALSE, 'data' => ''));
  }

  $heartbeatStream = heartbeat_stream($_REQUEST['stream'], TRUE);

  // Configuration at runtime.
  $heartbeatStream->setLatestActivityId($_REQUEST['latestUaid']);
  $heartbeatStream->setAjax(TRUE);

  if (isset($_REQUEST['language'])) {
    // change the language with the post value.
    $heartbeatStream->setLanguage($_REQUEST['language']);
  }

  heartbeat_stream_build($heartbeatStream);
  $build = heartbeat_stream_view($heartbeatStream);

  $json = array(
    'status' => TRUE,
    'data' => drupal_render($build),
    'time_updates' => array());

  // Get the times to update to the current time
  if (!empty($_REQUEST['uaids'])) {
    $uaids = explode(',', $_REQUEST['uaids']);
    $result = db_query("SELECT uaid, timestamp FROM {heartbeat_activity} WHERE uaid IN(:placeholders) ", array(':placeholders' => $uaids));
    while ($row = db_fetch_object($result)) {
      $json['time_updates'][$row->uaid] = _theme_time_ago($row->timestamp);
    }
  }

  return drupal_json_output($json);

}

/**
 * CTools modal wrapper function to delete activity.
 */
function heartbeat_activity_modal_delete($js = NULL, HeartbeatActivity $heartbeatActivity) {

  $uaid = $heartbeatActivity->uaid;

  if ($js) {
    ctools_include('modal');
    ctools_include('ajax');
  }

  $form_state = array(
    'title' => t('Are you sure you want to delete this message?'),
    'ajax' => TRUE,
    'uaid' => $uaid,
  );

  // Send this all off to our form. This is like drupal_get_form only wizardy.
  $output = ctools_modal_form_wrapper('heartbeat_delete_log_confirm', $form_state);

  // Fall back if $js is not set.
  if (!$js) {

    return drupal_get_form('heartbeat_delete_log_confirm', $uaid);

  }
  else {

    if (!empty($form_state['executed'])) {
      // We'll just overwrite the form output if it was successful.
      $output = array();
      $output[] = ctools_modal_command_dismiss();
      $output[] = ajax_command_invoke('.heartbeat-activity-' . $uaid, 'heartbeatRemoveElement', array($uaid, t('Activity deleted.')));
    }

    print ajax_render($output);
    exit;
  }

}

/**
 * Menu callback: confirm deleting of logs.
 */
function heartbeat_delete_log_confirm($form, &$form_state) {

  $path = isset($_GET['destination']) ? $_GET['destination'] : (isset($_SERVER['HTTP_REFERER']) ? $_SERVER['HTTP_REFERER']: '<front>');

  // Prepare the form.
  $form = array(
    'redirect_path' => array(
      '#type' => 'hidden',
      '#value' => $path,
    ),
  );

  $form['#uaid'] = $form_state['uaid'];
  if (!empty($form_state['uaid'])) {
    $form['#uaid'] = $form_state['uaid'];
  }

  $output = confirm_form($form,
    t('Are you sure you want to delete this message?'),
    $path,
    t('This action cannot be undone.'),
    t('Delete'),
    t('Cancel'));

  return $output;

}

/**
 * Handler for wipe confirmation
 */
function heartbeat_delete_log_confirm_submit($form, &$form_state) {

  // Set the flag so the form knows it's been executed.
  $form_state['executed'] = TRUE;
  //$form_state['complete'] = TRUE;

  if (!empty($form_state['uaid'])) {

    // Delete the activity message.
    heartbeat_activity_delete(array($form_state['uaid']));

  }

}

/**
 * ahah callback
 */
function heartbeat_activity_ahah($element) {

  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_build_id = $_POST['form_build_id'];
  $form = form_get_cache($form_build_id, $form_state);
  $args = $form['#parameters'];
  $form_id = array_shift($args);
  $form['#post'] = $_POST;
  $form['#redirect'] = FALSE;
  $form['#programmed'] = FALSE;
  $form_state['post'] = $_POST;
  drupal_process_form($form_id, $form, $form_state);
  $form = drupal_rebuild_form($form_id, $form_state, $args, $form_build_id);
  $javascript = drupal_add_js(NULL, NULL, 'header');
  $_SESSION['messages'] = array();

  if (isset($form['hb_fields'][$element])) {
    $ahah_form = $form['hb_fields'][$element];
  }
  elseif(isset($form['fs_settings'][$element])) {
    $ahah_form = $form['fs_settings'][$element];
  }

  if ($ahah_form) {

    //drupal_set_message(_heartbeat_activity_get_time($form_state['values'][$element]));
    $ahah_form['#description'] = '<div class="status">' . t('Currently set ') . _heartbeat_activity_get_time($form_state['values'][$element]) . '</div>';

    unset($ahah_form['#prefix'], $ahah_form['#suffix']); // Prevent duplicate wrappers.
    drupal_json(array(
      'status'   => TRUE,
      'data'     => theme('status_messages') . drupal_render($ahah_form),
      'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
    ));
  }
  else {
    drupal_json(array(
      'status'   => FALSE,
      'data'     => t('Element not found: @element', array('@element' => $element)),
      'settings' => call_user_func_array('array_merge_recursive', $javascript['setting']),
    ));
  }
  exit;
}