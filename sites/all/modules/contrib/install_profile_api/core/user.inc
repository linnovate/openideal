<?php
// $Id: user.inc,v 1.1.2.7.2.1 2009/03/11 16:29:39 jamesandres Exp $

/**
 * Add a user.
 *
 * @param $username
 *   User's username name.
 * @param $password
 *   User's plain-text password.
 * @param $email
 *   User's e-mail address.
 * @param $roles
 *   An array of role names to which the user should be assigned.
 *   For example: array('staff', 'moderator')
 * @param $status
 *   User's status. 1 = active, 0 = blocked.
 */
function install_add_user($username, $password, $email, $roles = array(), $status = 1) {
  $role_assignments = array();
  // Translate array of names into rid => name that user_save() is expecting.
  if (!empty($roles)) {
    foreach ($roles as $name) {
      $rid = install_get_rid($name);
      $role_assignments[$rid] = $name;
    }
  }

  user_save(
    new stdClass(),
    array(
      'name' => $username, 
      'pass' => $password,
      'mail' => $email,
      'roles' => $role_assignments,
      'status' => $status
    )
  );	
}

/**
 * Add a user to a role.
 */
function install_add_user_to_role($uid, $rid) {
  if ($uid <= 0 || $rid <= 0) {
    return FALSE;
  }

  $count = db_result(db_query("SELECT COUNT(uid) FROM {users_roles} WHERE uid = %d AND rid = %d", $uid, $rid));
  if ($count == 0) {
    return db_query("INSERT INTO {users_roles} (uid, rid) VALUES (%d, %d)", $uid, $rid);
  }

  return FALSE;
}

/**
 * Add a role to the roles table.
 */
function install_add_role($name) {
  // Check to see if role already exists. If not, create it.
  $rid = install_get_rid($name);
  if (!$rid) {
    db_query("INSERT INTO {role} (name) VALUES ('%s')", $name);
  }
  
  return ($rid) ? $rid : install_get_rid($name);
}

/**
 * Get the role ID for the role name.
 */
function install_get_rid($name) {
  static $roles = array();
  if (empty($roles[$name])) {
    $roles[$name] = db_result(db_query_range("SELECT rid FROM {role} WHERE name ='%s'", $name, 0, 1));
  }
  return $roles[$name];
}

/**
 * Add the permission for a certain role.
 */
function install_add_permissions($rid, $perms) {
  // Retrieve the currently set permissions.
  $result = db_query("SELECT p.perm FROM {role} r INNER JOIN {permission} p ON p.rid = r.rid WHERE r.rid = %d ", $rid);
  $existing_perms = array();
  while ($row = db_fetch_object($result)) {
    $existing_perms += explode(', ', $row->perm);
  }
  // If this role already has permissions, merge them with the new permissions being set.
  if (count($existing_perms) > 0) { 
    $perms = array_unique(array_merge($perms, (array)$existing_perms));
  }

  // Update the permissions.
  db_query('DELETE FROM {permission} WHERE rid = %d', $rid);
  db_query("INSERT INTO {permission} (rid, perm) VALUES (%d, '%s')", $rid, implode(', ', $perms));
}

/**
* Remove permissions for a certain role.
*/
function install_remove_permissions($rid, $perms) {
  // Retrieve the currently set permissions.
  $result = db_query("SELECT p.perm FROM {role} r INNER JOIN {permission} p ON p.rid = r.rid WHERE r.rid = %d ", $rid);
  $existing_perms = array();
  while ($row = db_fetch_object($result)) {
    $existing_perms += explode(', ', $row->perm);
  }
  $new_perms = array_diff($existing_perms, $perms);
 
  // Update the permissions.
  db_query('DELETE FROM {permission} WHERE rid = %d', $rid);
  db_query("INSERT INTO {permission} (rid, perm) VALUES (%d, '%s')", $rid, implode(', ', $new_perms));
}
