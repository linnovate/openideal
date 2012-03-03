<?php
/**
 * Render a field as a numeric value
 *
 * Definition terms:
 * - float: If true this field contains a decimal value. If unset this field
 *          will be assumed to be integer.
 *
 * @ingroup views_field_handlers
 */
class views_handler_field_first_stage_comments extends views_handler_field_numeric {

  function render($values) {
    $value = db_query("SELECT COUNT(*)
        FROM {comments} c
        WHERE c.nid = :c.nid
        AND c.pid < :c.pid", array(':c.nid' => $values->nid, ':c.pid' => 1))->fetchField();
    return check_plain($this->options['prefix'] . $value . $this->options['suffix']);
  }
}
