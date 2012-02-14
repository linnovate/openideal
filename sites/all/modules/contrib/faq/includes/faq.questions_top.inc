<?php
// $Id: faq.questions_top.inc,v 1.1.2.9 2010/09/15 12:28:09 snpower Exp $

/**
 * @file
 * FAQ page callbacks for the "questions top" layouts.
 */

/**
 * Create the structure of the page, when the questions are to be shown on top.
 *
 * @param &$variables
 *   Array reference of arguments given to the theme() function.
 * @return
 *   A variable holding the HTML formatted page.
 */
function template_preprocess_faq_questions_top(&$variables) {
  $data = $variables['data'];

  // Fetch configuration.
  $teaser = variable_get('faq_use_teaser', FALSE);
  $links = variable_get('faq_show_node_links', FALSE);
  $disable_node_links = variable_get('faq_disable_node_links', FALSE);

  // Configure labels.
  $variables['question_label'] = '';
  $variables['answer_label'] = '';
  if (variable_get('faq_qa_mark', FALSE)) {
    $variables['question_label'] = check_plain(variable_get('faq_question_label', "Q:"));
    $variables['answer_label'] = check_plain(variable_get('faq_answer_label', "A:"));
  }

  // Configure "back to top" link.
  $this_page = $_GET['q'];
  $back_to_top = faq_init_back_to_top($this_page);

  // Loop through results.
  $questions = array();
  $answers = array();
  $key = 0;
  foreach ($data as $node) {
    $anchor = 'n' . $node->nid;
    $questions[$key] = l($node->title, $this_page, array('fragment' => $anchor));
    faq_view_question($answers[$key], $node, '', $anchor);
    faq_view_answer($answers[$key], $node, $back_to_top, $teaser, $links);
    $key++;
  }
  $variables['limit'] = $key;

  $list_style = variable_get('faq_question_listing', 'ul');
  $variables['list_style'] = $list_style;
  $variables['use_teaser'] = $teaser;
  $variables['questions'] = $questions;
  $variables['answers'] = $answers;
  $variables['questions_list'] = theme('item_list', $questions, NULL, $list_style, array("class" => "faq-ul-questions-top"));
}

/**
 * Create categorized questions for FAQ page if set to show questions on top.
 *
 * @param &$variables
 *   Array reference of arguments given to the theme() function.
 */
function template_preprocess_faq_category_questions_top(&$variables) {
  $data = $variables['data'];
  $category_display = $variables['category_display'];
  $term = $variables['term'];
  $parent_term = $variables['parent_term'];
  $class = $variables['class'];

  // Fetch configuration.
  $teaser = variable_get('faq_use_teaser', FALSE);
  $links = variable_get('faq_show_node_links', FALSE);
  $disable_node_links = variable_get('faq_disable_node_links', FALSE);
  $display_faq_count = variable_get('faq_count', FALSE);
  $hide_child_terms = variable_get('faq_hide_child_terms', FALSE);
  $show_term_page_children = variable_get('faq_show_term_page_children', FALSE);
  $group_questions_top = variable_get('faq_group_questions_top', FALSE);
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

  // Configure "back to top" link.
  $back_to_top = faq_init_back_to_top($this_page);

  // Get number of questions, and account for hidden sub-categories.
  $count = 0;
  if ($display_faq_count && $hide_child_terms) {
    $count = taxonomy_term_count_nodes($term->tid, 'faq');
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
    $variables['subcat_body_list'] = faq_get_child_categories_faqs($term, 'faq_category_questions_top', $default_weight, $default_sorting, $category_display, $variables['class'], $parent_term);
  }

  if (!count($data)) {
    $variables['question_count'] = 0;
    return;
  }

  $questions = array();
  $nodes = array();
  foreach ($data as $node) {
    if (!$hide_child_terms) {
      $count++;
    }
    $anchor = 't' . $term->tid . 'n' . $node->nid;
    $node_var = array();
    faq_view_question($node_var, $node, '', $anchor);
    if ($group_questions_top || $category_display == 'hide_qa') {
      faq_view_answer($node_var, $node, $back_to_top, $teaser, $links);
    }
    $nodes[] = $node_var;
    $questions[] = l($node->title, $this_page, array('fragment' => $anchor));
  }
  $variables['question_count'] = $count;
  $variables['use_teaser'] = $teaser;
  $variables['question_list'] = $questions;
  $variables['question_list_style'] = variable_get('faq_question_listing', 'ul');

  $variables['group_questions_top'] = $group_questions_top;
  if ($group_questions_top || $category_display == "hide_qa") {
    $variables['nodes'] = $nodes;
    $variables['answer_category_name'] = variable_get('faq_answer_category_name', FALSE);
  }
  else {
    $variables['nodes'] = array();
    $variables['answer_category_name'] = FALSE;
  }
}

/**
 * Create categorized answers for FAQ page if set to show the questions on top.
 *
 * @param &$variables
 *   Array reference of arguments given to the theme() function.
 */
function template_preprocess_faq_category_questions_top_answers(&$variables) {
  $data = $variables['data'];
  $category_display = $variables['category_display'];
  $term = $variables['term'];
  $parent_term = $variables['parent_term'];
  $class = $variables['class'];

  // Fetch configuration.
  $teaser = variable_get('faq_use_teaser', FALSE);
  $links = variable_get('faq_show_node_links', FALSE);
  $disable_node_links = variable_get('faq_disable_node_links', FALSE);
  $hide_child_terms = variable_get('faq_hide_child_terms', FALSE);
  $show_term_page_children = variable_get('faq_show_term_page_children', FALSE);
  $group_questions_top = variable_get('faq_group_questions_top', FALSE);
  $default_sorting = variable_get('faq_default_sorting', 'DESC');

  // Configure labels.
  $variables['question_label'] = '';
  $variables['answer_label'] = '';
  if (variable_get('faq_qa_mark', FALSE)) {
    $variables['question_label'] = check_plain(variable_get('faq_question_label', "Q:"));
    $variables['answer_label'] = check_plain(variable_get('faq_answer_label', "A:"));
  }

  // Initialise some variables.
  $default_weight = 0;
  if ($default_sorting != 'DESC') {
    $default_weight = 1000000;
  }

  $variables['group_questions_top'] = $group_questions_top;
  if ($group_questions_top || $category_display == "hide_qa") {
    $variables['display_answers'] = FALSE;
    $variables['category_depth'] = 0;
    return;
  }
  $variables['display_answers'] = TRUE;

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

  // Configure "back to top" link.
  $back_to_top = faq_init_back_to_top($this_page);

   // Get taxonomy image.
  $variables['term_image'] = '';
  if (module_exists('taxonomy_image')) {
    $variables['term_image'] = taxonomy_image_display($term->tid, array('class' => 'faq-tax-image'));
  }

  // Configure sub-category bodies (theme recursively).
  $variables['subcat_body_list'] = array();
  if (($get_child_terms && $category_display == 'categories_inline') || ((($show_term_page_children && $this_page != 'faq') || $hide_child_terms) && $category_display == 'hide_qa')) {
    $variables['subcat_body_list'] = faq_get_child_categories_faqs($term, 'faq_category_questions_top_answers', $default_weight, $default_sorting, $category_display, $variables['class'], $parent_term);
  }


  $nodes = array();
  foreach ($data as $node) {
    $anchor = 't' . $term->tid . 'n' . $node->nid;
    $node_var = array();
    faq_view_question($node_var, $node, '', $anchor);
    faq_view_answer($node_var, $node, $back_to_top, $teaser, $links);
    $nodes[] = $node_var;
  }
  $variables['use_teaser'] = $teaser;
  $variables['nodes'] = $nodes;
  $variables['category_name'] = check_plain(faq_tt("taxonomy:term:$term->tid:name", $term->name));
  $variables['category_depth'] = $term->depth;
  $variables['display_header'] = FALSE;
  $variables['answer_category_name'] = variable_get('faq_answer_category_name', FALSE);
  if ($variables['answer_category_name'] && taxonomy_term_count_nodes($term->tid, 'faq')) {
    $variables['display_header'] = TRUE;
  }
}

