<?php
// $Id: theme.inc,v 1.1 2009/08/27 12:10:57 joachim Exp $
/**
 * @file
 *
 * Theme preprocessors for image gallery plugins.
 */

/**
 * Preprocessor for the gallery terms style plugin.
 */
function template_preprocess_image_gallery_view_image_gallery_terms(&$vars) {
  drupal_add_css(drupal_get_path('module', 'image_gallery') .'/image_gallery.css');
}
