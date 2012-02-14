<?php
// $Id: heartbeat.admin.inc,v 1.1.2.15.2.20.2.62 2010/09/26 21:03:17 stalski Exp $

/**
 * @file
 *   Admnistration tasks for heartbeat.
 *
 * @author
 *   Jochen Stals - Menhir - www.menhir.be
 */

/**
 * Callback menu page for heartbeat content administration.
 */
function heartbeat_activity_admin() {
  $edit = $_POST;

  if (isset($edit['operation']) && ($edit['operation'] == 'delete') && isset($edit['heartbeat-activity']) && $edit['heartbeat-activity']) {
    return drupal_get_form('heartbeat_messages_multiple_delete_confirm');
  }
  else {
    return drupal_get_form('heartbeat_messages_admin_overview', arg(4));
  }
}

/**
 * Form builder; Builds the comment overview form for the admin.
 *
 * @return
 *   The form structure.
 * @ingroup forms
 * @see heartbeat_messages_admin_overview_validate()
 * @see heartbeat_messages_admin_overview_submit()
 * @see theme_heartbeat_messages_admin_overview()
 */
function heartbeat_messages_admin_overview() {

  $form['options']['operation'] = array(
    '#type' => 'select',
    '#options' => array('delete' => t('Delete')),
    '#default_value' => 'delete',
  );
  $form['options']['submit'] = array('#type' => 'submit', '#value' => t('Update'));

  // load the comments that we want to display
  $form['header'] = array('#type' => 'value', '#value' => array(
    theme('table_select_header_cell'),
    array('data' => t('Activity message'), 'field' => 'message'),
    array('data' => t('Author'), 'field' => 'name'),
    array('data' => t('Language'), 'field' => 'language'),
    array('data' => t('Time'), 'field' => 'timestamp', 'sort' => 'desc'),
    array('data' => t('Operations'))
  ));
  $result = pager_query('SELECT DISTINCT ha.message AS \'message\', ha.uaid, ha.timestamp, ha.language, u.name AS registered_name, u.uid FROM {heartbeat_activity} ha LEFT JOIN {users} u ON u.uid = ha.uid '. tablesort_sql($form['header']['#value']), 50, 0, NULL);

  // build a table listing the appropriate comments
  $destination = drupal_get_destination();
  $anon = variable_get('anonymous', 'Anonymous user');
  while ($message = db_fetch_object($result)) {
    $rows[$message->uaid] = '';
    $message->name = $message->uid ? $message->registered_name : $anon;
    $title = strip_tags($message->message);
    $form['message'][$message->uaid] = array('#value' => l($title, 'heartbeat/message/'. $message->uaid, array('attributes' => array('title' => truncate_utf8($title, 128)), 'fragment' => 'heartbeat-message-'. $message->uaid)));
    $form['username'][$message->uaid] = array('#value' => theme('username', $message));
    $form['timestamp'][$message->uaid] = array('#value' => format_date($message->timestamp, 'small'));
    $form['language'][$message->uaid] = array('#value' => $message->language);
    $form['operations'][$message->uaid] = array('#value' => l(t('view'), 'heartbeat/message/'. $message->uaid. '', array('query' => $destination)));
    $form['operations'][$message->uaid]['#value'] .= ' - ' . l(t('delete'), 'heartbeat/delete/'. $message->uaid. '', array('query' => $destination));
  }
  $form['heartbeat-activity'] = array('#type' => 'checkboxes', '#options' => isset($rows) ? $rows: array());
  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
  return $form;
}

/**
 * Validate heartbeat_messages_admin_overview form submissions.
 *
 * We can't execute any 'Update options' if no messages were selected.
 */
function heartbeat_messages_admin_overview_validate($form, &$form_state) {
  $form_state['values']['heartbeat-activity'] = array_diff($form_state['values']['heartbeat-activity'], array(0));
  if (count($form_state['values']['heartbeat-activity']) == 0) {
    form_set_error('', t('Please select one or more messages to perform the update on.'));
    drupal_goto('admin/content/heartbeat');
  }
}

/**
 * Process heartbeat_messages_admin_overview form submissions.
 *
 * Execute the chosen 'Update option' on the selected comments, such as
 * publishing, unpublishing or deleting.
 */
function heartbeat_messages_admin_overview_submit($form, &$form_state) {

  if ($form_state['values']['operation'] == 'delete') {

    foreach ($form_state['values']['heartbeat-activity'] as $uaid => $value) {
      if ($value) {
        _heartbeat_activity_delete($uaid);
      }
    }

    drupal_set_message(t('The update has been performed.'));
    $form_state['redirect'] = 'admin/content/heartbeat';
  }
}

/**
 * Theme the comment admin form.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @ingroup themeable
 */
function theme_heartbeat_messages_admin_overview($form) {
  $output = drupal_render($form['options']);
  if (isset($form['message']) && is_array($form['message'])) {
    foreach (element_children($form['message']) as $key) {
      $row = array();
      $row[] = drupal_render($form['heartbeat-activity'][$key]);
      $row[] = drupal_render($form['message'][$key]);
      $row[] = drupal_render($form['username'][$key]);
      $row[] = drupal_render($form['language'][$key]);
      $row[] = drupal_render($form['timestamp'][$key]);
      $row[] = drupal_render($form['operations'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No messages available.'), 'colspan' => '5'));
  }

  $output .= theme('table', $form['header']['#value'], $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

/**
 * List the selected activitymessages and verify that the
 * admin really wants to delete them.
 *
 * @param $form_state
 *   An associative array containing the current state of the form.
 * @return
 *   TRUE if the messages should be deleted, FALSE otherwise.
 * @ingroup forms
 * @see heartbeat_messages_multiple_delete_confirm_submit()
 */
function heartbeat_messages_multiple_delete_confirm(&$form_state) {
  $edit = $form_state['post'];

  $form['heartbeat-activity'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter() returns only elements with actual values
  $counter = 0;
  foreach (array_filter($edit['heartbeat-activity']) as $uaid => $value) {
    $activity = heartbeat_load_message_instance($uaid);
    $form['heartbeat-activity'][$uaid] = array('#type' => 'hidden', '#value' => $uaid, '#prefix' => '<li>', '#suffix' => check_plain(strip_tags($activity->message)) .'</li>');
    $counter++;
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');

  if (!$counter) {
    drupal_set_message(t('There do not appear to be any messages to delete or your selected message was deleted by another administrator.'));
    drupal_goto('admin/content/heartbeat');
  }
  else {
    return confirm_form($form,
      t('Are you sure you want to delete these messages and all their attachments?'),
      'admin/content/heartbeat', t('This action cannot be undone.'),
      t('Delete activity messages'), t('Cancel'));
  }
}

/**
 * Process comment_multiple_delete_confirm form submissions.
 *
 * Perform the actual comment deletion.
 */
function heartbeat_messages_multiple_delete_confirm_submit($form, &$form_state) {

  if ($form_state['values']['confirm']) {

    foreach ($form_state['values']['heartbeat-activity'] as $uaid => $value) {
      if ($value) _heartbeat_activity_delete($uaid);
    }

    drupal_set_message(t('The messages have been deleted.'));
  }
  $form_state['redirect'] = 'admin/content/heartbeat';
}

/**
 * Function to maintain and administer heartbeat settings.
 *
 * @return settingsform
 */
function heartbeat_admin_settings() {

  $form = array();

  // log settings
  $form['hb_logging'] = array(
    '#type' => 'fieldset',
    '#weight' => -5,
    '#title' => t('Global heartbeat log settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['hb_logging']['heartbeat_enabled'] = array(
    '#title' => t('Display heartbeat activity streams'),
    '#description' => t('This is a quick way to deny the stream from display.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('heartbeat_enabled', 1),
    '#weight' => -5,
  );
  $form['hb_logging']['heartbeat_debug'] = array(
    '#title' => t('Run the heartbeat message builder in debug mode'),
    '#description' => t('This is not for production sites. It\'s a developer tool to see which messages are blocked for which reason. Note that this needs to show for all user roles!'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('heartbeat_debug', 0),
    '#weight' => -5,
  );
  $cron_delete_options = array(
    0 => t('Never, my queries are limited manually'),
    604800 => t('Older than a week'),
    2678400 => t('Older than a month'),
    5270400 => t('Older than two months'),
    7948800 => t('Older than three months'),
  );
  $form['hb_logging']['heartbeat_activity_log_cron_delete'] = array(
    '#title' => 'Delete messages older than ... (by cron)',
    '#type' => 'select',
    '#options' => $cron_delete_options,
    '#default_value' => variable_get('heartbeat_activity_log_cron_delete', 2678400),
  );

  // display settings
  $form['hb_fields'] = array(
    '#type' => 'fieldset',
    '#weight' => -4,
    '#title' => t('Global heartbeat display settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $allowed_tags = array('h1', 'h2', 'h3', 'h4', 'h5', 'h6', 'a', 'span', 'em', 'strong', 'ul', 'li', 'p', 'div', 'img', 'blockquote');
  $form['hb_fields'] ['heartbeat_allowed_tags'] = array(
    '#title' => t('Filter the messages with for these tags only'),
    '#type' => 'textfield',
    '#default_value' => variable_get('heartbeat_allowed_tags', implode(',', $allowed_tags))
  );
  $form['hb_fields'] ['heartbeat_show_message_times'] = array(
    '#title' => t('Show the time of action in message displays'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('heartbeat_show_message_times', 1),
    '#description' => t('Disabling the display overrules all other settings.'),
  );
  $form['hb_fields'] ['heartbeat_show_time_grouped_items'] = array(
    '#title' => t('Show the time of action with messages are grouped together'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('heartbeat_show_time_grouped_items', 1)
  );
  $form['hb_fields'] ['heartbeat_activity_stream_delete'] = array(
    '#title' => t('Show edit/delete buttons in the stream (role based)'),
    '#description' => t('Allow users with permission "delete heartbeat activity logs" to edit and delete activity stream.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('heartbeat_activity_stream_delete', 1)
  );
  $form['hb_fields'] ['heartbeat_activity_stream_actor_delete'] = array(
    '#title' => t('Show delete buttons in the stream for actors'),
    '#description' => t('Allow actors to delete their own message activity stream.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('heartbeat_activity_stream_actor_delete', 1)
  );
  $form['hb_fields'] ['heartbeat_activity_display_tags'] = array(
    '#title' => t('Show heartbeat tags under the messages'),
    '#description' => t('Tags are shown per activity message.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('heartbeat_activity_display_tags', 0)
  );
  $value = variable_get('heartbeat_activity_grouping_seconds', 7200);
  $form['hb_fields']['heartbeat_activity_grouping_seconds'] = array(
    '#title' => t('Maximum gap (in seconds)'),
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => $value,
    '#description' => t('Currently set @value.', array('@value' => _heartbeat_activity_get_time($value))) . '<br />' . t('Maximum gap for the same activity to be grouped together and before an identical activity can be logged again'),
    '#prefix' => '<div id="heartbeat-tabs-2">',
    '#suffix' => '</div>',
    '#ahah' => array(
      'path' => 'heartbeat/ahah/heartbeat_activity_grouping_seconds',
      'wrapper' => 'heartbeat-tabs-2',
      'event' => 'change',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );
  $form['hb_fields']['heartbeat_activity_grouping_how_many'] = array(
    '#title' => 'Maximum number of messages to group',
    '#type' => 'textfield',
    '#size' => 20,
    '#default_value' => variable_get('heartbeat_activity_grouping_how_many', 5),
    '#description' => 'Maximum number of items that can be grouped. This can be overruled for each heartbeat message specific.',
  );
  $form['hb_fields'] ['heartbeat_context_perm'] = array(
    '#title' => t('Allow messages where the node or user being used lead to access denied page'),
    '#description' => t(' heartbeat does not know about links being used in the messages. For
    this reason you can choose whether to show the links anyway. Note that when you don\'t even
     use links for nodes and/or users, you should enable this.'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('heartbeat_context_perm', 0)
  );

  // profile settings
  $form['hb_profile'] = array(
    '#type' => 'fieldset',
    '#weight' => -3,
    '#title' => t('User profile settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $templates = array();
  foreach(heartbeat_messages('all', TRUE, FALSE) as $template) {
    $templates[$template['message_id']] = $template['message_id'];
  }
  $form['hb_profile'] ['heartbeat_profile_message_templates'] = array(
    '#title' => t('Select the templates to show on the profile page'),
    '#description' => t('The enduser can have the possibility to set the restriction level of message types. Here the administrator can select and expose those message templates you want to display on the profile page.'),
    '#type' => 'select',
    '#multiple' => TRUE,
    '#options' => $templates,
    '#default_value' => variable_get('heartbeat_profile_message_templates', array())
  );

  $form = system_settings_form($form);

  return $form;
}

/**
 * Menu callback to show a page with heartbeat access types
 *   Shows the several composed streams.
 */
function heartbeat_messages_access_types($form_state = array()) {

  // Import default data (filters)
  heartbeat_default_data();

  $types = variable_get('heartbeat_access_types', array());
  $stream_data = variable_get('heartbeat_stream_data', array());

  $form = array();

  $form['#prefix'] = '<p>' . t('Visiting this page will check for new heartbeat streams.
    If you want to add a stream, just implement the hook
    <strong><em>hook_heartbeat_register_access_types</em></strong>.') . '</p>';

  $form['streams'] = array(
    '#type'   => 'fieldset',
    '#title'  => t('Streams'),
    '#collapsible' => FALSE,
    '#tree' => TRUE,
    '#theme' => 'heartbeat_stream_overview',
    '#weight' => 4,
    '#parameters' => array(
      'types' => $types,
    ),
  );
  $form['streams']['#description'] = '<p>' . t('You can select each stream to be set as profile page. This
    will create a menu entry to see the messages as displayed user and not the currently
    logged-in user. The menu tab will be available at http://yoursite.com/user/%user/%heartbeatstream.<br />
    Note that it often has very little purpose to use streams on profile page when the
    query does not target the actor. E.g. PublicHeartbeat.') . '</p>';

  foreach (array_keys($types) as $key) {
    $form['streams'][$key] = array(
      '#type' => 'checkbox',
      '#default_value' => isset($stream_data[$key]) ? $stream_data[$key]['profile'] : 0,
    );
  }

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Theme funtion for the stream overview form
 */
function theme_heartbeat_stream_overview($form) {

  $headers = array(
    t('Profile'),
    t('Stream'),
    t('Class'),
    t('Module / Path'),
    t('Operations'),
  );

  $types = $form['#parameters']['types'];
  unset($form['#parameters']);
  $rows = array();
  foreach (element_children($form) as $key) {
    $type = $types[drupal_strtolower($key)];
    if (drupal_strtolower($key) == 'singleheartbeat') {
      continue;
    }
    $rows[] = array(
      drupal_render($form[$key]),
      $type['name'],
      $type['class'],
      $type['module'] .'<br /><small>'. $type['path'] . '</small>',
      l(t('configure'), 'admin/build/heartbeat/stream/' . drupal_strtolower($key)) . ' - '.
      l(t('clone'), 'admin/build/heartbeat/stream/' . drupal_strtolower($key). '/clone')
    );
  }

  $output = theme('table', $headers, $rows);

  return $output;
}

/**
 * Submit function for the stream overview form.
 */
function heartbeat_messages_access_types_submit(&$form, &$form_state = array()) {

  $stream_data = variable_get('heartbeat_stream_data', array());
  foreach ($form_state['values']['streams'] as $stream_name => $profile) {
    if (!isset($stream_data[$stream_name])) {
      $stream_data[$stream_name] = array('profile' => 0);
    }
    $stream_data[$stream_name]['profile'] = $profile;
  }

  variable_set('heartbeat_stream_data', $stream_data);

  menu_cache_clear();
  variable_set('admin_menu_rebuild_links', TRUE);
}

/**
 * Callback function to clone a stream.
 */
function heartbeat_activity_stream_clone(& $form_state, $access_type) {

  // Permissions, allow/deny message templates
  $realname = isset($access_type['realname']) ? $access_type['realname'] : $access_type['class'];
  $form['stream_name'] = array(
    '#type' => 'textfield',
    '#title' => t('New stream name'),
    '#default_value' => '',
    '#description' => t('Choose a name for the stream, with origin <strong>@type</strong>', array('@type' => $realname)),
  );
  $form['stream_base'] = array(
    '#type' => 'hidden',
    '#default_value' => $realname,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Clone stream'),
  );

  return $form;
}

/**
 * Submit functon to clone a stream.
 */
function heartbeat_activity_stream_clone_submit($form, $form_state) {

  $used = variable_get('heartbeat_access_types', array());
  $base = drupal_strtolower($form_state['values']['stream_base']);
  $key = str_replace(array('_', '-', ' '), array('', '', ''), drupal_strtolower($form_state['values']['stream_name']));

  $stream_data = heartbeat_stream_load($base);
  $stream_data['name_original'] = $stream_data['name'];
  $stream_data['realname'] = $key;
  //$stream_data['class'] = $key;
  $stream_data['status'] = 'clone';
  $stream_data['name'] = $form_state['values']['stream_name'];
  $used[$key] = $stream_data;

  variable_set('heartbeat_access_types', $used);

}


/**
 * Callback function to configure a heartbeat stream
 */
function heartbeat_activity_stream_configure(& $form_state, $access_type) {

  $edit = $access_type['settings'];
  $form_state['stream'] = $edit;
  $class = drupal_strtolower($access_type['class']);
  $realname = isset($access_type['realname']) ? drupal_strtolower($access_type['realname']) : $class;

  $form = array('#prefix' => '<h3>'. t('Configure @type', array('@type' => $access_type['name'])) .'</h3>');

  // Permissions, allow/deny message templates
  $form['fs_perms'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Permissions'),
  );
  $messages = heartbeat_messages('all', FALSE, FALSE);
  $options = array(0 => t('No message selected'));
  foreach ($messages as $message) {
    $options[$message['message_id']] = !empty($message['description']) ? $message['description'] : str_replace('_', ' ', $message['message_id']);
  }
  $form['fs_perms']['messages_denied'] = array(
    '#type' => 'select',
    '#title' => t('Choose message types you want to deny from display'),
    '#multiple' => TRUE,
    '#default_value' => isset($access_type['messages_denied']) ? $access_type['messages_denied'] : 0,
    '#options' => $options,
  );

  // Activate stream filters
  $form['fs_filters'] = array(
    '#type' => 'fieldset',
    '#title' => t('Filters'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['fs_filters']['display_filters'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable a list of message filters for pages'),
    '#default_value' => isset($access_type['display_filters']) ? $access_type['display_filters'] : 0,
  );
  $form['fs_filters']['display_block_filters'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable a list of message filters for block displays'),
    '#default_value' => isset($access_type['display_block_filters']) ? $access_type['display_block_filters'] : 0,
  );
  $form['fs_filters']['filters_cumul'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow filters to accumulate (like checkboxes). If not checked, filters will behave as radio buttons.'),
    '#default_value' => isset($access_type['filters_cumul']) ? $access_type['filters_cumul'] : 0,
  );
  $filters = variable_get('heartbeat_filters', array());
  $options = array_keys($filters);
  $form['fs_filters']['filters'] = array(
    '#type' => 'select',
    '#title' => t('Use these filters to show parts of the stream'),
    '#multiple' => TRUE,
    '#default_value' => isset($access_type['filters']) ? $access_type['filters'] : 0,
    '#options' => drupal_map_assoc($options),
  );

  // Stream settings
  $form['fs_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['fs_settings']['poll_messages'] = array(
    '#type' => 'select',
    '#options' => array(
      0 => t('No'),
      10 => t('Every 10 seconds'),
      20 => t('Every 20 seconds'),
      30 => t('Every 30 seconds'),
      45 => t('Every 45 seconds'),
      60 => t('Every minute'),
      // XMPP/Jabber push instead of pulling the messages
      //100 => t('Use XMPP to push messages')
    ),
    '#title' => t('Poll every x seconds for newer messages to prepend the stream.'),
    '#default_value' => isset($access_type['poll_messages']) ? $access_type['poll_messages'] : 0,
  );

  $form['fs_settings']['num_load_max'] = array(
    '#title' => t('Fetch a maximum of logged messages '),
    '#type' => 'textfield',
    '#size' => 20,
    '#description' => t('Heartbeat loads a maximum number of activity messages to keep a final number.
      This number has to be bigger than the number of max items in blocks and pages. This is needed because
      streams can have messages that are denied, grouped or inhibited by permission. In
      order to make sure we have enough messages for display and to keep the performance to a high level, this
      odd way is needed.'),
    '#default_value' => $edit['num_load_max'],
  );

  if (!empty($form_state['values']['grouping_seconds'])) {
    $value = $form_state['values']['grouping_seconds'];
  }
  elseif (isset($edit['grouping_seconds'])) {
    $value = $edit['grouping_seconds'];
  }
  else {
    $value = variable_get('heartbeat_activity_grouping_seconds', 7200);
  }
  $form['fs_settings']['grouping_seconds'] = array(
    '#title' => t('Maximum gap (in seconds)'),
    '#type' => 'textfield',
    '#size' => 40,
    '#default_value' => $value,
    '#description' => t('Leave the field to see the updated value. Currently set @value.', array('@value' => _heartbeat_activity_get_time($value))) . '<br />' . t('Maximum gap for the same activity to be grouped together and before an identical activity can be logged again'),
    '#prefix' => '<div id="heartbeat-tabs-2">',
    '#suffix' => '</div>',
    '#ahah' => array(
      'path' => 'heartbeat/ahah/grouping_seconds',
      'wrapper' => 'heartbeat-tabs-2',
      'event' => 'change',
      'method' => 'replace',
      'effect' => 'fade',
    ),
  );

  $form['fs_settings']['block_items_max'] = array(
    '#title' => t('Maximum items in the @name blocks', array('@name' => $access_type['name'])),
    '#type' => 'textfield',
    '#size' => 20,
    '#default_value' => $edit['block_items_max'],
  );
  $options = array(
    0 => t('No more link'),
    1 => t('Display "full list" link in block display'),
    2 => t('Display an ajax-driven older messages link')
  );
  $form['fs_settings']['block_show_pager'] = array(
    '#title' => t('Show "older messages" link in block display'),
    '#type' => 'radios',
    '#options' => $options,
    '#default_value' => $edit['block_show_pager'],
  );

  $form['fs_settings']['page'] = array(
    '#type' => 'fieldset',
    '#title' => t('Page'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['fs_settings']['page']['page_disabled'] = array(
    '#title' => t('Disable the page for this stream'),
    '#type' => 'checkbox',
    '#default_value' => $edit['page_disabled'],
  );
  $form['fs_settings']['page']['page_items_max'] = array(
    '#title' => t('Maximum items in the @name pages', array('@name' => $access_type['name'])),
    '#type' => 'textfield',
    '#size' => 20,
    '#default_value' => $edit['page_items_max'],
  );
  $form['fs_settings']['page']['page_show_pager'] = array(
    '#title' => t('Display "older messages" link in page displays'),
    '#type' => 'checkbox',
    '#default_value' => $edit['page_show_pager'],
  );
  $form['fs_settings']['page']['page_pager_ajax'] = array(
    '#title' => t('Display "older messages" link in page displays with Ajax'),
    '#type' => 'checkbox',
    '#default_value' => $edit['page_pager_ajax'],
  );

  $form['fs_settings']['access_type'] = array(
    '#type' => 'value',
    '#value' => $realname,
  );
  $form['settings'] = array(
    '#tree' => TRUE,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#title' => t('Submit'),
    '#value' => t('Submit'),
  );

  $form['#redirect'] = 'admin/build/heartbeat/streams';

  return $form;
}

/**
 * Callback function to configure a heartbeat stream
 */
function heartbeat_activity_stream_configure_validate($form, $form_state) {
  if ($form_state['values']['num_load_max'] <= $form_state['values']['page_items_max']
    || $form_state['values']['num_load_max'] <= $form_state['values']['block_items_max']) {
    form_set_error('num_load_max', t('Number of messages to load must be bigger than the maximum set on blocks and pages'));
  }
}

/**
 * Callback function to configure a heartbeat stream
 */
function heartbeat_activity_stream_configure_submit($form, $form_state) {

  $type = $form_state['values']['access_type'];

  $settings = array(
    'block_items_max' => $form_state['values']['block_items_max'],
    'block_show_pager' => $form_state['values']['block_show_pager'],
    'page_items_max' => $form_state['values']['page_items_max'],
    'page_disabled' => $form_state['values']['page_disabled'],
    'page_show_pager' => $form_state['values']['page_show_pager'],
    'page_pager_ajax' => $form_state['values']['page_pager_ajax'],
    'num_load_max' => $form_state['values']['num_load_max'],
    'grouping_seconds' => $form_state['values']['grouping_seconds'],
  );
  // populate the settings with the form state
  if (isset($form_state['values']['settings'])) {
    $settings += $form_state['values']['settings'];
  }

  $additions = array(
    'poll_messages' => $form_state['values']['poll_messages'],
    'messages_denied' => $form_state['values']['messages_denied'],
    'display_filters' => $form_state['values']['display_filters'],
    'display_block_filters' => $form_state['values']['display_block_filters'],
    'filters_cumul' => $form_state['values']['filters_cumul'],
    'filters' => $form_state['values']['filters'],
    'settings' => $settings,
  );

  heartbeat_stream_save($type, $additions);
}

/**
 * Overview list of  heartbeat messages
 * This page must be viewed to make the messages
 * appear in the database after a module is installed
 * as well as make them translatable
 */
function heartbeat_messages_overview() {

  $intro = '';

  if (module_exists('rules')) {
    $intro = t('Go to !link to add your conditional actions to one of the existing events.',
      array('!link' => l('rules administration', 'admin/rules/trigger')));
  }

  // Try to synchronize messages (insert, delete)
  $operations = heartbeat_messages_rebuild();

  if ($operations['inserted'] > 0) {
    drupal_set_message(t('@count @messages were added to heartbeat.', array(
      '@count' => $operations['inserted'],
      '@messages' => $operations['inserted'] > 1 ? t('messages') : t('message'))
    ));
  }
  if ($operations['deleted'] > 0) {
    drupal_set_message(t('@count messages were deleted.', array(
      '@count' => $operations['deleted'],
      '@messages' => $operations['deleted'] > 1 ? t('messages') : t('message'))
    ));
  }

  // Fetch the heartbeat_message objects
  $messages = heartbeat_messages('all', TRUE, TRUE);
  if (count($messages) <= 0) {
    return t('No messages yet');
  }
  $default_rows = $custom_rows = array();

  $languages = module_exists('locale') ? locale_language_list() : array();
  foreach ($messages as $key => $message) {

    // Additional tasks for translatable messages
    if ($languages != array()) {
      _heartbeat_messages_overview_language($languages, $message);
    }

    $incode = $message->custom & HEARTBEAT_MESSAGE_DEFAULT;

    $links = l(t('edit'), "admin/build/heartbeat/edit/". $message->hid, array('query' => 'destination=admin/build/heartbeat/list'));

    if (!$incode) {
      $links .= ' - '. l(t('delete'), "admin/build/heartbeat/delete/". $message->hid, array('query' => 'destination=admin/build/heartbeat/list'));
    }

    if (($incode && $message->custom & HEARTBEAT_MESSAGE_CHANGED) || (isset($operations['diffs'][$message->message_id]))) {
      $links .= ' - ' . l(t('revert'), "admin/build/heartbeat/revert/". $message->hid, array('query' => 'destination=admin/build/heartbeat/list'));
    }

    $row = array(
      drupal_strlen($message->description) <= 0 ? str_replace('_', ' ', $message->message_id) : $message->description, $message->message_id,
      $links
    );

    if ($incode) {
      $default_rows[$key] = $row;
    }
    else {
      $custom_rows[$key] = $row;
    }

  }
  $headers = array(t('Description'), t('Message id (API usage)'), t('Operations'));

  // we check here if the reader needs to be informed about it.
  $intro .= t('<p>The messages are passed to the t-function when parsed to view.</p>');

  $intro .= '<h2>' . t('Default message templates') . '</h2>';
  $intro .= theme('table', $headers, $default_rows);
  if (count($custom_rows) > 0) {
    $intro .= '<h2>' . t('Custom message templates') . '</h2>';
    $intro .= theme('table', $headers, $custom_rows);
  }

  return $intro;
}

/**
 * Helper function to build list for multilingual messages
 */
function _heartbeat_messages_overview_language($languages, &$message) {

  // Pretend we are showing the strings to translate
  // Mis-usage of the t-function, but how could i fix this?
  $fake_message = t($message->message);
  $fake_message_concat = t($message->message_concat);

  // Show the admin user that there are things that need translation
  $report = array();
  foreach ($languages as $lang => $human_language) {

    if ($lang != 'en') {

      // Look into the messages
      if (locale($message->message, $lang) == locale($message->message, 'en')) {
        $label = t('@human_language translation', array('@human_language' => strip_tags($human_language)));
        $options = array('query' => 'op=Search&string='. str_replace(" ", "+", $message->message));
        $report[] = l($label, 'admin/build/translate/search', $options);
      }
      // Look into the message_concat groupings
      if (t($message->message_concat, array(), $lang) == t($message->message_concat, array(), 'en')) {
        $label = t('@human_language translation', array('@human_language' => strip_tags($human_language)));
        $options = array('query' => 'op=Search&string='. str_replace(" ", "+", $message->message_concat));
        $report[] = l($label, 'admin/build/translate/search', $options);
      }
    }
  }
  // Add a report of todo translations to the list
  if ($report != array()) {
    $message->description .= '<div><small>'. implode(', ', $report) .'</small></div>';
  }
}

/**
 * Function to maintain and administer heartbeat messages
 *
 * @return settingsform
 */
function heartbeat_messages_add(&$form_state) {
  return heartbeat_messages_edit($form_state, new stdClass());
}

/**
 * Function to maintain and administer heartbeat messages
 *
 * @return settingsform
 */
function heartbeat_messages_edit(&$form_state, $message) {

  drupal_add_js(drupal_get_path('module', 'heartbeat') . '/heartbeat-admin.js');

  $form = array();

  $form_state['message'] = $message;

  $edit = !empty($message->message_id);

  if ($edit) {
    $form['message_id_display'] = array(
      '#type' => 'textfield',
      '#title' => t('Unique but descriptive message id'),
      '#description' => t('Example "heartbeat_add_content" in the format heartbeat_do_something.'),
      '#default_value' => $edit ? $message->message_id : '',
      '#disabled' => TRUE,
    );
    $form['message_id'] = array(
      '#type' => 'hidden',
      '#default_value' => $message->message_id,
    );
  }
  else {
    $form['message_id'] = array(
      '#type' => 'textfield',
      '#title' => t('Unique but descriptive message id'),
      '#description' => t('Example "heartbeat_add_content" in the format heartbeat_do_something.'),
      '#default_value' => '',
    );
  }

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description of the message'),
    '#description' => t('(most of the time you already have an event in mind)'),
    '#cols' => 60,
    '#rows' => 1,
    '#default_value' => $edit ? (empty($message->description) ? '' : $message->description) : '',
  );
  $form['perms'] = array(
    '#type' => 'select',
    '#title' => t('Message display access'),
    '#description' => t('Defines to whom the message is meant for and who is entitled to see the message.'),
    '#options' => _heartbeat_perms_options(),
    '#default_value' => !isset($message->perms) ? HEARTBEAT_PUBLIC_TO_ALL : $message->perms,
  );
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Limit this message with roles'),
    '#description' => t('Select the roles to filter activity. Leaving empty means the messages will always be shown.'),
    '#options' => user_roles(),
    '#default_value' => empty($message->roles) ? array() : $message->roles,
  );
  $form['custom'] = array(
    '#type' => 'hidden',
    '#default_value' => $edit ? $message->custom : HEARTBEAT_MESSAGE_CUSTOM,
  );

  $tags = (string) (!empty($message->tags) ? implode(",", $message->tags) : '');
  // Autocomplete heartbeat tags
  $form['tags'] = array(
    '#type' => 'textfield',
    '#title' => t('Heartbeat tags'),
    '#description' => t('Enter a comma-separated list of tags for this message. It is used for tagged message displays (also available in heartbeat Views).'),
    '#default_value' => $tags,
    '#autocomplete_path' => 'heartbeat/autocomplete/tag',
  );

  // Examples with variables
  $form['examples'] = array(
    '#type' => 'fieldset',
    '#title' => t('Examples of message variables'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  // variables
  $form['examples']['tokens']['#type'] = 'markup';
  $form['examples']['tokens']['#value'] = '<p>'. t('Here are a few examples of usage of variables in heartbeat messages:') .'</p><div>';
  $form['examples']['tokens']['#value'] .= '<small>'. t('!username has updated !node_title') .' (for a single message)</small><br />';
  $form['examples']['tokens']['#value'] .= '<small>'. t('!username has added %node_title%') .' (for grouped messages with variable summary)</small><br />';

  // Extended example, specific to friendlist
  if (module_exists('friendlist_api')) {
    $form['examples']['tokens']['#value'] .= '<small>'. t('!user1 is now !relation_type with !user2') .' (use %user2% if user1 becomes friends with lots of users in last timespan)</small><br />';
  }
  $form['examples']['tokens']['#value'] .= '</div><p>'. t('Always append your variables with ! or embed the word in %\'s to group several instances of one part of a message.') .'</p>';

  // the replacement of @ from # is only needed to view them like that.
  // The actual implementation needs the # for partial message translations
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Single message'),
    '#cols' => 60,
    '#rows' => 1,
    '#required' => TRUE,
    '#default_value' => empty($message->message) ? '' : $message->message,
    '#description' => t('"!" is available to interpret a words as variables.'),
  );

  $desc = t("Type of message when it comes to grouping messages together.<br />
    <strong>Summary</strong> is when you want to group the same instance of several messages together.
    For this you will summarize a part of the message and use it as substitional variables (with separators) to
    form the merged messages. The occurrency of the message instance is also known as the count.<br />
    <strong>Count</strong> means you want to merge the messages together so you know the occurrency.
    Only one message in its single format will be displayed.<br />
    <strong>Single</strong> is when you want to repeat messages without merging them together. These messages
    are standalone and they don't take notice on previous and upcoming messages.<br />");
  $form['type'] = array(
    '#id' => 'heartbeat_message_type',
    '#type' => 'select',
    '#title' => t('Type of message'),
    '#description' => $desc,
    '#options' => drupal_map_assoc(variable_get('heartbeat_types', array('single', 'summary', 'count'))),
    '#required' => TRUE,
    '#default_value' => empty($message->concat_args['type']) ? '' : $message->concat_args['type'],
    '#attributes' => array('onchange' => 'javascript:heartbeat_message_type_onchange(this); return false;')
  );

  $form['type_summary'] = array(
    '#type' => 'markup',
    '#prefix' => '<div id="type-summary-wrapper">',
    '#suffix' => '</div>',
  );

  $form['type_summary']['message_concat'] = array(
    '#type' => 'textarea',
    '#title' => t('Message to group instances'),
    '#description' => t('You can use "%" to indicate that a variable word needs to be replaced with multiple instances of another variable (target variable). This is used when messages are merged together.<br />! is still available'),
    '#cols' => 60,
    '#rows' => 2,
    '#default_value' => empty($message->message_concat) ? '' : $message->message_concat,
  );
  // These are fields that end up as concatenation arguments (concat_args)
  $group_by = !empty($message->concat_args['group_by']) ? $message->concat_args['group_by'] : 'none';
  $form['type_summary']['group_by'] = array(
    '#type' => 'select',
    '#options' => array(
      'none' => t('No grouping'),
      'user' => t('Group by user to summarize nodes'),
      'node' => t('Group by node to summarize users'),
      'node-target' => t('Group by target object id to summarize nodes'),
      'user-user' => t('Group by user to summarize users'),
    ),
    '#title' => t('Message groups'),
    '#description' => t('<strong>Required for types summary. </strong>Messages with parts that merge together are grouped by user or node.
      E.g. Group by node if you want to summarize users and vice versa.<br />In some cases where the activity uses a relation
      between two users, then set the group by to "user-user". A good example is a friend-relation.'),
    '#required' => FALSE,
    '#default_value' => $group_by,
    '#attributes' => array('onchange' => 'javascript: if ($(this).val() == \'user-user\') {$(\'#group-by-target-wrapper\').show();} else {$(\'#group-by-target-wrapper\').hide();} return false;'),
  );
  $desc = t('<blockquote>
    Grouped message: !username added %images%.
    Single message: !username added an !image and a nice one.
    Then you will group by user and build a summary of images. The grouping variable here is "image".
    </blockquote>');
  $form['type_summary']['group_target'] = array(
    '#type' => 'textfield',
    '#title' => t('Variable to summarize'),
    '#description' => t('If you used a word between %-signs, you have to fill in the variable you want to summarize.') .'<br /> e.g.:'. $desc,
    '#required' => FALSE,
    '#default_value' => empty($message->concat_args['group_target']) ? '' : $message->concat_args['group_target'],
  );
  $form['type_summary']['group_by_target'] = array(
    '#type' => 'textfield',
    '#prefix' => '<div id="group-by-target-wrapper" style="display: ' . ($group_by == 'user-user' ? 'block' : 'none') . ';">',
    '#suffix' => '</div>',
    '#title' => t('The group by variable.'),
    '#description' => t('This is the part that you don\'t want to summarize. Group by parameter indicates your intensions. This always stays the same and can never be the same as the group variable.'),
    '#required' => FALSE,
    '#default_value' => empty($message->concat_args['group_by_target']) ? '' : $message->concat_args['group_by_target'],
  );
  $form['type_summary']['group_num_max'] = array(
    '#title' => 'Maximum number of messages to group',
    '#type' => 'textfield',
    '#size' => 5,
    '#default_value' => empty($message->concat_args['group_num_max']) ? '' : $message->concat_args['group_num_max'],
    '#description' => 'Maximum number of items that can be grouped to create one summarized message.',
  );

  $form['type_summary']['merge_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Fill in the target separator'),
    '#description' => t('Separators between the targets, like a colon. E.g. "title1<strong>,</strong> title2 and title3"'),
    '#required' => FALSE,
    '#default_value' => empty($message->concat_args['merge_separator']) ? '' : $message->concat_args['merge_separator'],
  );
  $form['type_summary']['merge_end_separator'] = array(
    '#type' => 'textfield',
    '#title' => t('Fill in the target end separator'),
    '#description' => t('Separators finishing listed targets. E.g. "title1, title2 <strong>and</strong> title3"'),
    '#required' => FALSE,
    '#default_value' => empty($message->concat_args['merge_end_separator']) ? '' : $message->concat_args['merge_end_separator'],
  );

  // Hidden elements
  $form['hid'] = array(
    '#type' => 'hidden',
    '#default_value' => empty($message->hid) ? 0 : $message->hid,
  );
  // Buttons
  $form['submit'] = array('#type' => 'submit', '#value' => t('Save'), '#weight' => 50);
  if (isset($edit['hid'])) {
    $form['delete'] = array('#type' => 'submit', '#value' => t('Delete'));
  }
  $form['data'] = array(
    '#tree' => TRUE,
  );

  // Set the attachments
  $attachments = module_invoke_all('heartbeat_attachments', $form_state['message']);
  if (!empty($attachments)) {
    $form['attachments'] = array(
      '#tree' => TRUE,
      '#type' => 'fieldset',
      '#title' => t('Attachments'),
    ) + $attachments;
  }

  return $form;
}

/**
 * Accept the form submission to add messages.
 */
function heartbeat_messages_add_submit($form, &$form_state) {

  if (!($form_state['clicked_button']['#value'] == t('Save') || $form_state['clicked_button']['#value'] == t('Import'))) {
    return;
  }

  if (!empty($form_state['values']['message'])) {

    $message = new stdClass();
    $message->message_id = $form_state['values']['message_id'];
    $message->message = $form_state['values']['message'];
    $message->message_concat = $form_state['values']['message_concat'];
    $message->perms = $form_state['values']['perms'];
    $message->custom = HEARTBEAT_MESSAGE_CUSTOM;
    $message->description = $form_state['values']['description'];
    $message->tags = $form_state['values']['tags'];

    $concat_args = array(
      'type' => $form_state['values']['type'],
      'group_by' => $form_state['values']['group_by'],
      'group_target' => $form_state['values']['group_target'],
      'group_by_target' => $form_state['values']['group_by_target'],
      'group_num_max' => $form_state['values']['group_num_max'],
      'merge_separator' => $form_state['values']['merge_separator'],
      'merge_end_separator' => $form_state['values']['merge_end_separator'],
      'roles' => $form_state['values']['roles']
    );
    $message->concat_args = $concat_args;

    if (!empty($form_state['values']['attachments'])) {
      $message->attachments = $form_state['values']['attachments'];
    }

    if (!empty($form_state['values']['data'])) {
      $message->variables = $form_state['values']['data'];
    }

    heartbeat_message_insert($message);

    drupal_set_message(t('New message was added and can be used in actions of a rule'));
  }

  return;
}

/**
 * Accept the form submission heartbeat messages to edit.
 */
function heartbeat_messages_edit_submit($form, &$form_state) {

  // Message_id's cannot be changed
  if ($form_state['clicked_button']['#value'] == t('Save') || $form_state['clicked_button']['#value'] == t('Import')) {

    $message = new stdClass();
    $message->hid = $form_state['values']['hid'];
    $message->message_id = $form_state['values']['message_id'];
    $message->message = $form_state['values']['message'];
    $message->message_concat = $form_state['values']['message_concat'];
    $message->perms = $form_state['values']['perms'];
    $message->description = $form_state['values']['description'];
    $message->tags = $form_state['values']['tags'];

    $concat_args = array(
      'type' => $form_state['values']['type'],
      'group_by' => $form_state['values']['group_by'],
      'group_target' => $form_state['values']['group_target'],
      'group_by_target' => $form_state['values']['group_by_target'],
      'group_num_max' => $form_state['values']['group_num_max'],
      'merge_separator' => $form_state['values']['merge_separator'],
      'merge_end_separator' => $form_state['values']['merge_end_separator'],
      'roles' => $form_state['values']['roles']
    );
    $message->concat_args = $concat_args;

    if (!empty($form_state['values']['attachments'])) {
      $message->attachments = $form_state['values']['attachments'];
    }

    if (!empty($form_state['values']['data'])) {
      $message->variables = $form_state['values']['data'];
    }

    if ($form_state['values']['custom'] < HEARTBEAT_MESSAGE_CHANGED) {
      $message->custom = $form_state['values']['custom'] + HEARTBEAT_MESSAGE_CHANGED;
    }
    else {
      $message->custom = $form_state['values']['custom'];
    }

    heartbeat_message_update($message);

    drupal_set_message(t('Settings saved'));
  }

  return;
}

/**
 * Menu callback: confirm delete message templates.
 */
function heartbeat_delete_confirm($form_state, $message) {
  return confirm_form(
    array('hid' => array('#type' => 'hidden', '#value' => $message->hid)),
    t('Are you sure you want to delete !mess template?', array('!mess' => $message->message_id)),
    'admin/build/heartbeat/list',
    t('This action cannot be undone.'),
    t('Delete heartbeat message template'),
    t('Cancel'));
}

/**
 * Handler for revert confirmation
 */
function heartbeat_delete_confirm_submit($form, &$form_state) {
  heartbeat_messages_delete($form_state['values']['hid']);
  $form_state['redirect'] = 'destination=admin/build/heartbeat/list';
}

/**
 * Function to delete a heartbeat message
 */
function heartbeat_messages_delete($hid) {
  global $base_url;
  $result = db_query("DELETE FROM {heartbeat_messages} WHERE hid = %d", $hid);
  drupal_set_message(t('Message deleted'));
}

/**
 * Menu callback: confirm revert message.
 */
function heartbeat_revert_confirm($form_state, $hid) {
  return confirm_form(array('hid' => array('#type' => 'hidden', '#value' => $hid)), t('Are you sure you want to revert this message template back to default?'),
    'admin/build/heartbeat/list', t('This action cannot be undone.'), t('Revert heartbeat message'), t('Cancel'));
}

/**
 * Handler for revert confirmation
 */
function heartbeat_revert_confirm_submit($form, &$form_state) {
  heartbeat_messages_revert($form_state['values']['hid']);
  $form_state['redirect'] = 'destination=admin/build/heartbeat/list';
}

/**
 * Revert a heartbeat message back to default
 */
function heartbeat_messages_revert($hid) {

  // Delete the old message
  $old_message = heartbeat_message_load($hid);

  $result = heartbeat_messages_uninstall($old_message->message_id, 'message');
  // Insert the default back in
  $defaults = module_invoke_all('heartbeat_message_info');
  foreach ($defaults as $key => $message) {
    $message = (object) $message;
    if ($message->message_id == $old_message->message_id) {
      heartbeat_messages_install(array($message));
    }
  }
}

/**
 * Function to export messages to use as default
 */
function heartbeat_messages_export($form_state = array()) {

  $form = array();

  $messages = heartbeat_messages('all', TRUE, TRUE);
  if (count($messages) == 0) {
    return t('There are not heartbeat messages to export.');
  }

  if (!isset($form_state['export'])) {
    $form['messages'] = array('#tree' => TRUE);
    foreach ($messages as $message) {
      $form['messages']['m_'. $message->hid] = array(
        '#type' => 'checkbox',
        '#title' => !empty($message->description) ? $message->description : str_replace('_', '', $message->message_id),
        '#default_value' => 0,
        '#description' => $message->message,
      );
    }
    $form['button'] = array('#type' => 'submit', '#weight' => 10, '#value' => t('Export'));
  }
  else {
    //show a textarea containg the exported configs
    $form['result'] = array(
      '#type' => 'textarea',
      '#title' => 'Exported heartbeat messages',
      '#description' => 'Copy this data and paste them in <strong>hook_heartbeat_message_info</strong>.',
      '#rows' => 15,
      '#attributes' => array('readonly' => 'readonly'),
      '#default_value' => var_export(heartbeat_messages_export_messages($form_state['export']), 1),
    );
  }

  return $form;
}

/**
 * Submit handler to stay on the same form and show a textbox
 *
 * @param array $form
 * @param array $form_state
 */
function heartbeat_messages_export_submit($form, &$form_state) {
  $count = 0;
  foreach ($form_state['values']['messages'] as $key => $value) {
    $count += $value;
  }
  if ($count) {
    $form_state['export'] = $form_state['values']['messages'];
    $form_state['rebuild'] = TRUE;
  }
}

/**
 * Import for heartbeat message templates().
 */
function heartbeat_messages_import($form_state = array()) {
  $form = array();
  $form['import'] = array(
    '#type' => 'textarea',
    '#default_valu' => '',
    '#title' => t('Paste your export here'),
    '#rows' => 15,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Import'),
  );
  return $form;
}

/**
 * Import for heartbeat message templates().
 */
function heartbeat_messages_import_submit($form, &$form_state) {
  @eval('$import = '. $form_state['values']['import'] .';');
  if (isset($import) && is_array($import) && count($import)) {
    $messages = array();
    foreach (heartbeat_messages('all', TRUE) as $template) {
      $messages[$template->message_id] = $template->hid;
    }

    foreach ($import as $key => $message) {
      $message['custom'] = HEARTBEAT_MESSAGE_CUSTOM;
      if (isset($messages[$message['message_id']])) {
        heartbeat_message_update($message);
        drupal_set_message(t('Updated %template.', array('%template' => $message['message_id'])));
      }
      else {
        heartbeat_message_insert($message);
        drupal_set_message(t('Imported %template.', array('%template' => $message['message_id'])));
      }

    }
  }
  else {
    drupal_set_message(t('Import failed.'), 'error');
  }
}

/**
 * Function to export messages to use as default
 */
function heartbeat_messages_export_messages($selected_messages) {
  $messages = heartbeat_messages('all', TRUE, FALSE);
  if (count($messages) == 0) {
    return t('There are not heartbeat messages to export.');
  }
  $info = array();
  foreach ($messages as $message) {
    $message = (object) $message;
    if (!$selected_messages['m_'. $message->hid]) {
      continue; // Leave if not selected
    }
    $concat_args = heartbeat_decode_message_variables($message->concat_args);
    $variables = heartbeat_decode_message_variables($message->variables);
    $attachments = unserialize($message->attachments);
    $info[$message->hid] = array(
      'message' => $message->message,
      'message_concat' => $message->message_concat,
      'message_id' => $message->message_id,
      'concat_args' => $concat_args,
      'description' => $message->description,
      'perms' => $message->perms,
      'custom' => HEARTBEAT_MESSAGE_DEFAULT,
      'variables' => $variables,
      'attachments' => $attachments
    );
  }
  return $info;
}

/**
 * Menu callback: confirm deleting of logs.
 */
function heartbeat_delete_logs_confirm() {
  return confirm_form(
    array(),
    t('Are you sure you want to delete all activity logs?'),
    'admin/settings/heartbeat/delete/confirm',
    t('This action can not be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Handler for wipe confirmation
 */
function heartbeat_delete_logs_confirm_submit($form, &$form_state) {
  db_query("DELETE FROM {heartbeat_activity}");
  $form_state['redirect'] = 'admin/settings/heartbeat/settings';
  drupal_set_message('All messages have been deleted');
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

/**
 * Page callback for heartbeat tag autocomplete
 */
function heartbeat_autocomplete_tag($string = '') {

  $matches = array();
  // Take the last array value
  $strings = explode(",", $string);
  $string = trim(array_pop($strings));

  if (!empty($string)) {
    $previous = implode(",", $strings);

    // Get tags that are known
    $tags = heartbeat_get_available_tags();
    foreach ($tags as $tag) {
      if (!empty($tag) && strpos($tag, $string) === 0) {
        $key = '';
        if (!empty($previous)) {
          $key .= $previous . ",";
        }
        $key .= $tag;
        $matches[$key] = $tag;
      }
    }
  }

  drupal_json($matches);
}

/**
 * Helper function to get a readable time
 */
function _heartbeat_activity_get_time($time) {

  global $language;

  return format_interval($time, 6, $language->language);
}