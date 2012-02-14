<?php
// $Id: taxonomy.inc,v 1.1.4.9 2010/03/21 13:11:23 kiam Exp $

/**
 * @file
 * Integration file for taxonomy.module.
 */

/**
 * Implements hook_nodewords_type_id().
 */
function taxonomy_nodewords_type_id(&$result, $arg) {
  if ($arg[0] == 'taxonomy') {
    // Taxonomy paths: taxonomy/term/$tid , taxonomy/term/$tid1+$tid2.
    if (isset($arg[1]) && isset($arg[2]) && $arg[1] == 'term') {
      $ids = preg_split('![+, ]!', $arg[2]);

      if (count($ids)) {
        // Take the first ID that is numeric.
        foreach ($ids as $id) {
          if (is_numeric($id)) {
            $result['type'] = NODEWORDS_TYPE_TERM;
            $result['id'] = $id;
            return;
          }
        }
      }
    }
  }
}
