<?php
// $Id: heartbeatparser.inc,v 1.1.2.3.2.28 2010/10/16 09:16:35 stalski Exp $

/**
 * @file
 *   HeartbeatParser object
 *   Parses raw activity messages into a stream.
 *
 */

/**
 * Class heartbeatParser
 *
 */
class heartbeatParser {

  private $_candidates = array();
  private $_messages = array();
  private $_sets = array();
  private $_timespan_gap = 0;
  private $_allowed_tags = array('p', 'span', 'a', 'em', 'strong', 'u', 'i', 'blockquote');
  public $raw_messages = array();

  private function __construct() {
    $this->_allowed_tags = heartbeat_allowed_tags();
  }

  /**
   * Function to remove duplicate messages
   * Duplicate messages whish are exactly the same as already build messages,
   *   delete them and increment the count of the message instance
   * Duplicate reversive messages that occur at the same server request
   *   time are deleted as well
   */
  private function remove_message_duplicates($message_set) {

    global $user;

    // hash holding all unique values
    // if this would be static ...you could do it for all timespan-sets
    $holder = array();
    $duplicates = array();
    $users_relations = array();
    foreach ($message_set as $key => $message) {

      // if an exact identical message occurs > twice contextual wise,
      // then skip it with an incrementation of the frequency
      // of the message. Eg: a node or comment has been edited several times
      // in a too small timespan, then we only show the last one
      if ($message->nid > 0 || $message->nid_target > 0) {
        $id = $message->message_id .'-'. $message->uid .'-'. $message->uid_target .'-'. $message->nid .'-'. $message->nid_target;
        if (in_array($id, $holder)) {
          $message_set[array_search($id, $holder)]->count++;
          unset($message_set[$key]);
        }
        else {
          $holder[$key] = $id;//$message->message;
        }
      }

      // Remove duplicates that were created by duplicate user logging
      // This happens when two users were active for the same event
      if (isset($message->variables['duplicate']) && $message->variables['duplicate']) {

        $users_relations[$message->message_id][$message->uid . '-' . $message->uid_target] = array(
          'duplicate' => isset($users_relations[$message->message_id][$message->uid_target . '-' . $message->uid]) ? TRUE : FALSE,
          'uid' => $message->uid,
          'relation' => $message->uid_target,
          'message_id' => $message->message_id,
          'key' => $key,
        );
      }

    }

    if (!empty($users_relations)) {

      foreach ($users_relations as $type => $relations) {

        // Check which of the duplicates should be second
        foreach ($relations as $uid => $relationship) {

          $uid = $relationship['uid'];
          $rel_uid = $relationship['relation'];

          if ($user->uid == $uid && isset($relations[$rel_uid . '-' . $uid])) {
            $duplicates[] = $relations[$rel_uid . '-' . $uid]['key'];
          }
          elseif ($user->uid == $rel_uid) {
            $duplicates[] = $relationship['key'];
          }
          elseif (isset($relations[$rel_uid . '-' . $uid]) && $relationship['duplicate']) {
            $duplicates[] = $relations[$rel_uid . '-' . $uid]['key'];
          }
        }
      }
    }

    $duplicates = array_unique($duplicates);
    foreach ($duplicates as $duplicate_key => $duplicate) {
      unset($message_set[$duplicate]);
    }

    return $message_set;
  }

  /**
   * Prepare message candidates
   * This is an important method that handles several things
   * - Build an id for each time-gap limiting groupable messages
   * - Hold the candidates for each id (and count of messages)
   * - Logic to handle the group_by node or user setting
   */
  private function prepare_candidates($message_set, $set_count=0) {

    static $singles = 0;

    // Remove duplicate messages in the same set,
    // incrementing the counter on the grouped message
    $message_set = $this->remove_message_duplicates($message_set);

    foreach ($message_set as $key => $message) {

      if (!$message->nid_access || !$message->nid_target_access || !$message->uid_access) {
        continue;
      }

      $type = isset($message->template->concat_args['type']) ? $message->template->concat_args['type'] : 'single';

      // Create a gap id, which is a unique id per time gap
      $gap_id = $this->create_gap_id($message, $type, $set_count);

      // Summaries have to be evaluated for merging
      if ($type == 'summary') {

        // Add a candidates if this one does not exist yet
        if (!isset($this->_candidates[$gap_id])) {
          $this->_candidates[$gap_id] = array(
            'count' => 0,
            'group_target' => $message->template->concat_args['group_target'],
            'variables' => array()
          );
          // Add the message
          $this->_messages[$gap_id] = $message;
        }
        // Make sure the the timestamp gets updated
        else {
          $this->_messages[$gap_id]->timestamp = $message->timestamp;
        }

        // Add the counters and variables needed for merging in groups
        $this->_messages[$gap_id]->target_count++; // Message occurrency, first time is 1

        $this->_candidates[$gap_id]['variables'][] = $message->variables;
        $this->_candidates[$gap_id]['count']++;  // variable count, NOT the same as target_count
      }
      elseif ($type == 'count') {

        $message_template = str_replace("%times%", $message->count, $message->message);
        $message->message = str_replace("%count%", $message->count, $message_template);
        $this->_messages[$gap_id] = $message;

      }
      else {

        // Autoincrement the singles to make the gap_id unique
        $gap_id .= '_'. $singles;
        $this->_messages[$gap_id] = $message;
        $singles++;
      }
    }

  }

  /**
   * Function to clean up dirty messages and xss attacks
   */
  private function clean_up() {
    $this->remove_variables_duplicates();
  }

  /**
   * Function to remove duplicate messages valuating the
   * variables and count properties
   */
  private function remove_variables_duplicates() {

    $uniques = array();
    foreach ($this->_candidates as $single_id => $info) {
      $uniques[$single_id] = array();
      // Loop through variables to check if there are identical
      foreach ($info['variables'] as $rid => $value) {
        if (!in_array($value, $uniques[$single_id])) {
          $uniques[$single_id][] = $value;
        }
        else {
          unset($this->_candidates[$single_id]['variables'][$rid]);
        }
      }
    }

    // Re-evaluate the counts
    foreach ($this->_candidates as $single_id => $info) {
      $this->_candidates[$single_id]['count'] = count($this->_candidates[$single_id]['variables']);
    }

    return $uniques;
  }

  /**
   * create_gap_id
   *   Private helper function to build the gap_id
   *
   * @param Object HeartbeatActivity $message
   *   Object that holds the data of a heartbeat stream message
   * @param String $type
   *   type of the HeartbeatActivity message
   * @param integer $set_count
   *   The count id of the current timespan where
   *   data must be grouped
   *
   * @return $gap_id
   *  A unique id to summarize data within a certain timespan
   */
  private function create_gap_id($message, $type, $set_count) {

    // Variable to hold the unique grouping gap id.
    // Extending the gap id will result in summaries
    // and groups of identical and related messages.
    $gap_id = 'BEAT_' . $set_count . '_' . $message->message_id;

    // Extend the gap id with the node_type if available
    // but this is too dangerous because i don't know if people intend to
    // separate the node types from merging ...
    if (!empty($message->variables['@node_type'])) {
      $gap_id .= '_' . $message->variables['@node_type'];
    }

    // For single types, we dont need larger unique id's
    if ($type == 'single') {
      return $gap_id . '_single';
    }
    elseif ($type == 'count') {
      return $gap_id . '_count_'. $message->uid;
    }


    if (isset($message->template->concat_args['group_by'])) {

      $grouptarget = $message->template->concat_args['group_target'];
      $groupby = $message->template->concat_args['group_by'];
      // group by node will result in a summary of
      // users performing the activity
      if ($groupby == 'node') {
        $gap_id .= '_' . $grouptarget . '_node_' . $message->nid;
      }

      // group by user will result in a summary of content
      // nodes which the user has done the activity on
      elseif ($groupby == 'user') {

        $gap_id .= '_' . $grouptarget . '_user_' . $message->uid;
        if ($message->nid_target > 0) {
          $gap_id .= '_'. $message->nid_target;
        }
      }

      // group by target object id will result in a summary of content
      // nodes which the user has done the activity on
      elseif ($groupby == 'node-target') {

        $gap_id .= '_' . $grouptarget . '_user_' . $message->uid . '_node_target_' . $message->nid_target;
      }

      // group by user-user will result in unique user-user
      // messages, expressing a relation between the two users
      elseif ($groupby == 'user-user') {

        $gap_id .= '_user_relation';
        //$gap_id .= '_'. ($target_uid == 0 ? $message->uid : $target_uid);
        $group_target = $message->template->concat_args['group_by_target'];
        $string = $message->variables['@' . $group_target];
        $group_gap_string = strip_tags(drupal_strtolower($string));
        $gap_id .= '_' . $group_gap_string;
      }
      else {

        // Handle the fall-backs as unique messages
        $gap_id .= '_' . $message->uid .'_' . (int)$message->uid_target;
      }
    }

    return $gap_id;
  }

  /**
   * Function to rebuild the messages in groups
   * all sets are handled already
   */
  private function rebuild_in_groups() {

    global $base_url;

    $max_items_to_group = variable_get('heartbeat_activity_grouping_how_many', 6);

    // Check the candidates and make the rebuild array
    foreach ($this->_candidates as $single_id => $candidate) {

      // if a candidate exists for this message and it has more than one count
      // Take care!! not message->count because this could be set with the sql as well
      // The purpose would be to fill %times% or its alias %count%
      if ($candidate['count'] > 1) {
        // rebuild the message using the candidate variables
        $this->_messages[$single_id]->create_grouped_message($candidate, $max_items_to_group);

      }
    }
  }

  /**
   * Creates an instance of heartbeat as singleton
   */
  static public function instantiate($type='cached') {
    static $instances;
    if (!$instances) {
      $instances = array();
    }
    if (!isset($instances[$type])) {
      $instances[$type] = new HeartbeatParser();
    }
    return $instances[$type];
  }

  /**
   * Get the tags to allow in messages
   */
  public function get_tags() {
    return $this->_allowed_tags;
  }

  /**
   * Set the first time , start time of the gap
   */
  public function set_timespan_gap($gap) {
    $this->_timespan_gap = $gap;
  }

  /**
   * build sets of messages
   */
  public function build_sets($messages_raw) {

    // The start is the end of the current day
    $renewal_time = $_SERVER['REQUEST_TIME'] - (3600 * date("H") + 60 * date("i") + date("s")) + (24 *3600);

    // Retrieve variables to put on the object
    $show_time = variable_get('heartbeat_show_message_times', 1);
    $time_info_grouped = variable_get('heartbeat_show_time_grouped_items', 1);

    // Check access on the buttons and put their state onto the object
    $delete_access = _heartbeat_activity_stream_delete_access();
    $actor_access = variable_get('heartbeat_activity_stream_actor_delete', 1) && user_access('delete own heartbeat activity logs');

    $raw_messages = array();
    foreach ($messages_raw as $key => $message) {

      // Set basic properties for all messages
      $message->show_time = $show_time;
      $message->time_info_grouped = $time_info_grouped;
      $message->delete_access = $delete_access;
      $message->actor_access = $actor_access;

      // if the time with the gap exceeds the starttime
      if ($renewal_time >= $message->timestamp) {
        // reset the start time of the set
        $renewal_time = $message->timestamp - $this->_timespan_gap;
      }

      $this->_sets[$renewal_time][$key] = $message;
    }
  }

  /**
   * Merges sets of messages to fully formatted messages
   * regenerated at runtime to group settings
   */
  public function merge_sets() {

    $set_count = 0;
    foreach ($this->_sets as $message_set) {
      $this->prepare_candidates($message_set, $set_count);
      $set_count++;
    }

    $this->clean_up();

    $this->rebuild_in_groups();

    return $set_count;
  }

  /**
   * Gets the messages as they exist for the time asked
   */
  public function get_messages($limit = 0) {
    if ($limit > 0) {
      return array_slice($this->_messages, 0, $limit);
    }
    return $this->_messages;
  }
}

// eof