<?php
/**
 * @file
 * Watcher DB layer
 * 
 * Watcher Module
 * by Jakob Persson of NodeOne <jakob@nodeone.se>
 * With ideas and feedback from Hans Dekker of Wordsy.com
 *
 * are updated or commented on.
 *
 * Sponsored by
 *   Wordsy - www.wordsy.com
 *   NodeOne - www.nodeone.se
 */

/***************************************************************************************
 * DECLARATION OF CONSTANTS
 ***************************************************************************************/

// Values for "watch_for" column in database
define('WATCHER_WATCH_FOR_ALL', 1);
define('WATCHER_WATCH_FOR_COMMENTS', 2);
define('WATCHER_WATCH_FOR_UPDATES', 3);

/***************************************************************************************
 * DATABASE FUNCTIONS
 ***************************************************************************************/

/**
 * Returns a wealth of information on the nodes a user is watching
 * Results are pageable
 *
 * @param $user
 *    User object of the user whose binder we want posts for
 * @param $header
 *    The table header array, used for table sorting
 * @return
 *    A paged database result resource
 */
function _watcher_db_get_watched_nodes_full($user = FALSE, $header = NULL) {
// Taken from tracker.module
// TODO: These queries are very expensive, see http://drupal.org/node/105639
  $result = FALSE;
  if ($user->uid) {
    $query = db_select('node', 'n')
      ->extend('PagerDefault')
      ->distinct()
      ->fields('n', array('nid', 'title', 'type', 'changed', 'uid'))
      ->fields('u', array('name'))
      ->fields('wn', array('send_email', 'added'))
      ->fields('l', array('comment_count'))
      ->limit(25);
    $query->addExpression('GREATEST(n.changed, l.last_comment_timestamp)', 'last_updated');
    $query->join('node_comment_statistics', 'l', 'n.nid = l.nid');
    $query->join('users', 'u', 'n.uid = u.uid');
    $query->join('watcher_nodes', 'wn', 'n.nid = wn.nid');
    $query->leftJoin('comment', 'c', 'n.nid = c.nid AND (c.status = :cstatus OR c.status IS NULL)', array(':cstatus' => COMMENT_PUBLISHED));
    $query->condition('n.status', 1);
    $query->condition('wn.uid', $user->uid);
    $query->addTag('node_access');
    
    if ($header) {
      $query->extend('TableSort')
        ->orderByHeader($header);
    }
    $result = $query->execute();
  }
  return $result;
}

/**
 * Returns minimal information on the nodes a user is watching
 *
 * @param $user
 *    User object of the user whose watched posts we want
 *
 * @return
 *    A database result resource
 */
function _watcher_db_get_watched_nodes_mini($user) {
  $sql = $user->uid
    ? 'SELECT DISTINCT(nid) FROM {watcher_nodes} WHERE uid = :uid_or_mail'
    : 'SELECT DISTINCT(nid) FROM {watcher_nodes} WHERE mail = :uid_or_mail';
  $result = db_query($sql, array(':uid_or_mail' => ( $user->uid ? $user->uid : $user->mail )));
  return $result;
}

/**
 * Get NIDs by UID
 * see what posts a user is watching
 *
 * @param $uid
 *     UID - a user id.
 * @return
 *     A database result set of matching table rows.
 */
function _watcher_db_get_nids_by_uid($uid) {
  $sql = 'SELECT nid FROM {watcher_nodes} WHERE uid = :uid';
  $result = db_query($sql, array(':uid' => $uid));
  return ( $result->rowCount() ? $result : FALSE );
}

/**
 * Get UIDs by NID
 * see what users are watching a post
 *
 * @param $nid
 *     NID - a node id.
 * @return
 *     A database result set of matching table rows.
 */
function _watcher_db_get_uids_by_nid($nid) {
  $sql = 'SELECT uid FROM {watcher_nodes} WHERE nid = :nid';
  $result = db_query($sql, array(':nid' => $nid));
  return ( $result->rowCount() ? $result : FALSE );
}

/**
 * Get NIDs by user
 * see what posts a user is watching
 *
 * @param $user
 *     A user object
 * @return
 *     An array of NIDs with NIDs as keys and values
 */
function _watcher_db_get_nids_by_user($user) {
  if ($user->uid) {
    $sql = 'SELECT nid FROM {watcher_nodes} WHERE uid = :uid';
    $result = db_query($sql, array(':uid' => $user->uid));
  }
  else {
    $sql = 'SELECT nid FROM {watcher_nodes} WHERE mail = :mail';
    $result = db_query($sql, array(':mail' => $user->mail));
  }
  $nids = array();
  foreach ($result as $row) {
    $nids[$row->nid] = $row->nid;
  }

  return $nids;
}

/**
 * Is user watching NID
 * check if a specific user is watching the given node
 *
 * @param $user
 *     A user object.
 * @param $nid
 *     A node NID
 *
 * @return
 *     A database result set of matching table rows.
 */
function _watcher_db_get_user_nid($user, $nid) {
  if (!$user->uid) {
    $sql = 'SELECT mail FROM {watcher_nodes} WHERE mail = :mail AND nid = :nid';
    $result = db_query($sql, array(':mail' => $user->mail, ':nid' => $nid));
  }
  else {
    $sql = 'SELECT uid FROM {watcher_nodes} WHERE uid = :uid AND nid = :nid';
    $result = db_query($sql, array(':uid' => $user->uid, ':nid' => $nid));
  }
  return ( $result->rowCount() ? $result : FALSE );
}

/**
 * Add NID to UID's watch list, enable email notifications if desired
 *
 * @param $nid
 *     The nid of the node the users is going to watch
 * @param $user
 *     A user object
 * @param $what
 *     A string indicating what an anonymous user watches for: all, comments, updates
 *
 * @return
 *     Boolean TRUE on success
 */
function _watcher_db_set_nid_user($nid, $user, $what = NULL) {

  if (!$user->uid) {
    // If the user is anonymous, we first remove any already existing rows and replace
    // them to allow the user to alter previously set watches
    db_delete('watcher_nodes')
      ->condition('nid', $nid)
      ->condition('uid', $uid)
      ->condition('mail', $mail)
      ->execute();
  }

  $res = TRUE;
  if ($what) {
    // Mapping integers to strings
    $for['all'] = WATCHER_WATCH_FOR_ALL;
    $for['comments'] = WATCHER_WATCH_FOR_COMMENTS;
    $for['updates'] = WATCHER_WATCH_FOR_UPDATES;

    try {
      db_insert('watcher_nodes')
        ->fields(array(
          'uid' => $user->uid,
          'mail' => $user->mail,
          'nid' => $nid,
          'added' => REQUEST_TIME,
          'watch_for' => $for[$what],
        ))
        ->execute();
    }
    catch (Exception $e) {
      $res = FALSE;
    }
  }
  else {
    try {
      db_insert('watcher_nodes')
        ->fields(array(
          'uid' => $user->uid,
          'nid' => $nid,
          'added' => REQUEST_TIME,
        ))
        ->execute();
    }
    catch (Exception $e) {
      $res = FALSE;
    }
  }

  return $res;
}

/**
 * Remove NIDs from the UID's watch list
 * @param $nids
 *     Array of node NIDs
 * @param $user
 *     A user object.
 *
 * @return
 *     A boolean indicating success/failure
 */
function _watcher_db_delete_nids_user($nids, $user) {
  if (!count($nids)) {
    return FALSE;
  }
  if (!$user->uid) {
    $res = db_delete('watcher_nodes')
      ->condition('mail', $user->mail)
      ->condition('nid', $nids, 'IN')
      ->execute();
  }
  else {
    $res = db_delete('watcher_nodes')
      ->condition('uid', $user->uid)
      ->condition('nid', $nids, 'IN')
      ->execute();
  }
  return (bool) $res;
}

/**
 * Delete a node from the watch list
 * @param $nid
 *    NID of node to delete
 * @return
 *    Boolean of success or failure
 */
function _watcher_db_delete_nid($nid) {
  $res = db_delete('watcher_nodes')
    ->condition('nid', $nid)
    ->execute();
  return (bool) $res;
}

/**
 * Remove all NIDs on UID's watch list
 * @param $uid
 *     UID - a user id.
 * @return
 *     A boolean indicating success/failur
 */
function _watcher_db_delete_uid($uid) {
  $res = db_delete('watcher_nodes')
    ->condition('uid', $uid)
    ->execute();
  return (bool) $res;
}

/**
 * Get email notification status given UID and NID
 *
 * @param $uid
 *     UID - a user id.
 * @param $nid
 *     NID - a node id.
 * @return
 *     Boolean True if email notifications are enabled for this user and node
 */
function _watcher_db_get_send_email_status($nid, $uid) {
  $sql = 'SELECT send_email FROM {watcher_nodes} WHERE uid = :uid AND nid = :nid';
  $result = db_query($sql, array(':uid' => $uid, ':nid' => $nid));
  $row = $result->fetchObject();
  return ($row->send_email ? TRUE : FALSE);
}

/**
 * Set email notification status given NID and UID
 *
 * @param $nid
 *     NID - a node id
 * @param $user
 *     A user object
 * @param $send_email
 *     A boolean integer, 1 or 0
 * @return
 *     Boolean denoting success/failure.
 */
function _watcher_db_set_send_email_status($nid, $user, $send_email = 0) {
  $res = TRUE;
  if ($user->uid) {
    try {
      db_update('watcher_nodes')
        ->fields(array(
          'send_email' => $send_email,
        ))
        ->condition('uid', $user->uid)
        ->condition('nid', $nid)
        ->execute();
    }
    catch (Exception $e) {
      $res = FALSE;
    }
  }
  else {
    try {
      db_update('watcher_nodes')
        ->fields(array(
          'send_email' => $send_email,
        ))
        ->condition('mail', $user->mail)
        ->condition('nid', $nid)
        ->execute();
    }
    catch (Exception $e) {
      $res = FALSE;
    }
  }
  return $res;
}

/**
 * Save user settings to database
 *
 * @param $uid
 *     A user UID
 * @param $settings
 *     An associative array of user settings
 * @return
 *     Boolean TRUE on success
 */
function _watcher_db_set_user_settings($uid, $settings) {

  //Add custom field to mark these user settings as custom
  $settings['custom'] = 1;

  // First delete existing settings
  db_delete('watcher_user_settings')
    ->condition('uid', $uid)
    ->execute();

  // Insert new settings
  $settings['uid'] = $uid;
  $res = TRUE;
  try {
    db_insert('watcher_user_settings')
      ->fields($settings)
      ->execute();
  }
  catch (Exception $e) {
    $res = FALSE;
  }
  return $res;
}

/**
 * Delete user settings for given UID
 *
 * @param $uid
 *     A user UID
 * @return
 *     Boolean True on success
 */
function _watcher_db_delete_user_settings($uid) {
  $res = TRUE;
  try {
    db_delete('watcher_user_settings')
      ->condition('uid', $uid)
      ->execute();
  }
  catch (Exception $e) {
    $res = FALSE;
  }
  return $res;
}

/**
 * Load user settings from database
 *
 * @param $uid
 *     A user UID
 * @return
 *     An assoc array of user settings
 */
function _watcher_db_get_user_settings($uid) {
  // Cache settings
  static $settings;
  if (!isset($settings)) {
    $sql = 'SELECT * FROM {watcher_user_settings} WHERE uid = :uid';
    $result = db_query($sql, array(':uid' => $uid));
    if ($result->rowCount()) {
      $settings = $result->fetchObject();
      unset($settings->uid);
    }
    else {
      $settings = FALSE;
    }
  }
  return $settings;
}

/**
 * Updates the default settings for all users who haven't customized their settings
 *
 * @param $settings
 *    An associative array of settings
 * @return
 *    A boolean TRUE on success
 */
function _watcher_db_user_settings_update_defaults($settings) {
  // Users may have registered and not had settings set yet so add rows for any users that are "missing"
  $query = db_select('users', 'u');
  $query->leftJoin('watcher_user_settings', 'wus', 'u.uid = wus.uid');
  $query->addField('u', 'uid');
  $query->isNull('custom');
  $res = TRUE;
  try {
    db_insert('watcher_user_settings')
      ->from($query)
      ->execute();
  }
  catch (Exception $e) {
    $res = FALSE;
  }

  // If defaults are set, apply them, otherwise return
  if (is_array($settings)) {
    // Update all users who haven't customized their settings yet
    try {
      db_update('watcher_user_settings')
        ->fields($settings)
        ->condition('custom', '1', '<>')
        ->execute();
    }
    catch (Exception $e) {
      $res = FALSE;
    }
  }
  return $res;
}

/**
 * Has UID customized his/her user settings?
 *
 * @param $uid
 *    A user UID
 * @return
 *    A boolean TRUE if given UID has customized his/her user settings
 */
function _watcher_db_user_settings_uid_custom($uid) {
  $sql = 'SELECT custom FROM {watcher_user_settings} WHERE uid = :uid AND custom = 1';
  $result = db_query($sql, array(':uid' => $uid));
  return ( (bool) $result->rowCount() );
}

/**
 * Return a list of users that want to get notified about node updates for given NID
 *
 * @param $nid
 *     The NID of the node that has been updated
 * @param $uid_exclude
 *     A UID we want to exclude from our set of users, normally the user that updated the node.
 * @return
 *     An array of objects with uid as key representing users, with attributes uid, mail and name
 */
function _watcher_db_get_users_notify_node_update($nid, $uid_exclude) {
  $sql = "SELECT wn.uid, wn.mail wnmail, u.mail umail, u.name FROM {watcher_nodes} wn LEFT JOIN {watcher_user_settings} wus ON wus.uid = wn.uid LEFT JOIN {users} u ON u.uid = wus.uid WHERE nid = :nid AND ((wn.uid <> 0 AND wn.uid <> :uid_exclude AND notifications_updates = 1 AND send_email = 1) OR (wn.uid = 0 AND (watch_for = 1 OR watch_for = 3)))";
  $res = db_query($sql, array(':nid' => $nid, ':uid_exclude' => $uid_exclude));
  $users = array();
  foreach ($res as $row) {
    $users[$row->uid] = $row;
  }
  return $users;
}

/**
 * Return a list of users that want to get notified about new comments for given NID
 *
 * @param $nid
 *     The nid of the node the comment belongs to
 * @param $uid_exclude
 *     A uid we want to exclude from our set of users, normally the user that wrote the comment,
 *     what's the point in being notified about your own comments, eh?
 *
 * @return
 *     An array of objects with uid as key representing users, with attributes uid, mail and name
 */
function _watcher_db_get_users_notify_comment_insert($nid, $uid_exclude) {
  $sql = 'SELECT wn.uid, wn.mail wnmail, u.mail umail, u.name FROM {watcher_nodes} wn LEFT JOIN {watcher_user_settings} wus ON wus.uid = wn.uid LEFT JOIN {users} u ON u.uid = wus.uid WHERE nid = :nid AND ((wn.uid <> 0 AND wn.uid <> :uid_exclude AND notifications_new_comments = 1 AND send_email = 1) OR (wn.uid = 0 AND (watch_for = 1 OR watch_for = 2)))';
  $res = db_query($sql, array(':nid' => $nid, ':uid_exclude' => $uid_exclude));
  return $res->fetchAll();
}

/**
 * Return a list of users that want to get notified about node updates or new comments
 * for a given NID
 *
 * @param $nid
 *      The nid of the node that has been updated
 * @param $uid_exclude
 *      A uid we want to exclude from our set of users, normally the user that updated the node.
 *
 * @return
 *      An array of objects with uid as key representing users, with attributes uid, mail and name
 */
function _watcher_db_get_users_notify_about_node($nid, $uid_exclude) {
  $sql = 'SELECT wn.uid, wn.mail wnmail, u.mail umail, u.name FROM {watcher_nodes} wn LEFT JOIN {users} u ON u.uid = wn.uid WHERE nid = :nid AND ((wn.uid <> 0 AND wn.uid <> :uid_exclude AND send_email = 1) OR (NOT ISNULL(watch_for)))';
  $res = db_query($sql, array(':nid' => $nid, ':uid_exclude' => $uid_exclude));
  $users = array();
  foreach ($res as $row) {
    $users[$row->uid] = $row;
  }
  return $users;
}

/**
 * Add a message to the notification queue table
 *
 * @param $qid
 *     A qid (table pk) generated by db_next_id()
 * @param $r_uid
 *     Recipient uid
 * @param $r_email
 *     Recipient email address
 * @param $subject
 *     The message subject
 * @param $message
 *     Message to be sent
 * @return
 *     A database result resource
 */
function _watcher_db_insert_users_notification_queue($r_uid, $r_email, $subject, $message) {
  $res = TRUE;
  try {
    db_insert('watcher_notify_queue')
      ->fields(array(
        'uid' => $r_uid,
        'mail' => $r_email,
        'subject' => $subject,
        'message' => $message,
      ))
      ->execute();
  }
  catch (Exception $e) {
    $res = FALSE;
  }
  return $res;
}

/**
 * Delete all rows in notification queue table
 *
 * @return
 *    A boolean denoting whether any rows were deleted, FALSE if none, TRUE if one or more
 */
function _watcher_db_delete_users_notification_queue_all() {
  $res = db_delete('watcher_notify_queue')->execute();
  return (bool) $res;
}

/**
 * Get messages from the notification queue table
 */
function _watcher_db_get_users_notification_queue() {
  $qmsgs = db_select('watcher_notify_queue')
    ->fields('watcher_notify_queue')
    ->execute()
    ->fetchAll();
  return $qmsgs;
}

/**
 * Delete a set of rows from the notification queue table
 *
 * @param $rows
 *     An array of qids (queue id)
 */
function _watcher_db_delete_users_notification_queue($qids) {

  // Database managers can only handle *finite* (gulp!) SQL queries
  // so we gotta limit the fun
  $limit = 4096; //2^12 elements

  $offset = 0;
  for ($i = 1; $offset < count($qids); $i++) {
    $batch = array_slice($qids, $offset, $limit);
    db_delete('watcher_notify_queue')
      ->condition('qid', $batch, 'IN')
      ->execute();
    $offset = (($i * $limit) - 1);
  }
}

/**
 * Statistics for Notification Queue Table
 *
 * @return
 *    An assoc array with keys num_rows and num_sent
 */
function _watcher_db_stats_notification_queue() {
  $sql = 'SELECT * FROM {watcher_notify_queue}';
  $sql2 = 'SELECT MAX(qid) FROM {watcher_notify_queue}';
  $sql3 = 'SELECT COUNT(*) FROM {watcher_notify_queue}';

  // Obtain number of rows
  $res = db_query($sql3);
  $stat['num_rows'] = $res->fetchField();

  // Obtain the contents of the queue table
  $res = db_query($sql);
  foreach ($res as $row) {

    // Load fields
    $data['qid'] = $row->qid;
    $data['uid'] = $row->uid;
    $data['mail'] = $row->mail;
    $data['subject'] = $row->subject;
    $data['message'] = '<pre>'. $row->message .'</pre>';

    // Add to data array
    $stat['data']['content'][] = $data;
  }

  // Add header
  $stat['data']['header'] = array('qid', 'uid', 'mail', 'subject', 'message');

  // Obtain the number of notifications sent - this is not 100% accurate since we cannot
  // entirely trust the increment counter
  $res2 = db_query($sql2);
  $num_sent = $res2->fetchField();
  $stat['num_sent'] = ( $num_sent = $res2->fetchField() ? $num_sent : 0 );

  return $res ? $stat : FALSE;
}

/**
 * Statistics for Watched Nodes Table
 * Numbers of users that watch nodes
 *
 * @return
 *    An assoc array with key num_users
 */
function _watcher_db_stats_watched_nodes_users() {
  $sql = 'SELECT COUNT(DISTINCT(uid)) FROM {watcher_nodes} WHERE uid <> 0';
  $sql2 = 'SELECT COUNT(DISTINCT(mail)) FROM {watcher_nodes} WHERE uid = 0';

  // Obtain number of rows
  $res = db_query($sql);
  $stat['num_users_reg'] = $res->fetchField();

  $res2 = db_query($sql2);
  $stat['num_users_anon'] = $res2->fetchField();

  return ($res && $res2) ? $stat : FALSE;
}

/**
 * Statistics for User Settings Table
 *
 * @return
 *    An assoc array with key num_users_with_settings, num_users and num_users_no_settings
 */
function _watcher_db_stats_user_settings() {

  // Users with custom settings
  $sql = 'SELECT COUNT(uid) FROM {watcher_user_settings} WHERE custom = 1';

  // Total number of users on the site
  $sql2 = 'SELECT COUNT(uid) FROM {users} WHERE uid <> 0';

  // Obtain number of users with settings
  $res = db_query($sql);
  $stat['num_users_with_settings'] = $res->fetchField();

  // Obtain number of users without setting
  $res2 = db_query($sql2);
  $stat['num_users'] = $res2->fetchField();
  $stat['num_users_no_settings'] = $stat['num_users'] - $stat['num_users_with_settings'];

  return $res ? $stat : FALSE;
}

/**
 * Return the newest node
 */
function _watcher_db_get_newest_node() {
  $sql = 'SELECT MAX(nid) nid FROM {node}';
  $nid = db_query($sql)->fetchField();
  return $nid ? $nid : FALSE;
}

/**
 * Return the newest comment
 */
function _watcher_db_get_newest_comment() {
  $sql = 'SELECT MAX(cid) cid FROM {comment}';
  $cid = db_query($sql)->fetchField();
  return $cid ? $cid : FALSE;
}
