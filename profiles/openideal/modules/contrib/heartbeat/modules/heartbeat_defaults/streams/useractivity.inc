<?php

/**
 * @file
 *   Personal/private heartbeat activity access object
 */


/**
 * Class PrivateHeartbeat
 * Concrete class to prepare messages for the
 * current user only
 */
class UserActivity extends HeartbeatStream {

  /**
   * Implementation of queryAlter().
   */
  protected function queryAlter() {
    $this->query->condition(db_or()
      ->condition('ha.uid', $this->viewed->uid)
      ->condition(db_and()->condition('ha.uid_target', $this->viewed->uid)->condition('ha.access', HEARTBEAT_PRIVATE, '>')));
  }

  /**
   * Function to add a part of a sql to a query built by views UI
   *
   * @param object $view The view handler object by reference to add our part to the query
   */
  protected function viewsQueryAlter(&$view) {
    $views->query->condition(db_or()
      ->condition('ha.uid', $this->viewed->uid)
      ->condition(db_and()->condition('ha.uid_target', $this->viewed->uid)->condition('ha.access', HEARTBEAT_PRIVATE, '>')));
  }
}