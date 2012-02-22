<?php
/**
 * @file
 * This drush script creates a sub-theme for Sasson.
 * Usage: drush sns "Theme Name"
 * Started as a patch by ssherriff (http://drupal.org/user/92501).
 * @see http://drupal.org/node/1350038
 *
 */

/**
 * Implementation of hook_drush_command().
 */
function sasson_drush_command() {
  $items = array();

  $items['sasson-new-subtheme'] = array(
    'description' => 'Set up a Sasson starter kit.',
    'aliases' => array('sns'),
    'arguments' => array(
      'name' => 'A name for your theme.',
      'machine_name' => '(optional)[a-z, 0-9] A machine-readable name for your theme.',
    ),
    'examples' => array(
      'example[1]' => 'drush sasson "My theme name"',
    ),
  );

  return $items;
}

/**
 * Implements hook_drush_help().
 */
function sasson_drush_help($section) {
  switch ($section) {
    case 'drush:sasson-new-subtheme':
      return dt("Set up a Sasson starter kit.");
  }
}


/**
 * Command: Set up a Sasson starter kit.
 */
function drush_sasson_new_subtheme($name = NULL, $machine_name = NULL) {
  if (empty($name)) {
    drush_print(dt("\nWe can't create a subtheme with no name. \nPlease provide a name for the subtheme.\nEx. drush sns [name]\n"));
    return;
  }
  
  $machine_name = !empty($machine_name) ? preg_replace('/[^a-z0-9_]+/', '', strtolower($machine_name)) : preg_replace('/[^a-z0-9_]+/', '', strtolower($name));
  $sasson_path = drush_locate_root() . '/' . drupal_get_path('theme', 'sasson');

  // From Sasson's location, we move up one directory and construct the path where
  // our sub theme will be created.
  $_sasson_path = explode('/', $sasson_path);
  array_pop($_sasson_path);
  $theme_dir = implode('/', $_sasson_path);
  $subtheme_path = $theme_dir . '/' . $machine_name;

  // Make a fresh copy of the original starter kit.
  $s = drush_copy_dir("$sasson_path/SUBTHEME/", $subtheme_path);
  if (empty($s)) {
    return;
  }

  // Rename the info file and fill in the theme name.
  drush_op('rename', "$subtheme_path/SUBTHEME.info", "$subtheme_path/$machine_name.info");
  drush_op('rename', "$subtheme_path/styles/SUBTHEME.scss", "$subtheme_path/styles/$machine_name.scss");
  drush_op('rename', "$subtheme_path/images/SUBTHEME.png", "$subtheme_path/images/$machine_name.png");
  drush_op('sasson_file_str_replace', "$subtheme_path/$machine_name.info", 'SUBTHEME', "$machine_name");
  drush_op('sasson_file_str_replace', "$subtheme_path/$machine_name.info", 'Starterkit', "$name");

  // Notify user of the newly created theme.
  drush_print(dt("\n!name sub-theme was created in !path. \n",
    array(
      '!name' => $name,
      '!path' => $subtheme_path,
    )
  ));
}


/**
 * Internal helper: Replace strings in a file.
 */
function sasson_file_str_replace($file_path, $find, $replace) {
  $file_contents = file_get_contents($file_path);
  $file_contents = str_replace($find, $replace, $file_contents);
  file_put_contents($file_path, $file_contents);
}