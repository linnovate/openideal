<?php
/**
 * @file
 * fe_default.features.user_role.inc
 */

/**
 * Implements hook_user_default_roles().
 */
function fe_default_user_default_roles() {
  $roles = array();

  // Exported role: privilege
  $roles['privilege'] = array(
    'name' => 'privilege',
    'weight' => '4',
  );

  // Exported role: site-manager
  $roles['site-manager'] = array(
    'name' => 'site-manager',
    'weight' => '5',
  );

  return $roles;
}
