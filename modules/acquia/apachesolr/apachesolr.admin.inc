<?php
// $Id: apachesolr.admin.inc,v 1.1.2.53 2010/06/28 01:59:56 pwolanin Exp $

/**
 * @file
 *   Administrative pages for the Apache Solr framework.
 */

function apachesolr_settings() {
  $form = array();

  // Perform a check to ensure the server is there
  if (empty($_POST)) {
    // update_help() loads install.inc via include_once, so not much
    // point in working around it.
    include_once './includes/install.inc';
    module_load_include('install', 'apachesolr');

    foreach (apachesolr_requirements('runtime') as $requirement) {
      $status = $requirement['severity'] == REQUIREMENT_ERROR ? 'error' : 'status';
      drupal_set_message($requirement['title'] . ': ' . $requirement['value'], $status);
    }
  }

  $form['apachesolr_host'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr host name'),
    '#default_value' => variable_get('apachesolr_host', 'localhost'),
    '#description' => t('Host name of your Solr server, e.g. <code>localhost</code> or <code>example.com</code>.'),
    '#required' => TRUE,
  );
  $form['apachesolr_port'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr port'),
    '#default_value' => variable_get('apachesolr_port', '8983'),
    '#description' => t('Port on which the Solr server listens. The Jetty example server is 8983, while Tomcat is 8080 by default.'),
    '#required' => TRUE,
  );
  $form['apachesolr_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Solr path'),
    '#default_value' => variable_get('apachesolr_path', '/solr'),
    '#description' => t('Path that identifies the Solr request handler to be used.'),
  );

  $numbers = drupal_map_assoc(array(1, 5, 10, 20, 50, 100, 200));
  $form['apachesolr_cron_limit'] = array(
    '#type' => 'select',
    '#title' => t('Number of items to index per cron run'),
    '#default_value' => variable_get('apachesolr_cron_limit', 50),
    '#options' => $numbers,
    '#description' => t('The maximum number of items indexed in each pass of a <a href="@cron">cron maintenance task</a>. If necessary, reduce the number of items to prevent timeouts and memory errors while indexing.', array('@cron' => url('admin/reports/status')))
  );

  $options = drupal_map_assoc(array(5, 10, 15, 20, 25, 30, 40, 50, 60, 70, 80, 90, 100));
  $form['apachesolr_rows'] = array(
    '#type' => 'select',
    '#title' => t('Results per page'),
    '#default_value' => variable_get('apachesolr_rows', 10),
    '#options' => $options,
    '#description' => t('The number of results that will be shown per page.'),
  );
  $form['apachesolr_failure'] = array(
    '#type' => 'select',
    '#title' => t('On failure'),
    '#options' => array('show_error' => t('Show error message'),
      'show_drupal_results' => t('Show core Drupal results'),
      'show_no_results' => t('Show no results')
    ),
    '#default_value' => variable_get('apachesolr_failure', 'show_error'),
    '#description' => t('What to display if Apache Solr search is not available.'),
  );
  // Add a link to add more mlt blocks.
  $form['mlt_link'] = array(
    '#type' => 'item',
    '#value' => l(t('Add a new content recommendation block'), 'admin/settings/apachesolr/mlt/add_block'),
    '#description' => format_plural(count(apachesolr_mlt_list_blocks()), 'You currently have 1 block.', 'You currenly have @count blocks.'),
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced configuration'),
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  $form['advanced']['apachesolr_set_nodeapi_messages'] = array(
    '#type' => 'radios',
    '#title' => t('Extra help messages for administrators'),
    '#default_value' => variable_get('apachesolr_set_nodeapi_messages', 1),
    '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
  );
  $form['advanced']['apachesolr_read_only'] = array(
    '#type' => 'radios',
    '#title' => t('Index write access'),
    '#default_value' => variable_get('apachesolr_read_only', APACHESOLR_READ_WRITE),
    '#options' => array(APACHESOLR_READ_WRITE => t('Read and write (normal)'), APACHESOLR_READ_ONLY => t('Read only')),
    '#description' => t('<em>Read only</em> stops this site from sending updates to your search index. Useful for development sites.'),
  );
  return system_settings_form($form);
}

/**
 * Validation function for the apachesolr_settings form.
 */
function apachesolr_settings_validate($form, &$form_state) {
  if (isset($form['apachesolr_port'])) {
    $port = $form_state['values']['apachesolr_port'];
    // TODO: Port range should be 0-65535, but 0 crashes apachesolr
    if (!ctype_digit($port) || $port < 1 || $port > 65535) {
      form_set_error('apachesolr_port', t('The port has to be an integer between 1 and 65535.'));
    }
  }
}

/**
 * Gets information about the fields already in solr index.
 */
function apachesolr_index_page() {
  $data = (object) array();
  try {
    $solr = apachesolr_get_solr();
    // TODO: possibly clear this every page view if we are running multi-site.
    if (apachesolr_index_updated()) {
      $solr->clearCache();
    }
    $data = $solr->getLuke();
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
    $data->fields = array();
  }

  $output = '';
  if (isset($data->index->numDocs)) {
    $pending_docs = $delay_msg = $delete_msg = '';
    try {
      $stats_summary = $solr->getStatsSummary();
      $solr_msg = '<p>' . t('Using schema.xml version: <strong>@schema_version</strong>', $stats_summary);
      if ($stats_summary['@core_name']) {
        $solr_msg .= '<br />' . t('Solr core name: <strong>@core_name</strong>', $stats_summary);
      }
      $delay_msg = '<br />' . t('<em>The server has a @autocommit_time delay before updates are processed.</em>', $stats_summary) . "</p>\n";
      $delete_msg = '<p>' . t('Number of pending deletions: @deletes_total', $stats_summary) . "</p>\n";
    }
    catch (Exception $e) {
      watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    }
    $output .= $solr_msg . $delay_msg;
    $pending_msg = $stats_summary['@pending_docs'] ? t('(@pending_docs sent but not yet processed)', $stats_summary) : '';
    $output .= '<p>' . t('Number of documents in index: @num !pending', array('@num' => $data->index->numDocs, '!pending' => $pending_msg)) . "</p>\n";
    $output .= $delete_msg;
  }
  $output .= '<p>' . l(t('View more details on the search index contents'), 'admin/reports/apachesolr') . "</p>\n";
  if (variable_get('apachesolr_read_only', APACHESOLR_READ_WRITE) == APACHESOLR_READ_WRITE) {
  // Display the Delete Index form.
  $output .= drupal_get_form('apachesolr_delete_index_form');
  }
  else {
    drupal_set_message(t('The index is in read-only mode. Options for deleting and re-indexing are therefore not available. The index mode can be changed on the !settings_page', array('!settings_page' => l(t('settings page'), 'admin/settings/apachesolr'))), 'warning');
  }

  return $output;
}

function apachesolr_index_report() {
  try {
    $solr = apachesolr_get_solr();
    // TODO: possibly clear this every page view if we are running multi-site.
    if (apachesolr_index_updated()) {
      $solr->clearCache();
    }
    $data = $solr->getLuke();
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
    $data->fields = array();
  }

  $output = '<p>' . t('Number of documents in index: @num !pending', array('@num' => $data->index->numDocs, '!pending' => '')) . "</p>\n";

  $limit = variable_get('apachesolr_luke_limit', 20000);
  if (isset($data->index->numDocs) && $data->index->numDocs > $limit) {
    $output .= '<p>' . t('You have more than @limit documents, so term frequencies are being omitted for performance reasons.', array('@limit' => $limit)) . "</p>\n";
    $not_found = t('<em>Omitted</em>');
  }
  elseif (isset($data->index->numDocs)) {
    $not_found = t('Not indexed');
    try {
      $solr = apachesolr_get_solr();
      // Note: we use 2 since 1 fails on Ubuntu Hardy.
      $data = $solr->getLuke(2);
      $output .= '<p>' . t('Number of terms in index: @num', array('@num' => $data->index->numTerms)) . "</p>\n";
    }
    catch (Exception $e) {
      watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
      $data->fields = array();
    }
  }

  $fields = (array)$data->fields;
  if ($fields) {
    $output .= '<p>' . t('Number of fields in index: @num', array('@num' => count($fields))) . "</p>\n";
    $rows = array();
    foreach ($fields as $name => $field) {
      // TODO: try to map the name to something more meaningful.
      $rows[$name] = array($name, $field->type, isset($field->distinct) ? $field->distinct : $not_found);
    }
    ksort($rows);
    // Display the table of Field names, Index Types, and term counts.
    $output .= theme('table', array(t('Field name'), t('Index type'), t('Distinct terms')), $rows);
  }
  else {
    $output .= '<p>' . t('No data on indexed fields.') . "</p>\n";
  }
  return $output;
}

/**
 * Indicates what order the specified facets should be listed in.  This function is used in a usort
 * invocation.
 * @param $a
 *   The first facet.
 * @param $b
 *   The second facet.
 * @return
 *   A signed integer that indicates which of the specified facets should come first.
 */
function _apachesolr_sort_facets($a, $b) {
  return strcasecmp($a['info'], $b['info']);
}

/**
 * This is the submit handler for the active facets form.
 *
 * The form values for each module are array filtereed to remove non-enabled items and
 * stored in the variable table with the name 'apachesolr_enabled_facets'.
 *
 * @see apachesolr_enabled_facets_form()
 */
function apachesolr_enabled_facets_form_submit($form, &$form_state) {
  $enabled = array();
  foreach ($form_state['values']['apachesolr_enabled_facets'] as $module => $facets) {
    $enabled[$module] = array_filter($facets);
  }
  apachesolr_save_enabled_facets($enabled);
  // This cache being stale can prevent new facet filters from working.
  apachesolr_clear_cache();
  drupal_set_message($form_state['values']['submit_message'], 'warning');
}

/**
 * Creates the form that allows the user to select which facets will be enabled.
 *
 * Only enabled facets are sent to solr.  Fewer enabled facets can reduce the
 * load on the search server.  Blocks are only offered for enabled facets, so
 * this also reduces the clutter on the blocks admin page.
 */
function apachesolr_enabled_facets_form() {
  $facets = array();
  $module_facets = array();
  $module_list = array();
  foreach (module_implements('apachesolr_facets') as $module) {
    $module_facets[$module] = module_invoke($module, 'apachesolr_facets');
    uasort($module_facets[$module], '_apachesolr_sort_facets');
    $module_list[$module] = $module;
  }

  $enabled_facets = apachesolr_get_enabled_facets();
  $form = array();
  $form['apachesolr_enabled_facets']['help'] = array(
    '#type' => 'item',
    '#value' => t('You can use this screen to select which search filter blocks should be created by enabling the corresponding filters on this page. For performance reasons, you should only enable filters that you intend to have available to users on the search page.  After selecting which filter blocks to create, you will be sent to the blocks page where you can choose which of those blocks should be enabled when your users search by placing each block in a region.'),
  );
  if ($module_list) {
    $placeholders = implode(', ', array_fill(0, count($module_list), "'%s'"));
    $result = db_query("SELECT name, info FROM {system} WHERE name IN (". $placeholders .") AND type = 'module'", $module_list);
    while ($item = db_fetch_array($result)) {
      $module_list[$item['name']] = unserialize($item['info']);
    }
  }
  foreach ($module_facets as $module => $facets) {
    $form['apachesolr_enabled_facets'][$module] = array(
      '#type' => 'fieldset',
      '#title' => check_plain($module_list[$module]['name']),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    // We must use module + delta as the keys since that combination is
    // guaranteed to be unique.  A single module could, for example, have
    // two different blocks that expose different faceting on the same
    // field in the index.
    foreach ($facets as $delta => $data) {
      $form['apachesolr_enabled_facets'][$module][$delta] = array(
        '#type' => 'checkbox',
        '#title' => $data['info'],
        '#return_value' => $data['facet_field'],
        '#default_value' => isset($enabled_facets[$module][$delta]) ? $data['facet_field'] : 0,
      );
    }
  }

  $has_facets = (bool)$module_facets;

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#access' => $has_facets,
  );
  $form['no-facets-message'] = array(
    '#value' => t('<em>No filters are available from your currently enabled modules</em>'),
    '#access' => !$has_facets,
  );

  $form['#tree'] = TRUE;
  $form['submit_message'] = array(
    '#type' => 'value',
    '#value' => t('The Apache Solr filters settings were changed.  To arrange the blocks for your enabled filters, visit the <a href="@url">blocks administration page</a>.', array('@url' => url('admin/build/block'))),
  );

  return $form;
}

/**
 * Create a form for deleting the contents of the Solr index.
 */
function apachesolr_delete_index_form() {
  $form = array();
  $form['markup'] = array(
    '#prefix' => '<h3>',
    '#value' => t('Index controls'),
    '#suffix' => '</h3>',
  );
  $form['reindex'] = array(
    '#type' => 'submit',
    '#value' => t('Re-index all content'),
    '#submit' => array('apachesolr_clear_index_submit'),
  );
  $form['reindex-desc'] = array(
    '#type' => 'item',
    '#description' => t('Re-indexing will add all content to the index again (overwriting the index), but existing content in the index will remain searchable.'),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Delete the index'),
    '#submit' => array('apachesolr_delete_index_submit'),
  );
  $form['delete-desc'] = array(
    '#type' => 'item',
    '#description' => t('Deletes all of the documents in the Solr index. This is rarely necessary unless your index is corrupt or you have installed a new schema.xml.'),
  );

  return $form;
}

/**
 * Submit function for the "Re-index all content" button.
 *
 * @see apachesolr_delete_index_form()
 */
function apachesolr_clear_index($form, &$form_state) {
  return apachesolr_comfirm_clear_index($form, $form_state);
}

/**
 * Submit function for the "Re-index all content" button
 * @see apachesolr_delete_index_form()
 *
 */
function apachesolr_clear_index_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/settings/apachesolr/index/clear/confirm';
}

/**
 * Confirmation form for "Re-index all content" button
 * @see apachesolr_clear_index_submit()
 */
function apachesolr_clear_index_confirm() {
  $form = array();
  return confirm_form($form, t('Are you sure you want to re-index?'), 'admin/settings/apachesolr/index', NULL, t('Re-index'), t('Cancel'));
}

/**
 * Submit function for the "Re-index all content" confirmation form.
 *
 * @see apachesolr_clear_index_confirm()
 */
function apachesolr_clear_index_confirm_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/settings/apachesolr/index';
  apachesolr_rebuild_index_table();
}


/**
 * Submit function for the "Delete the index" button
 * @see apachesolr_delete_index_form()
 *
 */
function apachesolr_delete_index_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/settings/apachesolr/index/delete/confirm';
}

/**
 * Confirmation form for "Delete the index" button
 * @see apachesolr_delete_index_submit()
 */
function apachesolr_delete_index_confirm() {
  $form = array();
  return confirm_form($form, t('Are you sure you want to delete your search index and start re-indexing?'), 'admin/settings/apachesolr/index', NULL, t('Delete'), t('Cancel'));
}

/**
 * Submit function for the "Delete the index" confirmation form.
 *
 * @see apachesolr_delete_index_confirm()
 */
function apachesolr_delete_index_confirm_submit($form, &$form_state) {
  $form_state['redirect'] = 'admin/settings/apachesolr/index';
  try {
    apachesolr_delete_index();
    // This form can't be seen by anyone without 'administer search'
    // permission, so no need to check perms before displaying a run-cron link.
    drupal_set_message(t('The Apache Solr content index has been erased. You must now !run_cron until your entire site has been re-indexed.', array('!run_cron' => l(t('run cron'), 'admin/reports/status/run-cron', array('query' => array('destination' => 'admin/settings/apachesolr/index'))))));

  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
  }
}

/**
 * Utility function to delete the index and reset all index counters.
 *
 * @param $type
 *   a single content type to be deleted from the index.
 *
 * @throws Exception
 */
function apachesolr_delete_index($type = NULL) {
  // Instantiate a new Solr object.
  $solr = apachesolr_get_solr();
  if ($type) {
    $query = 'type:' . $type;
  }
  else {
    $query = '*:*';
  }
  // Allow other modules to modify the delete query.
  // For example, use the site hash so that you only delete this site's
  // content:  $query = 'hash:' . apachesolr_site_hash()
  drupal_alter('apachesolr_delete_index', $query);
  $solr->deleteByQuery($query);
  $solr->commit();
  // Rebuild our node-tracking table.
  apachesolr_rebuild_index_table();
  apachesolr_index_updated(time());
}


/**
 * Try to map a schema field name to a human-readable description.
 */
function _apachesolr_field_name_map($field_name) {
  static $map;

  if (!isset($map)) {
    $map = array(
      'body' => t('Body text - the full, rendered content'),
      'title' => t('Title'),
      'teaser' => t('Teaser'),
      'name' => t('Author name'),
      'path_alias' => t('Path alias'),
      'taxonomy_names' => t('All taxonomy term names'),
      'tags_h1' => t('Body text inside H1 tags'),
      'tags_h2_h3' => t('Body text inside H2 or H3 tags'),
      'tags_h4_h5_h6' => t('Body text inside H4, H5, or H6 tags'),
      'tags_inline' => t('Body text in inline tags like EM or STRONG'),
      'tags_a' => t('Body text inside links (A tags)'),
      'tid' => t('Taxonomy term IDs'),
    );
    if (module_exists('taxonomy')) {
      foreach (taxonomy_get_vocabularies() as $vocab) {
        $map['ts_vid_'. $vocab->vid .'_names'] = t('Taxonomy term names only from the %name vocabulary', array('%name' => $vocab->name));
        $map['im_vid_'. $vocab->vid] = t('Taxonomy term IDs from the %name vocabulary', array('%name' => $vocab->name));
      }
    }
    foreach (apachesolr_cck_fields() as $name => $field) {
      $map[apachesolr_index_key($field)] = t('CCK @type field %label', array('@type' => $field['index_type'], '%name' => $field['label']));
    }
    drupal_alter('apachesolr_field_name_map', $map);
  }
  return isset($map[$field_name]) ? $map[$field_name] : $field_name;
}

/**
 * MoreLikeThis administration and utility functions.
 */

function apachesolr_mlt_add_block_form() {
  $form = apachesolr_mlt_block_form();
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
    '#weight' => '5',
  );
  return $form;
}

function apachesolr_mlt_add_block_form_submit($form, &$form_state) {
  apachesolr_mlt_save_block($form_state['values']);
  drupal_set_message('New content recommendation block created.  Drag it into a region to enable it');
  $form_state['redirect'] = 'admin/build/block';
}

/**
 * Form to edit moreLikeThis block settings.
 *
 * @param int $delta If editing, the id of the block to edit.
 *
 * @return array The form used for editing.
 * TODO:
 *   Add term boost settings.
 *   Enable the user to specify a query, rather then forcing suggestions based
 *     on the node id.
 *
 */
function apachesolr_mlt_block_form($delta = NULL) {
  if (isset($delta)) {
    $block = apachesolr_mlt_load_block($delta);
    if (!$block) {
      return array();
    }
  }
  else {
    $block = apachesolr_mlt_block_defaults();
  }

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Block name'),
    '#description' => t('The block name displayed to site users.'),
    '#required' => TRUE,
    '#default_value' => $block['name'],
    '#weight' => '-2',
  );
  $form['num_results'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of related items to display'),
    '#default_value' => $block['num_results'],
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15)),
    '#weight' => -1,
    );
  $form['mlt_fl'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Fields for finding related content'),
    '#description' => t('Choose the fields to be used in calculating similarity. The default combination of %taxonomy_names and %title will provide relevant results for typical sites.', array("%taxonomy_names" => _apachesolr_field_name_map("taxonomy_names"), "%title" => _apachesolr_field_name_map("title"))),
    '#options' => apachesolr_mlt_get_fields(),
    '#required' => TRUE,
    '#default_value' => $block['mlt_fl'],
  );
  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced configuration'),
    '#weight' => '1',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $options = drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7));
  $form['advanced']['mlt_mintf'] = array(
    '#type' => 'select',
    '#title' => t('Minimum term frequency'),
    '#description' => t('A word must appear this many times in any given document before the document is considered relevant for comparison.'),
    '#default_value' => $block['mlt_mintf'],
    '#options' => $options,
  );
  $form['advanced']['mlt_mindf'] = array(
    '#type' => 'select',
    '#title' => t('Minimum document frequency'),
    '#description' => t('A word must occur in at least this many documents before it will be used for similarity comparison.'),
    '#default_value' => $block['mlt_mindf'],
    '#options' => $options,
  );
  $form['advanced']['mlt_minwl'] = array(
    '#type' => 'select',
    '#title' => t('Minimum word length'),
    '#description' => 'You can use this to eliminate short words such as "the" and "it" from similarity comparisons. Words must be at least this number of characters or they will be ignored.',
    '#default_value' => $block['mlt_minwl'],
    '#options' => $options,
  );
  $form['advanced']['mlt_maxwl'] = array(
    '#type' => 'select',
    '#title' => t('Maximum word length'),
    '#description' => t('You can use this to eliminate very long words from similarity comparisons. Words of more than this number of characters will be ignored.'),
    '#default_value' => $block['mlt_maxwl'],
    '#options' => drupal_map_assoc(array(8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20)),
  );
  $form['advanced']['mlt_maxqt'] = array(
    '#type' => 'select',
    '#title' => t('Maximum number of query terms'),
    '#description' => t('The maximum number of query terms that will be included in any query. Lower numbers will result in fewer recommendations but will get results faster. If a content recommendation is not returning any recommendations, you can either check more "Comparison fields" checkboxes or increase the maximum number of query terms here.'),
    '#options' => drupal_map_assoc(array(3, 5, 7, 10, 12, 15, 20, 25, 30, 35, 40)),
    '#default_value' => $block['mlt_maxqt'],
  );

  return $form;
}

/**
 * Merge supplied settings with the standard defaults..
 */
function apachesolr_mlt_block_defaults($block = array()) {
  return $block + array(
    'name' => '',
    'num_results' => '5',
    'mlt_fl' => array(
      'title' => 'title',
      'taxonomy_names' => 'taxonomy_names',
    ),
    'mlt_mintf' => '1',
    'mlt_mindf' => '1',
    'mlt_minwl' => '3',
    'mlt_maxwl' => '15',
    'mlt_maxqt' => '20',
  );
}

/**
 * Constructs a list of field names used on the settings form.
 *
 * @return array An array containing a the fields in the solr instance.
 */
function apachesolr_mlt_get_fields() {
  $solr = apachesolr_get_solr();
  $fields = $solr->getFields();
  $rows = array();
  foreach ($fields as $field_name => $field) {
    if ($field->schema{4} == 'V') {
      $rows[$field_name] = _apachesolr_field_name_map($field_name);
    }
  }
  ksort($rows);
  return $rows;
}

/**
 * A helper function to save MLT block data.
 *
 * If passed a block delta, the function will update block settings. If it is
 * not passed a block delta, the function will create a new block.
 *
 * @param array $block_settings An array containing the settings required to form
 * a moreLikeThis request.
 *
 * @param int $delta The id of the block you wish to update.
 */
function apachesolr_mlt_save_block($block_settings = array(), $delta = NULL) {
  $blocks = variable_get('apachesolr_mlt_blocks', array());
  if (is_null($delta)) {
    $count = 0;
    ksort($blocks);
    // Construct a new array key.
    if (end($blocks)) {
      list(, $count) = explode('-', key($blocks));
    }
    $delta = sprintf('mlt-%03d', 1 + $count);
  }
  $defaults = apachesolr_mlt_block_defaults();
  // Remove stray form values.
  $blocks[$delta] = array_intersect_key($block_settings, $defaults) + $defaults;
  // Eliminate non-selected fields.
  $blocks[$delta]['mlt_fl'] = array_filter($blocks[$delta]['mlt_fl']);
  variable_set('apachesolr_mlt_blocks', $blocks);
}

function apachesolr_mlt_delete_block_form($form_state, $delta) {
  if ($block = apachesolr_mlt_load_block($delta)) {
    $form['delta'] = array(
      '#type' => 'value',
      '#value' => $delta
    );

    return confirm_form($form,
      t('Are you sure you want to delete the Apache Solr content recommendation block %name?', array('%name' => $block['name'])),
      'admin/build/block',
      t('The block will be deleted. This action cannot be undone.'),
      t('Delete'), t('Cancel'));
  }
}

function apachesolr_mlt_delete_block_form_submit($form, &$form_state) {
  $blocks = variable_get('apachesolr_mlt_blocks', array());
  unset($blocks[$form_state['values']['delta']]);
  variable_set('apachesolr_mlt_blocks', $blocks);
  drupal_set_message(t('The block has been deleted.'));
  $form_state['redirect'] = 'admin/build/block';
}

/**
 * Page callback to show available conf files.
 */
function apachesolr_config_files_overview() {
  $output = '';
  $xml = NULL;
  try {
    $solr = apachesolr_get_solr();
    $response = $solr->makeServletRequest('admin/file');
    $xml = simplexml_load_string($response->getRawResponse());
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
  }
  if ($xml) {
    $files = $xml->xpath('//lst[@name="files"]/lst');
    $items = array();
    foreach ($files as $file) {
      $atr = $file->attributes();
      $name = (string) $atr[0];
      $str = l($name, 'admin/reports/apachesolr/conf/'. $name);
      $str .= '<br />' . format_date(strtotime((string) $file->date));
      $str .= '<br />' . t('Size (bytes): @bytes', array('@bytes' => (string) $file->long));
      $items[$name] = $str;
    }
    ksort($items);
    $output .= theme('item_list', array_values($items));
  }
  return $output;
}

/**
 * Page callback to show one conf file.
 */
function apachesolr_config_file($name) {
  $output = '';
  try {
    $solr = apachesolr_get_solr();
    $response = $solr->makeServletRequest('admin/file', array('file' => $name));
    $raw_file = $response->getRawResponse();
    
    $output = '<pre>' . check_plain($raw_file) . '</pre>';
    drupal_set_title(check_plain($name));
  }
  catch (Exception $e) {
    watchdog('Apache Solr', nl2br(check_plain($e->getMessage())), NULL, WATCHDOG_ERROR);
    drupal_set_message(nl2br(check_plain($e->getMessage())), "warning");
  }
  return $output;
}

