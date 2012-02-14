<?php

/**
 * @file
 * Flag abuse admin forms and pages.
 */

/**
 * Configuration settings form.
 */
function flag_abuse_settings_form() {
  $form = array();

  $form['flag_abuse_flags'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Abuse Flags'),
    '#default_value' => flag_abuse_get_abuse_flags(),
    '#options' => _flag_abuse_get_flag_options(),
  );

  $form = system_settings_form($form);

  return $form;
}

/**
 * Helper function to return all flag machine names as options.
 */
function _flag_abuse_get_flag_options() {
  $flags = flag_get_flags();
  $default_flags = flag_get_default_flags(TRUE);
  $options = array();

  foreach ($flags + $default_flags as $name => $flag) {
    $options[$name] = $flag->title;
  }

  return $options;
}