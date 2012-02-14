<?php
// $Id: schema.drush.inc,v 1.1.2.1 2010/02/01 17:38:06 mikeryan Exp $

/**
 * @file
 *   Schema drush commands.
 */

/**
 * Implementation of hook_drush_command().
 */
function schema_drush_command() {
  $items = array();

  // the key in the $items array is the name of the command.
  $items['schema-inspect'] = array(
    'description' => "Show the Drupal schema definition for table(s)",
    'arguments' => array(
      'tables' => 'A comma delimited list of table names',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_drush_help().
 */
function schema_drush_help($section) {
  switch ($section) {
    case 'drush:schema-inspect':
      return dt("Show the Drupal schema definition for table(s).");
  }
}

/**
 * A drush command callback.
 */
function drush_schema_schema_inspect() {
  $args = func_get_args();
  $names = explode(',', $args[0]);
  foreach ($names as $name) {
    if ($table = schema_invoke('inspect', $name)) {
      $output = schema_phpprint_table($name, $table[$name]);
      drush_print($output);
    }
    else {
      drush_set_error(dt('Mising table: @table', array('@table' => $name)));
    }
  }
}