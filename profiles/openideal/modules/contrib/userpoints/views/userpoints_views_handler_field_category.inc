<?php

/**
 * Field handler for categories.
 *
 * Copied from views_handler_field_term_node_tid.inc
 */
class userpoints_views_handler_field_category extends views_handler_field {

  function init(&$view, $options) {
    parent::init($view, $options);

    $this->additional_fields['name'] = array('table' => 'taxonomy_term_data', 'field' => 'name');
  }

  /**
   * Add this term to the query
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function pre_render($values) {
    $this->field_alias = $this->aliases['name'];
  }

  function render($values) {
    if (!empty($values->{$this->field_alias})) {
      return $values->{$this->field_alias};
    }
    return t('!Uncategorized', userpoints_translation());
  }

  function document_self_tokens(&$tokens) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = t('The taxonomy term ID for the term.');
    $tokens['[' . $this->options['id'] . '-name' . ']'] = t('The taxonomy term name for the term.');
    $tokens['[' . $this->options['id'] . '-vid' . ']'] = t('The vocabulary ID for the vocabulary the term belongs to.');
    $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = t('The name for the vocabulary the term belongs to.');
  }

  function add_self_tokens(&$tokens, $item) {
    $tokens['[' . $this->options['id'] . '-tid' . ']'] = $item['tid'];
    $tokens['[' . $this->options['id'] . '-name' . ']'] = $item['name'];
    $tokens['[' . $this->options['id'] . '-vid' . ']'] = $item['vid'];
    $tokens['[' . $this->options['id'] . '-vocabulary' . ']'] = $item['vocabulary'];
  }
}

