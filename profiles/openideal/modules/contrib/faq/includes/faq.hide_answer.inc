<?php

/**
 * @file
 * FAQ page callbacks for the "hide answer" layouts.
 */

/**
 * Create FAQ page if set to show/hide answer when question is clicked.
 *
 * @param &$variables
 *   Array reference of arguments given to the theme() function.
 */
function template_preprocess_faq_hide_answer(&$variables) {
  $data = $variables['data'];
  $this_page = $_GET['q'];

  // Fetch configuration.
  $teaser = variable_get('faq_use_teaser', FALSE);
  $links = variable_get('faq_show_node_links', FALSE);
  $disable_node_links = variable_get('faq_disable_node_links', FALSE);

  $nodes = array();
  $count = 0;
  foreach ($data as $node) {
    $anchor = 'n' . $node->nid;
    faq_view_question($nodes[$count], $node, $this_page, $anchor);
    faq_view_answer($nodes[$count], $node, array(), $teaser, $links);
    $count++;
  }
  $variables['use_teaser'] = $teaser;
  $variables['nodes'] = $nodes;
}

/**
 * Create categorized FAQ page if set to show answer when question is clicked.
 *
 * @param &$variables
 *   Array reference of arguments given to the theme() function.
 */
function template_preprocess_faq_category_hide_answer(&$variables) {
  $data = $variables['data'];
  $category_display = $variables['category_display'];
  $term = $variables['term'];
  $parent_term = $variables['parent_term'];
  $class = $variables['class'];

  // Fetch configuration.
  $teaser = variable_get('faq_use_teaser', FALSE);
  $variables['use_teaser'] = $teaser;
  $links = variable_get('faq_show_node_links', FALSE);
  $disable_node_links = variable_get('faq_disable_node_links', FALSE);
  $display_faq_count = variable_get('faq_count', FALSE);
  $hide_child_terms = variable_get('faq_hide_child_terms', FALSE);
  $show_term_page_children = variable_get('faq_show_term_page_children', FALSE);
  $default_sorting = variable_get('faq_default_sorting', 'DESC');

  // Initialise some variables.
  $default_weight = 0;
  if ($default_sorting != 'DESC') {
    $default_weight = 1000000;
  }
  $this_page = $_GET['q'];
  $get_child_terms = 0;
  // Check if we're on a faq page.
  if (arg(0) == 'faq') {
    // Check if we're on a categorized faq page.
    if (is_numeric(arg(1))) {
      $get_child_terms = arg(1);
    }
  }
  // Force some settings in case we're processing a special faq question list
  // created by a custom call to faq_page().
  elseif (!empty($parent_term)) {
    $get_child_terms = $parent_term->tid;
    $show_term_page_children = TRUE;
  }


  // Get number of questions, and account for hidden sub-categories.
  $count = 0;
  if ($display_faq_count && $hide_child_terms) {
    $count = faq_taxonomy_term_count_nodes($term->tid);
  }
  $variables['display_faq_count'] = $display_faq_count;

  // Get taxonomy image.
  $variables['term_image'] = '';
  if (module_exists('taxonomy_image')) {
    $variables['term_image'] = taxonomy_image_display($term->tid, array('class' => 'faq-tax-image'));
  }

  // Configure header.
  $variables['category_depth'] = $term->depth;
  $variables['category_name'] = check_plain(faq_tt("taxonomy:term:$term->tid:name", $term->name));
  if ($category_display == 'hide_qa') {
    $variables['header_title'] = l(faq_tt("taxonomy:term:$term->tid:name", $term->name), "faq/$term->tid");
  }
  else {
    $variables['header_title'] = check_plain(faq_tt("taxonomy:term:$term->tid:name", $term->name));
  }

  // Configure category description.
  $variables['description'] = check_markup(faq_tt("taxonomy:term:$term->tid:description", $term->description));

  // Get list of sub-categories if necessary.
  $child_categories = array();
  if (($show_term_page_children || $hide_child_terms) && $category_display == 'new_page') {
    $child_categories = faq_view_child_category_headers($term);
  }
  $variables['subcat_list'] = $child_categories;
  $variables['subcat_list_style'] = variable_get('faq_category_listing', 'ul');

  // Configure class (faq-qa or faq-qa-hide).
  if ($get_child_terms == $term->tid) {
    $variables['container_class'] = 'faq-qa';
  }
  else {
    $variables['container_class'] = $class;
  }

  // Configure sub-category bodies (theme recursively).
  $variables['subcat_body_list'] = array();
  if (($get_child_terms && $category_display == 'categories_inline') || ((($show_term_page_children && $this_page != 'faq') || $hide_child_terms) && $category_display == 'hide_qa')) {
    $variables['subcat_body_list'] = faq_get_child_categories_faqs($term, 'faq_category_hide_answer', $default_weight, $default_sorting, $category_display, $variables['class'], $parent_term);
  }

  if (!count($data)) {
    $variables['question_count'] = $count;
    $variables['nodes'] = array();
    return;
  }

  $nodes = array();
  foreach ($data as $node) {
    if (!$hide_child_terms) {
      $count++;
    }
    $node_var = array();
    $anchor = 't' . $term->tid . 'n' . $node->nid;
    faq_view_question($node_var, $node, $this_page, $anchor);
    faq_view_answer($node_var, $node, array(), $teaser, $links);
    $nodes[] = $node_var;
  }
  $variables['nodes'] = $nodes;
  $variables['question_count'] = $count;
}

