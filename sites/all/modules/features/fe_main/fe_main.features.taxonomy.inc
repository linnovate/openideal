<?php

/**
 * Implementation of hook_taxonomy_default_vocabularies().
 */
function fe_main_taxonomy_default_vocabularies() {
  return array(
    'categories' => array(
      'name' => 'Categories',
      'description' => '',
      'help' => '',
      'relations' => '1',
      'hierarchy' => '0',
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
    ),
  );
}
