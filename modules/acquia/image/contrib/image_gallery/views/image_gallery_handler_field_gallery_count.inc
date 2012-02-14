<?php
// $Id: image_gallery_handler_field_gallery_count.inc,v 1.1.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * Views handler for Image gallery count field. This counts all the nodes in
 * the gallery, including descendant galleries, using taxonomy.module's
 * taxonomy_term_count_nodes.
 */
class image_gallery_handler_field_gallery_count extends views_handler_field_taxonomy {

  /**
   * Override query() so we don't query: fake field.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Return field html.
   */
  function render($values) {
    $count = taxonomy_term_count_nodes($values->tid, 'image');
    $output = theme('image_gallery_count', $count);
    return $this->render_link($output, $values);
  }
}

