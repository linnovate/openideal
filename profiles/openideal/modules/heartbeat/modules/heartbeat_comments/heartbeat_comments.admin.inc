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

  // Build the sortable table header.
  $header = array(
    'comment' => array('data' => t('Comment'), 'field' => 'subject'),
    'author' => t('Author'),
    'time' => array('data' => t('Time'), 'field' => 'c.time', 'sort' => 'desc'),
    'reacted_on' => array('data' => t('Reacted on'), 'field' => 'heartbeat_title')
  );

  $query = db_select('heartbeat_comments', 'c')->extend('PagerDefault')->extend('TableSort');
  $query->fields('c', array('hcid', 'uid', 'uaid', 'message', 'cleared', 'time'));
  $query->join('users', 'u', 'u.uid = c.uid');
  $query->fields('u', array('name'));
  $query->limit(50);
  $query->orderByHeader($header);
  $heartbeat_comments = $query->execute();

  // build a table listing the appropriate comments
  $destination = drupal_get_destination();

  foreach ($heartbeat_comments as $heartbeat_comment) {
    $options[$heartbeat_comment->hcid] = array(
      'comment' => truncate_utf8(strip_tags($heartbeat_comment->message), 70),
      'author' => $heartbeat_comment->uid ? $heartbeat_comment->name : variable_get('anonymous', 'Anonymous user'),
      'time' => format_date($heartbeat_comment->time, 'short'),
      'reacted_on' => array(
        'data' => array(
          '#type' => 'link',
          '#title' => 'activity',
          '#href' => 'heartbeat/message/' . $heartbeat_comment->uaid,
        ),
      ),
    );
  }

  $form['heartbeat_comments'] = array(
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => isset($options) ? $options : array(),
    '#empty' => t('No content available.'),
  );

  $form['pager'] = array('#markup' => theme('pager'));

  return $form;

}

/**
 * Validate heartbeat_comments_admin_overview form submissions.
 *
 * We can't execute any 'Update options' if no comments were selected.
 */
function heartbeat_comments_admin_overview_validate($form, &$form_state) {
  $form_state['values']['heartbeat_comments'] = array_diff($form_state['values']['heartbeat_comments'], array(0));
  if (count($form_state['values']['heartbeat_comments']) == 0) {
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
    $cids = array();
    foreach ($form_state['values']['heartbeat_comments'] as $cid => $value) {
      if ($value) {
        _heartbeat_comments_delete($cid);
        // Add an entry to the watchdog log.
        watchdog('content', 'Heartbeat comment: deleted %id.', array('%id' => $cid), WATCHDOG_NOTICE);
      }
    }

    drupal_set_message(t('The update has been performed.'));
    $form_state['redirect'] = 'admin/content/heartbeat/comments';
  }
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
  return db_select('heartbeat_comments', 'hc')->fields('hc')->condition('hc.hcid', $cid)->execute();
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
      'admin/content/heartbeat/comments',
      t('This action cannot be undone.'),
      t('Delete heartbeat comments'),
      t('Cancel'));
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
      _heartbeat_comments_delete($cid);
    }
    drupal_set_message(t('The comments have been deleted.'));
  }
  $form_state['redirect'] = 'admin/content/heartbeat/comments';
}