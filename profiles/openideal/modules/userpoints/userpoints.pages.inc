<?php

/**
 * @file
 * Menu callbacks for userpoints.module.
 */


/**
 * Displays a detailed transaction report for an individual user.
 *
 * @param $account
 *   For which account to display. Defaults to the current user.
 */
function userpoints_list_transactions($form, &$form_state, $account = NULL, $tid = NULL) {

  // If this is an AJAX request, update $_GET['q'] so that table sorting and
  // similar links are using the correct base path.
  if ($_GET['q'] == 'system/ajax') {
    $q = 'myuserpoints';
    if (!empty($account)) {
      $q = 'user/' . $account->uid . '/points';
    }
    $_GET['q'] = $q;
  }

  if (empty($account)) {
    global $user;
    $account = $user;
  }

  $settings = array(
    'show_user' => FALSE,
  );
  $header = userpoints_get_transaction_header($settings);

  $query = db_select('userpoints_txn', 'p')->extend('PagerDefault')->extend('TableSort')
    ->fields('p')
    ->condition('p.uid', $account->uid)
    ->orderByHeader($header)
    // Enforce consistent sort order.
    ->orderBy('p.txn_id', 'DESC')
    ->limit(variable_get(USERPOINTS_REPORT_LIMIT, 10));

  if (module_exists('taxonomy')) {
    $query->leftJoin('taxonomy_term_data', 't', 'p.tid = t.tid');
  }

  $unapproved_query = db_select('userpoints_txn', 'p')
    ->condition('uid', $account->uid)
    ->condition('status', USERPOINTS_TXN_STATUS_PENDING);
  $unapproved_query->addExpression('SUM(points)');

  $values = userpoints_filter_parse_input($form_state, $tid);
  $active_category = userpoints_filter_query($query, $values);
  userpoints_filter_query($unapproved_query, $values);

  if (isset($active_category)) {
    drupal_set_title(t('!Points for @username (%category category)', userpoints_translation() + array('%category' => $active_category, '@username' => format_username($account))), PASS_THROUGH);
    $total_title = t('Total !points (%category category)', userpoints_translation() + array('%category' => $active_category));
  }
  else {
    drupal_set_title(t('!Points for @username', userpoints_translation() + array('@username' => format_username($account))));
    $total_title = t('Total !points', userpoints_translation());
  }

  $rows = array();
  foreach ($query->execute() as $transaction) {
    $rows[] = userpoints_get_transaction_row($transaction, $settings);
  }

  // Store context in the output array so that modules have access to it.
  $output = array(
    '#account' => $account,
    '#attached' => array(
      'css' => array(
        drupal_get_path('module', 'userpoints') . '/userpoints.css',
      ),
    ),
  );

  $output['form'] = userpoints_filter_form($account, $values);

  $output['list'] = array(
    '#type' => 'container',
    '#id' => 'userpoints_list_wrapper',
  );
  $output['list']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => t('No !Points earned', userpoints_translation()),
    '#weight' => -5,
    '#attributes' => array('class' => array('userpoints-myuserpoints-list')),
  );
  $output['list']['pager'] = array(
    '#markup' => theme('pager'),
    '#weight' => 0,
  );

  // Fetch pending (not yet approved) points according to the category filter.
  $pending = (int)$unapproved_query
    ->execute()
    ->fetchField();

  // Display both pending and approved points in a simple table.
  $output['list']['summary_table'] = array(
    '#theme' => 'table',
    '#header' => array(
      array(
        'data' => $total_title,
        'colspan' => 2,
      ),
    ),
    '#rows' => array(
      array(
        'data' => array(t('Approved !points', userpoints_translation()), userpoints_get_current_points($account->uid, isset($values['tid']) ? $values['tid'] : 'all')),
        'class' => array('userpoints-myuserpoints-total-approved'),
      ),
      array(
        'data' => array(t('Pending !points', userpoints_translation()), $pending),
        'class' => array('userpoints-myuserpoints-total-pending'),
      ),
    ),
    '#weight' => 10,
    '#attributes' => array('class' => array('userpoints-myuserpoints-total')),
  );

  // For simplicity, the generated output is passed to a custom alter function.
  // This would also be possible through hook_page_alter(), but that hook is
  // hard to use.
  drupal_alter('userpoints_list_transactions', $output);

  return $output;
}


/**
 * Lists the users and their point totals by all or by category.
 */
function userpoints_list_users($form, &$form_state, $tid = NULL) {

  // If this is an AJAX request, update $_GET['q'] so that table sorting and
  // similar links are using the correct base path.
  if ($_GET['q'] == 'system/ajax') {
    $_GET['q'] = 'userpoints';
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

  $values = userpoints_filter_parse_input($form_state, $tid);
  $active_category = userpoints_filter_query($query, $values);

  if (isset($active_category)) {
    drupal_set_title(t('All points (%category category)', userpoints_translation() + array('%category' => $active_category)), PASS_THROUGH);
  }
  else {
    drupal_set_title(t('All points'));
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

  // Allow other modules and themes to customize the result.
  drupal_alter('userpoints_list', $output);

  return $output;
}

/**
 * Menu callback; display details about a specific transaction.
 *
 * @param $transaction
 *   Transaction object.
 * @return
 *   Render-able array with all the information about this transaction.
 */
function userpoints_view_transaction($transaction) {
  drupal_add_css(drupal_get_path('module', 'userpoints') . '/userpoints.css');

  drupal_set_title(t('View transaction #@txn_id', array('@txn_id' => $transaction->txn_id)));

  $css_stati = array(
    USERPOINTS_TXN_STATUS_APPROVED => 'approved',
    USERPOINTS_TXN_STATUS_DECLINED => 'declined',
    USERPOINTS_TXN_STATUS_PENDING => 'pending',
  );
  $classes = 'userpoints-view-' . $css_stati[$transaction->status] . ' userpoints-view-category-' . $transaction->tid . ' userpoints-view-' . ($transaction->points > 0 ? 'positive' : 'negative');
  if (!empty($transaction->expirydate)) {
    $classes .= $transaction->expired ? ' userpoints-view-expired' : ' userpoints-view-not-expired';
  }

  $content = array(
    '#prefix' => '<div class="userpoints-view-transaction ' . $classes . '">',
    '#suffix' => '</div>',
  );

  $content['details'] = array(
    '#theme' => 'userpoints_view_category',
    '#title' => t('Details'),
    '#weight' => 0,
    '#attributes' => array('class' => array('userpoints-group-details')),
  );

  $content['details']['user'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('User'),
    '#value' => theme('username', array('account' => $transaction->user)),
    '#weight' => 0,
    '#attributes' => array('class' => array('userpoints-item-user')),
  );

  $content['details']['points'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('!Points', userpoints_translation()),
    '#value' => $transaction->points,
    '#weight' => 10,
    '#attributes' => array('class' => array('userpoints-item-points')),
  );

  $content['details']['category'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Category'),
    '#value' => $transaction->category,
    '#weight' => 20,
    '#attributes' => array('class' => array('userpoints-item-category')),
  );

  $content['details']['reason'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Reason'),
    '#value' => userpoints_create_description($transaction, array('truncate' => FALSE)),
    '#weight' => 30,
    '#attributes' => array('class' => array('userpoints-item-reason')),
  );

  $content['details']['transaction'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Transaction ID'),
    '#value' => $transaction->txn_id,
    '#weight' => 40,
    '#attributes' => array('class' => array('userpoints-item-transaction')),
  );

  $content['status'] = array(
    '#theme' => 'userpoints_view_category',
    '#title' => t('Status'),
    '#weight' => 10,
    '#attributes' => array('class' => array('userpoints-group-status')),
  );

  $stati = userpoints_txn_status();
  $content['status']['status'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Approval status'),
    '#value' => $stati[$transaction->status],
    '#weight' => 0,
    '#attributes' => array('class' => array('userpoints-item-status')),
  );

  $content['status']['date'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Creation date'),
    '#value' => format_date($transaction->time_stamp),
    '#weight' => 10,
    '#attributes' => array('class' => array('userpoints-item-date')),
  );

  $content['status']['changed'] = array(
    '#theme' => 'userpoints_view_item',
    '#title' => t('Last modified'),
    '#value' => format_date($transaction->changed),
    '#weight' => 20,
    '#attributes' => array('class' => array('userpoints-item-changed')),
  );

  if (!empty($transaction->expirydate)) {
    $content['status']['expiration_status'] = array(
      '#theme' => 'userpoints_view_item',
      '#title' => t('Expiration status'),
      '#value' => $transaction->expired ? t('Expired') : t('Not expired'),
      '#weight' => 20,
      '#attributes' => array('class' => array('userpoints-item-expiration-status')),
    );
    $content['status']['expiration_date'] = array(
      '#theme' => 'userpoints_view_item',
      '#title' => t('Expiration date'),
      '#value' => format_date($transaction->expirydate),
      '#weight' => 30,
      '#attributes' => array('class' => array('userpoints-item-points-expiration-date')),
    );
  }

  if (!empty($transaction->parent_txn_id)) {
    $parent_transaction = userpoints_transaction_load($transaction->parent_txn_id);
    $parent = l(userpoints_create_description($parent_transaction, array('link' => FALSE)), 'userpoints/view/' . $transaction->parent_txn_id, array('html' => TRUE));
  }

  $child_txn_ids = db_query('SELECT txn_id FROM {userpoints_txn} WHERE parent_txn_id = :txn_id', array(':txn_id' => $transaction->txn_id))->fetchCol();
  $children = array();
  foreach ($child_txn_ids as $child_txn_id) {
    $child_transaction = userpoints_transaction_load($child_txn_id);
    $children[] = l(userpoints_create_description($child_transaction, array('link' => FALSE)), 'userpoints/view/' . $child_txn_id, array('html' => TRUE));
  }
  $children = !empty($children) ? theme('item_list', array('items' => $children)) : '';

  if (!empty($parent) || !empty($children)) {
    $content['related'] = array(
      '#theme' => 'userpoints_view_category',
      '#title' => t('Related !points transactions', userpoints_translation()),
      '#weight' => 20,
      '#attributes' => array('class' => array('userpoints-group-related')),
    );

    if (!empty($parent)) {
      $content['related']['parent'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Prior transaction'),
        '#value' => $parent,
        '#weight' => 0,
        '#attributes' => array('class' => array('userpoints-item-parent')),
      );
    }

    if (!empty($children)) {
      $content['related']['children'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Follow-up transactions'),
        '#value' => $children,
        '#weight' => 10,
        '#attributes' => array('class' => array('userpoints-item-children')),
      );
    }
  }

  if (userpoints_admin_access('edit')) {
    $content['admin'] = array(
      '#theme' => 'userpoints_view_category',
      '#title' => t('Admin'),
      '#weight' => 30,
      '#attributes' => array('class' => array('userpoints-group-admin')),
    );

    if (!empty($transaction->approver_uid)) {
      $content['admin']['moderator'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Moderator'),
        '#value' => theme('username', array('account' => user_load($transaction->approver_uid))),
        '#weight' => 0,
        '#attributes' => array('class' => array('userpoints-item-moderator')),
      );
    }

    if (!empty($transaction->description)) {
      $content['admin']['description_manual'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Description (manually entered)'),
        '#value' => $transaction->description,
        '#weight' => 10,
        '#attributes' => array('class' => array('userpoints-item-description-manual')),
      );

      $content['admin']['description_generated'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Description (auto generated)'),
        '#value' => userpoints_create_description($transaction, array('skip_description' => TRUE, 'truncate' => FALSE)),
        '#weight' => 20,
        '#attributes' => array('class' => array('userpoints-item-description-generated')),
      );
    }

    $content['admin']['operation'] = array(
      '#theme' => 'userpoints_view_item',
      '#title' => t('Operation'),
      '#value' => $transaction->operation,
      '#weight' => 30,
      '#attributes' => array('class' => array('userpoints-item-operation')),
    );

    if (!empty($transaction->reference)) {
      $content['admin']['reference'] = array(
        '#theme' => 'userpoints_view_item',
        '#title' => t('Internal reference'),
        '#value' => $transaction->reference,
        '#weight' => 40,
        '#attributes' => array('class' => array('userpoints-item-reference')),
      );
    }

    $content['admin']['actions'] = array(
      '#theme' => 'userpoints_view_item',
      '#title' => t('Actions'),
      '#value' => userpoints_get_transaction_actions($transaction, FALSE),
      '#weight' => 50,
      '#attributes' => array('class' => array('userpoints-item-actions')),
    );
  }
  return $content;
}