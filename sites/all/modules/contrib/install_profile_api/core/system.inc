<?php

/**
 * Example usage:
 * // Set site theme
 * install_disable_theme("garland");
 * install_default_theme("mytheme");
 */

/**
 * Enable a theme.
 *
 * @param $theme
 *   Unique string that is the name of theme.
 */
function install_enable_theme($theme) {
  system_theme_data();
  db_query("UPDATE {system} SET status = 1 WHERE type = 'theme' and name = '%s'", $theme);
  system_initialize_theme_blocks($theme);
}

/**
 * Disable a theme.
 *
 * @param $theme
 *   Unique string that is the name of theme.
 */
function install_disable_theme($theme) {
  system_theme_data();
  db_query("UPDATE {system} SET status = 0 WHERE type = 'theme' and name ='%s'", $theme);
}

/**
 * Set default theme.
 *
 * @param $theme
 *   Unique string that is the name of theme.
 */
function install_default_theme($theme) {
  global $theme_key;
  install_enable_theme($theme);
  variable_set('theme_default', $theme);
  // update the global variable too,
  // mainly so that block functions work correctly
  $theme_key = $theme;
}

/**
 * Set the admin theme.
 *
 * @param $theme
 *   Unique string that is the name of theme.
 */
function install_admin_theme($theme) {
  variable_set('admin_theme', $theme);
}


/**
 * Based on file_save_upload, this function simulates a file upload and returns
 * a file object which can be used anywhere an uploaded file and/or file id
 * (fid) is needed.  Almost a direct copy of file_save_upload, except that
 * $source is a filepath, rather than an upload identifier.
 * 
 * @param $source
 *   A string specifying the filepath to save.
 * @param $validators
 *   An optional, associative array of callback functions used to  validate the
 *   file. The keys are function names and the values arrays of callback
 *   parameters which will be passed in after the user and file objects. The
 *   functions should return an array of error messages, an empty array
 *   indicates that the file passed validation. The functions will be called
 *   in the order specified.
 * @param $dest
 *   A string containing the directory $source should be copied to. If this is
 *   not provided or is not writable, the temporary directory will be used.
 * @param $replace
 *   A boolean indicating whether an existing file of the same name in the
 *   destination directory should overwritten. A false value will generate a
 *   new, unique filename in the destination directory.
 */
function install_upload_file($source, $validators = array(), $dest = FALSE, $replace = FILE_EXISTS_RENAME, $mimetype = '' ) {
  global $user;
  static $upload_cache;

  // Add in our check of the the file name length.
  $validators['file_validate_name_length'] = array();

  // Return cached objects without processing since the file will have
  // already been processed and the paths in _FILES will be invalid.
  if (isset($upload_cache[$source])) {
    return $upload_cache[$source];
  }

  // If a file was uploaded, process it.
  if (file_exists($source)) {
    // Build the list of non-munged extensions.
    // @todo: this should not be here. we need to figure out the right place.
    $extensions = '';
    foreach ($user->roles as $rid => $name) {
      $extensions .= ' '. variable_get("upload_extensions_$rid",
      variable_get('upload_extensions_default', 'jpg jpeg gif png txt html doc xls pdf ppt pps odt ods odp'));
    }

    // Begin building file object.
    $file = new stdClass();
    $file->filename = file_munge_filename(trim(basename($source), '.'), $extensions);
    $file->filepath = $source;
    $file->filemime = $mimetype;

    // Rename potentially executable files, to help prevent exploits.
    if (preg_match('/\.(php|pl|py|cgi|asp|js)$/i', $file->filename) && (substr($file->filename, -4) != '.txt')) {
      $file->filemime = 'text/plain';
      $file->filepath .= '.txt';
      $file->filename .= '.txt';
    }

    // If the destination is not provided, or is not writable, then use the
    // temporary directory.
    if (empty($dest) || file_check_directory($dest, FILE_CREATE_DIRECTORY) === FALSE) {
      $dest = file_directory_temp();
    }

    $file->source = $source;
    $file->destination = file_destination(file_create_path($dest .'/'. $file->filename), $replace);
    $file->filesize = filesize($source);

    // Call the validation functions.
    $errors = array();
    foreach ($validators as $function => $args) {
      array_unshift($args, $file);
      $errors = array_merge($errors, call_user_func_array($function, $args));
    }

    // Check for validation errors.
    if (!empty($errors)) {
      $message = t('The selected file %name could not be uploaded.', array('%name' => $file->filename));
      if (count($errors) > 1) {
        $message .= '<ul><li>'. implode('</li><li>', $errors) .'</li></ul>';
      }
      else {
        $message .= ' '. array_pop($errors);
      }
      form_set_error($source, $message);
      return 0;
    }

    file_copy($file->filepath, $file->destination, $replace);

    // If we made it this far it's safe to record this file in the database.
    $file->uid = $user->uid;
    $file->status = FILE_STATUS_TEMPORARY;
    $file->timestamp = time();
    drupal_write_record('files', $file);

    // Add file to the cache.
    $upload_cache[$source] = $file;
    return $file;
  }
  return 0;
}
