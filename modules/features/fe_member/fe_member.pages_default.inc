<?php
/**
 * @file
 * fe_member.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function fe_member_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'user_edit__panel_context_01ac1be0-7c32-4495-9abc-5032d0e019c6';
  $handler->task = 'user_edit';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'user edit',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(
      0 => array(
        'identifier' => 'User edit form',
        'keyword' => 'user_edit',
        'name' => 'user_edit_form',
        'uid' => NULL,
        'id' => 1,
      ),
      1 => array(
        'identifier' => 'User',
        'keyword' => 'user_2',
        'name' => 'user',
        'type' => 'current',
        'uid' => '',
        'id' => 1,
      ),
    ),
    'relationships' => array(
      0 => array(
        'identifier' => 'User category edit form from user',
        'keyword' => 'user_category_form',
        'name' => 'user_category_edit_form_from_user',
        'context' => 'argument_user_edit_1',
        'id' => 1,
      ),
    ),
    'name' => '',
    'access' => array(
      'plugins' => array(),
    ),
  );
  $display = new panels_display();
  $display->layout = 'onecol_row_stack';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'region_first' => NULL,
      'region_second' => NULL,
      'region_third' => NULL,
      'region_fourth' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '889ebfd9-f38a-4762-bea6-6a023f5e7063';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-94a4096e-d9b1-4eec-990a-549d6d577645';
    $pane->panel = 'region_first';
    $pane->type = 'user_picture';
    $pane->subtype = 'user_picture';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_user_edit_1',
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
    $pane->uuid = '94a4096e-d9b1-4eec-990a-549d6d577645';
    $display->content['new-94a4096e-d9b1-4eec-990a-549d6d577645'] = $pane;
    $display->panels['region_first'][0] = 'new-94a4096e-d9b1-4eec-990a-549d6d577645';
    $pane = new stdClass();
    $pane->pid = 'new-cc75051b-f8e2-4a59-be2a-c448c46cd8b1';
    $pane->panel = 'region_first';
    $pane->type = 'entity_form_field';
    $pane->subtype = 'user:field_bio';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => '',
      'formatter' => '',
      'context' => 'argument_user_edit_1',
      'override_title' => 0,
      'override_title_text' => 'dfgdsfgdf',
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
    $pane->uuid = 'cc75051b-f8e2-4a59-be2a-c448c46cd8b1';
    $display->content['new-cc75051b-f8e2-4a59-be2a-c448c46cd8b1'] = $pane;
    $display->panels['region_first'][1] = 'new-cc75051b-f8e2-4a59-be2a-c448c46cd8b1';
    $pane = new stdClass();
    $pane->pid = 'new-16d44db8-f479-40cb-9a05-2c71502e6327';
    $pane->panel = 'region_first';
    $pane->type = 'form';
    $pane->subtype = 'form';
    $pane->shown = TRUE;
    $pane->access = array(
      'plugins' => array(),
      'logic' => 'and',
    );
    $pane->configuration = array(
      'context' => 'context_user_edit_form_1',
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '16d44db8-f479-40cb-9a05-2c71502e6327';
    $display->content['new-16d44db8-f479-40cb-9a05-2c71502e6327'] = $pane;
    $display->panels['region_first'][2] = 'new-16d44db8-f479-40cb-9a05-2c71502e6327';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['user_edit__panel_context_01ac1be0-7c32-4495-9abc-5032d0e019c6'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function fe_member_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'member_edit';
  $page->task = 'page';
  $page->admin_title = 'member edit';
  $page->admin_description = 'Enable a member to edit it\'s own page';
  $page->path = 'members/%user/edit';
  $page->access = array(
    'plugins' => array(
      0 => array(
        'name' => 'compare_users',
        'settings' => array(
          'equality' => '1',
        ),
        'context' => array(
          0 => 'logged-in-user',
          1 => 'argument_user_edit_1',
        ),
        'not' => FALSE,
      ),
    ),
    'logic' => 'and',
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'none',
    'title' => '',
    'name' => 'navigation',
    'weight' => '0',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'name' => 'navigation',
      'weight' => '0',
    ),
  );
  $page->arguments = array(
    'user' => array(
      'id' => 1,
      'identifier' => 'User edit form: User ID',
      'name' => 'user_edit',
      'settings' => array(),
    ),
  );
  $page->conf = array(
    'admin_paths' => FALSE,
  );
  $page->default_handlers = array();
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_member_edit__panel';
  $handler->task = 'page';
  $handler->subtask = 'member_edit';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'member edit',
    'no_blocks' => 0,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(
      0 => array(
        'identifier' => 'User edit form',
        'keyword' => 'user_edit',
        'name' => 'user_edit_form',
        'uid' => NULL,
        'id' => 1,
      ),
    ),
    'relationships' => array(),
    'name' => 'panel',
    'access' => array(
      'logic' => 'and',
      'plugins' => array(
        1 => array(
          'name' => 'perm',
          'settings' => array(
            'perm' => 'access content',
          ),
          'context' => 'logged-in-user',
          'not' => FALSE,
        ),
      ),
    ),
  );
  $display = new panels_display();
  $display->layout = 'onecol_row_stack';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'region_first' => NULL,
      'region_second' => NULL,
      'region_third' => NULL,
      'region_fourth' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '3b11a825-13be-41e5-8391-21d2669e87cc';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-efafe9c0-0f9b-4aa7-b716-1e28b187a165';
    $pane->panel = 'region_first';
    $pane->type = 'user_picture';
    $pane->subtype = 'user_picture';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_user_edit_1',
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
    $pane->uuid = 'efafe9c0-0f9b-4aa7-b716-1e28b187a165';
    $display->content['new-efafe9c0-0f9b-4aa7-b716-1e28b187a165'] = $pane;
    $display->panels['region_first'][0] = 'new-efafe9c0-0f9b-4aa7-b716-1e28b187a165';
    $pane = new stdClass();
    $pane->pid = 'new-6b0e304d-7d6c-4871-b881-bb0a44eac841';
    $pane->panel = 'region_second';
    $pane->type = 'form';
    $pane->subtype = 'form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_user_edit_1',
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
    $pane->uuid = '6b0e304d-7d6c-4871-b881-bb0a44eac841';
    $display->content['new-6b0e304d-7d6c-4871-b881-bb0a44eac841'] = $pane;
    $display->panels['region_second'][0] = 'new-6b0e304d-7d6c-4871-b881-bb0a44eac841';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['member_edit'] = $page;

  return $pages;

}
