<?php
/**
 * @file
 * fe_project.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function fe_project_user_default_permissions() {
  $permissions = array();

  // Exported permission: create project content
  $permissions['create project content'] = array(
    'name' => 'create project content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: delete any project content
  $permissions['delete any project content'] = array(
    'name' => 'delete any project content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: delete own project content
  $permissions['delete own project content'] = array(
    'name' => 'delete own project content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: edit any project content
  $permissions['edit any project content'] = array(
    'name' => 'edit any project content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: edit own project content
  $permissions['edit own project content'] = array(
    'name' => 'edit own project content',
    'roles' => array(
      0 => 'administrator',
      1 => 'site-manager',
    ),
    'module' => 'node',
  );

  return $permissions;
}
