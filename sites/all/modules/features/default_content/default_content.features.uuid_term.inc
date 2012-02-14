<?php

/**
 * Implementation of hook_uuid_features_default_terms().
 */
function default_content_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'uuid' => '87bd2aa7-79a8-11e0-ad89-c44619ee2e3d',
    'name' => 'Lyrics',
    'description' => '',
    'weight' => '0',
    'uuid_vocabulary' => 'fd8f023d-78ab-11e0-ad89-c44619ee2e3d',
  );
  $terms[] = array(
    'uuid' => '87be4a73-79a8-11e0-ad89-c44619ee2e3d',
    'name' => 'Songs',
    'description' => '',
    'weight' => '0',
    'uuid_vocabulary' => 'fd8f023d-78ab-11e0-ad89-c44619ee2e3d',
  );
  return $terms;
}
