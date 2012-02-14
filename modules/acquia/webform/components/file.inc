<?php
// $Id: file.inc,v 1.4.2.35 2010/03/21 19:45:01 quicksketch Exp $

/**
 * @file
 *   Webform module file component.
 */

/**
 * Create a default file component.
 */
function _webform_defaults_file() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'email' => 1,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'filtering' => array(
        'types' => array('gif', 'jpg', 'png'),
        'addextensions' => '',
        'size' => 800,
      ),
      'savelocation' => '',
      'width' => '',
      'description' => '',
      'attributes' => array(),
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form for editing this component.
 * Use care naming the form items, as this correlates directly to the database schema.
 * The component "Name" and "Description" fields are added to every component type and
 * are not necessary to specify here (although they may be overridden if desired).
 * @return
 *   An array of form items to be displayed on the edit component page
 */
function _webform_edit_file($currfield) {
    $edit_fields = array();
    $edit_fields['#theme'] = 'webform_edit_file';
    $edit_fields['extra']['filtering'] = array(
      '#type' => 'fieldset',
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
      '#title' => t('Upload Filtering'),
      '#description' => t('Select the types of uploads you would like to allow.'),
      '#element_validate' => array('_webform_edit_file_filtering_validate'),
    );

    // Find the list of all currently valid extensions.
    $current_types = isset($currfield['extra']['filtering']['types']) ? $currfield['extra']['filtering']['types'] : array();

    $types = array('gif', 'jpg', 'png');
    $edit_fields['extra']['filtering']['types']['webimages'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Web Images'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('bmp', 'eps', 'tif', 'pict', 'psd');
    $edit_fields['extra']['filtering']['types']['desktopimages'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Desktop Images'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('txt', 'rtf', 'html', 'odf', 'pdf', 'doc', 'ppt', 'xls', 'xml');
    $edit_fields['extra']['filtering']['types']['documents'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Documents'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('avi', 'mov', 'mp3', 'ogg', 'wav');
    $edit_fields['extra']['filtering']['types']['media'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Media'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $types = array('bz2', 'dmg', 'gz', 'jar', 'rar', 'sit', 'tar', 'zip');
    $edit_fields['extra']['filtering']['types']['archives'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Archives'),
      '#options' => drupal_map_assoc($types),
      '#default_value' => array_intersect($current_types, $types),
    );

    $edit_fields['extra']['filtering']['addextensions'] = array(
      '#type' => 'textfield',
      '#title' => t('Additional Extensions'),
      '#default_value' => $currfield['extra']['filtering']['addextensions'],
      '#description' => t('Enter a list of additional file extensions for this upload field, seperated by commas.<br /> Entered extensions will be appended to checked items above.'),
      '#size' => 60,
      '#weight' => 3,
      '#default_value' => $currfield['extra']['filtering']['addextensions'],
    );

    $edit_fields['extra']['filtering']['size'] = array(
      '#type' => 'textfield',
      '#title' => t('Max Upload Size'),
      '#default_value' => $currfield['extra']['filtering']['size'],
      '#description' => t('Enter the max file size a user may upload (in KB).'),
      '#size' => 10,
      '#weight' => 3,
      '#default_value' => $currfield['extra']['filtering']['size'],
    );
    $edit_fields['extra']['savelocation'] = array(
      '#type' => 'textfield',
      '#title' => t('Upload Directory'),
      '#default_value' => $currfield['extra']['savelocation'],
      '#description' => '<div style="display: block">'. t('Webform uploads are always saved in the site files directory. You may optionally specify a subfolder to store your files.') .'</div>',
      '#weight' => 3,
      '#element_validate' => array('_webform_edit_file_check_directory', '_webform_edit_file_validate'),
      '#after_build' => array('_webform_edit_file_check_directory'),
    );
    $edit_fields['extra']['width'] = array(
      '#type' => 'textfield',
      '#title' => t('Width'),
      '#default_value' => $currfield['extra']['width'],
      '#description' => t('Width of the file field.') .' '. t('Leaving blank will use the default size.'),
      '#size' => 5,
      '#maxlength' => 10,
      '#weight' => 4,
    );
    return $edit_fields;
}

function _webform_edit_file_check_directory($form_element) {
  $base_dir = file_directory_path() .'/webform';
  $base_success = file_check_directory($base_dir, FILE_CREATE_DIRECTORY);

  $destination_dir = $base_dir .'/'. $form_element['#value'];
  $destination_success = file_check_directory($destination_dir, FILE_CREATE_DIRECTORY);
  if (!$base_success || !$destination_success) {
    form_set_error('savelocation', t('The save directory %directory could not be created. Check that the webform files directory is writtable.', array('%directory' => $destination_dir)));
  }
  return $form_element;
}

/**
 * Change the submitted values of the component so that all filtering extensions
 * are saved as a single array.
 */
function _webform_edit_file_filtering_validate($form_element, &$form_state) {
  // Predefined types.
  $extensions = array();
  foreach (element_children($form_element['types']) as $category) {
    foreach (array_keys($form_element['types'][$category]['#value']) as $extension) {
      if ($form_element['types'][$category][$extension]['#value']) {
        $extensions[] = $extension;
      }
    }
  }

  // Additional types.
  $additional_extensions = explode(',', $form_element['addextensions']['#value']);
  foreach ($additional_extensions as $extension) {
    $clean_extension = drupal_strtolower(trim($extension));
    if (!empty($clean_extension) && !in_array($clean_extension, $extensions)) {
      $extensions[] = $clean_extension;
    }
  }

  form_set_value($form_element['types'], $extensions, $form_state);
}

function theme_webform_edit_file($form) {
  // Add a little javascript to check all the items in one type.
  $javascript = '
    <script type="text/javascript">
      function check_category_boxes () {
        var checkValue = !document.getElementById("edit-extra-filtering-types-"+arguments[0]+"-"+arguments[1]).checked;
        for(var i=1; i < arguments.length; i++) {
          document.getElementById("edit-extra-filtering-types-"+arguments[0]+"-"+arguments[i]).checked = checkValue;
        }
      }
  </script>
 ';
  drupal_set_html_head($javascript);

  // Format the components into a table.
  $per_row = 5;
  $rows = array();
  foreach (element_children($form['extra']['filtering']['types']) as $key => $filtergroup) {
    $row = array();
    $first_row = count($rows);
    if ($form['extra']['filtering']['types'][$filtergroup]['#type'] == 'checkboxes') {
      // Add the title.
      $row[] = $form['extra']['filtering']['types'][$filtergroup]['#title'];
      $row[] = '&nbsp;';
      // Convert the checkboxes into individual form-items.
      $checkboxes = expand_checkboxes($form['extra']['filtering']['types'][$filtergroup]);
      // Render the checkboxes in two rows.
      $checkcount = 0;
      $jsboxes = '';
      foreach (element_children($checkboxes) as $key) {
        $checkbox = $checkboxes[$key];
        if ($checkbox['#type'] == 'checkbox') {
          $checkcount++;
          $jsboxes .= "'". $checkbox['#return_value'] ."',";
          if ($checkcount <= $per_row) {
            $row[] = array('data' => drupal_render($checkbox));
          }
          elseif ($checkcount == $per_row + 1) {
            $rows[] = array('data' => $row, 'style' => 'border-bottom: none;');
            $row = array(array('data' => '&nbsp;'), array('data' => '&nbsp;'));
            $row[] = array('data' => drupal_render($checkbox));
          }
          else {
            $row[] = array('data' => drupal_render($checkbox));
          }
        }
      }
      // Pretty up the table a little bit.
      $current_cell = $checkcount % $per_row;
      if ($current_cell > 0) {
        $colspan = $per_row - $current_cell   + 1;
        $row[$current_cell + 1]['colspan'] = $colspan;
      }
      // Add the javascript links.
      $jsboxes = drupal_substr($jsboxes, 0, drupal_strlen($jsboxes) - 1);
      $rows[] = array('data' => $row);
      $select_link = ' <a href="javascript:check_category_boxes(\''. $filtergroup .'\','. $jsboxes .')">(select)</a>';
      $rows[$first_row]['data'][1] = array('data' => $select_link, 'width' => 40);
      unset($form['extra']['filtering']['types'][$filtergroup]);
    }
    elseif ($filtergroup != 'size') {
      // Add other fields to the table (ie. additional extensions).
      $row[] = $form['extra']['filtering']['types'][$filtergroup]['#title'];
      unset($form['extra']['filtering']['types'][$filtergroup]['#title']);
      $row[] = array(
        'data' => drupal_render($form['extra']['filtering']['types'][$filtergroup]),
        'colspan' => $per_row + 1,
      );
      unset($form['extra']['filtering']['types'][$filtergroup]);
      $rows[] = array('data' => $row);
    }
  }
  $header = array(array('data' => t('Category'), 'colspan' => '2'), array('data' => t('Types'), 'colspan' => $per_row));

  // Create the table inside the form.
  $form['extra']['filtering']['types']['table'] = array(
    '#value' => theme('table', $header, $rows)
  );

  $output = drupal_render($form);

  // Prefix the upload location field with the default path for webform.
  $output = str_replace('Upload Directory: </label>', 'Upload Directory: </label>'. file_directory_path() .'/webform/', $output);

  return $output;
}

/**
 * Build a form item array containing all the properties of this component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   An array of a form item to be displayed on the client-side webform.
 */
function _webform_render_file($component) {
  $form_item[$component['form_key']] = array(
    '#type'          => $component['type'],
    '#title'         => $component['name'],
    //'#required'      => $component['mandatory'], // Drupal core bug with required file uploads.
    '#weight'        => $component['weight'],
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
    '#attributes'    => $component['extra']['attributes'],
    '#tree'          => FALSE, // file_check_upload assumes a flat $_FILES structure.
    '#prefix'        => '<div class="webform-component-'. $component['type'] .'" id="webform-component-'. $component['form_key'] .'">',
    '#suffix'        => '</div>',
    '#element_validate'      => array(
      '_webform_validate_file',
      '_webform_required_file',  // Custom required routine.
    ),
    '#webform_component' => $component,
  );
  $form_item['#weight'] = $component['weight'];
  $form_item['new'] = array(
    '#type' => 'hidden',
    '#weight' => $component['weight'],
    '#value' => $component['form_key'],
    '#tree' => TRUE,
  );
  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $form_item[$component['form_key']]['#size'] = $component['extra']['width'];
  }

  return $form_item;
}

function _webform_required_file($form_element, $form_state) {
  $component = $form_element['#webform_component'];
  $form_key = $component['form_key'];

  if (empty($_FILES['files']['name'][$form_key]) && $component['mandatory']) {
    form_set_error($form_key, t('%field field is required.', array('%field' => $component['name'])));
  }
}

function _webform_validate_file($form_element, &$form_state) {
  $component = $form_element['#webform_component'];
  $form_key = $component['form_key'];

  if (empty($_FILES['files']['name'][$form_key])) {
    return;
  }

  // Build a human readable list of extensions:
  $extensions = $component['extra']['filtering']['types'];
  $extension_list = '';
  if (count($extensions) > 1) {
    for ($n = 0; $n < count($extensions) - 1; $n++) {
      $extension_list .= $extensions[$n] .', ';
    }
    $extension_list .= 'or '. $extensions[count($extensions)-1];
  }
  else {
    $extension_list = $extensions[0];
  }

  if (in_array('jpg', $extensions)) {
    $extensions[] = 'jpeg';
  }

  $dot = strrpos($_FILES['files']['name'][$form_key], '.');
  $extension = drupal_strtolower(substr($_FILES['files']['name'][$form_key], $dot+1));
  if (!in_array($extension, $extensions)) {
    form_set_error($form_key, t("Files with the '%ext' extension are not allowed, please upload a file with a %exts extension.", array('%ext' => $extension, '%exts' => $extension_list)));
  }

  // Now let's check the file size (limit is set in KB).
  if ($_FILES['files']['size'][$form_key] > $component['extra']['filtering']['size'] * 1024) {
    form_set_error($form_key, t("The file '%filename' is too large (%filesize KB). Please upload a file %maxsize KB or smaller.", array('%filename' => $_FILES['files']['name'][$form_key], '%filesize' => (int)($_FILES['files']['size'][$form_key]/1024), '%maxsize' => $component['extra']['filtering']['size'])));
  }
}

/**
 * Perform additional server-side processing on the submitted data, such as
 * managing an uploaded file.
 * @param $data
 *   The POST data associated with the component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Nothing.
 */
function _webform_submit_file(&$data, $component) {
  $upload_dir = file_directory_path() .'/webform/'. $component['extra']['savelocation'];
  if (!empty($_FILES['files']['name'][$data['new']])) {
    if (file_check_directory($upload_dir, FILE_CREATE_DIRECTORY)) {
      $file_saved = file_save_upload($data['new'], array(), $upload_dir);
      if (!$file_saved) {
        drupal_set_message(t('The uploaded file %filename was unable to be saved. The destination directory may not be writable.', array('%filename' => $file_saved['filename'])), 'error');
      }
      else {
        @chmod($file_saved->filepath, 0664);
        file_set_status($file_saved, FILE_STATUS_PERMANENT);
        if (isset($data['existing']['filepath'])) {
          file_delete($data['existing']['filepath']);
        }
        $data = serialize((array)$file_saved);
      }
    }
    else {
      drupal_set_message(t('The uploaded file was unable to be saved. The destination directory does not exist.'), 'error');
    }
  }
  else {
    $data = serialize(array());
  }
}

/**
 * Format the output of emailed data for this component
 *
 * @param mixed $data
 *   A string or array of the submitted data.
 * @param array $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output to be included in the email.
 */
function theme_webform_mail_file($data, $component) {
  $file = is_string($data) ? unserialize($data) : $data;
  $output = $component['name'] .': '. (!empty($file['filepath']) ? webform_file_url($file['filepath']) : '') ."\n";
  return $output;
}

/**
 * Display the result of a file submission. The output of this function will be
 * displayed under the "results" tab then "submissions".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @param $enabled
 *   If enabled, the value may be changed. Otherwise it will set to readonly.
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_submission_display_file($data, $component, $enabled = FALSE) {
  $filedata = unserialize($data['value'][0]);
  $form_item = _webform_render_file($component);
  if (!$enabled) {
    $form_item['#type'] = 'textfield';
    $form_item['#tree'] = TRUE;
    $form_item['#attributes']['readonly'] = 'readonly';
    $form_item['#default_value'] = empty($filedata['filepath']) ? $filedata['error'] : $filedata['filepath'];
  }
  if (!empty($filedata['filename'])) {
    $form_item['#suffix'] = ' <a href="'. webform_file_url($filedata['filepath']) .'">Download '. webform_file_name($filedata['filepath']) .'</a>'. $form_item['#suffix'];
    if ($enabled) {
      $form_item['#description'] = t('Uploading a new file will replace the current file.');
      $form_item['existing'] = array(
        '#type' => 'value',
        '#value' => $filedata,
      );
    }
  }
  return $form_item;
}

/**
 * Delete operation for file components or submissions.
 *
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 */
function _webform_delete_file($data, $component) {
  // Delete an individual submission file.
  $filedata = unserialize($data['value']['0']);
  if (isset($filedata['filepath']) && is_file($filedata['filepath'])) {
    unlink($filedata['filepath']);
    db_query("DELETE FROM {files} WHERE filepath = '%s'", $filedata['filepath']);
  }
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_file($section) {
  switch ($section) {
    case 'admin/settings/webform#file_description':
      return t('Allow users to submit files of the configured types.');
  }
}

/**
 * Module specific instance of hook_theme().
 */
function _webform_theme_file() {
  return array(
    'webform_edit_file' => array(
      'arguments' => array('form' => NULL),
    ),
    'webform_mail_file' => array(
      'arguments' => array('data' => NULL, 'component' => NULL),
    ),
  );
}

/**
 * Calculate and returns statistics about results for this component from all
 * submission to this webform. The output of this function will be displayed
 * under the "results" tab then "analysis".
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema
 * @param $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will be limited
 *   to these sids.
 * @return
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _webform_analysis_rows_file($component, $sids = array()) {
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (".implode(",", $placeholders).")" : "";
  $query = 'SELECT data '.
    ' FROM {webform_submitted_data} '.
    ' WHERE nid = %d '.
    ' AND cid = %d'. $sidfilter;
  $nonblanks = 0;
  $sizetotal = 0;
  $submissions = 0;

  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  while ($data = db_fetch_array($result)) {
    $filedata = unserialize($data['data']);
    if (isset($filedata['filesize'])) {
      $nonblanks++;
      $sizetotal += $filedata['filesize'];
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User uploaded file'), $nonblanks);
  $rows[2] = array(t('Average uploaded file size'), ($sizetotal !=0 ? (int)(($sizetotal/$nonblanks)/1024) .' KB' : '0'));
  return $rows;
}

/**
 * Return the result of this component's submission for display in a table. The
 * output of this function will be displayed under the "results" tab then "table".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_table_data_file($data) {
  $output = '';
  $filedata = unserialize($data['value']['0']);
  if (!empty($filedata['filename'])) {
    $output = '<a href="'. webform_file_url($filedata['filepath']) .'">'. webform_file_name($filedata['filepath']) .'</a>';
    $output .= ' ('. (int)($filedata['filesize']/1024) .' KB)';
  }
  elseif (!empty($filedata['error'])) {
    $output = $filedata['error'];
  }
  return $output;
}

/**
 * Return the header information for this component to be displayed in a comma
 * seperated value file. The output of this function will be displayed under the
 * "results" tab then "download".
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   An array of data to be displayed in the first three rows of a CSV file, not
 *   including either prefixed or trailing commas.
 */
function _webform_csv_headers_file($component) {
  $header = array();
  // Two columns in header.
  $header[0] = array('', '');
  $header[1] = array($component['name'], '');
  $header[2] = array(t('Name'), t('Filesize (KB)'));
  return $header;
}

/**
 * Return the result of a file submission. The output of this function will be
 * displayed under the "results" tab then "submissions".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   Textual output formatted for CSV, not including either prefixed or trailing
 *   commas.
 */
function _webform_csv_data_file($data) {
  $filedata = unserialize($data['value']['0']);
  return empty($filedata['filename']) ? array('', '') : array(webform_file_url($filedata['filepath']), (int)($filedata['filesize']/1024));
}

/**
 * Helper function to create proper URLs for uploaded file.
 */
function webform_file_url($filepath) {
  if (!empty($filepath)) {
    $info = pathinfo($filepath);
    $file_url = file_create_url($info['dirname'] .'/'. rawurlencode($info['basename']));
  }
  return isset($file_url) ? $file_url : '';
}

/**
 * Helper function to create proper file names for uploaded file.
 */
function webform_file_name($filepath) {
  if (!empty($filepath)) {
    $info = pathinfo($filepath);
    $file_name = $info['basename'];
  }
  return isset($file_name) ? $file_name : '';
}
