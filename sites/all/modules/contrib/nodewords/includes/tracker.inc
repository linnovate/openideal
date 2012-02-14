<?php
// $Id: tracker.inc,v 1.1.4.7 2009/12/30 18:00:51 kiam Exp $

/**
 * @file
 * Integration file for tracker.module.
 */

/**
 * @addtogroup nodewords
 * @{
 */

/**
 * Implements hook_nodewords_type_id().
 */
function tracker_nodewords_type_id($arg) {
  if ($arg[0] == 'tracker') {
    return array(NODEWORDS_TYPE_TRACKER, array(0));
  }
}

/**
 * @} End of "addtogroup nodewords".
 */
