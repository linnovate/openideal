<?php
// $Id: image_gallery_plugin_display_image_gallery.inc,v 1.1.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * A display plugin for image galleries.
 *
 * This embeds the gallery terms view above the gallery view that uses it.
 * We don't use an attachment because the two views are of different types
 * (nodes and terms) and killing off *all* the node view options in the
 * attachment is prohibitive (if even possible at all).
 */

/**
 * The plugin that handles a full page.
 *
 * @ingroup views_display_plugins
 */
class image_gallery_plugin_display_image_gallery extends views_plugin_display_page {
  function options_summary(&$categories, &$options) {
    // It is very important to call the parent function here:
    parent::options_summary($categories, $options);

    // Since we're childing off the 'page' type, we'll still *call* our
    // category 'page' but let's override it so it says feed settings.
    $categories['page'] = array(
      'title' => t('Gallery page settings'),
    );
  }

  /**
   * Embed the gallery term view above the normal rendering of this view.
   */
  function render() {
    // Check the current gallery view has a tid argument.
    if (isset($this->view->argument['tid'])) {
      $args = $this->view->argument['tid']->value;

      // No argument means we are at the top-level gallery.
      // We need to pass an argument of 0 to the gallery terms view so it
      // shows only the top level.
      if (count($args) == 0) {
        // Have to pass as string, otherwise it gets eaten by views validation.
        $args = array(0 => '0');
      }

      $this->gallery_terms_view = views_get_view('image_gallery_terms');

      // Tack THIS view onto the gallery terms view. We don't do anything with
      // this yet, but other handlers might need it.
      $this->gallery_terms_view->embedding_view = &$this->view;

      $output = $this->gallery_terms_view->preview('default', $args);
    }

    // Render this view as normal.
    $output .= parent::render();

    return $output;
  }

  /**
   * There are two special cases here:
   * - An empty view might have subgalleries. If that is the case we don't show
   * the empty text.
   * - Force the view title when no arguments are present.
   */
  function render_empty() {
    drupal_set_title(filter_xss_admin($this->view->get_title()));

    if (count($this->gallery_terms_view->result) == 0) {
      return $this->render_textarea('empty');
    }
  }
}

