<?php
// $Id: image_handler_field_image_node_image.inc,v 1.3 2009/08/26 14:42:04 joachim Exp $

/**
 * @file
 * Views handler for image field.
 *
 * This is a fake field that does not query anything. Instead, it adds the node
 * table fields we need to build a fake node object to send to Image module to
 * load the proper image.
 */

/**
 * Field handler to provide an image tag.
 *
 * Inherit from views_handler_field_node so we get the 'link to node' stuff for
 * free.
 */
class image_handler_field_image_node_image extends views_handler_field_node {

  /**
   * Constructor to provide additional fields to add.
   *
   * Adds the fields that image_load() will need.
   */
  function construct() {
    parent::construct();

    $this->additional_fields['image_node_nid'] = array(
      'table' => 'node',
      'field' => 'nid',
    );
    // Node title for image rendering.
    $this->additional_fields['image_node_title'] = array(
      'table' => 'node',
      'field' => 'title',
    );
    // Node type to verify it is an image node.
    $this->additional_fields['image_node_type'] = array(
      'table' => 'node',
      'field' => 'type',
    );
  }

  /**
   * Define default values for options.
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['image_derivative'] = array('default' => array(IMAGE_THUMBNAIL));
    return $options;
  }

  /**
   * Extend the field's basic options with more image specific options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

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
   * query() override to not query this fake field.
   */
  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  /**
   * Return field html if the result is an image node.
   */
  function render($values) {
    if ($values->{$this->aliases['image_node_type']} == 'image') {
      $image_html = $this->render_html($values);

      // We inherit render_link() from views_handler_field_node, which takes
      // care of providing a link to the node if requested.
      return $this->render_link($image_html, $values);
    }
    else {
      return NULL;
    }
  }

  /**
   * Return image html, using image_load() and image_display().
   *
   * We rely on Image module to handle getting the data because although we can
   * use the derivative option to restrict the join, we do not have the agility
   * to fall back to the original when the requested derivative is larger than
   * the image and is absent from the system.
   */
  function render_html($values) {
    $derivative = $this->options['image_derivative'];

    $node = $this->build_image_display_node($values);

    // image_load() will load the files for all derivatives. Derivatives larger
    // than the original fall back to the original. Stale derivatives will be
    // regenerated.
    image_load($node);

    return image_display($node, $derivative);
  }

  /**
   * Build a pseudo node suitable for image_load() and image_display().
   *
   * This is lighter than using node_load(). image_load() needs:
   * - title (for rendering)
   * - nid
   */
  function build_image_display_node($values) {
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['image_node_nid']};
    $node->title = $values->{$this->aliases['image_node_title']};
    return $node;
  }
}

