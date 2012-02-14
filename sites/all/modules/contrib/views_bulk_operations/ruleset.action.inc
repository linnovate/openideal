<?php
// $Id: ruleset.action.inc,v 1.1.2.6 2010/06/23 06:13:06 kratib Exp $

function views_bulk_operations_ruleset_action_info() {
  if (!module_exists('rules')) return array();
  $actions = array();
  foreach (rules_get_configured_items('rule_sets') as $ruleset_key => $ruleset) {
    if (count($ruleset['arguments']) == 1) { // For now, we only accept rulesets with one argument (taken to be the 'type')
      $arg = key($ruleset['arguments']);
      $actions["views_bulk_operations_ruleset_action_{$ruleset_key}"] = array(
        'type' => $ruleset['arguments'][$arg]['type'],
        'parameters' => array('ruleset' => $ruleset_key),
        'description' => $ruleset['label'],
        'configurable' => FALSE,
        'rules_ignore' => TRUE,
        'callback' => 'views_bulk_operations_ruleset_action_one',
      );
    }
    else if (!count($ruleset['arguments'])) { // No argument: system action
      $actions["views_bulk_operations_ruleset_action_{$ruleset_key}"] = array(
        'type' => 'system',
        'parameters' => array('ruleset' => $ruleset_key),
        'description' => $ruleset['label'],
        'configurable' => FALSE,
        'rules_ignore' => TRUE,
        'callback' => 'views_bulk_operations_ruleset_action_zero',
      );
    }
    else { // General case
      $actions["views_bulk_operations_ruleset_action_{$ruleset_key}"] = array(
        'type' => 'system',
        'parameters' => array('ruleset' => $ruleset_key),
        'description' => $ruleset['label'],
        'configurable' => TRUE,
        'rules_ignore' => TRUE,
        'callback' => "views_bulk_operations_ruleset_action_{$ruleset_key}",
        'form properties' => array('#arguments'),
      );
      eval("
if (!function_exists('views_bulk_operations_ruleset_action_{$ruleset_key}')) {
  function views_bulk_operations_ruleset_action_{$ruleset_key}(&\$object, \$context) {
    \$context['ruleset'] = '$ruleset_key';
    return views_bulk_operations_ruleset_action(\$object, \$context);
  }

  function views_bulk_operations_ruleset_action_{$ruleset_key}_form(\$context) {
    \$context['ruleset'] = '$ruleset_key';
    return views_bulk_operations_ruleset_action_form(\$context);
  }

  function views_bulk_operations_ruleset_action_{$ruleset_key}_validate(\$form, \$form_state) {
    return views_bulk_operations_ruleset_action_validate(\$form, \$form_state);
  }

  function views_bulk_operations_ruleset_action_{$ruleset_key}_submit(\$form, \$form_state) {
    return views_bulk_operations_ruleset_action_submit(\$form, \$form_state);
  }
}
      ");
    }
  }
  return $actions;
}

function views_bulk_operations_ruleset_action_one(&$object, $context) {
  rules_invoke_rule_set($context['ruleset'], $object);
}

function views_bulk_operations_ruleset_action_zero(&$object, $context) {
  rules_invoke_rule_set($context['ruleset']);
}

function views_bulk_operations_ruleset_action(&$object, $context) {
  $ruleset = rules_get_rule_sets($context['ruleset']);
  $args = array();
  foreach ($ruleset['arguments'] as $key => $argument) {
    $args[$key] = @eval($context['args'][$key]);
  }
  rules_invoke_rule_set($context['ruleset'], $args);
}

function views_bulk_operations_ruleset_action_form($context) {
  $ruleset = rules_get_rule_sets($context['ruleset']);
  $form['#arguments'] = $ruleset['arguments'];
  foreach ($form['#arguments'] as $key => $argument) {
    $form[$key] = array(
      '#type' => 'textarea',
      '#title' => $argument['label'],
      '#description' => t('Argument of type %type. Enter PHP script that will return a value for this argument. 
                           The variables <code>$object</code> and <code>$context</code> are available to this script.',
        array('%type' => $argument['type'])
      ),
      '#default_value' => @$context['args'][$key],
    );
  }
  return $form;
}

function views_bulk_operations_ruleset_action_validate($form, $form_state) {
}

function views_bulk_operations_ruleset_action_submit($form, $form_state) {
  $submit = array();
  foreach ($form['#arguments'] as $key => $argument) {
    $submit['args'][$key] = $form_state['values'][$key];
  }
  return $submit;
}

