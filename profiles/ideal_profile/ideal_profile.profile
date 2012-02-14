<?php

/**
 * @file
 *
 * An example Install profile that uses Profiler. To create your own Install 
 * profile, copy the directory that this file resides in, and rename all files 
 * and directories, replacing profiler_example with the machine name of your 
 * install profile. Then do a find and replace in this file to replace all 
 * instances of profiler_example with the machine name of your profile. Edit the 
 * renamed profiler_example.profiler.inc file to your taste, and presto-change-o, 
 * you've got yourself an install profile!
 */

// Don't leave this out or you'll be sorry! :)
//require_once('./profiles/profiler_example/profiles/profiler/profiler.inc');
!function_exists('profiler_v2') ? require_once('profiles/profiler/profiler.inc') : FALSE;
    profiler_v2('ideal_profile');

/**
 * Return a description of the profile for the initial installation screen. 
 * Profiler also calls this function for details about your Profiler include.
 *
 * @return
 *   An array with keys 'name' and 'description' describing this profile,
 *   and optional 'language' to override the language selection for
 *   language-specific profiles. Profiler also looks to 'path' to discover 
 *   the path of the Profiler include.
 */
function ideal_profile_profile_details() {
  return array(
    'name' => 'IdeaL by Linnovate',
    'description' => 'Installation of IdeaL. A community ideas management system.',
    //'path' => dirname(__FILE__),
  );
  global $conf;
  $conf['site_name'] = '';
  $conf['site_footer'] = 'IdeaL by <a href="http://www.linnovate.net">Linnovate</a>';
  $conf['theme_settings'] = array(
    'default_logo' => 0,
    'logo_path' => 'profiles/ideal_pro/ideal_pro_logo.png',
  );
}

/**
 * Return an array of the modules to be enabled when this profile is installed.
 *
 * @return
 *   An array of modules to enable.
 */
//function ideal_profile_profile_modules() {
//  return profiler_profile_modules('ideal_profile');
//}

/**
 * Return a list of tasks that this profile supports.
 *
 * @return
 *   A keyed array of tasks the profile will perform during the final stage. The 
 *   keys of the array will be used internally, while the values will be 
 *   displayed to the user in the installer task list.
 */
//function ideal_profile_profile_task_list() {
//  return profiler_profile_task_list('ideal_profile');
//}

/**
 * Perform any final installation tasks for this profile.
 *
 * @param $task
 *   The current $task of the install system. When hook_profile_tasks() is first 
 *   called, this is 'profile'.
 * @param $url
 *   Complete URL to be used for a link or form action on a custom page, if 
 *   providing any, to allow the user to proceed with the installation.
 *
 * @return
 *   An optional HTML string to display to the user. Only used if you modify 
 *   the $task, otherwise discarded.
 */
//function ideal_profile_profile_tasks(&$task, $url) {
//  profiler_profile_tasks('ideal_profile', $task, $url);
//}

/**
 * Implementation of hook_form_alter().
 *
 * Allows the profile to alter the site-configuration form. This is called 
 * through custom invocation, so $form_state is not populated.
 *
 * This function is unnecessary for your profile to work, however if you'd like 
 * to bypass the Configuration form during installation and set all those 
 * variables by hand in your Profiler include, you'll need this function in your 
 * install profile.
 */
//function ideal_profile_form_alter(&$form, $form_state, $form_id) {
//  profiler_form_alter('ideal_profile', $form, $form_state, $form_id);
//}