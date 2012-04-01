<?php

/**
 * @file
 * Provide views data and handlers for vud_comment.module
 */

/**
 * Implements hook_views_data_alter().
 */
function vud_comment_views_data_alter(&$data) {
  $data['comment']['vud_comment_widget'] = array(
    'group' => t('Vote Up/Down'),
    'title' => t('Comment Widget'),
    'help' => t('Provide a comment widget for the Vote up/Down module.'),
    'field' => array(
      'handler' => 'vud_comment_handler_field_widget',
    ),
  );
}



/**
 * Implements hook_views_handlers().
 */
function vud_comment_views_handlers() {
  return array(
    'vud_comment_handler_field_widget' => array(
      'parent' => 'views_handler_field',
    ),
  );
}
