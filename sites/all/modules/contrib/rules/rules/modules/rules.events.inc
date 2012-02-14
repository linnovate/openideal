<?php
// $Id: rules.events.inc,v 1.1.2.14 2010/07/27 11:09:27 fago Exp $


/**
 * @file Invokes events for supported modules. Usually this should be
 *   directly in the module providing rules integration instead.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_nodeapi().
 */
function rules_nodeapi(&$node, $op, $teaser = NULL, $page = NULL) {

  if (in_array($op, array('view', 'insert', 'update', 'presave', 'delete'))) {
    // We pass the node by reference so that changes are saved automatically on op submit
    $arguments = ($op == 'view') ? array('node' => &$node, 'teaser' => $teaser, 'page' => $page) : array('node' => &$node);
    rules_invoke_event('node_'. $op, $arguments);
  }
}

/**
 * Implementation of hook_user().
 */
function rules_user($op, &$edit, &$account, $category = NULL) {
  static $account_unchanged;

  // We don't support updates for other categories than 'account'
  if ($op == 'update' && $category == 'account') {
    // Save the unchanged account for the use with op after_update.
    $account_unchanged = drupal_clone($account);
  }
  else if ($op == 'after_update' && $category == 'account') {
    rules_invoke_event('user_update', array('account' => &$account, 'account_unchanged' => $account_unchanged));
  }
  else if (in_array($op, array('insert', 'login', 'logout', 'view', 'delete'))) {
    // At user creation add the assigned roles which are not available in
    // $account yet to $account, so rules is aware of them.
    if ($op == 'insert' && isset($edit['roles'])) {
      $system_roles = user_roles();
      foreach ($edit['roles'] as $role_id => $value) {
        if (isset($system_roles[$role_id])) {
          $account->roles[$role_id] = $system_roles[$role_id];
        }
      }
    }

    rules_invoke_event('user_'. $op, array('account' => &$account));

    // Allow adding user roles during registration.
    if ($op == 'insert' && isset($account->roles)) {
      $edit += array('roles' => array());
      $edit['roles'] += $account->roles;
    }
  }
}

/**
 * Implementation of hook_init().
 */
function rules_init() {
  rules_invoke_event('init');
}

/**
 * Implementation of hook_cron().
 */
function rules_cron() {
  rules_invoke_event('cron');
}

/**
 * Implementation of hook_comment().
 */
function rules_comment($comment, $op) {
  if (in_array($op, array('insert', 'update', 'delete', 'view', 'publish', 'unpublish'))) {
    if (is_array($comment)) {
      $comment = (object)$comment;
    }
    rules_invoke_event('comment_'. $op, $comment);
  }
}

/**
 * Implementation of hook_taxonomy().
 */
function rules_taxonomy($op, $type, $array) {
  if ($type == 'term' && in_array($op, array('insert', 'update'))) {
    $taxonomy_object = (object) $array;
    rules_invoke_event('taxonomy_term_'. $op, $taxonomy_object);
  }
}

/**
 * Implementation of hook_theme_registry_alter().
 *
 * We need to use the alter hook here, so we can make sure we are the first prepross
 * handler called. So drupal messages aren't consumed before the redirect is initiated.
 *
 * @see _rules_action_drupal_goto_handler()
 */
function rules_theme_registry_alter(&$items) {
  array_unshift($items['page']['preprocess functions'], '_rules_action_drupal_goto_handler');
}

/**
 * @}
 */