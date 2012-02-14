<?php
// $Id: rules_test.rules_defaults.inc,v 1.1.2.14 2009/04/19 15:03:43 fago Exp $


/**
 * @file Rules default rules - this rules are used by the tests
 */

/**
 * Implementation of hook_rules_defaults.
 */
function rules_test_rules_defaults() {
  $rules = array();


  $rules['rules_test_1'] = array(
    '#type' => 'rule',
    '#name' => 'rules_test_1',
    '#label' => t('Test altering arguments by reference'),
    '#set' => 'rules_test_1',
    '#active' => TRUE,
    '#status' => 'fixed',
  );
  $condition1 = rules_use_condition('rules_test_condition_alter_arg', array(
      '#settings' => array('#argument map' => array('rules_test' => 'rules_test')),
      '#weight' => -1,
    ));
  $condition2 = rules_use_condition('rules_test_condition_check_for_altered_arg', array(
      '#settings' => array('#argument map' => array('rules_test' => 'rules_test')),
    ));
  $action = rules_use_action('rules_test_action_log_msg', array(
      '#settings' => array(
        'msg' => 'Rules Test 1 - Success!',
      ),
    ));
  $rules['rules_test_1'] += array(
    '#conditions' => array($condition1, $condition2, $condition2),
    '#actions' => array($action),
  );

  $rules['rules_test_2_1'] = array(
    '#type' => 'rule',
    '#name' => 'rules_test_2_1',
    '#label' => t('Test changing arguments per action'),
    '#set' => 'rules_test_2',
    '#active' => TRUE,
    '#weight' => -1,
    '#status' => 'fixed',
  );
  $action = rules_use_action('rules_test_change_arg', array(
    //  leave out this argument map, as it should take this as default value!
    //  '#settings' => array('#argument map' => array('rules_test' => 'rules_test')),
    ));
  $rules['rules_test_2_1'] += array(
    '#conditions' => array(),
    '#actions' => array($action),
  );

  $rules['rules_test_2_2'] = array(
    '#type' => 'rule',
    '#name' => 'rules_test_2_2',
    '#label' => t('Test changing arguments per action - check'),
    '#set' => 'rules_test_2',
    '#active' => TRUE,
    '#status' => 'fixed',
  );
  $condition = rules_use_condition('rules_test_condition_check_for_altered_arg', array(
      '#settings' => array('#argument map' => array('rules_test' => 'rules_test')),
    ));
  $action = rules_use_action('rules_test_action_log_msg', array(
      '#settings' => array(
        'msg' => 'Rules Test 2 - Success!',
      ),
    ));
  $rules['rules_test_2_2'] += array(
    '#conditions' => array($condition),
    '#actions' => array($action),
  );


  $rules['rules_test_4_1'] = array(
    '#type' => 'rule',
    '#name' => 'rules_test_4_1',
    '#label' => t('Test changing arguments per action'),
    '#set' => 'rules_test_4',
    '#active' => TRUE,
    '#status' => 'fixed',
  );
  $condition = rules_use_condition('rules_test_condition_check_for_altered_arg', array(
      '#settings' => array('#argument map' => array('rules_test_2' => 'rules_test')),
    ));
  $action = rules_use_action('rules_test_action_log_msg', array(
      '#settings' => array(
        'msg' => 'Rules Test 4 - Success!',
      ),
    ));
  $rules['rules_test_4_1'] += array(
    '#conditions' => array($condition),
    '#actions' => array($action),
  );


  $rules['rules_test_5_1'] = array(
    '#type' => 'rule',
    '#name' => 'rules_test_5_1',
    '#label' => t('Test adding a new variable'),
    '#set' => 'rules_test_5',
    '#active' => TRUE,
    '#status' => 'fixed',
  );
  $action = rules_use_action('rules_test_add_var', array(
      '#settings' => array('#argument map' => array('rules_test_var' => 'rules_test')),
      '#info' => array(
        'label' => 'Adds a new variable',
        'new variables' => array('rules_test_var' => array('type' => 'rules_test', 'label' => 'test')),
      ),
    ));
  $rules['rules_test_5_1'] += array(
    '#conditions' => array(),
    '#actions' => array($action),
  );

  return array(
    'rules' => $rules,
    'rule_sets' => array(
      'rules_test_1' => array(
        'label' => 'Rule Testing 1',
        'arguments' => array('rules_test' => array('type' => 'rules_test', 'label' => 'node')),
        'status' => 'fixed',
      ),
      'rules_test_2' => array(
        'label' => 'Rule Testing 2',
        'arguments' => array('rules_test' => array('type' => 'rules_test', 'label' => 'node')),
        'status' => 'fixed',
      ),
      'rules_test_4' => array(
        'label' => 'Rule Testing 4',
        'arguments' => array(
          'rules_test_1' => array('type' => 'rules_test', 'label' => 'node'),
          'rules_test_2' => array('type' => 'rules_test', 'label' => 'node', 'handler' => 'rules_test_4_load_data'),
        ),
        'status' => 'fixed',
      ),
      'rules_test_5' => array(
        'label' => 'Rule Testing 5',
        'arguments' => array(),
        'status' => 'fixed',
      ),
    ),
  );
}

