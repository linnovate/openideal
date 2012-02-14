<?php

/**
 * @file
 * Flag attachment plugin for heartbeat.
 *
 */

/**
 * Class HeartbeatFlagAttachmentPlugin.
 */
class HeartbeatFlagAttachmentPlugin extends HeartbeatFlagPlugin implements iHeartbeatPlugin  {

  public $flags = array();

  /**
   * streamLoaded().
   */
  public function streamLoaded(HeartbeatStream $heartbeatStream) {
    $heartbeatStream->needsModal(TRUE);
  }

  /**
   * getMenuItems().
   */
  public function getMenuItems() {
    return array(
      'heartbeat/%ctools_js/flagged/%heartbeat_activity' => array(
        'title' => 'CTools js callback to show users who flagged activity',
        'type' => MENU_CALLBACK,
        'page callback' => 'heartbeat_plugins_modal_flagging_users',
        'page arguments' => array(1, 3),
        'access arguments' => array('access heartbeat activity profiles'),
      ),
    );
  }

  /**
   * hasAttachments().
   */
  public function hasAttachments() {
    return isset($this->settings['attachment']) && $this->settings['attachment'];
  }

  /**
   * loadAttachments().
   */
  public function loadAttachments(HeartbeatActivity $heartbeatActivity) {
    // nothing to do, buttons only.
  }

  /**
   * hasButtons().
   */
  public function hasButtons() {
    return isset($this->settings['attachment']) && $this->settings['attachment'];
  }

  /**
   * flagDefinitions().
   * Defines the flag definition.
   */
  public function flagDefinitions() {
    return array(
      'heartbeat_activity' => array(
        'title' => t('Heartbeat activity instance'),
        'description' => t("Heartbeat activity messages."),
        'handler' => 'flag_heartbeat_message',
      )
    );
  }

  /**
   * flagDefaults().
   * Defines the default flags.
   */
  public function flagDefaults() {

    $flags = array();
    // Exported flag: "I like".
    $flags['like'] = array(
      'content_type' => 'heartbeat_activity',
      'title' => 'I like',
      'global' => '0',
      'types' => array(),
      'flag_short' => 'I like this',
      'flag_long' => 'you like this',
      'flag_message' => '[heartbeat_activity:flag-like-count-linked] liked this',
      'unflag_short' => 'I don\'t like this',
      'unflag_long' => 'you don\'t like this',
      'unflag_message' => 'you don\'t like this',
      'unflag_denied_text' => '',
      'link_type' => 'toggle',
      'roles' => array(
        'flag' => array(
          '0' => '2',
        ),
        'unflag' => array(
          '0' => '2',
        ),
      ),
      'api_version' => 2,
    );
    return $flags;

  }

  /**
   * pluginUIForm().
   */
  public function pluginUIForm(&$form, &$form_state) {

    $form['settings']['count_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable the count data'),
      '#default_value' => isset($this->settings['count_enabled']) ? $this->settings['count_enabled'] : '',
    );

  }

  /**
   * pluginAttachmentForm().
   */
  public function pluginAttachmentForm(&$form, &$form_state) {

    // Get the flags for heartbeat activity objects.
    $flags = flag_get_flags('heartbeat_activity');
    foreach ($flags as $option) {
      $options[$option->name] = $option->title;
    }

    // Create element to trigger this plugin.
    $template = $form_state['item'];

    $form['attachments']['flagattachment'] = array();
    $form['attachments']['flagattachment']['flags'] = array(
      '#type' => 'checkboxes',
      '#default_value' => isset($template->attachments['flagattachment']['flags']) ? drupal_map_assoc($template->attachments['flagattachment']['flags']) : array(),
      '#title' => t('Add flags to this activity message'),
      '#options' => $options,
      '#weight' => 1,
    );

    // Create element for flag count enabled version.
    if ($this->countEnabled()) {

      $form['attachments']['flagattachment']['flag_count_enabled'] = array(
        'flags' => array(
          '#type' => 'checkbox',
          '#default_value' => isset($template->attachments['flagattachment']['flag_count_enabled']) ? $template->attachments['flagattachment']['flag_count_enabled']['flags'] : FALSE,
          '#title' => t('Enable flag count for this activity message'),
          '#weight' => 0,
        ),
      );

    }

  }

  /**
   * renderAttachments().
   */
  public function renderAttachments(HeartbeatActivity $heartbeatActivity) {

    $this->prepareFlags($heartbeatActivity);

    if ($this->countEnabled() && !empty($heartbeatActivity->template->attachments['flagattachment']['flag_flags'])) {

      foreach ($heartbeatActivity->template->attachments['flagattachment']['flag_flags'] as $flagname => $flag) {
        if ($heartbeatActivity->template->attachments['flagattachment']['flag_count'][$flagname]) {
          $heartbeatActivity->add_attachment('<span class="heartbeat-flag-count">' . $flag->get_label('flag_message', $heartbeatActivity) . '</span>');
        }
      }

    }

  }

  /**
   * renderButtons().
   */
  public function renderButtons(HeartbeatActivity $heartbeatActivity) {

    $this->prepareFlags($heartbeatActivity);

    if (!empty($heartbeatActivity->template->attachments['flagattachment']['flag_flags'])) {

      foreach ($heartbeatActivity->template->attachments['flagattachment']['flag_flags'] as $flagname => $flag) {
        $link = $flag->theme($flag->is_flagged($heartbeatActivity->uaid) ? 'unflag' : 'flag', $heartbeatActivity->uaid, FALSE);
        $heartbeatActivity->add_button($link);
      }

    }

  }

  /**
   * hasFlags().
   */
  protected function hasFlags(HeartbeatActivity $heartbeatActivity) {
    return isset($this->flags[$heartbeatActivity->uaid]) && $this->flags[$heartbeatActivity->uaid] === TRUE;
  }

  /**
   * countEnabled().
   */
  protected function countEnabled() {
    return isset($this->settings['count_enabled']) && $this->settings['count_enabled'];
  }

  /**
   * prepareFlags().
   */
  protected function prepareFlags(HeartbeatActivity $heartbeatActivity) {

    $this->flags[$heartbeatActivity->uaid] = FALSE;
    $flags = $heartbeatActivity->template->attachments['flagattachment']['flags'];

    if (!empty($flags)) {

      // Loop through flags to store runtime data.
      $counts = flag_get_counts('heartbeat_activity', $heartbeatActivity->uaid);
      foreach ($flags as $flagname) {

        if (!empty($flagname) && !is_numeric($flagname)) {

          $heartbeatActivity->template->attachments['flagattachment']['flag_flags'][$flagname] = flag_get_flag($flagname);

          // Store the link.
          $heartbeatActivity->template->attachments['flagattachment']['flag_link'][$flagname] = flag_create_link($flagname, $heartbeatActivity->uaid);

          // Store count to allow theme overrides to use this in another way.
          $heartbeatActivity->template->attachments['flagattachment']['flag_count'][$flagname] = isset($counts[$flagname]) ? $counts[$flagname] : 0;

          $this->flags[$heartbeatActivity->uaid] = TRUE;

        }

      }

    }

  }

}

/**
 * Implements a Heartbeat message flag.
 */
class flag_heartbeat_message extends flag_flag {

  function options_form(&$form) {
    parent::options_form($form);
    $form['access']['types'] = array(
      // A user flag doesn't support node types.
      // TODO: Support heartbeat templates instead of node types.
      '#type' => 'value',
      '#value' => array(0 => 0),
    );
  }

  function _load_content($content_id) {
    $heartbeatActivity = HeartbeatMessagePool::getInstance()->getMessage($content_id);
    return is_numeric($content_id) ? $heartbeatActivity : NULL;
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
    return TRUE;
  }

  function get_labels_token_types() {
    return array_merge(array('heartbeat_activity'), parent::get_labels_token_types());
  }

  function replace_tokens($label, $contexts, $options, $content_id) {
    if ($content_id) {
      if ($content_id instanceOf HeartbeatActivity) {
        $contexts['heartbeat_activity'] = $content_id;
        $content_id = $contexts['heartbeat_activity']->uaid;
      }
      elseif ($message = $this->fetch_content($content_id)) {
        $contexts['heartbeat_activity'] = $message;
      }
    }
    return parent::replace_tokens($label, $contexts, $options, $content_id);
  }

  function get_flag_action($content_id) {
    $flag_action = parent::get_flag_action($content_id);
    $flag_action->content_title = $this->fetch_content($content_id)->message;
    return $flag_action;
  }

  function get_relevant_action_objects($content_id) {
    return array('heartbeat_activity' => $this->fetch_content($content_id));
  }

  function rules_get_event_arguments_definition() {
    return array(
      'heartbeat_activity' => array(
        'type' => 'heartbeat_activity',
        'label' => t('flagged heartbeat message'),
        'handler' => 'flag_rules_get_event_argument',
      ),
    );
  }

  function rules_get_element_argument_definition() {
    return array('type' => 'heartbeat_activity', 'label' => t('Flagged heartbeat message'));
  }

  function get_views_info() {
    return array(
      'views table' => 'heartbeat_activity',
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