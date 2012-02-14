<?php
// $Id: privateheartbeat.inc,v 1.1.2.2.2.12 2010/08/31 20:49:55 stalski Exp $

/**
 * @file
 *   Personal/private heartbeat activity access object
 */


heartbeat_include('HeartbeatAccess');

/**
 * Class PrivateHeartbeat
 * Concrete class to prepare messages for the
 * current user only
 */
class PrivateHeartbeat extends HeartbeatAccess {

  public function dressUpMessages(HeartbeatParser $heartbeat) {

    $sql = " AND ((ua.uid = %d  AND hm.perms > %d ) OR (ua.uid_target = %d AND ua.access > %d) )";

    $heartbeat->raw_messages = $this->resultSql($sql, array($this->stream->uid, HEARTBEAT_PRIVATE, $this->stream->uid, HEARTBEAT_PUBLIC_TO_ADDRESSEE));

    return $heartbeat;
  }

  /**
   * Function to add a part of a sql to a query built by views UI
   *
   * @param object $view The view handler object by reference to add our part to the query
   */
  public function addViewQuery(&$view) {

    // Make the sql limited to the access
    $field = "$view->table_alias.$view->real_field";

    $view->query->set_where_group('OR', 'orgroup');

    $sql = "$field = %d  AND $view->table_alias.access > %d ";
    $view->query->add_where('orgroup', $sql, $this->_uid, HEARTBEAT_PRIVATE);

    $sql = "($view->table_alias.uid_target = %d AND $view->table_alias.access > %d)";
    $view->query->add_where('orgroup', $sql, $this->_uid, HEARTBEAT_PUBLIC_TO_ADDRESSEE);
  }
}