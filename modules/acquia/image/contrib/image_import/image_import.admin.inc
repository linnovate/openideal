<?php
// $Id: image_import.admin.inc,v 1.3.2.1 2010/08/03 17:43:00 sun Exp $

/**
 * Form builder function for Image Import settings form.
 */
function image_import_admin_settings() {
  $form['image_import_path'] = array(
    '#type' => 'textfield',
    '#title' => t('Import path'),
    '#default_value' => variable_get('image_import_path', file_directory_temp() . '/image'),
    '#description' => t("The directory to import image nodes from. Drupal will need to have write access to this directory so we can move the file.") .'<br />'
      . t("<strong>Note:</strong> a path begining with a <kbd>/</kbd> indicates the path is relative to the server's root, one starting without specifies a path relative to Drupal's root. I.e. <kbd>/tmp/image</kbd> would be the temp directory off the root while <kbd>tmp/image</kbd> would be inside Drupal's directory."),
    '#required' => TRUE,
  );
  $form = system_settings_form($form);
  // Apply our validation and submit handlers to the submit button.
  $form['buttons']['submit']['#validate'][] = 'image_import_admin_settings_submit_validate';
  $form['buttons']['submit']['#submit'][] = 'image_import_admin_settings_submit_submit';
  $form['buttons']['submit']['#submit'][] = 'system_settings_form_submit';
  return $form;
}

/**
 * Form validation handler for Image Import settings form.
 *
 * Checks the existence of the directory specified in $form_element.
 *
 * @see system_check_directory()
 */
function image_import_admin_settings_submit_validate($form, &$form_state) {
  $import_dir = $form_state['values']['image_import_path'];
  file_check_directory($import_dir, 0, 'image_import_path');
  $image_dir = variable_get('image_file_path', file_directory_path() . '/image');
  if (realpath($import_dir) == realpath($image_dir)) {
    form_set_error('image_import_path', t("You can't import from the image module's directory. The import deletes the original files so you would just be asking for trouble."));
  }
}

/**
 * Form submit handler for Image Import settings form.
 */
function image_import_admin_settings_submit_submit($form, &$form_state) {
  // Ensure that 'image_import_path' variable contains no trailing slash.
  $form_state['values']['image_import_path'] = rtrim($form_state['values']['image_import_path'], '/');

  drupal_set_message(t("Your settings are configured correctly, you can import images <a href='!image_import_page'>here</a>.", array('!image_import_page' => url('admin/content/image_import'))));
}

