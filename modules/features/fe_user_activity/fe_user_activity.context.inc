<?php
/**
 * @file
 * fe_user_activity.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function fe_user_activity_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user_log';
  $context->description = '';
  $context->tag = 'Message';
  $context->conditions = array(
    'user_page' => array(
      'values' => array(
        'view' => 'view',
      ),
      'options' => array(
        'mode' => 'all',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-recent_logs-block' => array(
          'module' => 'views',
          'delta' => 'recent_logs-block',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Message');
  $export['user_log'] = $context;

  return $export;
}
