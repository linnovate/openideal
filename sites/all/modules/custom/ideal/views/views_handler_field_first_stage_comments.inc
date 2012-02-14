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
    $value = db_result(
      db_query(
        "SELECT COUNT(*)
        FROM {comments} c
        WHERE c.nid = %d
        AND c.pid < 1",
      $values->nid
      )
    );
    return check_plain($this->options['prefix'] . $value . $this->options['suffix']);
  }
}
