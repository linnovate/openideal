<?php
// $Id: img_assist.views.inc,v 1.1.2.2 2008/10/12 17:04:12 sun Exp $

/**
 * @file
 * Views integration for Image assist.
 */


/**
 * Implementation of hook_views_plugins().
 * 
 * Define a row style plugin to display a thumbnail in the image browser.
 */
function img_assist_views_plugins() {
  return array(
    'row' => array(
      'img_assist_thumbnail' => array(
        'title' => t('Image Assist thumbnail'),
        'handler' => 'views_plugin_row',
        'theme' => 'img_assist_browser_thumbnail',
        'theme path' => drupal_get_path('module', 'img_assist') .'/theme',
        'theme file' => 'theme.inc',
        'uses options' => FALSE,
        'type' => 'normal',
      ),
    ),
  );
}

