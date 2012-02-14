<?php

/**
 * @file
 * Provides hook documentation for the Popups API.
 */

/**
 * Creates the rule registry for the popups.
 */
function hook_popups() {
  $popups = array();
  $popups['admin/content/taxonomy'] = array(
    // Act on the first primary tab.
    'div#tabs-wrapper a:eq(1)',
    // Act on the 2nd column link in the table.
    'table td:nth-child(2) a' => array(
      // Don't update the original page.
      'noUpdate' => TRUE,
    ),
  );
  return $popups;
}

/**
 * Allows altering of the popup rule registry.
 *
 * @param $popups
 *   The popup registry to be altered.
 */
function hook_popups_alter(&$popups) {
  // Remove acting on the primary tabs.
  unset($popups['admin/content/taxonomy']['div#tabs-wrapper a:eq(1)']);

  // Make clicking on the link update the original page.
  $popups['admin/content/taxonomy']['table td:nth-child(2) a']['noUpdate'] = FALSE;
}

/**
 * Adds skins to the Popups API.
 *
 * Returns an associative array where the key is the skin name, along
 * with CSS and JS values to tell where the skin can be found.
 */
function hook_popups_skins() {
  $skin['My Skin'] = array(
    'css' => drupal_get_path('module', 'myskin') .'/myskin.css',
    'js' => drupal_get_path('module', 'myskin') .'/myskin.js',
  );
}
