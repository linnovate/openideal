<?php
// $Id: taxonomy_export.inc,v 1.1.2.1 2009/06/25 15:01:28 jamesandres Exp $

/**
 * Import taxonomy, vocabularies and terms, from a file.
 *
 * @param $file
 *   The path to the file containing the export.
 * @param $format
 *   The export format, all taxonomy_xml module formats are supported
 *   including: xml, 
 **/
function install_taxonomy_export_import_from_file($file) {
  if (!file_exists($file)) {
    return FALSE;
  }

  $form_state = array(
    'values' => array(
      'op' => t('Import'),
      'import_data' => file_get_contents($file),
    ),
  );

  module_load_include('inc', 'taxonomy_export', 'taxonomy_export.pages');

  if (_taxonomy_export_prepare_import_data($form_state['values']['import_data'])) {
    taxonomy_export_import_submit(array(), $form_state);
  }
  else {
    drupal_set_message(t('Could not parse import data in "%file".', array('%file' => $file)), 'error');
  }
}
