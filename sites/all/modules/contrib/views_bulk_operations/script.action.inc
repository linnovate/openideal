<?php
// $Id: script.action.inc,v 1.1.2.6 2010/09/19 17:45:27 kratib Exp $

function views_bulk_operations_script_action_info() {
  if (!module_exists('actions_permissions') && !user_access('administer site configuration')) return array();
  return array('views_bulk_operations_script_action' => array(
    'type' => 'system',
    'description' => t('Execute arbitrary PHP script'),
    'configurable' => TRUE,
    'hooks' => array('any' => TRUE),
  ));
}

function views_bulk_operations_script_action(&$object, $context) {
  if (!module_exists('actions_permissions') && !user_access('administer site configuration')) {
    global $user;
    $msg = 'An attempt to execute arbitrary PHP script action by user <a href="user/!uid">%user</a> was blocked because of lack of permission.';
    $arg = array('%user' => $user->name, '!uid' => $user->uid);
    drupal_set_message(t($msg, $arg), 'error', FALSE);
    watchdog('actions', $msg, $arg, WATCHDOG_ERROR);
    return;
  }
  @eval($context['script']);
}

function views_bulk_operations_script_action_form($context) {
  $form['script'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP script'),
    '#description' => t('Type the PHP snippet that will run upon execution of this action. You can use variables <code>&$object</code> and <code>$context</code> in your snippet.
                         Note that it is up to the script to save the object once it\'s done modifying it. Do NOT enclose the script within a &lt;?php&gt; tag.'),
    '#default_value' => @$context['script'],
  );
  return $form;
}

function views_bulk_operations_script_action_validate($form, $form_state) {
}

function views_bulk_operations_script_action_submit($form, $form_state) {
  return array(
    'script' => $form_state['values']['script'],
  );
}

