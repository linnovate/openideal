<?php
/**
 * @file
 * fe_pages.pages_default.inc
 */

/**
 * Implements hook_default_page_manager_handlers().
 */
function fe_pages_default_page_manager_handlers() {
  $export = array();

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_2';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 1;
  $handler->conf = array(
    'title' => 'Idea',
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
              'idea' => 'idea',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'bootstrap:inner_page';
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
    $pane->pid = 'new-a52139c7-b3b5-41b3-90aa-9a8fcc853b83';
    $pane->panel = 'center';
    $pane->type = 'node_title';
    $pane->subtype = 'node_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'link' => 0,
      'markup' => 'h3',
      'id' => '',
      'class' => '',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = 'a52139c7-b3b5-41b3-90aa-9a8fcc853b83';
    $display->content['new-a52139c7-b3b5-41b3-90aa-9a8fcc853b83'] = $pane;
    $display->panels['center'][0] = 'new-a52139c7-b3b5-41b3-90aa-9a8fcc853b83';
    $pane = new stdClass();
    $pane->pid = 'new-55a50e1c-e7bf-4af3-a0be-e61456c7de98';
    $pane->panel = 'center';
    $pane->type = 'user_picture';
    $pane->subtype = 'user_picture';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'relationship_entity_from_schema:uid-node-user_1',
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
    $pane->uuid = '55a50e1c-e7bf-4af3-a0be-e61456c7de98';
    $display->content['new-55a50e1c-e7bf-4af3-a0be-e61456c7de98'] = $pane;
    $display->panels['center'][1] = 'new-55a50e1c-e7bf-4af3-a0be-e61456c7de98';
    $pane = new stdClass();
    $pane->pid = 'new-5c79888d-ad6b-4d99-bbb6-ff22848cc44a';
    $pane->panel = 'center';
    $pane->type = 'node_author';
    $pane->subtype = 'node_author';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'link' => 1,
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '5c79888d-ad6b-4d99-bbb6-ff22848cc44a';
    $display->content['new-5c79888d-ad6b-4d99-bbb6-ff22848cc44a'] = $pane;
    $display->panels['center'][2] = 'new-5c79888d-ad6b-4d99-bbb6-ff22848cc44a';
    $pane = new stdClass();
    $pane->pid = 'new-55a68bec-4261-4f51-bc32-cae72d54754b';
    $pane->panel = 'center';
    $pane->type = 'node_created';
    $pane->subtype = 'node_created';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'format' => 'long',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '55a68bec-4261-4f51-bc32-cae72d54754b';
    $display->content['new-55a68bec-4261-4f51-bc32-cae72d54754b'] = $pane;
    $display->panels['center'][3] = 'new-55a68bec-4261-4f51-bc32-cae72d54754b';
    $pane = new stdClass();
    $pane->pid = 'new-1bdd70ca-165f-4972-b10a-f8f7965427eb';
    $pane->panel = 'center';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_share';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'social_buttons_formatter_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'relationship_entity_from_field:field_challenge_ref-node-node_1',
      'override_title' => 1,
      'override_title_text' => 'Share',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '1bdd70ca-165f-4972-b10a-f8f7965427eb';
    $display->content['new-1bdd70ca-165f-4972-b10a-f8f7965427eb'] = $pane;
    $display->panels['center'][4] = 'new-1bdd70ca-165f-4972-b10a-f8f7965427eb';
    $pane = new stdClass();
    $pane->pid = 'new-3f83a2fb-a59b-4b90-9308-85791c77ec84';
    $pane->panel = 'center';
    $pane->type = 'token';
    $pane->subtype = 'node:field-challenge-ref';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'sanitize' => 1,
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => 'Challenge:',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = '3f83a2fb-a59b-4b90-9308-85791c77ec84';
    $display->content['new-3f83a2fb-a59b-4b90-9308-85791c77ec84'] = $pane;
    $display->panels['center'][5] = 'new-3f83a2fb-a59b-4b90-9308-85791c77ec84';
    $pane = new stdClass();
    $pane->pid = 'new-4e2a2eee-be21-4c7d-8651-e04d528458ca';
    $pane->panel = 'center';
    $pane->type = 'node_body';
    $pane->subtype = 'node_body';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => '<none>',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = '4e2a2eee-be21-4c7d-8651-e04d528458ca';
    $display->content['new-4e2a2eee-be21-4c7d-8651-e04d528458ca'] = $pane;
    $display->panels['center'][6] = 'new-4e2a2eee-be21-4c7d-8651-e04d528458ca';
    $pane = new stdClass();
    $pane->pid = 'new-7600b9f0-d6b7-4eec-b76f-ea0263ea5c98';
    $pane->panel = 'center';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_idea_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'image',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'image_style' => 'cropped_220_220',
        'image_link' => '',
        'slideshow_image_style' => 'cropped_220_220',
        'slideshow_link' => '',
        'slideshow_caption' => '',
        'slideshow_caption_link' => '',
        'slideshow_fx' => 'scrollLeft',
        'slideshow_speed' => '1000',
        'slideshow_timeout' => '4000',
        'slideshow_order' => '',
        'slideshow_controls' => 0,
        'slideshow_controls_pause' => 0,
        'slideshow_controls_position' => 'after',
        'slideshow_pause' => 0,
        'slideshow_start_on_hover' => 0,
        'slideshow_pager' => '',
        'slideshow_pager_position' => 'after',
        'slideshow_pager_image_style' => '',
        'slideshow_carousel_image_style' => '',
        'slideshow_carousel_visible' => '3',
        'slideshow_carousel_scroll' => '1',
        'slideshow_carousel_speed' => '500',
        'slideshow_carousel_vertical' => 0,
        'slideshow_carousel_circular' => 0,
        'slideshow_carousel_follow' => 0,
        'slideshow_carousel_skin' => '',
        'slide_style' => '0',
        'thumb_style' => '0',
        'style' => 'Greenarrows',
        'linking_method' => 'onclick_full',
        'link_to_full' => 0,
        'link_to_full_style' => 'cropped_220_220',
        'modal' => 'none',
        'singleItem' => 1,
        'items' => '',
        'navigation' => 0,
        'pagination' => 1,
        'scrollPerPage' => 0,
        'lazyLoad' => 0,
        'slideSpeed' => '300',
        'paginationSpeed' => '800',
        'stopOnHover' => 1,
        'rewindNav' => 1,
        'paginationNumbers' => 0,
        'responsiveRefreshRate' => '200',
        'theme' => 'owl-theme',
        'lazyFollow' => 1,
        'lazyEffect' => 'fade',
        'autoHeight' => 0,
        'jsonPath' => '',
        'jsonSuccess' => '',
        'dragBeforeAnimFinish' => 1,
        'mouseDrag' => 1,
        'touchDrag' => 1,
        'addClassActive' => 0,
        'transitionStyle' => '',
        'breakpointDesktop' => '1199',
        'breakpointDesktopSmall' => '973',
        'breakpointTablet' => '840',
        'breakpointTabletSmall' => '768',
        'breakpointMobile' => '479',
        'itemsDesktop' => '4',
        'itemsDesktopSmall' => '3',
        'itemsTablet' => '2',
        'itemsTabletSmall' => '2',
        'itemsMobile' => '1',
      ),
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => 'Images',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 7;
    $pane->locks = array();
    $pane->uuid = '7600b9f0-d6b7-4eec-b76f-ea0263ea5c98';
    $display->content['new-7600b9f0-d6b7-4eec-b76f-ea0263ea5c98'] = $pane;
    $display->panels['center'][7] = 'new-7600b9f0-d6b7-4eec-b76f-ea0263ea5c98';
    $pane = new stdClass();
    $pane->pid = 'new-c3c069da-e134-488b-b601-313937b4fa16';
    $pane->panel = 'content_bottom';
    $pane->type = 'node_comment_form';
    $pane->subtype = 'node_comment_form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'anon_links' => 0,
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => 'Comments',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'c3c069da-e134-488b-b601-313937b4fa16';
    $display->content['new-c3c069da-e134-488b-b601-313937b4fa16'] = $pane;
    $display->panels['content_bottom'][0] = 'new-c3c069da-e134-488b-b601-313937b4fa16';
    $pane = new stdClass();
    $pane->pid = 'new-c53b8dc1-2d06-4bf8-8ca5-ce4cb2e9cd3e';
    $pane->panel = 'content_bottom';
    $pane->type = 'node_comments';
    $pane->subtype = 'node_comments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'mode' => '1',
      'comments_per_page' => '50',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = 'c53b8dc1-2d06-4bf8-8ca5-ce4cb2e9cd3e';
    $display->content['new-c53b8dc1-2d06-4bf8-8ca5-ce4cb2e9cd3e'] = $pane;
    $display->panels['content_bottom'][1] = 'new-c53b8dc1-2d06-4bf8-8ca5-ce4cb2e9cd3e';
    $pane = new stdClass();
    $pane->pid = 'new-54cb9361-7019-4743-86c1-7ecfe4d3fc00';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_vote';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '54cb9361-7019-4743-86c1-7ecfe4d3fc00';
    $display->content['new-54cb9361-7019-4743-86c1-7ecfe4d3fc00'] = $pane;
    $display->panels['sidebar'][0] = 'new-54cb9361-7019-4743-86c1-7ecfe4d3fc00';
    $pane = new stdClass();
    $pane->pid = 'new-6d39ddb3-b92f-4ec9-bc58-b86c9b3383e3';
    $pane->panel = 'sidebar';
    $pane->type = 'block';
    $pane->subtype = 'ideal_blocks-idea_stats';
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
    $pane->uuid = '6d39ddb3-b92f-4ec9-bc58-b86c9b3383e3';
    $display->content['new-6d39ddb3-b92f-4ec9-bc58-b86c9b3383e3'] = $pane;
    $display->panels['sidebar'][1] = 'new-6d39ddb3-b92f-4ec9-bc58-b86c9b3383e3';
    $pane = new stdClass();
    $pane->pid = 'new-6027d044-d438-4784-81c3-6cbf7b2ea4e7';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_attachments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'file_default',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '6027d044-d438-4784-81c3-6cbf7b2ea4e7';
    $display->content['new-6027d044-d438-4784-81c3-6cbf7b2ea4e7'] = $pane;
    $display->panels['sidebar'][2] = 'new-6027d044-d438-4784-81c3-6cbf7b2ea4e7';
    $pane = new stdClass();
    $pane->pid = 'new-9f2feceb-597d-44c7-92ad-09953f3c6796';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '9f2feceb-597d-44c7-92ad-09953f3c6796';
    $display->content['new-9f2feceb-597d-44c7-92ad-09953f3c6796'] = $pane;
    $display->panels['sidebar'][3] = 'new-9f2feceb-597d-44c7-92ad-09953f3c6796';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_2'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'node_view_panel_context_3';
  $handler->task = 'node_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 2;
  $handler->conf = array(
    'title' => 'Challenge panel',
    'no_blocks' => 1,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(
      0 => array(
        'identifier' => 'User from Node (on Node: Moderator [field_moderator])',
        'keyword' => 'user',
        'name' => 'entity_from_field:field_moderator-node-user',
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
              'challenge' => 'challenge',
            ),
          ),
          'context' => 'argument_entity_id:node_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'bootstrap:inner_page';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
      'content_top' => NULL,
      'content_bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '0b009613-3d6b-4e3e-a0c5-ce58071215df';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-4c27aa01-a434-44ee-9ab0-6ab017a1c0b3';
    $pane->panel = 'center';
    $pane->type = 'node_title';
    $pane->subtype = 'node_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'link' => 0,
      'markup' => 'h3',
      'id' => '',
      'class' => '',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '4c27aa01-a434-44ee-9ab0-6ab017a1c0b3';
    $display->content['new-4c27aa01-a434-44ee-9ab0-6ab017a1c0b3'] = $pane;
    $display->panels['center'][0] = 'new-4c27aa01-a434-44ee-9ab0-6ab017a1c0b3';
    $pane = new stdClass();
    $pane->pid = 'new-2a5e72bf-9f6a-4797-8099-0b0bb3e72c5d';
    $pane->panel = 'center';
    $pane->type = 'user_picture';
    $pane->subtype = 'user_picture';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'context' => 'relationship_entity_from_field:field_moderator-node-user_1',
      'override_title' => 1,
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
    $pane->uuid = '2a5e72bf-9f6a-4797-8099-0b0bb3e72c5d';
    $display->content['new-2a5e72bf-9f6a-4797-8099-0b0bb3e72c5d'] = $pane;
    $display->panels['center'][1] = 'new-2a5e72bf-9f6a-4797-8099-0b0bb3e72c5d';
    $pane = new stdClass();
    $pane->pid = 'new-e6961a63-74db-40cb-9458-bb745641e83f';
    $pane->panel = 'center';
    $pane->type = 'node_author';
    $pane->subtype = 'node_author';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'link' => 1,
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = 'e6961a63-74db-40cb-9458-bb745641e83f';
    $display->content['new-e6961a63-74db-40cb-9458-bb745641e83f'] = $pane;
    $display->panels['center'][2] = 'new-e6961a63-74db-40cb-9458-bb745641e83f';
    $pane = new stdClass();
    $pane->pid = 'new-04ae6131-4339-47c6-941f-5005b758f64e';
    $pane->panel = 'center';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_status';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'taxonomy_term_reference_plain',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '04ae6131-4339-47c6-941f-5005b758f64e';
    $display->content['new-04ae6131-4339-47c6-941f-5005b758f64e'] = $pane;
    $display->panels['center'][3] = 'new-04ae6131-4339-47c6-941f-5005b758f64e';
    $pane = new stdClass();
    $pane->pid = 'new-c890ebbb-f3cc-4fbb-859a-c874551eb570';
    $pane->panel = 'center';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_share';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'social_buttons_formatter_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => 'Share',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = 'c890ebbb-f3cc-4fbb-859a-c874551eb570';
    $display->content['new-c890ebbb-f3cc-4fbb-859a-c874551eb570'] = $pane;
    $display->panels['center'][4] = 'new-c890ebbb-f3cc-4fbb-859a-c874551eb570';
    $pane = new stdClass();
    $pane->pid = 'new-a3375e66-341e-404c-bfbd-1a76fa8c16f5';
    $pane->panel = 'center';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_challenge_image';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'image',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'image_style' => '',
        'image_link' => '',
      ),
      'context' => 'argument_entity_id:node_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = 'a3375e66-341e-404c-bfbd-1a76fa8c16f5';
    $display->content['new-a3375e66-341e-404c-bfbd-1a76fa8c16f5'] = $pane;
    $display->panels['center'][5] = 'new-a3375e66-341e-404c-bfbd-1a76fa8c16f5';
    $pane = new stdClass();
    $pane->pid = 'new-14647954-c43b-42dd-bff0-71841bdaa16c';
    $pane->panel = 'center';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:body';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'hidden',
      'formatter' => 'text_default',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
      'override_title' => 0,
      'override_title_text' => '',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = '14647954-c43b-42dd-bff0-71841bdaa16c';
    $display->content['new-14647954-c43b-42dd-bff0-71841bdaa16c'] = $pane;
    $display->panels['center'][6] = 'new-14647954-c43b-42dd-bff0-71841bdaa16c';
    $pane = new stdClass();
    $pane->pid = 'new-425fd810-f0b7-4432-a9b9-0745a833be6f';
    $pane->panel = 'content_bottom';
    $pane->type = 'block';
    $pane->subtype = 'views-ideas_in_challenge-block';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_title' => 1,
      'override_title_text' => 'Browse existing ideas',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '425fd810-f0b7-4432-a9b9-0745a833be6f';
    $display->content['new-425fd810-f0b7-4432-a9b9-0745a833be6f'] = $pane;
    $display->panels['content_bottom'][0] = 'new-425fd810-f0b7-4432-a9b9-0745a833be6f';
    $pane = new stdClass();
    $pane->pid = 'new-4e365586-d194-4fa5-b6bb-ed61a292509d';
    $pane->panel = 'content_bottom';
    $pane->type = 'block';
    $pane->subtype = 'ideal_challenge-challenge_add_idea_button';
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
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'pull-right col-md-4 hidden-xs hidden-sm',
    );
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '4e365586-d194-4fa5-b6bb-ed61a292509d';
    $display->content['new-4e365586-d194-4fa5-b6bb-ed61a292509d'] = $pane;
    $display->panels['content_bottom'][1] = 'new-4e365586-d194-4fa5-b6bb-ed61a292509d';
    $pane = new stdClass();
    $pane->pid = 'new-f02c3dfe-eb90-454d-a0b4-6a7fbddf91b7';
    $pane->panel = 'content_bottom';
    $pane->type = 'node_comment_form';
    $pane->subtype = 'node_comment_form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'anon_links' => 1,
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => 'Comments',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'f02c3dfe-eb90-454d-a0b4-6a7fbddf91b7';
    $display->content['new-f02c3dfe-eb90-454d-a0b4-6a7fbddf91b7'] = $pane;
    $display->panels['content_bottom'][2] = 'new-f02c3dfe-eb90-454d-a0b4-6a7fbddf91b7';
    $pane = new stdClass();
    $pane->pid = 'new-8f4b7849-f690-4218-9b3c-1112b506fe84';
    $pane->panel = 'content_bottom';
    $pane->type = 'node_comments';
    $pane->subtype = 'node_comments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'mode' => '1',
      'comments_per_page' => '50',
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '8f4b7849-f690-4218-9b3c-1112b506fe84';
    $display->content['new-8f4b7849-f690-4218-9b3c-1112b506fe84'] = $pane;
    $display->panels['content_bottom'][3] = 'new-8f4b7849-f690-4218-9b3c-1112b506fe84';
    $pane = new stdClass();
    $pane->pid = 'new-427bc497-0cc1-4461-ad4f-44bb8bd91606';
    $pane->panel = 'sidebar';
    $pane->type = 'block';
    $pane->subtype = 'ideal_challenge-challenge_add_idea_button';
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
    $pane->uuid = '427bc497-0cc1-4461-ad4f-44bb8bd91606';
    $display->content['new-427bc497-0cc1-4461-ad4f-44bb8bd91606'] = $pane;
    $display->panels['sidebar'][0] = 'new-427bc497-0cc1-4461-ad4f-44bb8bd91606';
    $pane = new stdClass();
    $pane->pid = 'new-9eff432b-ab70-4a0b-b3dc-a79236f2e0a6';
    $pane->panel = 'sidebar';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_attachments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'file_default',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(),
      'context' => 'argument_entity_id:node_1',
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
    $pane->uuid = '9eff432b-ab70-4a0b-b3dc-a79236f2e0a6';
    $display->content['new-9eff432b-ab70-4a0b-b3dc-a79236f2e0a6'] = $pane;
    $display->panels['sidebar'][1] = 'new-9eff432b-ab70-4a0b-b3dc-a79236f2e0a6';
    $pane = new stdClass();
    $pane->pid = 'new-2da010b3-8380-4e19-b366-4d94eb6f5721';
    $pane->panel = 'sidebar';
    $pane->type = 'block';
    $pane->subtype = 'ideal_challenge-challenge_data';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '2da010b3-8380-4e19-b366-4d94eb6f5721';
    $display->content['new-2da010b3-8380-4e19-b366-4d94eb6f5721'] = $pane;
    $display->panels['sidebar'][2] = 'new-2da010b3-8380-4e19-b366-4d94eb6f5721';
    $pane = new stdClass();
    $pane->pid = 'new-cbc55058-13c9-4932-a80a-4297ddeadfe2';
    $pane->panel = 'sidebar';
    $pane->type = 'block';
    $pane->subtype = 'ideal_blocks-challenge_stats';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'cbc55058-13c9-4932-a80a-4297ddeadfe2';
    $display->content['new-cbc55058-13c9-4932-a80a-4297ddeadfe2'] = $pane;
    $display->panels['sidebar'][3] = 'new-cbc55058-13c9-4932-a80a-4297ddeadfe2';
    $pane = new stdClass();
    $pane->pid = 'new-0c1c2405-34ab-46a2-8916-4c13a2610b8f';
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '0c1c2405-34ab-46a2-8916-4c13a2610b8f';
    $display->content['new-0c1c2405-34ab-46a2-8916-4c13a2610b8f'] = $pane;
    $display->panels['sidebar'][4] = 'new-0c1c2405-34ab-46a2-8916-4c13a2610b8f';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['node_view_panel_context_3'] = $handler;

  $handler = new stdClass();
  $handler->disabled = FALSE; /* Edit this to true to make a default handler disabled initially */
  $handler->api_version = 1;
  $handler->name = 'term_view_panel_context';
  $handler->task = 'term_view';
  $handler->subtask = '';
  $handler->handler = 'panel_context';
  $handler->weight = 0;
  $handler->conf = array(
    'title' => 'Category',
    'no_blocks' => 0,
    'pipeline' => 'standard',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
    'access' => array(
      'plugins' => array(
        0 => array(
          'name' => 'term_vocabulary',
          'settings' => array(
            'vids' => array(
              3 => '3',
            ),
          ),
          'context' => 'argument_term_1',
          'not' => FALSE,
        ),
      ),
      'logic' => 'and',
    ),
  );
  $display = new panels_display();
  $display->layout = 'threecol_25_50_25_stacked';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'top' => NULL,
      'left' => NULL,
      'middle' => NULL,
      'right' => NULL,
      'bottom' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '78f6d889-3369-4155-8639-311509023340';
  $display->content = array();
  $display->panels = array();
  $display->hide_title = PANELS_TITLE_FIXED;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $export['term_view_panel_context'] = $handler;

  return $export;
}

/**
 * Implements hook_default_page_manager_pages().
 */
function fe_pages_default_page_manager_pages() {
  $page = new stdClass();
  $page->disabled = FALSE; /* Edit this to true to make a default page disabled initially */
  $page->api_version = 1;
  $page->name = 'home';
  $page->task = 'page';
  $page->admin_title = 'Home';
  $page->admin_description = '';
  $page->path = 'front';
  $page->access = array(
    'type' => 'none',
    'settings' => NULL,
  );
  $page->menu = array(
    'type' => 'none',
    'title' => '',
    'weight' => '0',
    'name' => 'navigation',
    'parent' => array(
      'type' => 'none',
      'title' => '',
      'weight' => 0,
      'name' => 'navigation',
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
  $handler->name = 'page_home__panel_context_2139fcd2-1e19-4e44-a238-8a837a80c585';
  $handler->task = 'page';
  $handler->subtask = 'home';
  $handler->handler = 'panel_context';
  $handler->weight = 6;
  $handler->conf = array(
    'title' => 'Landing page',
    'no_blocks' => 1,
    'pipeline' => 'ipe',
    'body_classes_to_remove' => '',
    'body_classes_to_add' => '',
    'css_id' => '',
    'css' => '',
    'contexts' => array(),
    'relationships' => array(),
  );
  $display = new panels_display();
  $display->layout = 'bootstrap:home';
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
      'content_views' => NULL,
      'sidebar' => NULL,
    ),
    'sidebar' => array(
      'style' => '-1',
    ),
    'content_views' => array(
      'style' => '-1',
    ),
  );
  $display->cache = array();
  $display->title = 'Home';
  $display->uuid = 'cc820d35-2db9-4adf-8096-43d88a9cb6c7';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-7f733a13-5fda-40aa-a1ea-35144b1eb7c5';
    $pane->panel = 'content_views';
    $pane->type = 'block';
    $pane->subtype = 'views-default_challenge-block';
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
    $pane->uuid = '7f733a13-5fda-40aa-a1ea-35144b1eb7c5';
    $display->content['new-7f733a13-5fda-40aa-a1ea-35144b1eb7c5'] = $pane;
    $display->panels['content_views'][0] = 'new-7f733a13-5fda-40aa-a1ea-35144b1eb7c5';
    $pane = new stdClass();
    $pane->pid = 'new-26bb24bf-64ef-4718-bae1-50486f7136a7';
    $pane->panel = 'content_views';
    $pane->type = 'block';
    $pane->subtype = 'views-challenges_featured_front-block';
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
    $pane->uuid = '26bb24bf-64ef-4718-bae1-50486f7136a7';
    $display->content['new-26bb24bf-64ef-4718-bae1-50486f7136a7'] = $pane;
    $display->panels['content_views'][1] = 'new-26bb24bf-64ef-4718-bae1-50486f7136a7';
    $pane = new stdClass();
    $pane->pid = 'new-71f7cfaa-0242-451f-9bb6-04291b4f246d';
    $pane->panel = 'content_views';
    $pane->type = 'panels_mini';
    $pane->subtype = 'latest_and_greatest';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '71f7cfaa-0242-451f-9bb6-04291b4f246d';
    $display->content['new-71f7cfaa-0242-451f-9bb6-04291b4f246d'] = $pane;
    $display->panels['content_views'][2] = 'new-71f7cfaa-0242-451f-9bb6-04291b4f246d';
    $pane = new stdClass();
    $pane->pid = 'new-fcc97e41-da68-4291-93c7-20b37a539afa';
    $pane->panel = 'content_views';
    $pane->type = 'block';
    $pane->subtype = 'views-head_to_head_ideas-block';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'fcc97e41-da68-4291-93c7-20b37a539afa';
    $display->content['new-fcc97e41-da68-4291-93c7-20b37a539afa'] = $pane;
    $display->panels['content_views'][3] = 'new-fcc97e41-da68-4291-93c7-20b37a539afa';
    $pane = new stdClass();
    $pane->pid = 'new-13fa8276-c46e-4cd6-9698-852950fca6e8';
    $pane->panel = 'sidebar';
    $pane->type = 'block';
    $pane->subtype = 'views-highlighted_content-block';
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
    $pane->uuid = '13fa8276-c46e-4cd6-9698-852950fca6e8';
    $display->content['new-13fa8276-c46e-4cd6-9698-852950fca6e8'] = $pane;
    $display->panels['sidebar'][0] = 'new-13fa8276-c46e-4cd6-9698-852950fca6e8';
    $pane = new stdClass();
    $pane->pid = 'new-22c22a09-d9f7-4f5c-9a0c-d17edc79e2be';
    $pane->panel = 'sidebar';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Intro video',
      'title' => 'How it works',
      'body' => '<iframe src="//player.vimeo.com/video/57515499?byline=0&amp;portrait=0" width="360" height="202" frameborder="0" webkitallowfullscreen mozallowfullscreen allowfullscreen></iframe>',
      'format' => 'full_html',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '22c22a09-d9f7-4f5c-9a0c-d17edc79e2be';
    $display->content['new-22c22a09-d9f7-4f5c-9a0c-d17edc79e2be'] = $pane;
    $display->panels['sidebar'][1] = 'new-22c22a09-d9f7-4f5c-9a0c-d17edc79e2be';
    $pane = new stdClass();
    $pane->pid = 'new-abf04e22-9bc4-44fc-b26a-c587159e347d';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'abf04e22-9bc4-44fc-b26a-c587159e347d';
    $display->content['new-abf04e22-9bc4-44fc-b26a-c587159e347d'] = $pane;
    $display->panels['sidebar'][2] = 'new-abf04e22-9bc4-44fc-b26a-c587159e347d';
    $pane = new stdClass();
    $pane->pid = 'new-cfe88c87-4f33-4e25-beff-094685b43633';
    $pane->panel = 'sidebar';
    $pane->type = 'block';
    $pane->subtype = 'ideal_blocks-ideal_key_stats';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'cfe88c87-4f33-4e25-beff-094685b43633';
    $display->content['new-cfe88c87-4f33-4e25-beff-094685b43633'] = $pane;
    $display->panels['sidebar'][3] = 'new-cfe88c87-4f33-4e25-beff-094685b43633';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['home'] = $page;

  return $pages;

}
