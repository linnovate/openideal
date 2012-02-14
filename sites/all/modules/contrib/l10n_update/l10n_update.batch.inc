<?php

/**
 * @file
 *   Reusable API for creating and running l10n update batches.
 */

include_once 'l10n_update.check.inc';

/**
 * Create a batch to just download files.
 *
 * @param $updates
 *   Translations sources to be downloaded.
 *   Note: All update sources must have a 'fileurl'.
 * @return array
 *   A batch definition for this download.
 */
function l10n_update_batch_download($updates) {
  foreach ($updates as $update) {
    $operations[] = array('_l10n_update_batch_download', array($update));
  }
  return _l10n_update_create_batch($operations);
}

/**
 * Create a batch to just import files.
 *
 * All update sources must have a 'filepath'.
 *
 * @param $updates
 *   Translations sources to be imported.
 *   Note: All update sources must have a 'fileurl'.
 * @param $import_mode
 *   Import mode. How to treat existing and modified translations.
 * @return array
 *   A batch definition for this import.
 */
function l10n_update_batch_import($updates, $import_mode) {
  foreach ($updates as $update) {
    $operations[] = array('_l10n_update_batch_import', array($update, $import_mode));
  }
  return _l10n_update_create_batch($operations);
}

/**
 * Create a big batch for multiple projects and languages.
 *
 * @param $updates
 *   Array of update sources to be run.
 * @param $mode
 *   Import mode. How to treat existing and modified translations.
 * @return array
 */
function l10n_update_batch_multiple($updates, $import_mode) {
  foreach ($updates as $update) {
    if ($update->type == 'download') {
      $operations[] = array('_l10n_update_batch_download', array($update));
      $operations[] = array('_l10n_update_batch_import', array(NULL, $import_mode));
    }
    else {
      $operations[] = array('_l10n_update_batch_import', array($update, $import_mode));
    }
    // This one takes always parameters from results.
    $operations[] = array('_l10n_update_batch_history', array(NULL));
  }
  if (!empty($operations)) {
    return _l10n_update_create_batch($operations);
  }
}

/**
 * Create batch stub for this module.
 *
 * @param $operations
 *   Operations to perform in this batch.
 * @return array
 *   A batch definition:
 *   - 'operations': Batch operations
 *   - 'title': Batch title.
 *   - 'init_message': Initial batch UI message.
 *   - 'error_message': Batch error message.
 *   - 'file': File containing callback function.
 *   - 'finished': Batch completed callback function.
 */
function _l10n_update_create_batch($operations = array()) {
  $t = get_t();
  $batch = array(
    'operations'    => $operations,
    'title'         => $t('Updating translation.'),
    'init_message'  => $t('Downloading and importing files.'),
    'error_message' => $t('Error importing interface translations'),
    'file'          => drupal_get_path('module', 'l10n_update') . '/l10n_update.batch.inc',
    'finished'      => '_l10n_update_batch_finished',
  );
  return $batch;
}

/**
 * Batch process: Download a file.
 *
 * @param $update
 *   Source object to be downloaded.
 * @param $context
 *   Batch context array.
 */
function _l10n_update_batch_download($update, &$context) {
  $t = get_t();
  if (l10n_update_source_download($update)) {
    $context['message'] = $t('Importing downloaded translation: %url.', array('%url' => $update->fileurl));
    $context['results'][] = $update;
  }
  else {
    drupal_set_message($t('Failed download from %url', array('%url' => $update->fileurl)), 'error');
  }
}

/**
 * Batch process: Update the download history table.
 *
 * @param $update
 *   Source object to be updated.
 * @param $context
 *   Batch context array.
 */
function _l10n_update_batch_history($update, &$context) {
  if ($update = _l10n_update_batch_param($update, $context)) {
    l10n_update_source_history($update);
    $context['results'][] = $update;
  }
}

/**
 * Batch process: Import translation file.
 *
 * This takes a file parameter or continues from previous batch
 * which should have downloaded a file.
 *
 * @param $file
 *   File to be imported. If empty, the file will be taken from $context['results'].
 * @param $mode
 *   Import mode. How to treat existing and modified translations.
 * @param $context
 *   Batch context array.
 */
function _l10n_update_batch_import($file, $mode, &$context) {
  $t = get_t();
  if ($file = _l10n_update_batch_param($file, $context)) {
    if (l10n_update_source_import($file, $mode)) {
      $context['results'][] = $file;
      if ($file->type == 'download') {
        drupal_set_message($t('Successfully downloaded and imported translation from %url', array('%url' => $file->fileurl)));
      }
      else {
        drupal_set_message($t('Imported translation file %name.', array('%name' => $file->filepath)));
      }
    }
    else {
      drupal_set_message($t('Failed import of translation file %name.', array('%name' => $file->filepath)), 'error');
    }
  }
}

/**
 * Batch process: Get parameter from results of previous batch if not present.
 *
 * @param $param
 *   Batch parameters.
 * @param $context
 *   Batch context array.
 * @return unknown_type
 *   Batch results.
 */
function _l10n_update_batch_param($param, &$context) {
  if (isset($param)) {
    return $param;
  }
  elseif (!empty($context['results'])) {
    return array_pop($context['results']);
  }
}

/**
 * Batch finished callback: Set result message.
 *
 * @param $success
 *   TRUE if batch succesfully completed.
 * @param $results
 *   Batch results.
 */
function _l10n_update_batch_finished($success, $results) {
  $t = get_t();
  if ($success) {
    drupal_set_message($t('Successfully imported translations.'));
  }
  else {
    drupal_set_message($t('Error importing translations.'), 'error');
  }
}
