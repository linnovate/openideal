<?php
// $Id: publicheartbeat.inc,v 1.1.2.2.2.11 2010/09/02 20:35:33 stalski Exp $

/**
 * @file
 *   Public heartbeat activity access object
 */

heartbeat_include('HeartbeatAccess');

/**
 * Class PublicHeartbeat
 * Concrete class to prepare messages for all
 * users in the site of whom the heartbeat is
 * set to public
 *
 */
class PublicHeartbeat extends HeartbeatAccess {

  /**
   * Implementation of dressUpMessages().
   *
   * @param object HeartbeatParser $heartbeat
   *   The heartbeat parser object
   * @return object HeartbeatParser $heartbeat
   */
  public function dressUpMessages(HeartbeatParser $heartbeat) {

    $sql = " AND ua.access > 0 ";
    $heartbeat->raw_messages = $this->resultSql($sql);

    return $heartbeat;
  }

  /**
   * Function to add a part of a sql to a query built by views.
   * @param object $view
   *   The view handler object by reference to add our part to the query
   */
  public function addViewQuery(&$view) {

    // Make the sql limited to the access
    $view->query->add_where('andgroup', "$view->table_alias.access > 0");
  }
}