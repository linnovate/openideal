<?php

/**
 * @file
 * Admin page callbacks for the heartbeat comments module.
 */

/**
 * Menu callback; present an administrative comment listing.
 */
function heartbeat_comments_admin($type = 'all') {
  $edit = $_POST;

  if (isset($edit['operation']) && ($edit['operation'] == 'delete') && isset($edit['comments']) && $edit['comments']) {
    return drupal_get_form('heartbeat_comments_multiple_delete_confirm');
  }
  else {
    return drupal_get_form('heartbeat_comments_admin_overview', arg(4));
  }
}

/**
 * Form builder; Builds the comment overview form for the admin.
 *
 * @return
 *   The form structure.
 * @ingroup forms
 * @see heartbeat_comments_admin_overview_validate()
 * @see heartbeat_comments_admin_overview_submit()
 * @see theme_heartbeat_comments_admin_overview()
 */
function heartbeat_comments_admin_overview() {

  $form['options']['operation'] = array(
    '#type' => 'hidden',
    '#default_value' => 'delete',
  );
  $form['options']['submit'] = array('#type' => 'submit', '#value' => t('Delete selected'));

  // load the comments that we want to display
  $form['header'] = array('#type' => 'value', '#value' => array(
    theme('table_select_header_cell'),
    array('data' => t('Comment'), 'field' => 'subject'),
    array('data' => t('Author'), 'field' => 'name'),
    array('data' => t('Reacted on'), 'field' => 'heartbeat_title'),
    array('data' => t('Time'), 'field' => 'timestamp', 'sort' => 'desc')
  ));
  $result = pager_query('SELECT c.hcid, c.uid, c.uaid, c.message AS \'subject\', c.cleared, c.time, ha.message AS \'heartbeat_title\', u.name AS registered_name, u.uid  FROM {heartbeat_comments} c LEFT JOIN {heartbeat_activity} ha ON ha.uaid = c.uaid LEFT JOIN {users} u ON u.uid = c.uid '. tablesort_sql($form['header']['#value']), 50, 0, NULL);

  // build a table listing the appropriate comments
  $destination = drupal_get_destination();
  $anon = variable_get('anonymous', 'Anonymous user');
  while ($comment = db_fetch_object($result)) {
    $comments[$comment->hcid] = '';
    $comment->name = $comment->uid ? $comment->registered_name : $anon;
    $comment->heartbeat_title = truncate_utf8(strip_tags($comment->heartbeat_title), 50);
    $comment_message = truncate_utf8(strip_tags($comment->subject), 70);
    $form['subject'][$comment->hcid] = array('#value' => $comment_message);
    $form['username'][$comment->hcid] = array('#value' => theme('username', $comment));
    $form['heartbeat_title'][$comment->hcid] = array('#value' => l($comment->heartbeat_title, 'heartbeat/message/'. $comment->uaid));
    $form['timestamp'][$comment->hcid] = array('#value' => $comment->time);
  }
  $form['comments'] = array('#type' => 'checkboxes', '#options' => isset($comments) ? $comments: array());
  $form['pager'] = array('#value' => theme('pager', NULL, 50, 0));
  return $form;
}

/**
 * Validate heartbeat_comments_admin_overview form submissions.
 *
 * We can't execute any 'Update options' if no comments were selected.
 */
function heartbeat_comments_admin_overview_validate($form, &$form_state) {
  $form_state['values']['comments'] = array_diff($form_state['values']['comments'], array(0));
  if (count($form_state['values']['comments']) == 0) {
    form_set_error('', t('Please select one or more comments to perform the update on.'));
    drupal_goto('admin/content/heartbeat/comments');
  }
}

/**
 * Process heartbeat_comments_admin_overview form submissions.
 *
 * Execute the chosen 'Update option' on the selected comments, such as
 * publishing, unpublishing or deleting.
 */
function heartbeat_comments_admin_overview_submit($form, &$form_state) {

  if ($form_state['values']['operation'] == 'delete') {
    // extract the appropriate database query operation
    $query = "DELETE FROM {heartbeat_comments} WHERE hc.hcid = % ";
    foreach ($form_state['values']['comments'] as $cid => $value) {
      if ($value) {
        // perform the update action, then refresh node statistics
        db_query($query, $cid);
        // Add an entry to the watchdog log.
        watchdog('content', 'Heartbeat comment: updated %subject.', array('%subject' => strip_tags($comment->message)), WATCHDOG_NOTICE);
      }
    }

    drupal_set_message(t('The update has been performed.'));
    $form_state['redirect'] = 'admin/content/heartbeat/comments';
  }
}

/**
 * Theme the comment admin form.
 *
 * @param $form
 *   An associative array containing the structure of the form.
 * @ingroup themeable
 */
function theme_heartbeat_comments_admin_overview($form) {
  $output = drupal_render($form['options']);
  if (isset($form['subject']) && is_array($form['subject'])) {
    foreach (element_children($form['subject']) as $key) {
      $row = array();
      $row[] = drupal_render($form['comments'][$key]);
      $row[] = drupal_render($form['subject'][$key]);
      $row[] = drupal_render($form['username'][$key]);
      $row[] = drupal_render($form['heartbeat_title'][$key]);
      $row[] = drupal_render($form['timestamp'][$key]);
      $rows[] = $row;
    }
  }
  else {
    $rows[] = array(array('data' => t('No comments available.'), 'colspan' => '6'));
  }

  $output .= theme('table', $form['header']['#value'], $rows);
  if ($form['pager']['#value']) {
    $output .= drupal_render($form['pager']);
  }

  $output .= drupal_render($form);

  return $output;
}

/**
 * Load the entire comment by cid.
 *
 * @param $cid
 *   The identifying comment id.
 * @return
 *   A heartbeat comment object.
 */
function _heartbeat_comment_load($cid) {
  return db_fetch_object(db_query('SELECT * FROM {heartbeat_comments} WHERE hcid = %d', $cid));
}

/**
 * List the selected comments and verify that the admin really wants to delete
 * them.
 *
 * @param $form_state
 *   An associative array containing the current state of the form.
 * @return
 *   TRUE if the comments should be deleted, FALSE otherwise.
 * @ingroup forms
 * @see heartbeat_comments_multiple_delete_confirm_submit()
 */
function heartbeat_comments_multiple_delete_confirm(&$form_state) {
  $edit = $form_state['post'];

  $form['comments'] = array('#prefix' => '<ul>', '#suffix' => '</ul>', '#tree' => TRUE);
  // array_filter() returns only elements with actual values
  $comment_counter = 0;
  foreach (array_filter($edit['comments']) as $cid => $value) {
    $comment = _heartbeat_comment_load($cid);
    $form['comments'][$cid] = array('#type' => 'hidden', '#value' => $cid, '#prefix' => '<li>', '#suffix' => check_plain(strip_tags($comment->message)) .'</li>');
    $comment_counter++;
  }
  $form['operation'] = array('#type' => 'hidden', '#value' => 'delete');

  if (!$comment_counter) {
    drupal_set_message(t('There do not appear to be any comments to delete or your selected comment was deleted by another administrator.'));
    drupal_goto('admin/content/heartbeat/comments');
  }
  else {
    return confirm_form($form,
      t('Are you sure you want to delete these comments?'),
      'admin/content/heartbeat/comments', t('This action cannot be undone.'),
      t('Delete heartbeat comments'), t('Cancel'));
  }
}

/**
 * Process comment_multiple_delete_confirm form submissions.
 *
 * Perform the actual comment deletion.
 */
function heartbeat_comments_multiple_delete_confirm_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    foreach ($form_state['values']['comments'] as $cid => $value) {
      db_query("DELETE FROM {heartbeat_comments} WHERE hcid = %d", $cid);
    }

    drupal_set_message(t('The comments have been deleted.'));
  }
  $form_state['redirect'] = 'admin/content/heartbeat/comments';
}