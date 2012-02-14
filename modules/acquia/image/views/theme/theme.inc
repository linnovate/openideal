<?php
// $Id: theme.inc,v 1.2.2.1 2010/08/03 17:43:00 sun Exp $


/**
 * @file
 *
 * Theme preprocessors for image plugins.
 */

/**
 * Preprocessor for the gallery style plugin.
 */
function template_preprocess_image_view_image_gallery(&$vars) {
  // Call the parent preprocess function to get a heap of variables.
  template_preprocess_views_view_list($vars);

  drupal_add_css(drupal_get_path('module', 'image') . '/image.css');
}

