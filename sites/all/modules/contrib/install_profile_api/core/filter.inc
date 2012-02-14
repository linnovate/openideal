<?php
// $Id: filter.inc,v 1.1.2.4.2.2 2009/06/16 16:55:35 jamesandres Exp $

/**
 * Set the roles that can be used with the filter.
 *
 * @param $roles
 *   An array of role IDs.
 * @param $format_id
 *   An integer of the format ID.
 */
function install_format_set_roles($roles, $format_id) {
  $roles = implode(',',$roles);
  // Drupal core depends on the list of roles beginning and ending with commas.
  if (!empty($roles)) {
    $roles = ','. $roles .',';
  }
  db_query("UPDATE {filter_formats} SET roles = '%s' WHERE format = %d", $roles, $format_id);
}

/**
 * Add a new input format .
 *
 * @param $name
 *   The human-readable name of the new format.
 * @param $cache
 *   If this format is cacheable.
 * @return
 *   The new format ID.
 */
function install_add_format($name, $cache = 1) {
  db_query("INSERT INTO {filter_formats} (name, roles, cache) VALUES ('%s', '', %d)", $name, $cache);
  return db_last_insert_id('filter_formats', 'format');
}

/**
 * Remove the specified filter from the specified format.
 * @param $format_id
 *   The ID of the format to remove the filter from.
 * @param $module
 *   The module this filter belongs to.
 * @param $delta
 *   The delta of this filter.
 *
 * NOTE: the module name + the delta is what uniquely identifies a filter
 */
function install_remove_filter($format_id, $module, $delta) {	
  db_query("DELETE FROM {filters} WHERE format = %d AND module = '%s' AND delta = %d", $format_id, $module, $delta);
}

/**
* Add a filter to an input format. Note: This function is deprecated.
*
* @see install_set_filter()
*/
function install_add_filter($format_id, $module, $delta = 0, $weight = 0) {
  install_set_filter($format_id, $module, $delta, $weight);
}

/**
 * Update an existing a filter or insert a new one if not present.
 *
 * @param $format_id
 *   The ID of the format to add the filter to.
 * @param $module
 *   The module this filter belongs to.
 * @param $delta
 *   The delta of this filter.
 * @param $weight
 *   The weight to be applied to this filter.
 */
function install_set_filter($format_id, $module, $delta = 0, $weight = 0) {
  // if the format-module-delta exists, update
  if (db_result(db_query("SELECT `weight` FROM {filters} WHERE `format` = %d AND `module` = '%s' AND `delta` = %d", $format_id, $module, $delta)) !== FALSE) {
    db_query("UPDATE {filters} SET `weight` = %d WHERE `format` = %d AND `module` = '%s' AND `delta` = %d", $weight, $format_id, $module, $delta);
  }
  // otherwise insert a new record
  else {
    db_query("INSERT INTO {filters} (`format`, `module`, `delta`, `weight`) VALUES (%d, '%s', %d, %d)", $format_id, $module, $delta, $weight);
  }
}
