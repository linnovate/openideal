<?php
// $Id: theme.inc,v 1.1.2.1 2008/10/12 17:04:12 sun Exp $

/**
 * @file
 * Theme functions for Image assist.
 */


/**
 * Setup variables for the browser thumbnail template.
 * 
 * @param array $vars
 */
function template_preprocess_img_assist_browser_thumbnail(&$vars) {
  $node = node_load($vars['row']->nid);

  $vars['thumbnail'] = img_assist_display($node, IMAGE_THUMBNAIL);
  $vars['path'] = url('img_assist/properties/'. $node->nid); 
  $vars['title'] = $node->title; 
}

