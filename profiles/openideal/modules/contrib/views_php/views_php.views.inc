<?php

/**
 * @file
 * Provide views handlers and plugins that allow usage of PHP.
 */

/**
 * Implements hook_views_data().
 */
function views_php_views_data() {
  $data['views']['php'] = array(
    'title' => t('PHP'),
    'help' => t('Use PHP code.'),
    'area' => array(
      'help' => t('Use PHP code to construct the output of an area.'),
      'handler' => 'views_php_handler_area',
    ),
    'field' => array(
      'help' => t('Use PHP code to construct the output of a field.'),
      'handler' => 'views_php_handler_field',
    ),
    'filter' => array(
      'help' => t('Use PHP code to filter the result of the view.'),
      'handler' => 'views_php_handler_filter',
    ),
    'sort' => array(
      'help' => t('Use PHP code to sort the result of the view.'),
      'handler' => 'views_php_handler_sort',
    ),
  );
  return $data;
}

/**
 * Implements hook_views_plugins().
 */
function views_php_views_plugins() {
  $plugins = array(
    'access' => array(
      'php' => array(
        'title' => t('PHP'),
        'help' => t('Use PHP code to grant access.'),
        'handler' => 'views_php_plugin_access',
        'uses options' => TRUE,
      ),
    ),
    'cache' => array(
      'php' => array(
        'title' => t('PHP'),
        'help' => t('Use PHP code to determine whether a should be cached.'),
        'handler' => 'views_php_plugin_cache',
        'uses options' => TRUE,
      ),
    ),
  );
  return $plugins;
}
