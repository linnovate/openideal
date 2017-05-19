<?php
/**
 * @file
 * fe_news.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function fe_news_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view__panel_context_419d7e6c-8881-42c2-b7b8-87edc15f48b5';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 3;
  $handler->conf = array(
    'title' => 'New',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(
      0 => array(
        'identifier' => 'User from Node (on node.node_author)',
        'keyword' => 'user',
        'name' => 'entity_from_schema:uid-node-user',
        'context' => 'argument_entity_id:node_1',
        'id' => 1,
      ),
      1 => array(
        'identifier' => 'Node from Node (on Node: Is Part Of A Challenge [field_challenge_ref])',
        'keyword' => 'node_2',
        'name' => 'entity_from_field:field_challenge_ref-node-node',
        'delta' => 0,
        'context' => 'argument_entity_id:node_1',
        'id' => 1,
      ),
    ),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'node_type',
          'settings' => array(
            'type' => array(
              'news' => 'news',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
    'name' => '',
  );
  $display = new panels_display();
  $display->layout = 'tow_col';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => array(
        'column_type' => 'col-lg',
        'column_size' => '9',
        'offset_type' => 'col-lg-offset',
        'offset_size' => '0',
      ),
      'middle' => NULL,
      'right' => array(
        'column_type' => 'col-lg',
        'column_size' => '3',
        'offset_type' => 'col-lg-offset',
        'offset_size' => '0',
      ),
      'bottom' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
      'content_bottom' => NULL,
      'content_top' => NULL,
      'region_first' => NULL,
      'region_second' => NULL,
      'region_third' => NULL,
      'region_fourth' => NULL,
    ),
    'left' => array(
      'style' => 'bootstrap_region',
    ),
    'right' => array(
      'style' => 'bootstrap_region',
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '4842e810-22e3-4629-9dda-31b581fa08fe';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-0c3da4ca-240e-4ef0-a591-ffe87d1382e1';
    $pane->panel = 'region_first';
    $pane->type = 'node_title';
    $pane->subtype = 'node_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'link' => 1,
      'markup' => 'none',
      'id' => '',
      'class' => '',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '0c3da4ca-240e-4ef0-a591-ffe87d1382e1';
    $display->content['new-0c3da4ca-240e-4ef0-a591-ffe87d1382e1'] = $pane;
    $display->panels['region_first'][0] = 'new-0c3da4ca-240e-4ef0-a591-ffe87d1382e1';
    $pane = new stdClass();
    $pane->pid = 'new-de307de6-746b-4241-adb8-214c33819aa9';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'new_display';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 1,
      'nodes_per_page' => '1',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'page',
      'context' => array(
        0 => '',
      ),
      'override_title' => 0,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'de307de6-746b-4241-adb8-214c33819aa9';
    $display->content['new-de307de6-746b-4241-adb8-214c33819aa9'] = $pane;
    $display->panels['region_first'][1] = 'new-de307de6-746b-4241-adb8-214c33819aa9';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view__panel_context_419d7e6c-8881-42c2-b7b8-87edc15f48b5'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function fe_news_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'News';
  $page->task = 'page';
  $page->admin_title = 'Challenges';
  $page->admin_description = '';
  $page->path = 'news';
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
  $handler->name = 'page_News__panel_context_1aab52d5-5a2c-4859-9882-b9de0b27f412';
  $handler->task = 'page';
  $handler->subtask = 'News';
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
    'name' => '',
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
    $pane->pid = 'new-88a1b44c-4008-48d4-9e26-8bd255acdd33';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'news';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '5',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block',
      'override_title' => 0,
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
    $pane->uuid = '88a1b44c-4008-48d4-9e26-8bd255acdd33';
    $display->content['new-88a1b44c-4008-48d4-9e26-8bd255acdd33'] = $pane;
    $display->panels['region_first'][0] = 'new-88a1b44c-4008-48d4-9e26-8bd255acdd33';
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
    $pane->pid = 'new-7dc7c245-2d3b-4f19-836a-a85e278f06a6';
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
    $pane->uuid = '7dc7c245-2d3b-4f19-836a-a85e278f06a6';
    $display->content['new-7dc7c245-2d3b-4f19-836a-a85e278f06a6'] = $pane;
    $display->panels['region_second'][2] = 'new-7dc7c245-2d3b-4f19-836a-a85e278f06a6';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['News'] = $page;

  return $pages;

}
