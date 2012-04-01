<?php

/**
 * @file
 * Callbacks and access callbacks for userpoints services integration.
 */

/**
 * Access callback for viewing points of users.
 */
function userpoints_service_view_access($uid = NULL) {
  global $user;

  return user_access('view userpoints') || ($uid && user_access('view own userpoints') && $user->uid == $uid);
}

/**
 * Return an array of paged userpoints list.
 *
 * @param $page
 *   Page number of results to return (in pages of 20).
 *
 * @return
 *   An array of userpoints objects.
 **/
function userpoints_service_index($page, $tid, $sort, $dir) {
  if (!in_array($sort, array('points', 'uid', 'last_updated', 'max_points'))) {
    $sort = 'points';
  }

  if (strtoupper($dir) != 'ASC') {
    $dir = 'DESC';
  }

  $select = db_select('userpoints', 't')
    ->orderBy($sort, $dir);


  if ($tid != 'all') {
    if ($tid === NULL) {
      $tid = userpoints_get_default_tid();
    }
    $select->condition('tid', $tid);
  }
  services_resource_build_index_query($select, $page, 'uid, points, max_points', array());

  $results = $select->execute();

  return services_resource_build_index_list($results, 'userpoints', 'uid');
}

/**
 * Get the number of points of a given user.
 */
function userpoints_service_get($uid, $tid = NULL, $type = 'current') {
  if (!$uid) {
    return services_error(t('User ID parameter is required.'));
  }

  if ($tid === NULL) {
    $tid = userpoints_get_default_tid();
  }

  if ($type == 'max') {
    return userpoints_get_max_points($uid, $tid);
  }
  return userpoints_get_current_points($uid, $tid);
}

/**
 * Add points to a user.
 */
function userpoints_service_add($uid, $points, $tid, $operation, $description, $entity_type, $entity_id) {
  if (!$uid) {
    return services_error(t('User ID parameter is required.'));
  }

  if (!$points) {
    return services_error(t('Points parameter must be a negative or positive number.'));
  }

  $params = array(
    'uid' => $uid,
    'points' => $points,
    'tid' => $tid,
    'operation' => $operation,
    'description' => $description,
    'entity_type' => $entity_type,
    'entity_id' => $entity_id,
  );
  $result = userpoints_userpointsapi($params);

  if (!$result['status']) {
    return services_error(t('Adding points failed: @reason' ,array('@reason' => $result['reason'])));
  }

  return (object) array(
    'id' => $result['transaction']['txn_id'],
    'uri' => services_resource_uri(array('userpoints_transaction', $result['transaction']['txn_id'])),
  );
}