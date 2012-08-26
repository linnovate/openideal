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
  if (empty($options)) {
    $form['default_challenge']['#type'] = 'item';
    $form['default_challenge']['#description'] = t('There are NO challenges created yet.<br /> Go and !add your first challenge.', array('!add' => l(t('add'), 'node/add/challenge')));
    
    return $form;
  }

  return system_settings_form($form);
}
