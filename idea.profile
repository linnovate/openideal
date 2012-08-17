<?php

/**
 * Implements hook_form_FORM_ID_alter().
 *
 * Allows the profile to alter the site configuration form.
 */
function idea_form_install_configure_form_alter(&$form, $form_state) {
  drupal_get_messages('status');
  drupal_get_messages('warning');
  
  // Pre-populate the site name with the server name.
  $form['site_information']['site_name']['#default_value'] = $_SERVER['SERVER_NAME'];
  
  // Set reasonable defaults for site configuration form
  $form['site_information']['site_name']['#default_value'] = 'Open ideaL';
  $form['site_information']['site_mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST']; 
  $form['site_information']['site_frontpage']['#default_value'] = 'home'; 
  $form['admin_account']['account']['name']['#default_value'] = 'admin';
  $form['admin_account']['account']['mail']['#default_value'] = 'admin@'. $_SERVER['HTTP_HOST'];
}

/**
 * Set openideal as default install profile.
 *
 * Must use system as the hook module because openideal is not active yet
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  foreach($form['profile'] as $key => $element) {
    $form['profile'][$key]['#value'] = 'idea';
  }
}
