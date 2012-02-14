<?php

/**
 * @file
 * Provide better integration into the rules group
 */

/**
 * Implements hook_rules_action_info().
 */
function userpoints_rules_rules_action_info() {
  return array(
    'userpoints_action_grant_points' => array(
      'label' => t('Grant !points to a user', userpoints_translation()),
      'named parameter' => TRUE,
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user that will receive the !points', userpoints_translation()),
        ),
        'points' => array(
          'type' => 'integer',
          'label' => t('!Points', userpoints_translation()),
          'description' => t('Amount of !points to give or take.', userpoints_translation()),
        ),
        'tid' => array(
          'label' => t('!Points category', userpoints_translation()),
          'type' => 'integer',
          'options list' => 'userpoints_rules_get_categories',
        ),
        'entity' => array(
          'label' => t('Entity'),
          'type' => 'entity',
          'description' => t('The entity to which this transaction refers.'),
          'optional' => TRUE,
         ),
        'description' => array(
          'label' => t('Description'),
          'type' => 'text',
          'description' => t('Can contain the reason why the points have been granted.'),
          'restriction' => 'input',
          'optional' => TRUE,
        ),
        'operation' => array(
          'label' => t('Operation'),
          'type' => 'text',
          'description' => t('Describes the operation (Insert/Remove/...).'),
          'restriction' => 'input',
        ),
        'reference' => array(
          'label' => t('Reference'),
          'type' => 'text',
          'description' => t('Can contain a reference for this transaction.'),
          'optional' => TRUE,
        ),
        'display' => array(
          'label' => t('Display'),
          'type' => 'boolean',
          'description' => t('Whether or not to show a message to the user when this !points transaction is added.', userpoints_translation()),
          'default value' => TRUE,
        ),
        'moderate' => array(
          'label' => t('Moderate'),
          'type' => 'text',
          'description' => t('Whether or not this !points transaction should be moderated.', userpoints_translation()),
          'options list' => 'userpoints_rules_moderate',
        ),
        'expirydate' => array(
          'label' => t('Expiration Date'),
          'type' => 'date',
          'description' => t('Define when the !points should expire.', userpoints_translation()),
          'optional' => TRUE,
        ),
      ),
      'group' => t('!Points', userpoints_translation()),
    ),
    'userpoints_rules_get_current_points' => array(
      'label' => t('Load !points of a user', userpoints_translation()),
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user that will receive the !points', userpoints_translation()),
        ),
        'tid' => array(
          'label' => t('!Points category', userpoints_translation()),
          'type' => 'text',
          'options list' => 'userpoints_rules_get_all_categories',
        ),
      ),
      'new variables' => array(
        'loaded_points' => array(
          'type' => 'integer',
          'label' => t('Number of !points in the specified category.', userpoints_translation()),
        ),
      ),
      'group' => t('!Points', userpoints_translation()),
    ),
  );
}

/**
 * Wrapper function for userpoints_get_categories().
 *
 * Rules.module uses different arguments for option list callbacks than
 * userpoints_get_categories expects.
 */
function userpoints_rules_get_categories() {
  return userpoints_get_categories();
}

/**
 * Simple callback that lists the categories including an option for all.
 */
function userpoints_rules_get_all_categories() {
  return array('all' => t('All categories')) + userpoints_get_categories();
}

/**
 * Simple callback that lists the possible moderate values.
 */
function userpoints_rules_moderate() {
  return array(
    'default' => t('Use the site default'),
    'approved' => t('Automatically approved'),
    'moderated' => t('Added to moderation'),
  );
}

/**
 * Implements hook_rules_data_info().
 */
function userpoints_rules_rules_data_info() {
  return array(
    'userpoints_transaction' => array(
      'label' => t('!Points transaction', userpoints_translation()),
      'wrap' => TRUE,
      'property info' => _userpoints_userpoints_transaction_properties(),
    ),
  );
}

/**
 * Implements hook_rules_event_info().
 */
function userpoints_rules_rules_event_info() {
  return array(
    'userpoints_event_points_awarded_before' => array(
       'label' => t('User will be awarded !points', userpoints_translation()),
       'variables' => array(
          'userpoints_transaction' => array(
            'type' => 'userpoints_transaction',
            'label' => t('!Points transaction', userpoints_translation()),
          )
        ),
       'group' => t('!Points', userpoints_translation()),
    ),
    'userpoints_event_points_awarded_after' => array(
       'label' => t('User was awarded !points', userpoints_translation()),
       'variables' => array(
          'userpoints_transaction' => array(
            'type' => 'userpoints_transaction',
            'label' => t('!Points transaction', userpoints_translation()),
          )
        ),
       'group' => t('!Points', userpoints_translation()),
    ),
  );
}

/**
 * Rules action - grant points to a user.
 */
function userpoints_action_grant_points($params) {
  // The passed in $entity is the unwrapped object. To get type and id, we need
  // the wrapped version of it.
  $state = $params['state'];
  $entity = $state->currentArguments['entity'];

  // Map $moderate value to the actual value used by the API.
  $moderate_mapping = array(
    'default' => NULL,
    'approved' => FALSE,
    'moderated' => TRUE,
  );
  $params = array(
    // User id might be a int or a EntityValueWrapper.
    'uid' => is_object($params['user']->uid) ? $params['user']->getIdentifier() : $params['user']->uid,
    'entity_type' => $entity ? $entity->type() : NULL,
    'entity_id' => $entity ? $entity->getIdentifier() : NULL,
    'moderate' => $moderate_mapping[$params['moderate']],
    // Rules defaults to FALSE if the date format can not be parsed.
    // Use NULL instead since FALSE means no expiration.
    'expirydate' => $params['expirydate'] ? $params['expirydate'] : NULL,
  ) + $params;

  unset($params['state']);
  unset($params['user']);
  unset($params['entity']);
  unset($params['settings']);
  userpoints_userpointsapi($params);
}

function userpoints_action_grant_points_form_alter(&$form, &$form_state) {
  // Empty value by default.
  $form['parameter']['expirydate']['settings']['expirydate']['#default_value'] = '';
  // Operation is a single line textfield.
  $form['parameter']['operation']['settings']['operation']['#type'] = 'textfield';
}

/**
 * Rules action - load points of a user.
 */
function userpoints_rules_get_current_points($account, $tid) {
  return array('loaded_points' => userpoints_get_current_points($account->uid, $tid));
}
