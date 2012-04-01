<?php
// $Id: admin_content_taxonomy.inc,v 1.5 2010/03/11 02:22:13 sun Exp $

$view = new view;
$view->name = 'admin_content_taxonomy';
$view->description = 'Manage tagging, categorization, and classification of your content.';
$view->tag = 'admin';
$view->view_php = '';
$view->base_table = 'term_data';
$view->is_cacheable = FALSE;
$view->api_version = 2;
$view->disabled = TRUE; /* Edit this to true to make a default view disabled initially */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->override_option('fields', array(
  'name_1' => array(
    'label' => '',
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
    'exclude' => 1,
    'id' => 'name_1',
    'table' => 'vocabulary',
    'field' => 'name',
    'relationship' => 'none',
  ),
  'name' => array(
    'label' => 'Term',
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
    'link_to_taxonomy' => 1,
    'exclude' => 0,
    'id' => 'name',
    'table' => 'term_data',
    'field' => 'name',
    'relationship' => 'none',
  ),
  'tid' => array(
    'label' => 'Operations',
    'alter' => array(
      'alter_text' => 1,
      'text' => 'edit',
      'make_link' => 1,
      'path' => 'admin/content/taxonomy/edit/term/[tid]',
      'link_class' => '',
      'alt' => '',
      'prefix' => '',
      'suffix' => '',
      'target' => '',
      'help' => '',
      'trim' => 0,
      'max_length' => '',
      'word_boundary' => 1,
      'ellipsis' => 1,
      'strip_tags' => 0,
      'html' => 0,
    ),
    'empty' => '',
    'hide_empty' => 0,
    'empty_zero' => 0,
    'set_precision' => FALSE,
    'precision' => 0,
    'decimal' => '.',
    'separator' => '',
    'prefix' => '',
    'suffix' => '',
    'exclude' => 0,
    'id' => 'tid',
    'table' => 'term_data',
    'field' => 'tid',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
));
$handler->override_option('sorts', array(
  'weight' => array(
    'order' => 'ASC',
    'id' => 'weight',
    'table' => 'term_data',
    'field' => 'weight',
    'relationship' => 'none',
  ),
  'name' => array(
    'order' => 'ASC',
    'id' => 'name',
    'table' => 'term_data',
    'field' => 'name',
    'relationship' => 'none',
  ),
));
$handler->override_option('arguments', array(
  'vid' => array(
    'default_action' => 'ignore',
    'style_plugin' => 'default_summary',
    'style_options' => array(),
    'wildcard' => '',
    'wildcard_substitution' => 'All',
    'title' => 'Terms in %1',
    'breadcrumb' => '',
    'default_argument_type' => 'fixed',
    'default_argument' => '',
    'validate_type' => 'none',
    'validate_fail' => 'ignore',
    'id' => 'vid',
    'table' => 'vocabulary',
    'field' => 'vid',
    'validate_user_argument_type' => 'uid',
    'validate_user_roles' => array(
      '2' => 0,
      '3' => 0,
    ),
    'override' => array(
      'button' => 'Override',
    ),
    'relationship' => 'none',
    'default_options_div_prefix' => '',
    'default_argument_user' => 0,
    'default_argument_fixed' => '',
    'validate_argument_node_type' => array(
      'image' => 0,
      'page' => 0,
      'story' => 0,
    ),
    'validate_argument_node_access' => 0,
    'validate_argument_nid_type' => 'nid',
    'validate_argument_vocabulary' => array(
      '2' => 0,
      '1' => 0,
    ),
    'validate_argument_type' => 'tid',
    'validate_argument_transform' => 0,
    'validate_user_restrict_roles' => 0,
    'default_argument_php' => '',
    'image_size' => array(
      '_original' => '_original',
      'thumbnail' => 'thumbnail',
      'preview' => 'preview',
    ),
    'validate_argument_php' => '',
  ),
));
$handler->override_option('filters', array(
  'vid' => array(
    'operator' => 'in',
    'value' => array(),
    'group' => '0',
    'exposed' => TRUE,
    'expose' => array(
      'use_operator' => 0,
      'operator' => 'vid_op',
      'identifier' => 'vid',
      'label' => 'Vocabulary',
      'optional' => 0,
      'single' => 1,
      'remember' => 1,
      'reduce' => 0,
    ),
    'id' => 'vid',
    'table' => 'term_data',
    'field' => 'vid',
    'relationship' => 'none',
    'override' => array(
      'button' => 'Override',
    ),
  ),
));
$handler->override_option('access', array(
  'type' => 'perm',
  'perm' => 'administer taxonomy',
));
$handler->override_option('cache', array(
  'type' => 'none',
));
$handler->override_option('title', 'Terms');
$handler->override_option('use_ajax', TRUE);
$handler->override_option('items_per_page', 50);
$handler->override_option('use_pager', '1');
$handler->override_option('style_plugin', 'bulk');
$handler->override_option('style_options', array(
  'grouping' => '',
  'override' => 1,
  'sticky' => 1,
  'order' => 'asc',
  'columns' => array(
    'name_1' => 'name_1',
    'name' => 'name',
    'tid' => 'tid',
  ),
  'info' => array(
    'name_1' => array(
      'sortable' => 0,
      'separator' => '',
    ),
    'name' => array(
      'sortable' => 1,
      'separator' => '',
    ),
    'tid' => array(
      'sortable' => 0,
      'separator' => '',
    ),
  ),
  'default' => 'name',
  'execution_type' => '2',
  'display_type' => '0',
  'hide_select_all' => 0,
  'skip_confirmation' => 0,
  'display_result' => 1,
  'merge_single_action' => 0,
  'selected_operations' => array(
    'views_bulk_operations_delete_term_action' => 'views_bulk_operations_delete_term_action',
    'system_message_action' => 0,
    'views_bulk_operations_action' => 0,
    'views_bulk_operations_script_action' => 0,
    'views_bulk_operations_argument_selector_action' => 0,
    'system_goto_action' => 0,
    'system_send_email_action' => 0,
  ),
));
$handler = $view->new_display('system', 'System', 'system_1');
$handler->override_option('path', 'admin/content/taxonomy/%');
