<?php

/**
 * @file
 *   HeartbeatMessageTemplate
 *   Template object that holds configuration for
 *   a message template. A HeartbeatActivity object
 *   will be able to have the template as composite.
 */

/**
 * Class HeartbeatMessageTemplate
 *
 */
class HeartbeatMessageTemplate extends HeartbeatCtoolsObject {

  /**
   * Heartbeat message base properties.
   */
  public $hid = 0;
  public $message_id = '';
  public $perms = HEARTBEAT_PUBLIC_TO_ALL;
  public $description = '';
  public $message = '';
  public $message_concat = '';
  public $group_type = 'single';
  public $concat_args = array();
  public $variables = array();
  public $attachments = array();
  public $roles = array();
  public $new_type = NULL;
  public $base = NULL;


  /**
   * Constructor
   */
  public function __construct() {
    $this->table = 'heartbeat_messages';
  }

  /**
   * init().
   *
   * Function to initialize the template from a db_fetch_object.
   */
  public function init($row) {

    if (is_array($row)) {
      $row = (object) $row;
    }

    if (isset($row->hid)) {
      $this->hid = $row->hid;
    }
    $this->message_id = $row->message_id;
    $this->perms = (int) $row->perms;
    $this->description = $row->description;

    if (isset($row->message_orig)) {
      $this->message = $row->message_orig;
    }
    else {
      $this->message = $row->message;
    }
    if (isset($row->message_concat_orig)) {
      $this->message_concat = $row->message_concat_orig;
    }
    else {
      $this->message_concat = isset($row->message_concat) ? $row->message_concat : '';
    }

    $this->setArguments($row->concat_args);

    if (isset($row->variables_orig)) {
      $this->setVariables($row->variables_orig);
    }
    elseif (isset($row->variables)) {
      $this->setVariables($row->variables);
    }

    if (isset($row->attachments)) {
      $this->setAttachments($row->attachments);
    }

    $this->setRoles(isset($this->concat_args['roles']) ? $this->concat_args['roles'] : array());

  }

  /**
   * Sets the concatenation arguments
   */
  public function setArguments($concat_args) {

    if (is_string($concat_args)) {
      $this->concat_args = heartbeat_decode_message_variables($concat_args);
    }
    elseif (is_array($concat_args)) {
      $this->concat_args = $concat_args;
    }
  }

  /**
   * Sets the attachments
   */
  public function setAttachments($attachments = array()) {

    $this->attachments = array();

    // Strings however should be converted to array
    if (!empty($attachments)) {
      if (is_string($attachments)) {
        $this->attachments = unserialize($attachments);
      }
      elseif (is_array($attachments)) {
        $this->attachments = $attachments;
      }
    }

  }

  /**
   * hasAttachments().
   */
  public function hasAttachments() {
    return !empty($this->attachments);
  }

  /**
   * Sets the roles
   */
  public function setRoles($roles = array()) {

    // Filter the roles out of the arguments
    if (!empty($roles) && is_array($roles)) {
      foreach ($roles as $rid) {
        if ($rid != 0) {
          $this->roles[$rid] = $rid;
        }
      }
    }

  }

  /**
   * hasRoleRestrictions().
   *
   * Function to check if there are roles restrictrions for this template
   */
  public function hasRoleRestrictions() {
    return empty($this->roles);
  }

  /**
   * Sets the variables array
   * Data variables are stored in an array to use them to build real variables.
   * this means that when activity message objects get merged, the variables
   * will be filled with variables components from all instances.
   */
  public function setVariables($variables = NULL) {

    if (!empty($variables)) {

      if (is_string($variables)) {
        $this->variables = heartbeat_decode_message_variables($variables);
      }
      elseif (is_array($variables)) {
        $this->variables = $variables;
      }
    }

  }

  /**
   * delete().
   *
   * Function to delete the template from database.
   */
  public function delete() {

    return db_delete('heartbeat_messages')
      ->condition('message_id', $this->message_id)
      ->execute();

  }

  /**
   * save().
   *
   * Function to save the template to database.
   */
  public function save() {

    $this->setMessageVariables();

    $fields = array(
      'message' => $this->message,
      'message_concat' => $this->message_concat,
      'attachments' => $this->attachments,
      'variables' => $this->variables,
      'description' => $this->description,
      'concat_args' => $this->concat_args,
      'perms' => (int)$this->perms,
      'disabled' => (int)$this->disabled
    );
    if (isset($this->message_id)) {
      $fields['message_id'] = $this->message_id;
    }

    // Update a record.
    if (!empty($this->hid)) {
      $query = db_update('heartbeat_messages')->condition('hid', $this->hid);
    }
    // Insert new record.
    else {
      $query = db_insert('heartbeat_messages');
    }

    return $query->fields($fields)->execute();

  }

  /**
   * setMessageVariables().
   *
   * Look for variables in string.
   */
  public function setMessageVariables() {

    if (!empty($this->variables)) {
      return;
    }

    $this->variables = HeartbeatMessageTemplate::getVariablesFromMessage($this->message, $this->message_concat);

  }

  /**
   * getVariablesFromMessage().
   */
  static public function getVariablesFromMessage($message, $message_concat) {

    $variables = array();
    preg_match_all("/\!([a-zA-Z0-9_]+)/", $message . ' ' . $message_concat, $matches);
    if (!empty($matches)) {
      // matches[0] contains variables prefixed with ! sign
      // matches[1] contains the variable names
      foreach ($matches[0] as $match) {
        $variables[$match] = '';
      }
    }

    return $variables;

  }

}