<?php

/**
  * Implements hook_rules_condition_info()
  */
function ideal_main_rules_condition_info() {
  return array(
    'userpoints_condition_check_points' => array(
      'label' => t('Check how many !points a user has', userpoints_translation()),
      'named parameter' => TRUE,
      'parameter' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
          'description' => t('The user whose !points to check', userpoints_translation()),
        ),
        'tid' => array(
          'label' => t('!Points category', userpoints_translation()),
          'type' => 'integer',
          'options list' => 'userpoints_rules_get_categories',
        ),
        'threshold' => array(
          'type' => 'integer',
          'label' => t('Threshold'),
          'description' => t('Compare the number of !points the user has to this number', userpoints_translation()),
       ),
        'comparison' => array(
          'type' => 'text',
          'label' => t('Comparison'),
          'description' => t('How to compare the user\'s !points with the threshold', userpoints_translation()),
          'options list' => 'userpoints_condition_get_comparisons',
        ),
      ),
      'group' => t('!Points', userpoints_translation()),
    ),
  );
}

 /**
 * Rules condition - compare the user's points to a threshold
 */
function userpoints_condition_check_points($params) {
  $points = userpoints_get_current_points($params['user']->uid, $params['tid']);
  $threshold = $params['threshold'];
  switch ($params['comparison']) {
    case 'lt':
      if ($threshold > $points) { return true; } else { return false; }
      break;
    case 'le':
      if ($threshold >= $points) { return true; } else { return false; }
      break;
    case 'eq':
      if ($threshold == $points) { return true; } else { return false; }
      break;
    case 'ge':
      if ($threshold <= $points) { return true; } else { return false; }
      break;
    case 'gt':
      if ($threshold < $points) { return true; } else { return false; }
      break;
  }
  return false;
}

/**
 * Returns options for ways to compare a number to a number of points.
 */
function userpoints_condition_get_comparisons() {
  return array(
    'lt' => t('Less than'),
    'le' => t('Less than or equal to'),
    'eq' => t('Equal to'),
    'ge' => t('Greater than or equal to'),
    'gt' => t('Greater than'),
  );
}