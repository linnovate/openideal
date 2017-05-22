<?php
/**
 * @file
 * fe_sidebar_first.panels_default.inc
 */

/**
 * Implements hook_default_panels_mini().
 */
function fe_sidebar_first_default_panels_mini() {
  $export = array();

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'main_memu';
  $mini->category = '';
  $mini->admin_title = 'Main memu';
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
  $display->uuid = '4cc24f8f-b481-4266-aee7-2bcad1c73428';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-2c0f286d-6f3f-4315-aa2f-0313387a4cdb';
    $pane->panel = 'middle';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'top menu',
      'title' => '',
      'body' => '<div class ="header_menu">
<a href="/"> 
<div class = "header_name">Openidea</div>
  <div class ="sub_name">Powered by Linnovate</div>
<span class="home-icon">icon-hom</span>
</a>

<div/>',
      'format' => 'full_html',
      'substitute' => TRUE,
      'title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '2c0f286d-6f3f-4315-aa2f-0313387a4cdb';
    $display->content['new-2c0f286d-6f3f-4315-aa2f-0313387a4cdb'] = $pane;
    $display->panels['middle'][0] = 'new-2c0f286d-6f3f-4315-aa2f-0313387a4cdb';
    $pane = new stdClass();
    $pane->pid = 'new-e1bfef45-823a-4409-8365-a9fb336a71dc';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'system-user-menu';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'e1bfef45-823a-4409-8365-a9fb336a71dc';
    $display->content['new-e1bfef45-823a-4409-8365-a9fb336a71dc'] = $pane;
    $display->panels['middle'][1] = 'new-e1bfef45-823a-4409-8365-a9fb336a71dc';
    $pane = new stdClass();
    $pane->pid = 'new-e42912ff-f815-4637-8ce6-6e393c7989c6';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'system-main-menu';
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
    $pane->uuid = 'e42912ff-f815-4637-8ce6-6e393c7989c6';
    $display->content['new-e42912ff-f815-4637-8ce6-6e393c7989c6'] = $pane;
    $display->panels['middle'][2] = 'new-e42912ff-f815-4637-8ce6-6e393c7989c6';
    $pane = new stdClass();
    $pane->pid = 'new-6e476430-912e-4478-8e48-fc67855c54f4';
    $pane->panel = 'middle';
    $pane->type = 'block';
    $pane->subtype = 'search-form';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '6e476430-912e-4478-8e48-fc67855c54f4';
    $display->content['new-6e476430-912e-4478-8e48-fc67855c54f4'] = $pane;
    $display->panels['middle'][3] = 'new-6e476430-912e-4478-8e48-fc67855c54f4';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = 'new-e42912ff-f815-4637-8ce6-6e393c7989c6';
  $mini->display = $display;
  $export['main_memu'] = $mini;

  $mini = new stdClass();
  $mini->disabled = FALSE; /* Edit this to true to make a default mini disabled initially */
  $mini->api_version = 1;
  $mini->name = 'sidebar_first';
  $mini->category = '';
  $mini->admin_title = 'sidebar first';
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
  $display->uuid = 'ef956122-ad00-4ef6-97b2-abb2b5b6d136';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-e909649d-275b-4487-b38c-3f21c82adcc7';
    $pane->panel = 'middle';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'openAndClose',
      'title' => '',
      'body' => '<div class="close-menu"> x </div>
<div class="open-menu"> 0 </div>',
      'format' => 'full_html',
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
    $pane->uuid = 'e909649d-275b-4487-b38c-3f21c82adcc7';
    $display->content['new-e909649d-275b-4487-b38c-3f21c82adcc7'] = $pane;
    $display->panels['middle'][0] = 'new-e909649d-275b-4487-b38c-3f21c82adcc7';
    $pane = new stdClass();
    $pane->pid = 'new-32af9d41-8bb0-490f-a8ba-210483997331';
    $pane->panel = 'middle';
    $pane->type = 'panels_mini';
    $pane->subtype = 'main_memu';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '32af9d41-8bb0-490f-a8ba-210483997331';
    $display->content['new-32af9d41-8bb0-490f-a8ba-210483997331'] = $pane;
    $display->panels['middle'][1] = 'new-32af9d41-8bb0-490f-a8ba-210483997331';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $mini->display = $display;
  $export['sidebar_first'] = $mini;

  return $export;
}
