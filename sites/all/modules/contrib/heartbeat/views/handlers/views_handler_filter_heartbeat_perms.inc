<?php
// $Id: views_handler_filter_heartbeat_perms.inc,v 1.1.2.1.2.3 2010/02/21 22:45:26 stalski Exp $

/**
 * @file
 *   Implementation of equality filter to match the permission
 *   configuration with the message permission.
 */

/**
 * Class views_handler_filter_heartbeat_perms
 */
class views_handler_filter_heartbeat_perms extends views_handler_filter_equality {

  function construct() {
    parent::construct();
    $this->definition['numeric'] = TRUE;
  }

  /**
   * Display the filter on the administrative summary.
   */
  function admin_summary() {
    $options = _heartbeat_perms_options();
    return (!empty($this->value) ? $options[$this->value] : $options[1]);
  }

  /**
   * Child classes should override this function to set the 'value options'.
   * This can use a guard to be used to reduce database hits as much as
   * possible.
   */
  function value_form(&$form, &$form_state) {
    $form['value'] = array(
      '#type' => 'radios',
      '#options' => _heartbeat_perms_options(),
      '#title' => t('Restrict activity stream to message permissions'),
      '#default_value' => $this->value,
    );
  }

  function query() {
    $this->ensure_my_table();
    $sql = "$this->table_alias.$this->real_field = '%s' ";
    $this->query->add_where($this->options['group'], $sql, $this->value);
  }
}