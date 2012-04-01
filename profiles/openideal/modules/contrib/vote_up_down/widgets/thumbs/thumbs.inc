<?php
/**
 * @file
 *   Thumbs widget.
 */

/**
 * Implementation of hook_vud_widgets(). (Specialized)
 */
function vud_thumbs_vud_widgets() {
  return array(
    'title' => t('Thumbs up and down'),
    'widget template' => 'widget',
    'alter template variables' => 'vud_widget_thumbs_alter_template_vars',
    'js' => 'thumbs.js',
  );
}

function vud_widget_thumbs_alter_template_vars($template_type, &$variables) {
  $criteria['entity_id'] = $variables['entity_id'];
  $criteria['entity_type'] = $variables['type'];
  $criteria['value_type'] = 'points';
  $criteria['tag'] = $variables['tag'];
  $criteria['function']  = 'sum';
  $vote_sum = votingapi_select_single_result_value($criteria);
  $variables['vote_sum'] = ($vote_sum) ? $vote_sum : 0;
}
