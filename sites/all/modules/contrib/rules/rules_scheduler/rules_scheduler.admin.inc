<?php
// $Id: rules_scheduler.admin.inc,v 1.1.2.1 2009/07/13 13:34:27 fago Exp $

/**
 * @file
 * Admin form for scheduling
 */
 
function rules_scheduler_scheduling() {
  return drupal_get_form('rules_scheduler_form');
}

/**
 * Builds the scheduling page with a form for manual scheduling and deletion of tasks.
 */
function rules_scheduler_form($form_state) {
  $form = array();
  // Display view for all scheduled tasks
  if (module_exists('views')) {
    $task_list = views_embed_view('rules_scheduler');
  }
  else {
    $task_list = t('To display scheduled tasks you have to install the <a href="http://drupal.org/project/views">Views</a> module.');
  }
  $form['task_view'] = array(
    '#type' => 'markup',
    '#value' => $task_list,
  );
  
  // form fieldset for manual scheduling
  $form['manual_scheduling'] = array(
    '#type' => 'fieldset',
    '#title' => t('Manual scheduling of rule sets without arguments'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $rulesets = rules_scheduler_rulesets_without_arguments();
  if (empty($rulesets)) {
    $form['manual_scheduling']['#description'] = t('There are currently no rule sets without arguments available.');
  }
  else {
    $form['manual_scheduling']['ruleset_manual'] = array(
      '#title' => t('Rule set name'),
      '#type' => 'select',
      '#options' => $rulesets,
      '#multiple' => FALSE,
    );
    $form['manual_scheduling']['task_identifier'] = array(
      '#title' => t('Identifier'),
      '#type' => 'textfield',
      '#description' => t('User provided string to identify the task. Existing tasks for this rule set with the same identifier will be replaced.'),
    );
    $rules_date = new rules_data_type_date();
    $form['manual_scheduling']['date'] = $rules_date->get_default_input_form(array('label' => 'Execution date'), NULL);
    $form['manual_scheduling']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Schedule'),
      '#submit' => array('rules_scheduler_manual_submit'),
      '#validate' => array('rules_scheduler_manual_validate'),
    );
  }
  
  // form fieldset for deleting by rule set name
  $result = db_query("SELECT DISTINCT set_name FROM {rules_scheduler}");
  $ruleset_names = array();
  while ($name = db_fetch_array($result)) {
    $ruleset_names[$name['set_name']] = $name['set_name'];
  }
  
  $form['delete_by_ruleset_name'] = array(
    '#type' => 'fieldset',
    '#title' => t('Delete tasks by rule set name'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if (empty($ruleset_names)) {
    $form['delete_by_ruleset_name']['#description'] = t('There are currently no scheduled tasks available to delete.');
  }
  else {
    $form['delete_by_ruleset_name']['ruleset_delete'] = array(
      '#title' => t('Rule set name'),
      '#type' => 'select',
      '#options' => $ruleset_names,
      '#multiple' => FALSE,
    );
    $form['delete_by_ruleset_name']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('rules_scheduler_delete_submit'),
    );
  }
  
  return $form;
}

/**
 * Retrieves all rule sets that do not have arguments attached.
 */
function rules_scheduler_rulesets_without_arguments() {
  $rule_sets = array();
  foreach (rules_get_configured_items('rule_sets') as $name => $set_info) {
    if (empty($set_info['arguments'])) {
      $rule_sets[$name] = $set_info['label'] .' ('. $name .')';
    }
  }
  return $rule_sets;
}

/**
 * Submit handler for manual scheduling of tasks.
 */
function rules_scheduler_manual_submit($form, &$form_state) {
  rules_scheduler_schedule_task( $form_state['values']['ruleset_manual'], $form_state['values']['date'], $form_state['values']['task_identifier']);
  drupal_set_message(t('The rule set %name has been scheduled on %date (GMT).', array('%name' => $form_state['values']['ruleset_manual'], '%date' => $form_state['values']['date'])));
}

/**
 * Submit handler for deletion/cancellation of future scheduled tasks.
 */
function rules_scheduler_delete_submit($form, &$form_state) {
  module_load_include('inc', 'rules_scheduler', 'rules_scheduler.rules');
  rules_scheduler_action_delete(NULL, $form_state['values']['ruleset_delete']);
  drupal_set_message(t('All tasks associated with %ruleset have been deleted.', array('%ruleset' => $form_state['values']['ruleset_delete'])));
}

/**
 * Confirmation form for deleting single tasks.
 */
function rules_scheduler_delete_task(&$form_state, $task) {
  $form = array();
  $form_state['task'] = $task;

  $path = array();
  $path['path'] = isset($_GET['destination']) ? $_GET['destination'] : RULES_SCHEDULER_PATH;
  
  $title = t('Are you sure you want to delete task %tid?', array('%tid' => $task['tid']));
  if (!empty($task['identifier'])) {
    $msg = t('This task refers to the custom identifier %id and rule set %ruleset, it will be executed on %date. The delete action cannot be undone.', array(
      '%ruleset' => $task['set_name'],
      '%id' => $task['identifier'],
      '%date' => format_date(rules_gmstrtotime($task['date'])),
    ));
  }
  else {
    $msg = t('This task refers to the rule set %ruleset and will be executed on %date. The delete action cannot be undone.', array(
      '%ruleset' => $task['set_name'],
      '%id' => $task['identifier'],
      '%date' => $task['date'],
    ));
  }
  return confirm_form($form, $title, $path, $msg, t('Delete'), t('Cancel'));
}

/**
 * Submit handler for deleting single tasks.
 */
function rules_scheduler_delete_task_submit($form, &$form_state) {
  db_query("DELETE FROM {rules_scheduler} WHERE tid = %d", $form_state['task']['tid']);
  drupal_set_message(t("Task %label has been deleted.", array('%label' => $form_state['task']['tid'])));
  $form_state['redirect'] = RULES_SCHEDULER_PATH;
}
