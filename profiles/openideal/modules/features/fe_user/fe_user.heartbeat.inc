<?php
/**
 * @file
 * fe_user.heartbeat.inc
 */

/**
 * Implements hook_heartbeat_template_info().
 */
function fe_user_heartbeat_template_info() {
  $export = array();

  $heartbeatmessagetemplate = new HeartbeatMessageTemplate;
  $heartbeatmessagetemplate->disabled = FALSE; /* Edit this to true to make a default heartbeatmessagetemplate disabled initially */
  $heartbeatmessagetemplate->api_version = 1;
  $heartbeatmessagetemplate->message_id = 'oi_add_comment';
  $heartbeatmessagetemplate->description = 'user replied on some content';
  $heartbeatmessagetemplate->message = 'You replied on !title ';
  $heartbeatmessagetemplate->message_concat = '%username% replied on !title.';
  $heartbeatmessagetemplate->perms = 4;
  $heartbeatmessagetemplate->group_type = 'single';
  $heartbeatmessagetemplate->concat_args = array(
    'group_by' => 'node',
    'group_target' => 'username',
    'group_by_target' => 'title',
    'group_num_max' => '5',
    'merge_separator' => ', ',
    'merge_end_separator' => ' and ',
  );
  $heartbeatmessagetemplate->variables = array(
    '!title' => '',
  );
  $heartbeatmessagetemplate->attachments = array();
  $export['oi_add_comment'] = $heartbeatmessagetemplate;

  $heartbeatmessagetemplate = new HeartbeatMessageTemplate;
  $heartbeatmessagetemplate->disabled = FALSE; /* Edit this to true to make a default heartbeatmessagetemplate disabled initially */
  $heartbeatmessagetemplate->api_version = 1;
  $heartbeatmessagetemplate->message_id = 'oi_add_node';
  $heartbeatmessagetemplate->description = 'User adds a node';
  $heartbeatmessagetemplate->message = 'You added !node_title ';
  $heartbeatmessagetemplate->message_concat = '!username has added the following !types: %node_title%.';
  $heartbeatmessagetemplate->perms = 4;
  $heartbeatmessagetemplate->group_type = 'single';
  $heartbeatmessagetemplate->concat_args = array(
    'group_by' => 'user',
    'group_target' => 'node_title',
    'group_by_target' => 'username',
    'group_num_max' => '4',
    'merge_separator' => ',',
    'merge_end_separator' => 'and',
  );
  $heartbeatmessagetemplate->variables = array(
    '!node_title' => '',
    '!username' => '',
    '!types' => '',
  );
  $heartbeatmessagetemplate->attachments = array();
  $export['oi_add_node'] = $heartbeatmessagetemplate;

  $heartbeatmessagetemplate = new HeartbeatMessageTemplate;
  $heartbeatmessagetemplate->disabled = FALSE; /* Edit this to true to make a default heartbeatmessagetemplate disabled initially */
  $heartbeatmessagetemplate->api_version = 1;
  $heartbeatmessagetemplate->message_id = 'oi_comment_on_my_content';
  $heartbeatmessagetemplate->description = 'Other user reply on my content';
  $heartbeatmessagetemplate->message = '!username replied on your\'s idea !title ';
  $heartbeatmessagetemplate->message_concat = '';
  $heartbeatmessagetemplate->perms = 4;
  $heartbeatmessagetemplate->group_type = 'single';
  $heartbeatmessagetemplate->concat_args = array(
    'group_by' => 'none',
    'group_target' => '',
    'group_by_target' => '',
    'group_num_max' => '',
    'merge_separator' => '',
    'merge_end_separator' => '',
  );
  $heartbeatmessagetemplate->variables = array(
    '!username' => '',
    '!title' => '',
  );
  $heartbeatmessagetemplate->attachments = array();
  $export['oi_comment_on_my_content'] = $heartbeatmessagetemplate;

  $heartbeatmessagetemplate = new HeartbeatMessageTemplate;
  $heartbeatmessagetemplate->disabled = FALSE; /* Edit this to true to make a default heartbeatmessagetemplate disabled initially */
  $heartbeatmessagetemplate->api_version = 1;
  $heartbeatmessagetemplate->message_id = 'oi_vote_on_my_content';
  $heartbeatmessagetemplate->description = 'Other user voted on my content';
  $heartbeatmessagetemplate->message = '!username voted on your\'s idea !title ';
  $heartbeatmessagetemplate->message_concat = '';
  $heartbeatmessagetemplate->perms = 4;
  $heartbeatmessagetemplate->group_type = 'single';
  $heartbeatmessagetemplate->concat_args = array(
    'group_by' => 'none',
    'group_target' => '',
    'group_by_target' => '',
    'group_num_max' => '',
    'merge_separator' => '',
    'merge_end_separator' => '',
  );
  $heartbeatmessagetemplate->variables = array(
    '!username' => '',
    '!title' => '',
  );
  $heartbeatmessagetemplate->attachments = array();
  $export['oi_vote_on_my_content'] = $heartbeatmessagetemplate;

  $heartbeatmessagetemplate = new HeartbeatMessageTemplate;
  $heartbeatmessagetemplate->disabled = FALSE; /* Edit this to true to make a default heartbeatmessagetemplate disabled initially */
  $heartbeatmessagetemplate->api_version = 1;
  $heartbeatmessagetemplate->message_id = 'oi_vote_on_node';
  $heartbeatmessagetemplate->description = 'User votes on some content';
  $heartbeatmessagetemplate->message = 'You voted on !node_title ';
  $heartbeatmessagetemplate->message_concat = '!username has added the following !types: %node_title%.';
  $heartbeatmessagetemplate->perms = 4;
  $heartbeatmessagetemplate->group_type = 'single';
  $heartbeatmessagetemplate->concat_args = array(
    'group_by' => 'user',
    'group_target' => 'node_title',
    'group_by_target' => 'username',
    'group_num_max' => '4',
    'merge_separator' => ',',
    'merge_end_separator' => 'and',
  );
  $heartbeatmessagetemplate->variables = array(
    '!node_title' => '',
    '!username' => '',
    '!types' => '',
  );
  $heartbeatmessagetemplate->attachments = array();
  $export['oi_vote_on_node'] = $heartbeatmessagetemplate;

  return $export;
}

/**
 * Implements hook_heartbeat_stream_info().
 */
function fe_user_heartbeat_stream_info() {
  $export = array();

  $heartbeatstream = new HeartbeatStreamConfig;
  $heartbeatstream->disabled = FALSE; /* Edit this to true to make a default heartbeatstream disabled initially */
  $heartbeatstream->api_version = 1;
  $heartbeatstream->class = 'userpage_activity';
  $heartbeatstream->real_class = 'useractivity';
  $heartbeatstream->name = 'User activity';
  $heartbeatstream->module = 'heartbeat_defaults';
  $heartbeatstream->title = 'User page activity';
  $heartbeatstream->path = 'streams/useractivity.inc';
  $heartbeatstream->settings = array(
    'settings__active_tab' => 'edit-settings-fs-pages',
    'messages_denied' => array(
      'heartbeat_add_node' => 'heartbeat_add_node',
      'heartbeat_add_comment' => 'heartbeat_add_comment',
      'heartbeat_comment_on_user_idea' => 'heartbeat_comment_on_user_idea',
      'clone_of_heartbeat_add_comment_oi' => 'clone_of_heartbeat_add_comment_oi',
      'heartbeat_edit_account' => 'heartbeat_edit_account',
      'heartbeat_edit_node' => 'heartbeat_edit_node',
    ),
    'show_message_times' => 1,
    'show_message_times_grouped' => 1,
    'poll_messages' => '10',
    'num_load_max' => '100',
    'grouping_seconds' => '7200',
    'has_block' => 1,
    'block_items_max' => '25',
    'block_show_pager' => '0',
    'page_disabled' => 0,
    'profilePage' => 0,
    'page_items_max' => '50',
    'page_show_pager' => 0,
    'page_pager_ajax' => 0,
  );
  $heartbeatstream->variables = array();
  $export['userpage_activity'] = $heartbeatstream;

  return $export;
}
