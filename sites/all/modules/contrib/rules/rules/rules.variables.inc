<?php
// $Id: rules.variables.inc,v 1.1.2.33 2009/07/20 16:22:24 fago Exp $


/**
 * @file Provides functions and classes for handling variables
 */

/**
 * Processes the given variables and initializes the variables data of the state
 *
 * Note that the order of the arguments will be kept, as it might be important for passing
 * the variables to further variable loading handlers.
 *
 * @param $state The current evaluation state
 * @param $args The arguments passed with the event invocation
 */
function _rules_initialize_variables(&$state, $args) {
  $argument_names = array_keys($state['set_info']['arguments']);

  if (isset($args[0]) && count($args) == 1 && is_array($args[0]) && count($args[0]) > 0 && array_diff(array_keys($args[0]), $argument_names) === array()) {
    //the caller passed an array of arguments, so let's use that
    $data = $args[0];
  }
  else {
    $data = array();
    foreach ($argument_names as $index => $name) {
      $data[$name] = &$args[$index];
    }
  }
  $state['variables'] = array();
  foreach ($state['set_info']['arguments'] as $name => $info) {
    $variable = new rules_variable;
    $data += array($name => NULL);
    $variable->construct($state, $name, $data[$name]);
  }
}

/**
 * Gets an array of variables specified in $names from the current evalutation state.
 *
 * @param $names
 *   An array of variable names to get.
 * @param $state
 *   The current evalutation state.
 * @param $force
 *   Use specified variable handlers to load variables, if necessary.
 *
 * @return
 *   The array of requested variables with the keys as given in $names. If it's not possible
 *   to get all variables, FALSE will be returned.
 */
function rules_get_variables($names, &$state, $force = TRUE) {
  $args = array();
  foreach ($names as $key => $name) {
    if (isset($state['variables'][$name])) {
      $args[$key] = &$state['variables'][$name]->get($force);
    }
    else {
      rules_log(t('Warning: Unable to get variable "@name".', array('@name' => $key)));
      return FALSE;
    }
  }
  return $args;
}

/**
 * Gets the arguments as defined by the element.
 *
 * Returns the arguments, which may have been specified using the data type's input
 * form or by mapping to a state variable. Else a possible defined default value is
 * returned.
 *
 * @param $element
 *   The element to get arguments for.
 * @param $state
 *   The current evalutation state.
 *
 * @return
 *   An array of argument values. If it's not possible to get all arguments, FALSE
 *   will be returned.
 */
function rules_get_element_arguments($element, &$state) {
  $element_info = rules_get_element_info($element);
  $map = rules_get_mapped_argument_names($element);

  $args = array();
  foreach (array_keys($element_info['arguments']) as $key => $name) {
    if (($var = rules_get_element_variable($element, $name)) !== NULL) {
      $args[$key] = $var;
    }
    elseif (isset($map[$name]) && isset($state['variables'][$map[$name]])) {
      $args[$key] = &$state['variables'][$map[$name]]->get();
    }
    elseif (array_key_exists('default value', $element_info['arguments'][$name])) {
      $args[$key] = $element_info['arguments'][$name]['default value'];
    }
    else {
      rules_log(t('Warning: Unable to get argument "@name".', array('@name' => $key)));
      return FALSE;
    }
  }
  return $args;
}

/**
 * Gets a variable for the given element which uses an own input form.
 *
 * @param $element
 *   The element to get the variable for.
 * @param $name
 *   The name of the variable to get.
 *
 * @return The variable, if available.
 */
function rules_get_element_variable($element, $name) {
  if (isset($element['#settings'][$name]) && isset($element['#info']['arguments'][$name])) {
    $object = rules_get_data_object($element['#info']['arguments'][$name]);
    return $object->check_value($element['#info']['arguments'][$name], $element['#settings'][$name]);
  }
}

/**
 * Gets the argument map, mapping the specified arguments of an
 * element to the variable names of the currently evaluated set.
 *
 * @param $element The configured element, which variables to map
 * @param $state The current evaluation state
 *
 * @return The argument map.
 */
function rules_get_mapped_argument_names($element) {
  $element['#settings'] += array('#argument map' => array());
  $map = $element['#settings']['#argument map'];

  if ($element_info = rules_get_element_info($element)) {
    // Initialize the map with default mappings (name => name).
    return $map + drupal_map_assoc(array_keys($element_info['arguments']));
  }
  return $map;
}

/**
 * Returns the execution arguments needed by the given element
 * It applies the input evaluators and the #argument map and gets
 * all needed arguments.
 *
 * @param $element The configured element, which is to be executed
 * @param $state The current evaluation state
 *
 * @return If not all execution arguments are available, it returns FALSE
 */
function rules_get_execution_arguments(&$element, &$state) {
  //apply the input evaluators
  $success = rules_apply_input_evaluators($element, $state);

  if ($success === FALSE) {
    rules_log(t('Element "@name" has not been executed. There are not all execution arguments needed by an input evaluator available.', array('@name' => rules_get_element_label($element))));
    return FALSE;
  }

  // First off get the arguments of the element as specified.
  $exec_args = rules_get_element_arguments($element, $state);

  if ($exec_args !== FALSE) {
    // Then we always append some other useful variables
    $settings    = isset($element['#settings']) ? $element['#settings'] : array();
    $exec_args[] = $settings;
    $exec_args[] = $element;
    $exec_args[] = &$state;
    return $exec_args;
  }
  rules_log(t('Element "@name" has not been executed. There are not all execution arguments available.', array('@name' => rules_get_element_label($element))));
  return FALSE;
}

/**
 * Saves variables, which are returned by an action
 */
function rules_save_variables($element, $result, &$state) {
  $element_info = rules_get_element_info($element);
  $element['#settings'] += array('#argument map' => array());
  $map = $element['#settings']['#argument map'];

  foreach (element_children($result) as $argument_name) {
    $map += array($argument_name => $argument_name);

    if (isset($state['variables'][ $map[$argument_name] ])) {
      //variable exists, so update it
      $variable = &$state['variables'][ $map[$argument_name] ];
      $variable->update( $result[$argument_name] );
      $variable->save();
    }
    else if (isset($element_info['new variables'][ $map[$argument_name] ])) {
      //it is a new variable, so add it
      $info = $element_info['new variables'][ $map[$argument_name] ];

      $variable = new rules_variable;
      $variable->construct($state, $map[$argument_name], $result[$argument_name], $info);
      if (isset($info['save']) && $info['save']) {
        $variable->save();
      }
      rules_log(t('Successfully added the new variable "@arg"', array('@arg' => $info['label'])));
    }
    else {
      rules_log(t('Unknown variable name "@var" return by action "@name".', array('@var' => $argument_name, '@name' => $element['#name'])), TRUE);
    }
  }
}

/**
 * Packs the given variables ready for serialization. Data types which
 * are identifiable are replaced by their identifiers and loaded fresh from
 * the db when the variables are unpacked.
 *
 * @param $variables
 *   An array of variable information
 * @param $data
 *   An array of data for the given variables, in the same order as the information.
 *
 * @return
 *   The packed variables or FALSE if packing failed.
 */
function rules_pack_variables($variables, $data) {
  $packed = array('variables' => $variables, 'data' => array());
  foreach (array_values($variables) as $i => $info) {
    $type = rules_get_data_object($info, $data[$i]);
    if (isset($type) && $type->is_identifiable()) {
      $id = $type->get_identifier();
      if (!isset($id)) {
        return FALSE;
      }
      $packed['data'][$i] = $id;
    }
    else {
      $packed['data'][$i] = $data[$i];
    }
  }
  return $packed;
}

/**
 * Unacks the given packed variables.
 *
 * @param $packed
 *   Packed variables, as returned from rules_pack_variables().
 *
 * @return
 *   An array, containing the key 'variables' with info about the packed
 *   variables and the key 'data' with the actual data.
 *   If unpacking failed, FALSE.
 */
function rules_unpack_variables($packed) {
  foreach (array_values($packed['variables']) as $i => $info) {
    $type = rules_get_data_object($info);
    if (isset($type) && $type->is_identifiable()) {
      $packed['data'][$i] = $type->load($packed['data'][$i]);
      if ($packed['data'][$i] === FALSE) {
        return FALSE;
      }
    }
  }
  return $packed;
}

/**
 * Handles loading and saving a variable
 */
class rules_variable {

  var $name;
  var $info;
  var $data;
  var $_state;
  var $_changed = FALSE;

  /**
   * Constructor
   *
   * @param $state The current evaluation state
   * @param $name The name of the given variable
   * @param $data If available, the actual data, else NULL.
   * @param $info If given, the info for the variable. If not given it will be retrieved
   *   from the current's set info.
   *
   * @return If the variable name isn't valid, FALSE.
   */
  function construct(&$state, $name, &$data, $info = NULL) {
    $this->_state = &$state;
    $info = isset($info) ? $info : $state['set_info']['arguments'][$name];

    if (isset($info)) {
      $this->info = $info + array('saved' => FALSE, 'handler' => '');
      $this->name = $name;
      $this->_set_data($data);
      $state['variables'][$name] = &$this;
      return TRUE;
    }
    return FALSE;
  }

  /**
   * Gets the actual data. Be sure to keep the reference intact.
   *
   * @param $load Use the variable handler to load the variable, if necessary.
   *
   * @return The data or NULL.
   */
  function &get($load = TRUE) {
    $data = NULL;

    if ($load && !isset($this->data) && function_exists($this->info['handler'])) {
      // Call the handler to get the runtime data.
      $args = rules_get_variables(array_keys($this->_state['variables']), $this->_state, FALSE);
      $data = call_user_func_array($this->info['handler'], $args);
      $this->_set_data($data);

      $this->info['handler'] = ''; // Do not invoke it twice, if it fails
      rules_log(t('Loaded variable "@arg"', array('@arg' => $this->info['label'])));
    }
    else if (isset($this->data)) {
      $data = &$this->data->get();
    }
    return $data;
  }

  function _set_data(&$data) {
    if (isset($data)) {
      $this->data = rules_get_data_object($this->info);
      // Set the data in the data object and make sure to keep a reference intact
      $this->data->init($data);
    }
  }

  /**
   * Marks the variable to be saved.
   */
  function save() {
    $this->_changed = TRUE;
  }

  /**
   * Updates the actual variable
   */
  function update(&$data) {
    $this->data->update($data);
  }

  /**
   * Saves the variable to db, if necessary
   */
  function save_changes() {
    //if the variable is not saved automatically, save it
    if ($this->_changed && !$this->info['saved'] && $this->data->is_savable()) {
      rules_log(t('Saved variable @name of type @type.', array('@name' => $this->info['label'], '@type' => $this->info['type'])));

      $return = $this->data->save();
      $this->_changed = FALSE;

      if (!$return) {
        rules_log(t('Failed saving variable @name of type @type.', array('@name' => $this->info['label'], '@type' => $this->info['type'])), TRUE);
      }
    }
  }
}

