<?php
// $Id: image_handler_relationship_node_image_file.inc,v 1.1 2009/05/01 22:29:47 sun Exp $

/**
 * @file
 * Views relationship handler for image size filter.
 */

class image_handler_relationship_node_image_file extends views_handler_relationship {

  function option_definition() {
    $options = parent::option_definition();
    $options['image_size'] = array('default' => IMAGE_THUMBNAIL);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $image_size_options = array();
    foreach (image_get_sizes() as $key => $size) {
      $image_size_options[$key] = $size['label'];
    }

    $form['image_size'] = array(
      '#type' => 'select',
      '#title' => t('Image sizes'),
      '#options' => $image_size_options,
      '#default_value' => $this->options['image_size'],
      '#multiple' => TRUE,
      '#description' => t('Which image sizes to join on. You can select none (to include all sizes), one, or multiple; Every size will introduce a new row. Example: If the result are 5 image nodes and you selected 2 sizes; The result will be 10 rows. There is no fallback; if a size does not exist (because the original is smaller) then no data will be obtained.'),
    );
  }

  function ensure_my_table() {
    if (!isset($this->table_alias)) {
      $join = $this->get_join();

      // Adjust the join for the selected image size.
      if (!empty($this->options['image_size'])) {
        $join->extra[] = array(
          'field' => 'image_size',
          'value' => $this->options['image_size'],
        );
      }
      $this->table_alias = $this->query->add_table($this->table, $this->relationship, $join);
    }
    return $this->table_alias;
  }
}

