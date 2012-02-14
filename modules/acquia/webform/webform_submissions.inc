<?php
// $Id: webform_submissions.inc,v 1.6.2.27 2010/06/18 01:31:32 quicksketch Exp $

/**
 * @file
 * This file is loaded when handling submissions, either submitting new,
 * editing, or viewing. It also contains all CRUD functions for submissions.
 *
 * @author Nathan Haug <nate@lullabot.com>
 */

function webform_submission_update($node, $sid, $submitted) {
  // Update the submission data by first removing all this submissions data.
  db_query('DELETE FROM {webform_submitted_data} WHERE sid = %d', $sid);
  // Then re-add it to the database.
  foreach ($submitted as $cid => $value) {
    // Don't save pagebreaks as submitted data.
    if ($node->webform['components'][$cid]['type'] == 'pagebreak') {
      continue;
    }

    if (is_array($value)) {
      $delta = 0;
      foreach ($value as $k => $v) {
        db_query("INSERT INTO {webform_submitted_data} (nid, sid, cid, no, data) VALUES (%d, %d, %d, %d, '%s')", $node->nid, $sid, $cid, $delta, $v);
        $delta++;
      }
    }
    else {
      db_query("INSERT INTO {webform_submitted_data} (nid, sid, cid, no, data) VALUES (%d, %d, %d, %d, '%s')", $node->nid, $sid, $cid, 0, $value);
    }
  }

  return $sid;
}

function webform_submission_insert($node, $submitted) {
  global $user;

  $result = db_query("INSERT INTO {webform_submissions} (nid, uid, submitted, remote_addr) VALUES (%d, %d, %d, '%s')", $node->nid, $user->uid, time(), ip_address());

  $sid = db_last_insert_id('webform_submissions', 'sid');

  foreach ($submitted as $cid => $value) {
    // Don't save pagebreaks as submitted data.
    if ($node->webform['components'][$cid]['type'] == 'pagebreak') {
      continue;
    }

    if (is_array($value)) {
      $delta = 0;
      foreach ($value as $k => $v) {
        db_query("INSERT INTO {webform_submitted_data} (nid, sid, cid, no, data) VALUES (%d, %d, %d, %d, '%s')", $node->nid, $sid, $cid, $delta, $v);
        $delta++;
      }
    }
    else {
      db_query("INSERT INTO {webform_submitted_data} (nid, sid, cid, no, data) VALUES (%d, %d, %d, %d, '%s')", $node->nid, $sid, $cid, 0, $value);
    }
  }

  return $sid;
}

/**
 * Delete a single submission.
 *
 * @param $nid
 *   ID of node for which this webform was submitted.
 * @param $sid
 *   ID of submission to be deleted (from webform_submitted_data).
 */
function webform_submission_delete($node, $submission) {
  // Iterate through all components and let each do cleanup if necessary.
  webform_load_components();
  foreach ($node->webform['components'] as $cid => $component) {
    $delete_function = '_webform_delete_'. $component['type'];
    if (function_exists($delete_function) && isset($submission->data[$cid])) {
      $delete_function($submission->data[$cid], $component);
    }
  }

  db_query('DELETE FROM {webform_submitted_data} WHERE nid = %d AND sid = %d', $node->nid, $submission->sid);
  db_query('DELETE FROM {webform_submissions} WHERE nid = %d AND sid = %d', $node->nid, $submission->sid);
}

/**
 * Confirm form to delete a single form submission.
 *
 * @param $form_state
 *   The current form state.
 * @param $node
 *   The node for which this webform was submitted.
 * @param $submission
 *   The submission to be deleted (from webform_submitted_data).
 */
function webform_submission_delete_form($form_state, $node, $submission) {
  drupal_set_title(t('Delete Form Submission'));

  $form = array();
  $form['node'] = array('#type' => 'value', '#value' => $node);
  $form['submission'] = array('#type' => 'value', '#value' => $submission);
  $question = t('Are you sure you want to delete this submission?');

  if (isset($_GET['destination'])) {
    $destination = $_GET['destination'];
  }
  elseif (user_access('access webform results')) {
    $destination = 'node/'. $node->nid .'/webform-results';
  }
  else {
    $destination = 'node/'. $node->nid .'/submissions';
  }

  return confirm_form($form, $question, $destination, NULL, t('Delete'), t('Cancel'));
}

function webform_submission_delete_form_submit($form, &$form_state) {
  webform_submission_delete($form_state['values']['node'], $form_state['values']['submission']);
  drupal_set_message(t('Submission deleted.'));

  $form_state['redirect'] = 'node/'. $form_state['values']['node']->nid .'/webform-results';
}

/**
 * Return all the submissions for a particular node.
 *
 * @param $nid
 *   The node ID for which submissions are being fetched.
 * @param $header
 *   If the results of this fetch will be used in a sortable
 *   table, pass the array header of the table.
 * @param $uid
 *   Optional; the user ID to filter the submissions by.
 * @return $submissions
 *   An array of submissions matching your filters.
 */
function webform_get_submissions($nid, $header = NULL, $uid = NULL, $pager_count = 0) {
  $sids = NULL;
  if ($pager_count) {
    $pager_query = 'SELECT * FROM {webform_submissions} WHERE nid = %d';
    if ($uid) {
      $pager_query .= ' AND uid = %d';
    }
    $pager_query .= is_array($header) ? tablesort_sql($header) : ' ORDER BY sid ASC';

    $res = pager_query($pager_query, $pager_count, 0, NULL, $nid, $uid);
    $sids = array();
    while ($row = db_fetch_object($res)) {
      $sids[] = $row->sid;
    }
    $sids = implode($sids, ',');
  }

  $query = 'SELECT s.*, sd.cid, sd.no, sd.data, u.name, u.mail, u.status '.
           'FROM {webform_submissions} s '.
           'LEFT JOIN {webform_submitted_data} sd ON sd.sid = s.sid '.
           'LEFT JOIN {users} u ON u.uid = s.uid '.
           'WHERE sd.nid = %d';

  if ($pager_count && !empty($sids)) {
    $query .= ' AND s.sid IN (%s)';
  }

  if ($uid) {
    $query .= ' AND u.uid = %d';
  }

  if (is_array($header)) {
    $query .= tablesort_sql($header);
    if (!isset($header[0]['sort'])) {
      $query .= ', sid ASC';
    }
    $query .= ', cid ASC, no ASC';
  }
  else {
    $query .= ' ORDER BY sid ASC, cid ASC, no ASC';
  }

  $res = $pager_count ? db_query($query, $nid, $sids, $uid) : db_query($query, $nid, $uid);
  $submissions = array();
  $previous = array();

  // Outer loop: iterate for each submission.
  while ($row = db_fetch_object($res)) {
    if ($row->sid != $previous) {
      $submissions[$row->sid] = new stdClass();
      $submissions[$row->sid]->sid = $row->sid;
      $submissions[$row->sid]->submitted = $row->submitted;
      $submissions[$row->sid]->remote_addr = $row->remote_addr;
      $submissions[$row->sid]->uid = $row->uid;
      $submissions[$row->sid]->name = $row->name;
      $submissions[$row->sid]->status = $row->status;
    }
    $submissions[$row->sid]->data[$row->cid]['value'][$row->no] = $row->data;
    $previous = $row->sid;
  }

  return $submissions;
}

/**
 * Return a count of the total number of submissions for a node.
 *
 * @param $nid
 *   The node ID for which submissions are being fetched.
 * @param $uid
 *   Optional; the user ID to filter the submissions by.
 * @return
 *   An integer value of the number of submissions.
 */
function webform_get_submission_count($nid, $uid = NULL) {
  $query = 'SELECT count(*) FROM {webform_submissions} WHERE nid = %d';
  if ($uid) {
    $query .= ' AND uid = %d';
  }
  return db_result(db_query($query, $nid, $uid));
}

/**
 * Fetch a specified submission for a webform node.
 */
function webform_get_submission($nid, $sid, $reset = FALSE) {
  static $submissions = array();

  if (!isset($submissions[$nid][$sid]) || $reset) {
    $query = 'SELECT s.*, sd.cid, sd.no, sd.data '.
             'FROM {webform_submitted_data} as sd '.
             'LEFT JOIN {webform_submissions} as s on (sd.sid = s.sid) '.
             'WHERE sd.sid = %d AND s.nid = %d';

    $result = db_query($query, $sid, $nid);
    if ($row = db_fetch_object($result)) {
      $submission = new stdClass;
      $submission->nid = $row->nid;
      $submission->sid = $row->sid;
      $submission->uid = $row->uid;
      $submission->remote_addr = $row->remote_addr;
      $submission->submitted = $row->submitted;

      while ($row) {
        $submission->data[$row->cid]['value'][$row->no] = $row->data;
        $row = db_fetch_object($result);
      }
    }
    $submissions[$nid][$sid] = isset($submission) ? $submission : NULL;
  }

  return $submissions[$nid][$sid];
}


function _webform_submission_spam_check($to, $subject, $from, $headers = array()) {
  $headers = implode('\n', (array)$headers);
  // Check if they are attempting to spam using a bcc or content type hack.
  if (preg_match('/(b?cc\s?:)|(content\-type:)/i', $to ."\n". $subject ."\n". $from ."\n". $headers)) {
    return TRUE; // Possible spam attempt.
  }
  return FALSE; // Not spam.
}

/**
 * Check if the current user has exceeded the limit on this form.
 *
 * @param $node
 *   The webform node to be checked.
 * @return
 *   Boolean TRUE if the user has exceeded their limit. FALSE otherwise.
 */
function _webform_submission_limit_check($node) {
  global $user;

  // Check if submission limiting is enabled.
  if ($node->webform['submit_limit'] == '-1') {
    return FALSE; // No check enabled.
  }

  // Retrieve submission data for this IP address or username from the database.
  $query = 'SELECT count(*) '.
           'FROM {webform_submissions} '.
           "WHERE (( 0 = %d AND remote_addr = '%s') OR (uid > 0 AND uid = %d)) ".
           'AND submitted > %d AND nid = %d';

  // Fetch all the entries from the database within the submit interval with this username and IP.
  $num_submissions_database = db_result(db_query($query, $user->uid, ip_address(), $user->uid, ($node->webform['submit_interval'] != -1) ? (time() - $node->webform['submit_interval']) : $node->webform['submit_interval'], $node->nid));

  // Double check the submission history from the users machine using cookies.
  $num_submissions_cookie = 0;
  if ($user->uid == 0 && variable_get('webform_use_cookies', 0)) {
    $cookie_name = 'webform-'. $node->nid;

    if (isset($_COOKIE[$cookie_name]) && is_array($_COOKIE[$cookie_name])) {
      foreach ($_COOKIE[$cookie_name] as $key => $timestamp) {
        if ($node->webform['submit_interval'] != -1 && $timestamp <= time() - $node->webform['submit_interval']) {
          // Remove the cookie if past the required time interval.
          setcookie($cookie_name .'['. $key .']', '', 0);
        }
      }
      // Count the number of submissions recorded in cookies.
      $num_submissions_cookie = count($_COOKIE[$cookie_name]);
    }
    else {
      $num_submissions_cookie = 0;
    }
  }

  if ($num_submissions_database >= $node->webform['submit_limit'] || $num_submissions_cookie >= $node->webform['submit_limit']) {
    // Limit exceeded.
    return TRUE;
  }

  // Limit not exceeded.
  return FALSE;
}
