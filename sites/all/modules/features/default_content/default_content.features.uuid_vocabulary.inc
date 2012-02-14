<?php

/**
 * Implementation of hook_uuid_features_default_vocabularies().
 */
function default_content_uuid_features_default_vocabularies() {
  $vocabularies = array();

  $vocabularies[] = array(
    'uuid' => 'fd8f023d-78ab-11e0-ad89-c44619ee2e3d',
    'name' => 'Categories',
    'description' => '',
    'help' => '',
    'relations' => '1',
    'multiple' => '0',
    'required' => '1',
    'tags' => '0',
    'module' => 'features_categories',
    'weight' => '0',
    'nodes' => array(
      'challenge' => 'challenge',
      'idea' => 'idea',
      'project' => 'project',
    ),
  );
  return $vocabularies;
}
