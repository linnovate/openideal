<?php
//$Id: date.views.inc,v 1.1.2.3.2.2 2008/11/12 14:19:40 karens Exp $

/**
 * Implementation of hook_views_handlers().
 */
function date_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'date'),
    ),
    // A date-specific handler for grouping multiple values.
    'handlers' => array(
      'date_handler_field_multiple' => array(
        'parent' => 'content_handler_field_multiple',
      ),
     ),
   );
}
