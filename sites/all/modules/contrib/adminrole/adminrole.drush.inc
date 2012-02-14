<?php
// $Id: adminrole.drush.inc,v 1.1.2.1 2010/08/14 17:25:00 davereid Exp $

/**
 * @file
 * Drush integration functions for the adminrole module.
 */

/**
 * Implements hook_drush_command().
 */
function adminrole_drush_command() {
  $items['adminrole-update'] = array(
    'description' => 'Update the administrator role permissions.',
    'callback' => 'drush_adminrole_update',
    'drupal dependencies' => array('adminrole'),
  );
  return $items;
}

/**
 * Drush callback; update the admin role with all current available permissions.
 */
function drush_adminrole_update() {
  adminrole_update_permissions();
  drush_print(dt('Administrator role permissions updated.'));
}
