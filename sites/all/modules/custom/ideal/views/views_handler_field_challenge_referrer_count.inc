<?php
/**
* Handler.
 */
class views_handler_field_challenge_referrer_count extends views_handler_field_numeric {

  function query() {
    $this->add_additional_fields();
  }
  
  function render($values) {
    $nid = $values->nid;
    $db_info = content_database_info(content_fields('field_challenge', 'idea'));
    $table_name = $db_info['table'];
	  $field_name = $db_info['columns']['nid']['column'];

	  $count = db_result(db_query("SELECT COUNT(*) FROM $table_name fcn WHERE fcn.$field_name = %d", $nid));
	  return $count;
  }
}

