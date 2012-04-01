<?php

/**
 * @file
 * Up Down Widget.
 */

/**
 * Plugin decleration.
 */
$plugin = array(
  'title' => t('Up and down'),
  'widget template' => 'widget',
  'alter template variables' => 'vud_widget_upanddown_alter_template_vars',
);

/**
 * Alter template variables.
 */
function vud_widget_upanddown_alter_template_vars($template_type, &$variables) {
  $criteria = array(
    'entity_type' => $variables['type'],
    'entity_id' => $variables['entity_id'],
    'value_type' => 'points',
    'tag' => $variables['tag'],
    'function' => 'positives'
  );
  $positives = (int)votingapi_select_single_result_value($criteria);
  $variables['up_points'] = $positives;

  $criteria = array(
    'entity_type' => $variables['type'],
    'entity_id' => $variables['entity_id'],
    'value_type' => 'points',
    'tag' => $variables['tag'],
    'function' => 'negatives'
  );
  $negatives = (int)votingapi_select_single_result_value($criteria);
  $variables['down_points'] = $negatives;
}
