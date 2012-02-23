<?php
/**
 * @file
 * fe_user.views_default.inc
 */

/**
 * Implements hook_views_default_views().
 */
function fe_user_views_default_views() {
  $export = array();

  $view = new view;
  $view->name = 'most_active_users';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'users';
  $view->human_name = 'Most active users';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'Most active users';
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access user profiles';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['query']['options']['query_comment'] = FALSE;
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'some';
  $handler->display->display_options['pager']['options']['items_per_page'] = '10';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  /* Field: User: Name */
  $handler->display->display_options['fields']['name']['id'] = 'name';
  $handler->display->display_options['fields']['name']['table'] = 'users';
  $handler->display->display_options['fields']['name']['field'] = 'name';
  $handler->display->display_options['fields']['name']['label'] = '';
  $handler->display->display_options['fields']['name']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['name']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['name']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['name']['alter']['word_boundary'] = 0;
  $handler->display->display_options['fields']['name']['alter']['ellipsis'] = 0;
  $handler->display->display_options['fields']['name']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['name']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['name']['alter']['html'] = 0;
  $handler->display->display_options['fields']['name']['hide_empty'] = 0;
  $handler->display->display_options['fields']['name']['empty_zero'] = 0;
  $handler->display->display_options['fields']['name']['link_to_user'] = 1;
  $handler->display->display_options['fields']['name']['overwrite_anonymous'] = 0;
  /* Field: Userpoints total: Current total points */
  $handler->display->display_options['fields']['points']['id'] = 'points';
  $handler->display->display_options['fields']['points']['table'] = 'userpoints_total';
  $handler->display->display_options['fields']['points']['field'] = 'points';
  $handler->display->display_options['fields']['points']['label'] = '';
  $handler->display->display_options['fields']['points']['alter']['alter_text'] = 0;
  $handler->display->display_options['fields']['points']['alter']['make_link'] = 0;
  $handler->display->display_options['fields']['points']['alter']['absolute'] = 0;
  $handler->display->display_options['fields']['points']['alter']['external'] = 0;
  $handler->display->display_options['fields']['points']['alter']['replace_spaces'] = 0;
  $handler->display->display_options['fields']['points']['alter']['trim_whitespace'] = 0;
  $handler->display->display_options['fields']['points']['alter']['nl2br'] = 0;
  $handler->display->display_options['fields']['points']['alter']['word_boundary'] = 1;
  $handler->display->display_options['fields']['points']['alter']['ellipsis'] = 1;
  $handler->display->display_options['fields']['points']['alter']['strip_tags'] = 0;
  $handler->display->display_options['fields']['points']['alter']['trim'] = 0;
  $handler->display->display_options['fields']['points']['alter']['html'] = 0;
  $handler->display->display_options['fields']['points']['element_label_colon'] = FALSE;
  $handler->display->display_options['fields']['points']['element_default_classes'] = 1;
  $handler->display->display_options['fields']['points']['hide_empty'] = 0;
  $handler->display->display_options['fields']['points']['empty_zero'] = 0;
  $handler->display->display_options['fields']['points']['hide_alter_empty'] = 1;
  $handler->display->display_options['fields']['points']['format_plural'] = 0;
  $handler->display->display_options['fields']['points']['suffix'] = ' points';
  /* Sort criterion: Userpoints total: Current total points */
  $handler->display->display_options['sorts']['points']['id'] = 'points';
  $handler->display->display_options['sorts']['points']['table'] = 'userpoints_total';
  $handler->display->display_options['sorts']['points']['field'] = 'points';
  $handler->display->display_options['sorts']['points']['order'] = 'DESC';
  /* Filter criterion: User: Active */
  $handler->display->display_options['filters']['status']['id'] = 'status';
  $handler->display->display_options['filters']['status']['table'] = 'users';
  $handler->display->display_options['filters']['status']['field'] = 'status';
  $handler->display->display_options['filters']['status']['value'] = '1';
  $handler->display->display_options['filters']['status']['group'] = 1;
  $handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;

  /* Display: Block */
  $handler = $view->new_display('block', 'Block', 'block');
  $export['most_active_users'] = $view;

  return $export;
}
