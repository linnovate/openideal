<?php

define('TAXONOMY_ACTION_ADD', 0);
define('TAXONOMY_ACTION_REPLACE', 1);
define('TAXONOMY_ACTION_DELETE', 2);
define('TAXONOMY_ACTION_REPLACE_VOCABULARY', 3);

function views_bulk_operations_taxonomy_action_info() {
  if (!module_exists('taxonomy')) return array();
  return array('views_bulk_operations_taxonomy_action' => array(
    'type' => 'node',
    'description' => t('Modify node taxonomy terms'),
    'configurable' => TRUE,
    'behavior' => array('changes_node_property'),
  ));
}

function views_bulk_operations_taxonomy_action(&$node, $context) {
  if (empty($context['terms']) && $context['do'] == TAXONOMY_ACTION_REPLACE) { // Just delete terms
    taxonomy_node_delete($node);
    $node->taxonomy = array();
    return;
  }

  // Operate on a terms array.
  if (isset($context['terms']['tags'])) {
    $tags = $context['terms']['tags'];
    unset($context['terms']['tags']);
  }
  $terms = array();
  foreach ($context['terms'] as $tid) {
    $terms[$tid] = taxonomy_get_term($tid);
  }
  switch ($context['do']) {
  case TAXONOMY_ACTION_DELETE:
    $existing_terms = taxonomy_node_get_terms($node);
    while (list($delete_tid) = each($terms)) {
      if (array_key_exists($delete_tid,$existing_terms)) {
        unset($existing_terms[$delete_tid]);
      }
    }
    $terms = $existing_terms;
    if (empty($terms)) {
      taxonomy_node_delete($node);
    }
    break;
  case TAXONOMY_ACTION_ADD:
    $existing_terms = taxonomy_node_get_terms($node);
    foreach ($terms as $add_tid => $term) {
      if (array_key_exists($add_tid, $existing_terms)) {
        unset($terms[$add_tid]);
      }
    }
    $terms = array_merge($terms, $existing_terms);
    break;
  case TAXONOMY_ACTION_REPLACE_VOCABULARY:
    $existing_terms = taxonomy_node_get_terms($node);
    foreach ($existing_terms as $existing_term) {
      foreach ($terms as $term) {
        if ($term->vid == $existing_term->vid) {
          unset($existing_terms[$existing_term->tid]);
        }
      }
    }
    $terms = array_merge($terms, $existing_terms);
    break;
  case TAXONOMY_ACTION_REPLACE:
    break;
  }

  // Reassemble a taxonomy array that can be saved.
  $taxonomy = array();
  if (!empty($terms)) foreach ($terms as $term) {
    if (!isset($taxonomy[$term->vid])) {
      $taxonomy[$term->vid] = $term->tid;
    }
    else if (is_array($taxonomy[$term->vid])) {
      $taxonomy[$term->vid][$term->tid] = $term->tid;
    }
    else {
      $previous_tid = $taxonomy[$term->vid];
      $taxonomy[$term->vid] = array(
        $previous_tid => $previous_tid,
        $term->tid => $term->tid
      );
    }
  }
  if (isset($tags) && $context['do'] != TAXONOMY_ACTION_DELETE) {
    $taxonomy['tags'] = $tags;
  }
  $node->taxonomy = $taxonomy;
}

function views_bulk_operations_taxonomy_action_form($context) {
  if (isset($context['selection']) && isset($context['view'])) {
    $vocabularies = array();
    $nids = array_map('_views_bulk_operations_get_oid', $context['selection'], array_fill(0, count($context['selection']), $context['view']->base_field));
    $result = db_query("SELECT DISTINCT v.vid FROM {vocabulary_node_types} v LEFT JOIN {node} n ON v.type = n.type WHERE n.nid IN (%s)", implode(',', $nids));
    while ($v = db_fetch_object($result)) {
      $vocabularies[$v->vid] = taxonomy_vocabulary_load($v->vid);
    }
  }
  else {
    $vocabularies = taxonomy_get_vocabularies();
  }
  if (empty($vocabularies)) {
    drupal_set_message(t('The selected nodes are not associated with any vocabulary. Please select other nodes and try again.'), 'error');
    return array();
  }
  $form['taxonomy'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vocabularies'),
    '#tree' => TRUE,
  );  
  while (list(,$vocabulary) = each($vocabularies)) {
    $form['taxonomy'][$vocabulary->vid] = taxonomy_form($vocabulary->vid, isset($context['terms']) ? $context['terms'] : NULL);
    $form['taxonomy'][$vocabulary->vid]['#weight'] = $vocabulary->weight;
    if ($vocabulary->tags) { // If tags, give the ability to add new terms
      if ($vocabulary->help) {
        $help = $vocabulary->help;
      }
      else {
        $help = t('A comma-separated list of terms describing this content. Example: funny, bungee jumping, "Company, Inc.".');
      }
      $help .= t('<br />Note that this field has no effect when deleting terms.');
      $form['taxonomy']['tags'][$vocabulary->vid] = array('#type' => 'textfield',
        '#title' => $vocabulary->name .' '. t('(new tags)'),
        '#autocomplete_path' => 'taxonomy/autocomplete/'. $vocabulary->vid,
        '#weight' => $vocabulary->weight,
        '#maxlength' => 1024,
        '#description' => $help,
        '#default_value' => isset($context['terms']['tags'][$vocabulary->vid]) ? $context['terms']['tags'][$vocabulary->vid] : '',
      );
    }
  }
  $form['do'] = array(
    '#type' => 'radios',
    '#title' => t('Action to take'),
    '#default_value' => isset($context['do']) ? $context['do'] : TAXONOMY_ACTION_ADD,
    '#options' => array(
      TAXONOMY_ACTION_ADD => t('Add the selected terms'),
      TAXONOMY_ACTION_REPLACE => t('Replace existing terms with selected ones'),
      TAXONOMY_ACTION_REPLACE_VOCABULARY => t('Replace terms within same vocabulary'),
      TAXONOMY_ACTION_DELETE => t('Delete selected terms')
    ),
    '#required' => TRUE,
    '#weight' => -2,
  );
  return $form;
}

/**
 * Take a multi-dimensional array of vocabularies -> selected terms and
 * return an array of only the terms selected
 */
function _taxonomy_action_parse_terms($taxonomy) {
  $ret = array();
  while (list($key,$vocab) = each($taxonomy)) {
    if ($key == 'tags') {
      $ret['tags'] = $vocab;
    }
    else if (!is_array($vocab)) {
      if (!empty($vocab)) $ret[] = $vocab;
    } 
    else {
      while (list(,$tid) = each($vocab)) {
        if (!empty($tid)) $ret[] = $tid;
      }
    }
  }
  return $ret;
}

function views_bulk_operations_taxonomy_action_submit($form, $form_state) {
  return array(
    'do' => $form_state['values']['do'], 
    'terms' => _taxonomy_action_parse_terms($form_state['values']['taxonomy']),
  );
}

function views_bulk_operations_taxonomy_action_validate($form, $form_state) {
  if ($form_state['values']['do'] != TAXONOMY_ACTION_REPLACE) {
    $terms = _taxonomy_action_parse_terms($form_state['values']['taxonomy']);
    if (empty($terms)) {
      form_set_error('terms', t('You did not select any term nor did you choose to replace existing terms. Please select one or more terms or choose to replace the terms.'));
    }
  }
}

