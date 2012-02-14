<?php
// $Id: image_plugin_argument_default_image_size.inc,v 1.1.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * @file
 * Contains the image size argument default plugin.
 */

/**
 * Default argument plugin to pick an image size.
 */
class image_plugin_argument_default_image_size extends views_plugin_argument_default {
  var $option_name = 'default_argument_image_size';

  function argument_form(&$form, &$form_state) {
    foreach (image_get_sizes() as $key => $size) {
      $sizes[$key] = $size['label'];
    }

    $form[$this->option_name] = array(
      '#type' => 'select',
      '#title' => t('Default argument'),
      '#options' => $sizes,
      '#default_value' => $this->get_argument(),
      '#process' => array('views_process_dependency'),
      '#dependency' => array(
        'radio:options[default_action]' => array('default'),
        'radio:options[default_argument_type]' => array($this->id),
      ),
      '#dependency_count' => 2,
    );
  }
}

