<?php
// $Id: rules.rules.inc,v 1.1.2.44 2010/06/25 11:15:57 fago Exp $


/**
 * @file rules integration for the rules module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_rules_data_type_info().
 */
function rules_rules_data_type_info() {
  return array(
    'string' => array(
      'label' => t('string'),
      'class' => 'rules_data_type_string',
      'identifiable' => FALSE,
      'eval input' => TRUE,
      'token type' => 'string',
    ),
    'number' => array(
      'label' => t('number'),
      'class' => 'rules_data_type_number',
      'identifiable' => FALSE,
      'eval input' => TRUE,
      'token type' => 'number',
    ),
    'date' => array(
      'label' => t('date'),
      'class' => 'rules_data_type_date',
      'identifiable' => FALSE,
      'eval input' => TRUE,
      'token type' => FALSE,
    ),
    'boolean' => array(
      'label' => t('truth value'),
      'class' => 'rules_data_type_boolean',
      'identifiable' => FALSE,
      'eval input' => TRUE,
      'token type' => FALSE,
    ),
    'value' => array(
      'label' => t('a fixed value'),
      'class' => 'rules_data_type',
      'identifiable' => FALSE,
      'hidden' => TRUE,
    ),
  );
}

/**
 * Rules string data type
 */
class rules_data_type_string extends rules_data_type {

  /**
   * Gets the input form for the data
   * Implement it, if your data is not identifiable.
   */
  function get_default_input_form($info, $value, &$form_state) {
    $info += array('long' => FALSE, 'required' => TRUE, 'description' => '');
    return array(
      '#type' => $info['long'] ? 'textarea' : 'textfield',
      '#title' => $info['label'],
      '#description' => $info['description'],
      '#required' => $info['required'],
      '#default_value' => $value,
    );
  }
}

/**
 * Rules number data type
 */
class rules_data_type_number extends rules_data_type_string {
  function check_value($info, $value) {
    return (float)$value;
  }
}

/**
 * Rules date data type
 */
class rules_data_type_date extends rules_data_type_string {
  function get_default_input_form($info, $value, &$form_state) {
    $info += array('long' => FALSE, 'required' => TRUE, 'description' => '');
    $value = isset($value) ? $value : gmdate('Y-m-d H:i:s', time());
    $info['description'] = $info['description'] .' '. t('Format: %format or other values in GMT known by the PHP !strtotime function like "+1 day". ', array('%format' => gmdate('Y-m-d H:i:s', time() + 86400), '!strtotime' => l('strtotime()', 'http://php.net/strtotime')));
    $info['description'] .= '<br />'. t('You may also enter a timestamp in GMT. E.g. use !code together with the PHP input evalutor to specify a date one day after the evaluation time. ', array('!code' => '<pre>'. check_plain('<?php echo time() + 86400 * 1; ?>') .'</pre>'));
    return array(
      '#type' => $info['long'] ? 'textarea' : 'textfield',
      '#title' => $info['label'],
      '#description' => $info['description'],
      '#required' => $info['required'],
      '#default_value' => $value,
    );
  }

  function check_value($info, $value) {
    if (is_numeric($value)) {
      $value = gmdate('Y-m-d H:i:s', $value);
    }
    else if (is_string($value)) {
      $value = gmdate('Y-m-d H:i:s', rules_gmstrtotime($value));
    }
    if (is_string($value) && preg_match(RULES_DATE_REGEX_LOOSE, $value)) {
      return $value;
    }
    rules_log(t('The argument %label is no valid date.', array('%label' => $info['label'])));
  }
}

/**
 * Rules boolean data type
 */
class rules_data_type_boolean extends rules_data_type_string {
  function check_value($info, $value) {
    return (boolean)$value;
  }

  function get_default_input_form($info, $value, &$form_state) {
    $info += array('long' => FALSE, 'required' => TRUE, 'description' => '');
    $info['description'] = $info['description'] .' '. t('Just enter 1 for TRUE, 0 for FALSE or make use of an input evaluator.');
    return array(
      '#type' => $info['long'] ? 'textarea' : 'textfield',
      '#title' => $info['label'],
      '#description' => $info['description'],
      '#required' => $info['required'],
      '#default_value' => $value,
    );
  }
}

/**
 * Implementation of hook_rules_condition_info().
 */
function rules_rules_condition_info() {
  return array(
    'rules_condition_text_compare' => array(
      'label' => t('Textual comparison'),
      'arguments' => array(
        'text1' => array('label' => t('Text 1'), 'type' => 'string'),
        'text2' => array('label' => t('Text 2'), 'type' => 'string'),
      ),
      'help' => t('TRUE is returned, if both texts are equal.'),
      'module' => 'Rules',
    ),
    'rules_condition_number_compare' => array(
      'label' => t('Numeric comparison'),
      'arguments' => array(
        'number1' => array('label' => t('Number 1'), 'type' => 'number'),
        'number2' => array('label' => t('Number 2'), 'type' => 'number'),
      ),
      'help' => t('Select greater than, less than or equal to.'),
      'module' => 'Rules',
    ),
    'rules_condition_check_boolean' => array(
      'label' => t('Check a truth value'),
      'arguments' => array(
        'boolean' => array('type' => 'boolean', 'label' => t('Truth value')),
      ),
      'module' => 'Rules',
    ),
  );
}

/**
 * Condition Implementation: Textual comparison.
 */
function rules_condition_text_compare($text1, $text2, $settings) {
  return $settings['regex'] ? ereg($text2, $text1) : $text1 == $text2;
}

/**
 * Condition implementation: Numeric comparison.
 */
function rules_condition_number_compare($number1, $number2, $settings) {
  switch ($settings['operation']) {
    case 'greater':
      return $number1 > $number2;

    case 'equal':
      return $number1 == $number2;

    case 'less':
      return $number1 < $number2;
  }
}

/**
 * Condition implementation: Boolean check.
 */
function rules_condition_check_boolean($boolean) {
  return (bool)$boolean;
}

/**
 * Implementation of hook_rules_action_info().
 */
function rules_rules_action_info() {
  $items = array();
  // Add actions for all rule sets.
  foreach (rules_get_configured_items('rule_sets') as $name => $set_info) {
    $items['rules_action_invoke_set_'. $name] = $set_info + array(
      'module' => 'Rule Sets',
      'base' => 'rules_action_invoke_set',
      'set' => $name,
    );
  }
  // Add drupal 6 core actions.
  $core_actions = actions_list();
  foreach ($core_actions as $name => $core_action_info) {
    $type_map = rules_gather_data('rules_action_type_map');
    $items['rules_core_'. $name] = rules_core_action_transform_info($name, $core_action_info, $type_map);
  }
  $items = array_filter($items);

  // Add an action to add a new variable for all data types that use an input form.
  foreach (rules_get_data_types() as $name => $type_info) {
    $type_info += array('use_input_form' => empty($type_info['identifiable']));
    if (!empty($type_info['use_input_form']) && empty($type_info['hidden'])) {
      $items['rules_add_var_'. $name] = array(
        'module' => 'Rules',
        'base' => 'rules_action_save_variable',
        'label' => t('Add a new @type variable', array('@type' => $type_info['label'])),
        'arguments' => array(
          'var_name' => array('type' => 'value', 'default value' => $name),
          $name => array('type' => $name, 'label' => $type_info['label']),
        ),
        'new variables' => array(
          $name => array(
            'type' => $name,
            'label' => t('Added @type', array('@type' => $type_info['label'])),
          ),
        ),
      );
    }
  }
  // Add an action to save savable data types
  foreach (rules_get_data_types() as $name => $type_info) {
    if (!empty($type_info['savable']) && empty($type_info['hidden']) && empty($type_info['use_input_form'])) {
      $items['rules_save_var_'. $name] = array(
        'module' => isset($type_info['module']) ? $type_info['module'] : 'Rules',
        'base' => 'rules_action_save_variable',
        'label' => t('Save a @type', array('@type' => $type_info['label'])),
        'arguments' => array(
          'var_name' => array('type' => 'value', 'default value' => $name),
          $name => array('type' => $name, 'label' => $type_info['label']),
        ),
      );
    }
  }
  return $items;
}

/**
 * Base action implementation for invoking all rule sets.
 *
 * We could just call rules_invoke_rule_set(), but then the changed variables
 * would be saved immediately. Instead we create a new evaluation state,
 * mapping to existing variables, so the outer state takes care of saving changes
 * for those variables.
 */
function rules_action_invoke_set() {
  $args     = func_get_args();
  $state    = array_pop($args);
  $element  = array_pop($args);
  $settings = array_pop($args);
  $set_name = $element['#info']['set'];
  $set      = rules_get_rule_set($set_name);

  if (!$set) {
    rules_log(t("The configured rule set %set doesn't exist any more.", array('%set' => $set_name)), TRUE);
    return;
  }

  // We create a new evaluation state, that maps the variables to the outer
  // state whenever possible. If necessary, we initialize new variables. So
  // we have to care for saving the changes of the new variables only.
  $new_state = array('set_info' => $set['info'], 'variables' => array());
  $mapped_vars = array();
  $map = rules_get_mapped_argument_names($element);

  $set['info'] += array('arguments' => array());
  foreach (array_keys($set['info']['arguments']) as $i => $name) {
    if (isset($map[$name]) && isset($state['variables'][$map[$name]])) {
      $new_state['variables'][$name] =& $state['variables'][$map[$name]];
      $mapped_vars[] = $name;
    }
    else {
      $variable = new rules_variable;
      $variable->construct($new_state, $name, $args[$i]);
    }
  }
  // Evaluate, but don't save mapped variables.
  rules_evaluate_rule_set($set_name, $set, $new_state, $mapped_vars);
}

/**
 * Base action implementation for adding a new variable and saving variables.
 */
function rules_action_save_variable($name, $variable, $settings, $element, &$state) {
  if (!empty($settings['immediate'])) {
    $map = rules_get_mapped_argument_names($element);
    // Force imediate saving!
    $state['variables'][$map[$name]]->save();
    $state['variables'][$map[$name]]->save_changes();
  }
  else {
    return array($name => $variable);
  }
}

/**
 * Transforms action info written for core to the rules format.
 * If the action should not be used by the rules modules, an empty array is returned.
 */
function rules_core_action_transform_info($name, $core_action_info, $type_map) {
  // If there is no entry in the type map or we should ignore it, we don't use this action.
  if (!empty($core_action_info['rules_ignore']) || !isset($type_map[$core_action_info['type']])) {
    return array();
  }

  $info = $type_map[$core_action_info['type']];

  // Make sure there is a object.
  $info += array('arguments' => array('object' => array('type' => 'value', 'default value' => NULL)));

  $info['label'] = $core_action_info['description'];

  // Special handling of labels for node actions
  // For consistency with the rule naming convention of calling a node "content".
  if ($core_action_info['type'] == 'node') {
    $info['label'] = str_replace(t('post'), t('content'), $info['label']);
  }

  $info['base'] = 'rules_core_action_execute';
  $info['action_name'] = $name;
  $info['configurable'] = $core_action_info['configurable'];
  return $info;
}

/**
 * Action implementation: Execute a core action
 */
function rules_core_action_execute(&$object, $settings, $element, &$state) {
  $info = rules_get_element_info($element);

  $info['action_name']($object, $settings);

  if (isset($settings['auto_save']) && $settings['auto_save']) {
    $argument_name = array_shift(array_keys($info['arguments']));
    return array($argument_name => $object);
  }
}

/**
 * Action form implementation: Get the core actions form
 */
function rules_core_action_execute_form($settings, &$form, &$form_state) {
  $element = $form_state['element'];
  $info = rules_get_element_info($element);
  if ($info['configurable']) {
    $function = $info['action_name'] .'_form';
    $form['settings'] = $function($settings);
  }
  if ($info['arguments']) {
    $arg = array_shift($info['arguments']);
    $data_type = rules_get_data_types($arg['type']);

    if (isset($data_type['savable']) && $data_type['savable']) {
      $settings += array('auto_save' => TRUE);
      $form['settings']['auto_save'] = array(
        '#type' => 'checkbox',
        '#title' => t('Permanently apply changes'),
        '#description' => t('If checked, changes to the argument are saved automatically.'),
        '#default_value' => $settings['auto_save'],
        '#weight' => -10,
      );
    }
  }
}

/**
 * Apply the core actions form validation
 */
function rules_core_action_execute_validate(&$form, &$form_state) {
  $element = $form_state['element'];
  $info = rules_get_element_info($element);
  if ($info['configurable'] && function_exists($function = $info['action_name'] .'_validate')) {
    $function($form['settings'], $form_state);
  }
}

/**
 * Action form submit implementation: Apply the core actions form submit
 */
function rules_core_action_execute_submit(&$settings, &$form, &$form_state) {
  $element = $form_state['element'];
  $info = rules_get_element_info($element);
  if ($info['configurable']) {
    $function = $info['action_name'] .'_submit';
    $settings = $function($form['settings'], $form_state) + $settings;
  }
}

/**
 * Implementation of hook_rules_action_type_map().
 *
 * Maps core action types to rules action info. This provides mappings for all
 * action types used in core.
 */
function rules_rules_action_type_map() {
  return array(
    'node' => array(
      'module' => 'Node',
      'arguments' => array(
        'node' => array(
          'label' => t('Content'),
          'type' => 'node',
        ),
      ),
    ),
    'comment' => array(
      'module' => 'Comment',
      'arguments' => array(
        'comment' => array(
          'label' => t('Comment'),
          'type' => 'comment',
        ),
      ),
    ),
    'user' => array(
      'module' => 'User',
      'arguments' => array(
        'user' => array(
          'label' => t('User'),
          'type' => 'user',
        ),
      ),
    ),
  );
}

/**
 * @}
 */
