<?php
// $Id: image_gallery_handler_field_gallery_cover_thumbnail.inc,v 1.1.2.2 2010/08/03 17:43:00 sun Exp $

/**
 * Field handler for gallery cover node image.
 *
 * Displays a thumbnail of the cover image node, at the chosen derivative size.
 * This covers basic cases where the handler definition can specify the query
 * and the order to get the cover node.
 */
class image_gallery_handler_field_gallery_cover_thumbnail extends image_gallery_handler_field_gallery_cover {

  /**
   * Defines default values for options.
   */
  function option_definition() {
    $options = parent::option_definition();

    $options['image_derivative'] = array('default' => array(IMAGE_THUMBNAIL));

    return $options;
  }

  /**
   * Extends the field's basic options with more image specific options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Overwrite the descendants option so the text is relevant.
    $form['descendants']['#description'] = theme('advanced_help_topic', 'image_gallery', 'descendants') .
      t('Whether to go into subgalleries when getting a cover image. Either: consider only this gallery itself, consider subgalleries all together, or recurse into subgalleries one by one if the gallery itself is empty.');
    $form['descendants']['#options'] = array(
      'single' => t('Only get cover image from this gallery'),
      'flat' => t('Consider subgalleries, flattened.'),
      'recurse' => t('Consider subgalleries, recursively. (Warning: this can produce many queries per row if your parent galleries are empty!).'),
    );

    foreach (image_get_sizes() as $key => $size) {
      $sizes[$key] = $size['label'];
    }
    $form['image_derivative'] = array(
      '#type' => 'select',
      '#title' => t('Image size to show'),
      '#options' => $sizes,
      '#default_value' => $this->options['image_derivative'],
      '#description' => t('Pick an image derivative to display.'),
    );
  }

  /**
   * Returns field html.
   */
  function render($values) {
    $nid = $this->get_cover_node_nid($values);

    $latest_node = node_load($nid);
    $output = image_display($latest_node, $this->options['image_derivative']);
    $output = $this->render_link($output, $values);
    // Wrap a common class so the thumbnail can be style no matter what field.
    // TODO: theme function?
    return '<span class="image-gallery-view-cover-thumbnail">' . $output . '</span>';
  }
}

