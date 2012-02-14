<?php
// $Id: rules_scheduler.rules_forms.inc,v 1.1.2.4 2009/08/28 22:05:43 fago Exp $

/**
 * @file
 * Contains forms for rules scheduler actions.
 */

/**
 * Changes the style of the action form, rule sets have to be selected from a select box.
 */
function rules_scheduler_action_delete_form($settings, &$form) {
  $form['settings']['ruleset']['#type'] = 'select';
  $rule_sets = array('' => '');
  foreach (rules_get_configured_items('rule_sets') as $name => $set_info) {
    $rule_sets[$name] = $set_info['label'] .' ('. $name .')';
  }
  $form['settings']['ruleset']['#options'] = $rule_sets;
  $form['settings']['ruleset']['#multiple'] = FALSE;
  $form['settings']['ruleset']['#required'] = FALSE;
  
  $form['settings']['task_identifier']['#required'] = FALSE;
}

/**
 * Validates the parameters of the delete action, at least one must be specified.
 */
function rules_scheduler_action_delete_validate($form, $form_state) {
  if ($form_state['values']['settings']['task_identifier'] === "" && empty($form_state['values']['settings']['ruleset'])) {
    form_set_error('settings', t('You have to specify at least one field.'));
  }
}

/**
 * Also add in the invoked rule set when exporting for features.
 */
function rules_scheduler_action_features_export(&$export, &$pipe, $settings, $element) {
  $set_name = $element['#info']['set'];
  if (!isset($export['features']['rules_sets'][$set_name])) {
    $export['features']['rules_sets'][$set_name] = $set_name;
    rules_features_process_set($set_name, $export, $pipe);
  }
}