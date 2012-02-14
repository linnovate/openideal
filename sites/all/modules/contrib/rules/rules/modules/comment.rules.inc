<?php
// $Id: comment.rules.inc,v 1.1.2.8 2009/08/19 14:58:41 fago Exp $


/**
 * @file rules integration for the comment module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_rules_event_info()
 */
function comment_rules_event_info() {
  return array(
    'comment_insert' => array(
      'label' => t('After saving a new comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('created comment')),
    ),
    'comment_update' => array(
      'label' => t('After saving an updated comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('updated comment')),
    ),
    'comment_delete' => array(
      'label' => t('After deleting a comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('deleted comment')),
    ),
    'comment_view' => array(
      'label' => t('Comment is being viewed'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('viewed comment')),
    ),
    'comment_publish' => array(
      'label' => t('After publishing a comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('published comment')),
    ),
    'comment_unpublish' => array(
      'label' => t('After unpublishing a comment'),
      'module' => 'Comment',
      'arguments' => rules_events_hook_comment_arguments(t('unpublished comment')),
    ),
  );
}

/**
 * Returns some arguments suitable for hook comment
 */
function rules_events_hook_comment_arguments($comment_label) {
  return array(
    'comment' => array(
      'type' => 'comment',
      'label' => $comment_label,
    ),
    'comment_author' => array(
      'type' => 'user',
      'label' => $comment_label .' '. t('author'),
      'handler' => 'rules_events_argument_comment_author',
    ),
    'node' => array(
      'type' => 'node',
      'label' => t('commented content'),
      'handler' => 'rules_events_argument_comment_node',
    ),
    'node_author' => array(
      'type' => 'user',
      'label' => t('commented content author'),
      'handler' => 'rules_events_argument_comment_node_author',
    ),
  ) + rules_events_global_user_argument();
}

/**
 * Gets the author of the comment
 */
function rules_events_argument_comment_author($comment) {
  // if comment author is anonymous user get basic info from comment
  if (!$comment->uid) {
    $account = new stdClass();
    $account->uid = 0;
    $account->name = $comment->name;
    $account->mail = $comment->mail;
    $account->roles = array(DRUPAL_ANONYMOUS_RID => 'anonymous user');
  }
  else {
    $account = user_load(array('uid' => $comment->uid));
  }
  return $account;
}

/**
 * Gets the comment's node
 */
function rules_events_argument_comment_node($comment) {
  return node_load($comment->nid);
}

/**
 * Gets the comment's node's author
 */
function rules_events_argument_comment_node_author($comment) {
  return rules_events_argument_node_author(rules_events_argument_comment_node($comment));
}

/**
 * Implementation of hook_rules_action_info().
 */
function comment_rules_action_info() {
  return array(
    'rules_action_load_comment' => array(
      'label' => t('Load comment by id'),
      'arguments' => array(
        'cid' => array(
          'type' => 'number',
          'label' => t('Comment id'),
          'required' => TRUE,
        ),
      ),
      'new variables' => array(
        'comment_loaded' => array(
          'type' => 'comment',
          'label' => t('Loaded comment'),
          'label callback' => 'rules_action_load_comment_variable_label',
        ),
      ),
      'module' => 'Comment',
    ),
  );
}

/**
 * Action: Load a comment by id.
 */
function rules_action_load_comment($cid) {
  return array('comment_loaded' => _comment_load($cid));
}

/**
 * Implementation of hook_rules_data_type_info()
 */
function comment_rules_data_type_info() {
  return array(
    'comment' => array(
      'label' => t('comment'),
      'class' => 'rules_data_type_comment',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'module' => 'Comment',
    ),
  );
}

/**
 * Defines the comment data type
 */
class rules_data_type_comment extends rules_data_type {
  function load($cid) {
    return _comment_load($cid);
  }

  function get_identifier() {
    $comment = $this->get();
    return $comment->cid;
  }
}

/**
 * @}
 */
