<?php
/**
 * @file
 * fe_challenge.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function fe_challenge_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'on_challenge_page';
  $context->description = '';
  $context->tag = 'Challenge';
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
        'views-ideas_in_challenge-block' => array(
          'module' => 'views',
          'delta' => 'ideas_in_challenge-block',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Challenge');
  $export['on_challenge_page'] = $context;

  return $export;
}
