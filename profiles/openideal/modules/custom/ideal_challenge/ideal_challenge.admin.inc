<?php

/**
 * Form to select the default chellenge.
 */
function ideal_challenge_default_challenge_form($form) {
  $options = array();
  $result = db_query("SELECT nid, title FROM {node} WHERE type = :type", array(':type' => 'challenge'));
  while ($fetch = $result->fetchObject()) {
    $options[$fetch->nid] = $fetch->title;
  }

  $form['default_challenge'] = array(
    '#type' => 'select',
    '#title' => t('Default Challenge'),
    '#description' => 'Select the site current idea.',    
    '#options' => $options,
    '#default_value' => variable_get('default_challenge', NULL),
  );

  return system_settings_form($form);
}
