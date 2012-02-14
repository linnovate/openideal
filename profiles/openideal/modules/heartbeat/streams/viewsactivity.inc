<?php

/**
 * @file
 * Views heartbeat activity access object.
 *
 * This is a pseudo-stream used to fallback in the normal heartbeat flow
 * when views is the query builder.
 *
 */


/**
 * Class ViewsActivity
 *
 * Concrete class to prepare messages from a views query.
 *
 */
class ViewsActivity extends HeartbeatStream {

  /**
   * Implementation of queryAlter().
   */
  protected function queryAlter() {
    //$this->query->condition('ha.access', 0, '>');
  }

  /**
   * Function to add a part of a sql to a query built by views.
   * @param object $view
   *   The view handler object by reference to add our part to the query
   */
  protected function viewsQueryAlter(&$view) {
    //$views->query->condition('ha.access', 0, '>');
  }
}