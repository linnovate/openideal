<?php
/**
 * @file
 * fe_permissions.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function fe_permissions_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'access administration menu'.
  $permissions['access administration menu'] = array(
    'name' => 'access administration menu',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'admin_menu',
  );

  // Exported permission: 'access administration pages'.
  $permissions['access administration pages'] = array(
    'name' => 'access administration pages',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access content overview'.
  $permissions['access content overview'] = array(
    'name' => 'access content overview',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'access contextual links'.
  $permissions['access contextual links'] = array(
    'name' => 'access contextual links',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'contextual',
  );

  // Exported permission: 'access dashboard'.
  $permissions['access dashboard'] = array(
    'name' => 'access dashboard',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'dashboard',
  );

  // Exported permission: 'access site reports'.
  $permissions['access site reports'] = array(
    'name' => 'access site reports',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'system',
  );

  // Exported permission: 'access statistics'.
  $permissions['access statistics'] = array(
    'name' => 'access statistics',
    'roles' => array(
      'administrator' => 'administrator',
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'statistics',
  );

  // Exported permission: 'administer analytics dashboard'.
  $permissions['administer analytics dashboard'] = array(
    'name' => 'administer analytics dashboard',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'analytics_dashboard',
  );

  // Exported permission: 'administer nodes'.
  $permissions['administer nodes'] = array(
    'name' => 'administer nodes',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'administer users'.
  $permissions['administer users'] = array(
    'name' => 'administer users',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'user',
  );

  // Exported permission: 'bypass node access'.
  $permissions['bypass node access'] = array(
    'name' => 'bypass node access',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'view analytics dashboard'.
  $permissions['view analytics dashboard'] = array(
    'name' => 'view analytics dashboard',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'analytics_dashboard',
  );

  // Exported permission: 'view the administration theme'.
  $permissions['view the administration theme'] = array(
    'name' => 'view the administration theme',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'system',
  );

  return $permissions;
}
