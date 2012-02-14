<?php
// $Id: user.rules.inc,v 1.1.2.20 2009/07/14 09:16:30 fago Exp $


/**
 * @file rules integration for the user module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_rules_event_info().
 */
function user_rules_event_info() {
  return array(
    'user_insert' => array(
      'label' => t('User account has been created'),
      'module' => 'User',
      'arguments' => rules_events_hook_user_arguments(t('registered user')),
    ),
    'user_update' => array(
      'label' => t('User account details have been updated'),
      'module' => 'User',
      'arguments' => rules_events_hook_user_arguments(t('updated user')) + array(
        'account_unchanged' => array('type' => 'user', 'label' => t('unchanged user')),
      ),
    ),
    'user_view' => array(
      'label' => t('User page has been viewed'),
      'module' => 'User',
      'arguments' => rules_events_hook_user_arguments(t('viewed user')),
      'help' => t("Note that if drupal's page cache is enabled, this event won't be generated for pages served from cache."),
    ),
    'user_delete' => array(
      'label' => t('User has been deleted'),
      'module' => 'User',
      'arguments' => rules_events_hook_user_arguments(t('deleted user')),
    ),
    'user_login' => array(
      'label' => t('User has logged in'),
      'module' => 'User',
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('logged in user')),
      ),
    ),
    'user_logout' => array(
      'label' => t('User has logged out'),
      'module' => 'User',
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('logged out user')),
      ),
    ),
  );
}

/**
 * Returns the argument definition for the acting user (global $user)
 */
function rules_events_global_user_argument() {
  return array('user' => array('type' => 'user', 'label' => t('acting user'), 'handler' => 'rules_events_argument_global_user'));
}

/**
 * Returns some arguments suitable for hook_user
 */
function rules_events_hook_user_arguments($account_label) {
  return array(
    'account' => array('type' => 'user', 'label' => $account_label),
  ) + rules_events_global_user_argument();
}

/**
 * Gets the user account of the "acting" user - which is always the global user
 */
function rules_events_argument_global_user() {
  global $user;
  return user_load(array('uid' => $user->uid));
}

/**
 * Implementation of hook_rules_condition_info().
 */
function user_rules_condition_info() {
  return array(
    'rules_condition_user_comparison' => array(
      'label' => t('Compare two users'),
      'arguments' => array(
        'user1' => array('type' => 'user', 'label' => t('User account 1')),
        'user2' => array('type' => 'user', 'label' => t('User account 2')),
      ),
      'help' => t('Evaluates to TRUE, if both given user accounts are the same.'),
      'module' => 'User',
    ),
    'rules_condition_user_hasrole' => array(
      'label' => t('User has role(s)'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('User')),
      ),
      'help' => t('Whether the user has the selected role(s).'),
      'module' => 'User',
    ),
  );
}

/**
 * A simple user comparison
 */
function rules_condition_user_comparison($user1, $user2) {
  return $user1->uid == $user2->uid;
}

/**
 * Condition user: condition to check whether user has particular roles
 */
function rules_condition_user_hasrole($user, $settings) {
  $roles = $settings['roles'];
  $operation = $settings['operation'];

  switch ($operation) {
    case 'OR':
      foreach ($roles as $rid) {
        if (isset($user->roles[$rid])) {
          return TRUE;
        }
      }
      return FALSE;

    case 'AND':
      foreach ($roles as $rid) {
        if (!isset($user->roles[$rid])) {
          return FALSE;
        }
      }
      return TRUE;
  }
}

/**
 * Implementation of hook_rules_action_info().
 */
function user_rules_action_info() {
  return array(
    'rules_action_user_addrole' => array(
      'label' => t('Add user role'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('User whos roles should be changed')),
      ),
      'module' => 'User',
    ),
    'rules_action_user_removerole' => array(
      'label' => t('Remove user role'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('User whos roles should be changed')),
      ),
      'module' => 'User',
    ),
    'rules_action_load_user' => array(
      'label' => t('Load a user account'),
      'new variables' => array(
        'user_loaded' => array('type' => 'user', 'label' => t('Loaded user')),
      ),
      'help' => t('Enter an id or a name of the user to load.'),
      'module' => 'User',
      'eval input' => array('userid', 'username'),
    ),
    'rules_action_user_create' => array(
      'label' => t('Create a user'),
      'arguments' => array(
        'username' => array('type' => 'string', 'label' => t('User name')),
        'email' => array('type' => 'string', 'label' => t("User's E-mail")),
      ),
      'new variables' => array(
        'user_added' => array('type' => 'user', 'label' => t('New user')),
        'user_added_pw' => array('type' => 'string', 'label' => t("New user's password")),
      ),
      'module' => 'User',
    ),
  );
}

/**
 * Action user: adds roles to a particular user
 */
function rules_action_user_addrole($user, $settings) {
  if ($user->uid) {
    // Get role list (minus the anonymous)
    $role_list = user_roles(TRUE);

    foreach ($settings['roles'] as $rid) {
      $user->roles[$rid] = $role_list[$rid];
    }
    user_save($user, array('roles' => $user->roles), NULL);
    return array('user' => $user);
  }
}

/**
 * User Action: deletes roles from a given user
 */
function rules_action_user_removerole($user, $settings) {
  if ($user->uid) {
    $roles = $settings['roles'];
    foreach ($roles as $rid) {
      // If the user has this role, delete it
      if (isset($user->roles[$rid])) {
        unset($user->roles[$rid]);
      }
    }
    user_save($user, array('roles' => $user->roles), NULL);
    return array('user' => $user);
  }
}

/**
 * Loads a user
 */
function rules_action_load_user($settings) {
  $userarray = array('name' => $settings['username'], 'uid' => $settings['userid']);
  return array('user_loaded' => user_load(array_filter($userarray)));
}

/**
 * Action: Creates a user.
 */
function rules_action_user_create($user_name, $user_mail) {
  $password = user_password();
  $user_data = array(
    'name' => $user_name,
    'mail' => $user_mail,
    'status' => 1,
    'pass' => $password,
  );
  //validate name input.
  if (user_validate_name($user_data['name'])) {
    rules_log(t('No appropriate user name. No user has been created.'));
  }
  elseif (drupal_is_denied('user', $user_data['name'])) {
    rules_log(t('The name %name has been denied access. No user has been created.', array('%name' => $user_data['name'])));
  }
  elseif ($account = user_load(array('name' => $user_data['name']))) {
    rules_log(t('User !name already exists. No user has been created.', array('!name' => $user_data['name'])));
  }
  //validate e-mail.
  elseif ($error = user_validate_mail($user_data['mail'])) {
    rules_log(t('No appropriate mail address. No user has been created.'));
  }
  elseif (drupal_is_denied('mail', $user_data['mail'])) {
    rules_log(t('The e-mail address %email has been denied access. No user has been created.', array('%email' => $user_data['mail'])));
  }
  elseif ($account = user_load(array('mail' => $user_data['mail']))) {
    rules_log(t('The e-mail address %email is already registered. No user has been created.', array('%email' => $user_data['mail'])));
  }
  else {
    $user = user_save(NULL, $user_data);
    return array('user_added' => $user, 'user_added_pw' => $password);
  }
}

/**
 * Implementation of hook_rules_data_type_info().
 */
function user_rules_data_type_info() {
  return array(
    'user' => array(
      'label' => t('user'),
      'class' => 'rules_data_type_user',
      'savable' => FALSE,
      'identifiable' => TRUE,
    ),
  );
}

/**
 * Defines the user data type
 */
class rules_data_type_user extends rules_data_type {
  function load($uid) {
    return user_load($uid);
  }

  function get_identifier() {
    $user = $this->get();
    return $user->uid;
  }
}

/**
 * Implementation of hook_rules_action_info_alter().
 *
 * Adapts the action info of some core actions to better fit for being
 * used with rules.
 */
function user_rules_action_info_alter(&$actions) {
  // This action doesn't take the user argument.
  $actions['rules_core_user_block_ip_action']['arguments'] = array('object' => array('type' => 'value', 'default value' => NULL));
  // This action can be configured to other users than the current one, so improve the label.
  $actions['rules_core_user_block_user_action']['label'] = t('Block a user');
}

/**
 * @}
 */
