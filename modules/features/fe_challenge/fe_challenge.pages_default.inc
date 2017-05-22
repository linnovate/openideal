<?php
/**
 * @file
 * fe_challenge.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function fe_challenge_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'challenges';
  $page->task = 'page';
  $page->admin_title = 'Challenges';
  $page->admin_description = '';
  $page->path = 'challenges';
  $page->access = array(
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Challenges',
    'name' => 'main-menu',
    'weight' => '-1',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array();
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_challenges_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'challenges';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Panel',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'tow_col';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
      'region_first' => NULL,
      'region_second' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '97c888e9-bd4e-4493-bf0f-e62fd1481b30';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-99148fab-a45f-42d2-a64b-7582a929d197';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'challenges_list';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '0',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_1',
      'override_title' => 1,
      'override_title_text' => 'Challenges',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '99148fab-a45f-42d2-a64b-7582a929d197';
    $display->content['new-99148fab-a45f-42d2-a64b-7582a929d197'] = $pane;
    $display->panels['region_first'][0] = 'new-99148fab-a45f-42d2-a64b-7582a929d197';
    $pane = new stdClass();
    $pane->pid = 'new-7faa4b27-a2a1-4a92-8cf1-b600877e9dc8';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'ideal_blocks-ideal_key_stats';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '7faa4b27-a2a1-4a92-8cf1-b600877e9dc8';
    $display->content['new-7faa4b27-a2a1-4a92-8cf1-b600877e9dc8'] = $pane;
    $display->panels['region_second'][0] = 'new-7faa4b27-a2a1-4a92-8cf1-b600877e9dc8';
    $pane = new stdClass();
    $pane->pid = 'new-f14c9369-4dbe-44a6-bba0-1976bd6367d0';
    $pane->panel = 'region_second';
    $pane->type = 'views';
    $pane->subtype = 'activity';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '11',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_3',
      'override_title' => 1,
      'override_title_text' => 'Comments',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'display-comments',
    );
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'f14c9369-4dbe-44a6-bba0-1976bd6367d0';
    $display->content['new-f14c9369-4dbe-44a6-bba0-1976bd6367d0'] = $pane;
    $display->panels['region_second'][1] = 'new-f14c9369-4dbe-44a6-bba0-1976bd6367d0';
    $pane = new stdClass();
    $pane->pid = 'new-be348ec6-965c-44f6-a8ac-bc346900d372';
    $pane->panel = 'region_second';
    $pane->type = 'views';
    $pane->subtype = 'activity';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '4',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'comment_hp',
      'override_title' => 1,
      'override_title_text' => 'Comments',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'be348ec6-965c-44f6-a8ac-bc346900d372';
    $display->content['new-be348ec6-965c-44f6-a8ac-bc346900d372'] = $pane;
    $display->panels['region_second'][2] = 'new-be348ec6-965c-44f6-a8ac-bc346900d372';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['challenges'] = $page;

  return $pages;

}
