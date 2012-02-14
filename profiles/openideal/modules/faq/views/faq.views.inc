<?php

/**
 * Implements hook_views_data().
 */
function faq_views_data() {
  // faq_questions table.
  $data['faq_questions']['table']['group'] = t('FAQ');
  $data['faq_questions']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
  );
  $data['faq_questions']['question'] = array(
    'title' => t('FAQ short question'),
    'help' => t('The short question text for FAQ nodes.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );
  $data['faq_questions']['detailed_question'] = array(
    'title' => t('FAQ detailed question'),
    'help' => t('The long question text for FAQ nodes.'),
    'field' => array(
      'handler' => 'views_handler_field',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_string',
    ),
  );


  // faq_weights table.
  $data['faq_weights']['table']['group'] = t('FAQ');
  $data['faq_weights']['table']['join'] = array(
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'taxonomy_term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );

  return $data;
}

