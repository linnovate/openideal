<?php
// $Id: taxonomy.inc,v 1.1.2.8.2.4 2009/06/01 22:22:51 jamesandres Exp $

/**
 * Given the name of a vocabulary, return its Vocab ID.
 *
 * @param $name
 *   A text string identifying the vocabulary.
 */
function install_taxonomy_get_vid($name) {
  // Not guaranteed to be unique, hence the range limit.
  return db_result(db_query_range("SELECT vid FROM {vocabulary} WHERE name = '%s'", $name, 0, 1));
};

/**
 * Given the name of a term, return its tid.
 *
 * @param $name
 *   A term name to search on.
 * @param $vid
 *   A vocabulary to limit the search to.
 */
function install_taxonomy_get_tid($name, $vid = 0) {
  // if a vid is passed in, limit to it
  if ($vid) {
    $results = db_query_range("SELECT tid FROM {term_data} WHERE name = '%s' and vid = %d", $name, $vid, 0, 1);
  }
  else {
    $results = db_query_range("SELECT tid FROM {term_data} WHERE name = '%s'", $name, 0, 1);
  }
  return db_result($results);
};

/**
 * Create a new taxonomy vocabulary
 * 
 * @param $vocab_name
 *   The vocabulary name.
 * @param $node_types
 *   Optional. An array of content types.
 * @param $properties
 *   Optional additional properties to the vocabulary
 * @return  integer
 *   The database ID of the created vocabulary.
 */
function install_taxonomy_add_vocabulary($vocab_name, $content_types = array(), $properties = array()) {
  $return = 0;

  // Default properties so you don't have to pass anything.
  $defaults = array(
    'name' => $vocab_name,
    'tags' => 0,
    'multiple' => 0,
    'required' => 0,
    'hierarchy' => 0,
    'relations' => 0,
    'module' => 'taxonomy',
    'weight' => 0,
    'nodes' => $content_types,
    'help' => '',
  );

  $vocabulary = array_merge($defaults, $properties);
  taxonomy_save_vocabulary($vocabulary);
  $return = install_taxonomy_get_vid($vocab_name);
  
  return $return;
}

/**
 * Create a new taxonomy term.
 *
 * @param $vid
 *   The vocabulary ID
 * @param $name
 *   The text name of the new term.
 * @param $description
 *   Term description.
 * @return integer
 *   The database ID of the created term.
 */
function install_taxonomy_add_term($vid, $name, $description = '', $properties = array()) {
  $return = 0;
  // default properties.
  $defaults = array(
    'name' => $name,
    'description' => $description,
    'parent' => array(),
    'relations' => array(),
    'weight' => 0,
    'vid' => $vid,
  );
  $term = array_merge($defaults, $properties);
  taxonomy_save_term($term);
  
  // taxonomy_save_term() saves the tid to the array passed in
  if (isset($term['tid']) && is_numeric($term['tid'])){
    $return = $term['tid'];
  }

  return $return;
}

/**
 * Assign a term to a node.
 *
 * TODO: NOT TESTED FOR D6 - MAY STILL BE ONLY D5
 * NOTE: does not check whether the assignment is valid.
 *
 * @param $vocab
 *   The vocabulary ID.
 * @param $name
 *   The text name of the new term.
 */
function install_taxonomy_assign_nid_tid($nid, $tid) {
  if (!$tid || !$nid) {
    return;
  }
  db_query('DELETE FROM {term_node} WHERE nid = %d AND tid = %d', $nid, $tid);
  db_query('INSERT INTO {term_node} (nid, tid) VALUES (%d, %d)', $nid, $tid);
}

/**
 * Creates a series of taxonomy vocabularies and terms from the passed
 * definition.
 *
 * Sample Definition:
 *   array(
 *   0 => array(
 *     'name' => 'vocab name', 
 *     'content_types' => array(), 
 *     'properties' => array(),
 *     'terms' => array(
 *       0 => array(
 *         'name' => 'Term Name', 
 *         'desription' => '', 
 *         'properties' => array(),
 *       )
 *     ),
 *   ), 
 * );
 *
 * NOTE: This function does not currently do any error or duplicate checking.
 *
 * @param $definition
 *   Definintion of the taxonomy structure.
 */
function install_taxonomy_import($definition) {
  foreach ($definition as $vocab_index => $vocab_definition) {
    $vid = install_taxonomy_add_vocabulary($vocab_definition['name'], $vocab_definition['content_types'], $vocab_definition['properties']);
    foreach ($vocab_definition['terms'] as $term_index => $term_definition) {
      install_taxonomy_add_term($vid, $term_definition['name'], $term_definition['description'], $term_definition['properties']);
    }
  }
}
