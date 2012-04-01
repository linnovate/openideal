<?php
/**
 * Argument handler to accept a user id to check for nodes that
 * commented on.
 */
class views_handler_argument_idea_comment_only_user_uid extends views_handler_argument {
  function title() {
    if (!$this->argument) {
      $title = variable_get('anonymous', t('Anonymous'));
    }
    else {
      $title = db_query("SELECT u.name FROM {users} u WHERE u.uid = :uid", array(':uid' => $this->argument))->fetchField();
    }
    if (empty($title)) {
      return t('No user');
    }

    return check_plain($title);
  }

  function default_actions($which = NULL) {
    // Disallow summary views on this argument.
    if (!$which) {
      $actions = parent::default_actions();
      unset($actions['summary asc']);
      unset($actions['summary desc']);
      return $actions;
    }

    if ($which != 'summary asc' && $which != 'summary desc') {
      return parent::default_actions($which);
    }
  }
  
  function query($group_by = FALSE) {
    $this->ensure_my_table();
    $subselect = db_select('comment', 'c');
    $subselect->addField('c', 'cid');
    $subselect->condition('c.uid', $this->argument);
    $subselect->where("c.nid = $this->table_alias.nid");

    $condition = db_or()
      ->exists($subselect);

    $this->query->add_where(0, $condition);
  }
}