<?php

/**
 * Implements hook_views_default_views().
 */
function flag_abuse_views_default_views() {
  $views = $functions = array();

  if (module_exists('flag')) {
    foreach (array_filter(flag_abuse_get_abuse_flags()) as $flag_name) {
      if ($flag = flag_get_flag($flag_name)) {
        $type = $flag->content_type;
        $function = "_flag_abuse_views_default_abuse_$type";
        if (function_exists($function)) {
          $functions[$function][$flag_name] = $flag->title;
        }
      }
    }
  }

  if (!empty($functions)) {
    foreach ($functions as $function => $flags) {
      $function($views, $flags);
    }
  }

  return $views;
}

function _flag_abuse_views_default_abuse_comment(&$views, $flags) {
  $view = new view;
  $view->name = 'all_flag_abuse_comment';
  $view->description = 'View for flag: Comment Abuse ';
  $view->tag = 'flag.abuse';
  $view->base_table = 'comment';
  $view->human_name = '';
  $view->core = 0;
  $view->api_version = '3.0-alpha1';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Defaults */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->display->display_options['title'] = 'Comment Abuse';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'reset abuse flags';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = 25;
  $handler->display->display_options['style_plugin'] = 'table';
  $handler->display->display_options['style_options']['columns'] = array(
    'timestamp' => 'timestamp',
    'name_1' => 'name_1',
    'title' => 'title',
    'name_2' => 'name_2',
    'entity_id' => 'entity_id',
    'delete_comment' => 'delete_comment',
    'ops' => 'ops',
  );
  $handler->display->display_options['style_options']['default'] = 'timestamp';
  $handler->display->display_options['style_options']['info'] = array(
    'timestamp' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'name_1' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'name_2' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'entity_id' => array(
      'sortable' => 0,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
    ),
    'delete_comment' => array(
      'align' => '',
      'separator' => '',
    ),
    'ops' => array(
      'align' => '',
      'separator' => '',
    ),
  );
  $handler->display->display_options['style_options']['override'] = 1;
  $handler->display->display_options['style_options']['sticky'] = 0;
  /* Empty text: Global: Text area */
  $handler->display->display_options['empty']['text']['id'] = 'text';
  $handler->display->display_options['empty']['text']['table'] = 'views';
  $handler->display->display_options['empty']['text']['field'] = 'area';
  $handler->display->display_options['empty']['text']['empty'] = FALSE;
  $handler->display->display_options['empty']['text']['content'] = 'No comments have been reported...';
  $handler->display->display_options['empty']['text']['format'] = 'plain_text';
  /* Relationship: Flags: comment_abuse */
  $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'comment';
  $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['flag_content_rel']['flag'] = 'comment_abuse';
  $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
  /* Relationship: Comment: Node */
  $handler->display->display_options['relationships']['nid']['id'] = 'nid';
  $handler->display->display_options['relationships']['nid']['table'] = 'comment';
  $handler->display->display_options['relationships']['nid']['field'] = 'nid';
  $handler->display->display_options['relationships']['nid']['required'] = 1;
  /* Relationship: Flags: User */
  $handler->display->display_options['relationships']['uid']['id'] = 'uid';
  $handler->display->display_options['relationships']['uid']['table'] = 'flag_content';
  $handler->display->display_options['relationships']['uid']['field'] = 'uid';
  $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['relationships']['uid']['required'] = 1;
  /* Field: Flags: Flagged time */
  $handler->display->display_options['fields']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['table'] = 'flag_content';
  $handler->display->display_options['fields']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['fields']['timestamp']['relationship'] = 'flag_content_rel';
  $handler->display->display_options['fields']['timestamp']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['timestamp']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['timestamp']['alter']['html'] = 0;
  $handler->display->display_options['fields']['timestamp']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['timestamp']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['timestamp']['hide_empty'] = 0;
  $handler->display->display_options['fields']['timestamp']['empty_zero'] = 0;
  $handler->display->display_options['fields']['timestamp']['date_format'] = 'custom';
  $handler->display->display_options['fields']['timestamp']['custom_date_format'] = 'j M, Y';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name_1']['id'] = 'name_1';
  $handler->display->display_options['fields']['name_1']['table'] = 'users';
  $handler->display->display_options['fields']['name_1']['field'] = 'name';
  $handler->display->display_options['fields']['name_1']['relationship'] = 'uid';
  $handler->display->display_options['fields']['name_1']['label'] = 'Reported by';
  $handler->display->display_options['fields']['name_1']['link_to_user'] = 1;
  /* Field: Node: Title */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'node';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['relationship'] = 'nid';
  $handler->display->display_options['fields']['title']['label'] = 'Parent Node';
  $handler->display->display_options['fields']['title']['link_to_node'] = 1;
  /* Field: Comment: Author */
  $handler->display->display_options['fields']['name_2']['id'] = 'name_2';
  $handler->display->display_options['fields']['name_2']['table'] = 'comment';
  $handler->display->display_options['fields']['name_2']['field'] = 'name';
  /* Field: Fields: comment_body */
  $handler->display->display_options['fields']['entity_id']['id'] = 'entity_id';
  $handler->display->display_options['fields']['entity_id']['table'] = 'field_data_comment_body';
  $handler->display->display_options['fields']['entity_id']['field'] = 'entity_id';
  /* Field: Comment: Delete link */
  $handler->display->display_options['fields']['delete_comment']['id'] = 'delete_comment';
  $handler->display->display_options['fields']['delete_comment']['table'] = 'comment';
  $handler->display->display_options['fields']['delete_comment']['field'] = 'delete_comment';
  $handler->display->display_options['fields']['delete_comment']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['delete_comment']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['delete_comment']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['delete_comment']['alter']['html'] = 0;
  $handler->display->display_options['fields']['delete_comment']['element_label_colon'] = 1;
  $handler->display->display_options['fields']['delete_comment']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['delete_comment']['hide_empty'] = 0;
  $handler->display->display_options['fields']['delete_comment']['empty_zero'] = 0;
  /* Field: Flags: Flag link */
  $handler->display->display_options['fields']['ops']['id'] = 'ops';
  $handler->display->display_options['fields']['ops']['table'] = 'flag_content';
  $handler->display->display_options['fields']['ops']['field'] = 'ops';
  $handler->display->display_options['fields']['ops']['relationship'] = 'flag_content_rel';
  /* Sort criterion: Flags: Flagged time */
  $handler->display->display_options['sorts']['timestamp']['id'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['table'] = 'flag_content';
  $handler->display->display_options['sorts']['timestamp']['field'] = 'timestamp';
  $handler->display->display_options['sorts']['timestamp']['relationship'] = 'flag_content_rel';
  /* Filter: Comment: Approved */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'comment';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';

  /* Display: Page */
  $first = TRUE;
  foreach ($flags as $flag_name => $flag_title) {
    $handler = $view->new_display('page', "Page: {$flag_title}", "page_{$flag_name}");
    $handler->display->display_options['defaults']['relationships'] = FALSE;
    /* Relationship: Flags: comment_abuse */
    $handler->display->display_options['relationships']['flag_content_rel']['id'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['table'] = 'comment';
    $handler->display->display_options['relationships']['flag_content_rel']['field'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['flag_content_rel']['flag'] = $flag_name;
    $handler->display->display_options['relationships']['flag_content_rel']['user_scope'] = 'any';
    /* Relationship: Comment: Node */
    $handler->display->display_options['relationships']['nid']['id'] = 'nid';
    $handler->display->display_options['relationships']['nid']['table'] = 'comment';
    $handler->display->display_options['relationships']['nid']['field'] = 'nid';
    $handler->display->display_options['relationships']['nid']['required'] = 1;
    /* Relationship: Flags: User */
    $handler->display->display_options['relationships']['uid']['id'] = 'uid';
    $handler->display->display_options['relationships']['uid']['table'] = 'flag_content';
    $handler->display->display_options['relationships']['uid']['field'] = 'uid';
    $handler->display->display_options['relationships']['uid']['relationship'] = 'flag_content_rel';
    $handler->display->display_options['relationships']['uid']['required'] = 1;

    $handler->display->display_options['path'] = "admin/content/comment/flag-abuse/{$flag_name}";
    $handler->display->display_options['menu']['title'] = $flag_title;
    $handler->display->display_options['menu']['weight'] = '0';
    $handler->display->display_options['menu']['type'] = 'tab';

    if ($first) {
      $handler->display->display_options['menu']['type'] = 'default tab';
      $handler->display->display_options['tab_options']['type'] = 'tab';
      $handler->display->display_options['tab_options']['title'] = 'Reported comments';
      $handler->display->display_options['tab_options']['weight'] = '10';
      $first = FALSE;
    }
  }

  $translatables['all_flag_abuse_comment'] = array(
    t('Defaults'),
    t('Comment Abuse'),
    t('more'),
    t('Apply'),
    t('Reset'),
    t('Sort By'),
    t('Asc'),
    t('Desc'),
    t('Items per page'),
    t('- All -'),
    t('Offset'),
    t('No comments have been reported...'),
    t('flag'),
    t('Node'),
    t('Flag user'),
    t('Flagged time'),
    t('Reported by'),
    t('Parent Node'),
    t('Author'),
    t('comment_body'),
    t('Delete link'),
    t('Flag link'),
    t('Page'),
  );
  $views[$view->name] = $view;
}

function _flag_abuse_views_default_abuse_node(&$views) {
  $view = new view;
  $view->name = 'all_flag_abuse_node';
  $view->description = 'View for flag: Node Abuse';
  $view->tag = 'flag.abuse';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to TRUE to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'abuse_node',
      'required' => 1,
      'flag' => 'abuse_node',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'node',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
    'uid' => array(
      'label' => 'user',
      'required' => 0,
      'id' => 'uid',
      'table' => 'flag_content',
      'field' => 'uid',
      'relationship' => 'flag_content_rel',
    ),
  ));
  $handler->override_option('fields', array(
    'name_1' => array(
      'label' => 'Report by',
      'link_to_user' => 1,
      'exclude' => 0,
      'id' => 'name_1',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'uid',
    ),
    'type' => array(
      'label' => 'Type',
      'link_to_node' => 0,
      'exclude' => 1,
      'id' => 'type',
      'table' => 'node',
      'field' => 'type',
      'relationship' => 'none',
    ),
    'title' => array(
      'id' => 'title',
      'table' => 'node',
      'field' => 'title',
      'label' => 'Title',
      'link_to_node' => 1,
    ),
    'name' => array(
      'label' => 'Author',
      'link_to_user' => 1,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
    ),
    'body' => array(
      'label' => 'Body',
      'exclude' => 0,
      'id' => 'body',
      'table' => 'node_revisions',
      'field' => 'body',
      'relationship' => 'none',
    ),
    'edit_node' => array(
      'label' => 'Actions',
      'text' => 'edit',
      'exclude' => 0,
      'id' => 'edit_node',
      'table' => 'node',
      'field' => 'edit_node',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'delete_node' => array(
      'label' => '',
      'text' => 'remove',
      'exclude' => 0,
      'id' => 'delete_node',
      'table' => 'node',
      'field' => 'delete_node',
      'override' => array(
        'button' => 'Override',
      ),
      'relationship' => 'none',
    ),
    'ops' => array(
      'label' => '',
      'alter' => array(
        'alter_text' => 0,
        'text' => '',
        'make_link' => 0,
        'path' => '',
        'alt' => '',
        'prefix' => '',
        'suffix' => '',
        'help' => '',
        'trim' => 0,
        'max_length' => '',
        'word_boundary' => 1,
        'ellipsis' => 1,
        'html' => 0,
      ),
      'link_type' => '',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
      'override' => array(
        'button' => 'Override',
      ),
    ),
  ));
  $handler->override_option('filters', array(
    'status' => array(
      'operator' => '=',
      'value' => 1,
      'group' => '0',
      'exposed' => FALSE,
      'expose' => array(
        'operator' => FALSE,
        'label' => '',
      ),
      'id' => 'status',
      'table' => 'node',
      'field' => 'status',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'role',
    'role' => array(
      '2' => '2',
    ),
    'perm' => '',
  ));
  $handler->override_option('title', 'Node abuse');
  $handler->override_option('empty', 'No content has been reported.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', '25');
  $handler->override_option('use_pager', TRUE);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => 'type',
    'override' => 0,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name_1' => 'name_1',
      'type' => 'type',
      'title' => 'title',
      'name' => 'name',
      'body' => 'body',
      'edit_node' => 'edit_node',
      'delete_node' => 'edit_node',
      'ops' => 'edit_node',
    ),
    'info' => array(
      'name_1' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'type' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'title' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'body' => array(
        'separator' => '',
      ),
      'edit_node' => array(
        'separator' => ' | ',
      ),
      'delete_node' => array(
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
    ),
    'default' => '-1',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'admin/content/flags/abuse_node');
  $handler->override_option('menu', array(
    'type' => 'default tab',
    'title' => 'Reported node abuse',
    'description' => '',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'normal',
    'title' => 'Reported abuse',
    'description' => '',
    'weight' => '',
  ));
  $views[$view->name] = $view;
}

function _flag_abuse_views_default_abuse_user(&$views) {
  $view = new view;
  $view->name = 'all_flag_abuse_user';
  $view->description = 'View for flag: User Abuse';
  $view->tag = 'flag.abuse';
  $view->view_php = '';
  $view->base_table = 'users';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to TRUE to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
    'flag_content_rel' => array(
      'label' => 'abuse_user',
      'required' => 1,
      'flag' => 'abuse_user',
      'user_scope' => 'any',
      'id' => 'flag_content_rel',
      'table' => 'users',
      'field' => 'flag_content_rel',
      'relationship' => 'none',
    ),
  ));
  $handler->override_option('fields', array(
    'name' => array(
      'label' => 'Name',
      'link_to_user' => 1,
      'exclude' => 0,
      'id' => 'name',
      'table' => 'users',
      'field' => 'name',
      'relationship' => 'none',
    ),
    'ops' => array(
      'label' => 'Ops',
      'exclude' => 0,
      'id' => 'ops',
      'table' => 'flag_content',
      'field' => 'ops',
      'relationship' => 'flag_content_rel',
    ),
  ));
  $handler->override_option('access', array(
    'type' => 'role',
    'role' => array(
      '2' => '2',
    ),
    'perm' => '',
  ));
  $handler->override_option('title', 'User abuse');
  $handler->override_option('empty', 'No users have been reported.');
  $handler->override_option('empty_format', '1');
  $handler->override_option('items_per_page', 30);
  $handler->override_option('use_pager', TRUE);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
    'grouping' => '',
    'override' => 1,
    'sticky' => 0,
    'order' => 'asc',
    'columns' => array(
      'name' => 'name',
      'ops' => 'ops',
    ),
    'info' => array(
      'name' => array(
        'sortable' => 1,
        'separator' => '',
      ),
      'ops' => array(
        'separator' => '',
      ),
    ),
    'default' => 'name',
  ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'admin/content/flags/abuse_user');
  $handler->override_option('menu', array(
    'type' => 'tab',
    'title' => 'Reported user abuse',
    'weight' => '0',
    'name' => 'navigation',
  ));
  $handler->override_option('tab_options', array(
    'type' => 'none',
    'title' => NULL,
    'weight' => 0,
  ));
  $views[$view->name] = $view;
}
