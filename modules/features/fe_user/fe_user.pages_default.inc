<?php
/**
 * @file
 * fe_user.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function fe_user_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'user_view__panel_context_8f7d765e-8906-4132-94e8-78f65bf9d7e0';
  $handler->task = 'user_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'User display',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'name' => '',
  );
  $display = new panels_display();
  $display->layout = 'tow_col';
  $display->layout_settings = array(
    'items' => array(
      'canvas' => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 'main',
        ),
        'parent' => NULL,
      ),
      'main' => array(
        'type' => 'column',
        'width' => 100,
        'width_type' => '%',
        'children' => array(
          0 => 'main-row',
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'left',
          1 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Center',
        'width' => 50,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => 'center',
        'hide_empty' => 0,
      ),
      'left' => array(
        'type' => 'region',
        'title' => 'Left',
        'width' => 50,
        'width_type' => '%',
        'parent' => 'main-row',
        'class' => 'left',
        'hide_empty' => 0,
      ),
    ),
  );
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'left' => NULL,
      'region_first' => NULL,
      'region_second' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '9c0d1f7a-01e0-4aab-a796-0e30ace9244c';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-d340c5c6-f0f3-455f-a9cf-b9e4aa685527';
    $pane->panel = 'region_first';
    $pane->type = 'views_panes';
    $pane->subtype = 'user_details-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'd340c5c6-f0f3-455f-a9cf-b9e4aa685527';
    $display->content['new-d340c5c6-f0f3-455f-a9cf-b9e4aa685527'] = $pane;
    $display->panels['region_first'][0] = 'new-d340c5c6-f0f3-455f-a9cf-b9e4aa685527';
    $pane = new stdClass();
    $pane->pid = 'new-bdc0cf17-7b90-427c-9845-e2b702098beb';
    $pane->panel = 'region_first';
    $pane->type = 'views_panes';
    $pane->subtype = 'user_ideas_full-panel_pane_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'bdc0cf17-7b90-427c-9845-e2b702098beb';
    $display->content['new-bdc0cf17-7b90-427c-9845-e2b702098beb'] = $pane;
    $display->panels['region_first'][1] = 'new-bdc0cf17-7b90-427c-9845-e2b702098beb';
    $pane = new stdClass();
    $pane->pid = 'new-aca95d91-5d33-43ee-bcb5-d49a4504e77b';
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
    $pane->uuid = 'aca95d91-5d33-43ee-bcb5-d49a4504e77b';
    $display->content['new-aca95d91-5d33-43ee-bcb5-d49a4504e77b'] = $pane;
    $display->panels['region_second'][0] = 'new-aca95d91-5d33-43ee-bcb5-d49a4504e77b';
    $pane = new stdClass();
    $pane->pid = 'new-e4b76f52-457f-4f16-9794-7d965c967f2f';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'ideal_idea-most_popular_user';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
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
    $pane->uuid = 'e4b76f52-457f-4f16-9794-7d965c967f2f';
    $display->content['new-e4b76f52-457f-4f16-9794-7d965c967f2f'] = $pane;
    $display->panels['region_second'][1] = 'new-e4b76f52-457f-4f16-9794-7d965c967f2f';
    $pane = new stdClass();
    $pane->pid = 'new-f4c74dab-2bc4-45b9-a3a6-f2329067fdff';
    $pane->panel = 'region_second';
    $pane->type = 'views_panes';
    $pane->subtype = 'news-news_slider';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array();
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'f4c74dab-2bc4-45b9-a3a6-f2329067fdff';
    $display->content['new-f4c74dab-2bc4-45b9-a3a6-f2329067fdff'] = $pane;
    $display->panels['region_second'][2] = 'new-f4c74dab-2bc4-45b9-a3a6-f2329067fdff';
    $pane = new stdClass();
    $pane->pid = 'new-620660ac-5a9e-4715-a969-c6698db169e4';
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
      'context' => array(
        0 => '',
      ),
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '620660ac-5a9e-4715-a969-c6698db169e4';
    $display->content['new-620660ac-5a9e-4715-a969-c6698db169e4'] = $pane;
    $display->panels['region_second'][3] = 'new-620660ac-5a9e-4715-a969-c6698db169e4';
    $pane = new stdClass();
    $pane->pid = 'new-b5418b8e-5b82-4396-8277-b8c6e48ab3f4';
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = 'b5418b8e-5b82-4396-8277-b8c6e48ab3f4';
    $display->content['new-b5418b8e-5b82-4396-8277-b8c6e48ab3f4'] = $pane;
    $display->panels['region_second'][4] = 'new-b5418b8e-5b82-4396-8277-b8c6e48ab3f4';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['user_view__panel_context_8f7d765e-8906-4132-94e8-78f65bf9d7e0'] = $handler;

  return $export;
}
