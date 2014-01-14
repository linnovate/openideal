<?php

/**
 * Administrative settings for FBSSTS.
 */

/**
 * The admin settings form.
 */
function ideal_suggestions_admin($form, $form_state) {
  $form = array();
  $form['suggestions_count'] = array(
    '#type' => 'select',
    '#title' => t('Number of tags to suggest'),
    '#default_value' => variable_get('suggestions_count', 5),
    '#options' => drupal_map_assoc(range(1, 20)),
  );
  return system_settings_form($form);
}