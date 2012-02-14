<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function fe_main_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'brand_page';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'taxonomy/term/*' => 'taxonomy/term/*',
      ),
    ),
    'views' => array(
      'values' => array(
        'brand_page_ideas' => 'brand_page_ideas',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-brand_page_description-block_1' => array(
          'module' => 'views',
          'delta' => 'brand_page_description-block_1',
          'region' => 'content',
          'weight' => 0,
        ),
        'quicktabs-brand_page' => array(
          'module' => 'quicktabs',
          'delta' => 'brand_page',
          'region' => 'content',
          'weight' => 1,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  $export['brand_page'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'front_page';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '<front>' => '<front>',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'ideal-three_main_buttons' => array(
          'module' => 'ideal',
          'delta' => 'three_main_buttons',
          'region' => 'content',
          'weight' => 0,
        ),
        'views-idea_in_focus-block_1' => array(
          'module' => 'views',
          'delta' => 'idea_in_focus-block_1',
          'region' => 'content',
          'weight' => 1,
        ),
        'views-news-block_1' => array(
          'module' => 'views',
          'delta' => 'news-block_1',
          'region' => 'content',
          'weight' => 2,
        ),
        'views-recent_ideas-block_1' => array(
          'module' => 'views',
          'delta' => 'recent_ideas-block_1',
          'region' => 'content',
          'weight' => 3,
        ),
        'ideal-head_to_head' => array(
          'module' => 'ideal',
          'delta' => 'head_to_head',
          'region' => 'content',
          'weight' => 4,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  $export['front_page'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'ideas-index';
  $context->description = '';
  $context->tag = 'ideas';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'ideas' => 'ideas',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'quicktabs-ideas_index' => array(
          'module' => 'quicktabs',
          'delta' => 'ideas_index',
          'region' => 'content',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('ideas');

  $export['ideas-index'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'in_action';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'in-action' => 'in-action',
      ),
    ),
    'views' => array(
      'values' => array(
        'ideas_in_action' => 'ideas_in_action',
        'ideas_in_action:page_1' => 'ideas_in_action:page_1',
        'ideas_in_action:page_2' => 'ideas_in_action:page_2',
        'ideas_in_action:page_3' => 'ideas_in_action:page_3',
        'ideas_in_action:page_4' => 'ideas_in_action:page_4',
        'launced_projects' => 'launced_projects',
        'launced_projects:page_1' => 'launced_projects:page_1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'boxes-idea_in_action_video' => array(
          'module' => 'boxes',
          'delta' => 'idea_in_action_video',
          'region' => 'content',
          'weight' => 0,
        ),
        'boxes-idea_in_action_text' => array(
          'module' => 'boxes',
          'delta' => 'idea_in_action_text',
          'region' => 'content',
          'weight' => 1,
        ),
        'quicktabs-in_action' => array(
          'module' => 'quicktabs',
          'delta' => 'in_action',
          'region' => 'content',
          'weight' => 2,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  $export['in_action'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'not_front_page';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        '~<front>' => '~<front>',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 1;

  $export['not_front_page'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'sitewide';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'sitewide' => array(
      'values' => array(
        1 => 1,
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 1;

  $export['sitewide'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user_page';
  $context->description = '';
  $context->tag = 'user';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'current',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-user_page-block_1' => array(
          'module' => 'views',
          'delta' => 'user_page-block_1',
          'region' => 'content',
          'weight' => 1,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('user');

  $export['user_page'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user_page_other';
  $context->description = '';
  $context->tag = 'user';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'other',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-user_page_other-block_1' => array(
          'module' => 'views',
          'delta' => 'user_page_other-block_1',
          'region' => 'content',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('user');

  $export['user_page_other'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'view_ideas';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'views' => array(
      'values' => array(
        'ideas:page_1' => 'ideas:page_1',
        'ideas:page_2' => 'ideas:page_2',
        'ideas:page_3' => 'ideas:page_3',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-recent_comments-block' => array(
          'module' => 'views',
          'delta' => 'recent_comments-block',
          'region' => 'right',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  $export['view_ideas'] = $context;
  return $export;
}
