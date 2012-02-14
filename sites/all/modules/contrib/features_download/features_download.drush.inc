<?php 
/**
 * Implementation of hook_drush_command().
 * 
 * @See drush_parse_command() for a list of recognized keys.
 *
 * @return
 *   An associative array describing your command(s).
 */
function features_download_drush_command() {
  $items = array();
  $items['git-pull'] = array(
    'description' => "Do git pull if all features are in default.",
    //'drupal dependencies' => array('features'),
    'aliases' => array('pull'),
    'argument' => array(
    	'force' => 'Force the command',
    )
  );
  $items['git-push'] = array(
    'description' => "Do git push if all features are in default.",
    //'drupal dependencies' => array('features'),
    'aliases' => array('push'),
  	'argument' => array(
    	'force' => 'Force the command',
    )
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function features_download_drush_help($section) {
  switch ($section) {
    case 'drush:git-pull':
      return dt("Do git pull if all features are in default.");
    case 'drush:git-push':
      return dt("Do git push if all features are in default.");
  }
}

/**
 * git pull.
 */
//One of the problems working with Git and features module, 
//is when you pulling from git before you update you feature changes to code.
//Well, now (let say you got some feature changes comming form the pull) you have to deside, 
//revert the feature and ignore all of your settings change or recreate the feature and override some else job.
//This drush command is tring to help you enforce the very basic procedures of 
//"Alwayes update your feature to code before you pulling from git"
function drush_features_download_git_pull($force = '') {
  if (_git_ready() || ($force == 'force' && _confirm())) {
    exec('git pull 2>&1', $output);
    foreach ($output as $line) {
      drush_print(dt($line));
      drush_print(dt('A new base code might changed please check if your feature need to revert (drush fr).'));
    }
    //TODO: learn to work with "drush_shell_exec('git pull')" and maybe "drush_shell_exec_output()"
  }
}
/**
 * git push.
 */
function drush_features_download_git_push($force = '') {
  exec('git push 2>&1', $output);
  foreach ($output as $line) {
    drush_print(dt($line));
  }
  //TODO: learn to work with "drush_shell_exec('git pull')" and maybe "drush_shell_exec_output()"
}


function _git_ready() {
  module_load_include('inc', 'features', 'features.export');
  $overridden_modules = array();
  $need_review_modules = array();
  foreach (features_get_features(NULL, TRUE) as $key => $module) {
    switch (features_get_storage($module->name)) {
      case FEATURES_OVERRIDDEN:
      case FEATURES_NEEDS_REVIEW:
        $overridden_modules[] = $module->name;
        break;
    }
  }
  if (!empty($overridden_modules)) {
    drush_print(dt('The following modules are overridden: (!modules) please update your features BEFORE you continue with pulling new code (drush fu or drush fe)', array('!modules' => implode(' | ', $overridden_modules))));
    return FALSE;
  }
  else {drush_print(dt('true'));
    return TRUE;
  }  
}

function _confirm() {
    if (drush_confirm(dt('Do you really want to force and continue?'))) {
      return TRUE;
    }
    else {
      drush_die('Aborting.');
    }
}