<?php

/**
 * @file
 * Defines activity comments.
 */


/**
 * Class HeartbeatFlagAttachmentPlugin.
 */
class HeartbeatActivityCommentsPlugin extends HeartbeatBasePlugin implements iHeartbeatPlugin  {

  public $HEARTBEAT_NODE_COMMENTS_PER_PAGE = 5;
  public $HEARTBEAT_REACTIONS_PER_PAGE = 5;

  /**
   * __construct().
   */
  public function __construct($label, $settings) {

    parent::__construct($label, $settings);

    $HEARTBEAT_NODE_COMMENTS_PER_PAGE = $settings['heartbeat_comments_node_count'];
    $HEARTBEAT_REACTIONS_PER_PAGE = $settings['heartbeat_comments_comment_count'];

    drupal_add_js(drupal_get_path('module', 'heartbeat') . '/js/autoresize.jquery.js');
    drupal_add_js(drupal_get_path('module', 'heartbeat_comments') . '/heartbeat_comments.js');

  }

  /**
   * isEnabled().
   */
  public function isEnabled(HeartbeatActivity $heartbeatActivity) {
    return isset($heartbeatActivity->template->attachments[$this->settings['plugin_name']], $heartbeatActivity->template->attachments[$this->settings['plugin_name']]['enabled']) && $heartbeatActivity->template->attachments[$this->settings['plugin_name']]['enabled'];
  }

  /**
   * getMenuItems().
   */
  public function getMenuItems() {
    return array();
  }

  /**
   * hasAttachments().
   */
  public function hasAttachments() {
    return isset($this->settings['attachment']) && $this->settings['attachment'];
  }

  /**
   * hasButtons().
   */
  public function hasButtons() {
    return isset($this->settings['attachment']) && $this->settings['attachment'];
  }

  /**
   * streamLoaded().
   */
  public function streamLoaded(HeartbeatStream $heartbeatStream) {
    $heartbeatStream->needsModal(TRUE);
  }

  /**
   * pluginUIForm().
   */
  public function pluginUIForm(&$form, &$form_state) {

    $settings = $form_state['item']->settings;
    $form['settings']['heartbeat_comments_cache'] = array(
      '#title' => t('Enable heartbeat comments cache'),
      '#description' => t('When enabled, heartbeat will cache the cache with the form foreach activity. This means times and so will not be updated untill new comment is made.'),
      '#type' => 'checkbox',
      '#default_value' => isset($settings['heartbeat_comments_cache']) ? $settings['heartbeat_comments_cache'] : '',
      '#weight' => -5,
    );
    $form['settings']['heartbeat_comments_position'] = array(
      '#title' => t('Choose the position of the comment box'),
      '#type' => 'select',
      '#options' => array('down' => t('Comment list is beneath the comment box'), 'up' => t('Comment list is on top of the comment box')),
      '#default_value' => isset($settings['heartbeat_comments_position']) ? $settings['heartbeat_comments_position'] : 'up',
      '#weight' => -5,
    );
    $form['settings']['heartbeat_comments_order'] = array(
      '#title' => t('Select the sort order of the comments'),
      '#type' => 'select',
      '#options' => array('recent_on_top' => t('Most recent comments on top'), 'oldest_on_top' => t('Oldest comments on top')),
      '#default_value' => isset($settings['heartbeat_comments_order']) ? $settings['heartbeat_comments_order'] : 'oldest_on_top',
      '#weight' => -5,
    );
    $form['settings']['heartbeat_comments_comment_count'] = array(
      '#title' => t('Maximum of displayed heartbeat comments'),
      '#type' => 'textfield',
      '#default_value' => isset($settings['heartbeat_comments_comment_count']) ? $settings['heartbeat_comments_comment_count'] : '',
      '#weight' => -5,
    );
    $form['settings']['heartbeat_comments_node_count'] = array(
      '#title' => t('Maximum of displayed heartbeat node comments'),
      '#type' => 'textfield',
      '#default_value' => isset($settings['heartbeat_comments_node_count']) ? $settings['heartbeat_comments_node_count'] : '',
      '#weight' => -5,
    );
    $form['settings']['heartbeat_comments_load_more'] = array(
      '#title' => t('Choose the behaviour of the more comments button'),
      '#type' => 'select',
      '#options' => array('ajax' => t('Load with ajax'), 'page' => t('Load on detail page')),
      '#default_value' => isset($settings['heartbeat_comments_load_more']) ? $settings['heartbeat_comments_load_more'] : '',
      '#description' => t('Note that the ajax loading will be very heavy on for high traffic sites. Use this feature wisely!')
    );

  }

  /**
   * pluginAttachmentForm().
   */
  public function pluginAttachmentForm(&$form, &$form_state) {

    $template = $form_state['item'];
    $form['attachments']['activitycomments'] = array(
      'enabled' => array(
        '#type' => 'checkbox',
        '#default_value' => isset($template->attachments['activitycomments']['enabled']) ? $template->attachments['activitycomments']['enabled'] : FALSE,
        '#title' => t('Enable comments for this activity template'),
        '#weight' => 10,
      ),
      'activitycomments_node' => array(
        '#type' => 'checkbox',
        '#default_value' => isset($template->attachments['activitycomments']['activitycomments_node']) ? $template->attachments['activitycomments']['activitycomments_node'] : array(),
        '#title' => t('Enable node comments for this activity template if a node is available'),
        '#weight' => 11,
      ),
    );

  }

  /**
   * createActivityAdditions().
   */
  public function createActivityAdditions($heartbeatActivity) {

    // Skip if already called.
    if (isset($heartbeatActivity->additions->object_type)) {
      return;
    }

    // Get the original message from the translation table if needed.
    $heartbeatActivity->tuaid = heartbeat_get_tuaid($heartbeatActivity->uaid);

    // Prepare required additional variables for comment handling.
    $heartbeatActivity->additions->comment_count = 0;
    $heartbeatActivity->additions->node = NULL;
    $heartbeatActivity->additions->node_comment = 0;
    $heartbeatActivity->additions->object_type = 'heartbeat_activity';
    $heartbeatActivity->additions->can_comment = FALSE;
    $heartbeatActivity->additions->comments_order = $this->settings['heartbeat_comments_order'];
    $heartbeatActivity->additions->comments_position = $this->settings['heartbeat_comments_position'];

    // Case if the comments are possible with nodes.
    if ($heartbeatActivity->nid > 0 && isset($heartbeatActivity->variables['node_type']) && module_exists('comment')) {

      $heartbeatActivity->message_nid_cache = $heartbeatActivity->nid;

      // Only nid & type is needed here (also for heartbeat_get_reactions)
      // so node_load is really over the top here.
      $heartbeatActivity->additions->node = new stdClass;
      $heartbeatActivity->additions->node->nid = $heartbeatActivity->nid;
      $heartbeatActivity->additions->node->uid = $heartbeatActivity->variables['node_uid'];
      $heartbeatActivity->additions->node->type = $heartbeatActivity->variables['node_type'];
      $heartbeatActivity->additions->node->status = $heartbeatActivity->variables['node_status'];
      $heartbeatActivity->additions->node_comment = $heartbeatActivity->template->attachments['activitycomments']['activitycomments_node'] && variable_get('comment_' . $heartbeatActivity->additions->node->type, 2) > 0;

      $heartbeatActivity->additions->object_type = $heartbeatActivity->additions->node->type;

    }

    // Case to check if the target node is set and is commentable and confiugred that way.
    if ($heartbeatActivity->nid_target > 0 && isset($heartbeatActivity->variables['node_target_type'])) {

      $heartbeatActivity->additions->node_target = new stdClass;
      $heartbeatActivity->additions->node_target->nid = $heartbeatActivity->nid_target;
      $heartbeatActivity->additions->node_target->uid = $heartbeatActivity->variables['node_target_uid'];
      $heartbeatActivity->additions->node_target->type = $heartbeatActivity->variables['node_target_type'];
      $heartbeatActivity->additions->node_target->status = $heartbeatActivity->variables['node_target_status'];

    }

    if (!$heartbeatActivity->additions->node_comment) {
      // Reset the nid to 0 since we don't use it for node-type disabled comments.
      $heartbeatActivity->message_nid_cache = 0;
    }

    // Check access on the form submission.
    // Note that normal heartbeat comments will be used in case of node restrictions.
    if ($heartbeatActivity->message_nid_cache && $heartbeatActivity->additions->node_comment) {
      $heartbeatActivity->additions->can_comment = user_access('post comments');
    }
    else {
      $heartbeatActivity->additions->can_comment = user_access('add heartbeat comment');
    }

  }

  /**
   * loadAttachments().
   */
  public function loadAttachments(HeartbeatActivity $heartbeatActivity) {

    // Create required additions made to the activity object.
    $this->createActivityAdditions($heartbeatActivity);

    if (!$heartbeatActivity->additions->can_comment) {
      return;
    }

    // Load from cache if configured and available.
    if ($this->settings['heartbeat_comments_cache'] && $objects_cache = cache_get('heartbeat:comments:'. $heartbeatActivity->tuaid. ':'. $heartbeatActivity->message_nid_cache)) {

      // Make sure the textarea.js script is attached.
      drupal_add_js('misc/textarea.js');
      $heartbeatActivity->additions->heartbeat_comments = array('_cached' => $objects_cache->data);

    }
    // Create and cache the list.
    else {

      // For the single message, make sure all comments are loaded.
      $heartbeatActivity->additions->all_comments = isset($this->stream) && $this->stream instanceOf SingleActivity;

      // HeartbeatActivity.additions.comment_count is cached in the message itself.
      // Node comment reactions follow core. Here an extra query is needed to
      // the node comment statistics table.
      if (isset($heartbeatActivity->additions->node) && $heartbeatActivity->additions->node_comment) {
        $result = db_query('SELECT comment_count FROM {node_comment_statistics} WHERE nid = :nid', array(':nid' => $heartbeatActivity->additions->node->nid));
      }
      else {
        $result = db_query('SELECT COUNT(hcid) AS comment_count FROM {heartbeat_comments} WHERE uaid = :uaid', array(':uaid' => $heartbeatActivity->uaid));
      }
      foreach ($result as $row) {
        $heartbeatActivity->additions->comment_count = $row->comment_count;
      }

      // Fetch the comments if needed.
      $heartbeatActivity->additions->reactions = array();
      if ($heartbeatActivity->additions->comment_count > 0) {
        $heartbeatActivity->additions->reactions = heartbeat_get_reactions($heartbeatActivity->tuaid, $heartbeatActivity->additions->node_comment, $heartbeatActivity->additions->node, $heartbeatActivity->additions->all_comments);
        $heartbeatActivity->classes .= 'has-comments';
      }
      else {
        $heartbeatActivity->classes .= 'no-comments';
      }

    }

  }

  /**
   * renderAttachments().
   */
  public function renderAttachments(HeartbeatActivity $heartbeatActivity) {

    // Create required additions made to the activity object.
    $this->createActivityAdditions($heartbeatActivity);

    if (!$heartbeatActivity->additions->can_comment) {
      return;
    }

    // Keep a variable to indicate if the comments for this activity need to be cached.
    $cache = $this->settings['heartbeat_comments_cache'];

    // Check if the user has access to the comment widget/list.
    if (!$heartbeatActivity->additions->can_comment) {

      $attachment = '<div class="user-login-teaser">' . t('!login or !register to make a comment', array('!login' => l(t('Login'), 'user'), '!register' => l(t('register'), 'user/register'))) . '</div>';
      $heartbeatActivity->add_attachment($attachment);
      return;

    }

    // check if we can return data from cache.
    if (isset($heartbeatActivity->additions->heartbeat_comments, $heartbeatActivity->additions->heartbeat_comments['_cached'])) {

      $list = $heartbeatActivity->additions->heartbeat_comments['_cached'];
      $extraCssClass = empty($list) ? "heartbeat-comments-nocomments" : "heartbeat-comments-comments";
      // Don't cache it if it's already been cached.
      $cache = FALSE;

    }
    else {

      $list = '';
      $extraCssClass = "heartbeat-comments-nocomments";

      if ($heartbeatActivity->additions->comment_count > 0) {

        $extraCssClass = "heartbeat-comments-comments";
        $heartbeatActivity->additions->has_more = FALSE;
        $comments_per_page = $this->HEARTBEAT_REACTIONS_PER_PAGE;
        if ($heartbeatActivity->template->attachments['activitycomments']['activitycomments_node'] && variable_get('comment_' . $heartbeatActivity->additions->object_type, 2) > 0) {
          $comments_per_page = $this->HEARTBEAT_NODE_COMMENTS_PER_PAGE;
        }

        if (!$heartbeatActivity->additions->all_comments && $heartbeatActivity->additions->comment_count > $comments_per_page) {
          $heartbeatActivity->additions->has_more = TRUE;
          $heartbeatActivity->additions->reactions = array_slice($heartbeatActivity->additions->reactions, 0, $comments_per_page);
        }

        if ($heartbeatActivity->additions->comments_order == 'oldest_on_top') {
          $heartbeatActivity->additions->reactions = array_reverse($heartbeatActivity->additions->reactions);
        }
        $list .= theme('heartbeat_comments', array('comments' => $heartbeatActivity->additions->reactions, 'activity' => $heartbeatActivity));

      }

    }

    // Cache the output if not cached yet.
    if ($cache) {
      cache_set('heartbeat:comments:'. $heartbeatActivity->tuaid. ':'. $heartbeatActivity->message_nid_cache, $list);
    }

    // Create the list and the form in correct order.
    $output = '';
    $output .= '<div class="heartbeat-comments heartbeat-comments-'. $heartbeatActivity->additions->object_type .' '. $extraCssClass  .'">';
    if ($heartbeatActivity->additions->comments_position == 'up') {
      $output .= $list;
    }
    $elements = drupal_get_form('heartbeat_comments_form', $heartbeatActivity);
    $output .= drupal_render($elements);

    if ($heartbeatActivity->additions->comments_position == 'down') {
      $output .= $list;
    }
    $output .= '</div>';

    // Create the attachment output.
    $display = $heartbeatActivity->additions->comment_count || !empty($heartbeatActivity->additions->comment_open_override) ? 'block' : 'none';
    $attachment = '<div id="heartbeat-comments-wrapper-' . $heartbeatActivity->tuaid . '" class="heartbeat-comments-wrapper" style="display: ' . $display . ';">';
    $attachment .= $output;
    $attachment .= '</div>';

    $heartbeatActivity->add_attachment($attachment);

  }

  /**
   * renderButtons().
   */
  public function renderButtons(HeartbeatActivity $heartbeatActivity) {

    // Create required additions made to the activity object.
    $this->createActivityAdditions($heartbeatActivity);

    if (!$heartbeatActivity->additions->can_comment) {
      return;
    }

    $label = heartbeat_comments_get_count_label($heartbeatActivity->additions->comment_count);

    $output = l($label, 'heartbeat/message/' . $heartbeatActivity->tuaid, array('attributes' => array('onclick' => 'javascript:Drupal.heartbeat.comments.toggleComments(this, ' . $heartbeatActivity->tuaid . '); return false;')));
    $heartbeatActivity->add_button('<span class="heartbeat-attachment-button">' . $output . '</span>');

  }

}
