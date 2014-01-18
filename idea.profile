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
* Implements hook_install_tasks().
*/
function idea_install_tasks($install_state) {
  // $languages = variable_get('idea_languages', array());
  // $languages_imported = file_scan_directory('profiles/translations', '/(^[a-z]{2})\.po$/', array('recurse' => FALSE));
  $dummy_content = variable_get('idea_add_dummy_content', TRUE);

  $tasks = array(
/*
    'idea_taxonomy' => array(
      'display' => FALSE,
      'type' => '',
      'run' => $dummy_content ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
    ),
*/
/*
    'idea_dummy_users' => array(
      'display' => FALSE,
      'type' => '',
      'run' => $dummy_content ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
    ),
*/
    'idea_dummy_content' => array(
      'display' => FALSE,
      'type' => '',
      'run' => $dummy_content ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
    ),
    //'idea_install_modules' => array(
    //  'type' => 'batch',
    //  'display_name' => st('Install modules'),
    //  'display' => TRUE,
    //  'run' => INSTALL_TASK_RUN_IF_NOT_COMPLETED,
    //),
    //'idea_import_languages' => array(
    //  'type' => 'batch',
    //  'display_name' => st('Import translations'),
    //  'display' => !empty($languages_imported) ? TRUE : FALSE,
    //  'run' => !empty($languages) ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
    //),
  );
  return $tasks;
}

/**
 * Implements hook_install_tasks_alter().
 */
function idea_install_tasks_alter(&$tasks, $install_state) {
  unset($tasks['install_select_profile']);
  unset($tasks['install_select_locale']);
    
  // Add a welcome page.
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

/**
 * This function generates dummy content
 */
function idea_dummy_content() {

  global $base_url;

  // Create dummy challenge
  $challenge = new stdClass();
  $challenge->type = 'challenge';
  node_object_prepare($challenge);

  $challenge->title = 'The first challenge';
  $challenge->uid = 1;
  $challenge->language = LANGUAGE_NONE;
  $challenge->created = time() - 7200;
  $challenge->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $challenge->body[LANGUAGE_NONE][0]['format'] = filter_default_format();
/*
  $challenge->field_end_date[LANGUAGE_NONE][0]['value'] = date('Y-m-d H:i:s', time() - 604800);
  $challenge->field_end_date[LANGUAGE_NONE][0]['value2'] = date('Y-m-d H:i:s', time() + 30 * 86400);
*/

  node_save($challenge);

  $picture_result = drupal_http_request($base_url . '/profiles/idea/images/challenge.jpg');
  $file = file_save_data($picture_result->data, 'public://challenge.jpg', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $challenge->nid);
  $challenge->field_challenge_image = array(LANGUAGE_NONE => array('0' => (array)$file));

  node_save($challenge);

  // Create dummy idea
  $idea = new stdClass();
  $idea->type = 'idea';
  node_object_prepare($idea);

  $idea->title = 'A very good idea';
  $idea->uid = 1;
  $idea->language = LANGUAGE_NONE;
  $idea->created = time() - 3600;
  $idea->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $idea->body[LANGUAGE_NONE][0]['format'] = filter_default_format();
  $idea->field_challenge[LANGUAGE_NONE][0]['target_id'] = $challenge->nid;

  node_save($idea);

  $picture_result = drupal_http_request($base_url . '/profiles/idea/images/idea.jpg');
  $file = file_save_data($picture_result->data, 'public://idea.jpg', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $idea->nid);
  $idea->field_idea_image = array(LANGUAGE_NONE => array('0' => (array)$file));

  node_save($idea);

  variable_del('idea_add_dummy_content');
}
