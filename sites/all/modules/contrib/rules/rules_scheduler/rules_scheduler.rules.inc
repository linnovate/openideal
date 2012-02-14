<?php
// $Id: rules_scheduler.rules.inc,v 1.1.2.7 2009/09/06 16:24:13 klausi Exp $


/**
 * @file
 * Rules integration for the rules scheduler module.
 */

/**
 * Implementation of hook_rules_action_info().
 */
function rules_scheduler_rules_action_info() {
  $items = array();
  // Add actions for all rule sets.
  foreach (rules_get_configured_items('rule_sets') as $name => $set_info) {
    $items['rules_action_schedule_set_'. $name] = $set_info + array(
      'module' => 'Rule Scheduler',
      'base' => 'rules_scheduler_action',
      'set' => $name,
    );
    $item = &$items['rules_action_schedule_set_'. $name];
    $item['label'] = t('Schedule "@set"', array('@set' => $set_info['label']));
    $item += array('arguments' => array());
    $date_arg = array('task_date' => array(
        'type' => 'date',
        'label' => t('Scheduled evaluation date'),
      ));
    $item['arguments'] = array_merge($date_arg, $item['arguments']);
    $identifier_arg = array('task_identifier' => array(
        'type' => 'string',
        'label' => t('Identifier'),
        'description' => t('User provided string to identify the task. Existing tasks for this rule set with the same identifier will be replaced.'),
      ));
    $item['arguments'] = array_merge($identifier_arg, $item['arguments']);
  }
  
  // Add action to delete scheduled tasks
  $items['rules_action_delete_scheduled_set'] = array(
    'label' => t('Delete scheduled tasks'),
    'module' => 'Rule Scheduler',
    'base' => 'rules_scheduler_action_delete',
    'arguments' => array(
      'task_identifier' => array(
        'type' => 'string',
        'label' => t('Identifier'),
        'description' => t('All tasks that are annotated with this user provided identifier will be deleted.'),
      ),
      'ruleset' => array(
        'type' => 'string',
        'label' => t('Rule set'),
        'description' => t('All tasks that execute this rule set will be deleted.'),
      ),
    ),
  );
  return $items;
}

/**
 * Base action implementation for scheduling all rule sets
 */
function rules_scheduler_action() {
  $args     = func_get_args();
  // Remove additional information from the arguments array
  $state    = array_pop($args);
  $element  = array_pop($args);
  $settings = array_pop($args);
  // Remove task ID and date arguments
  $task_id = array_shift($args);
  $date = array_shift($args);
  // Remove task ID and date from info entry
  array_shift($element['#info']['arguments']);
  array_shift($element['#info']['arguments']);
  
  rules_scheduler_schedule_task($element['#info']['set'], $date, $task_id, $element['#info']['arguments'], $args, TRUE);
}

/**
 * Help for the base action implementation.
 */
function rules_scheduler_action_help() {
  return t("The evaluation of the rule set is going to be scheduled by cron. So make sure you have configured cron correctly by checking your site's !status.", array('!status' => l('Status report', 'admin/reports/status'))) .' '. t('Also note that the scheduling time accuracy depends on your configured cron interval.');
}

/**
 * Action implementation for deleting already scheduled tasks.
 */
function rules_scheduler_action_delete($task_identifier = NULL, $rule_set_name = NULL) {
  if (!empty($rule_set_name)) {
    if (!empty($task_identifier)) {
      db_query("DELETE FROM {rules_scheduler} WHERE set_name = '%s' AND identifier = '%s'", $rule_set_name, $task_identifier);
    }
    else {
      db_query("DELETE FROM {rules_scheduler} WHERE set_name = '%s'", $rule_set_name);
    }
  }
  else {
    if (!empty($task_identifier)) {
      db_query("DELETE FROM {rules_scheduler} WHERE identifier = '%s'", $task_identifier);
    }
  }
}

/**
 * Help for the delete action.
 */
function rules_scheduler_action_delete_help() {
  return t('This action allows you to cancel scheduled tasks that are waiting for future execution.') .' '. t('They can be addressed by an identifier or by the rule set name, if both are specified only tasks fulfilling both requirements will be deleted.');
}
