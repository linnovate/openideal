<?php

/**
 * Implements hook_user_default_roles().
 */
function fe_main_user_default_roles() {
  $roles = array();

  // Exported role: administrator
  $roles['administrator'] = array(
    'name' => 'administrator',
  );

  // Exported role: privilege
  $roles['privilege'] = array(
    'name' => 'privilege',
  );

  // Exported role: site-manager
  $roles['site-manager'] = array(
    'name' => 'site-manager',
  );

  return $roles;
}
