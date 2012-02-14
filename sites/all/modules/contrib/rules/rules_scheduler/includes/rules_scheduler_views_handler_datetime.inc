<?php
// $Id: rules_scheduler_views_handler_datetime.inc,v 1.1.2.1 2009/07/13 13:34:27 fago Exp $

/**
 * @file
 * An extended subclass for datetime handling.
 * We need this to correctly convert our SQL datetime field to local user date/time.
 */
class rules_scheduler_views_handler_datetime extends views_handler_field_date {

  function render($values) {
    $value = $values->{$this->field_alias};
    // overwrite date string with timestamp
    $values->{$this->field_alias} = rules_gmstrtotime($value);
    // call parent method with modified values
    return parent::render($values);
  }
}
