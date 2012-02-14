<?php

/**
 * A handler to sort a view using PHP defined by the administrator.
 *
 * @ingroup views_sort_handlers
 */
class views_php_handler_sort extends views_handler_sort {

  protected $php_static_variable = NULL;

  /**
   * Implements views_object#option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['use_php_setup'] = array('default' => FALSE);
    $options['php_setup'] = array('default' => '');
    $options['php_sort'] = array('default' => '');
    return $options;
  }

  /**
   * Implements views_handler#option_definition().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form += views_php_form_element($this,
      array('use_php_setup', t('Use setup code'), t('If checked, you can provide PHP code to be run once right before execution of the view. This may be useful to define functions to be re-used in the value and/or output code.')),
      array('php_setup', t('Setup code'), t('Code to run right before execution of the view.'), FALSE),
      array('$view', '$handler', '$static')
    );
    $form += views_php_form_element($this,
      FALSE,
      array('php_sort', t('Sort code'), t('The comparison code must return an integer less than, equal to, or greater than zero if the first row should respectively appear before, stay where it was compared to, or appear after the second row.'), FALSE),
      array(
        '$view', '$handler', '$static',
        '$row1' => t('Data of row.'),
        '$row2' => t('Data of row to compare against.'),
      )
    );
  }

  /**
   * Implements views_handler_sort#query().
   */
  function query() {
    // Inform views_php_views_pre_execute() to seize control over the query.
    $this->view->views_php = TRUE;
  }

  /**
   *
   * @see views_php_views_pre_execute()
   */
  function php_pre_execute() {
    // Ecexute static PHP code.
    if (!empty($this->options['php_setup'])) {
      $function = create_function('$view, $handler, &$static', $this->options['php_setup'] . ';');
      ob_start();
      $function($this->view, $this, $this->php_static_variable);
      ob_end_clean();
    }
  }

  /**
   *
   * @see views_php_views_post_execute()
   */
  function php_post_execute() {
    if (!empty($this->options['php_sort']) && $this->view->style_plugin->build_sort()) {
      $this->php_sort_function = create_function('$view, $handler, &$static, $row1, $row2', $this->options['php_sort'] . ';');
      ob_start();
      usort($this->view->result, array($this, 'php_sort'));
      ob_end_clean();
    }
  }

  /**
   * Helper function; usort() callback for sort support.
   */
  function php_sort($row1, $row2) {
    $factor = strtoupper($this->options['order']) == 'ASC' ? 1 : -1;
    $function = $this->php_sort_function;
    foreach (array('row1' => 'normalized_row1', 'row2' => 'normalized_row2') as $name => $normalized_name) {
      $$normalized_name = new stdClass;
      foreach ($this->view->display_handler->get_handlers('field') as $field => $handler) {
        $$normalized_name->$field = isset($$name->{$handler->field_alias}) ? $$name->{$handler->field_alias} : NULL;
      }
    }
    $result = (int)$function($this->view, $this, $this->php_static_variable, $normalized_row1, $normalized_row2);
    return $factor * $result;
  }
}
