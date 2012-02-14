<?php
// $Id: views_handler_filter_heartbeat_language.inc,v 1.1.2.2.2.3 2010/02/21 22:45:26 stalski Exp $

/**
 * @file
 *   Boolean filter implementation for heartbeat languages.
 */

/**
 * Class views_handler_filter_heartbeat_access
 *   Filter by language
 */
class views_handler_filter_heartbeat_language extends views_handler_filter_boolean_operator {

  function construct() {
    parent::construct();
    $this->value_value = t('Is the current language');
  }

  function query() {
    $this->ensure_my_table();
    //watchdog('useractivity',"$this->table_alias.$this->real_field " . (empty($this->value) ? '!=' : '=') . " ***CURRENT_LANGUAGE***");

    $this->query->add_where($this->options['group'], "$this->table_alias.$this->real_field " . (empty($this->value) ? '!=' : '=') . " '***CURRENT_LANGUAGE***'");
  }

}