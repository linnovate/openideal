<?php
/**
 * @file
 * fe_idea.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function fe_idea_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'all_ideas';
  $context->description = '';
  $context->tag = 'Ideas';
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
        'quicktabs-all_ideas' => array(
          'module' => 'quicktabs',
          'delta' => 'all_ideas',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Ideas');
  $export['all_ideas'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'category_page';
  $context->description = 'The term page with list of ideas.';
  $context->tag = 'Ideas';
  $context->conditions = array(
    'taxonomy_term' => array(
      'values' => array(
        'categories' => 'categories',
      ),
      'options' => array(
        'term_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'quicktabs-ideas_by_category' => array(
          'module' => 'quicktabs',
          'delta' => 'ideas_by_category',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Ideas');
  t('The term page with list of ideas.');
  $export['category_page'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'front';
  $context->description = 'the home page';
  $context->tag = 'Front';
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
        'views-most_active_users-block' => array(
          'module' => 'views',
          'delta' => 'most_active_users-block',
          'region' => 'content',
          'weight' => '-10',
        ),
        'views-frontpage_ideas-block' => array(
          'module' => 'views',
          'delta' => 'frontpage_ideas-block',
          'region' => 'content',
          'weight' => '-10',
        ),
        'views-frontpage_ideas-block_1' => array(
          'module' => 'views',
          'delta' => 'frontpage_ideas-block_1',
          'region' => 'content',
          'weight' => '-10',
        ),
        'views-frontpage_ideas-block_3' => array(
          'module' => 'views',
          'delta' => 'frontpage_ideas-block_3',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Front');
  t('the home page');
  $export['front'] = $context;

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'in_action';
  $context->description = '';
  $context->tag = 'Ideas';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'in-action' => 'in-action',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'quicktabs-in_action' => array(
          'module' => 'quicktabs',
          'delta' => 'in_action',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Ideas');
  $export['in_action'] = $context;

  return $export;
}
