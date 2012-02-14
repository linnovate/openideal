<?php
// $Id: user.rules_forms.inc,v 1.1.2.6 2009/07/31 10:21:43 fago Exp $


/**
 * @file Rules configuration forms for the user module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Condition User: form to select roles to check for
 */
function rules_condition_user_hasrole_form($settings, &$form) {
  $form['settings'] += _rules_user_form_roles($settings, FALSE);
  $form['settings']['operation'] = array(
    '#type' => 'select',
    '#title' => t('Match against any or all of the selected roles'),
    '#options' => array('OR' => t('any'), 'AND' => t('all')),
    '#description' => t('If matching against all selected roles the user must have <em>all</em> the roles checked in the list above.'),
    '#default_value' => isset($settings['operation']) ? $settings['operation'] : 'OR',
  );
}

function rules_condition_user_hasrole_submit(&$settings, $form, $form_state) {
  $settings['roles'] = array_filter(array_keys(array_filter($settings['roles'])));
}

/**
 * Action: add user roles form
 */
function rules_action_user_addrole_form($settings, &$form) {
  $form['settings'] += _rules_user_form_roles($settings);
}

function rules_action_user_addrole_submit(&$settings, $form, $form_state) {
  $settings['roles'] = array_filter(array_keys(array_filter($settings['roles'])));
}

/**
 * User Action: form for selecting roles to be deleted
 */
function rules_action_user_removerole_form($settings, &$form) {
  $form['settings'] += _rules_user_form_roles($settings);
}

function rules_action_user_removerole_submit(&$settings, $form, $form_state) {
  $settings['roles'] = array_filter(array_keys(array_filter($settings['roles'])));
}

/**
 * Helper function to create role form
 */
function _rules_user_form_roles($settings = array(), $roles_only = TRUE) {
  $roles = array_map('filter_xss_admin', user_roles($roles_only));
  if ($roles_only) {
    unset($roles[DRUPAL_AUTHENTICATED_RID]);
  }

  $form = array();
  $form['roles'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Select role(s)'),
    '#options' => $roles,
    '#default_value' => isset($settings['roles']) ? $settings['roles'] : array(),
    '#required' => TRUE,
  );
  return $form;
}

function rules_action_load_user_form($settings, &$form) {
  $form['settings']['username'] = array(
    '#type' => 'textfield',
    '#title' => t('User name'),
    '#maxlength' => 60,
    '#autocomplete_path' => 'user/autocomplete',
    '#default_value' => isset($settings['username']) ? $settings['username'] : '',
    '#weight' => -1,
    '#description' => t('Name of the user to be loaded.'),
  );
  $form['settings']['userid'] = array(
    '#type' => 'textfield',
    '#title' => t('User id'),
    '#default_value' => isset($settings['userid']) ? $settings['userid'] : '',
    '#description' => t('Id of the user to be loaded.'),
  );
}

function rules_action_load_user_validate($form, $form_state) {
  if (!$form_state['values']['settings']['username'] && !$form_state['values']['settings']['userid']) {
    form_set_error('username', t('You have to enter the user name or the user id.'));
  }
}

/**
 * @}
 */
