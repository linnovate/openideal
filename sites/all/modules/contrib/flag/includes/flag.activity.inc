<?php
// $Id: flag.activity.inc,v 1.1.2.1 2009/11/02 06:17:02 quicksketch Exp $

/**
 * @file
 * Provides Flag integration with Activity2.
 * @see http://drupal.org/project/activity
 */

/**
 * Implementation of hook_activity_grants().
 */
function flag_activity_grants($activity) {
  $flags = flag_get_flags();
  $realms = array();
  // Assign all those flags for nodes.
  foreach ($flags as $fid => $flag) {
    // This will also work for comments as those activity records have nids.
    if ($activity->nid && $flag->content_type == 'node') {
      $realms['flag_' . $flag->fid] = array($activity->nid);
    }
    elseif ($flag->content_type == 'user') {
      $realms['flag_' . $flag->fid] = array($activity->uid);
    }
  }
  return $realms;
}

/**
 * Implementation of hook_activity_access_grants().
 */
function flag_activity_access_grants($account) {
  // Get all the user and node flags.
  $user_flags = flag_get_user_flags('user', NULL, $account->uid);
  $node_flags = flag_get_user_flags('node', NULL, $account->uid);

  $flag_grants = array();
  if (!empty($node_flags)) {
    foreach ($node_flags as $flagged_objects) {
      foreach ($flagged_objects as $nid => $flagged) {
        // Tell activity to grant user $account access to those flagged with
        // $fid that are $nid.
        $flag_grants['flag_' . $flagged->fid][] = $nid;
      }
    }
  }

  if (!empty($user_flags)) {
    foreach ($user_flags as $flagged_objects) {
      foreach ($flagged_objects as $uid => $flagged) {
        // Tell Activity to grant user $account access to those flagged with
        // $fid that are $uid.
        $flag_grants['flag_' . $flagged->fid][] = $uid;
      }
    }
  }

  return $flag_grants;
}

/**
 * Implementation of hook_activity_type_check().
 */
function flag_activity_type_check($token_objects, $types) {
  return (in_array($token_objects['flag']->fid, $types));
}
