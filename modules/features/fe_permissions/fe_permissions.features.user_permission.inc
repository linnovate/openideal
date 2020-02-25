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

  // Exported permission: 'access all webform results'.
  $permissions['access all webform results'] = array(
    'name' => 'access all webform results',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'webform',
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

  // Exported permission: 'administer comments'.
  $permissions['administer comments'] = array(
    'name' => 'administer comments',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'administer faq order'.
  $permissions['administer faq order'] = array(
    'name' => 'administer faq order',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'faq',
  );

  // Exported permission: 'administer nodequeue'.
  $permissions['administer nodequeue'] = array(
    'name' => 'administer nodequeue',
    'roles' => array(
      'administrator' => 'administrator',
    ),
    'module' => 'nodequeue',
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

  // Exported permission: 'administer taxonomy'.
  $permissions['administer taxonomy'] = array(
    'name' => 'administer taxonomy',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'taxonomy',
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

  // Exported permission: 'administer views'.
  $permissions['administer views'] = array(
    'name' => 'administer views',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'views',
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

  // Exported permission: 'create faq content'.
  $permissions['create faq content'] = array(
    'name' => 'create faq content',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'create url aliases'.
  $permissions['create url aliases'] = array(
    'name' => 'create url aliases',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'path',
  );

  // Exported permission: 'delete all webform submissions'.
  $permissions['delete all webform submissions'] = array(
    'name' => 'delete all webform submissions',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'delete any faq content'.
  $permissions['delete any faq content'] = array(
    'name' => 'delete any faq content',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'delete revisions'.
  $permissions['delete revisions'] = array(
    'name' => 'delete revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit all webform submissions'.
  $permissions['edit all webform submissions'] = array(
    'name' => 'edit all webform submissions',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'webform',
  );

  // Exported permission: 'edit any faq content'.
  $permissions['edit any faq content'] = array(
    'name' => 'edit any faq content',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'edit boxes'.
  $permissions['edit boxes'] = array(
    'name' => 'edit boxes',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'boxes',
  );

  // Exported permission: 'edit own comments'.
  $permissions['edit own comments'] = array(
    'name' => 'edit own comments',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'comment',
  );

  // Exported permission: 'edit userpoints'.
  $permissions['edit userpoints'] = array(
    'name' => 'edit userpoints',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'userpoints',
  );

  // Exported permission: 'flag abuse_comment'.
  $permissions['flag abuse_comment'] = array(
    'name' => 'flag abuse_comment',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag abuse_node'.
  $permissions['flag abuse_node'] = array(
    'name' => 'flag abuse_node',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'flag abuse_user'.
  $permissions['flag abuse_user'] = array(
    'name' => 'flag abuse_user',
    'roles' => array(
      'administrator' => 'administrator',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'manipulate all queues'.
  $permissions['manipulate all queues'] = array(
    'name' => 'manipulate all queues',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'nodequeue',
  );

  // Exported permission: 'manipulate queues'.
  $permissions['manipulate queues'] = array(
    'name' => 'manipulate queues',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'nodequeue',
  );

  // Exported permission: 'reset vote up/down votes'.
  $permissions['reset vote up/down votes'] = array(
    'name' => 'reset vote up/down votes',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'vud',
  );

  // Exported permission: 'revert revisions'.
  $permissions['revert revisions'] = array(
    'name' => 'revert revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
  );

  // Exported permission: 'unflag abuse_comment'.
  $permissions['unflag abuse_comment'] = array(
    'name' => 'unflag abuse_comment',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag abuse_node'.
  $permissions['unflag abuse_node'] = array(
    'name' => 'unflag abuse_node',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'flag',
  );

  // Exported permission: 'unflag abuse_user'.
  $permissions['unflag abuse_user'] = array(
    'name' => 'unflag abuse_user',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'flag',
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

  // Exported permission: 'view revisions'.
  $permissions['view revisions'] = array(
    'name' => 'view revisions',
    'roles' => array(
      'administrator' => 'administrator',
      'site-manager' => 'site-manager',
    ),
    'module' => 'node',
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
