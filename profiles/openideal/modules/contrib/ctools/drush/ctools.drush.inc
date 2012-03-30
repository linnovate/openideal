<?php

/**
 * @file
 * CTools Drush commands.
 */

/**
 * Implements hook_drush_command().
 */
function ctools_drush_command() {
  $items = array();

  $items['ctools-export'] = array(
    'aliases' => array('ctex'),
    'description' => 'Export multiple CTools-handled data objects directly to code.',
    'arguments' => array(
      'module'    => 'Name of your module.',
    ),
    'options' => array(
      'subdir' => 'The name of the sub directory to create the module in. Defaults to ctools_export which will be placed into sites/all/modules.',
    ),
    'bootstrap' => DRUSH_BOOTSTRAP_DRUPAL_FULL,
    'drupal dependencies' => array('bulk_export'),
    'examples' => array(
      'drush ctex export_module' => 'Export CTools exportables to a module called "export_module".',
    ),
  );

  $items['ctools-export-info'] = array(
    'aliases' => array('ctei'),
    'description' => 'Show available CTools exportables.',
    'arguments' => array(),
    'options' => array(
      'format' => 'Display exportables info in a different format such as print_r, json, export. The default is to show in a tabular format.',
      'tables-only' => 'Only show list of exportable types/table names and not available objects.',
    ),
    'examples' => array(
      'drush ctools-export-info' => 'View export info on all exportables.',
      'drush ctools-export-info views_view variable' => 'View export info for views_view and variable exportable types only.',
    ),
  );

  return $items;
}

/**
 * Drush callback: export
 */
function drush_ctools_export($module = 'foo') {
  $error = FALSE;
  if (preg_match('@[^a-z_]+@', $module)) {
    $error = dt('The name of the module must contain only lowercase letters and underscores') . '.';
    drush_log($error, 'error');
    return;
  }

  // Subdirectory.
  $dest_exists = FALSE;
  $subdir = drush_get_option('subdir', 'ctools_export');
  $dest = 'sites/all/modules/' . $subdir . '/' . $module;

  // Check if module or folder exists.
  if (module_exists($module) || file_exists($dest)) {
    $dest_exists = TRUE;
    if (drush_confirm(dt('Are you sure to overwrite files in folder !module ? An extra confirmation will be asked for the module file.', array('!module' => $module)))) {
      drush_log(dt('Files will be overwritten'), 'success');
    }
    else {
      drush_log(dt('Export aborted.'), 'success');
      return;
    }
  }

  // Create new dir if needed.
  if (!$dest_exists) {
    if (!file_exists('sites/all/modules/' . $subdir)) {
      drush_mkdir('sites/all/modules/' . $subdir);
    }
  }

  // Create destination directory.
  drush_mkdir($dest);

  // Create options and call Bulk export function.
  // We create an array, because maybe in the future we can pass in more
  // options to the export function (pre-selected modules and/or exportables).
  $options = array(
    'name' => $module,
  );
  $files = bulk_export_export(TRUE, $options);

  // Start writing.
  if (is_array($files)) {
    foreach ($files as $base_file => $data) {
      $filename = $dest . '/' . $base_file;
      // Extra check for .module file.
      if ($base_file == $module . '.module' && file_exists($filename)) {
        if (!drush_confirm(dt('Do you want to overwrite !module_file', array('!module_file' => $base_file)))) {
          drush_log(dt('Writing of !filename skipped. This is the code that was supposed to be written.', array('!filename' => $filename)), 'success');
          drush_print('---------');
          drush_print($data);
          drush_print('---------');
          continue;
        }
      }
      if (file_put_contents($filename, $data)) {
        drush_log(dt('Succesfully written !filename', array('!filename' => $filename)), 'success');
      }
      else {
        drush_log(dt('Error writing !filename', array('!filename' => $filename)), 'error');
      }
    }
  }
  else {
    drush_log(dt('No files were found to be written.'), 'error');
  }
}

/**
 * Drush callback: Export info.
 *
 * @params $table_names
 *  Each argument will be taken as a CTools exportable table name.
 */
function drush_ctools_export_info() {
  // Collect array of table names from args.
  $table_names = func_get_args();
  // Get info on these tables, or all if none specified.
  $exportables = _drush_ctools_export_info($table_names);
  // Get format option to allow for alternative output.
  $format = drush_get_option('format');
  $tables_only = drush_get_option('tables-only');

  if (empty($exportables)) {
    drush_log(dt('There are no exportables available.'), 'warning');
    return;
  }

  // Only use array keys if --types-only option is set.
  if ($tables_only) {
    $exportables = array_keys($exportables);
  }

  // Use format from --format
  if ($format) {
    drush_print(drush_format($exportables, $format, NULL));
  }
  // Build a tabular output as default.
  else {
    $header = $tables_only? array(dt("Base table")) : array(dt("Base table"), dt("Exportables"));
    $rows = array();
    foreach ($exportables as $table => $info) {
      if (!is_array($info)) {
        $row = array(
          $info
        );
      }
      else {
        $row = array(
          $table,
          implode("\n", $info) . "\n",
        );
      }
      $rows[] = $row;
    }
    array_unshift($rows, $header);
    drush_print_table($rows, TRUE);
  }

}

/**
 * Return array of CTools exportable info
 * based on available tables returned from
 * ctools_export_get_schemas().
 *
 * @param $table_names
 *  Array of table names to return.
 *
 * @return
 *  Nested arrays of available exportables, keyed by table name.
 */
function _drush_ctools_export_info($table_names = NULL) {
  ctools_include('export');
  // Get available schemas that declare exports.
  $schemas = ctools_export_get_schemas();
  $exportables = array();

  if (!empty($schemas)) {
    // Remove types we don't want, if any.
    if (!empty($table_names)) {
      foreach (array_keys($schemas) as $table_name) {
        if (!in_array($table_name, $table_names)) {
          unset($schemas[$table_name]);
        }
      }
    }
    // Load array of available exportables for each schema.
    foreach ($schemas as $table_name => $schema) {
      $exportables[$table_name] = ctools_export_default_list($table_name, $schema);
    }
  }

  return $exportables;
}
