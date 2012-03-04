<?php

/**
 * @file
 * Admin pages for the gRaphaël module.
 */

/**
 * Admin settings form/
 */
function graphael_admin_settings_form($form, $form_state) {
  $form['graphael_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Path'),
    '#description' => t('An alternate path for the gRaphaël files. You only need to change this if you want to use your own version of the library.'),
    '#default_value' => variable_get('graphael_path', RAPHAEL_PATH),
  );

  return system_settings_form($form);
}

/**
 * Validate handler for graphael_admin_settings_form().
 */
function graphael_admin_settings_form_validate($form, &$form_state) {
  if (!count(file_scan_directory($form_state['values']['graphael_path'] . '/graphael/js', '/g.raphael.min.js$/', array('recurse' => FALSE)))) {
    form_set_error('graphael_path', t('Invalid path, could not find g.raphael.min.js file in selected directory.'));
  }
}

