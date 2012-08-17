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
 * Must use system as the hook module because openideal is not active yet
 */
function system_form_install_select_profile_form_alter(&$form, $form_state) {
  install_goto('install.php?profile=idea&locale=en');
}

/**
 * Implements hook_install_tasks_alter().
 */
function idea_install_tasks_alter(&$tasks, $install_state) {
  unset($tasks['install_select_profile']);
  unset($tasks['install_select_locale']);
  
  // Add a wolcome page.
  $new_task['install_welcome'] = array(
    'display' => TRUE,
    'display_name' => st('Welcome'),
    'type' => 'form',
    'run' => isset($install_state['parameters']['welcome']) ? INSTALL_TASK_SKIP : INSTALL_TASK_RUN_IF_REACHED,
  );
  $tasks = array_merge($new_task, $tasks);
  _openideal_set_theme('ideal7');
}

/**
 * Force-set a theme at any point during the execution of the request.
 *
 * Drupal doesn't give us the option to set the theme during the installation
 * process and forces enable the maintenance theme too early in the request
 * for us to modify it in a clean way.
 */
function _openideal_set_theme($target_theme) {
  if ($GLOBALS['theme'] != $target_theme) {
    unset($GLOBALS['theme']);

    drupal_static_reset();
    $GLOBALS['conf']['maintenance_theme'] = $target_theme;
    _drupal_maintenance_theme();
  }
}

/**
 * Task callback: shows the welcome screen.
 */
function install_welcome($form, &$form_state, &$install_state) {
  drupal_set_title(st('Welcome'));

  $message = st('Thank you for choosing OpenideaL') . '<br />';
  $message .= '<p>' . st('OpenideaL distribution will install a Drupal based idea management system (IDMS), <br />
    includes the best community contributions, packed and configured.') . '</p>';
  $message .= '<p>' . st('For more informations please check ' . l('http://www.openidealapp.com', 'http://www.openidealapp.com')) . '</p>';

  $form = array();
  $form['welcome_message'] = array(
    '#markup' => $message,
  );
  $form['actions'] = array(
    '#type' => 'actions',
  );
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => st("Let's Get Started!"),
    '#weight' => 10,
  );
  return $form;
}

/**
 * install_welcome submit.
 */
function install_welcome_submit($form, &$form_state) {
  global $install_state;

  $install_state['parameters']['welcome'] = 'done';
  $install_state['parameters']['locale'] = 'en';
}