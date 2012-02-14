<?php
// $Id: image.inc,v 1.1.4.7 2010/03/21 13:11:23 kiam Exp $

/**
 * @file
 * Integration file for image.module.
 */

/**
 * Implements hook_nodewords_type_id().
 */
function image_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'image') {
    // Image gallery paths: image/ , image/???/$tid.
    if (!isset($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_VOCABULARY;
      $result['id'] = variable_get('image_gallery_nav_vocabulary', 0);
    }
    elseif (isset($arg[2]) && is_numeric($arg[2])) {
      $result['type'] = NODEWORDS_TYPE_TERM;
      $result['id'] = $arg[1];
    }
  }
}
