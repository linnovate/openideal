<?php
// $Id: image_import.pages.inc,v 1.7.2.2 2010/08/03 17:43:00 sun Exp $

/**
 * Verify a candidate file for import.
 *
 * @param $file
 *  A file object to check, of the format returned by file_scan_directory().
 */
function image_import_validate_file($file) {
  $errors = array();
  $info = image_get_info($file->filename);
  if ($info && !empty($info['extension'])) {
    $file->height = $info['height'];
    $file->width = $info['width'];
  }
  else {
    $errors[] = t('Not a JPG, GIF or PNG file.');
    $file->height = $file->width = 0;
  }

  // Check files are not too large to import.
  $file->filesize = filesize($file->filename);
  if ($file->filesize > variable_get('image_max_upload_size', 800) * 1024) {
    $errors[] = t('File too big.');
  }

  return $errors;
}

/**
 * Form generating function for the image import page.
 */
function image_import_form() {
  $form = array();

  $dirpath = variable_get('image_import_path', '');
  if (!file_check_directory($dirpath)) {
    drupal_set_message(t("You need to configure the import directory on the image import module's <a href='!admin-settings-import'>settings page</a>.", array('!admin-settings-import' => url('admin/settings/image/image_import'))), 'error');
    return $form;
  }

  $form['#dirpath'] = $dirpath;
  $form['#node_type'] = 'image';

  $files = file_scan_directory($dirpath, '.*');
  ksort($files);

  // When the form gets too large we end up running out of memory submitting it.
  // To avoid this we use a pager and rather than setting up all the variables
  // ourself we just send in a fake query and then select the desired part of
  // the files array.
  $page_size = variable_get('image_import_page_size', 50);
  pager_query('SELECT %d', $page_size, 0, 'SELECT %d', array(count($files)));
  $files = array_slice($files, $GLOBALS['pager_page_array'][0] * $page_size, $page_size);

  $file_metadata_modules = module_implements('file_metadata');

  if ($files) {
    if (module_exists('taxonomy')) {
      // here's a little hack to get the taxonomy controls onto our form
      $form['type'] = array('#type' => 'value', '#value' => $form['#node_type']);
      $form['#node'] = new stdClass();
      $form['#node']->type = $form['#node_type'];
      taxonomy_form_alter($form, array(), $form['#node_type'] . '_node_form');
      unset($form['type']);
      unset($form['#node']);
    }
    if (module_exists('image_gallery')) {
      $form['tree'] = array(
        '#type' => 'checkbox',
        '#title' => t('Reproduce directories structure with subgalleries'),
        '#description' => t('A subgallery will be created only if the folder contains at least an image.'),
      );
    }

    $form['delete_directories'] = array(
      '#type' => 'checkbox',
      '#title' => t('Delete empty directories after import'),
      '#description' => '',
    );

    // Put the image files into an array for the checkboxes and gather
    // additional information like dimensions and filesizes. Make sure that
    // there's no 0th element, because a checkbox with a zero value is seen as
    // unchecked and won't be imported.
    $index = 0;
    foreach ($files as $file) {
      $index++;
      $filelist[$index] = substr($file->filename, strlen($dirpath) + 1);

      $problems = image_import_validate_file($file);
      // Allow other modules to supply metadata about the images being imported.
      // hook_file_metadata() may populate the $file properties 'title' and
      // 'description'.
      foreach ($file_metadata_modules as $module) {
        $function = $module . '_file_metadata';
        $function($file);
      }

      // Spit out the import form elements.
      $form['files']['import'][$index] = array(
        '#type' => 'checkbox',
        '#title' => substr($file->filename, strlen($dirpath) + 1),
      );
      $form['files']['filesize'][$index] = array(
        '#type' => 'item',
        '#value' => format_size(filesize($file->filename)),
      );
      $form['files']['dimensions'][$index] = array(
        '#type' => 'item',
        '#value' => $file->width . 'x' . $file->height,
      );
      $form['files']['title'][$index] = array(
        '#type' => 'textfield',
        '#size' => 20,
        '#default_value' => isset($file->title) ? $file->title : basename($file->name),
      );
      $form['files']['body'][$index] = array(
        '#type' => 'textfield',
        '#size' => 20,
        '#default_value' => isset($file->body) ? $file->body : basename($file->name),
      );

      // If there were problems don't let them import it
      if (count($problems)) {
        $form['files']['import'][$index]['#type'] = 'item';
        $form['files']['errors'][$index] = array(
          '#type' => 'markup',
          '#value' => '<em>' . implode(' ', $problems) . '</em>',
        );
        unset($form['files']['title'][$index]);
        unset($form['files']['body'][$index]);
      }
    }

    $form['pager'] = array('#value' => theme('pager', NULL, $page_size, 0));

    // Put the titles into an array.
    $form['files']['import']['#tree'] = TRUE;
    $form['files']['title']['#tree'] = TRUE;
    $form['files']['body']['#tree'] = TRUE;

    // Store a copy of the list into a form value so we can compare it to what
    // they submit and not have to worry about files being added or removed
    // from the filesystem.
    $form['file_list'] = array(
      '#type' => 'value',
      '#value' => $filelist,
    );

    $form['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Import'),
    );
  }
  else {
    $form['none_found'] = array(
      '#type' => 'item',
      '#value' => '<em>' . t('No files were found.') . '</em>',
    );
  }

  return $form;
}

/**
 * Theme function for the image import form.
 */
function theme_image_import_form($form) {
  $output = drupal_render($form['token_help']);

  if (!empty($form['file_list']['#value'])) {
    $type = node_get_types('type', $form['#node_type']);
    $header = array(theme('table_select_header_cell'), t('Image'), t('Name'), t('Size'), t('Dimensions'), check_plain($type->title_label), check_plain($type->body_label));
    $rows = array();
    foreach (element_children($form['files']['import']) as $key) {
      $filename = $form['files']['import'][$key]['#title'];
      unset($form['files']['import'][$key]['#title']);
      $row = array(
        array('data' => drupal_render($form['files']['import'][$key])),
        array('data' => '<img width="40" src="' . url($form['#dirpath'] . '/' . $filename) . '">'),
        array('data' => $filename),
        array('data' => drupal_render($form['files']['filesize'][$key])),
        array('data' => drupal_render($form['files']['dimensions'][$key])),
      );
      if (!isset($form['files']['errors'][$key])) {
        $row[] = array('data' => drupal_render($form['files']['title'][$key]));
        $row[] = array('data' => drupal_render($form['files']['body'][$key]));
      }
      else {
        $row[] = array('colspan' => 2, 'data' => drupal_render($form['files']['errors'][$key]));
      }

      $rows[] = $row;
    }
    $output .= theme('table', $header, $rows);
  }
  return $output . drupal_render($form);
}

/**
 * Submit handler for the image import form.
 */
function image_import_form_submit($form, &$form_state) {
  $batch = array(
    'title' => t('Importing image'),
    'progress_message' => 'Importing @current of @total.',
    'operations' => array(),
    'finished' => '_image_import_batch_finished',
    'file' => drupal_get_path('module', 'image_import') . '/image_import.pages.inc',
  );

  foreach (array_filter($form_state['values']['import']) as $index => $true) {
    $origname = $form_state['values']['file_list'][$index];
    if ($filepath = file_check_location($form['#dirpath'] . '/' . $origname, $form['#dirpath'])) {
      $args = array(
        'node_type' => $form['#node_type'],
        'title' => isset($form_state['values']['title'][$index]) ? $form_state['values']['title'][$index] : NULL,
        'body' => isset($form_state['values']['body'][$index]) ? $form_state['values']['body'][$index] : NULL,
        'taxonomy' => isset($form_state['values']['taxonomy']) ? $form_state['values']['taxonomy'] : array(),
        'filepath' => $filepath,
        'origname' => $origname,
        'subgallery' => $form_state['values']['tree'],
      );
      $batch['operations'][] = array('_image_import_batch_op', array($args));
    }
  }
  if ($form_state['values']['delete_directories']) {
    $batch['operations'][] = array('_image_import_recursive_delete_empty_directories_batch_op', array(array('basepath' => $form['#dirpath'])));
  }

  batch_set($batch);
}

/**
 * Batch operation callback for image import.
 */
function _image_import_batch_op($args, &$context) {
  $error = FALSE;
  // if user choses to reproduce directories structure, save the chosen gallery,
  // then create, if needed, the nested galleries where the image goes
  if ($args['subgallery']) {
    static $subgalleries = array();
    $image_galleries_vid = _image_gallery_get_vid();
    $subgalleries_parent_tid = $args['taxonomy'][$image_galleries_vid];
    $args['taxonomy'][$image_galleries_vid] = _image_import_create_subgalleries(
      $args['origname'],
      $image_galleries_vid,
      $subgalleries_parent_tid,
      $subgalleries,
      $context
    );
    if (!$args['taxonomy'][$image_galleries_vid]) {
      $error = TRUE;
    }
  }

  // Create the node object.
  if (!$error && $node = image_create_node_from($args['filepath'], $args['title'], $args['body'], $args['taxonomy'])) {
    // Remove the original image now that the import has completed.
    file_delete($args['filepath']);

    $context['results']['good'][] = t('Imported %origname as !link @status [!edit-link].', array(
      '%origname' => $args['origname'],
      '!link' => l($node->title, 'node/' . $node->nid),
      '@status' => $node->status ? '' : t('(Unpublished)'),
      '!edit-link' => l(t('edit'), 'node/' . $node->nid . '/edit'),
    ));
  }
  else {
    watchdog('image_import', 'There was an error that prevented %filename from being imported.', array('%filename' => $args['filepath']), WATCHDOG_ERROR);
    $context['results']['bad'][] = t('Error importing %filename.', array('%filename' => $args['filepath']));
  }

  $context['finished'] = 1;
}

/**
 * Batch finished callback for image import.
 */
function _image_import_batch_finished($success, $results, $operations) {
  if (!$success) {
    if (count($results['bad'])) {
      drupal_set_message(t('There was a problem importing files: !bad-list', array('!bad-list' => theme('item_list', $results['bad']))), 'error');
    }
    else {
      drupal_set_message(t('There was a problem importing the files.'), 'error');
    }
  }
  if (count($results['good'])) {
    drupal_set_message(t('Successfully imported: !good-list', array('!good-list' => theme('item_list', $results['good']))));
  }
  watchdog('image_import', 'Completed image import.');
}

/**
 * Creates subgalleries.
 *
 * @param $origname
 *   The original name (with its path) of the image which subcategory has to be created.
 * @param $image_galleries_vid
 *   The vocabulary id of corresponding to image galleries.
 * @param $subgalleries_parent_tid
 *   The term id of the parent gallery, which will contain the subgallery.
 * @param $subgalleries
 *   An array of the tids of each subgallery already created, indexed by subgallery path.
 * @param $context
 *   The execution log.
 *
 * @return
 *   The tid of the subgallery in which the image goes to.
 */
function _image_import_create_subgalleries($origname, $image_galleries_vid, $subgalleries_parent_tid, &$subgalleries, &$context) {
  // Get a term id from its parent (if any), its vocabulary, and its name.
  $tid_query = 'SELECT td.tid FROM {term_data} td, {term_hierarchy} th WHERE td.tid = th.tid AND vid = %d AND name = \'%s\' AND parent = %d';
  // Explode path to image.
  $requested_subgalleries = explode('/', $origname);
  // Remove the filename at the end.
  array_pop($requested_subgalleries);
  // Parent gallery.
  $parent_tid = (int) $subgalleries_parent_tid;
  // Current image's gallery.
  $subgallery_tid = (int) $subgalleries_parent_tid;
  // Create each gallery in the path, if necessary.
  foreach ($requested_subgalleries as $key => $subgallery_name) {
    // The current subgallery's relative path.
    $subgallery_path = implode('/', array_slice($requested_subgalleries, 0, $key + 1));
    // Was this subgallery already created by the current batch?
    $subgallery_tid = $subgalleries[$subgallery_path];
    if (!$subgallery_tid) {
      // First, look in DB.
      $subgallery_tid = db_result(db_query($tid_query, $image_galleries_vid, $subgallery_name, $parent_tid));
      // Didn't find? Create it.
      if (!$subgallery_tid) {
        $subgallery_term = array(
          'name' => $subgallery_name,
          'parent' => $parent_tid,
          'vid' => $image_galleries_vid,
        );
        $status = taxonomy_save_term($subgallery_term);
        if ($status) {
          // Get its tid back.
          $subgallery_tid = db_result(db_query($tid_query, $image_galleries_vid, $subgallery_name, $parent_tid));
          $term = taxonomy_get_term($subgallery_tid);
          $context['results']['good'][] = t('Created gallery !link [!edit-link].', array(
            '!link' => l($subgallery_name, taxonomy_term_path($term)),
            '!edit-link' => l(t('edit'), 'admin/content/taxonomy/edit/term/' . $subgallery_tid),
          ));
        }
        else {
          watchdog('image_import', 'There was an error that prevented gallery %gallery_path from being created.', array('%gallery_path' => $subgallery_path), WATCHDOG_ERROR);
          $context['results']['bad'][] = t('Error creating gallery %gallery_path.', array('%gallery_path' => $subgallery_path));
          // Return an error.
          return 0;
        }
      }
      $subgalleries[$subgallery_path] = $subgallery_tid;
    }
    // For the next subgallery to be created, last created will be its parent.
    $parent_tid = (int)$subgallery_tid;
  }
  return $subgallery_tid;
}

/**
 * Delete recursively all directories inside $basepath.
 *
 * @param $basepath
 *   The base directory where to find empty directories.
 * @param $dir
 *   A directory inside $basepath, used for recursive purpose.
 *   Should not be used when calling this method.
 *
 * @return
 *   An array of the deleted directories paths, relative to $basepath.
 */
function _image_import_recursive_delete_empty_directories($basepath, $dir = '') {
  $deleted = array();
  // Append $basepath to $dir only if $dir is not empty (to avoid trailing
  // slash).
  if ($dir != '') {
    $crnt_dir = $basepath . '/' . $dir;
  }
  else {
    $crnt_dir = $basepath;
  }
  // Get each file in $crnt_dir.
  $ls = file_scan_directory($crnt_dir, '.*', array('.', '..', 'CVS'), 0, FALSE);
  // For each directory inside, recursively delete empty directories inside.
  foreach ($ls as $file) {
    if (is_dir($file->filename)) {
      $next_dir = $dir ? $dir . '/' . $file->basename : $file->basename;
      $deleted = array_merge($deleted, _image_import_recursive_delete_empty_directories($basepath, $next_dir));
      if (sizeof(file_scan_directory($file->filename, '.*', array('.', '..'), 0, FALSE)) == 0) {
        rmdir($file->filename);
        $deleted[] = '<em>' . $next_dir . '</em>';
      }
    }
  }
  return $deleted;
}

/**
 * Batch operation callback for deletion of empty directories after the import.
 */
function _image_import_recursive_delete_empty_directories_batch_op($args, &$context) {
  $deleted = _image_import_recursive_delete_empty_directories($args['basepath']);
  if (!empty($deleted)) {
    $context['results']['good'][] = t('Deleted directories: !dir-list.', array(
      '!dir-list' => theme('item_list', $deleted),
    ));
  }
  else {
    $context['results']['good'][] = t('No directories were deleted.');
  }
  $context['finished'] = 1;
}

