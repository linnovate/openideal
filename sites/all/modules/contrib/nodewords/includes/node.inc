<?php
// $Id: node.inc,v 1.1.4.10 2010/03/21 13:11:23 kiam Exp $

/**
 * @file
 * Integration file for node.module.
 */

/**
 * Implements hook_nodewords_type_id().
 */
function node_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'node') {
    // Node paths: node/$nid.
    if (isset($arg[1]) && is_numeric($arg[1]) && ($node = menu_get_object())) {
      $bool = (
        (
          $node->type == 'poll' &&
          isset($arg[2]) &&
          in_array($arg[2], array('edit', 'results'))
        ) ||
        !isset($arg[2])
      );
      if ($bool) {
        $result['type'] = NODEWORDS_TYPE_NODE;
        $result['id'] = $node->nid;
      }
    }
  }
}
