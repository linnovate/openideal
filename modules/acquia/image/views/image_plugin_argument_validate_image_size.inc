<?php
// $Id: image_plugin_argument_validate_image_size.inc,v 1.2 2009/08/26 18:41:44 joachim Exp $

/**
 * @file
 * Views validation plugin for image size argument.
 */

/**
 * Validation handler for image size.
 *
 * Validates an image size argument and allows to restrict which image sizes
 * are valid.
 */
class image_plugin_argument_validate_image_size extends views_plugin_argument_validate {
  var $image_sizes = array();

  function init(&$view, &$argument, $id = NULL) {
    parent::init($view, $argument, $id);
    $this->image_sizes = image_get_sizes();
  }

  function validate_form(&$form, &$form_state) {
    $image_size_options = array();
    foreach ($this->image_sizes as $key => $size) {
      $image_size_options[$key] = $size['label'];
    }

    $form['image_size'] = array(
      '#type' => 'select',
      '#title' => t('Image sizes'),
      '#options' => $image_size_options,
      '#default_value' => $this->get_argument(),
      '#multiple' => TRUE,
      '#required' => TRUE,
      '#process' => array('views_process_dependency'),
      '#dependency' => array('edit-options-validate-type' => array($this->id)),
      '#description' => t("Which image sizes are allowed to be passed through this argument. Both 'original' and '_original' are valid."),
    );
  }

  function get_argument() {
    return isset($this->argument->options['image_size']) ? $this->argument->options['image_size'] : array_keys($this->image_sizes);
  }

  function validate_argument($argument) {
    // Allow 'original' as well as '_original'. It's really up to the handler to
    // deal with this and change it before it gets here, but seeing as we
    // promise to do this in the UI we should back it up here.
    if ($argument == 'original') {
      $argument = '_original';
    }

    return $this->argument->options['image_size'][$argument];
  }
}

