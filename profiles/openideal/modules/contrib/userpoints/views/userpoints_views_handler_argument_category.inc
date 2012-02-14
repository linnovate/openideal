<?php

/**
 * Argument handler to accept a userpoints category.
 */
class userpoints_views_handler_argument_category extends views_handler_argument {
  function construct() {
    parent::construct('category');
  }

  /**
   * Override the behavior of summary_name(). Get the user friendly version
   * of the node type.
   */
  function summary_name($data) {
    return $this->category($data->{$this->name_alias});
  }

  /**
   * Override the behavior of title(). Get the user friendly version of the
   * node type.
   */
  function title() {
    return $this->category($this->argument);
  }

  function category($id) {
    $output = userpoints_get_categories();
    if (empty($output[$id])) {
      $output = t('Unknown category');
    }
    return check_plain($output[$id]);
  }
}