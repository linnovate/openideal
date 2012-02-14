<?php
// $Id: rules_scheduler.views.inc,v 1.1.2.1 2009/07/13 13:34:27 fago Exp $

/**
 * @file
 * Views integration for the rules scheduler module.
 */

/**
 * Implementation of hook_views_handlers().
 */
function rules_scheduler_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'rules_scheduler') .'/includes',
    ),
    // A handler for datetime
    'handlers' => array(
      'rules_scheduler_views_handler_datetime' => array(
        'parent' => 'views_handler_field_date',
      ),
    ),
  );
}

/**
 * Implementation of hook_views_data(). Specifies the list of future scheduled
 * tasks displayed on the scheduling page.
 */
function rules_scheduler_views_data() {
  $table = array(
    'rules_scheduler' => array(
      'table' => array(
        'group' => 'Rules scheduler',
        'base' => array(
          'field' => 'tid',
          'title' => t('Scheduled rule sets'),
          'help' => t("Scheduled rules that are executed based on time and cron"),
          'weight' => -10,
        ),
      ),
      'tid' => array(
        'title' => t('Tid'),
        'help' => t('The internal ID of the scheduled rule set'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_numeric',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'set_name' => array(
        'title' => t('Rule set name'),
        'help' => t('The name of the rule set'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'date' => array(
        'title' => t('Scheduled date'),
        'help' => t('Scheduled date and time stamp'),
        'field' => array(
          'handler' => 'rules_scheduler_views_handler_datetime',
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
      'identifier' => array(
        'title' => t('User provided identifier'),
        'help' => t('ID to recognize this specific scheduled task'),
        'field' => array(
          'click sortable' => TRUE,
        ),
        'filter' => array(
          'handler' => 'views_handler_filter',
        ),
        'sort' => array(
          'handler' => 'views_handler_sort',
        ),
      ),
    ),
  );
  return $table;
}
