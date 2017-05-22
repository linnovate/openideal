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
    $pane->pid = 'new-db9c9538-a414-4972-b04b-abe0a4e0c81c';
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
    $pane->uuid = 'db9c9538-a414-4972-b04b-abe0a4e0c81c';
    $display->content['new-db9c9538-a414-4972-b04b-abe0a4e0c81c'] = $pane;
    $display->panels['region_first'][0] = 'new-db9c9538-a414-4972-b04b-abe0a4e0c81c';
    $pane = new stdClass();
    $pane->pid = 'new-d04aa660-a1c8-49db-8641-7cdbb0310118';
    $pane->panel = 'region_first';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_challenge_ref';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'entityreference_label',
      'delta_limit' => 0,
      'delta_offset' => '0',
      'delta_reversed' => FALSE,
      'formatter_settings' => array(
        'link' => 0,
      ),
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'd04aa660-a1c8-49db-8641-7cdbb0310118';
    $display->content['new-d04aa660-a1c8-49db-8641-7cdbb0310118'] = $pane;
    $display->panels['region_first'][1] = 'new-d04aa660-a1c8-49db-8641-7cdbb0310118';
    $pane = new stdClass();
    $pane->pid = 'new-54cb9361-7019-4743-86c1-7ecfe4d3fc00';
    $pane->panel = 'region_first';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '54cb9361-7019-4743-86c1-7ecfe4d3fc00';
    $display->content['new-54cb9361-7019-4743-86c1-7ecfe4d3fc00'] = $pane;
    $display->panels['region_first'][2] = 'new-54cb9361-7019-4743-86c1-7ecfe4d3fc00';
    $pane = new stdClass();
    $pane->pid = 'new-e805b735-e5c1-409f-9d19-25607804e258';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'idea_display';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '1',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'idea',
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'e805b735-e5c1-409f-9d19-25607804e258';
    $display->content['new-e805b735-e5c1-409f-9d19-25607804e258'] = $pane;
    $display->panels['region_first'][3] = 'new-e805b735-e5c1-409f-9d19-25607804e258';
    $pane = new stdClass();
    $pane->pid = 'new-6d606a89-3334-4e8e-85b7-40784a5ea98b';
    $pane->panel = 'region_first';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Back to challenge btn',
      'title' => '',
      'body' => '<?php 
if("%node_2:url" != \'\') 
  print "<a class=\'back-challenge\' href=\'%node_2:url\'>".t(\'Back to challenge\'). "</a>";
?>',
      'format' => 'php_code',
      'substitute' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array();
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '6d606a89-3334-4e8e-85b7-40784a5ea98b';
    $display->content['new-6d606a89-3334-4e8e-85b7-40784a5ea98b'] = $pane;
    $display->panels['region_first'][4] = 'new-6d606a89-3334-4e8e-85b7-40784a5ea98b';
    $pane = new stdClass();
    $pane->pid = 'new-a822cf03-1cf5-4739-a560-b45de2314855';
    $pane->panel = 'region_first';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'How To Make Solution',
      'title' => 'How To Make Solution',
      'title_heading' => 'h2',
      'body' => '<?php
  $output = \'<div class= "howToMakeIdea">\';
  $output .=\'<div class = "title">5 Essentials of an  award winning case</div>\';

  $output .= \'<div class="challenge-process">\';
  $output .= \'<div class="level">\' . t(\'Cultural context\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Game changing idea\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Insight\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Connected System\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Results\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'</div></div>\';

   return $output;

?>',
      'format' => 'php_code',
      'substitute' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'how-to-make-solution',
    );
    $pane->extras = array();
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = 'a822cf03-1cf5-4739-a560-b45de2314855';
    $display->content['new-a822cf03-1cf5-4739-a560-b45de2314855'] = $pane;
    $display->panels['region_first'][5] = 'new-a822cf03-1cf5-4739-a560-b45de2314855';
    $pane = new stdClass();
    $pane->pid = 'new-076df314-5fe2-4a4d-94ab-5a784aa3f309';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'ideas_all';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '2',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_2',
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
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = '076df314-5fe2-4a4d-94ab-5a784aa3f309';
    $display->content['new-076df314-5fe2-4a4d-94ab-5a784aa3f309'] = $pane;
    $display->panels['region_first'][6] = 'new-076df314-5fe2-4a4d-94ab-5a784aa3f309';
    $pane = new stdClass();
    $pane->pid = 'new-48135a6b-2312-43c3-978b-6975e8a9aa79';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'ideal_challenge-challenge_amount_of_comments';
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '48135a6b-2312-43c3-978b-6975e8a9aa79';
    $display->content['new-48135a6b-2312-43c3-978b-6975e8a9aa79'] = $pane;
    $display->panels['region_second'][0] = 'new-48135a6b-2312-43c3-978b-6975e8a9aa79';
    $pane = new stdClass();
    $pane->pid = 'new-c3c069da-e134-488b-b601-313937b4fa16';
    $pane->panel = 'region_second';
    $pane->type = 'node_comment_form';
    $pane->subtype = 'node_comment_form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'anon_links' => 1,
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => 'Comments',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => 'comment-form-id',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'c3c069da-e134-488b-b601-313937b4fa16';
    $display->content['new-c3c069da-e134-488b-b601-313937b4fa16'] = $pane;
    $display->panels['region_second'][1] = 'new-c3c069da-e134-488b-b601-313937b4fa16';
    $pane = new stdClass();
    $pane->pid = 'new-c53b8dc1-2d06-4bf8-8ca5-ce4cb2e9cd3e';
    $pane->panel = 'region_second';
    $pane->type = 'node_comments';
    $pane->subtype = 'node_comments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'mode' => '1',
      'comments_per_page' => '50',
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => '',
      'override_title_heading' => 'h3',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => 'comments',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'c53b8dc1-2d06-4bf8-8ca5-ce4cb2e9cd3e';
    $display->content['new-c53b8dc1-2d06-4bf8-8ca5-ce4cb2e9cd3e'] = $pane;
    $display->panels['region_second'][2] = 'new-c53b8dc1-2d06-4bf8-8ca5-ce4cb2e9cd3e';
    $pane = new stdClass();
    $pane->pid = 'new-6d39ddb3-b92f-4ec9-bc58-b86c9b3383e3';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'ideal_blocks-idea_stats';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '6d39ddb3-b92f-4ec9-bc58-b86c9b3383e3';
    $display->content['new-6d39ddb3-b92f-4ec9-bc58-b86c9b3383e3'] = $pane;
    $display->panels['region_second'][3] = 'new-6d39ddb3-b92f-4ec9-bc58-b86c9b3383e3';
    $pane = new stdClass();
    $pane->pid = 'new-43cf49fe-ba12-41ae-ab8b-f55283b28e0b';
    $pane->panel = 'region_second';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_attachments';
    $pane->shown = FALSE;
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
    $pane->uuid = '43cf49fe-ba12-41ae-ab8b-f55283b28e0b';
    $display->content['new-43cf49fe-ba12-41ae-ab8b-f55283b28e0b'] = $pane;
    $display->panels['region_second'][4] = 'new-43cf49fe-ba12-41ae-ab8b-f55283b28e0b';
    $pane = new stdClass();
    $pane->pid = 'new-ff2054f4-49ca-41df-9c34-8b9203d01f29';
    $pane->panel = 'region_second';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_idea_image';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'label' => 'title',
      'formatter' => 'image',
      'delta_limit' => '0',
      'delta_offset' => '0',
      'delta_reversed' => 0,
      'formatter_settings' => array(
        'image_style' => '',
        'image_link' => '',
      ),
      'context' => 'argument_entity_id:node_1',
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = 'ff2054f4-49ca-41df-9c34-8b9203d01f29';
    $display->content['new-ff2054f4-49ca-41df-9c34-8b9203d01f29'] = $pane;
    $display->panels['region_second'][5] = 'new-ff2054f4-49ca-41df-9c34-8b9203d01f29';
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
    'no_blocks' => 0,
    'pipeline' => 'standard',
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
  $display->layout = 'tow_col';
  $display->layout_settings = array();
  $display->panel_settings = array(
    'style_settings' => array(
      'default' => NULL,
      'center' => NULL,
      'sidebar' => NULL,
      'content_top' => NULL,
      'content_bottom' => NULL,
      'region_first' => NULL,
      'region_second' => NULL,
    ),
  );
  $display->cache = array();
  $display->title = '';
  $display->uuid = '0b009613-3d6b-4e3e-a0c5-ce58071215df';
  $display->content = array();
  $display->panels = array();
    $pane = new stdClass();
    $pane->pid = 'new-beaa6e21-6f47-447f-9e5a-d6881fd2d253';
    $pane->panel = 'region_first';
    $pane->type = 'node_title';
    $pane->subtype = 'node_title';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'link' => 0,
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
    $pane->uuid = 'beaa6e21-6f47-447f-9e5a-d6881fd2d253';
    $display->content['new-beaa6e21-6f47-447f-9e5a-d6881fd2d253'] = $pane;
    $display->panels['region_first'][0] = 'new-beaa6e21-6f47-447f-9e5a-d6881fd2d253';
    $pane = new stdClass();
    $pane->pid = 'new-bfb6e505-bc4c-455e-9c5c-841fb2e50636';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'challenge_display';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '1',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_1',
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
    $pane->uuid = 'bfb6e505-bc4c-455e-9c5c-841fb2e50636';
    $display->content['new-bfb6e505-bc4c-455e-9c5c-841fb2e50636'] = $pane;
    $display->panels['region_first'][1] = 'new-bfb6e505-bc4c-455e-9c5c-841fb2e50636';
    $pane = new stdClass();
    $pane->pid = 'new-7471dd49-0029-46f9-83fc-0e1eb2637743';
    $pane->panel = 'region_first';
    $pane->type = 'block';
    $pane->subtype = 'idea_widget-add_assigned_idea_button';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '7471dd49-0029-46f9-83fc-0e1eb2637743';
    $display->content['new-7471dd49-0029-46f9-83fc-0e1eb2637743'] = $pane;
    $display->panels['region_first'][2] = 'new-7471dd49-0029-46f9-83fc-0e1eb2637743';
    $pane = new stdClass();
    $pane->pid = 'new-d9c05505-089f-4a48-a0e8-0e5ff007d45a';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'ideas_in_challenge';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '0',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block',
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'd9c05505-089f-4a48-a0e8-0e5ff007d45a';
    $display->content['new-d9c05505-089f-4a48-a0e8-0e5ff007d45a'] = $pane;
    $display->panels['region_first'][3] = 'new-d9c05505-089f-4a48-a0e8-0e5ff007d45a';
    $pane = new stdClass();
    $pane->pid = 'new-898a0157-5c80-411d-b702-3aaeaad4725a';
    $pane->panel = 'region_first';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'How To Make Solution',
      'title' => 'Keep In Mind',
      'title_heading' => 'h2',
      'body' => '<?php
  $output = \'<div class= "howToMakeIdea">\';
  $output .=\'<div class = "title">5 Essentials of an  award winning case</div>\';

  $output .= \'<div class="challenge-process">\';
  $output .= \'<div class="level">\' . t(\'Cultural context\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Game changing idea\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Insight\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Connected System\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Results\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'</div></div>\';

   return $output;

?>',
      'format' => 'php_code',
      'substitute' => 1,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'how-to-make-solution',
    );
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '898a0157-5c80-411d-b702-3aaeaad4725a';
    $display->content['new-898a0157-5c80-411d-b702-3aaeaad4725a'] = $pane;
    $display->panels['region_first'][4] = 'new-898a0157-5c80-411d-b702-3aaeaad4725a';
    $pane = new stdClass();
    $pane->pid = 'new-f73df4c2-a8a7-4cc4-bd52-18ed7b9e467e';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'ideas_all';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '2',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_2',
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = 'f73df4c2-a8a7-4cc4-bd52-18ed7b9e467e';
    $display->content['new-f73df4c2-a8a7-4cc4-bd52-18ed7b9e467e'] = $pane;
    $display->panels['region_first'][5] = 'new-f73df4c2-a8a7-4cc4-bd52-18ed7b9e467e';
    $pane = new stdClass();
    $pane->pid = 'new-8d488c8f-bf5d-46fc-be3b-f99143356011';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'ideal_challenge-challenge_add_idea_button';
    $pane->shown = FALSE;
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '8d488c8f-bf5d-46fc-be3b-f99143356011';
    $display->content['new-8d488c8f-bf5d-46fc-be3b-f99143356011'] = $pane;
    $display->panels['region_second'][0] = 'new-8d488c8f-bf5d-46fc-be3b-f99143356011';
    $pane = new stdClass();
    $pane->pid = 'new-2da010b3-8380-4e19-b366-4d94eb6f5721';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'ideal_challenge-challenge_data';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = '2da010b3-8380-4e19-b366-4d94eb6f5721';
    $display->content['new-2da010b3-8380-4e19-b366-4d94eb6f5721'] = $pane;
    $display->panels['region_second'][1] = 'new-2da010b3-8380-4e19-b366-4d94eb6f5721';
    $pane = new stdClass();
    $pane->pid = 'new-9eff432b-ab70-4a0b-b3dc-a79236f2e0a6';
    $pane->panel = 'region_second';
    $pane->type = 'entity_field';
    $pane->subtype = 'node:field_attachments';
    $pane->shown = FALSE;
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
    $pane->uuid = '9eff432b-ab70-4a0b-b3dc-a79236f2e0a6';
    $display->content['new-9eff432b-ab70-4a0b-b3dc-a79236f2e0a6'] = $pane;
    $display->panels['region_second'][2] = 'new-9eff432b-ab70-4a0b-b3dc-a79236f2e0a6';
    $pane = new stdClass();
    $pane->pid = 'new-cbc55058-13c9-4932-a80a-4297ddeadfe2';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'ideal_blocks-challenge_stats';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'cbc55058-13c9-4932-a80a-4297ddeadfe2';
    $display->content['new-cbc55058-13c9-4932-a80a-4297ddeadfe2'] = $pane;
    $display->panels['region_second'][3] = 'new-cbc55058-13c9-4932-a80a-4297ddeadfe2';
    $pane = new stdClass();
    $pane->pid = 'new-0c1c2405-34ab-46a2-8916-4c13a2610b8f';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'views-activity-block';
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
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'display-comments',
    );
    $pane->extras = array();
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '0c1c2405-34ab-46a2-8916-4c13a2610b8f';
    $display->content['new-0c1c2405-34ab-46a2-8916-4c13a2610b8f'] = $pane;
    $display->panels['region_second'][4] = 'new-0c1c2405-34ab-46a2-8916-4c13a2610b8f';
    $pane = new stdClass();
    $pane->pid = 'new-771a58b0-6868-4b17-882d-ff393857b5b7';
    $pane->panel = 'region_second';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Essentials of challenge process',
      'title' => '5 Essentials of an  award winning case',
      'title_heading' => 'h2',
      'body' => '<?php
  $output = \'<div class="challenge-process">\';
  $output .= \'<div class="level">\' . t(\'Cultural context\') . \'<div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div></div>\';
  $output .= \'<div class="level">\' . t(\'Game changing idea\') . \'<div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div></div>\';
  $output .= \'<div class="level">\' . t(\'Insight\') . \'<div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div></div>\';
  $output .= \'<div class="level">\' . t(\'Connected System\') . \'<div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div></div>\';
  $output .= \'<div class="level">\' . t(\'Results\') . \'<div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div></div>\';
  $output .= \'</div>\';

   return $output;

?>',
      'format' => 'php_code',
      'substitute' => 0,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => 'challenge_process',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = '771a58b0-6868-4b17-882d-ff393857b5b7';
    $display->content['new-771a58b0-6868-4b17-882d-ff393857b5b7'] = $pane;
    $display->panels['region_second'][5] = 'new-771a58b0-6868-4b17-882d-ff393857b5b7';
    $pane = new stdClass();
    $pane->pid = 'new-d2e986a8-bac3-435d-94f8-9e317b3fcf1c';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'ideal_challenge-challenge_amount_of_comments';
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
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = 'd2e986a8-bac3-435d-94f8-9e317b3fcf1c';
    $display->content['new-d2e986a8-bac3-435d-94f8-9e317b3fcf1c'] = $pane;
    $display->panels['region_second'][6] = 'new-d2e986a8-bac3-435d-94f8-9e317b3fcf1c';
    $pane = new stdClass();
    $pane->pid = 'new-f02c3dfe-eb90-454d-a0b4-6a7fbddf91b7';
    $pane->panel = 'region_second';
    $pane->type = 'node_comment_form';
    $pane->subtype = 'node_comment_form';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'anon_links' => 1,
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => 'תגובות',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => 'comment-form-id',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 7;
    $pane->locks = array();
    $pane->uuid = 'f02c3dfe-eb90-454d-a0b4-6a7fbddf91b7';
    $display->content['new-f02c3dfe-eb90-454d-a0b4-6a7fbddf91b7'] = $pane;
    $display->panels['region_second'][7] = 'new-f02c3dfe-eb90-454d-a0b4-6a7fbddf91b7';
    $pane = new stdClass();
    $pane->pid = 'new-8f4b7849-f690-4218-9b3c-1112b506fe84';
    $pane->panel = 'region_second';
    $pane->type = 'node_comments';
    $pane->subtype = 'node_comments';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'mode' => '1',
      'comments_per_page' => '50',
      'context' => 'argument_entity_id:node_1',
      'override_title' => 1,
      'override_title_text' => '',
      'override_title_heading' => 'h2',
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => 'comments',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 8;
    $pane->locks = array();
    $pane->uuid = '8f4b7849-f690-4218-9b3c-1112b506fe84';
    $display->content['new-8f4b7849-f690-4218-9b3c-1112b506fe84'] = $pane;
    $display->panels['region_second'][8] = 'new-8f4b7849-f690-4218-9b3c-1112b506fe84';
    $pane = new stdClass();
    $pane->pid = 'new-121fa630-05d0-404d-bfed-3347dd92e237';
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
      'context' => array(
        0 => '',
      ),
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
    $pane->position = 9;
    $pane->locks = array();
    $pane->uuid = '121fa630-05d0-404d-bfed-3347dd92e237';
    $display->content['new-121fa630-05d0-404d-bfed-3347dd92e237'] = $pane;
    $display->panels['region_second'][9] = 'new-121fa630-05d0-404d-bfed-3347dd92e237';
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
      'top' => NULL,
      'left' => NULL,
      'middle' => NULL,
      'right' => NULL,
      'bottom' => NULL,
      'center' => NULL,
      'content_views' => NULL,
      'sidebar' => NULL,
      'region_first' => NULL,
      'region_second' => NULL,
      'region_third' => NULL,
      'region_fourth' => NULL,
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
    $pane->pid = 'new-a355f414-7f86-4170-8f92-3d12690c4fc3';
    $pane->panel = 'region_first';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = FALSE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'Roundtable',
      'title' => '',
      'body' => 'OpenideaL is an idea management system (IDMS), adapted for companies & organizations interested in sharing strategic decisions and future planning of products and services with their customers.

It is a perfect tool for decision makers and their analysts who need to take strategic decisions, and wish to expand their view of the problem with the poser of the crowd, whether these are the organization\'s employees or any other public.

OpenideaL is an open source project based on Drupal and therefore it is modular, and allows growth and adaptation to the organization’s specific needs. These adaptations may include a unique design, polls and surveys, interfacing with external applications such as Facebook or to applications within the organization itself, and adapting the interface to a range of devices: phones, desktops, tablets or kiosks.',
      'format' => 'plain_text',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'round-table',
    );
    $pane->extras = array();
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = 'a355f414-7f86-4170-8f92-3d12690c4fc3';
    $display->content['new-a355f414-7f86-4170-8f92-3d12690c4fc3'] = $pane;
    $display->panels['region_first'][0] = 'new-a355f414-7f86-4170-8f92-3d12690c4fc3';
    $pane = new stdClass();
    $pane->pid = 'new-cfe88c87-4f33-4e25-beff-094685b43633';
    $pane->panel = 'region_first';
    $pane->type = 'block';
    $pane->subtype = 'ideal_blocks-ideal_key_stats';
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
    $pane->position = 1;
    $pane->locks = array();
    $pane->uuid = 'cfe88c87-4f33-4e25-beff-094685b43633';
    $display->content['new-cfe88c87-4f33-4e25-beff-094685b43633'] = $pane;
    $display->panels['region_first'][1] = 'new-cfe88c87-4f33-4e25-beff-094685b43633';
    $pane = new stdClass();
    $pane->pid = 'new-71f7cfaa-0242-451f-9bb6-04291b4f246d';
    $pane->panel = 'region_first';
    $pane->type = 'panels_mini';
    $pane->subtype = 'latest_and_greatest';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = '71f7cfaa-0242-451f-9bb6-04291b4f246d';
    $display->content['new-71f7cfaa-0242-451f-9bb6-04291b4f246d'] = $pane;
    $display->panels['region_first'][2] = 'new-71f7cfaa-0242-451f-9bb6-04291b4f246d';
    $pane = new stdClass();
    $pane->pid = 'new-dd636cdc-0bb4-4493-8590-69b0057152f1';
    $pane->panel = 'region_first';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'hp-title',
      'title' => '',
      'title_heading' => 'h2',
      'body' => '<?php print t("The power of the group to solve business challenges"); ?>',
      'format' => 'php_code',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => 'hp-title',
      'css_class' => '',
    );
    $pane->extras = array();
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = 'dd636cdc-0bb4-4493-8590-69b0057152f1';
    $display->content['new-dd636cdc-0bb4-4493-8590-69b0057152f1'] = $pane;
    $display->panels['region_first'][3] = 'new-dd636cdc-0bb4-4493-8590-69b0057152f1';
    $pane = new stdClass();
    $pane->pid = 'new-55172024-51aa-4a63-87ea-6786e9e35b36';
    $pane->panel = 'region_first';
    $pane->type = 'block';
    $pane->subtype = 'idea_widget-add_unassigned_idea_button';
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
    $pane->position = 4;
    $pane->locks = array();
    $pane->uuid = '55172024-51aa-4a63-87ea-6786e9e35b36';
    $display->content['new-55172024-51aa-4a63-87ea-6786e9e35b36'] = $pane;
    $display->panels['region_first'][4] = 'new-55172024-51aa-4a63-87ea-6786e9e35b36';
    $pane = new stdClass();
    $pane->pid = 'new-8003a480-5e5a-4e9b-bdfd-b780f37a2802';
    $pane->panel = 'region_first';
    $pane->type = 'block';
    $pane->subtype = 'challenge_widget-add_challenge_button';
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
    $pane->position = 5;
    $pane->locks = array();
    $pane->uuid = '8003a480-5e5a-4e9b-bdfd-b780f37a2802';
    $display->content['new-8003a480-5e5a-4e9b-bdfd-b780f37a2802'] = $pane;
    $display->panels['region_first'][5] = 'new-8003a480-5e5a-4e9b-bdfd-b780f37a2802';
    $pane = new stdClass();
    $pane->pid = 'new-babc9a1a-48f6-4827-af4f-15baceedb50f';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'challenges_home_page';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '2',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'default',
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
    $pane->position = 6;
    $pane->locks = array();
    $pane->uuid = 'babc9a1a-48f6-4827-af4f-15baceedb50f';
    $display->content['new-babc9a1a-48f6-4827-af4f-15baceedb50f'] = $pane;
    $display->panels['region_first'][6] = 'new-babc9a1a-48f6-4827-af4f-15baceedb50f';
    $pane = new stdClass();
    $pane->pid = 'new-bfae1b21-5798-4a9e-9100-1c00d815cd02';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'ideas_all';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '3',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'latest_solutions',
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
    $pane->position = 7;
    $pane->locks = array();
    $pane->uuid = 'bfae1b21-5798-4a9e-9100-1c00d815cd02';
    $display->content['new-bfae1b21-5798-4a9e-9100-1c00d815cd02'] = $pane;
    $display->panels['region_first'][7] = 'new-bfae1b21-5798-4a9e-9100-1c00d815cd02';
    $pane = new stdClass();
    $pane->pid = 'new-0b05cdba-a76b-4f63-aeab-4628983d4e13';
    $pane->panel = 'region_first';
    $pane->type = 'custom';
    $pane->subtype = 'custom';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'admin_title' => 'How To Make Solution',
      'title' => 'Keep In Mind',
      'title_heading' => 'h2',
      'body' => '<?php
  $output = \'<div class= "howToMakeIdea">\';
  $output .=\'<div class = "title">5 Essentials of an  award winning case</div>\';

  $output .= \'<div class="challenge-process">\';
  $output .= \'<div class="level">\' . t(\'Cultural context\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Game changing idea\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Insight\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Connected System\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'<div class="level">\' . t(\'Results\') . \'</div><div class="description">\' . t(\'Lorem ipsum dolor sit amet\') . \'</div>\';
  $output .= \'</div></div>\';

   return $output;

?>',
      'format' => 'php_code',
      'substitute' => TRUE,
    );
    $pane->cache = array();
    $pane->style = array(
      'settings' => NULL,
    );
    $pane->css = array(
      'css_id' => '',
      'css_class' => 'how-to-make-solution',
    );
    $pane->extras = array();
    $pane->position = 8;
    $pane->locks = array();
    $pane->uuid = '0b05cdba-a76b-4f63-aeab-4628983d4e13';
    $display->content['new-0b05cdba-a76b-4f63-aeab-4628983d4e13'] = $pane;
    $display->panels['region_first'][8] = 'new-0b05cdba-a76b-4f63-aeab-4628983d4e13';
    $pane = new stdClass();
    $pane->pid = 'new-222938e2-d58a-4d8f-b72f-acfb0596714b';
    $pane->panel = 'region_first';
    $pane->type = 'views';
    $pane->subtype = 'ideas_all';
    $pane->shown = TRUE;
    $pane->access = array();
    $pane->configuration = array(
      'override_pager_settings' => 0,
      'use_pager' => 0,
      'nodes_per_page' => '2',
      'pager_id' => '0',
      'offset' => '0',
      'more_link' => 0,
      'feed_icons' => 0,
      'panel_args' => 0,
      'link_to_view' => 0,
      'args' => '',
      'url' => '',
      'display' => 'block_2',
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
    $pane->position = 9;
    $pane->locks = array();
    $pane->uuid = '222938e2-d58a-4d8f-b72f-acfb0596714b';
    $display->content['new-222938e2-d58a-4d8f-b72f-acfb0596714b'] = $pane;
    $display->panels['region_first'][9] = 'new-222938e2-d58a-4d8f-b72f-acfb0596714b';
    $pane = new stdClass();
    $pane->pid = 'new-0021a622-c53c-44c0-916a-7062c5ea6f0d';
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
    $pane->position = 0;
    $pane->locks = array();
    $pane->uuid = '0021a622-c53c-44c0-916a-7062c5ea6f0d';
    $display->content['new-0021a622-c53c-44c0-916a-7062c5ea6f0d'] = $pane;
    $display->panels['region_second'][0] = 'new-0021a622-c53c-44c0-916a-7062c5ea6f0d';
    $pane = new stdClass();
    $pane->pid = 'new-7f733a13-5fda-40aa-a1ea-35144b1eb7c5';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'views-default_challenge-block';
    $pane->shown = FALSE;
    $pane->access = array(
      'plugins' => array(),
    );
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
    $pane->uuid = '7f733a13-5fda-40aa-a1ea-35144b1eb7c5';
    $display->content['new-7f733a13-5fda-40aa-a1ea-35144b1eb7c5'] = $pane;
    $display->panels['region_second'][1] = 'new-7f733a13-5fda-40aa-a1ea-35144b1eb7c5';
    $pane = new stdClass();
    $pane->pid = 'new-fcc97e41-da68-4291-93c7-20b37a539afa';
    $pane->panel = 'region_second';
    $pane->type = 'block';
    $pane->subtype = 'views-head_to_head_ideas-block';
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
    $pane->position = 2;
    $pane->locks = array();
    $pane->uuid = 'fcc97e41-da68-4291-93c7-20b37a539afa';
    $display->content['new-fcc97e41-da68-4291-93c7-20b37a539afa'] = $pane;
    $display->panels['region_second'][2] = 'new-fcc97e41-da68-4291-93c7-20b37a539afa';
    $pane = new stdClass();
    $pane->pid = 'new-829ab91c-c5bb-40ae-a82e-56e6f291a53d';
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
    $pane->position = 3;
    $pane->locks = array();
    $pane->uuid = '829ab91c-c5bb-40ae-a82e-56e6f291a53d';
    $display->content['new-829ab91c-c5bb-40ae-a82e-56e6f291a53d'] = $pane;
    $display->panels['region_second'][3] = 'new-829ab91c-c5bb-40ae-a82e-56e6f291a53d';
    $pane = new stdClass();
    $pane->pid = 'new-28b2d7d8-d74a-46ac-9dff-fea66696ae91';
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
    $pane->uuid = '28b2d7d8-d74a-46ac-9dff-fea66696ae91';
    $display->content['new-28b2d7d8-d74a-46ac-9dff-fea66696ae91'] = $pane;
    $display->panels['region_second'][4] = 'new-28b2d7d8-d74a-46ac-9dff-fea66696ae91';
  $display->hide_title = PANELS_TITLE_NONE;
  $display->title_pane = '0';
  $handler->conf['display'] = $display;
  $page->default_handlers[$handler->name] = $handler;
  $pages['home'] = $page;

  return $pages;

}
