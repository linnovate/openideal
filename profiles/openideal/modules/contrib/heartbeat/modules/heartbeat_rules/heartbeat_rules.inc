<?php

/**
 * @file
 * Rules action callbacks.
 *
 */

/**
 * Rules action to log activity.
 */
function rules_action_heartbeat_activity_log() {

  //$entity_id, $uid, $entity_id_target, $uid_target, $message_id, $variables
  $args = func_get_args();

  array_pop($args);
  array_pop($args);
  $rulesState = array_pop($args);

  // Prepare the arguments we need.
  $arguments = $rulesState->currentArguments;
  unset($arguments['settings'], $arguments['state']);

  // Fix the message id and load the template.
  $info = $arguments['message_id']->info();
  $message_id = $info['default value'];
  $variables = array('variables' => array());

  ctools_include('export');
  $template = ctools_export_crud_load('heartbeat_messages', $message_id);
  foreach ($arguments as $key => $value) {
    if (isset($template->variables[$key])) {
      $variables['variables'][$key] = $value;
    }
    else {
      $variables[$key] = $value;
    }
    if (in_array($key, array('nid', 'uid', 'nid_target', 'uid_target'))) {
      $variables[$key] = (int) $variables[$key];
    }
  }

  $variables['access'] = $template->perms;
  $variables['message_id'] = $message_id;

  // Break logging if no actor is specified.
  if (!is_numeric($variables['uid'])) {
    return;
  }

  heartbeat_log($variables, $variables['variables']);

}