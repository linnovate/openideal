<?php
// $Id: emfield.audit.inc,v 1.1.2.4 2010/10/22 18:43:23 aaron Exp $

/**
 * @file emfield/includes/emfield.audit.inc
 *
 * Perform an audit of providers on the server, to ensure all are present.
 */

/**
 * This will audit all nodes to ensure all emfield providers are present.
 *
 * The function will pass through all emfield fields to see if there is any
 * content in the system without a provider.
 *
 * @return array
 *  An array of all missing providers. Ideally the array will be empty.
 */
function _emfield_audit() {
  $audit = array();

  // Build a list of fields that need data updating.
  $fields = array();
  foreach (content_fields() as $field) {
    if (in_array($field['module'], array('emvideo', 'emimage', 'emaudio'))) {
      // We only process a given field once.
      $fields[$field['field_name']] = $field;
    }
  }

  // Update database storage.
  foreach ($fields as $field) {
    $db_info = content_database_info($field);
    $table = $db_info['table'];
    $provider_column = $db_info['columns']['provider']['column'];
    $module = $field['module'];
    $sql = "SELECT $provider_column FROM {{$table}} GROUP BY $provider_column";
    $results = db_query($sql);
    while ($result = db_fetch_object($results)) {
      $provider = emfield_system_list($module, $result->$provider_column);
      if (empty($provider)) {
        $audit[$result->$provider_column] = $result->$provider_column;
      }
    }
  }
  return $audit;
}
