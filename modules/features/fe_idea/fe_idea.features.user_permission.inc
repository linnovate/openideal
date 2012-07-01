<?php
/**
 * @file
 * fe_idea.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function fe_idea_user_default_permissions() {
  $permissions = array();

  // Exported permission: admin ideas.
  $permissions['admin ideas'] = array(
    'name' => 'admin ideas',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'ideal_main',
  );

  // Exported permission: create idea content.
  $permissions['create idea content'] = array(
    'name' => 'create idea content',
    'roles' => array(
      0 => 'authenticated user',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any idea content.
  $permissions['delete any idea content'] = array(
    'name' => 'delete any idea content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own idea content.
  $permissions['delete own idea content'] = array(
    'name' => 'delete own idea content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any idea content.
  $permissions['edit any idea content'] = array(
    'name' => 'edit any idea content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own idea content.
  $permissions['edit own idea content'] = array(
    'name' => 'edit own idea content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: use vote up/down.
  $permissions['use vote up/down'] = array(
    'name' => 'use vote up/down',
    'roles' => array(
      0 => 'administrator',
      1 => 'authenticated user',
    ),
    'module' => 'vud',
  );

  return $permissions;
}
