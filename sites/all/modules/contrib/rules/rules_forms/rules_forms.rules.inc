<?php
// $Id: rules_forms.rules.inc,v 1.1.2.9 2010/03/11 12:12:19 klausi Exp $


/**
 * @file
 * Rules integration to provide form support
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_rules_event_info().
 */
function rules_forms_rules_event_info() {
  $form_events = variable_get('rules_forms_events', array());
  $events = array();
  foreach ($form_events as $form_id => $label) {
    $events[$form_id .'_form_built'] = array(
      'label' => t('@form is being built', array('@form' => $label)),
      'module' => 'Rules Forms',
      'arguments' => rules_forms_events_arguments(),
    );
    $events[$form_id .'_form_submit'] = array(
      'label' => t('@form is submitted', array('@form' => $label)),
      'module' => 'Rules Forms',
      'arguments' => rules_forms_events_arguments(),
    );
    $events[$form_id .'_form_validate'] = array(
      'label' => t('@form is being validated', array('@form' => $label)),
      'module' => 'Rules Forms',
      'arguments' => rules_forms_events_arguments(),
    );
  }
  return $events;
}

/**
 * Returns some arguments suitable for hook form alter.
 */
function rules_forms_events_arguments() {
  return array(
    'form' => array('type' => 'form', 'label' => t('Form')),
    'form_state' => array('type' => 'form_state', 'label' => t('Form state')),
    'form_id' => array('type' => 'string', 'label' => t('Form ID')),
  ) + rules_events_global_user_argument();
}

/**
 * Implementation of hook_rules_action_info().
 */
function rules_forms_rules_action_info() {
  return array(
    'rules_forms_action_redirect' => array(
      'label' => t('Set the redirect target of the form'),
      'arguments' => array(
        'form' => array('type' => 'form', 'label' => t('Form')),
        'path' => array('type' => 'string', 'label' => t('Path')),
        'query' => array('type' => 'string', 'label' => t('Query')),
        'fragment' => array('type' => 'string', 'label' => t('Fragment')),
      ),
      'module' => 'Rules Forms',
      'help' => t('Enter a Drupal path, path alias, or external URL to redirect to. Enter (optional) queries after "?" and (optional) anchor after "#".'),
    ),
    'rules_forms_action_hide_element' => array(
      'label' => t('Hide an element of the form'),
      'arguments' => array(
        'form' => array('type' => 'form', 'label' => t('Form')),
        'hide' => array(
          'type' => 'string',
          'label' => t('Form element ID'),
          'description' => t('The element that should not appear.') .' '. _rules_forms_element_description(),
        ),
      ),
      'module' => 'Rules Forms',
    ),
    'rules_forms_action_adjust_weight' => array(
      'label' => t('Adjust weight of an element in the form'),
      'arguments' => array(
        'form' => array('type' => 'form', 'label' => t('Form')),
        'element' => array(
          'type' => 'string',
          'label' => t('Form element ID'),
          'description' => t('The element that should be adjusted.') .' '. _rules_forms_element_description(),
        ),
        'weight' => array(
          'type' => 'number',
          'label' => t('Element weight'),
          'description' => t('Low numbers make the element bubble up, high numbers sink it down.'),
        ),
      ),
      'module' => 'Rules Forms',
    ),
    'rules_forms_action_prefix_suffix_html' => array(
      'label' => t('Insert HTML prefix/suffix code'),
      'arguments' => array(
        'form' => array('type' => 'form', 'label' => t('Form')),
        'element' => array(
          'type' => 'string',
          'label' => t('Form element ID'),
          'description' => t('ID of the form element to be targeted.') .' '. _rules_forms_element_description(),
        ),
        'prefix' => array(
          'type' => 'string',
          'label' => t('Prefixed HTML'),
          'description' => t('HTML inserted before.'),
        ),
        'suffix' => array(
          'type' => 'string',
          'label' => t('Suffixed HTML'),
          'description' => t('HTML inserted after.'),
        ),
      ),
      'module' => 'Rules Forms',
    ),
    'rules_forms_action_set_error' => array(
      'label' => t('Set a form error'),
      'arguments' => array(
        'form' => array('type' => 'form', 'label' => t('Form')),
        'element' => array(
          'type' => 'string',
          'label' => t('Form element ID'),
          'description' => t('The element that should be marked.') .' '. _rules_forms_element_description(),
        ),
        'message' => array(
          'type' => 'string',
          'label' => t('Message'),
          'description' => t('The message that should be displayed to the user.'),
        ),
      ),
      'module' => 'Rules Forms',
    ),
    'rules_forms_action_set_element' => array(
      'label' => t('Set the default value of a form element'),
      'arguments' => array(
        'form' => array('type' => 'form', 'label' => t('Form')),
        'element' => array(
          'type' => 'string',
          'label' => t('Form element ID'),
          'description' => t('The element that should be targeted.') .' '. _rules_forms_element_description(),
        ),
        'value' => array(
          'type' => 'string',
          'label' => t('Default value'),
          'description' => t('The value(s) that will be displayed as default. If the form element allows multiple values, enter one value per line.'),
        ),
      ),
      'module' => 'Rules Forms',
    ),
  );
}

function _rules_forms_element_description() {
  return t('Examples on the "Create Story" form: "title" for the title field or "body_field[body]" for the body field.');
}

/**
 * Implementation of hook_rules_data_type_info().
 */
function rules_forms_rules_data_type_info() {
  return array(
    'form' => array(
      'label' => t('form'),
      'class' => 'rules_data_type',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'use_input_form' => FALSE,
      'module' => 'Rules Forms',
    ),
    'form_state' => array(
      'label' => t('form state'),
      'class' => 'rules_data_type',
      'savable' => FALSE,
      'identifiable' => FALSE,
      'use_input_form' => FALSE,
      'module' => 'Rules Forms',
    ),
  );
}

/**
 * Action implementation: Set the redirect target.
 */
function rules_forms_action_redirect(&$form, $path, $query, $fragment) {
  $form['#redirect'] = array($path, $query, $fragment);
}

/**
 * Action implementation: Hide a form element.
 */
function rules_forms_action_hide_element(&$form, $hide) {
  $form_element = &_rules_forms_get_element($form, $hide);
  $form_element['#access'] = FALSE;
}

/**
 * Action implementation: Adjust weight of a form element.
 */
function rules_forms_action_adjust_weight(&$form, $element, $weight) {
  // CCK overrides weight settings in #content_extra_fields
  if (isset($form['#content_extra_fields'][$element]['weight'])) {
    $form['#content_extra_fields'][$element]['weight'] = $weight;
  }
  else {
    $form_element = &_rules_forms_get_element($form, $element);
    $form_element['#weight'] = $weight;
  }
}

/**
 * Action implementation: Insert prefix/suffix HTML code.
 */
function rules_forms_action_prefix_suffix_html(&$form, $element, $prefix, $suffix) {
  if (empty($element)) {
    if (!empty($prefix)) {
      $form['#prefix'] = $prefix;
    }
    if (!empty($suffix)) {
      $form['#suffix'] = $suffix;
    }
  }
  else {
    $form_element = &_rules_forms_get_element($form, $element);
    if (!empty($prefix)) {
      $form_element['#prefix'] = $prefix;
    }
    if (!empty($suffix)) {
      $form_element['#suffix'] = $suffix;
    }
  }
}

/**
 * Action implementation: set form error.
 */
function rules_forms_action_set_error(&$form, $element, $message) {
  if (substr($element, strlen($element)-1) === ']') {
    $form_element = _rules_forms_get_element($form, $element);
    if (isset($form_element['#parents'])) {
      $element = implode('][', $form_element['#parents']);
    }
    else {
      // remove trailing ']'
      $element = substr($element, 0, strlen($element)-1);
      // insert ']' before the first '['
      $first_part = substr($element, 0, strpos($element, '['));
      $second_part = substr($element, strpos($element, '['));
      $element = $first_part .']'. $second_part;
    }
  }
  form_set_error($element, $message);
}

/**
 * Action implementation: Set the default value.
 */
function rules_forms_action_set_element(&$form, $element, $value) {
  $form_element = &_rules_forms_get_element($form, $element);
  if (empty($form_element) || !array_key_exists('#default_value', $form_element)) {
    return;
  }
  $lines = explode("\r\n", $value);
  // Check if the form element is a CCK field
  if (array_key_exists('#field_name', $form_element)) {
    $default_value_key = $form_element['#columns'][0];
    // Check if multiple default values are allowed
    if (isset($form_element['#default_value'][0])) {
      // Unset old default values
      $form_element['#default_value'] = array();
      foreach ($lines as $i => $line) {
        $form_element['#default_value'][$i][$default_value_key] = $line;
      }
    }
    else {
      $form_element['#default_value'][$default_value_key] = $value;
    }
  }
  else {
    if (count($lines) == 1) {
      $form_element['#default_value'] = $value;
    }
    else {
      $form_element['#default_value'] = $lines;
    }
  }
}

/**
 * Helper function to extract a reference to a form element by a given name.
 */
function &_rules_forms_get_element(&$form, $name) {
  $names = explode('[', $name);
  $element = &$form;
  foreach ($names as $name) {
    if (substr($name, strlen($name)-1) === ']') {
      // remove trailing ']'
      $name = substr($name, 0, strlen($name)-1);
    }
    $element = &$element[$name];
  }
  return $element;
}

/**
 * Implementation of hook_rules_condition_info().
 */
function rules_forms_rules_condition_info() {
  return array(
    'rules_forms_condition_element_value' => array(
      'label' => t('Form element has value'),
      'arguments' => array(
        'form' => array('type' => 'form', 'label' => t('Form')),
        'form_state' => array('type' => 'form_state', 'label' => t('Form state')),
        'element' => array(
          'type' => 'string',
          'label' => t('Form element ID'),
          'description' => t('ID of the form element to be targeted.') .' '. _rules_forms_element_description(),
        ),
        'value' => array(
          'type' => 'string',
          'label' => t('Value(s)'),
          'description' => t('Value(s) assigned to the form element. If the form element allows multiple values, enter one value per line.'),
        ),
      ),
      'module' => 'Rules Forms',
    ),
  );
}

/**
 * Condition implementation: Check a form element value.
 */
function rules_forms_condition_element_value($form, $form_state, $element, $value) {
  $form_element = _rules_forms_get_element($form, $element);
  if (isset($form_element['#value'])) {
    // Multiple values come in as array
    if (is_array($form_element['#value'])) {
      $lines = explode("\r\n", $value);
      return rules_forms_equal_array_values($lines, $form_element['#value']);
    }
    return $form_element['#value'] === $value;
  }
  if (isset($form_element['#default_value'])) {
    if (is_array($form_element['#default_value'])) {
      $lines = explode("\r\n", $value);
      return rules_forms_equal_array_values($lines, $form_element['#default_value']);
    }
    return $form_element['#default_value'] === $value;
  }
  return FALSE;
}

/**
 * Returns TRUE if both arrays contain the same values, regardless of their keys
 * and value ordering.
 */
function rules_forms_equal_array_values($array1, $array2) {
  $diff1 = array_diff($array1, $array2);
  $diff2 = array_diff($array2, $array1);
  return empty($diff1) && empty($diff2);
}

/**
 * @}
 */
