<?php
/**
 * @file
 * fe_idea.features.taxonomy.inc
 */

/**
 * Implements hook_taxonomy_default_vocabularies().
 */
function fe_idea_taxonomy_default_vocabularies() {
  return array(
    'hashtags' => array(
      'name' => 'Hashtags',
      'machine_name' => 'hashtags',
      'description' => 'Hashtag vocabulary',
      'hierarchy' => '1',
      'module' => 'hashtags',
      'weight' => '-11',
      'rdf_mapping' => array(
        'rdftype' => array(
          0 => 'skos:ConceptScheme',
        ),
        'name' => array(
          'predicates' => array(
            0 => 'dc:title',
          ),
        ),
        'description' => array(
          'predicates' => array(
            0 => 'rdfs:comment',
          ),
        ),
      ),
    ),
  );
}
