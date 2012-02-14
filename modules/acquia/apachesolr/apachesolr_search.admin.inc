<?php
// $Id: apachesolr_search.admin.inc,v 1.1.2.26 2010/06/16 20:43:39 jpmckinney Exp $

/**
 * @file
 *   Administrative settings for searching.
 */

/**
 * Menu callback - the settings form.
 */
function apachesolr_search_settings_page() {
  $output = '';
  // Try to fetch the schema fields.
  try {
    $solr = apachesolr_get_solr();
    $fields = $solr->getFields();
    $output .= drupal_get_form('apachesolr_search_settings_form', $fields);
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
    $output .= t('Cannot get information about the fields in the index at this time.');
  }
  return $output;
}

/**
 * Menu callback - boosts settings form.
 */
function apachesolr_boost_settings_page() {
  $output = drupal_get_form('apachesolr_search_bias_form');
  $output .= drupal_get_form('apachesolr_search_type_boost_form');
  return $output;
}

/**
 * Form builder function to set date, comment, etc biases.
 */
function apachesolr_search_bias_form($form_state) {

  $date_settings = variable_get('apachesolr_search_date_boost', '4:200.0');
  $comment_settings = variable_get('apachesolr_search_comment_boost', '0:0');
  $changed_settings = variable_get('apachesolr_search_changed_boost', '0:0');
  $sticky_boost = variable_get('apachesolr_search_sticky_boost', '0');
  $promote_boost = variable_get('apachesolr_search_promote_boost', '0');

  $options = array(
    '10:2000.0' => '10',
    '8:1000.0' => '9',
    '8:700.0' => '8',
    '8:500.0' => '7',
    '4:300.0' => '6',
    '4:200.0' => '5',
    '4:150.0' => '4',
    '2:150.0' => '3',
    '2:100.0' => '2',
    '1:100.0' => '1',
    '0:0' => t('Ignore'),
  );

  $weights = drupal_map_assoc(array('21.0', '13.0', '8.0', '5.0', '3.0', '2.0', '1.0', '0.8', '0.5', '0.3', '0.2', '0.1'));
  $weights['0'] = t('Ignore');

  $form['biasing'] = array(
    '#type' => 'fieldset',
    '#title' => t('Result biasing'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => t('Give bias to certain properties when ordering the search results. Any value except <em>Ignore</em> will increase the score of the given type in search results. Choose <em>Ignore</em> to ignore any given property.'),
  );
  $form['biasing']['apachesolr_search_sticky_boost'] = array(
    '#type' => 'select',
    '#options' => $weights,
    '#title' => t("Sticky at top of lists"),
    '#default_value' => $sticky_boost,
    '#description' => t("Select additional bias to give to nodes that are set to be 'Sticky at top of lists'."),
  );
  $form['biasing']['apachesolr_search_promote_boost'] = array(
    '#type' => 'select',
    '#options' => $weights,
    '#title' => t("Promoted to home page"),
    '#default_value' => $promote_boost,
    '#description' => t("Select additional bias to give to nodes that are set to be 'Promoted to home page'."),
  );
  $form['biasing']['apachesolr_search_date_boost'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t("More recently created"),
    '#default_value' => $date_settings,
    '#description' => t('This setting will change the result scoring so that nodes created more recently may appear before those with higher keyword matching.'),
  );
  $form['biasing']['apachesolr_search_comment_boost'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t("More comments"),
    '#default_value' => $comment_settings,
    '#description' => t('This setting will change the result scoring so that nodes with more comments may appear before those with higher keyword matching.'),
  );
  $form['biasing']['apachesolr_search_changed_boost'] = array(
    '#type' => 'select',
    '#options' => $options,
    '#title' => t("More recent comments"),
    '#default_value' => $changed_settings,
    '#description' => t('This setting will change the result scoring so that nodes with the most recent comments (or most recent updates to the node itself) may appear before those with higher keyword matching.'),
  );
  $form = system_settings_form($form);
  $form['biasing']['buttons'] = $form['buttons'];
  unset($form['buttons']);
  return $form;
}

/**
 * Form builder function to set query field weights.
 */
function apachesolr_search_settings_form($form_state, $fields) {
  $form = array();

  // get the current weights
  $qf = variable_get('apachesolr_search_query_fields', array());
  $weights = drupal_map_assoc(array('21.0', '13.0', '8.0', '5.0', '3.0', '2.0', '1.0', '0.8', '0.5', '0.3', '0.2', '0.1'));
  $weights['0'] = t('Omit');
  // Note - we have default values set in solrconfig.xml, which will operate
  // when none are set.  That's the preferred state.
  $defaults = array(
    'body' => '1.0',
    'title' => '5.0',
    'name' => '3.0',
    'taxonomy_names' => '2.0',
    'tags_h1' => '5.0',
    'tags_h2_h3' => '3.0',
    'tags_h4_h5_h6' => '2.0',
    'tags_inline' => '1.0',
    'tags_a' => '0',
  );
  if (!$qf) {
    $qf = $defaults;
  }
  if ($fields) {
    $form['apachesolr_search_query_fields'] = array(
      '#type' => 'fieldset',
      '#title' => t('Field biases'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#tree' => TRUE,
      '#description' => t('Specify here which fields are more important when searching. Give a field a greater numeric value to make it more important. If you omit a field, it will not be searched.'),
    );
    foreach ($fields as $field_name => $field) {
      // Only indexed feids are searchable.
      if ($field->schema{0} == 'I') {
        // By default we only show text fields.  Use hook_form_alter to change.
        $form['apachesolr_search_query_fields'][$field_name] = array(
          '#access' => $field->type == 'text',
          '#type' => 'select',
          '#options' => $weights,
          '#title' => apachesolr_field_name_map($field_name),
          '#default_value' => isset($qf[$field_name]) ? $qf[$field_name] : '0',
        );
      }
    }
    // Make sure all the default fields are included, even if they have
    // no indexed content.
    foreach ($defaults as $field_name => $weight) {
      $form['apachesolr_search_query_fields'][$field_name] = array(
        '#type' => 'select',
        '#options' => $weights,
        '#title' => apachesolr_field_name_map($field_name),
        '#default_value' => isset($qf[$field_name]) ? $qf[$field_name] : $defaults[$field_name],
      );
    }

    ksort($form['apachesolr_search_query_fields']);
  }

  return system_settings_form($form);
}

/**
 * Form builder function to set query type weights.
 */
function apachesolr_search_type_boost_form($form_state) {
  $form = array();

  $form['apachesolr_search_type_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Type biasing and exclusion'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['apachesolr_search_type_settings']['apachesolr_search_type_boosts'] = array(
    '#type' => 'item',
    '#description' => t("Specify here which node types should get a higher relevancy score in searches. Any value except <em>Ignore</em> will increase the score of the given type in search results."),
    '#tree' => TRUE,
  );

  $weights = drupal_map_assoc(array('21.0', '13.0', '8.0', '5.0', '3.0', '2.0', '1.0', '0.8', '0.5', '0.3', '0.2', '0.1'));
  $weights['0'] = t('Ignore');

  // Get the current boost values.
  $type_boosts = variable_get('apachesolr_search_type_boosts', array());
  $names = node_get_types('names');

  foreach ($names as $type => $name) {
    $form['apachesolr_search_type_settings']['apachesolr_search_type_boosts'][$type] = array(
      '#type' => 'select',
      '#title' => t('%type type content bias', array('%type' => $name)),
      '#options' => $weights,
      '#default_value' => isset($type_boosts[$type]) ? $type_boosts[$type] : 0,
    );
  }

  $form['apachesolr_search_type_settings']['apachesolr_search_excluded_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Types to exclude from the search index'),
    '#options' => $names,
    '#default_value' => variable_get('apachesolr_search_excluded_types', array()),
    '#description' => t("Specify here which node types should be totally excluded from the search index. Content excluded from the index will never appear in any search results."),
  );

  $form['#submit'][] = 'apachesolr_search_type_boost_form_submit';
  $form = system_settings_form($form);
  $form['apachesolr_search_type_settings']['buttons'] = $form['buttons'];
  unset($form['buttons']);
  return $form;
}

/**
 * Submit callback for apachesolr_search_type_boost_form().
 *
 * This is called before system_settings_form_submit().
 */
function apachesolr_search_type_boost_form_submit($form_id, &$form_state) {
  $old_excluded_types = variable_get('apachesolr_search_excluded_types', array());
  $new_excluded_types = $form_state['values']['apachesolr_search_excluded_types'];
  // Check whether we are resetting the values.
  if ($form_state['clicked_button']['#value'] == t('Reset to defaults')) {
    $new_excluded_types = array();
  }

  foreach ($new_excluded_types as $type => $excluded) {
    // Remove newly omitted node types.
    // Note - we omit a check on empty($old_excluded_types[$type]) so that
    // the admin can re-submit this page if the delete operation fails.
    if (!empty($new_excluded_types[$type])) {
      try {
        $solr = apachesolr_get_solr();
        $solr->deleteByQuery("type:$type");
        apachesolr_index_updated(time());
      }
      catch (Exception $e) {
        watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
        drupal_set_message(t('The Apache Solr search engine is not available. Please contact your site administrator.'), 'error');
      }
    }
  }

  foreach ($old_excluded_types as $type => $excluded) {
    // Set no longer omitted node types for reindexing.
    if (empty($new_excluded_types[$type]) && !empty($old_excluded_types[$type])) {
      db_query("UPDATE {apachesolr_search_node} SET changed = %d WHERE nid IN (SELECT nid FROM {node} WHERE type = '%s')", time(), $type);
    }
  }
}
