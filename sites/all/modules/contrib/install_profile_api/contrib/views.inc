<?php
// $Id: views.inc,v 1.1.2.4 2009/07/14 18:21:16 jamesandres Exp $

/**
 * Import a view from a file.
 *
 * @param $file
 *   The path to a file containing the export.
 * @param $name
 *   The name of the view, must be EXACTLY the same as the 'name'
 *   attribute in the export!
 */
function install_views_ui_import_from_file($file, $name = '') {
  if (!file_exists($file)) {
    return FALSE;
  }

  $export = file_get_contents($file);

  $form = array();
  $form_state = array();
  $form_state['values']['view'] = $export;
  $form_state['values']['name'] = $name;
  $form_state['values']['op'] = t('Import');

  module_load_include('inc', 'views', 'includes/admin');

  // Not using drupal_execute because it doesn't pass $form_state by
  // reference, specifically the 'view' element is lost.
  views_ui_import_validate($form, $form_state);
  views_ui_import_submit($form, $form_state);

  $form_state['view']->save();

  // Remove this view from cache so we can edit it properly.
  views_object_cache_clear('view', $form_state['view']->name);
}
