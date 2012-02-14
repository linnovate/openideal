<?php

/**
 * Implementation of hook_context_default_contexts().
 */
function fe_challenge_context_default_contexts() {
  $export = array();
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'challenge';
  $context->description = '';
  $context->tag = 'challenge';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'challenge' => 'challenge',
      ),
      'options' => array(
        'node_form' => '1',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-challenge-block_1' => array(
          'module' => 'views',
          'delta' => 'challenge-block_1',
          'region' => 'content',
          'weight' => 1,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('challenge');

  $export['challenge'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'challenge_front_page';
  $context->description = '';
  $context->tag = 'challenge';
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
        'ideal_challenge-challenge_add_idea' => array(
          'module' => 'ideal_challenge',
          'delta' => 'challenge_add_idea',
          'region' => 'content',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('challenge');

  $export['challenge_front_page'] = $context;
  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'challenge_page_add_idea';
  $context->description = '';
  $context->tag = 'challenge';
  $context->conditions = array(
    'php' => array(
      'values' => array(
        0 => TRUE,
      ),
      'options' => array(
        'phpcode' => '$default_challenge_nid = variable_get(\'default_challenge\', NULL);
if ($node = menu_get_object()) {
  if ($default_challenge_nid == $node->nid && arg(2) != \'edit\') {
    return TRUE;
  }
}',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'ideal_challenge-challenge_add_idea' => array(
          'module' => 'ideal_challenge',
          'delta' => 'challenge_add_idea',
          'region' => 'content',
          'weight' => 0,
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('challenge');

  $export['challenge_page_add_idea'] = $context;
  return $export;
}
