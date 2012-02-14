<?php
// $Id: class.flag_heartbeat_message.inc,v 1.1.2.1.2.4 2010/02/21 22:45:27 stalski Exp $

/**
 * @file
 *   Class file for a heartbeat message flag
 */

/**
 * Implements a Heartbeat message flag.
 */
class flag_heartbeat_message extends flag_flag {
  function default_options() {
    $options = parent::default_options();
    $options += array(
      'show_on_heartbeat_message' => TRUE,
    );
    return $options;
  }

  function options_form(&$form) {
    parent::options_form($form);
    $form['types'] = array(
      // A user flag doesn't support node types. (Maybe will support roles instead, in the future.)
      '#type' => 'value',
      '#value' => array(0 => 0),
    );
    $form['display']['show_on_heartbeat_message'] = array(
      '#type' => 'checkbox',
      '#title' => t('Display link under heartbeat messages'),
      '#default_value' => $this->show_on_heartbeat_message,
      '#access' => empty($this->locked['show_on_heartbeat_message']),
    );
  }

  function _load_content($content_id) {
    return heartbeat_load_message_instance($content_id);
  }

  function applies_to_content_object($message_instance) {

    if ($message_instance) {
      return TRUE;
    }
    return FALSE;
  }

  function get_content_id($heartbeat_message) {
    return $heartbeat_message->uaid;
  }

  function uses_hook_link($teaser) {
    return $this->show_on_heartbeat_message;
  }

  function get_labels_token_types() {
    return array('heartbeat_message');
  }

  function replace_tokens($label, $contexts, $content_id) {
    if ($content_id) {
      if (($message = $this->fetch_content($content_id))) {
        $contexts['heartbeat_message'] = $message;
      }
    }
    return parent::replace_tokens($label, $contexts, $content_id);
  }

  function get_flag_action($content_id) {
    $flag_action = parent::get_flag_action($content_id);
    $flag_action->content_title = $this->fetch_content($content_id)->message;
    return $flag_action;
  }

  function get_relevant_action_objects($content_id) {
    return array('heartbeat_message' => $this->fetch_content($content_id));
  }

  function rules_get_event_arguments_definition() {
    return array(
      'heartbeat_message' => array(
        'type' => 'heartbeat_message',
        'label' => t('flagged heartbeat message'),
        'handler' => 'flag_rules_get_event_argument',
      ),
    );
  }

  function rules_get_element_argument_definition() {
    return array('type' => 'heartbeat_message', 'label' => t('Flagged heartbeat message'));
  }

  function get_views_info() {
    return array(
      'views table' => 'heartbeat_activity',
      'join field' => 'message_id',
      'title field' => 'message',
      'title' => t('Activity flag'),
      'help' => t('Limit results to only those messages flagged by a certain flag; Or display information about the flag set on an activity message.'),
      'counter title' => t('Activity flag counter'),
      'counter help' => t('Include this to gain access to the flag counter field.'),
    );
  }

  function applies_to_content_id_array($content_ids) {
    $passed = array();
    foreach ($content_ids as $uaid) {
      $passed[$uaid] = TRUE;
    }
    return $passed;
  }
}