<?php

/**
 * @file
 * User relations activity object.
 */

/**
 * Class RelationsActivity
 * Concrete class to build a stream with activity messages for all
 * users that are connected to the viewed user.
 */
class RelationsActivity extends HeartbeatStream {

  /**
   * Fake constructor to hook this method instead of the constructor.
   */
  public function construct() {
    // Add the relations to the viewed user object.
    $this->viewed->relations = heartbeat_related_uids($this->viewed->uid);
  }

  /**
   * Implementation of queryAlter().
   */
  protected function queryAlter() {

    // Access needs to be higher than private or addressee.
    $this->query->condition('ha.access', HEARTBEAT_PUBLIC_TO_ADDRESSEE, '>');

    // The viewed user is central in a set of related users,
    // or where the viewed user is the target user in the activity.
    $this->query->condition(db_or()
      ->condition('ha.uid', $this->viewed->relations, 'IN')
      ->condition('ha.uid_target', $this->viewed->uid)
    );

  }

  /**
   * Function to add a part of a sql to a query built by views.
   * @param object $view
   *   The view handler object by reference to add our part to the query
   */
  protected function viewsQueryAlter(&$view) {

    // Access needs to be higher than private or addressee.
    $view->query->condition('ha.access', HEARTBEAT_PUBLIC_TO_ADDRESSEE, '>');

    // The viewed user is central in a set of related users,
    // or where the viewed user is the target user in the activity.
    $view->query->condition(db_or()
      ->condition('ha.uid', $this->viewed->relations, 'IN')
      ->condition('ha.uid_target', $this->viewed->uid)
    );

  }
}