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
    'no_blocks' => 1,
    'pipeline' => 'ipe',
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
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '97c888e9-bd4e-4493-bf0f-e62fd1481b30';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-be2d700d-5398-49e2-bb00-8eae27379658';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'views-challenges_list-block_1';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'be2d700d-5398-49e2-bb00-8eae27379658';
    $display->content['new-be2d700d-5398-49e2-bb00-8eae27379658'] = $pane;
    $display->panels['middle'][0] = 'new-be2d700d-5398-49e2-bb00-8eae27379658';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-be2d700d-5398-49e2-bb00-8eae27379658';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['challenges'] = $page;

  return $pages;

}
