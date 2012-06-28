<?php

/**
 * Filter handler to accept a user id to check for nodes that user voted on.
 */
class views_handler_filter_vote_only_user_uid extends views_handler_filter_user_name {
  function query() {
    $this->ensure_my_table();
    $replace = array_fill(0, sizeof($this->value), '%d');
    $condition = $this->operator . ' (' . implode(", ", $replace) . ')';

    // We include the WHERE condition twice in the query, so we need two
    // copies of these argument values.
    $args = array_values($this->value);
    $args = array_merge($args, $args);

//    $this->query->add_where($this->options['group'], "$this->table_alias.uid " . $condition . " OR
//      ((SELECT COUNT(*) FROM {comments} c WHERE c.uid " . $condition . " AND c.nid = $this->table_alias.nid) > 0)",
//      $args);
    $this->query->add_where($this->options['group'],
      "((SELECT COUNT(*)
      FROM {comments} c
      WHERE c.uid " . $condition . "
      AND c.nid = $this->table_alias.nid) > 0
      OR
      (SELECT COUNT(*)
      FROM {votingapi_vote} v
      WHERE v.uid " . $condition . "
      AND v.content_id = $this->table_alias.nid) > 0)
      AND $this->table_alias.uid <> %d",
      $args, $args, $args);
  }
}
