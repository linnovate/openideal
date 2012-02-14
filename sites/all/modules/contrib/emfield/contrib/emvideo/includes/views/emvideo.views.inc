<?php
// $Id: emvideo.views.inc,v 1.1.2.1 2009/09/30 15:56:48 aaron Exp $

/**
 * @file
 * Interface between emfield.module and views.module.
 */

// Include the files defining the classes we extend.
module_load_include('inc', 'content', 'includes/views/content.views');

/**
 * Implementation of hook_views_handlers().
 */
function emvideo_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'emvideo') . '/includes/views/handlers',
    ),
    'handlers' => array(

      // argument handlers
//       'emfield_handler_argument' => array(
//         'parent' => 'views_handler_argument',
//       ),
      'emvideo_handler_argument_duration' => array(
        'parent' => 'content_handler_argument_numeric',
      ),
//       'emfield_handler_argument_numeric' => array(
//         'parent' => 'views_handler_argument_numeric',
//       ),
//       'emfield_handler_argument_reference' => array(
//         'parent' => 'emfield_handler_argument_numeric',
//       ),
//       'emfield_handler_argument_many_to_one' => array(
//         'parent' => 'views_handler_argument_many_to_one',
//       ),

      // field handlers
      'emvideo_handler_field_duration' => array(
        'parent' => 'content_handler_field_multiple',
      ),
//       'emfield_handler_field_multiple' => array(
//         'parent' => 'emfield_handler_field',
//       ),

      // filter handlers
//       'emfield_handler_filter_string' => array(
//         'parent' => 'views_handler_filter_string',
//       ),
//       'emfield_handler_filter_numeric' => array(
//         'parent' => 'views_handler_filter_numeric',
//       ),
//       'emfield_handler_filter_float' => array(
//         'parent' => 'views_handler_filter_float',
//       ),
//       'emfield_handler_filter_many_to_one' => array(
//         'parent' => 'views_handler_filter_many_to_one',
//       ),

      // relationship handlers
//       'emfield_handler_relationship' => array(
//         'parent' => 'views_handler_relationship',
//       ),

      // sort handlers
//       'emfield_handler_sort' => array(
//         'parent' => 'views_handler_sort',
//       ),
    ),
  );
}

function theme_emvideo_handler_field_duration($item, $field, $node, $values, $format) {
  $output = '';
  if (!empty($item)) {
    $duration = $item['duration'];
    switch ($format) {
      case 'duration_time':
        $output = emvideo_seconds_to_time($duration);
        break;
      case 'duration_seconds':
        $output = check_plain($duration);
        break;
    }
  }
  return $output;
}
