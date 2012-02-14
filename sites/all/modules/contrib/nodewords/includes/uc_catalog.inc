<?php
// $Id: uc_catalog.inc,v 1.1.2.5 2010/03/21 13:11:23 kiam Exp $

/**
 * @file
 * Integration file for uc_catalog.module.
 */

/**
 * Implements hook_nodewords_type_id().
 */
function uc_catalog_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'catalog') {
    if (isset($arg[1]) && is_numeric($arg[1])) {
      $result['type'] = NODEWORDS_TYPE_TERM;
      $result['id'] = $arg[1];
    }
  }
}
