<?php
// $Id: rules_admin.inc,v 1.1.2.13 2009/11/02 11:34:33 fago Exp $


/**
 * @file Rules Admin UI - Utility functions
 */
require_once drupal_get_path('module', 'rules_admin') .'/rules_admin.rule_proxy.inc';

/**
 * Gets the variable, which is assigned (mapped) to the given argument.
 */
function rules_admin_element_map_variable_name($name, $element) {
  if (!isset($element['#settings']['#argument map'])) {
    //per default try the the same name
    return $name;
  }
  $map = $element['#settings']['#argument map'];
  return isset($map[$name]) ? $map[$name] : $name;
}

/**
 * Shows the element help
 */
function rules_admin_element_help(&$form, $element) {
  $info = rules_retrieve_element_info($element);
  if (isset($info['help']) || isset($element['help'])) {
    $help = isset($element['help']) ? $element['help'] : $info['help'];
  }
  else if (isset($element['#type']) && in_array($element['#type'], array('action', 'condition'))) {
    rules_include('rules_forms');
    $help = rules_element_invoke($element, 'help', array());
  }
  if (isset($help) && $help) {
    $form['help'] = array(
      '#type' => 'fieldset',
      '#title' => t('Description'),
      '#description' => $help,
    );
  }
}

/**
 * Gets all compatible sets, which means that all arguments are available in the other set too
 */
function rules_admin_get_compatible_sets($rule, $sets) {
  if (isset($rule['#set']) && $rule['#set']) {
    $set = rules_get_rule_sets($rule['#set']);
    foreach ($sets as $key => $this_set) {
      if (array_diff(array_keys($set['arguments']), array_keys($this_set['arguments']))) {
        unset($sets[$key]);
      }
    }
  }
  return $sets;
}

/**
 * Gets an array of element labels grouped by modules
 *
 * @param $elements
 *   An array of info elements, e.g. as returned from rules_get_events().
 * @param $key
 *   The key for which the value is used for grouping the elements.
 */
function rules_admin_get_grouped_labels($elements, $key = 'module') {
  $grouped = array();
  $ungrouped = array();

  foreach (array_filter($elements, 'rules_admin_element_filter') as $name => $element) {
    if (isset($element[$key])) {
      $grouped[$element[$key]][$name] = rules_get_element_label($element);
    }
    else {
      $ungrouped[$name] = rules_get_element_label($element);
    }
  }
  foreach ($grouped as $name => $group) {
    asort($grouped[$name]);
  }
  ksort($grouped);
  asort($ungrouped);
  return $ungrouped + $grouped;
}

/**
 * This is used for filtering the rules, sets, action and conditions that should be shown.
 * Rules and sets are filtered when they are fixed and the variable 'rules_show_fixed'
 * is  not set to on. Actions and conditions are filtered out if they are marked as
 * being hidden.
 *
 * @return TRUE, when the element should be kept. Else FALSE.
 */
function rules_admin_element_filter($element) {
  if (!variable_get('rules_show_fixed', FALSE)) {
    if (isset($element['#status']) && $element['#status'] == 'fixed') {
      return FALSE;
    }
    if (isset($element['status']) && $element['status'] == 'fixed') {
      return FALSE;
    }
  }
  if (isset($element['hidden']) && $element['hidden']) {
    return FALSE;
  }
  return TRUE;
}

/**
 * Determines if the given data type is valid for the given argument.
 *
 * @param $data_type
 *   The data type to check.
 * @param $arg_type
 *   The arguments needed data type.
 *
 * @return
 *   True or false.
 */
function rules_admin_is_valid_data_type($data_type, $arg_type) {
  if (is_array($arg_type)) {
    foreach ($arg_type as $type) {
      // We have multiple options to check for.
      if (rules_admin_is_valid_data_type($data_type, $type)) {
        return TRUE;
      }
    }
    return FALSE;
  }
  if ($arg_type == '*') {
    return TRUE;
  }
  return $arg_type == $data_type;
}

/**
 * Filters the info about elements (actions, conditions), so that only elements
 * compatible with the given variables are kept, which means suiting variables
 * for all arguments are available, so that the action/condition can be
 * configured.
 * Additionally, arguments having no suiting variables are recorded
 * in the 'unsatisfied arguments' slot of the element in $infos.
 */
function rules_admin_filter_info($variables, &$infos) {
  $filtered = array();
  foreach ($infos as $key => $info) {
    if (isset($info['arguments']) && is_array($info['arguments'])) {
      foreach ($info['arguments'] as $name => $argument_info) {
        if (!rules_admin_argument_satisifable($argument_info, $variables)) {
          // Get the unsatisfied arguments.
          $infos[$key] += array('unsatisfied arguments' => array());
          $infos[$key]['unsatisfied arguments'][$name] = $argument_info['label'];
        }
      }
      if (empty($infos[$key]['unsatisfied arguments'])) {
        $filtered[$key] = $info;
      }
    }
    else {
      $filtered[$key] = $info;
    }
  }
  return $filtered;
}

/**
 * Determines whether the argument can be configured properly, so whether
 * there are matching variables available or needed.
 */
function rules_admin_argument_satisifable($info, $variables) {
  if ($info['type'] == '*') {
    return TRUE;
  }
  if (($type_info = rules_get_data_types($info['type']))) {
    // For backward compatibility use_input_form is interpreted as !identifiable if it's unset.
    if (!empty($type_info['use_input_form']) || (!isset($type_info['use_input_form']) && empty($type_info['identifiable']))) {
      // Argument value will be set on configuration time.
      return TRUE;
    }
  }
  if (count(rules_admin_map_get_possible_arguments($info, $variables))) {
    return TRUE;
  }
  return FALSE;
}

/**
 * Gets the possible variables (= of the same entity) for an argument
 */
function rules_admin_map_get_possible_arguments($arg_info, $variables) {
  $matches = array();
  foreach ($variables as $name => $info) {
    if (rules_admin_is_valid_data_type($info['type'], $arg_info)) {
      $matches[$name] = $info['label'];
    }
  }
  return $matches;
}

/**
 * Returns an array of all existing categories
 *
 * @param $item_type
 *   Either 'rules' or 'rule_sets'
 *
 * @return
 *   An array of categories to use.
 */
function rules_admin_get_categories($item_type) {
  $categories = array();
  foreach (array_filter(rules_get_configured_items($item_type), 'rules_admin_element_filter') as $item) {
    if ($item_type == 'rules' && isset($item['#categories']) && is_array($item['#categories'])) {
      $categories = $categories + drupal_map_assoc($item['#categories']);
    }
    if ($item_type == 'rule_sets' && isset($item['categories']) && is_array($item['categories'])) {
      $categories = $categories + drupal_map_assoc($item['categories']);
    }
  }
  return array_filter($categories);
}

/**
 * Gets an unique name for a newly added rule.
 */
function _rules_admin_rule_get_new_unique_name() {
  $id = variable_get('rules_counter', 0);
  $id++;
  variable_set('rules_counter', $id);
  return 'rules_'. $id;
}

/**
 * Gets the label for the info of the given element by apply label callbacks.
 * Note that this is also used for argument infos.
 *
 * @param $cfirst
 *   If TRUE, capitalize the first letter, if FALSE lower case the first letter.
 * @param $element
 *   The element having or containing this label. Pass by referenec to allow
 *   callbacks to alter some info, used by rules_core_node_label_callback().
 *
 * @return An array of changed properties of the given info
 */
function _rules_admin_get_label(&$form_state, $info, &$element, $value, $cfirst = NULL) {
  $info_changes = array();

  if (drupal_strtolower($info['label']) != drupal_strtolower($value)) {
    //label has been customized
    $info_changes['label'] = $value;
    $info_changes['label callback'] = FALSE;
  }
  else if (isset($info['label callback']) && $info['label callback'] && function_exists($info['label callback'])) {
    $argument_labels = rules_admin_get_argument_labels($form_state['proxy'], $element);
    $info_changes['label'] = $info['label callback']($element['#settings'], $argument_labels, $element);
    // Labels are treated as user input, so we decode any entites.
    $info_changes['label'] = decode_entities($info_changes['label']);
    if (isset($cfirst)) {
      $function = $cfirst ? 'drupal_strtoupper' : 'drupal_strtolower';
      $info_changes['label'] = $function(drupal_substr($info_changes['label'], 0, 1)) . drupal_substr($info_changes['label'], 1);
    }
  }
  return $info_changes;
}

/**
 * Gets the labels of the variables, which are configured to be passed as arguments
 * to the given element.
 */
function rules_admin_get_argument_labels($proxy, $element, $prefix = '@') {
  $labels = array();
  $names  = rules_get_mapped_argument_names($element);
  $vars   = $proxy->get_available_variables($element['#id']);
  $labels = array();
  foreach ($names as $argument_name => $variable_name) {
    if (isset($vars[$variable_name]['label'])) {
      $labels[$prefix . $argument_name] = $vars[$variable_name]['label'];
    }
  }
  return $labels;
}

/**
 * Returns a table of rules filtered by the given parameters
 *
 * Supported parameters: fixed, category, set, active.
 * Category and set may be set to NULL to hide their columns.
 */
function rules_admin_overview_table($params) {
  $rules = rules_get_configured_items('rules');
  _rules_element_defaults($rules);
  rules_sort_children($rules);

  //set parameter defaults
  $params += array('category' => NULL, 'set' => NULL, 'active' => TRUE, 'fixed' => FALSE);
  extract($params);

  $is_set = $set && strpos($set, 'event_') !== 0;

  $header = array(t('Label'), $is_set ? t('Set') : t('Event'), t('Category'), t('Status'), t('Operations'));
  $rows = array();

  foreach (element_children($rules) as $name) {
    $rule = $rules[$name];
    _rules_element_defaults($rule);

    if ((!$category || in_array($category, $rule['#categories'])) && (!$set && strpos($rule['#set'], 'event_') === 0 || $rule['#set'] == $set) && $rule['#active'] == $active && ($rule['#status'] == 'fixed') == $fixed) {
      $path = RULES_ADMIN_RULE_PATH .'/'. $name;
      $ops = array();
      if ($rule['#status'] == 'custom') {
        $ops[] = l(t('delete'), $path .'/delete', array('query' => drupal_get_destination()));
      }
      else if ($rule['#status'] == 'altered') {
        $ops[] = l(t('revert'), $path .'/revert', array('query' => drupal_get_destination()));
      }
      $ops[] = l(t('clone'), $path .'/clone', array(), drupal_get_destination());
      $categories = array_map('check_plain', $rule['#categories']);

      $set_info = rules_get_rule_sets($rule['#set']);
      // If the set info is missing, the module providing the event is gone and the
      // rule can't be edited.
      $rows[] = array(
        $set_info ? l($rule['#label'], $path .'/edit') : check_plain($rule['#label']),
        $set_info ? check_plain(rules_get_element_label($set_info)) : array('class' => 'error', 'data' => check_plain($rule['#set'])),
        implode(', ', $categories),
        theme('rules_admin_configuration_status', $rule['#status']),
        implode(' ', $ops),
      );
      if (!$set_info) {
        rules_handle_error_msg("%set can't be found. Probably the providing module has been deactivated.", array('%set' => $rule['#set']));
      }
    }
  }

  if (count($rows)) {
    return array('#value' => theme('table', $header, $rows, array('class' => 'rules-rule-configurations')));
  }
  return array('#value' => '<p>'. t('None') .'</p>');
}

/**
 * Returns the given status rendered as html.
 */
function theme_rules_admin_configuration_status($status) {
  if ($status == 'altered') {
    $help = t('This rule has been provided by a module, but has been modified.');
    return '<span title="'. $help .'">'. t('Modified') .'</span>';
  }
  else if ($status == 'fixed') {
    $help = t("This rule has been provided by a module and can't be customized.");
    return '<span title="'. $help .'">'. t('Fixed') .'</span>';
  }
  else if ($status == 'custom') {
    $help = t('A custom defined rule.');
    return '<span title="'. $help .'">'. t('Custom') .'</span>';
  }
  $help = t('This rule has been provided by a module.');
  return '<span title="'. $help .'">'. t('Default') .'</span>';
}

/**
 * Fixes the breadcrumb on the rule edit/delete/revert
 */
function rules_admin_fix_breadcrumb($rule, $name) {
  $crumbs = array();
  if (strpos($rule['#set'], 'event_') !== 0) {
    $crumbs[] = l(t('Rule sets'), RULES_ADMIN_SET_PATH);
    $set_info = rules_get_rule_sets($rule['#set']);
    $crumbs[] = l($set_info['label'], RULES_ADMIN_SET_PATH .'/'. $rule['#set']);
  }
  else {
    $crumbs[] = l(t('Triggered rules'), RULES_ADMIN_TRIGGER_PATH);
  }
  if (arg(5)) {
    // Also add a crumb to the rule itself.
    array_unshift($crumbs, l(t('Rules'), RULES_ADMIN_PATH));
    $crumbs[] = l($rule['#label'], RULES_ADMIN_RULE_PATH .'/'. $name);
  }
  rules_admin_add_crumbs($crumbs);
}

/**
 * Adds some further crumbs to the breadcrumb.
 * Due to the bug http://drupal.org/node/430304, some pages might have no breadcrumb.
 */
function rules_admin_add_crumbs($crumbs) {
  if ($breadcrumb = drupal_get_breadcrumb()) {
    $breadcrumb = array_merge($breadcrumb, $crumbs);
    drupal_set_breadcrumb($breadcrumb);
  }
}

function rules_admin_is_event_rule($rule) {
  return !empty($rule['#set']) && strpos($rule['#set'], 'event_') === 0;
}