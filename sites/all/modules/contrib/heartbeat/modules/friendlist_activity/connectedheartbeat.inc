<?php
// $Id: connectedheartbeat.inc,v 1.1.2.1 2010/04/14 20:09:10 stalski Exp $

/**
 * @file
 *   Relational heartbeat activity access object
 */

heartbeat_include('HeartbeatAccess');

/**
 * Class ConnectedHeartbeat
 * Concrete class to prepare messages for the current user
 * and all of his/her relations.
 *
 */
class ConnectedHeartbeat extends HeartbeatAccess {

  public function dressUpMessages(HeartbeatParser $heartbeat) {

    if (!empty($this->stream->user_relations)) {
      $sql = " AND ((ua.uid in( %s ) AND ua.access >= 0) OR (ua.uid_target = %d AND ua.nid <> 0))";
      $heartbeat->raw_messages = $this->resultSql($sql, array(implode(',', $this->stream->user_relations), $this->stream->uid));
    }
    else {
      $heartbeat->raw_messages = array();
    }

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
    $sql = "$field in( ". implode(',', $GLOBALS['user']->heartbeat_relations) ." ) AND $view->table_alias.access >= 0 ";
    $view->query->add_where('orgroup', $sql);

    $sql = "$view->table_alias.uid_target = %d AND $view->table_alias.nid <> 0";
    $view->query->add_where('orgroup', $sql, $this->_uid);
  }
}