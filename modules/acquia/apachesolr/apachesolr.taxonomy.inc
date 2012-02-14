<?php
// $Id: apachesolr.taxonomy.inc,v 1.1.2.6 2010/08/05 08:18:35 robertDouglass Exp $

/**
 * Overrides taxonomy/term/X links
 */
function apachesolr_search_taxonomy_term_page($str_tids = '', $depth = 0, $op = 'page') {
  drupal_add_feed(url('taxonomy/term/' . $str_tids . '/' . $depth . '/feed'), 'RSS - '. $title);

  $vids_to_override = apachesolr_get_enabled_facets('apachesolr_search');

  $terms = taxonomy_terms_parse_string($str_tids);
  $redirect_to_apachesolr = TRUE;

  // Only support one term, only page callbacks, and only depth = 0 (because of way Solr indexing works)
  if ((count($terms['tids']) > 1 && $terms['operator'] != 'and') || ($op != 'page') || ($depth != 0)) {
    $redirect_to_apachesolr = FALSE;
  }
  else {
    // Check if term belongs to vocabulary selected by admin as an available filter
    $term = taxonomy_get_term($terms['tids'][0]);
    $vocabulary_facet_name = 'im_vid_' . $term->vid;

    if (!in_array($vocabulary_facet_name, $vids_to_override)) {
      $redirect_to_apachesolr = FALSE;
    }
  }

  if ($redirect_to_apachesolr == FALSE) {
    // Fallback to normal taxonomy/term page
    require_once(drupal_get_path('module', 'taxonomy') .'/taxonomy.pages.inc');
    return taxonomy_term_page($str_tids, $depth, $op);
  }
  else {
    $filters = 'tid:' . array_pop($terms['tids']);
    $solrsort = variable_get('apachesolr_search_taxonomy_sort', 'created desc');
    $page = isset($_GET['page']) ? $_GET['page'] : 0;
    $_GET['retain-filters'] = 1;

    try {
      module_load_include('inc', 'search', 'search.pages');
      $data = apachesolr_search_execute('', $filters, $solrsort, 'search/apachesolr_search', $page);
      $results = theme('search_results', $data, 'apachesolr_search');
    }
    catch (Exception $e) {
      watchdog('Apache Solr', t('Error running search'));
    }

    // Set the title to the term name like taxonomy does.
    drupal_set_title(check_plain($term->name));

    if ($results) {
      $results = theme('box', t('Results for %term', array('%term' => $term->name)), $results);
    }
    else {
      $results = theme('box', t('Your search yielded no results'), search_help('search#noresults', drupal_help_arg()));
    }

    return drupal_get_form('search_form', NULL, '', 'apachesolr_search') . $results;
  }
}
