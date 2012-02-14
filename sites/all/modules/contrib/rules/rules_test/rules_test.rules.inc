<?php
// $Id: rules_test.rules.inc,v 1.1.2.6 2009/04/19 15:03:43 fago Exp $


/**
 * @file Define some conditions and actions, used by the tests
 */

/**
 * Implementation of hook_rules_condition_info
 */
function rules_test_rules_condition_info() {
  return array(
    'rules_test_condition_alter_arg' => array(
      'label' => 'Alter data',
      'arguments' => array('rules_test' => array('type' => 'rules_test', 'label' => 'test')),
      'hidden' => TRUE,
    ),
    'rules_test_condition_check_for_altered_arg' => array(
      'label' => 'Check for altered data',
      'arguments' => array('rules_test' => array('type' => 'rules_test', 'label' => 'test')),
      'hidden' => TRUE,
    ),
  );
}

/**
 * Implementation of hook_rules_action_info
 */
function rules_test_rules_action_info() {
  return array(
    'rules_test_action_log_msg' => array(
      'label' => 'Logs a message to the rules log',
      'arguments' => array(),
      'hidden' => TRUE,
    ),
    'rules_test_change_arg' => array(
      'label' => 'Changes the argument',
      'arguments' => array('rules_test' => array('type' => 'rules_test', 'label' => 'test')),
      'hidden' => TRUE,
    ),
    'rules_test_add_var' => array(
      'label' => 'Adds a new variable',
      'new variables' => array('rules_test' => array('type' => 'rules_test', 'label' => 'test')),
      'hidden' => TRUE,
    ),
  );
}

function rules_test_condition_alter_arg(&$array) {
  $array['altered'] = TRUE;
  return TRUE;
}

function rules_test_condition_check_for_altered_arg($array) {
  return isset($array['altered']) && $array['altered'];
}

function rules_test_action_log_msg($settings) {
  rules_log($settings['msg']);
}

function rules_test_change_arg($array) {
  $array['altered'] = TRUE;
  return array('rules_test' => $array);
}

function rules_test_add_var() {
  $array['altered'] = FALSE;
  return array('rules_test' => $array);
}

