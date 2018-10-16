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
  $form['site_information']['site_name']['#default_value'] = 'OpenideaL - Ideas and Innovation Management System';
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
  $dummy_content = variable_get('idea_add_dummy_content', TRUE);

  $tasks = array(
    'idea_taxonomy' => array(
      'display' => FALSE,
      'type' => '',
      'run' => $dummy_content ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
    ),
    'idea_dummy_users' => array(
      'display' => FALSE,
      'type' => '',
      'run' => $dummy_content ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
    ),
    'idea_dummy_content' => array(
      'display' => FALSE,
      'type' => '',
      'run' => $dummy_content ? INSTALL_TASK_RUN_IF_NOT_COMPLETED : INSTALL_TASK_SKIP,
    ),
  );

  # PANOPOLY INTEGRATION
  // Add our custom CSS file for the installation process
  drupal_add_css(drupal_get_path('profile', 'panopoly') . '/panopoly.css');

  // Add the Panopoly app selection to the installation process
  $panopoly_server = array(
    'machine name' => 'panopoly',
    'default apps' => array('panopoly_demo'),
    'default content callback' => 'panopoly_default_content',
  );
  require_once(drupal_get_path('module', 'apps') . '/apps.profile.inc');
  $tasks = $tasks + apps_profile_install_tasks($install_state, $panopoly_server);

  // Add the Panopoly theme selection to the installation process
  require_once(drupal_get_path('module', 'panopoly_theme') . '/panopoly_theme.profile.inc');
  $tasks = $tasks + panopoly_theme_profile_theme_selection_install_task($install_state);

  # END PANOPOLY INTEGRATION

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

  # PANOPOLY INTEGRATION
  // Magically go one level deeper in solving years of dependency problems
  require_once(drupal_get_path('module', 'panopoly_core') . '/panopoly_core.profile.inc');
  $tasks['install_load_profile']['function'] = 'panopoly_core_install_load_profile';

  // If we only offer one language, define a callback to set this
  require_once(drupal_get_path('module', 'panopoly_core') . '/panopoly_core.profile.inc');
  if (!(count(install_find_locales($install_state['parameters']['profile'])) > 1)) {
    $tasks['install_select_locale']['function'] = 'panopoly_core_install_locale_selection';
  }
  # END PANOPOLY INTEGRATION

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
  if ($form_state['values']['install_options']['content'] === 'content') {
  }
}

/**
 * Generate default taxonomy terms.
 */
function idea_taxonomy() {

  // Create default taxonomy terms.
  $terms = array(
    array('name' => 'Products', 'vid' => 4, 'weight' => 1),
    array('name' => 'Services', 'vid' => 4, 'weight' => 2),
    array('name' => 'Processes', 'vid' => 4, 'weight' => 3),
  );
  //find vid for challenge_status
  $vocabularies = taxonomy_vocabulary_get_names();
  if(!empty($vocabularies) && isset($vocabularies['challenge_status'])){
    $vid = $vocabularies['challenge_status']->vid;
    $terms[] = array('name' => 'Open', 'vid' => $vid, 'weight' => 1);
    $terms[] = array('name' => 'Reviewed', 'vid' => $vid, 'weight' => 2);
    $terms[] = array('name' => 'Closed', 'vid' => $vid, 'weight' => 3);
  }

  foreach ($terms as $term) {
    idea_create_term($term['name'], $term['vid'], $term['weight']);
  }
}

/**
 * Save term to database.
 */
  function idea_create_term($name, $vid, $weight) {
    $term = new stdClass();
    $term->name = $name;
    $term->vid = $vid;
    $term->weight = $weight;
    taxonomy_term_save($term);
}

/**
 * Generate dummy content
 */
function idea_dummy_content() {

  global $base_url;
  $accounts = array(
    1 => user_load(1),
    2 => user_load(2),
    3 => user_load(3),
    4 => user_load(4),
    5 => user_load(5),
    6 => user_load(6),
  );

  // Create messages for users.
  foreach ($accounts as $account) {
    $message = message_create('new_user', array(), $account);
    $message->save();
  }

  // First challenge
  $challenge = new stdClass();
  $challenge->type = 'challenge';
  node_object_prepare($challenge);

  $challenge->title = 'The first challenge';
  $challenge->uid = 3;
  $challenge->promote = 1;
  $challenge->language = LANGUAGE_NONE;
  $challenge->created = time() - 7200;
  $challenge->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $challenge->body[LANGUAGE_NONE][0]['format'] = filter_default_format();
  $challenge->field_dates[LANGUAGE_NONE][0]['value'] = date('Y-m-d H:i:s', time() - 604800);
  $challenge->field_dates[LANGUAGE_NONE][0]['value2'] = date('Y-m-d H:i:s', time() + 30 * 86400);
  $challenge->field_moderator[LANGUAGE_NONE][0]['target_id'] = 4;
  $challenge->field_category[LANGUAGE_NONE][0]['tid'] = 1;
  node_save($challenge);

  $picture_result = drupal_http_request($base_url . '/profiles/idea/files/challenge1.jpg');
  $file = file_save_data($picture_result->data, 'public://challenge1.jpg', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $challenge->nid);
  $challenge->field_challenge_image = array(LANGUAGE_NONE => array('0' => (array)$file));

  $file_result = drupal_http_request($base_url . '/profiles/idea/files/challenge.txt');
  $file = file_save_data($file_result->data, 'public://challenge.txt', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $challenge->nid);
  $challenge->field_attachments[LANGUAGE_NONE][] = array(
    'fid' => $file->fid,
    'display' => 1,
    'description' => '',
  );

  node_save($challenge);

  // Create message for challenge.
  $message = message_create('new_challenge', array(), $accounts[3]);
  $message->field_challenge[LANGUAGE_NONE][0]['target_id'] = $challenge->nid;
  $message->save();

  // Second challenge
  $challenge = new stdClass();
  $challenge->type = 'challenge';
  node_object_prepare($challenge);

  $challenge->title = 'The second challenge';
  $challenge->uid = 4;
  $challenge->language = LANGUAGE_NONE;
  $challenge->created = time() - 7200;
  $challenge->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $challenge->body[LANGUAGE_NONE][0]['format'] = filter_default_format();
  $challenge->field_dates[LANGUAGE_NONE][0]['value'] = date('Y-m-d H:i:s', time() - 604800);
  $challenge->field_dates[LANGUAGE_NONE][0]['value2'] = date('Y-m-d H:i:s', time() + 30 * 86400);
  $challenge->field_moderator[LANGUAGE_NONE][0]['target_id'] = 4;
  $challenge->field_category[LANGUAGE_NONE][0]['tid'] = 1;

  node_save($challenge);

  $picture_result = drupal_http_request($base_url . '/profiles/idea/files/idea1.jpg');
  $file = file_save_data($picture_result->data, 'public://challenge2.jpg', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $challenge->nid);
  $challenge->field_challenge_image = array(LANGUAGE_NONE => array('0' => (array)$file));

  node_save($challenge);

  // Create message for challenge.
  $message = message_create('new_challenge', array(), $accounts[4]);
  $message->field_challenge[LANGUAGE_NONE][0]['target_id'] = $challenge->nid;
  $message->save();

    // Third challenge
  $challenge = new stdClass();
  $challenge->type = 'challenge';
  node_object_prepare($challenge);

  $challenge->title = 'The third challenge';
  $challenge->uid = 2;
  $challenge->language = LANGUAGE_NONE;
  $challenge->created = time() - 7200;
  $challenge->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $challenge->body[LANGUAGE_NONE][0]['format'] = filter_default_format();
  $challenge->field_dates[LANGUAGE_NONE][0]['value'] = date('Y-m-d H:i:s', time() - 604800);
  $challenge->field_dates[LANGUAGE_NONE][0]['value2'] = date('Y-m-d H:i:s', time() + 30 * 86400);
  $challenge->field_moderator[LANGUAGE_NONE][0]['target_id'] = 4;
  $challenge->field_category[LANGUAGE_NONE][0]['tid'] = 1;

  node_save($challenge);

  $picture_result = drupal_http_request($base_url . '/profiles/idea/files/challenge3.jpg');
  $file = file_save_data($picture_result->data, 'public://challenge3.jpg', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $challenge->nid);
  $challenge->field_challenge_image = array(LANGUAGE_NONE => array('0' => (array)$file));

  node_save($challenge);

  // Create message for challenge.
  $message = message_create('new_challenge', array(), $accounts[2]);
  $message->field_challenge[LANGUAGE_NONE][0]['target_id'] = $challenge->nid;
  $message->save();

  // Fourth challenge
  $challenge = new stdClass();
  $challenge->type = 'challenge';
  node_object_prepare($challenge);

  $challenge->title = 'The fourth challenge';
  $challenge->uid = 5;
  $challenge->language = LANGUAGE_NONE;
  $challenge->created = time() - 7200;
  $challenge->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $challenge->body[LANGUAGE_NONE][0]['format'] = filter_default_format();
  $challenge->field_dates[LANGUAGE_NONE][0]['value'] = date('Y-m-d H:i:s', time() - 604800);
  $challenge->field_dates[LANGUAGE_NONE][0]['value2'] = date('Y-m-d H:i:s', time() + 30 * 86400);
  $challenge->field_moderator[LANGUAGE_NONE][0]['target_id'] = 4;
  $challenge->field_category[LANGUAGE_NONE][0]['tid'] = 1;

  node_save($challenge);

  $picture_result = drupal_http_request($base_url . '/profiles/idea/files/challenge4.jpg');
  $file = file_save_data($picture_result->data, 'public://challenge4.jpg', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $challenge->nid);
  $challenge->field_challenge_image = array(LANGUAGE_NONE => array('0' => (array)$file));

  node_save($challenge);

  // Create message for challenge.
  $message = message_create('new_challenge', array(), $accounts[5]);
  $message->field_challenge[LANGUAGE_NONE][0]['target_id'] = $challenge->nid;
  $message->save();

  // Create dummy ideas
  // First idea
  $idea = new stdClass();
  $idea->type = 'idea';
  node_object_prepare($idea);

  $idea->title = 'A very good idea';
  $idea->uid = 6;
  $idea->language = LANGUAGE_NONE;
  $idea->created = time() - 3600;
  $idea->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $idea->body[LANGUAGE_NONE][0]['format'] = filter_default_format();
  $idea->field_challenge_ref[LANGUAGE_NONE][0]['target_id'] = $challenge->nid;

  node_save($idea);

  $picture_result = drupal_http_request($base_url . '/profiles/idea/files/idea1.jpg');
  $file = file_save_data($picture_result->data, 'public://idea1.jpg', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $idea->nid);
  $idea->field_idea_image = array(LANGUAGE_NONE => array('0' => (array)$file));

  node_save($idea);

  // Create message for idea.
  $message = message_create('new_idea', array(), $accounts[6]);
  $message->field_posted_idea[LANGUAGE_NONE][0]['target_id'] = $idea->nid;
  $message->field_challenge[LANGUAGE_NONE][0]['target_id'] = $idea->field_challenge_ref[LANGUAGE_NONE][0]['target_id'];
  $message->save();

  // Second idea
  $idea = new stdClass();
  $idea->type = 'idea';
  node_object_prepare($idea);

  $idea->title = 'An even better idea';
  $idea->uid = 4;
  $idea->promote = 1;
  $idea->language = LANGUAGE_NONE;
  $idea->created = time() - 3600;
  $idea->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $idea->body[LANGUAGE_NONE][0]['format'] = filter_default_format();
  $idea->field_challenge_ref[LANGUAGE_NONE][0]['target_id'] = $challenge->nid;

  node_save($idea);

  $picture_result = drupal_http_request($base_url . '/profiles/idea/files/idea2.jpg');
  $file = file_save_data($picture_result->data, 'public://idea2.jpg', FILE_EXISTS_RENAME);
  file_usage_add($file, 'node', 'node', $idea->nid);
  $idea->field_idea_image = array(LANGUAGE_NONE => array('0' => (array)$file));

  node_save($idea);

  // Create message for idea.
  $message = message_create('new_idea', array(), $accounts[4]);
  $message->field_posted_idea[LANGUAGE_NONE][0]['target_id'] = $idea->nid;
  $message->field_challenge[LANGUAGE_NONE][0]['target_id'] = $idea->field_challenge_ref[LANGUAGE_NONE][0]['target_id'];
  $message->save();

  // News
  // First news
  $news = new stdClass();
  $news->type = 'news';
  node_object_prepare($news);

  $news->title = 'First news';
  $news->uid = 1;
  $news->language = LANGUAGE_NONE;
  $news->created = time() - 3600;
  $news->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $news->body[LANGUAGE_NONE][0]['format'] = filter_default_format();

  node_save($news);

  // Second news
  $news = new stdClass();
  $news->type = 'news';
  node_object_prepare($news);

  $news->title = 'Second news';
  $news->uid = 1;
  $news->language = LANGUAGE_NONE;
  $news->created = time() - 3600;
  $news->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $news->body[LANGUAGE_NONE][0]['format'] = filter_default_format();

  node_save($news);

  // Third news
  $news = new stdClass();
  $news->type = 'news';
  node_object_prepare($news);

  $news->title = 'Third news';
  $news->uid = 1;
  $news->promote = 1;
  $news->language = LANGUAGE_NONE;
  $news->created = time() - 3600;
  $news->body[LANGUAGE_NONE][0]['value'] = "Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
  sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.
  Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua.
  At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet.";
  $news->body[LANGUAGE_NONE][0]['format'] = filter_default_format();

  node_save($news);

  // Dummy comments.
  $comment = new stdClass();
  $comment->nid = 5;
  $comment->cid = 0;
  $comment->pid = 0;
  $comment->uid = $accounts[2]->uid;
  $comment->mail = $accounts[2]->mail;
  $comment->name = $accounts[2]->name;
  $comment->thread = '01/';
  $comment->created = time() - 1800;
  $comment->is_anonymous = 0;
  $comment->homepage = '';
  $comment->status = COMMENT_PUBLISHED;
  $comment->language = LANGUAGE_NONE;
  $comment->subject = 'First comment';
  $comment->comment_body[$comment->language][0]['value'] = 'Comment body text'; // Everything here is pretty much like with a node
  $comment->comment_body[$comment->language][0]['format'] = 'filtered_html';
  comment_save($comment);

  // Create message for comment.
  $message = message_create('new_comment', array(), $accounts[2]);
  $message->field_comment[LANGUAGE_NONE][0]['target_id'] = $comment->cid;
  $message->save();

  $comment = new stdClass();
  $comment->nid = 6;
  $comment->cid = 0;
  $comment->pid = 0;
  $comment->uid = $accounts[3]->uid;
  $comment->mail = $accounts[3]->mail;
  $comment->name = $accounts[3]->name;
  $comment->thread = '01/';
  $comment->created = time() - 1800;
  $comment->is_anonymous = 0;
  $comment->homepage = '';
  $comment->status = COMMENT_PUBLISHED;
  $comment->language = LANGUAGE_NONE;
  $comment->subject = 'Second comment';
  $comment->comment_body[$comment->language][0]['value'] = 'Comment body text'; // Everything here is pretty much like with a node
  $comment->comment_body[$comment->language][0]['format'] = 'filtered_html';
  comment_save($comment);

  // Create message for comment.
  $message = message_create('new_comment', array(), $accounts[3]);
  $message->field_comment[LANGUAGE_NONE][0]['target_id'] = $comment->cid;
  $message->save();

  $comment = new stdClass();
  $comment->nid = 7;
  $comment->cid = 0;
  $comment->pid = 0;
  $comment->uid = $accounts[4]->uid;
  $comment->mail = $accounts[4]->mail;
  $comment->name = $accounts[4]->name;
  $comment->thread = '01/';
  $comment->created = time() - 1800;
  $comment->is_anonymous = 0;
  $comment->homepage = '';
  $comment->status = COMMENT_PUBLISHED;
  $comment->language = LANGUAGE_NONE;
  $comment->subject = 'Third comment';
  $comment->comment_body[$comment->language][0]['value'] = 'Comment body text'; // Everything here is pretty much like with a node
  $comment->comment_body[$comment->language][0]['format'] = 'filtered_html';
  comment_save($comment);

  // Create message for comment.
  $message = message_create('new_comment', array(), $accounts[4]);
  $message->field_comment[LANGUAGE_NONE][0]['target_id'] = $comment->cid;
  $message->save();

  // Add nodes to Head to Head nodequeue.
  db_insert('nodequeue_nodes')
    ->fields(array(
      'qid' => 1,
      'sqid' => 1,
      'nid' => 5,
      'position' => 1,
      'timestamp' => time() - 3600,
    ))
    ->execute();

  db_insert('nodequeue_nodes')
    ->fields(array(
      'qid' => 1,
      'sqid' => 1,
      'nid' => 6,
      'position' => 2,
      'timestamp' => time() - 3600,
    ))
    ->execute();

  variable_del('idea_add_dummy_content');
}

/**
 * Task for creating dummy users.
 */
function idea_dummy_users() {

  // Create dummy users.
  $users = array(
    'Lan Stallworth',
    'Sharan Mcneal',
    'Matilda Griggs',
    'Stan Chu',
    'Mardell Tenney',
  );

  foreach ($users as $name) {
    list($first_name, $last_name)  = explode(" ", $name);
    $password = user_password(8);

    $mail = drupal_strtolower(str_replace(' ', '.', $name)) . '@example.com';

    $fields = array(
      'name' => $name,
      'mail' => $mail,
      'pass' => $password,
      'status' => 1,
      'init' => $mail,
      'roles' => array(
        DRUPAL_AUTHENTICATED_RID => 'authenticated user'
      ),
    );

    $fields['field_first_name'][LANGUAGE_NONE][0]['value'] = $first_name;
    $fields['field_last_name'][LANGUAGE_NONE][0]['value'] = $last_name;

    $account = user_save('', $fields);

    // Add pictures to dummmy users.
    idea_add_user_picture($account);
  }
}

/**
 * Function to add pictures to dummy users.
 */
function idea_add_user_picture($account) {
  global $base_url;

  if ($account->uid) {
    $picture_directory =  file_default_scheme() . '://' . variable_get('user_picture_path', 'pictures');
    if(file_prepare_directory($picture_directory, FILE_CREATE_DIRECTORY)){
      $picture_result = drupal_http_request($base_url . '/profiles/idea/files/users/' . drupal_strtolower(str_replace(' ', '_', $account->name)) . '.jpg');
      $picture_path = file_stream_wrapper_uri_normalize($picture_directory . '/picture-' . $account->uid . '-' . REQUEST_TIME . '.jpg');
      $picture_file = file_save_data($picture_result->data, $picture_path, FILE_EXISTS_REPLACE);

      // Check to make sure the picture isn't too large for the site settings.
      $validators = array(
        'file_validate_is_image' => array(),
        'file_validate_image_resolution' => array(variable_get('user_picture_dimensions', '85x85')),
        'file_validate_size' => array(variable_get('user_picture_file_size', '30') * 1024),
      );

      // attach photo to user's account.
      $errors = file_validate($picture_file, $validators);

      if (empty($errors)) {
        // Update the user record.
        $picture_file->uid = $account->uid;
        $picture_file = file_save($picture_file);
        file_usage_add($picture_file, 'user', 'user', $account->uid);
        db_update('users')
          ->fields(array(
          'picture' => $picture_file->fid,
          ))
          ->condition('uid', $account->uid)
          ->execute();
        $account->picture = $picture_file->fid;
      }
    }
  }
}

/**
 * Hook Cron to set Indexes back to index.php
 * This runs during install_finished()
 */
function idea_cron() {
  # Allow system-side post-install procedure
  if (variable_get('openideal_post_install', FALSE) == FALSE) {
    system(DRUPAL_ROOT . '/profiles/idea/post-install.sh');
    variable_set('openideal_post_install', TRUE);
  }
  //drupal_set_message($retval);
}