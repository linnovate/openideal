<?php
// $Id: archive.action.inc,v 1.1.2.1 2010/03/08 08:15:48 kratib Exp $

function views_bulk_operations_archive_action_info() {
  return array('views_bulk_operations_archive_action' => array(
    'type' => 'file',
    'description' => t('Download archive of selected files'),
    'configurable' => FALSE,
    'aggregate' => TRUE,
  ));
}

function views_bulk_operations_archive_action($fids, $context) {
  // Iterate on files.
  $files = array();
  $f = db_query("SELECT * FROM {files} WHERE fid IN (%s)", implode(',', $fids));
  while ($file = db_fetch_object($f)) {
    $files[] = $file;
  }
  views_bulk_operations_archive_action_do($files, $context);
}

function views_bulk_operations_archive_action_do($files, $context) {
  // Create zip file.
  $zipfile = tempnam(file_directory_temp(), 'zip');
  $zip = new ZipArchive();
  if (!$zip->open($zipfile, ZIPARCHIVE::CREATE | ZIPARCHIVE::OVERWRITE)) {
    return;
  }
  foreach ($files as $file) {
    $zip->addFile(file_create_path($file->filepath), $file->filename);
  }
  $zip->close();

  // Download zip file.
  $view = views_get_view($context['view']['vid']);
  $zipname = $view->name .'-'. date('Ymd-His') .'.zip';
  header('Content-Type: application/zip');
  header('Content-Disposition: attachment; filename='. $zipname);
  header('Content-Transfer-Encoding: binary');
  header('Cache-Control: must-revalidate, post-check=0, pre-check=0');
  header('Pragma: public');
  header('Expires: 0');
  header('Content-Length: '. filesize($zipfile));
  readfile($zipfile);
  unlink($zipfile);
  exit;
}

