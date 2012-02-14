<?php
// $Id: heartbeatmessagetemplate.inc,v 1.1.2.6 2010/02/21 22:45:26 stalski Exp $

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
class HeartbeatMessageTemplate {

  // Inherited from heartbeat message
  public $hid = 0;
  public $message_id = '';
  public $message = '';
  public $message_concat = '';
  public $concat_args = array();
  public $perms = 0;
  public $custom = 0;
  public $description = '';
  public $variables = array();
  public $attachments = array();
  public $roles = array();
  public $tags = array();

  /**
   * Constructor
   */
  public function __construct($hid, $message_id, $message, $message_concat, $concat_args) {

    $this->hid = $hid;
    $this->message_id = $message_id;
    $this->message = $message;
    $this->message_concat = $message_concat;

    $this->set_arguments($concat_args);

    if (variable_get('heartbeat_activity_display_tags', 0) == 1) {
      $this->tags = heartbeat_get_available_tags($this->hid);
    }
  }

  /**
   * Sets the concatenation arguments
   */
  public function set_arguments($concat_args) {

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
  public function set_attachments($attachments = array()) {

    // if it is empty, or an array or whatever, fill it in.
    $this->attachments = $attachments;

    // Strings however should be converted to array
    if (!empty($attachments)) {
      if (is_string($attachments)) {
        $this->attachments = unserialize($attachments);
      }
    }
  }

  /**
   * Sets the roles
   */
  public function set_roles($roles = array()) {

    // Filter the roles out of the arguments
    if (!empty($roles) && is_array($roles)) {
      foreach ($roles as $rid) {
        if ($rid != 0) {
          $this->roles[$rid] = $rid;
          //$this->roles[$rid] = db_result(db_query('SELECT r.name FROM {role} r WHERE r.rid=%d LIMIT 1', $rid));
        }
      }
    }
    // if no roles have been set, no role-based restrictions
    if ($this->roles == array()) {
      $this->roles[DRUPAL_ANONYMOUS_RID] = DRUPAL_ANONYMOUS_RID;
    }
  }

  /**
   * Sets the variables array
   * Data variables are stored in an array to use them to build real variables.
   * this means that when activity message objects get merged, the variables
   * will be filled with variables components from all instances.
   */
  public function set_variables($variables = NULL) {

    if (!empty($variables)) {

      if (is_string($variables)) {
        $this->variables = heartbeat_decode_message_variables($variables);
      }
      elseif (is_array($variables)) {
        $this->variables = $variables;
      }
    }
  }

}