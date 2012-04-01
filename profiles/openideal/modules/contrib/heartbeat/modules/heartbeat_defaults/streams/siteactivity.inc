<?php

/**
 * @file
 *   Public heartbeat activity access object
 */


/**
 * Class PublicHeartbeat
 * Concrete class to prepare messages for all
 * users in the site of whom the heartbeat is
 * set to public
 *
 */
class SiteActivity extends HeartbeatStream {

  /**
   * Implementation of queryAlter().
   */
  protected function queryAlter() {
    $this->query->condition('ha.access', 0, '>');
  }

  /**
   * Function to add a part of a sql to a query built by views.
   * @param object $view
   *   The view handler object by reference to add our part to the query
   */
  protected function viewsQueryAlter(&$view) {
    $views->query->condition('ha.access', 0, '>');
  }
}