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

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'challenge_front_page';
  $context->description = '';
  $context->tag = 'Challenge';
  $context->conditions = array();
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Challenge');
  $export['challenge_front_page'] = $context;

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'on_challenge_page';
  $context->description = '';
  $context->tag = 'Challenge';
  $context->conditions = array();
  $context->reactions = array();
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Challenge');
  $export['on_challenge_page'] = $context;

  return $export;
}
