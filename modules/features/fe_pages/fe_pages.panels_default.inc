<?php
/**
 * @file
 * fe_pages.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function fe_pages_default_panels_mini() {
  $export = array();

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'footer';
  $mini->category = '';
  $mini->admin_title = 'Footer';
  $mini->admin_description = '';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '0ad98262-c344-4d3b-86eb-e4d3a6317372';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-8eb3492c-869d-4e8e-b593-001a5d030364';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'menu-menu-footer-menu';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => '<none>',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '8eb3492c-869d-4e8e-b593-001a5d030364';
    $display->content['new-8eb3492c-869d-4e8e-b593-001a5d030364'] = $pane;
    $display->panels['middle'][0] = 'new-8eb3492c-869d-4e8e-b593-001a5d030364';
    $pane = new stdClass();
    $pane->pid = 'new-d1b112d1-1229-46e2-ada1-16d295d1aa6f';
    $pane->panel = 'middle';
    $pane->type = 'social_links';
    $pane->subtype = 'social_links';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'facebook' => 'https://www.facebook.com/',
      'twitter' => 'https://twitter.com/',
      'google' => 'http://www.google.com/',
      'linkedin' => 'https://www.linkedin.com/',
      'override_title' => 0,
      'override_title_text' => '',
      'company_logo' => 'http://www.linnovate.org.il/',
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
    $pane->uuid = 'd1b112d1-1229-46e2-ada1-16d295d1aa6f';
    $display->content['new-d1b112d1-1229-46e2-ada1-16d295d1aa6f'] = $pane;
    $display->panels['middle'][1] = 'new-d1b112d1-1229-46e2-ada1-16d295d1aa6f';
    $pane = new stdClass();
    $pane->pid = 'new-cbfa26b8-b80f-4a0b-a68c-de2ea53407ee';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'block-2';
    $pane->shown = TRUE;
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'cbfa26b8-b80f-4a0b-a68c-de2ea53407ee';
    $display->content['new-cbfa26b8-b80f-4a0b-a68c-de2ea53407ee'] = $pane;
    $display->panels['middle'][2] = 'new-cbfa26b8-b80f-4a0b-a68c-de2ea53407ee';
    $pane = new stdClass();
    $pane->pid = 'new-50f2e7f1-ed5a-4512-bd8a-4443d161bee5';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'ideal_main-openideal_credits';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => '<none>',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '50f2e7f1-ed5a-4512-bd8a-4443d161bee5';
    $display->content['new-50f2e7f1-ed5a-4512-bd8a-4443d161bee5'] = $pane;
    $display->panels['middle'][3] = 'new-50f2e7f1-ed5a-4512-bd8a-4443d161bee5';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $mini->display = $display;
  $export['footer'] = $mini;

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'header';
  $mini->category = '';
  $mini->admin_title = 'Header';
  $mini->admin_description = '';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'onecol';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'middle' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = 'b64635b2-5e1e-45ff-9bc0-f34fcb9be92b';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-b77e2ef2-3d24-4b09-8048-06a48d461bcc';
    $pane->panel = 'middle';
    $pane->type = 'node';
    $pane->subtype = 'node';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'nid' => '10',
      'links' => 1,
      'leave_node_title' => 0,
      'identifier' => '',
      'build_mode' => 'teaser',
      'link_node_title' => 0,
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
    $pane->uuid = 'b77e2ef2-3d24-4b09-8048-06a48d461bcc';
    $display->content['new-b77e2ef2-3d24-4b09-8048-06a48d461bcc'] = $pane;
    $display->panels['middle'][0] = 'new-b77e2ef2-3d24-4b09-8048-06a48d461bcc';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $mini->display = $display;
  $export['header'] = $mini;

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'latest_and_greatest';
  $mini->category = 'OpenideaL';
  $mini->admin_title = 'Latest and greatest';
  $mini->admin_description = 'A three column panel that displays the latest and greatest users and content';
  $mini->requiredcontexts = array();
  $mini->contexts = array();
  $mini->relationships = array();
  $display = new panels_display();
  $display->layout = 'bootstrap:latest_and_greatest';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'middle' => NULL,
      'right' => NULL,
      'bottom' => NULL,
      'center' => NULL,
      'column_2' => NULL,
      'column_3' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = 'Latest and greatest';
  $display->uuid = '5929fb7a-6a9b-4361-8d5e-18cfb841aee0';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-d8ada4da-6069-41ec-9cbf-2130ba128260';
    $pane->panel = 'center';
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
      'display' => 'block_1',
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
    $pane->uuid = 'd8ada4da-6069-41ec-9cbf-2130ba128260';
    $display->content['new-d8ada4da-6069-41ec-9cbf-2130ba128260'] = $pane;
    $display->panels['center'][0] = 'new-d8ada4da-6069-41ec-9cbf-2130ba128260';
    $pane = new stdClass();
    $pane->pid = 'new-dc82b6eb-65a3-49dc-89fe-6ab659340054';
    $pane->panel = 'column_2';
    $pane->type = 'block';
    $pane->subtype = 'views-activity-block_2';
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'dc82b6eb-65a3-49dc-89fe-6ab659340054';
    $display->content['new-dc82b6eb-65a3-49dc-89fe-6ab659340054'] = $pane;
    $display->panels['column_2'][0] = 'new-dc82b6eb-65a3-49dc-89fe-6ab659340054';
    $pane = new stdClass();
    $pane->pid = 'new-fc013ecc-af9a-4dab-a790-23ebd571c643';
    $pane->panel = 'column_3';
    $pane->type = 'block';
    $pane->subtype = 'views-activity-block_3';
    $pane->shown = FALSE;
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'fc013ecc-af9a-4dab-a790-23ebd571c643';
    $display->content['new-fc013ecc-af9a-4dab-a790-23ebd571c643'] = $pane;
    $display->panels['column_3'][0] = 'new-fc013ecc-af9a-4dab-a790-23ebd571c643';
    $pane = new stdClass();
    $pane->pid = 'new-710fa0e8-5c1f-40a6-a089-95ff93fbb581';
    $pane->panel = 'column_3';
    $pane->type = 'views';
    $pane->subtype = 'activity';
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
      'display' => 'block_4',
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
    $pane->uuid = '710fa0e8-5c1f-40a6-a089-95ff93fbb581';
    $display->content['new-710fa0e8-5c1f-40a6-a089-95ff93fbb581'] = $pane;
    $display->panels['column_3'][1] = 'new-710fa0e8-5c1f-40a6-a089-95ff93fbb581';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $mini->display = $display;
  $export['latest_and_greatest'] = $mini;

  return $export;
}
