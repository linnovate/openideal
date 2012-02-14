<?php
// $Id: rules_admin.export.inc,v 1.1.2.9 2009/08/25 13:01:03 fago Exp $


/**
 * @file Rules Import/Export Admin UI
 */

rules_include('rules_admin');
module_load_include('inc', 'rules', 'rules.export');


/**
 * Imports a configuration of rules, rule sets, ..
 */
function rules_admin_form_import() {
  $form = array();
  $form['import'] = array(
    '#type' => 'textarea',
    '#title' => t('Configuration to import'),
    '#description' => t('Just paste your exported configuration here.'),
    '#rows' => 15,
    '#required' => TRUE,
  );
  $form['button'] = array('#type' => 'submit', '#weight' => 10, '#value' => t('Import'));
  return $form;
}

function rules_admin_form_import_submit($form, $form_state) {
  @eval('$import = '. $form_state['values']['import'] .';');
  if (isset($import) && is_array($import) && count($import)) {
    // Check whether it are workflow-ng rules
    if (rules_admin_import_workflow_ng($import)) {
      return;
    }

    foreach ($import as $item_type => $elements) {
      foreach ($elements as $name => $element) {
        //let the item type alter the data before it's saved
        rules_item_type_invoke($item_type, 'import', array(&$name, &$element));
        rules_item_save($item_type, $name, $element);
        drupal_set_message(t('Imported %label.', array('%label' => rules_get_element_label($element))));
      }
    }
    rules_clear_cache();
  }
  else {
    drupal_set_message(t('Import failed.'), 'error');
  }
}

/**
 * Item type callback: Customize to be imported rules
 */
function rules_item_rule_import(&$name, &$rule) {
  $rules = rules_get_configured_items('rules');

  if (!isset($rule['#status']) || $rule['#status'] == 'default') {
    if (!isset($rules[$name])) {
      // This default rule doesn't exist on this system, so we make it custom
      $rule['#status'] = 'custom';
    }
  }
  if (isset($rule['#status']) && $rule['#status'] == 'custom' && (!isset($rules[$name]) || $rules[$name]['#label'] != $rule['#label'] || $rules[$name]['#set'] != $rule['#set'])) {
    if (strpos($name, 'rules_') === 0) {
      // Generate a new unique name for this rule, but only if it's "our rule".
      $name = _rules_admin_rule_get_new_unique_name();
    }
    $rule['#status'] = 'custom';
  }
  rules_import_hook($rule);
}

/**
 * Item type callback: Customize to be imported rule sets
 */
function rules_item_rule_set_import(&$name, &$rule_set) {

  if (!isset($rule_set['status']) || $rule_set['status'] == 'default') {
    $sets = rules_get_configured_items('rule_sets');

    if (!isset($sets[$name])) {
      // This default rule set doesn't exist on this system, so we make it custom
      $rule_set['status'] = 'custom';
    }
  }
}

/**
 * Tries to import workflow-ng rules.
 *
 * @return TRUE, only if workflow-ng rules are detected.
 */
function rules_admin_import_workflow_ng($import) {
  foreach ($import as $name => $cfg) {
    if (count(element_children($cfg)) == count($cfg)) {
      // This is no workflow-ng rule, so exit
      return FALSE;
    }
    rules_include('rules');

    // Load the install and form include files, as there modules
    // may provide upgrade information.
    module_load_all_includes('install');
    rules_include('rules_forms');

    $rule = rules_import_workflow_ng_rule($name, $cfg);
    if ($rule) {
      // Handle rule format upgrades as the workflow-ng import just returns rules of the initial format
      $rule = rules_rule_format_upgrade($rule);

      rules_item_save('rules', $name, $rule);
      drupal_set_message(t('Successfully imported the workflow-ng rule %label.', array('%label' => $rule['#label'])));
    }
    else {
      drupal_set_message(t('Failed importing the workflow-ng rule %label.', array('%label' => $rule['#label'])), 'error');
    }
  }
  rules_clear_cache();
  return TRUE;
}

/**
 * Exports one or more configurations
 */
function rules_admin_form_export($form_state) {
  $form = array();
  if (!isset($form_state['export'])) {
    $form['export'] = array('#tree' => TRUE);
    foreach (rules_get_items() as $name => $info) {
      $items = rules_get_configured_items($name);
      if ($name === 'rules') {
        // Filter out rules within rule sets, we only want event-triggered,
        // standalone rules.
        $items = array_filter($items, 'rules_admin_is_event_rule');
        $info['label'] = t('Triggered rules');
      }
      $items = rules_admin_get_grouped_labels($items);
      if (count($items)) {
        $form['export'][$name] = array(
          '#type' => 'select',
          '#title' => t('Select the %label to export', array('%label' => $info['label'])),
          '#options' => array(0 => '-') + $items,
          '#multiple' => TRUE,
          '#default_value' => 0,
        );
      }
      else {
        $msg = t('There are no %label to be exported.', array('%label' => $info['label']));
        $form['export'][$name] = array('#value' => '<p>'. $msg .'</p>');
      }
    }
    // Add the possibility to export by category
    if ($tags = rules_admin_get_categories('rules') + rules_admin_get_categories('rule_sets')) {
      $form['export_by_tag'] = array(
        '#type' => 'select',
        '#title' => t('Export by category'),
        '#options' => array(0 => '-') + $tags,
        '#multiple' => TRUE,
        '#default_value' => 0,
      );
    }
    $form['button'] = array('#type' => 'submit', '#weight' => 10, '#value' => t('Export'));
  }
  else {
    //show a textarea containg the exported configs
    $form['result'] = array(
      '#type' => 'textarea',
      '#title' => t('Exported rule configurations'),
      '#description' => t('Copy these data and paste them into the import page, to import.'),
      '#rows' => 30,
      '#default_value' => var_export($form_state['export'], TRUE),
    );
  }
  return $form;
}

function rules_admin_form_export_submit($form, &$form_state) {
  $export = array();
  foreach (array_filter($form_state['values']['export']) as $item_type => $item_names) {
    $items = rules_get_configured_items($item_type);
    $export[$item_type] = array_intersect_key($items, array_filter($item_names));
  }
  if (isset($form_state['values']['export_by_tag']) && $tags_to_export = array_filter($form_state['values']['export_by_tag'])) {
    rules_admin_export_by_category($export, $tags_to_export);
  }
  if ($export = array_filter($export)) {
    $form_state['export'] = rules_export_items($export);
  }
  else {
    drupal_set_message(t('Please select the items to export.'), 'error');
  }
  $form_state['rebuild'] = TRUE;
}

/**
 * Adds in the rule items for the given tags.
 */
function rules_admin_export_by_category(&$export, $tags_to_export = array()) {
  // Search all items for the given tag.
  foreach (rules_get_items() as $item_type => $info) {
    $items = array_filter(rules_get_configured_items($item_type), 'rules_admin_element_filter');
    foreach ($items as $name => $item) {
      $categories = isset($item['categories']) ? $item['categories'] : (isset($item['#categories']) ? $item['#categories'] : array());
      if (array_intersect($tags_to_export, $categories)) {
        $export[$item_type][$name] = $item;
      }
    }
  }
}
