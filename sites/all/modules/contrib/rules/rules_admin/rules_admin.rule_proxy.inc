<?php
// $Id: rules_admin.rule_proxy.inc,v 1.1.2.2 2009/04/19 15:03:43 fago Exp $


/**
 * @file Contains the rules proxy class
 */

/**
 * This is a smally proxy for the real rule. It provides some useful operations for the admin UI.
 * It builds a small index for the elements of a rule, so that they can be easily identified and modified.
 */
class rules_admin_rule_proxy {

  var $_rule;
  var $_rule_name;
  var $_counter;      //used when generating ids
  var $_variables;    //variables defined by previous rules
  var $map = array(); //stores the id => element map

  /**
   * Constructor
   *
   * @param $rule_name The name of the rule
   * @param $rule The rule to create a proxy for.
   */
  function rules_admin_rule_proxy($rule_name, &$rule) {
    $rule += array('#conditions' => array(), '#actions' => array());

    //save a copy of the original rule for later
    $this->_rule = $rule;
    $this->_rule_name = $rule_name;

    //and the reference
    $this->map[0] = &$rule;

    $this->_counter = 0;
    $this->_generate_index($rule['#conditions']);
    $this->_generate_index($rule['#actions']);
  }

  /**
   * Gets the referenced rule
   */
  function &get_rule() {
    return $this->get_element(0);
  }

  /**
   * Gets the rule's name
   */
  function get_rule_name() {
    return $this->_rule_name;
  }

  /**
   * Gets an element of the referenced rule by id
   */
  function &get_element($id) {
    if (!isset($this->map[$id])) {
      $this->map[$id] = FALSE;
    }
    return $this->map[$id];
  }

  /**
   * Gets the id of the parent element
   */
  function get_element_parent_id($id) {
    $element = $this->get_element($id);
    while ($id > 0) {
      $id--;
      //get the element and look if it's the parent
      $parent = $this->get_element($id);
      foreach (element_children($parent) as $key) {
        if ($parent[$key] == $element) {
          //parent found!
          return $id;
        }
      }
    }
    return FALSE;
  }

  /**
   * Saves the changes in the rule and clears the cache if necessary.
   */
  function save_changes() {
    $rule = $this->get_rule();
    $orig_rule = $this->_rule;
    if ($orig_rule != $rule) {
      $this->save_rule();
      rules_clear_cache();
    }
  }

  /**
   * Creates an id for each element and stores a reference on it
   */
  function _generate_index(&$elements) {
    //generate ids
    $this->_counter++;
    $this->map[$this->_counter] = &$elements;

    //recurse
    foreach (element_children($elements) as $key) {
      $this->_generate_index($elements[$key]);
    }
  }

  /**
   * Gets the rule with set #id properties, useful for rendering.
   * Note: Any possible changes done, won't appear in the returned rule.
   */
  function get_indexed_rule() {
    $this->_counter = 0;
    $index_rule = $this->_rule;
    $this->_generate_index_rule($index_rule['#conditions']);
    $this->_generate_index_rule($index_rule['#actions']);
    return $index_rule;
  }

  function _generate_index_rule(&$elements) {
    //generate ids
    $this->_counter++;
    $elements['#id'] = $this->_counter;

    //recurse
    foreach (element_children($elements) as $key) {
      $this->_generate_index_rule($elements[$key]);
    }
  }

  /**
   * Saves a the rule in the database
   */
  function save_rule() {
    $rule = $this->get_rule();

    if (!isset($rule['#status']) || $rule['#status'] == 'default') {
      $rule['#status'] = 'altered';
    }

    // Sort conditions and actions.
    rules_sort_elements($rule['#conditions']);
    rules_sort_elements($rule['#actions']);

    rules_item_save('rules', $this->get_rule_name(), $rule);
  }

  /**
   * Deletes the rule configuration from the database
   */
  function delete_rule() {
    $rule = $this->get_rule();
    rules_item_delete('rules', $this->get_rule_name(), $rule);
  }

  /**
   * Cleans the given rule. This means, array keys that are neither elements
   * nor properties are removed.
   */
  function clean_rule() {
    $rule = &$this->get_rule();
    $this->_clean_rule($rule['#conditions']);
    $this->_clean_rule($rule['#actions']);
  }

  function _clean_rule(&$element) {
    $children = array();
    foreach (element_children($element) as $key) {
      if (!isset($element[$key]['#type'])) {
        //this element can be removed, but care for it's children
        foreach (element_children($element[$key]) as $child_key) {
          $children[$child_key] = $element[$key][$child_key];
        }
        unset($element[$key]);
      }
      else {
        $this->_clean_rule($element[$key]);
      }
    }
    if (count($children)) {
      $element = array_merge($element, $children);
    }
  }

  /**
   * Gets the set of this rule, which contains only active rules.
   */
  function get_set() {
    $rule = $this->get_rule();
    $set = rules_get_rule_set($rule['#set']);
    return $set ? $set : array('info' => array(), 'rules' => array());
  }

  /**
   * Gets the info about the set of this rule
   */
  function get_set_info() {
    $rule = $this->get_rule();
    return rules_get_rule_sets($rule['#set']) + array('arguments' => array());
  }

  /**
   * Gets the info about all new defined variables in this rule,
   * before the element with the given id
   */
  function get_new_variables($id = NULL) {
    $vars = array();
    $i = 0;
    $element = TRUE;
    while (isset($id) && $i < $id || !isset($id) && $element !== FALSE) {
      $element = $this->get_element($i);
      $vars = $this->element_get_new_variables($element) + $vars;
      $i++;
    }
    return $vars;
  }

  /**
   * Gets info about all new defined variables by the given element
   */
  function element_get_new_variables($element) {
    if (isset($element['#type']) && $element['#type'] == 'action') {
      $info = rules_get_element_info($element);
      return $info['new variables'];
    }
    return array();
  }

  /**
   * Gets info about all available variables
   * This are the arguments of the rule's set as well as further arguments that
   * might be provided by actions of this or preceding rules. Variables set to be hidden
   * are left out.
   *
   * @param $id The id of the element of the current rule until which we should get new variables.
   *   If not given all new variables of this rule will be considered.
   */
  function get_available_variables($id = NULL) {
    $set_info = $this->get_set_info();
    $vars = $this->get_new_variables($id) + $this->_get_available_variables() + $set_info['arguments'];
    return array_filter($vars, 'rules_admin_element_filter');
  }

  /**
   * Gets info about all defined variables
   * This are all available variables as well as variables defined in and after this rule.
   */
  function get_defined_variables() {
    $set_info = $this->get_set_info();
    $vars = $this->_get_available_variables(TRUE) + $set_info['arguments'];
    return array_filter($vars, 'rules_admin_element_filter');
  }

  /**
   * Gets new variables defined by actions in rules, which are evaluated until this rule.
   *
   * @param $all
   *   If set to TRUE all variables defined by any rules in this rule set will returned.
   */
  function _get_available_variables($all = FALSE) {
    if (!isset($this->_variables[$all])) {
      $this->_variables[$all] = array();
      $set = $this->get_set();

      //sort the rules
      rules_sort_children($set['rules']);

      foreach (element_children($set['rules']) as $name) {
        if ($name == $this->get_rule_name()) {
          if (!$all) {
            return $this->_variables[$all];
          }
        }
        $set['rules'][$name] += array('#actions' => array());
        foreach ($set['rules'][$name]['#actions'] as $action) {
          $this->_variables[$all] += $this->element_get_new_variables($action);
        }
      }
    }
    return $this->_variables[$all];
  }
}

