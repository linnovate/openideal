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
  $page->menu = array();
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
  $display->layout = 'tow_col';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
      'region_first' => NULL,
      'region_second' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Solutions';
  $display->uuid = '17b296fb-32c9-443c-9756-128212b80a39';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-0ea15633-5909-4570-a40d-12ba9404429a';
    $pane->panel = 'region_first';
    $pane->type = 'block';
    $pane->subtype = 'quicktabs-all_ideas';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => 'solutions',
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
    $pane->uuid = '0ea15633-5909-4570-a40d-12ba9404429a';
    $display->content['new-0ea15633-5909-4570-a40d-12ba9404429a'] = $pane;
    $display->panels['region_first'][0] = 'new-0ea15633-5909-4570-a40d-12ba9404429a';
    $pane = new stdClass();
    $pane->pid = 'new-6f3c90ee-a549-4e0f-84b7-8311bf80d593';
    $pane->panel = 'region_second';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Add Idea',
      'title' => '',
      'body' => '<?php print \'<a href="node/add/idea" class="add-idea">\'.t(\'add idea\').\'</a>\' ?>',
      'format' => 'php_code',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'add-idea-btn',
    );
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '6f3c90ee-a549-4e0f-84b7-8311bf80d593';
    $display->content['new-6f3c90ee-a549-4e0f-84b7-8311bf80d593'] = $pane;
    $display->panels['region_second'][0] = 'new-6f3c90ee-a549-4e0f-84b7-8311bf80d593';
    $pane = new stdClass();
    $pane->pid = 'new-df42fec8-886c-4ba2-92a9-cfc515423f9d';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'df42fec8-886c-4ba2-92a9-cfc515423f9d';
    $display->content['new-df42fec8-886c-4ba2-92a9-cfc515423f9d'] = $pane;
    $display->panels['region_second'][1] = 'new-df42fec8-886c-4ba2-92a9-cfc515423f9d';
    $pane = new stdClass();
    $pane->pid = 'new-309e6623-ebd3-4631-b990-8ad132a5ca75';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '309e6623-ebd3-4631-b990-8ad132a5ca75';
    $display->content['new-309e6623-ebd3-4631-b990-8ad132a5ca75'] = $pane;
    $display->panels['region_second'][2] = 'new-309e6623-ebd3-4631-b990-8ad132a5ca75';
    $pane = new stdClass();
    $pane->pid = 'new-6344da81-eab8-49c0-a3f3-7d1f2a7ba3ad';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '6344da81-eab8-49c0-a3f3-7d1f2a7ba3ad';
    $display->content['new-6344da81-eab8-49c0-a3f3-7d1f2a7ba3ad'] = $pane;
    $display->panels['region_second'][3] = 'new-6344da81-eab8-49c0-a3f3-7d1f2a7ba3ad';
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
