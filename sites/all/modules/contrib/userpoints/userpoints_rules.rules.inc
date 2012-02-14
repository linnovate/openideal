<?php
// $Id: userpoints_rules.rules.inc,v 1.1.2.1 2009/09/13 02:47:01 kbahey Exp $

/**
 * @file
 * Provide better integration into the rules module
 */

/**
 * Implementation of hook_rules_action_info().
 */
function userpoints_rules_action_info() {
  return array(
    'userpoints_action_grant_points' => array(
      'label' => t('Grant !points to a user', userpoints_translation()),
      'arguments' => array(
          'user' => array('type' => 'user', 'label' => t('User'))
        ),
      'module' => 'Userpoints',
      'eval input' => array('points'),
    ),
  );
}

/**
 * Implementation of hook_rules_event_info().
 */
function userpoints_rules_event_info() {
  return array(
    'userpoints_event_points_awarded' => array(
       'label' => t('User was awarded !points', userpoints_translation()),
       'arguments' => array(
          'user' => array('type' => 'user', 'label' => t('updated user')),
          'points' => array('type' => 'number', 'label' => t('Points awarded'))
        ),
       'module' => 'Userpoints',
    ),
  );
}

/**
 * Rules action - grant points to a user.
 */
function userpoints_action_grant_points($account, $settings) {
  userpoints_userpointsapi(array('uid' => $account->uid, 'points' => $settings['points']));
}

/**
 * Rules action form configuration - allow number of points to be set.
 */
function userpoints_action_grant_points_form($settings = array(), &$form) {
  $form['settings']['points'] = array(
    '#type' => 'textfield',
    '#title' => t('Number of points'),
    '#default_value' => isset($settings['points']) ? $settings['points'] : '',
    '#description' => t('The number of !points to award. You may enter a negative number as-well', userpoints_translation())
  );
}


/**
 * Rules Conditions - compare userpoints with a defined amount
 */

/**
 * Implementation of hook_rules_condition_info().
 */
function userpoints_rules_rules_condition_info() {
  return array(
    'userpoints_rules_amount' => array(
      'label' => t('Userpoints amount is >= than x'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('User'))
      ),
      'module' => 'Userpoints',
      'eval input' => array('amount'),
    ),
  );
}

/**
 * Rules Condition form configuration - set the amount to compare
 */
function userpoints_rules_amount_form($settings = array(), &$form) {
  $form['settings']['amount'] = array(
    '#type' => 'textfield',
    '#title' => t('amount to compare'),
    '#default_value' => isset($settings['amount']) ? $settings['amount'] : '',
    '#description' => t('The amount to compare if userpoints are >=. Example:30, will trigger the condition if the user userpoints are >= than 30 points.')
  );
  $form['settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Userpoints Categories to analyze'),
    '#options' => userpoints_get_categories(),
    '#default_value' => isset($settings['type']) ? $settings['type'] : '',
    '#description' => t('Userpoints Categories to analyze'),
    '#required' => TRUE,
  );
}

/**
 * Rules Condition - Userpoints amount is >= than
 */
function userpoints_rules_amount($account, $settings) {
  $balance = userpoints_get_current_points($account->uid, $settings['type']);
return ($balance >= $settings['amount']);
}