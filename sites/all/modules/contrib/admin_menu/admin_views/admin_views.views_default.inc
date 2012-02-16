<?php
// $Id: admin_views.views_default.inc,v 1.3 2010/03/11 02:20:14 sun Exp $

/**
 * @file
 * Default views.
 */

/**
 * Implements hook_views_default_views().
 */
function admin_views_views_default_views() {
  static $views;

  if (isset($views)) {
    return $views;
  }

  $files = file_scan_directory(drupal_get_path('module', 'admin_views') . '/views_default', '/\.inc$/');
  foreach ($files as $filepath => $file) {
    include $filepath;
    if (isset($view)) {
      $views[$view->name] = $view;
    }
  }
  return $views;
}

