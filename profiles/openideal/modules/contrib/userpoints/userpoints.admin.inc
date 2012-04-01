<?php

/**
 * @file
 * Admin menu callbacks for userpoints.module.
 */

function userpoints_confirm_approve_submit($form, &$form_state) {
  global $user;

  $params = array(
    'txn_id' => $form_state['values']['txn_id'],
    'approver_uid' => $user->uid,
    'status' => $form_state['values']['operation'],
  );
  userpoints_userpointsapi($params);

  $form_state['redirect'] = 'admin/config/people/userpoints/moderate';
}

/**
 * Form builder for add/edit userpoints transaction form.
 */
function userpoints_admin_txn($form, &$form_state, $mode, $txn = NULL) {

  drupal_add_css(drupal_get_path('module', 'userpoints') . '/userpoints.css');

  $timestamp = format_date(REQUEST_TIME, 'custom', 'Y-m-d H:i O');
  if ($mode == 'edit') {
    drupal_set_title(t('Edit !points transaction', userpoints_translation()));
    $timestamp = format_date($txn->time_stamp, 'custom', 'Y-m-d H:i:s O');
    $txn_user = $txn->user;

    $form['txn'] = array(
      '#type' => 'value',
      '#value' => $txn,
    );
  }
  elseif ($mode == 'add') {
    drupal_set_title(t('Add !points', userpoints_translation()));
    if ($txn) {
      $txn_user = user_load($txn);
    }
  }
  $form['txn_user'] = array(
      '#type' => 'textfield',
      '#title' => t('User Name'),
      '#size' => 30,
      '#maxlength' => 60,
      '#default_value' => isset($txn_user) ? $txn_user->name : '',
      '#autocomplete_path' => $mode == 'edit' ? NULL : 'user/autocomplete',
      '#description' => t('The name of the user who should gain or lose !points.', userpoints_translation()),
      '#required' => TRUE,
      '#weight' => -20,
      '#disabled' => $mode == 'edit',
  );

  $form['points'] = array(
      '#type' => 'textfield',
      '#title' => t('Points'),
      '#size' => 10,
      '#maxlength' => 10,
      '#default_value' => isset($txn->points) ? $txn->points : 0,
      '#description' => t('The number of !points to add or subtract.  For example, enter %positive to add !points or %negative to deduct !points.', array('%positive' => 25, '%negative' => -25) + userpoints_translation()),
      '#required' => TRUE,
      '#weight' => -15,
  );

  if (module_exists('taxonomy')) {
    $options = userpoints_get_categories();
    $form['tid'] = array(
      '#type' => 'select',
      '#title' => t('Category'),
      '#default_value' => isset($txn->tid) ? $txn->tid : userpoints_get_default_tid(),
      '#options' => $options,
      '#description' => t('The !points category that should apply to this transaction.', userpoints_translation()),
      '#weight' => 0,
      // Only show the category if there are actually categories to choose from.
      '#access' => count($options) > 1,
    );
  }

  $form['additional_settings'] = array(
    '#type' => 'vertical_tabs',
  );

  $form['status'] = array(
    '#type' => 'fieldset',
    '#title' => t('Status'),
    '#group' => 'additional_settings',
  );

  if ($mode == 'add') {
    $form['status']['moderate'] = array(
      '#title' => t('Moderated'),
      '#type' => 'checkbox',
      '#description' => t('If checked, this !points transaction must be approved, through the moderation process.', userpoints_translation()),
      '#default_value' => variable_get(USERPOINTS_POINTS_MODERATION, 0),
      '#access' => userpoints_admin_access('moderate'),
      '#weight' => -10,
    );
  }
  else {
    $form['status']['status'] = array(
      '#title' => t('Approval status'),
      '#type' => 'radios',
      '#options' => userpoints_txn_status(),
      '#description' => t('Approval status of the transaction.'),
      '#default_value' => $txn->status,
      '#access' => userpoints_admin_access('moderate'),
      '#weight' => -10,
    );
  }

  $form['status']['time_stamp'] = array(
    '#type' => 'textfield',
    '#title' => t('Date/Time'),
    '#default_value' => $timestamp,
    '#size' => 30,
    '#maxlength' => 30,
    '#description' => t('The date and time recorded for this transaction. Use this format: YYYY-MM-DD HH:MM +ZZZZ.'),
    '#weight' => -5,
    // Do not show this if it is not allowed to change the timestamp anyway.
    '#access' => !variable_get(USERPOINTS_TRANSACTION_TIMESTAMP, 1),
  );

  $expirydate = 0;
  if (isset($txn->txn_id)) {
    if ($txn->expirydate > 0) {
      $expirydate = format_date($txn->expirydate, 'custom', 'Y-m-d H:i:s O');
    }
  }
  else {
    // If we're not editing we use site defaults.
    $expirydate = userpoints_get_default_expiry_date();
    if ($expirydate) {
      $expirydate = format_date($expirydate, 'custom', 'Y-m-d H:i:s O');
    }
  }
  $form['status']['expirydate'] = array(
      '#type' => 'textfield',
      '#title' => t('Expiration date'),
      '#default_value' => $expirydate ? $expirydate : '',
      '#size' => 30,
      '#maxlength' => 30,
      '#description' => t('The date and time that the !points should expire. Use this format: YYYY-MM-DD HH:MM +ZZZZ. Leave this field blank if the !points should never expire.', userpoints_translation()),
      '#weight' => 25,
  );

  $form['reason'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reason'),
    '#group' => 'additional_settings',
  );

  $form['reason']['operation'] = array(
      '#type' => 'textfield',
      '#title' => t('Operation'),
      '#default_value' => isset($txn->operation) ? $txn->operation : t('admin'),
      '#maxlength' => 48,
      '#description' => t('The operation type for this transaction (default is %admin). Any value is valid but using a defined operation will cause an auto-generated description (specific to the chosen operation) to be included. This description can be translated into multiple languages.', array('%admin' => t('admin'))),
      '#weight' => 5,
      '#required' => FALSE,
      '#autocomplete_path' => 'userpoints/operation-autocomplete',
  );

  $form['reason']['description'] = array(
      '#type' => 'textarea',
      '#title' => t('Description'),
      '#default_value' => isset($txn->description) ? $txn->description : '',
      '#rows' => 7,
      '#cols' => 40,
      '#description' => t('Enter an optional description for this transaction, such as the reason !points were added or subtracted.', userpoints_translation()),
      '#weight' => 10,
  );

  $form['reference'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reference'),
    '#group' => 'additional_settings',
  );

  $options = array('' => '< ' . t('None') . ' >');
  foreach (entity_get_info() as $type => $info) {
    $options[$type] = $info['label'];
  }
  $form['reference']['entity_type'] = array(
    '#type' => 'select',
    '#title' => t('Linked entity'),
    '#weight' => 0,
    '#options' => $options,
    '#default_value' => isset($txn->entity_type) ? $txn->entity_type : '',
  );

  $form['reference']['entity_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Entity ID'),
    '#title_display' => 'invisible',
    '#weight' => 5,
    '#size' => 5,
    '#maxlength' => 20,
    '#default_value' => isset($txn->entity_id) ? $txn->entity_id : '',
    '#description' => t('Choose the entity type and ID to be referenced. A link to the entity will be shown.'),
  );

  $form['reference']['reference'] = array(
      '#type' => 'textfield',
      '#title' => t('Internal reference'),
      '#default_value' => isset($txn->reference) ? $txn->reference : '',
      '#size' => 30,
      '#maxlength' => 128,
      '#description' => t('Enter an optional reference code for this transaction. This is for internal tracking and is not shown to the end user.', userpoints_translation()),
      '#weight' => 10,
  );

  $approved_by = !empty($txn->approver_uid) ? user_load($txn->approver_uid) : NULL;
  if ($approved_by) {
    $form['status']['approver'] = array(
      '#type' => 'textfield',
      '#title' => t('Moderator'),
      '#default_value' => $approved_by->name,
      '#size' => 30,
      '#maxlength' => 30,
      '#description' => t('The user who gave the transaction its current status.'),
      '#weight' => 30,
    );
  }

  $form['mode'] = array(
      '#type' => 'hidden',
      '#default_value' => $mode
  );

  $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save'),
      '#weight' => 50,
  );
  return $form;
}
/**
 * Autocomplete callback for search an operation.
 *
 * @param $search
 *   Search string.
 */
function userpoints_operation_autocomplete($search) {
  $results = array();
  if (!empty($search)) {
    foreach (userpoints_get_info() as $operation => $info) {
      if (strpos($operation, $search) !== FALSE) {
        $results[$operation] = userpoints_create_operation_autocomplete_label($operation, $info, $search);
      }
      elseif (isset($info['admin description']) && strpos($info['admin description'], $search) !== FALSE) {
        $results[$operation] = userpoints_create_operation_autocomplete_label($operation, $info, $search);
      }
      elseif (isset($info['description']) && strpos($info['description'], $search) !== FALSE) {
        $results[$operation] = userpoints_create_operation_autocomplete_label($operation, $info, $search);
      }
    }
  }
  drupal_json_output((object)$results);
}

/**
 * Create a autocomplete label with highlighted search text.
 *
 * @param $operation
 *   Operation string.
 * @param $info
 *   Operation info array.
 * @param $search
 *   Search text that will be highlighted if found.
 *
 * @return
 *   Highlighted label. If existing, the admin description is used and if not,
 *   the operation string.
 */
function userpoints_create_operation_autocomplete_label($operation, $info, $search) {
  $label = $operation;
  if (!empty($info['admin description'])) {
    $label = $info['admin description'];
  }
  return preg_replace("/(" . preg_quote($search) . ")/i","<strong>$1</strong>", $label);
}

/**
 * Validate function for userpoints transaction form.
 */
function userpoints_admin_txn_validate($form, &$form_state) {
  $txn_user = user_load_by_name($form_state['values']['txn_user']);
  if (!is_object($txn_user)) {
    form_set_error('txn_user', t('Specified user does not exist.'));
  }
  else {
    form_set_value($form['txn_user'], $txn_user, $form_state);
  }

  if ((int)$form_state['values']['points'] == 0) {
    form_set_error('points', t('Amount of !points must be a positive or negative number.', userpoints_translation()));
  }

  if (!strtotime($form_state['values']['time_stamp'])) {
    form_set_error('time_stamp', t('The provided timestamp is not a valid date.'));
  }

}

/**
 * Submit function for userpoints transaction form.
 */
function userpoints_admin_txn_submit($form, &$form_state) {
  global $user;
  if ($form_state['values']['mode'] == 'add') {
    $params = array(
      'points' => $form_state['values']['points'],
      'uid' => $form_state['values']['txn_user']->uid,
      'operation' => $form_state['values']['operation'],
      'description' => $form_state['values']['description'],
      'reference' => $form_state['values']['reference'],
      'tid' => $form_state['values']['tid'],
      'time_stamp' => strtotime($form_state['values']['time_stamp']),
      'moderate' => (bool)$form_state['values']['moderate'],
      'approver_uid' => $user->uid,
    );
    if ($form_state['values']['expirydate']) {
      // Check for the existence of an expirydate.
      $params['expirydate'] = strtotime($form_state['values']['expirydate']);
    }
    if (!empty($form_state['values']['entity_id']) && !empty($form_state['values']['entity_type'])) {
      $params['entity_type'] = $form_state['values']['entity_type'];
      $params['entity_id'] = (int)$form_state['values']['entity_id'];
    }
  }
  else {
    $expirydate = 0;
    if (!empty($form_state['values']['expirydate'])) {
      $expirydate = strtotime($form_state['values']['expirydate']);
    }

    // If status changed, the current user is the new approver, when not
    // changed, then the current approver is kept.
    if ($form_state['values']['txn']->status == $form_state['values']['status']) {
      $approver_uid = $form_state['values']['txn']->approver_uid;
    }
    else {
      $approver_uid = $user->uid;
    }

    $params = array(
      'uid' => $form_state['values']['txn']->uid,
      'approver_uid' => $approver_uid,
      'points' => $form_state['values']['points'],
      'tid' => $form_state['values']['tid'],
      'time_stamp' => strtotime($form_state['values']['time_stamp']),
      'operation' => $form_state['values']['operation'],
      'description' => $form_state['values']['description'],
      'reference' => $form_state['values']['reference'],
      'status' => $form_state['values']['status'],
      'expirydate' => $expirydate,
      'txn_id' => $form_state['values']['txn']->txn_id,
      'display' => FALSE,
    );
    if (!empty($form_state['values']['entity_id']) && !empty($form_state['values']['entity_type'])) {
      $params['entity_type'] = $form_state['values']['entity_type'];
      $params['entity_id'] = (int)$form_state['values']['entity_id'];
    }
    drupal_set_message(t('Changes to the !points transaction have been saved.', userpoints_translation()));
  }
  userpoints_userpointsapi($params);

  $form_state['redirect'] = 'admin/config/people/userpoints';
}

/**
 * Provides an administrative interface for managing points.
 */
function userpoints_admin_points($form, &$form_state) {

  // If this is an AJAX request, update $_GET['q'] so that table sorting and
  // similar links are using the correct base path.
  if ($_GET['q'] == 'system/ajax') {
    $_GET['q'] = 'admin/config/people/userpoints';
  }

  $header = userpoints_get_list_header();
  $query = db_select('userpoints', 'p')->extend('PagerDefault')->extend('TableSort')
    ->fields('p', array('uid', 'points', 'tid'))
    ->fields('u', array('name'))
    ->groupBy('p.uid')
    ->groupBy('u.name')
    ->groupBy('p.points')
    ->groupBy('p.tid')
    ->orderByHeader($header)
    ->limit(variable_get(USERPOINTS_REPORT_USERCOUNT, 30));

  $query->join('users', 'u', 'p.uid = u.uid');
  if (module_exists('taxonomy')) {
    $query->groupBy('t.name');
    $query->leftJoin('taxonomy_term_data', 't', 'p.tid = t.tid');
  }

  $values = userpoints_filter_parse_input($form_state);
  $active_category = userpoints_filter_query($query, $values);

  if (isset($active_category)) {
    drupal_set_title(t('Totals (%category category)', userpoints_translation() + array('%category' => $active_category)), PASS_THROUGH);
  }
  else {
    drupal_set_title(t('Totals'));
  }

  if (variable_get(USERPOINTS_REPORT_DISPLAYZERO, 1) == 0) {
    // The user would NOT like to see users with zero points.
    $query->condition('p.points', 0, '<>');
  }

  $rows = array();
  foreach ($query->execute() as $data) {
    $rows[] = userpoints_get_list_row($data);
  }

  $output = array();
  $output['form'] = userpoints_filter_form(NULL, $values);
  $output['list'] = array(
    '#type' => 'container',
    '#id' => 'userpoints_list_wrapper',
  );
  $output['list']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
  );
  $output['list']['pager'] = array(
    '#theme' => 'pager',
  );
  return $output;
}


/**
 * Displays a list of transactions.
 *
 */
function userpoints_admin_transactions($form, &$form_state, $moderate) {

  // If this is an AJAX request, update $_GET['q'] so that table sorting and
  // similar links are using the correct base path.
  if ($_GET['q'] == 'system/ajax') {
    $_GET['q'] = $moderate ? 'admin/config/people/userpoints/moderate' : 'admin/config/people/userpoints/transaction';
  }

  $settings = array(
    'show_status' => !$moderate,
  );
  $header = userpoints_get_transaction_header($settings);
  $query = db_select('userpoints_txn', 'p')->extend('PagerDefault')->extend('TableSort')
    ->fields('p')
    ->orderByHeader($header)
    // Enforce consistent sort order.
    ->orderBy('p.txn_id', 'DESC')
    ->limit(variable_get(USERPOINTS_REPORT_USERCOUNT, 30));

  if ($moderate) {
    $query->condition('p.status', USERPOINTS_TXN_STATUS_PENDING);
  }

  if (module_exists('taxonomy')) {
    $query->leftJoin('taxonomy_term_data', 't', 'p.tid = t.tid');
  }

  $values = userpoints_filter_parse_input($form_state);
  $active_category = userpoints_filter_query($query, $values);

  if ($moderate) {
    if (isset($active_category)) {
      drupal_set_title(t('Moderation (%category category)', userpoints_translation() + array('%category' => $active_category)), PASS_THROUGH);
    }
    else {
      drupal_set_title(t('Moderation'));
    }
  }
  else {
    if (isset($active_category)) {
      drupal_set_title(t('Transactions (%category category)', userpoints_translation() + array('%category' => $active_category)), PASS_THROUGH);
    }
    else {
      drupal_set_title(t('Transactions'));
    }
  }

  $rows = array();
  foreach ($query->execute() as $transaction) {
    $rows[] = userpoints_get_transaction_row($transaction, $settings);
  }

  // Store context in the output array so that modules have access to it.
  $output = array(
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'userpoints') . '/userpoints.css',
      ),
    ),
  );
  $output['form'] = userpoints_filter_form(NULL, $values);

  $output['list'] = array(
    '#type' => 'container',
    '#id' => 'userpoints_list_wrapper',
  );

  $output['list']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => $moderate ? t('No !points awaiting moderation', userpoints_translation()) : t('No !Points earned', userpoints_translation()),
    '#weight' => -5,
    '#attributes' => array('class' => array($moderate ? 'userpoints-moderation-list' : 'userpoints-transactions-list')),
  );
  $output['list']['pager'] = array(
    '#theme' => 'pager',
    '#weight' => 0,
  );

  return $output;
}

function userpoints_confirm_approve($form, $form_state, $operation, $transaction) {
  $form = array(
    'txn_id' => array(
        '#type' => 'value',
        '#value' => $transaction->txn_id,
    ),
  );

  $arguments = array(
    '!user' => theme('username', array('account' => $transaction->user)),
    '%category' => $transaction->category,
  ) + userpoints_translation();

  if ($operation == 'approve') {
    $question = t('Approve transaction');
    $description = format_plural($transaction->points, 'Do you want to approve @count !point for !user in the %category category?', 'Do you want to approve @count !points for !user in the %category category?', $arguments);
    $form['operation'] = array(
      '#type' => 'value',
      '#value' => USERPOINTS_TXN_STATUS_APPROVED,
    );
  }
  else {
    $question = t('Decline transaction');
    $description = format_plural($transaction->points, 'Do you want to decline @count !point for !user in the %category category?', 'Do you want to decline @count !points for !user in the %category category?', $arguments);
    $form['operation'] = array(
      '#type' => 'value',
      '#value' => USERPOINTS_TXN_STATUS_DECLINED,
    );
  }

  $description = '<p><strong>' . $description . '</strong></p>';
  $description .= '<p>' . t('Reason: !reason', array('!reason' => userpoints_create_description($transaction))) . '</p>';

  return confirm_form($form, $question, 'admin/config/people/userpoints/moderate', $description);
}
/**
 * Menu callback for settings form.
 */
function userpoints_admin_settings($form, &$form_state) {

  drupal_set_title(t('!Points settings', userpoints_translation()));

  drupal_add_js(drupal_get_path('module', 'userpoints') . '/userpoints_admin.js');

  $form['settings'] = array(
    '#prefix' => '<h3>' . t('Core !points settings', userpoints_translation()) . '</h3>',
    '#type' => 'vertical_tabs',
  );
  $group = 'renaming';
  $form[$group] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Branding'),
      '#group' => 'settings',
  );

  $form[$group][USERPOINTS_TRANS_UCPOINTS] = array(
      '#type' => 'textfield',
      '#title' => t('Word to use in the interface for the upper case plural word !Points', userpoints_translation()),
      '#default_value' => variable_get(USERPOINTS_TRANS_UCPOINTS, 'Points'),
      '#size' => 20,
      '#maxlength' => 20,
  );

  $form[$group][USERPOINTS_TRANS_LCPOINTS] = array(
      '#type' => 'textfield',
      '#title' => t('Word to use in the interface for the lower case plural word !points', userpoints_translation()),
      '#default_value' => variable_get(USERPOINTS_TRANS_LCPOINTS, 'points'),
      '#size' => 20,
      '#maxlength' => 20,
  );
  $form[$group][USERPOINTS_TRANS_UCPOINT] = array(
      '#type' => 'textfield',
      '#title' => t('Word to use in the interface for the upper case singular word !Point', userpoints_translation()),
      '#default_value' => variable_get(USERPOINTS_TRANS_UCPOINT, 'Point'),
      '#size' => 20,
      '#maxlength' => 20,
  );
  $form[$group][USERPOINTS_TRANS_LCPOINT] = array(
      '#type' => 'textfield',
      '#title' => t('Word to use in the interface for the lower case singular word !point', userpoints_translation()),
      '#default_value' => variable_get(USERPOINTS_TRANS_LCPOINT, 'point'),
      '#size' => 20,
      '#maxlength' => 20,
  );

  $group = 'status';
  $form[$group] = array(
      '#type' => 'fieldset',
      '#title' => t('Moderation'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#weight' => -1,
      '#group' => 'settings',
  );

  $form[$group][USERPOINTS_POINTS_MODERATION] = array(
      '#type' => 'radios',
      '#title' => t('Transaction status'),
      '#default_value' => variable_get(USERPOINTS_POINTS_MODERATION, 0),
      '#options' => array(t('Approved'), t('Moderated')),
      '#description' => t('Select whether all !points should be approved automatically, or moderated, and require admin approval', userpoints_translation()),
  );

  $group = "Points expiration";
  $form[$group] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Expiration', userpoints_translation()),
      '#description' => t('These settings affect new !points only, they are not retroactive. !Points expiration depends upon cron.', userpoints_translation()),
      '#group' => 'settings',
  );

  $form[$group][USERPOINTS_EXPIREAFTER_DATE] = array(
      '#type' => 'select',
      '#title' => t('Expire !points after', userpoints_translation()),
      '#description' => t('Once !points have been obtained by the user
                          they will expire according to this setting', userpoints_translation()),
      '#options' => userpoints_expiry_dates(),
      '#default_value' => variable_get(USERPOINTS_EXPIREAFTER_DATE, NULL),
  );

  // If the expiration date is earlier than today/ new points will last forever.
  // Although this may be desirable/ it could also be an oversight so we'll
  // display a message to the administrator.
  $warning = "";
  if (userpoints_date_to_timestamp(variable_get(USERPOINTS_EXPIREON_DATE, array('day' => 1, 'month' => 1, 'year' => 1900))) < REQUEST_TIME) {
    $warning = '<br /><strong>' . t('This setting will not take affect, date must be in the future') . '</strong>';
  }

  $form[$group][USERPOINTS_EXPIREON_DATE] = array(
      '#type' => 'date',
      '#title' => t('Expire !points on this date', userpoints_translation()),
      '#description' => t('Once !points have been obtained by the user they will
                         last until this date. This setting overrides the
                         "Expire after setting" above ', userpoints_translation()) . $warning,
      '#default_value' => variable_get(USERPOINTS_EXPIREON_DATE, array('day' => 1, 'month' => 1, 'year' => 1980)),
  );
  $form[$group][USERPOINTS_EXPIRY_DESCRIPTION] = array(
      '#type' => 'textarea',
      '#title' => t('Expiration entry description'),
      '#description' => t('When !points expire, you may include a description that explains to the user why his !points balance has changed. You may include the following variables in your description:', userpoints_translation()) .
                         '<br /><br />' . t('!points = Brand name used for points (lowercase, plural)
                         <br />!point = Brand name used for point (lowercase, singular)
                         <br />!Points = Brand name used for Points (capitalized, plural)
                         <br />!Point = Brand name used for Point (capitalized, singular)
                         <br />!operation = Operation that granted the original points transaction
                         <br />!description = Description of the original points transaction
                         <br />!txn_id = Transaction ID for the original points transaction
                         <br />!date = Date and time of the original points transaction'),
      '#default_value' => variable_get(USERPOINTS_EXPIRY_DESCRIPTION, ''),
  );

  $group = "misc";
  $form[$group] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Messages'),
      '#description' => t('Control the behavior of messages users see.'),
      '#group' => 'settings',
  );

  $form[$group][USERPOINTS_DISPLAY_MESSAGE] = array(
      '#type' => 'radios',
      '#title' => t('Display message'),
      '#default_value' => variable_get(USERPOINTS_DISPLAY_MESSAGE, 1),
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#description' => t('Determines if a message should be displayed whenever !points are awarded/subtracted.', userpoints_translation()),
  );
  $group = "reports";
  $form[$group] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Listings'),
      '#group' => 'settings',
  );

  $form[$group][USERPOINTS_REPORT_LIMIT] = array(
      '#type' => 'select',
      '#title' => t('Transactions per page'),
      '#default_value' => variable_get(USERPOINTS_REPORT_LIMIT, 10),
      '#options' => array(10 => 10, 20 => 20, 30 => 30, 40 => 40, 50 => 50, 100 => 100),
      '#description' => t('Limits the number of transactions displayed per page.'),
  );
  $form[$group][USERPOINTS_REPORT_DISPLAYZERO] = array(
      '#type' => 'radios',
      '#title' => t('Display zero !point users?', userpoints_translation()),
      '#default_value' => variable_get(USERPOINTS_REPORT_DISPLAYZERO, 1),
      '#options' => array(t('No'), t('Yes')),
      '#description' => t('If set to "No" users with zero !points will not be displayed in the reports', userpoints_translation()),
  );
  $form[$group][USERPOINTS_REPORT_USERCOUNT] = array(
      '#type' => 'select',
      '#title' => t('Users per page'),
      '#default_value' => variable_get(USERPOINTS_REPORT_USERCOUNT, 30),
      '#options' => array(10 => 10, 20 => 20, 30 => 30, 40 => 40, 50 => 50, 100 => 100),
      '#description' => t('When listing !points by user limit how many users are displayed on a single page', userpoints_translation()),
  );

  $form[$group]['userpoints_truncate'] = array(
    '#type'          => 'textfield',
    '#title'         => t('Truncation length for the "Reason" column in transaction listings'),
    '#description'   => t('Choose the truncation length in characters for the "Reason" column in transaction listings. The reason is not truncated on the transaction details page.'),
    '#default_value' => variable_get('userpoints_truncate', 30),
    '#size'          => 5,
    '#maxlength'     => 5,
  );

  // Categories will only appear if the taxonomy module is enabled as
  // the module is required for this functionality but not necessarily
  // a requirement for the module.
  if (module_exists('taxonomy')) {
    $group = 'category';
    $form[$group] = array(
        '#type' => 'fieldset',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#title' => t('Categorization', userpoints_translation()),
      '#group' => 'settings',
    );
    $form[$group][USERPOINTS_CATEGORY_DEFAULT_TID] = array(
        '#type' => 'select',
        '#title' => t('Default Category'),
        '#default_value' => variable_get(USERPOINTS_CATEGORY_DEFAULT_TID, NULL),
        '#options' => userpoints_get_categories(),
        '#description' => t('By default all !points are assigned to this category.  You can modify what categories are available by modifying the <a href="!url">Userpoints taxonomy</a>',
                array_merge(userpoints_translation(), array('!url' =>  url('admin/structure/taxonomy/' . taxonomy_vocabulary_load(variable_get(USERPOINTS_CATEGORY_DEFAULT_VID, ''))->machine_name)))),
    );
    $options = userpoints_get_categories(NULL);
    // 0 can not be used as a checkbox value.
    $options = array('uncategorized' => $options[0]) + $options + array('all' => t('Total !points in all categories', userpoints_translation()));
    unset($options[0]);
    $form[$group][USERPOINTS_CATEGORY_PROFILE_DISPLAY_TID] = array(
        '#type' => 'checkboxes',
        '#title' => t("Categories to display on the user profile page and in the User's !points block", userpoints_translation()),
        '#default_value' => variable_get(USERPOINTS_CATEGORY_PROFILE_DISPLAY_TID, array_keys($options)),
        '#options' => $options,
        '#description' => t('Select the !points categories that should be displayed. Check "Total !points in all categories" to display a sum total of all individual !points categories.', userpoints_translation()),
    );

    $form[$group][USERPOINTS_TRANS_UNCAT] = array(
      '#type' => 'textfield',
      '#title' => t('Word to use for the general category'),
      '#default_value' => variable_get(USERPOINTS_TRANS_UNCAT, 'General'),
      '#description' => t("By default, %default is the name used for the module's umbrella category. You may change this here.", array('%default' => t('General'))),
      '#size' => 20,
      '#maxlength' => 20,
    );
  }
  // New configuration options to override current timestamp.
  $group = "stamping";
  $form[$group] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#title' => t('Transaction stamping'),
      '#group' => 'settings',
  );
  $form[$group][USERPOINTS_TRANSACTION_TIMESTAMP] = array(
      '#type' => 'checkbox',
      '#title' => t('Always use system time'),
      '#default_value' => variable_get(USERPOINTS_TRANSACTION_TIMESTAMP, 1),
      '#description' => t('Sets if the transaction timestamp should obey current time, or can be modified by the API operations. Unchecking this option will allow customization of timestamp for the transactions.'),
  );

  $form['settings_additional'] = array(
    '#prefix' => '<h3>' . t('Additional !points settings', userpoints_translation()) . '</h3>',
    '#type' => 'vertical_tabs',
  );

  $form['setting'] = userpoints_invoke_all('setting');

  // Hide the additional vertical_tabs element if nothing is being displayed in
  // it.
  if (isset($form['settings']) && empty($form['settings'])) {
    $form['settings_additional']['#access'] = FALSE;
  }

  return system_settings_form($form);
}
