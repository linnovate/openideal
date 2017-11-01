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
  $display->storage_type = 'panels_mini';
  $display->storage_id = 'footer';
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
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = '50f2e7f1-ed5a-4512-bd8a-4443d161bee5';
  $display->content['new-50f2e7f1-ed5a-4512-bd8a-4443d161bee5'] = $pane;
  $display->panels['middle'][2] = 'new-50f2e7f1-ed5a-4512-bd8a-4443d161bee5';
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
  $display->storage_type = 'panels_mini';
  $display->storage_id = 'header';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-6897968c-a832-405f-9590-53e67307012a';
  $pane->panel = 'middle';
  $pane->type = 'page_logo';
  $pane->subtype = 'page_logo';
  $pane->shown = TRUE;
  $pane->access = array();
  $pane->configuration = array();
  $pane->cache = array();
  $pane->style = array(
    'settings' => NULL,
  );
  $pane->css = array(
    'css_id' => '',
    'css_class' => 'hidden-xs',
  );
  $pane->extras = array();
  $pane->position = 0;
  $pane->locks = array();
  $pane->uuid = '6897968c-a832-405f-9590-53e67307012a';
  $display->content['new-6897968c-a832-405f-9590-53e67307012a'] = $pane;
  $display->panels['middle'][0] = 'new-6897968c-a832-405f-9590-53e67307012a';
  $pane = new stdClass();
  $pane->pid = 'new-62127dde-96a3-4ab9-bf4c-c6cc6d296c53';
  $pane->panel = 'middle';
  $pane->type = 'block';
  $pane->subtype = 'system-main-menu';
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
  $pane->position = 1;
  $pane->locks = array();
  $pane->uuid = '62127dde-96a3-4ab9-bf4c-c6cc6d296c53';
  $display->content['new-62127dde-96a3-4ab9-bf4c-c6cc6d296c53'] = $pane;
  $display->panels['middle'][1] = 'new-62127dde-96a3-4ab9-bf4c-c6cc6d296c53';
  $pane = new stdClass();
  $pane->pid = 'new-c950dc0d-517e-4510-b37e-664f00618a37';
  $pane->panel = 'middle';
  $pane->type = 'block';
  $pane->subtype = 'search-form';
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
  $pane->css = array(
    'css_id' => '',
    'css_class' => 'visible-lg',
  );
  $pane->extras = array();
  $pane->position = 2;
  $pane->locks = array();
  $pane->uuid = 'c950dc0d-517e-4510-b37e-664f00618a37';
  $display->content['new-c950dc0d-517e-4510-b37e-664f00618a37'] = $pane;
  $display->panels['middle'][2] = 'new-c950dc0d-517e-4510-b37e-664f00618a37';
  $pane = new stdClass();
  $pane->pid = 'new-7a282cec-2c34-4c8f-a635-d37f47eec066';
  $pane->panel = 'middle';
  $pane->type = 'block';
  $pane->subtype = 'top_searches-0';
  $pane->shown = FALSE;
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
  $pane->uuid = '7a282cec-2c34-4c8f-a635-d37f47eec066';
  $display->content['new-7a282cec-2c34-4c8f-a635-d37f47eec066'] = $pane;
  $display->panels['middle'][3] = 'new-7a282cec-2c34-4c8f-a635-d37f47eec066';
  $pane = new stdClass();
  $pane->pid = 'new-aba73889-e19d-47b5-92ab-7222bc9b95dd';
  $pane->panel = 'middle';
  $pane->type = 'block';
  $pane->subtype = 'system-user-menu';
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
  $pane->position = 4;
  $pane->locks = array();
  $pane->uuid = 'aba73889-e19d-47b5-92ab-7222bc9b95dd';
  $display->content['new-aba73889-e19d-47b5-92ab-7222bc9b95dd'] = $pane;
  $display->panels['middle'][4] = 'new-aba73889-e19d-47b5-92ab-7222bc9b95dd';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = 'new-6897968c-a832-405f-9590-53e67307012a';
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
  $display->storage_type = 'panels_mini';
  $display->storage_id = 'latest_and_greatest';
  $display->content = array();
  $display->panels = array();
  $pane = new stdClass();
  $pane->pid = 'new-c15b9041-9c82-4c78-a457-165f2e1c2d1f';
  $pane->panel = 'center';
  $pane->type = 'block';
  $pane->subtype = 'views-activity-block_1';
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
  $pane->uuid = 'c15b9041-9c82-4c78-a457-165f2e1c2d1f';
  $display->content['new-c15b9041-9c82-4c78-a457-165f2e1c2d1f'] = $pane;
  $display->panels['center'][0] = 'new-c15b9041-9c82-4c78-a457-165f2e1c2d1f';
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
  $pane->uuid = 'fc013ecc-af9a-4dab-a790-23ebd571c643';
  $display->content['new-fc013ecc-af9a-4dab-a790-23ebd571c643'] = $pane;
  $display->panels['column_3'][0] = 'new-fc013ecc-af9a-4dab-a790-23ebd571c643';
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $mini->display = $display;
  $export['latest_and_greatest'] = $mini;

  return $export;
}
