<?php

/**
 * @file
 * Provide views data and handlers for custom_teasers_views.module
 */

/**
 * Implements hook_views_data().
 */
function service_links_views_data() {
  // We're registering the 'custom_teasers' table.
  $data['node']['service_links'] = array(
    'title' => t('Service links'),
    'help' => t('Display links to social sharing websites like Digg, del.icio.us, reddit, Technorati etc..'),
    'field' => array(
      'handler' => 'service_links_handler_field_service_links',
    ),
  );

  return $data;
}

/**
 * Implements hook_views_handlers().
 */
function service_links_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'service_links'),
    ),
    'handlers' => array(
      'service_links_handler_field_service_links' => array(
        'parent' => 'views_handler_field',
      ),
    ),
  );
}
