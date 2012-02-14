<?php

// $Id: views_groupby.views.inc,v 1.2 2009/03/03 01:07:33 inadarei Exp $

/** @file 
* Group By Views2 Include File
*/


/**
 * Implementation of hook_views_data()
 */
function views_groupby_views_data() {

 $data = array();

 $data['views_groupby']['table']['group']  = t('SQL Aggregation'); 
 $data['views_groupby']['table']['join'] = array(
    '#global' => array(),
  );

  $data['views_groupby']['views_sql_groupedfields'] = array(
    'title' => t('Group By Fields'),
    'help' => t('Select Fields to Group On'),
    'field' => array(
      'handler' => 'views_groupby_handler_field_groupfields',
    )
  );



  return $data;
}


/**
 * Implementation of hook_views_handlers().
 */
function views_groupby_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'views_groupby') . '/handlers',
    ),
    'handlers' => array(
      // field
      'views_groupby_handler_field_groupfields' => array(
        'parent' => 'views_handler_field',
      )
    ),
  );
}
