<?php

/**
 * @file
 *   HeartbeatStream Configuration object
 *   Parameter object that is given to a HeartbeatStreamConfig object
 *   to form a full stream of handled activity objects.
 */

/**
 * Class HeartbeatStreamConfig
 *
 * Changeable object with configurations for an activity stream
 */
class HeartbeatStreamConfig extends HeartbeatCtoolsObject {

  // Class name actualy used.
  public $name = '';

  // Class to ease the read/write.
  public $class = '';

  // Real class to load for cloned streams.
  public $real_class = '';

  // The path to the class.
  public $path = '';

  // Human readable name.
  public $title = '';

  // The module the query builder is located.
  public $module = '';

  // Extra variables.
  public $variables = array();

  // Indicates whether this stream has a block display or not.
  public $has_block = TRUE;

  // Maximum number of items in the block display.
  public $block_items_max = 25;

  // Number to indicate how a block-pager should be shown.
  public $block_show_pager = 0;

  // Maximum number of items in the page display.
  public $page_items_max = 50;

  // Boolean to indicate of a page-pager should be shown.
  public $page_show_pager = 0;

  // Boolean to indicate if the pager is ajax-driven.
  public $page_pager_ajax = 0;

  // Setting for the number of grouped items maximum.
  public $show_message_times = 1;

  // Setting for the number of grouped items maximum in a grouped message.
  public $show_message_times_grouped = 0;

  // Denied message templates.
  public $messages_denied = array();

  // Limit the number of messages by maximum messages to load.
  public $num_load_max = 100;

  // Limit the timespan to group messages.
  public $grouping_seconds = 7200;

  // Boolean for to skip the viewing user, defaults to false.
  public $skip_active_user = FALSE;

  // Timestamp used to poll for newer messages.
  public $poll_messages = 0;

  // Page disabled indicates if there is page for this stream or not.
  public $page_disabled = FALSE;

  // Profile page exists.
  public $profilePage = FALSE;

  // Settings variable
  public $settings = array();

  /**
   * Constructor to load the type variable
   */
  public function __construct() {
    $this->grouping_seconds = variable_get('heartbeat_activity_grouping_seconds', 7200);
  }

}