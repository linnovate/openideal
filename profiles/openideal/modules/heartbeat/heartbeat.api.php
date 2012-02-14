<?php

/**
 * @file
 * Hooks provided by the Heartbeat API.
 *
 * This file is divided into static hooks (hooks with string literal names) and
 * dynamic hooks (hooks with pattern-derived string names).
 *
 * Note that the export hooks like
 *   ° hook_defaults_heartbeat_template_info,
 *   ° hook_defaults_heartbeat_stream_info
 *   ° hook_heartbeat_plugin_info
 * are dependant on CTools. This requires you to include the
 * hook_ctools_plugin_api() as well.
 *
 * There are also some utility or api functions available.
 *
 * heartbeat_api_log
 *   API function to log a message when an event occurs. Most people will log through
 *   the rules api though. However, developer are very likely to like this approach better
 *   as you can abstract the activity functionality to one module.
 *
 *   @param string $message_id
 *     Id of the message that is known in the message
 *   @param integer $uid
 *     Actor or user performing the activity
 *   @param integer $uid_target [optional]
 *     user id of the target user if present. Target users can be an addresse or a
 *     user relation transaction with the actor $uid
 *   @param integer $nid [optional]
 *     Node id for content (for context node)
 *   @param integer $nid_target [optional]
 *     Node id for content that is related to other content
 *   @param array $variables [optional]
 *     Variables can be used if you used them in the used message. Take care to use
 *     the @-sign for words that are prefix with the question mark sign in the messages
 *   @param integer $access
 *     The access to restrict the message
 *
 * function heartbeat_api_log($message_id, $uid, $uid_target = 0, $nid = 0, $nid_target = 0, $variables = array(), $access = HEARTBEAT_PUBLIC_TO_ALL, $time = 0) {
 * }
 *
 */


/**
 * @addtogroup hooks
 * @{
 */

/**
 * Implement hook_ctools_plugin_api().
 *
 * This hook is needed to let ctools know about exportables.
 */
function hook_ctools_plugin_api($module, $api) {
  if ($module == 'yourmodule' && $api == 'yourmodule') {
    return array('version' => 1);
  }
}

/**
 * Register default heartbeat streams.
 *
 * @see hook_ctools_plugin_api.
 */
function hook_defaults_heartbeat_stream_info() {

  $heartbeatstreams = array();

  $heartbeatstream = new HeartbeatStreamConfig;
  $heartbeatstream->disabled = FALSE; /* Edit this to true to make a default heartbeatstream disabled initially */
  $heartbeatstream->api_version = 1;
  $heartbeatstream->class = 'siteactivity';
  $heartbeatstream->name = 'Site activity';
  $heartbeatstream->module = 'heartbeat_defaults';
  $heartbeatstream->title = 'Site activity';
  $heartbeatstream->path = 'streams/siteactivity.inc';
  $heartbeatstream->settings = array();
  $heartbeatstream->variables = array();
  $heartbeatstreams['siteactivity'] = $heartbeatstream;

  return $heartbeatstreams;

}

/**
 * Register heartbeat templates.
 *
 * @see hook_ctools_plugin_api.
 */
function hook_defaults_heartbeat_template_info() {

  $heartbeatmessagetemplates = array();

  $heartbeatmessagetemplate = new HeartbeatMessageTemplate;
  $heartbeatmessagetemplate->disabled = FALSE; /* Edit this to true to make a default heartbeatmessagetemplate disabled initially */
  $heartbeatmessagetemplate->api_version = 1;
  $heartbeatmessagetemplate->message_id = 'heartbeat_add_node';
  $heartbeatmessagetemplate->description = 'User adds a node, save user activity';
  $heartbeatmessagetemplate->message = '!username has added !node_type !node_title.';
  $heartbeatmessagetemplate->message_concat = '!username has added the following !types: %node_title%.';
  $heartbeatmessagetemplate->perms = TRUE;
  $heartbeatmessagetemplate->group_type = 'summary';
  $heartbeatmessagetemplate->concat_args = array(
    'group_by' => 'user',
    'group_target' => 'node_title',
    'group_num_max' => '4',
    'merge_separator' => ',',
    'merge_end_separator' => 'and',
  );
  $heartbeatmessagetemplate->variables = array(
    'username' => '',
    'node_type' => '',
    'node_title' => '',
    'types' => '',
  );
  $heartbeatmessagetemplate->attachments = array();
  $heartbeatmessagetemplates['heartbeat_add_node'] = $heartbeatmessagetemplate;

  return $heartbeatmessagetemplates;

}

/**
 * Add attachments to a heartbeat message template.
 */
function hook_heartbeat_attachments(HeartbeatMessageTemplate &$heartbeatMessageTemplate) {

  $form = array();
  $form['myproperty'] = array(
    '#type' => 'checkbox',
    '#title' => t('Flag i like this'),
    '#default_value' => TRUE,
  );

  return $form;

}

/**
 * Take action when activity is being deleted.
 *
 * @param array $uaids
 *   Array of activity ID's
 */
function hook_heartbeat_activity_delete($uaids) {
  // Do your stuff...
}

/**
 * Take action before activity is saved.
 *
 * @param HeartbeatActivity $heartbeatActivity
 *   The heartbeat activity object.
 */
function hook_heartbeat_activity_presave($heartbeatActivity) {
  // Do your stuff...
}

/**
 * Take action when activity is being inserted.
 *
 * @param HeartbeatActivity $heartbeatActivity
 *   The heartbeat activity object.
 */
function hook_heartbeat_activity_insert($heartbeatActivity) {
  // Do your stuff...
}

/**
 * Take action when activity is being saved.
 *
 * @param HeartbeatActivity $heartbeatActivity
 *   The heartbeat activity object.
 */
function hook_heartbeat_activity_save($heartbeatActivity) {
  // Do your stuff...
}

/**
 * Hook to alter the stream or take action when the stream
 * is loaded.
 */
function hook_heartbeat_stream_load(HeartbeatStream $heartbeatStream) {
  // Alter things in the configuration object or the stream itself.
  // Load extra requirements such as includes, behaviors, ...
}

/**
 * Hook to load extra stuff to heartbeat messages.
 *
 * Additions and variables could easily be changed here.
 * Typical example would be to load the output string or array of
 * attachments.
 * HeartbeatStream will be by address by default, however changes made
 * at this point will not enforced as it is too late in the process.
 * Changes made to the configurated stream are done in hook_heartbeat_stream_load().
 *
 * @param array $messages_raw
 *   Array of messages resulting from the stream query.
 * @param HeartbeatStream $heartbeatStream
 *   The heartbeat stream object.
 */
function hook_heartbeat_load(&$messages_raw, HeartbeatStream $heartbeatStream) {
  // Load extra data in the attachments section.
}

/**
 * Hook to block messages that came from the query result.
 *
 * This hook interacts at a point where heartbeat parser did not set the
 * maximum number of messages yet. Try to avoid this hook as much as possible,
 * as this hook should only be used to unset/remove activity messages from
 * the stream.
 *
 * @param array $messages
 *   Array of messages to alter by address.
 * @param HeartbeatStream $heartbeatStream
 *   The heartbeat stream object.
 */
function hook_heartbeat_messages_alter(&$messages, HeartbeatStream $heartbeatStream) {
  // Only use this to change access, add hardcoded messages on top of stream,
  // deny messages, things like that ...
}

/**
 * Hook to alter messages before render engine starts.
 *
 * This hook is invoked right before the parsing of messages has finished and
 * before the template engine started rendering.
 *
 * @param array $messages
 *   Array of messages to alter by address.
 * @param HeartbeatStream $heartbeatStream
 *   The heartbeat stream object.
 */
function hook_heartbeat_theme_alter(&$messages, HeartbeatStream $heartbeatStream) {
  // Here you do pretty much whatever you want.
}

/**
 * This hook is provided by Heartbeat to prepare content for an activity object.
 *
 * In fact the result needs to be a well formed drupal build to alter in any way.
 * It will be mostly used to add output of fields retrieved by a theme function.
 * This hook will be used to load build elements or already parsed output.
 *
 * Underneath is the example of heartbeat's own implementation of this hook.
 *
 * @param HeartbeatActivity $heartbeatActivity
 *   The activity message activity object.
 * @param String $view_mode
 *   The view mode of the activity object.
 * @param string $language
 *   The language of the activity message.
 *
 */
function hook_heartbeat_activity_view(HeartbeatActivity $heartbeatActivity, $view_mode = 'full', $language = NULL) {

  if (isset($heartbeatActivity->actor->picture->uri)) {
    $heartbeatActivity->content['avatar'] = array(
      '#markup' => theme('image_style', array('style_name' => 'thumbnail', 'path' => $heartbeatActivity->actor->picture->uri, 'attributes' => array('class' => 'avatar'))),
    );
  }
  $heartbeatActivity->content['message'] = array(
    '#attributes' => array('class' => array('activity-message')),
    '#title' => t('Heartbeat activity message'),
    '#markup' => $heartbeatActivity->message,
  );
  $heartbeatActivity->content['time'] = array(
    '#title' => t('Activity on'),
    '#markup' => theme('heartbeat_time_ago', array('message' => $heartbeatActivity)),
  );
  $heartbeatActivity->content['buttons'] = array(
    '#markup' => theme('heartbeat_buttons', array('message' => $heartbeatActivity)),
  );
  $heartbeatActivity->content['attachments'] = array(
    '#markup' => theme('heartbeat_attachments', array('message' => $heartbeatActivity)),
  );

}

/**
 * Hook to calculate user ID's based on a defined relation. This can be OG, friends, followers, etc ... .
 *
 * @param Integer $uid
 *   The user ID whom we calculate the relations for.
 */
function hook_heartbeat_related_uids($uid) {
  return array($uid);
}

/**
 * @} End of "addtogroup hooks".
 */