<?php
/**
 * @file
 * fe_idea.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_pages().
 */
function fe_idea_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'ideas';
  $page->task = 'page';
  $page->admin_title = 'ideas';
  $page->admin_description = '';
  $page->path = 'ideas';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'normal',
    'title' => 'Ideas',
    'name' => 'main-menu',
    'weight' => '-50',
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
  $handler->name = 'page_ideas_panel_context_3';
  $handler->task = 'page';
  $handler->subtask = 'ideas';
  $handler->handler = 'panel_context';
  $handler->weight = -30;
  $handler->conf = array(
    'title' => 'Ideas',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'bootstrap:ideal_two_column';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '17b296fb-32c9-443c-9756-128212b80a39';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_ideas_panel_context_3';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-0ea15633-5909-4570-a40d-12ba9404429a';
  $pane->panel = 'center';
  $pane->type = 'block';
  $pane->subtype = 'quicktabs-all_ideas';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'override_title_text' => 'Ideas',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '0ea15633-5909-4570-a40d-12ba9404429a';
  $display->content['new-0ea15633-5909-4570-a40d-12ba9404429a'] = $pane;
  $display->panels['center'][0] = 'new-0ea15633-5909-4570-a40d-12ba9404429a';
  $pane = new stdClass();
  $pane->pid = 'new-6f3c90ee-a549-4e0f-84b7-8311bf80d593';
  $pane->panel = 'sidebar';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'admin_title' => 'Add Idea',
    'title' => '',
    'body' => '<a href="node/add/idea" class="add-idea">Add Idea</a>',
    'format' => 'filtered_html',
    'substitute' => TRUE,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '6f3c90ee-a549-4e0f-84b7-8311bf80d593';
  $display->content['new-6f3c90ee-a549-4e0f-84b7-8311bf80d593'] = $pane;
  $display->panels['sidebar'][0] = 'new-6f3c90ee-a549-4e0f-84b7-8311bf80d593';
  $pane = new stdClass();
  $pane->pid = 'new-0bee39e0-9b1a-409b-ab20-33854b8cc1fa';
  $pane->panel = 'sidebar';
  $pane->type = 'block';
  $pane->subtype = 'views-f82b385d04a27538a1646ec3e53c5fef';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 1,
    'override_title_text' => 'Featured Challenges',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '0bee39e0-9b1a-409b-ab20-33854b8cc1fa';
  $display->content['new-0bee39e0-9b1a-409b-ab20-33854b8cc1fa'] = $pane;
  $display->panels['sidebar'][1] = 'new-0bee39e0-9b1a-409b-ab20-33854b8cc1fa';
  $pane = new stdClass();
  $pane->pid = 'new-12177c78-e00b-461c-854b-8804f25b611d';
  $pane->panel = 'sidebar';
  $pane->type = 'block';
  $pane->subtype = 'views-activity-block';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => '',
    'override_title_heading' => 'h2',
    'title_class' => 'bg-primary',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '12177c78-e00b-461c-854b-8804f25b611d';
  $display->content['new-12177c78-e00b-461c-854b-8804f25b611d'] = $pane;
  $display->panels['sidebar'][2] = 'new-12177c78-e00b-461c-854b-8804f25b611d';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'page_ideas_panel_context';
  $handler->task = 'page';
  $handler->subtask = 'ideas';
  $handler->handler = 'panel_context';
  $handler->weight = -28;
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
  );
  $display = new panels_display();
  $display->layout = 'bootstrap:ideal_two_column';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '2394d056-104f-4271-b394-bd1f47fa9e67';
  $display->storage_type = 'page_manager';
  $display->storage_id = 'page_ideas_panel_context';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-60d926ab-d1b7-4aca-873b-5de35f25f063';
  $pane->panel = 'sidebar';
  $pane->type = 'custom';
  $pane->subtype = 'custom';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'admin_title' => 'Add Idea',
    'title' => '',
    'body' => '<a href="node/add/idea" class="add-idea">Add Idea</a>',
    'format' => 'filtered_html',
    'substitute' => TRUE,
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '60d926ab-d1b7-4aca-873b-5de35f25f063';
  $display->content['new-60d926ab-d1b7-4aca-873b-5de35f25f063'] = $pane;
  $display->panels['sidebar'][0] = 'new-60d926ab-d1b7-4aca-873b-5de35f25f063';
  $pane = new stdClass();
  $pane->pid = 'new-a8ddfb66-04fd-4ef2-957f-ca95d1991bdc';
  $pane->panel = 'sidebar';
  $pane->type = 'block';
  $pane->subtype = 'views-activity-block';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array(
    'override_title' => 0,
    'override_title_text' => '',
  );
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array();
  $pane->extras = array();
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = 'a8ddfb66-04fd-4ef2-957f-ca95d1991bdc';
  $display->content['new-a8ddfb66-04fd-4ef2-957f-ca95d1991bdc'] = $pane;
  $display->panels['sidebar'][1] = 'new-a8ddfb66-04fd-4ef2-957f-ca95d1991bdc';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['ideas'] = $page;

  return $pages;

}
