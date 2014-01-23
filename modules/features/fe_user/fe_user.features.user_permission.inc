<?php
/**
 * @file
 * fe_user.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function fe_user_user_default_permissions() {
  $permissions = array();

  // Exported permission: access user profiles.
  $permissions['access user profiles'] = array(
    'name' => 'access user profiles',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'user',
  );

  // Exported permission: change own user settings.
  $permissions['change own user settings'] = array(
    'name' => 'change own user settings',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'watcher',
  );

  // Exported permission: use watcher.
  $permissions['use watcher'] = array(
    'name' => 'use watcher',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'watcher',
  );

  // Exported permission: view own userpoints.
  $permissions['view own userpoints'] = array(
    'name' => 'view own userpoints',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'userpoints',
  );

  // Exported permission: view userpoints.
  $permissions['view userpoints'] = array(
    'name' => 'view userpoints',
    'roles' => array(
      0 => 'administrator',
      1 => 'anonymous user',
      2 => 'authenticated user',
    ),
    'module' => 'userpoints',
  );

  return $permissions;
}
