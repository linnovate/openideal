<?php
// $Id: service_links.views.inc,v 1.1.2.1 2010/09/16 10:31:31 thecrow Exp $

/**
 * @file
 * Provide views data and handlers for custom_teasers_views.module
 */

/**
 * Implementation of hook_views_data().
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
 * Implementation of hook_views_handlers().
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
