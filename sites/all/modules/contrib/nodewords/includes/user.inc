<?php
// $Id: user.inc,v 1.1.4.9 2010/03/21 13:11:23 kiam Exp $

/**
 * @file
 * Integration file for user.module.
 */

/**
 * Implements hook_nodewords_type_id().
 */
function user_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'user') {
    // User page paths: user/$uid.
    if (isset($arg[1]) && is_numeric($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_USER;
      $result['id'] = $arg[1];
    }
  }
}
