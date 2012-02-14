<?php
// $Id: faq.views.inc,v 1.1.2.3 2010/10/29 08:07:16 snpower Exp $

/**
 * @file
 * Exposes faq module's database tables to views.
 */

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
    'term_data' => array(
      'left_field' => 'tid',
      'field' => 'tid',
    ),
  );
  $data['faq_weights']['weight'] = array(
    'title' => t('FAQ weight'),
    'help' => t('The weight of the FAQ node.'),
    'field' => array(
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'sort' => array(
      'handler' => 'views_handler_sort',
    ),
    'filter' => array(
      'handler' => 'views_handler_filter_numeric',
    ),
  );

  return $data;
}

