<?php
// $Id: image_handler_argument_image_node_image_size.inc,v 1.1.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * @file
 *
 * Provide an argument to set the image size of an Image node image field.
 *
 * This expects the image_handler_field_image_node_image to be present.
 */
class image_handler_argument_image_node_image_size extends views_handler_argument_string {

  /**
   * Change the argument's options for more image specific options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Wildcard makes no sense for this argument: remove it.
    unset($form['wildcard']);
    unset($form['wildcard_substitution']);
  }

  /**
   * Provide a list of default behaviors for this argument if the argument
   * is not present.
   *
   * Override the parent method to provide only the behaviours that make sense.
   */
  function default_actions($which = NULL) {
    $defaults = array(
      'ignore' => array(
        'title' => t('Ignore and use the field setting'),
        'method' => 'default_ignore',
        // generate a breadcrumb to here
        'breadcrumb' => TRUE,
      ),
      'not found' => array(
        'title' => t('Hide view / Page not found (404)'),
        'method' => 'default_not_found',
        // This is a hard fail condition.
        'hard fail' => TRUE,
      ),
      'empty' => array(
        'title' => t('Display empty text'),
        'method' => 'default_empty',
        // generate a breadcrumb to here
        'breadcrumb' => TRUE,
      ),
      'default' => array(
        'title' => t('Provide default argument'),
        'method' => 'default_default',
        'form method' => 'default_argument_form',
        'has default argument' => TRUE,
        // this can only be used for missing argument, not validation failure
        'default only' => TRUE,
      ),
    );

    if ($which) {
      if (!empty($defaults[$which])) {
        return $defaults[$which];
      }
    }
    else {
      return $defaults;
    }
  }

  /**
   * Allow 'original' as well as '_original' for nicer URLs.
   */
  function set_argument($arg) {
    if ($arg == 'original') {
      $arg = '_original';
    }

    return parent::set_argument($arg);
  }

  /**
   * Check that the image node image field is present.
   */
  function validate() {
    // @todo This will not catch cases involving defaulted handlers.
    //   For example, it is possible to add a display which inherits this
    //   argument, but overrides its fields.
    // @see <http://drupal.org/node/544098>
    if (!isset($this->view->display_handler->handlers['field']['image_image'])) {
      $errors[] = t('The @argument argument requires the "Image: Image" field.', array('@argument' => $this->definition['title']));
    }
    return $errors;
  }

  /**
   * Don't query, but change the image field's values instead.
   *
   * @todo This only works with the image node field.
   * For it to work on image attach fields too, the handler for that should
   * probably be subclassed (so it uses the same option name for starters).
   * This handler would need to look at all the fields and have a best stab
   * at guessing which one to fiddle with. Remember to change validate() too!
   */
  function query() {
    $argument = $this->argument;

    // Checking here so we don't get nasty errors when validation doesn't work.
    // @see validate() for details.
    if (isset($this->view->field['image_image'])) {
      $this->view->field['image_image']->options['image_derivative'] = $argument;
    }
  }
}

