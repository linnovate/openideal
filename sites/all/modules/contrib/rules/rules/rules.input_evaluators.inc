<?php
// $Id: rules.input_evaluators.inc,v 1.1.2.9 2009/04/19 15:03:44 fago Exp $


/**
 * @file Rules input evaluation system.
 */

/**
 * Gets all available input evaluators, sorted by their weight.
 */
function rules_get_input_evaluators() {
  $evaluators = rules_gather_data('rules_evaluator');
  uasort($evaluators, '_rules_evaluator_sort');
  return $evaluators;
}

function _rules_evaluator_sort($a, $b) {
  return $a['weight'] < $b['weight'] ? -1 : ($a['weight'] > $b['weight'] ? 1 : 0);
}

/**
 * Prepares the available input evaluator on the element.
 */
function rules_prepare_input_evaluators(&$element, $variables) {

  $evaluators    = rules_get_input_evaluators();
  $eval_settings = rules_get_settings_to_evaluate($element);
  $settings      = &$element['#settings'];

  foreach ($evaluators as $base => $info) {
    foreach ($eval_settings as $name) {
      $setting = _rules_get_setting($settings, $name);
      if (isset($setting)) {
        // prepare the evaluation
        $data = call_user_func_array($base .'_prepare', array($setting, $variables));
        if (isset($data)) {
          $settings['#eval input'][$base][$name] = $data;
        }
      }
    }
  }
}

/**
 * Applies the available input evaluator on the element.
 */
function rules_apply_input_evaluators(&$element, &$state) {
  $settings = &$element['#settings'];

  if (isset($settings['#eval input'])) {
    foreach ($settings['#eval input'] as $base => $info) {
      foreach ($info as $name => $data) {
        $setting = &_rules_get_setting($settings, $name);
        $setting = call_user_func_array($base .'_apply', array($setting, $data, &$state));
        if ($setting === FALSE) {
          // application failed
          return FALSE;
        }
      }
    }
  }
}

/**
 * Gets all settings that should be evaluated - this are manually specified settings
 * and arguments of a data type like string, which has 'eval input' turned on.
 */
function rules_get_settings_to_evaluate($element) {
  $info = rules_get_element_info($element);
  $eval_args = $info['eval input'];

  foreach ($info['arguments'] as $name => $arg_info) {
    if ($data_type = rules_get_data_types($arg_info['type'])) {
      if (isset($data_type['eval input']) && $data_type['eval input']) {
        $eval_args[] = $name;
      }
    }
  }
  return $eval_args;
}

/**
 * Returns a setting of a given name. For settings sitting a nested array
 * the array keys may be separated by '|' in the name.
 */
function &_rules_get_setting(&$settings, $name) {
  $names = explode('|', $name);
  $setting = &$settings;
  foreach ($names as $name) {
    $setting = &$setting[$name];
  }
  return $setting;
}

/**
 * Returns help for the input evalutors as an array as suitable for drupal_render().
 */
function rules_input_evaluators_help($element, $variables) {
  $render = array();
  if (rules_get_settings_to_evaluate($element) && $variables) {
    rules_include('rules_forms');
    foreach (rules_get_input_evaluators() as $base => $info) {
      $render[$base] = array(
        '#type' => 'fieldset',
        '#title' => check_plain($info['label']),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => $info['weight'],
      );
      $return = call_user_func_array($base .'_help', array($variables));
      $render[$base]['content'] = is_array($return) ? $return : array('#value' => $return);
    }
  }
  return $render;
}

