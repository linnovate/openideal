<?php
// $Id: rules.export.inc,v 1.1.2.11 2010/08/05 14:51:41 fago Exp $

/**
 * @file
 * Provides export functionality and integrates with the features module.
 */

/**
 * Export all items given, where $export has to be an array of arrays
 * keyed by item types, containing the items keyed by their names.
 */
function rules_export_items($export, $module = NULL) {
  foreach (array_keys($export) as $item_type) {
    // Allow item specific adaption before exporting
    foreach ($export[$item_type] as $item_name => $item) {
      rules_item_type_invoke($item_type, 'export', array($item_name, &$export[$item_type][$item_name], &$export, $module));
    }
  }
  return $export;
}

/**
 * Item type callback: When exporting a rule set, add its rules to the export.
 */
function rules_item_rule_set_export($set_name, &$rule_set, &$export, $module = NULL) {
  $rules = rules_get_configured_items('rules');
  foreach ($rules as $name => $rule) {
    if ($rule['#set'] == $set_name) {
      $export['rules'][$name] = $rule;
      rules_item_type_invoke('rules', 'export', array($name, &$export['rules'][$name], &$export, $module));
    }
  }
  if (!empty($module)) {
    // Add the module name as category.
    $rule_set += array('categories' => array());
    $rule_set['categories'][$module] = $module;
    $rule_set['status'] = 'default';
  }
}


/**
 * Prefix exported rules with the module name (if given) and change the status to default.
 * If used through the features integration the module name is given.
 */
function rules_item_rule_export($rule_name, &$rule, &$export, $module = NULL) {
  static $counter = 0;

  // Be sure the exported rule has the right version specified, which is
  // important if the export is used as default.
  $rule['#version'] = 6003;

  if (!empty($module)) {
    // Change #status to 'default'.
    $rule['#status'] = 'default';
    // Add the module name as category.
    if (empty($rule['#categories']) || !in_array($module, $rule['#categories'])) {
      $rule['#categories'][$module] = $module;
    }

    // We increase the counter in any case. As already renamed rules are passed
    // in first, this makes sure to not overwrite the names of those even if
    // they aren't saved yet.
    $counter++;

    if (strpos($rule_name, $module) !== 0) {
      // Make sure the name isn't already taken.
      $rules = rules_get_configured_items('rules');
      while (isset($rules[$module .'_'. $counter])) {
        $counter++;
      }

      // Rename the rule.
      $export['rules'][$module .'_'. $counter] = $rule;
      // Unset the non-prefixed item.
      unset($export['rules'][$rule_name]);
    }
  }
}

/**
 * Implementation of hook_features_export_options() for tags.
 */
function rules_categories_features_export_options() {
  rules_include('rules_admin');
  return rules_admin_get_categories('rules') + rules_admin_get_categories('rule_sets');
}

/**
 * Implementation of hook_features_export_render() for categories.
 */
function rules_categories_features_export_render($module = 'foo', $data) {
  $code = array();
  $code[] = '  $rules = array();';
  $code[] = '';

  // Build the usual rules $export structure for sets and categories.
  $rules_export = array();
  $items = rules_get_configured_items('rule_sets');
  rules_include('rules_admin');
  module_load_include('inc', 'rules_admin', 'rules_admin.export');
  rules_admin_export_by_category($rules_export, $data);

  $module = strpos($module, '_features_comparison') === 0 ? '' : $module;
  $export = rules_export_items($rules_export, $module);
  $defaults = features_var_export($export, '  ');

  return array('rules_defaults' => "  return $defaults;");
}

/**
 * Implementation of hook_features_export() for categories.
 */
function rules_categories_features_export($data, &$export, $module_name = '') {
  rules_include('rules_admin');
  module_load_include('inc', 'rules_admin', 'rules_admin.export');

  $rules_export = array();
  rules_admin_export_by_category($rules_export, $data);

  // Process the rules and sets to be exported for dependencies.
  $pipe = array();
  $rules_export += array('rules' => array(), 'rule_sets' => array());
  foreach ($rules_export['rules'] as $rule) {
    rules_features_process_rule($rule, $export, $pipe);
  }
  foreach ($rules_export['rule_sets'] as $set_name => $set) {
    rules_features_process_set($set_name, $export, $pipe);
  }

  $export['features']['rules_categories'] = drupal_map_assoc($data);
  $export['dependencies']['rules'] = 'rules';

  return $pipe;
}

/**
 * Implementation of hook_features_revert() for rule categories.
 *
 * @param $module
 * name of module to revert content for
 */
function rules_categories_features_revert($module = NULL) {
  rules_include('rules_admin');
  module_load_include('inc', 'rules_admin', 'rules_admin.export');
  $rules_export = array();
  rules_admin_export_by_category($rules_export, array($module));

  // Delete Normal / Overridden rules items that are defined in code
  foreach ($rules_export as $item_type => $items) {
    foreach ($items as $item_name => $item) {
      rules_item_delete($item_type, $item_name);
    }
  }
  rules_clear_cache();
  return TRUE;
}


/**
 * Retrieves all necessary module dependencies for a list of rule sets.
 */
function rules_features_process_set($set_name, &$export, &$pipe) {
  $rules = rules_get_configured_items('rules');
  foreach ($rules as $key => $rule) {
    if ($rule['#set'] == $set_name) {
      rules_features_process_rule($rule, $export, $pipe);
    }
  }
}

/**
 * Processes a rule and identifes needed components or dependencies.
 */
function rules_features_process_rule($rule, &$export, &$pipe) {
  $dependencies = array();
  // If it is an event-triggered rule, add event dependencies
  if (strpos($rule['#set'], 'event_') === 0) {
    $event = substr($rule['#set'], strlen('event_'));
    if ($module = rules_features_providing_module('event_info', $event)) {
      $export['dependencies'][$module] = $module;
    }
  }
  foreach (array('condition', 'action') as $type) {
    _rules_features_process_rule((array)$rule['#'. $type .'s'], $export, $pipe);
  }
}

/**
 * Helper to recursively process all elements of a rule.
 */
function _rules_features_process_rule($element, &$export, &$pipe) {
  if (isset($element['#type']) && isset($element['#name']) && $module = rules_features_providing_module($element['#type'] .'_info', $element['#name'])) {
    $export['dependencies'][$module] = $module;
  }
  // Invoke the features_export callback if any.
  $element += array('#settings' => array());
  rules_element_invoke($element, 'features_export', array(&$export, &$pipe, $element['#settings'], $element), FALSE);

  // Add used input evaluators.
  $element['#settings'] += array('#eval input' => array());
  foreach ($element['#settings']['#eval input'] as $base => $info) {
    if ($module = rules_features_providing_module('evaluator', $base)) {
      $export['dependencies'][$module] = $module;
    }
  }

  // Recurse
  foreach (element_children($element) as $key) {
    _rules_features_process_rule((array)$element[$key], $export, $pipe);
  }
}


/**
 * Retrieves the providing module for any items defined with rules hooks.
 *
 * @param $hook
 *   The name of the hook without the 'rules_' prefix.
 * @param $name
 *   The name of the item provided by the given hook.
 * @return
 *   The module name or FALSE if it can't be found.
 */
function rules_features_providing_module($hook, $name) {
  static $map = array();

  if (!isset($map[$hook])) {
    $map[$hook] = array();
    foreach (module_implements('rules_'. $hook ) as $module) {
      if ($info = module_invoke($module, 'rules_'. $hook)) {
        $map[$hook] += array_combine(array_keys($info), array_fill(0, count($info), $module));
      }
    }
  }
  return isset($map[$hook][$name]) ? $map[$hook][$name] : FALSE;
}

