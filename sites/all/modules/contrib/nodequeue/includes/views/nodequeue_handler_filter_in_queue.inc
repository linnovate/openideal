<?php
//$Id: nodequeue_handler_filter_in_queue.inc,v 1.1 2008/09/06 15:42:05 ezrag Exp $
/**
 * Handler to filter for nodes that are not in a queue.
 */
class nodequeue_handler_filter_in_queue extends views_handler_filter_boolean_operator {
  function query() {
    $this->ensure_my_table();
    $this->query->add_where($this->options['group'], "$this->table_alias.nid " . ($this->value ? '<> 0' : 'IS NULL'));
  }
}
