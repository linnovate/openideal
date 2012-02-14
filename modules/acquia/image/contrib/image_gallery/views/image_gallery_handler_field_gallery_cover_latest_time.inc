<?php
// $Id: image_gallery_handler_field_gallery_cover_latest_time.inc,v 1.4.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * Field handler for the gallery's updated time.
 */
class image_gallery_handler_field_gallery_cover_latest_time extends image_gallery_handler_field_gallery_cover {
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Overwrite the descendants option so the text is relevant.
    $form['descendants']['#description'] = theme('advanced_help_topic', 'image_gallery', 'descendants') .
      t('Whether to only consider this gallery itself, or consider subgalleries all together, or recurse into subgalleries if the gallery itself is empty.');
    $form['descendants']['#options'] = array(
      'single' => t('Only get latest time from this gallery'),
      'flat' => t('Consider subgalleries, flattened.'),
      'recurse' => t('Consider subgalleries, recursively. (Warning: this can produce many queries per row if your parent galleries are empty!).'),
    );
  }

  /**
   * Returns field html.
   */
  function render($values) {
    $nid = $this->get_cover_node_nid($values);

    // If there is no node (ie gallery empty), suppress the label.
    // Because this is called for each row, but $this->options['label'] applies
    // to everything, we have to muck about and stash the original, user-set
    // value so we can restore it when needed.
    // @todo: views 6.x-2.7 will make all the label stuff superfluous.
    static $original_label;
    if (!isset($original_label)) {
      $original_label = $this->options['label'];
    }

    if (isset($nid)) {
      $this->options['label'] = $original_label;
      $latest_node = node_load($nid);
      $output = theme('image_gallery_updated', $latest_node->changed);
      return $this->render_link($output, $values);
    }
    else {
      $this->options['label'] = '';
    }
  }
}

