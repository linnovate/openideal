<?php

/**
 * @file Includes any rules integration provided by heartbeat comments.
 */

/**
 * Implements hook_rules_event_info().
 */
function heartbeat_comments_rules_event_info() {

  $items = array(
    'heartbeat_comment_create' => array(
      'label' => t('After saving new heartbeat comment'),
      'group' => t('Heartbeat'),
      'variables' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('Heartbeat comment actor'),
        ),
        'heartbeat_comment' => array(
          'type' => 'heartbeat_comment',
          'label' => t('Heartbeat comment'),
        ),
        'actor' => array(
          'type' => 'user',
          'label' => t('Actor of the parent activity'),
        ),
      ),
    ),
  );

  return $items;

}

/**
 * Implements hook_rules_data_info().
 */
function heartbeat_comments_rules_data_info() {
  return array(
    'heartbeat_comment' => array(
      'label' => t('heartbeat comment'),
      'group' => t('Heartbeat'),
    ),
  );
}