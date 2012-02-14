<?php
// $Id: groupsactivity.inc,v 1.1.2.3 2010/08/14 18:36:13 stalski Exp $

/**
 * @file
 *   HeartbeatAccess stream object defining activity in ALL groups.
 */


heartbeat_include('HeartbeatAccess');

/**
 * Class GroupsActivity
 *   Concrete class to prepare messages for activity in groups
 *
 */
class GroupsActivity extends HeartbeatAccess {

  /**
   * Override of the prepareStream function
   *
   * @param $text
   * @return HeartbeatParser object
   */
  protected function prepareStream() {

    global $user, $language;
    $this->stream->offset_sql = $this->_offset_sql;

    // Prevent already assigned language to be overriden
    if (empty($this->stream->language)) {
      $this->stream->language = $language->language;
    }

    // Sql parts
    $this->stream->sql_start = "SELECT
      hm.hid AS 'hid', hm.message AS 'message_orig',
      hm.message_concat AS 'message_concat_orig',
      hm.attachments AS 'attachments', hm.concat_args,
      hm.custom AS 'custom', hm.description as 'description',
      hm.perms, hm.variables AS 'variables_orig',
      ua.*,
      1 AS 'count'
    FROM {heartbeat_activity} ua
    LEFT JOIN {heartbeat_messages} hm ON ua.message_id = hm.message_id
    INNER JOIN {og_ancestry} oa ON (oa.nid = ua.nid OR oa.group_nid = ua.nid)
    WHERE
      ua.language = '%s'
    ";

    // Calls with an offset uaid will fetch newer messages
    if ($this->stream->latest_uaid > 0) {
      $this->stream->sql_start .= "AND (ua.uaid > %d ) ";
    }
    else {
      $this->stream->sql_start .= "AND (ua.timestamp < %d ";

      // Add sql to delete too old messages if set
      if ($this->stream->oldest_date) {
        $this->stream->sql_start .= "AND ua.timestamp > %d ";
      }
      $this->stream->sql_start .= ")";
    }

    $this->checkDeniedMessages();

    $this->stream->sql_end = " ORDER BY timestamp DESC";

    $heartbeat = $this->createHeartbeatParser();

    return $heartbeat;
  }

  public function dressUpMessages(HeartbeatParser $heartbeat) {

    $heartbeat->raw_messages = $this->resultSql();

    return $heartbeat;
  }

  /**
   * Function to add a part of a sql to a query built by views UI
   *
   * @param object $view The view handler object by reference to add our part to the query
   */
  public function addViewQuery(&$view) {

    /*$join = new views_join();
    $join->construct('og_ancestry', $view->table_alias, 'nid', 'group_nid', array(), 'INNER');
    $view->query->add_relationship('og_ancestry', $join, $view->table_alias);*/

  }

}