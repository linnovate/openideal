<?php
/**
 * Page callback for settings the default challenge.
 */
function ideal_challenge_page() {
  return drupal_get_form('ideal_challenge_default_challenge_form');
}

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
    '#options' => $options,
    '#default_value' => variable_get('default_challenge', NULL),
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );

  return $form;
}

/**
 * @param $form
 * @param $form_state
 * @return unknown_type
 */
function ideal_challenge_default_challenge_form_submit($form, &$form_state) {
  $default_challenge = $form_state['values']['default_challenge'];
  variable_set('default_challenge', $default_challenge);
}
