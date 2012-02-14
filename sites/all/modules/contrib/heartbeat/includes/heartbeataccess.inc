<?php
// $Id: heartbeataccess.inc,v 1.1.2.2.2.30 2010/10/28 18:36:01 stalski Exp $

/**
 * @file
 *   HeartbeatAccess object is the object that takes stream
 *   configuration to create a stream of activity objects.
 *   It is the controlling organ at the pre-query, query and
 *   post-query phases.
 */

/**
 * Abstract class heartbeataccess
 *   This base class has final template methods which are
 *   used by the derived concretes. The HeartbeatAccess is a
 *   state object that is given to the HeartbeatMessageBuilder to
 *   set the access to the current request.
 */
abstract class HeartbeatAccess {

  protected $_whoisuser_types = array(
    self::TYPE_ACTOR => 'Acting user',
    self::TYPE_USER_PROFILE => 'Requested user (from url)'
  );
  protected $_whoisuser_type = self::TYPE_ACTOR;
  protected $_uid = 0;
  protected $_actor = NULL;
  protected $_errors = array();
  protected $_has_errors = FALSE;
  protected $_offset_sql = 0;
  protected $_page = FALSE;
  public $stream = NULL;

  const TYPE_ACTOR = 0;
  const TYPE_USER_PROFILE = 1;

  /**
   * Constructor
   *
   * @param $stream HeartbeatStream object with the neccesairy parameters
   * @param $page Boolean to indicate if this is a page view
   */
  public function __construct(HeartbeatStream $stream, $page = FALSE, $account = NULL) {

    $this->_page = $page;
    $this->stream = $stream;
    $this->stream->setItemsMax($this->_page);

    $this->whoIsActor($account);

    $this->construct();

  }

  /**
   * Fake constructor to hook this method instead of the constructor.
   */
  public function construct() {

  }

  /**
   * proctected fuinction whoIsActor
   *   Calculate the user of whom we want to see activity for.
   *   Set the actor of the stream.
   */
  protected function whoIsActor($account = NULL) {

    // Break all calculation if the requested user is given.
    if (isset($account)) {
      $this->_whoisuser_type = self::TYPE_USER_PROFILE;
      $this->_actor = $account;
      $this->stream->uid = $this->_uid = $account->uid;
      $this->stream->user_relations = $account->heartbeat_relations;
      return;
    }

    global $user;

    $this->_whoisuser_type = self::TYPE_ACTOR;
    $this->_actor = $user;
    $this->stream->uid = $this->_uid = $user->uid;
    $this->stream->user_relations = $user->heartbeat_relations;

  }

  /**
   * setError
   *
   * @param $text String of found error
   * @return void
   */
  protected function setError($text) {
    $this->_errors[] = $text;
    $this->_has_errors = TRUE;
  }

  /**
   * hasAccess
   *
   * @param $text
   * @return boolean to indicate the access to the stream
   */
  protected function hasAccess() {
    return TRUE;
  }

  /**
   * hasErrors
   *
   * @return boolean has errors
   */
  public function hasErrors() {
    return $this->_has_errors;
  }

  /**
   *
   * @param $text
   * @return unknown_type
   */
  public function getErrors() {
    return $this->_errors;
  }

  /**
   * Getter function for heartbeat page/blocks
   */
  public function isPage() {
    return $this->_page;
  }

  /**
   * Get HeartbeatStream object with all configurations
   * @return HeartbeatStream object
   */
  public function getStream() {
    return $this->stream;
  }

  /**
   *
   * @param $text
   * @return Array of messages
   */
  public function fetchMessages() {

  	if ($this->hasAccess()) {
	    $heartbeat = $this->prepareStream();
	    // Call to a hook method to adapt sql and thus result
	    $heartbeat = $this->dressUpMessages($heartbeat);
	    return $this->finishMessages($heartbeat);
  	}

    return array();
  }

  /**
   * resultSql
   *
   * @param $sql String query to extend the current query
   * @param $values Array of values to substitute in the query extension
   * @return array results
   */
  protected function resultSql($sql = '', $values = array()) {

    $args = array();
    //$args[] = $this->stream->sql_start . $sql . $this->stream->sql_end;
    $args[] = $this->stream->language;

    if ($this->stream->latest_uaid > 0) {
      $args[] = $this->stream->latest_uaid;
    }
    else {
      // Limit the query for messages older than offset_sql
      $args[] = $this->stream->offset_sql;

      // Limit the query for messages newer than oldest_date,
      // add param delete too old messages if set
      if ($this->stream->oldest_date) {
        $args[] = $_SERVER['REQUEST_TIME'] - $this->stream->oldest_date;
      }
    }

    // Put the arguments defined in subclasses into query
    $args = array_merge($args, $values);

    //$args[] = 0;
    //$args[] = $this->stream->num_load_max;

    // The cron and maximum number limits results to a minimum
    //$result = call_user_func_array('db_query_range', $args);
    $result = db_query_range($this->stream->sql_start . $sql . $this->stream->sql_end, $args, 0, $this->stream->num_load_max);
    $results = array();
    while ($row = db_fetch_object($result) ) {

      $template = new HeartbeatMessageTemplate($row->hid, $row->message_id, $row->message_orig, $row->message_concat_orig, $row->concat_args);
      $template->perms = $row->perms;
      $template->custom = $row->custom;
      $template->description = $row->description;
      $template->set_variables($row->variables_orig);
      $template->set_attachments($row->attachments);
      $template->set_roles(isset($template->concat_args['roles']) ? $template->concat_args['roles'] : array());

      $heartbeatactivity = new HeartbeatActivity($row, $template);

      $results[$row->uaid] = $heartbeatactivity;
    }

    $stream = heartbeat_stream_load($this->getAccess());

    $this->stream->display_filters = !empty($stream['display_' . ($this->_page ? '' : 'block_') . 'filters']);
    if ($this->stream->display_filters) {
      $this->stream->filters = $stream['filters'];
    }

    return $results;
  }

  /**
   * prepareStream
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

    $this->stream->sql_end = " ORDER BY ua.timestamp DESC, ua.uaid DESC";

    $heartbeat = $this->createHeartbeatParser();

    return $heartbeat;
  }

  /**
   * createHeartbeatParser
   *
   * @return HeartbeatParser object
   */
  final protected function createHeartbeatParser() {
    $heartbeat = HeartbeatParser::instantiate($this->getAccess());
    return $heartbeat;
  }

  /**
   * setOffsetSql
   *
   * @param $offset integer to set the offset time
   * @return void
   */
  final public function setOffsetSql($offset = 0) {
    if ($offset == 0) {
      $offset = $_SERVER['REQUEST_TIME'];
    }
    $this->_offset_sql = $offset;
  }

  /**
   *
   * @param $text
   * @return HeartbeatParser object
   */
  protected function dressUpMessages(HeartbeatParser $heartbeat) {
    $heartbeat->raw_messages = $this->resultSql();

    return $heartbeat;
  }

  /**
   * finishMessages
   *
   * @param HeartbeatParser $heartbeat Parser object
   * @return HeartbeatParser object
   */
  protected function finishMessages(HeartbeatParser $heartbeat) {
    return $heartbeat;
  }

  /**
   * Check denied messages
   */
  protected function checkDeniedMessages() {

    // Messages that have been denied through stream configuration
    // or overriden by user profile can be taken out in the query.
    // Note that for private and connected restrictions, it can only
    // be done later after the query.
    $denied_messages = array();
    if (!empty($this->stream->messages_denied)) {
      $denied_messages += array_keys($this->stream->messages_denied);
    }
    $denied_messages = array_unique($denied_messages);
    if (!empty($denied_messages)) {
      $this->stream->sql_start .= " AND ua.message_id NOT IN ('" . implode("','", $denied_messages) . "') ";
    }

  }

  /**
   * addViewQuery
   *
   * @param $view Object View
   * @return void
   */
  public function addViewQuery(&$view) {
    // Nothing for base class
  }

  /**
   * getAccess
   *
   * @return string Stream
   */
  final public function getAccess() {
    return drupal_strtolower($this->stream->realname);
  }
}
