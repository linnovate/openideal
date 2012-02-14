<?php
// $Id: views_handler_filter_heartbeat_access.inc,v 1.1.2.2.2.6 2010/02/21 22:45:26 stalski Exp $

/**
 * @file
 *   Numeric filter implementation for heartbeat access states.
 *   Per stream we can set the view to query differently.
 */

/**
 * Class views_handler_filter_heartbeat_access
 *   Filter by access type or stream type
 */
class views_handler_filter_heartbeat_access extends views_handler_filter_numeric {

  public $stream = NULL;

  function construct() {
    parent::construct();
    $this->value_value = t('Are restricted to access type');
    $this->value['access'] = 0;
  }

  /**
   * Implementation override option_definition
   * Information about options for all kinds of purposes will be held here.
   *
   * @return array options definition
   */
  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  /**
   * Set default values for the heartbeat access filter.
   */
  function options(&$options) {
    parent::options($options);
    $options['value']['access'] = array();
  }

  /**
   * Definition of available operators
   * Override the parent method
   *
   * @return array of operators
   */
  function operators() {
    $operators = array(
      '=' => array(
        'title' => t('Is equal to'),
        'method' => 'op_simple',
        'short' => t('='),
        'values' => 1,
      ),
      '!=' => array(
        'title' => t('Is not equal to'),
        'method' => 'op_simple',
        'short' => t('!='),
        'values' => 1,
      )
    );
    // if the definition allows for the empty operator, add it.
    if (!empty($this->definition['allow empty'])) {
      $operators += array(
        'empty' => array(
          'title' => t('Is empty (NULL)'),
          'method' => 'op_empty',
          'short' => t('empty'),
          'values' => 0,
        ),
        'not empty' => array(
          'title' => t('Is not empty (NULL)'),
          'method' => 'op_empty',
          'short' => t('not empty'),
          'values' => 0,
        ),
      );
    }
    return $operators;
  }

  /**
   * Display the filter on the administrative summary.
   * After selecting, let the builder of the view
   * know what he selected
   *
   * @return human title for the access type
   */
  function admin_summary() {
    $options = _heartbeat_access_type_options();
    $key =  (!empty($this->value['access']) ? $this->value['access'] : 'publicheartbeat');
    $return = $options[$key];

    return $return;
  }

  /**
   * Adding things to query object
   *
   */
  function query() {

    $this->ensure_my_table();
    $info = $this->operators();

    $type = $this->value['access'];
    $stream = heartbeat_stream_load($type);

    $class = $type;
    heartbeat_include('HeartbeatStream');
    heartbeat_include($class, $stream['module']);
    $heartbeataccess = new $class(new HeartbeatStream($stream));

    $this->stream = $heartbeataccess->getStream();

    // Get extra sql part for this access state
    $heartbeataccess->addViewQuery($this);
  }

  /**
   * Add a type selector to the value form
   */
  function value_form(&$form, &$form_state) {
    if (empty($form_state['exposed'])) {
      $form['value']['access'] = array(
        '#type' => 'radios',
        '#title' => t('Access type'),
        '#options' => _heartbeat_access_type_options(),
        '#default_value' => !empty($this->value['access']) ? $this->value['access'] : 'PublicHeartbeat',
      );
    }
    parent::value_form($form, $form_state);
  }

}