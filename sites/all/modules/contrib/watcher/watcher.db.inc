<?php
// $Id: watcher.db.inc,v 1.1.2.1.2.10 2009/07/10 15:58:06 solipsist Exp $
/**
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
function _watcher_db_get_watched_nodes_full($user = false, $header = null) {
// Taken from tracker.module
// TODO: These queries are very expensive, see http://drupal.org/node/105639
  if ($user->uid) {
    $sql = 'SELECT DISTINCT(n.nid), n.title, n.type, n.changed, n.uid, u.name, wn.send_email, wn.added, GREATEST(n.changed, l.last_comment_timestamp) AS last_updated, l.comment_count FROM {node} n INNER JOIN {node_comment_statistics} l ON n.nid = l.nid INNER JOIN {users} u ON n.uid = u.uid INNER JOIN {watcher_nodes} wn ON n.nid = wn.nid LEFT JOIN {comments} c ON n.nid = c.nid AND (c.status = %d OR c.status IS NULL) WHERE n.status = 1 AND wn.uid = %d';
    $sql_count = 'SELECT COUNT(DISTINCT(n.nid)) FROM {node} n LEFT JOIN {comments} c ON n.nid = c.nid AND (c.status = %d OR c.status IS NULL) INNER JOIN {watcher_nodes} wn ON n.nid = wn.nid WHERE n.status = 1 AND wn.uid = %d AND wn.uid = %d';
    if ($header) {
      $sql .= tablesort_sql($header);
    }
    $sql = db_rewrite_sql($sql);
    $sql_count = db_rewrite_sql($sql_count);
    $result = pager_query($sql, 25, 0, $sql_count, COMMENT_PUBLISHED, $user->uid, $user->uid, $user->uid);
    return $result;
  }
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
  if ($user->uid) {
    $sql = 'SELECT DISTINCT(nid) FROM {watcher_nodes} WHERE uid = %d';
  }
  else {
    $sql = "SELECT DISTINCT(nid) FROM {watcher_nodes} WHERE mail = '%s'";
  }
  $result = db_query($sql, ( $user->uid ? $user->uid : $user->mail ));
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
  $sql = 'SELECT nid FROM {watcher_nodes} WHERE uid = %d';
  $result = db_query($sql, $uid);
  while($row = db_fetch_object($result)) {
    return $result;
  }
  return false;
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
  $sql = 'SELECT uid FROM {watcher_nodes} WHERE nid = %d';
  $result = db_query($sql, $nid);
  while ($row = db_fetch_object($result)) {
    return $result;
  }
  return false;
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
    $sql = 'SELECT nid FROM {watcher_nodes} WHERE uid = %d';
    $result = db_query($sql, $user->uid);
  }
  else {
    $sql = "SELECT nid FROM {watcher_nodes} WHERE mail = '%s'";
    $result = db_query($sql, $user->mail);
  }

  while ($row = db_fetch_object($result)) {
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
    $sql = "SELECT mail FROM {watcher_nodes} WHERE mail = '%s' AND nid = %d";
    $result = db_query($sql, $user->mail, $nid);
  }
  else {
  $sql = 'SELECT uid FROM {watcher_nodes} WHERE uid = %d AND nid = %d';
    $result = db_query($sql, $user->uid, $nid);
  }

  while($row = db_fetch_object($result)) {
    return $result;
  }
  return false;
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
 *     Boolean true on success
 */
function _watcher_db_set_nid_user($nid, $user, $what = null) {

  if (!$user->uid) {
    // If the user is anonymous, we first remove any already existing rows and replace
    // them to allow the user to alter previously set watches
    $sql = "DELETE FROM {watcher_nodes} WHERE nid = %d AND uid = %d AND mail = '%s'";
    db_query($sql, $nid, $user->uid, $user->mail);
  }

  if ($what) {
    // Mapping integers to strings
    $for['all'] = WATCHER_WATCH_FOR_ALL;
    $for['comments'] = WATCHER_WATCH_FOR_COMMENTS;
    $for['updates'] = WATCHER_WATCH_FOR_UPDATES;

    $sql = "INSERT INTO {watcher_nodes} (uid, mail, nid, added, watch_for) VALUES(%d, '%s', %d, %d, %d)";
    $res = db_query($sql, $user->uid, $user->mail, $nid, time(), $for[$what]);
  }
  else {
    $sql = "INSERT INTO {watcher_nodes} (uid, nid, added) VALUES(%d, %d, %d)";
    $res = db_query($sql, $user->uid, $nid, time());
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
    return;
  }
  $sql_in = implode(',', $nids);
  if (!$user->uid) {
    $sql = "DELETE FROM {watcher_nodes} WHERE mail = '%s' AND nid IN(%s)";
    $res = db_query($sql, $user->mail, $sql_in);
  }
  else {
    $sql = 'DELETE FROM {watcher_nodes} WHERE uid = %d AND nid IN(%s)';
    $res = db_query($sql, $user->uid, $sql_in);
  }
  return (bool) db_affected_rows();
}

/**
 * Delete a node from the watch list
 * @param $nid
 *    NID of node to delete
 * @return
 *    Boolean of success or failure
 */
function _watcher_db_delete_nid($nid) {
  $sql = 'DELETE FROM {watcher_nodes} WHERE nid = %d';
  $res = db_query($sql, $nid);
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
  $sql = 'DELETE FROM {watcher_nodes} WHERE uid = %d';
  $res = db_query($sql, $uid);
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
  $sql = 'SELECT send_email FROM {watcher_nodes} WHERE uid = %d AND nid = %d';
  $result = db_query($sql, $uid, $nid);
  $row = db_fetch_object($result);
  if ($row->send_email) return true;
  return false;
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
  if ($user->uid) {
    $sql = 'UPDATE {watcher_nodes} SET send_email = %d WHERE uid = %d AND nid = %d';
    $res = db_query($sql, $send_email, $user->uid, $nid);
  }
  else {
    $sql = "UPDATE {watcher_nodes} SET send_email = %d WHERE mail = '%s' AND nid = %d";
    $res = db_query($sql, $send_email, $user->mail, $nid);
  }
  return ((bool) $res);
}

/**
 * Save user settings to database
 *
 * @param $uid
 *     A user UID
 * @param $settings
 *     An associative array of user settings
 * @return
 *     Boolean true on success
 */
function _watcher_db_set_user_settings($uid, $settings) {

  //Add custom field to mark these user settings as custom
  $settings['custom'] = 1;

  //Serialize settings
  $fields = implode(', ', array_keys($settings));
  $values = implode(', ', array_values($settings));

  // First delete existing settings
  $sql = 'DELETE FROM {watcher_user_settings} WHERE uid = %d';
  db_query($sql, $uid);

  // Insert new settings
  $sql = 'INSERT INTO {watcher_user_settings} (uid, %s) VALUES (%d, %s)';
  $res = db_query($sql, $fields, $uid, $values);
  return (bool) $res;
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
  $sql = 'DELETE FROM {watcher_user_settings} WHERE uid = %d';
  $res = db_query($sql, $uid);
  return (bool) $res;
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
    $sql = 'SELECT * FROM {watcher_user_settings} WHERE uid = %d';
    $result = db_query($sql, $uid);
    while($row = db_fetch_array($result)) {
      unset($row['uid']);
      $settings = $row;
      return $settings;
    }
    $settings = false;
  }
  return $settings;
}

/**
 * Updates the default settings for all users who haven't customized their settings
 *
 * @param $settings
 *    An associative array of settings
 * @return
 *    A boolean true on success
 */
function _watcher_db_user_settings_update_defaults($settings) {
  // Users may have registered and not had settings set yet so add rows for any users that are "missing"
  $sql = 'INSERT INTO {watcher_user_settings} (uid) SELECT u.uid FROM {users} u LEFT JOIN {watcher_user_settings} wus ON u.uid = wus.uid WHERE ISNULL(custom)';
  $res = db_query($sql);

  // If defaults are set, apply them, otherwise return
  if (is_array($settings)) {
    // Format settings array as string
    foreach ($settings as $setting => $value) {
      $insert_array[] = $setting .'='. $value;
    }
    $insert_string = implode(', ', $insert_array);

    // Update all users who haven't customized their settings yet
    $sql = 'UPDATE {watcher_user_settings} SET %s WHERE custom <> 1';
    $res = db_query($sql, $insert_string);
  }
  return (bool) $res;
}

/**
 * Has UID customized his/her user settings?
 *
 * @param $uid
 *    A user UID
 * @return
 *    A boolean true if given UID has customized his/her user settings
 */
function _watcher_db_user_settings_uid_custom($uid) {
  $sql = 'SELECT custom FROM {watcher_user_settings} WHERE uid = %d AND custom = 1';
  $res = db_query($sql, $uid);
  while($row = db_fetch_object($res)) {
    return true;
  }
  return false;
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
  $sql = "SELECT wn.uid, wn.mail wnmail, u.mail umail, u.name FROM {watcher_nodes} wn LEFT JOIN {watcher_user_settings} wus ON wus.uid = wn.uid LEFT JOIN {users} u ON u.uid = wus.uid WHERE nid = %d AND ((wn.uid <> 0 AND wn.uid <> %d AND notifications_updates = 1 AND send_email = 1) OR (wn.uid = 0 AND (watch_for = 1 OR watch_for = 3)))";
  $res = db_query($sql, $nid, $uid_exclude);
  $users = array();
  while ($row = db_fetch_object($res)) {
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
  $sql = 'SELECT wn.uid, wn.mail wnmail, u.mail umail, u.name FROM {watcher_nodes} wn LEFT JOIN {watcher_user_settings} wus ON wus.uid = wn.uid LEFT JOIN {users} u ON u.uid = wus.uid WHERE nid = %d AND ((wn.uid <> 0 AND wn.uid <> %d AND notifications_new_comments = 1 AND send_email = 1) OR (wn.uid = 0 AND (watch_for = 1 OR watch_for = 2)))';
  $res = db_query($sql, $nid, $uid_exclude);
  $users = array();
  while ($row = db_fetch_object($res)) {
    $users[] = $row;
  }
  return $users;
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
  $sql = 'SELECT wn.uid, wn.mail wnmail, u.mail umail, u.name FROM {watcher_nodes} wn LEFT JOIN {users} u ON u.uid = wn.uid WHERE nid = %d AND ((wn.uid <> 0 AND wn.uid <> %d AND send_email = 1) OR (NOT ISNULL(watch_for)))';
  $res = db_query($sql, $nid, $uid_exclude);
  $users = array();
  while ($row = db_fetch_object($res)) {
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
  $sql = "INSERT INTO {watcher_notify_queue} (uid, mail, subject, message) VALUES (%d, '%s', '%s', '%s')";
  $res = db_query($sql, $r_uid, $r_email, $subject, $message);
  return (bool) $res;
}

/**
 * Delete all rows in notification queue table
 *
 * @return
 *    A boolean denoting whether any rows were deleted, false if none, true if one or more
 */
function _watcher_db_delete_users_notification_queue_all() {
  $sql = "DELETE FROM {watcher_notify_queue}";
  $res = db_query($sql);
  return (bool) $res;
}

/**
 * Get messages from the notification queue table
 */
function _watcher_db_get_users_notification_queue() {
  $sql = "SELECT * FROM {watcher_notify_queue}";
  $res = db_query($sql);
  while ($qmsg = db_fetch_object($res)) {
    $qmsgs[] = $qmsg;
  }
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
    $in = implode(',', $batch);
    $sql = "DELETE FROM {watcher_notify_queue} WHERE qid IN (%s)";
    db_query($sql, $in);
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
  $stat['num_rows'] = db_result($res);

  // Obtain the contents of the queue table
  $res = db_query($sql);
  while ($row = db_fetch_object($res)) {

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
  $num_sent = db_result($res2);
  $stat['num_sent'] = ( $num_sent = db_result($res2) ? $num_sent : 0 );

  if ($res) {
    return $stat;
  }
  return false;
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
  $stat['num_users_reg'] = db_result($res);

  $res2 = db_query($sql2);
  $stat['num_users_anon'] = db_result($res2);


  if ($res && $res2) return $stat;
  return false;
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
  $stat['num_users_with_settings'] = db_result($res);

  // Obtain number of users without setting
  $res2 = db_query($sql2);
  $stat['num_users'] = db_result($res2);
  $stat['num_users_no_settings'] = $stat['num_users'] - $stat['num_users_with_settings'];

  if ($res) return $stat;
  return false;
}

/**
 * Return the newest node
 */
function _watcher_db_get_newest_node() {
  $sql = 'SELECT nid FROM {node} ORDER BY created DESC LIMIT 0,1';
  $res = db_query($sql);
  if ($nid = db_result($res)) {
    return $nid;
  }
  return false;
}

/**
 * Return the newest comment
 */
function _watcher_db_get_newest_comment() {
  $sql = 'SELECT cid FROM {comments} ORDER BY timestamp DESC LIMIT 0,1';
  $res = db_query($sql);
  if ($cid = db_result($res)) {
    return $cid;
  }
  return false;
}
