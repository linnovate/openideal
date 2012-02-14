<?php
// $Id: rules_scheduler.views_default.inc,v 1.1.2.1 2009/07/13 13:34:27 fago Exp $

/**
 * @file
 * Views integration for the rules scheduler module.
 */

/**
 * Implementation of hook_views_default_views().
 */
function rules_scheduler_views_default_views() {
  $view = new view;
  $view->name = 'rules_scheduler';
  $view->description = 'Rules scheduler view';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'rules_scheduler';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('fields', array(
    'tid' => array(
      'label' => 'Task ID',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'tid',
      'table' => 'rules_scheduler',
      'field' => 'tid',
      'relationship' => 'none',
    ),
    'set_name' => array(
      'label' => 'Rule set name',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'set_name',
      'table' => 'rules_scheduler',
      'field' => 'set_name',
      'relationship' => 'none',
    ),
    'date' => array(
      'label' => 'Scheduled date',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'date_format' => 'custom',
      'custom_date_format' => 'Y-m-d H:i:s',
      'exclude' => 0,
      'id' => 'date',
      'table' => 'rules_scheduler',
      'field' => 'date',
      'relationship' => 'none',
    ),
    'identifier' => array(
      'label' => 'User provided identifier',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'link_class' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'identifier',
      'table' => 'rules_scheduler',
      'field' => 'identifier',
      'relationship' => 'none',
    ),
    'nothing' => array(
      'label' => 'Operations',
      'alter' => array(
        'text' => 'delete',
        'make_link' => 1,
        'path' => 'admin/rules/rule_sets/scheduling/[tid]/delete',
        'link_class' => '',
        'alt' => 'Delete this scheduled task',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'strip_tags' => 0,
        'html' => 0,
      ),
      'exclude' => 0,
      'id' => 'nothing',
      'table' => 'views',
      'field' => 'nothing',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('sorts', array(
    'date' => array(
      'order' => 'ASC',
      'id' => 'date',
      'table' => 'rules_scheduler',
      'field' => 'date',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'perm',
    'perm' => 'administer rules',
  ));
  $handler->override_option('cache', array(
    'type' => 'none',
  ));
  $handler->override_option('empty', 'No tasks scheduled.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 30);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'set_name' => 'set_name',
      'date' => 'date',
      'tid' => 'tid',
    ),
    'info' => array(
      'set_name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'date' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'tid' => array(
        'sortable' => 1,
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));

  $views = array();
  $views[$view->name] = $view;
  return $views;
}
