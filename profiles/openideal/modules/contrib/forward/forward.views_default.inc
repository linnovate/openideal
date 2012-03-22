<?php

/**
 * Implements hook_views_default_views().
 */
function forward_views_default_views() {
  $view = new view;
  $view->name = 'forward_forwards';
  $view->description = 'Display most forwarded nodes';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'forward_count' => array(
      'label' => 'Forwards',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'forward_count',
      'table' => 'forward_statistics',
      'field' => 'forward_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'forward_count' => array(
      'order' => 'DESC',
      'id' => 'forward_count',
      'table' => 'forward_statistics',
      'field' => 'forward_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status_extra' => array(
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
    ),
    'forward_count' => array(
      'operator' => '>=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'forward_count',
      'table' => 'forward_statistics',
      'field' => 'forward_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('title', 'Most forwarded');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('style_plugin', 'list');
  $handler = $view->new_display('block', 'Block', 'block_1');

  $views[$view->name] = $view;


  $view = new view;
  $view->name = 'forward_clickthroughs';
  $view->description = 'Display forwarded nodes most with most clickthroughs';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'clickthrough_count' => array(
      'label' => 'Forwards',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'clickthrough_count',
      'table' => 'forward_statistics',
      'field' => 'clickthrough_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'clickthrough_count' => array(
      'order' => 'DESC',
      'id' => 'clickthrough_count',
      'table' => 'forward_statistics',
      'field' => 'clickthrough_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status_extra' => array(
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
    ),
    'clickthrough_count' => array(
      'operator' => '>=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'clickthrough_count',
      'table' => 'forward_statistics',
      'field' => 'clickthrough_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('title', 'Most clickthroughs');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('style_plugin', 'list');
  $handler = $view->new_display('block', 'Block', 'block_1');

  $views[$view->name] = $view;


  $view = new view;
  $view->name = 'forward_recent';
  $view->description = 'Display recently forwarded nodes';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'title' => array(
      'label' => '',
      'link_to_node' => 1,
      'exclude' => 0,
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'relationship' => 'none',
    ),
    'forward_count' => array(
      'label' => 'Forwards',
      'set_precision' => FALSE,
      'precision' => 0,
      'decimal' => '.',
      'separator' => ',',
      'prefix' => '',
      'suffix' => '',
      'exclude' => 0,
      'id' => 'forward_count',
      'table' => 'forward_statistics',
      'field' => 'forward_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'last_forward_timestamp' => array(
      'order' => 'DESC',
      'granularity' => 'second',
      'id' => 'last_forward_timestamp',
      'table' => 'forward_statistics',
      'field' => 'last_forward_timestamp',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('filters', array(
    'status_extra' => array(
      'id' => 'status_extra',
      'table' => 'node',
      'field' => 'status_extra',
    ),
    'forward_count' => array(
      'operator' => '>=',
      'value' => array(
        'value' => '1',
        'min' => '',
        'max' => '',
      ),
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'forward_count',
      'table' => 'forward_statistics',
      'field' => 'forward_count',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'none',
    'role' => array(),
    'perm' => '',
  ));
  $handler->override_option('title', 'Recent forwards');
  $handler->override_option('items_per_page', 5);
  $handler->override_option('style_plugin', 'list');
  $handler = $view->new_display('block', 'Block', 'block_1');

  $views[$view->name] = $view;

  return $views;
}

