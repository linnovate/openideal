<?php
// $Id: image_handler_field_image_size.inc,v 1.1 2009/05/01 22:29:47 sun Exp $

/**
 * @file
 * Views handler for image size field.
 */

/**
 * Render the human-readable name of a given image size.
 *
 * @return
 *   The human-readable name or an empty string for an invalid image size.
 */
class image_handler_field_image_size extends views_handler_field {
  var $image_sizes = array();

  function init(&$view, $options) {
    parent::init($view, $options);
    $this->image_sizes = image_get_sizes();
  }

  function render($values) {
    $image_size = $values->{$this->field_alias};

    if (!empty($image_size) && isset($this->image_sizes[$image_size])) {
      return check_plain($this->image_sizes[$image_size]['label']);
    }

    return '';
  }
}

