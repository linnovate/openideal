<?php

/**
 * Implements hook_views_data().
 */
function forward_views_data() {
  // Basic table information.

  // Define the base group of this table. Fields that don't
  // have a group defined will go into this field by default.
  $data['forward_statistics']['table'] = array(
    'group' => t('Forward'),
  );

  // For other base tables, explain how we join
  $data['forward_statistics']['table']['join']['node'] = array(
    'left_field' => 'nid',
    'field' => 'nid',
  );

  // Fields
  $data['forward_statistics']['forward_count'] = array(
    'title' => t('forward count'),
    'help' => t('The module that associated a product with the node.'),
    'field' => array('handler' => 'views_handler_field_numeric',),
    'filter' => array('handler' => 'views_handler_filter_numeric',),
    'sort' => array('handler' => 'views_handler_sort',),
  );
  $data['forward_statistics']['clickthrough_count'] = array(
    'title' => t('clickthrough count'),
    'help' => t('The module that associated a product with the node.'),
    'field' => array('handler' => 'views_handler_field_numeric',),
    'filter' => array('handler' => 'views_handler_filter_numeric',),
    'sort' => array('handler' => 'views_handler_sort',),
  );
  $data['forward_statistics']['last_forward_timestamp'] = array(
    'title' => t('most recent timestamp'),
    'help' => t('The module that associated a product with the node.'),
    'field' => array('handler' => 'views_handler_field_date',),
    'filter' => array('handler' => 'views_handler_filter_timestamp',),
    'sort' => array('handler' => 'views_handler_sort_date',),
  );

  return $data;
}

