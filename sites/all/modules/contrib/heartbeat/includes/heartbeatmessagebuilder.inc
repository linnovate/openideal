<?php
// $Id: heartbeatmessagebuilder.inc,v 1.1.2.5.2.27 2010/10/16 09:16:35 stalski Exp $

/**
 * @file
 *   Strategy with access behaviour
 *
 * @usage
 * $context = new HeartbeatMessageBuilder(new PublicBehaviour());
 * $context->execute();
 *
 * $context = new HeartbeatMessageBuilder(new PrivateBehaviour());
 * $context->execute();
 *
 * $context = new HeartbeatMessageBuilder(new ConnectedBehaviour());
 * $context->execute();
 *
 */

/**
 * Class HeartbeatMessageBuilder
 * Message builder that fetches and customizes
 * users activity on the site.
 * The builder takes a heartbeataccess object to
 * handle the request to fetch the messages.
 */
class HeartbeatMessageBuilder {

  private $_heartbeatState = NULL;
  private $_hasErrors = FALSE;
  private $_errors = array();

  public function __construct(HeartbeatAccess $heartbeatState) {

    if (!$heartbeatState->hasErrors()) {
      $this->_heartbeatState = $heartbeatState;
    }
    else {
      $this->setError($heartbeatState->getErrors());
    }
  }

  public function hasErrors() {
    return $this->_hasErrors;
  }

  protected function setError($errors) {
    $this->_errors = $errors;
    $this->_hasErrors = TRUE;
  }

  public function getErrors() {
    return $this->_errors;
  }

  public function getState() {
    return $this->_heartbeatState;
  }

  public function updateState($heartbeatAccess) {
    $this->_heartbeatState = $heartbeatAccess;
  }

  /**
   * Executes a query, putting the result into a wellformed
   *   stream of heartbeat activity objects
   *
   * @return $messages
   *   array of messages
   */
  public function execute() {

    // Fetch the messages from database for current State
    $heartbeat = $this->_heartbeatState->fetchMessages();

    if (!empty($heartbeat->raw_messages)) {

      // Filter messages by permission
      $this->checkAccess($heartbeat);

      // Group the activity messages as configured
      $messages = $this->group($heartbeat);
    }

    if (!empty($messages)) {

      // Give contributes modules the opportunity to load
      // additions for the heartbeat activity message object
      $hook = 'heartbeat_load';
      foreach (module_implements($hook) as $module) {
        $function = $module .'_'. $hook;
        // $messages should be implemented by reference!!!
        $function($messages, $this->_heartbeatState);
      }

      // Let other modules retheme or completely rebuild messages
      $hook = 'heartbeat_theme_alter';
      foreach (module_implements($hook) as $module) {
        $function = $module .'_'. $hook;
        $result = $function($messages, $this->_heartbeatState);
      }

      // Give contributes modules the last chance to hook into the messages
      $hook = 'heartbeat_view';
      foreach (module_implements($hook) as $module) {
        $function = $module .'_'. $hook;
        // $messages should be implemented by reference!!!
        $function($messages, $this->_heartbeatState);
      }

      return $messages;
    }

    return array();
  }

  /**
   * Function to check access on messages
   * This behaviour is set by a heartbeat message configuration
   * to overrule the chosen display access type
   */
  private function checkAccess(HeartbeatParser $heartbeat) {

    global $user;
    $stream = $this->_heartbeatState->getStream();
    $tags = $heartbeat->get_tags();

    // First check access on message perms, roles and node access.
    foreach ($heartbeat->raw_messages as $key => $message) {

      // Check on xss attack before other modules can change messages.
      $heartbeat->raw_messages[$key]->message = filter_xss($heartbeat->raw_messages[$key]->message, $tags);
      $heartbeat->raw_messages[$key]->message_concat = filter_xss($heartbeat->raw_messages[$key]->message_concat, $tags);

      // Rewrite message access if the access was not changed at log-time
      // and if the user has configured the access state of this message type.
      if (isset($message->actor->heartbeat_activity_settings[$message->message_id]['access'])) {
        $message->access = $message->actor->heartbeat_activity_settings[$message->message_id]['access'];
      }

      // Remove messages that are excluded for everyone
      if ($message->access == HEARTBEAT_NONE) {
        unset($heartbeat->raw_messages[$key]);
        continue;
      }

      // Remove messages set private by site administrator
      // and remove messages set private by user profile setting.
      $private = $message->template->perms == HEARTBEAT_PRIVATE || $message->access == HEARTBEAT_PRIVATE;
      if ($private && $user->uid != $message->actor->uid) {
        unset($heartbeat->raw_messages[$key]);
        continue;
      }

      // Remove messages that non-related users don't have access for.
      // Note the fact that the load increases a lot by loading
      // the actors relations into the user object.
      $connected = $message->access == HEARTBEAT_PUBLIC_TO_CONNECTED || $message->template->perms == HEARTBEAT_PUBLIC_TO_CONNECTED;
      if ($connected && !in_array($user->uid, $message->actor->heartbeat_relations)) {
        unset($heartbeat->raw_messages[$key]);
        continue;
      }

      // Remove messages that are restricted to roles.
      if (isset($message->template->roles) && !isset($message->template->roles[DRUPAL_ANONYMOUS_RID])) {
        $access = FALSE;
        foreach ($message->template->roles as $rid) {
          if (isset($user->roles[$rid])) {
            $access = TRUE;
            break;
          }
        }
        if (!$access) {
          unset($heartbeat->raw_messages[$key]);
          continue;
        }
      }

      // Check if the current user has access to nodes and user profiles,
      // but only if the administrator did not overrule this setting
      if (variable_get('heartbeat_context_perm', 0) == 0) {

        if (!user_access('access user profiles', $user)) {
          $heartbeat->raw_messages[$key]->uid_access = FALSE;
        }

        $this->checkNodeAccess($heartbeat->raw_messages[$key], $user);

      }

    }

    // Check if any filters are active giving other modules the chance
    // to list the messages that are evaluated by this filter
    if (isset($_GET['filters'])) {

      $active_filters = drupal_map_assoc(explode(',', $_GET['filters']));
      $filtered_messages = array();

      foreach ($active_filters as $filter) {

        $function = 'heartbeat_filter_' . str_replace('-', '_', $filter);
        if (function_exists($function)) {
          $filtered_messages += $function($heartbeat->raw_messages, $this->_heartbeatState);
        }
      }
      $heartbeat->raw_messages = $filtered_messages;
    }

    // This hook is invoked BEFORE calculating the maximum
    // number of messages to show,
    // giving other modules the opportunity to remove messages
    // based on their own parameters and custom reasons.
    $hook = 'heartbeat_messages_alter';
    foreach (module_implements($hook) as $module) {
      $function = $module .'_'. $hook;
      $result = $function($heartbeat->raw_messages, $this->_heartbeatState);
    }

    // The difficulty remains in having the possibility to leave the
    // user with no site activity at display. Any propositions?
    // In short: problem when querying to few messages, having lots
    // of them denied and leaving the user with no messages left.

  }

  /**
   * Check the node access.
   */
  public function checkNodeAccess(& $message, $user) {


    if ($message->nid > 0) {

      // node_access only needs, nid, uid, type & format.
      $message_node = new stdClass;
      $message_node->nid = $message->nid;
      $message_node->uid = $message->nid_info['uid'];
      $message_node->type = $message->nid_info['type'];
      $message_node->format = $message->nid_info['format'];
      $message_node->status = 1; // unpublished nodes ignore access control

      //if (!node_access('view', node_load($message->nid), $user)) {
      if (!node_access('view', $message_node, $user)) {
        $message->nid_access = FALSE;
      }

      if ($message->nid_target > 0) {

        $message_node_target = new stdClass;
        $message_node_target->nid = $message->nid_target;
        $message_node_target->uid = $message->nid_target_info['info'];
        $message_node_target->type = $message->nid_target_info['type'];
        $message_node_target->format = $message->nid_target_info['format'];
        $message_node_target->status = 1; // unpublished nodes ignore access control

        //if (!node_access('view', node_load($message->nid_target), $user)) {
        if (!node_access('view', $message_node_target, $user)) {
          $message->nid_target_access = FALSE;
        }
      }

    }
  }

  /**
   * Function to check if more/older messages can be loaded
   *
   * @return Boolean has more messages
   */
  public function hasMoreMessages($page = TRUE) {

    $stream = $this->_heartbeatState->getStream();
    if ($page) {
      $display_pager = $stream->page_show_pager;
    }
    else {
      $display_pager = $stream->block_show_pager;
    }

    return $stream->pager && $display_pager;
  }

  /**
   * @see heartbeatparser.inc
   */
  public function group(HeartbeatParser $heartbeat) {

    $heartbeat->set_timespan_gap($this->_heartbeatState->stream->grouping_seconds);
    $heartbeat->build_sets($heartbeat->raw_messages);
    $heartbeat->merge_sets();
    $messages = $heartbeat->get_messages();

    // $messages = $heartbeat->remove_broken_messages();
    $num_total_messages = count($messages);

    // From here we know the number of messages actualy loaded (and allowed)
    $messages = array_slice($messages, 0, $this->_heartbeatState->stream->limit_sql);

    // Set the possibility of a pager appearence
    if ($num_total_messages > $this->_heartbeatState->stream->limit_sql) {
      $this->_heartbeatState->stream->pager = TRUE;
    }

    return $messages;
  }
}