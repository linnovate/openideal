<?php
// $Id: heartbeatstream.inc,v 1.1.2.12 2010/09/26 21:03:17 stalski Exp $

/**
 * @file
 *   HeartbeatStream object
 *   Parameter object that is given to a HeartbeatAccess object
 *   to form a full stream of handled activity objects.
 */

/**
 * Class HeartbeatStream
 *
 * Changeable object with configurations for an activity stream
 */
class HeartbeatStream {

  // Class name actualy used
  public $name = '';

  // Real name actualy used
  public $realname = '';

  // Human readable name
  public $title = '';

  // The actor
  public $uid = 0;

  // The relations of the user <uid>
  public $user_relations = array();

  // The module the query builder is located
  public $module = '';

  // Stream access
  public $access = -1;

  // The language for the current stream display
  public $language = '';

  // Timestamp to offset the query
  public $offset_sql = 0;

  // Total allowed
  public $limit_sql = 0;

  // First part of the query that is unchangeable
  public $sql_start = '';

  // Last part to finish the sql as heartbeat intends
  public $sql_end = '';

  // Boolean to indicate if it is a page display
  public $pager = FALSE;

  // Boolean to indicate the usage of filters for pages
  public $display_filters = FALSE;

  // The filters set for this stream
  public $filters = array();

  // Boolean to indicate if filters have radio or checkbox functionality
  public $filters_cumul = FALSE;

  // Denied message templates
  public $messages_denied = array();

  // Limit the number of messages by oldest time
  public $oldest_date = 604800;

  // Limit the number of messages by maximum messages to load
  public $num_load_max = 100;

  // Limit the timespan to group messages
  public $grouping_seconds = 7200;

  // Maximum number of items in the block display
  public $block_items_max = 25;

  // Number to indicate how a block-pager should be shown
  public $block_show_pager = 0;

  // Maximum number of items in the page display
  public $page_items_max = 50;

  // Boolean to indicate of a page-pager should be shown
  public $page_show_pager = 0;

  // Boolean to indicate if the pager is ajax-driven
  public $page_pager_ajax = 0;

  // Poll for newer messages
  public $poll_messages = 0;

  // The latest found uaid
  public $latest_uaid = 0;

  // Extra variables
  public $variables = array();

  /**
   * Constructor to load the type variable
   */
  public function __construct($type) {

    $this->name = drupal_strtolower($type['class']);
    $this->realname = drupal_strtolower(isset($type['realname']) ? $type['realname'] : $type['class']);
    $this->title = $type['name'];
    $this->module = $type['module'];
    $this->access = $type['access'];

    $this->display_filters = !empty($type['display_filters']) || !empty($type['display_block_filters']);
    if ($this->display_filters) {
      $this->filters = $type['filters'];
      $this->filters_cumul = $type['filters_cumul'];
    }

    if (isset($type['messages_denied'])) {
      $this->messages_denied = $type['messages_denied'];
    }

    if (isset($type['poll_messages'])) {
      $this->poll_messages = $type['poll_messages'];
    }

    if (isset($type['grouping_seconds'])) {
      $this->grouping_seconds = $type['grouping_seconds'];
    }
    else {
      $this->grouping_seconds = variable_get('heartbeat_activity_grouping_seconds', 7200);
    }

    if (isset($type['settings']) && is_array($type['settings'])) {
      foreach ($type['settings'] as $key => $val) {
        if (property_exists($this, $key)) {
          $this->{$key} = $val;
        }
        else {
          $this->variables[$key] = $val;
        }
      }
    }

  }

  /**
   * Set the maximum number of items to fetch
   */
  public function setItemsMax($page) {

    $this->oldest_date = variable_get('heartbeat_activity_log_cron_delete', 2678400);

    if ($page) {
      $this->limit_sql = $this->page_items_max;
    }
    else {
      $this->limit_sql = $this->block_items_max;
    }
  }
}