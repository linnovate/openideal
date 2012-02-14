<?php
// $Id: og_activity.rules.inc,v 1.1.2.1.2.3 2010/02/21 22:45:26 stalski Exp $

/**
 * @file
 *   Activity integration for organic groups
 */

/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function og_activity_rules_event_info() {
  return array(
    'og_become_co_admin' => array(
      'label' => t('User becomes co-owner of a group'),
      'module' => 'og_activity',
      'arguments' => array(
        'node' => array('type' => 'node', 'label' => t('Group node to become an admin.')),
        'user' => array('type' => 'user', 'label' => t('User becoming the co-owner.')),
      ),
      'redirect' => TRUE,
    ),
  );
}

/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function og_activity_rules_condition_info() {
  return array(
    'og_rules_condition_admin_in_group' => array(
      'label' => t('User is group admin'),
      'arguments' => array(
        'user' => array(
          'type' => 'user',
          'label' => t('User'),
        ),
        'group' => array(
          'type' => 'node',
          'label' => t('Group'),
        ),
      ),
      'help' => t('Evaluates to TRUE if the user is the admin of the group.'),
      'module' => 'og_activity',
    ),
  );
}

/**
 * Function that expresses a condition to check if the user
 * is an admin in the group.
 * @ingroup rules
 */
function og_rules_condition_admin_in_group($user, $node, $settings) {

  // Because the node is not known yet in the user object in og_groups,
  // we have to check ourselves if the author of the created group is the current user
  return (!empty($node->nid) && $node->uid == $user->uid);
}


