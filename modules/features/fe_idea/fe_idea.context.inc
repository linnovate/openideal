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

  $context = new stdClass();
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

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'idea_page';
  $context->description = '';
  $context->tag = 'Ideas';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'idea' => 'idea',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-merged_ideas-block' => array(
          'module' => 'views',
          'delta' => 'merged_ideas-block',
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
  $export['idea_page'] = $context;

  $context = new stdClass();
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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'search';
  $context->description = '';
  $context->tag = 'Search';
  $context->conditions = array(
    'path' => array(
      'values' => array(
        'search/*' => 'search/*',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'top_searches-0' => array(
          'module' => 'top_searches',
          'delta' => 0,
          'region' => 'sidebar_first',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Search');
  $export['search'] = $context;

  return $export;
}
