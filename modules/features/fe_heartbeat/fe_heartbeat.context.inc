<?php
/**
 * @file
 * fe_heartbeat.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function fe_heartbeat_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'user_heartbeat';
  $context->description = '';
  $context->tag = 'Heartbeat';
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
        'heartbeat-userpage_activity' => array(
          'module' => 'heartbeat',
          'delta' => 'userpage_activity',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Heartbeat');
  $export['user_heartbeat'] = $context;

  return $export;
}
