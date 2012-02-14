<?php

/**
 * Form builder; Builds the content overview form for the admin.
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
    $form['username'][$message->uaid] = array('#value' => theme('username', array('name' => user_load($message->uid)->name)));
    $form['timestamp'][$message->uaid] = array('#value' => format_date($message->timestamp, 'small'));
    $form['language'][$message->uaid] = array('#value' => $message->language);
    $form['operations'][$message->uaid] = array('#value' => l(t('view'), 'heartbeat/message/'. $message->uaid. '', array('query' => $destination)));
    $form['operations'][$message->uaid]['#value'] .= ' - ' . l(t('delete'), 'heartbeat/delete/'. $message->uaid. '', array('query' => $destination));
  }
  $form['heartbeat-activity'] = array('#type' => 'checkboxes', '#options' => isset($rows) ? $rows: array());
  $form['pager'] = array('#markup' => theme('pager', NULL));

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
function theme_heartbeat_messages_admin_overview($variables) {

  $form = $variables['form'];
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

  $output .= theme('table', array('header' => $form['header']['#value'], 'rows' => $rows));
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

/**
 * Function to maintain and administer heartbeat settings.
 *
 * @return settingsform
 */
function heartbeat_admin_settings() {

  $form = array();
  $form['general'] = array('#type' => 'vertical_tabs');

  // log settings
  $form['hb_logging'] = array(
    '#type' => 'fieldset',
    '#weight' => -5,
    '#title' => t('Log activity'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#group' => 'general',
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
    '#title' => t('Display activity'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'general',
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
  $value = variable_get('heartbeat_activity_grouping_seconds', 7200);
  $form['hb_fields']['heartbeat_activity_grouping_seconds'] = array(
    '#title' => t('Maximum gap (in seconds)'),
    '#type' => 'select',
    '#default_value' => $value,
    '#options' => array(300 => t('5 minutes'), 600 => t('10 minutes'), 1200 => t('20 minutes'), 1800 => t('30 minutes'), 3600 => t('1 hour'), 7200 => t('2 hours'), 14400 => t('4 hours'), 86400 => t('24 hours')),
    '#description' => t('Maximum gap for the same activity to be grouped together and before an identical activity can be logged again'),
  );
  $form['hb_fields']['heartbeat_activity_grouping_how_many'] = array(
    '#title' => 'Maximum number of messages to group',
    '#type' => 'textfield',
    '#size' => 20,
    '#default_value' => variable_get('heartbeat_activity_grouping_how_many', 5),
    '#description' => 'Maximum number of items that can be grouped. This can be overruled for each heartbeat message specific.',
  );

  // profile settings
  $form['hb_profile'] = array(
    '#type' => 'fieldset',
    '#weight' => -3,
    '#title' => t('User profile'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#group' => 'general',
  );
  $templates = array();
  ctools_include("export");
  foreach(ctools_export_crud_load_all('heartbeat_messages') as $template) {
    $templates[$template->message_id] = $template->message_id;
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
 * Delete callback for all activity logged to the table {heartbeat_activity}.
 */
function heartbeat_activity_delete_all_confirm($form, &$form_state) {
  return confirm_form($form,
    t('Are you sure you want to delete all content from the heartbeat activity database table?'),
    'admin/structure/heartbeat',
    t('This action can not be undone.'),
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submit callback to delete all activity logged to the table {heartbeat_activity}.
 */
function heartbeat_activity_delete_all_confirm_submit($form, &$form_state) {

  heartbeat_activity_delete(array(), TRUE);

  $form_state['redirect'] = 'admin/structure/heartbeat';
  drupal_set_message(t('All activity has been removed from the database.'));

}

/**
 * eof().
 */