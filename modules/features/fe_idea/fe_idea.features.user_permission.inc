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

  // Exported permission: create field_challenge_ref.
  $permissions['create field_challenge_ref'] = array(
    'name' => 'create field_challenge_ref',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: create field_in_action.
  $permissions['create field_in_action'] = array(
    'name' => 'create field_in_action',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: create field_merge_to.
  $permissions['create field_merge_to'] = array(
    'name' => 'create field_merge_to',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
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

  // Exported permission: edit field_challenge_ref.
  $permissions['edit field_challenge_ref'] = array(
    'name' => 'edit field_challenge_ref',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit field_in_action.
  $permissions['edit field_in_action'] = array(
    'name' => 'edit field_in_action',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit field_merge_to.
  $permissions['edit field_merge_to'] = array(
    'name' => 'edit field_merge_to',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own field_challenge_ref.
  $permissions['edit own field_challenge_ref'] = array(
    'name' => 'edit own field_challenge_ref',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own field_in_action.
  $permissions['edit own field_in_action'] = array(
    'name' => 'edit own field_in_action',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: edit own field_merge_to.
  $permissions['edit own field_merge_to'] = array(
    'name' => 'edit own field_merge_to',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
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

  // Exported permission: view field_challenge_ref.
  $permissions['view field_challenge_ref'] = array(
    'name' => 'view field_challenge_ref',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view field_in_action.
  $permissions['view field_in_action'] = array(
    'name' => 'view field_in_action',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view field_merge_to.
  $permissions['view field_merge_to'] = array(
    'name' => 'view field_merge_to',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own field_challenge_ref.
  $permissions['view own field_challenge_ref'] = array(
    'name' => 'view own field_challenge_ref',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own field_in_action.
  $permissions['view own field_in_action'] = array(
    'name' => 'view own field_in_action',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  // Exported permission: view own field_merge_to.
  $permissions['view own field_merge_to'] = array(
    'name' => 'view own field_merge_to',
    'roles' => array(
      0 => 'administrator',
    ),
    'module' => 'field_permissions',
  );

  return $permissions;
}
