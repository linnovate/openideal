<?php
/**
 * @file
 * fe_project.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function fe_project_context_default_contexts() {
  $export = array();

  $context = new stdClass;
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'on_project_page';
  $context->description = '';
  $context->tag = 'Project';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'project' => 'project',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array(
    'block' => array(
      'blocks' => array(
        'views-ideas_in_project-block' => array(
          'module' => 'views',
          'delta' => 'ideas_in_project-block',
          'region' => 'content',
          'weight' => '-10',
        ),
      ),
    ),
  );
  $context->condition_mode = 0;

  // Translatables
  // Included for use with string extractors like potx.
  t('Project');
  $export['on_project_page'] = $context;

  return $export;
}
