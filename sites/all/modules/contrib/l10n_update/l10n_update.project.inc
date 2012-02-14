<?php

/**
 * @file
 * Library for querying Drupal projects
 *
 * Most code is taken from update module. We don't want to depend on it though as it may not be enabled.
 *
 * For each project, the information about where to fetch translations may be specified in the info files
 * as follows:
 *
 * - Localization server to be used for this project. Defaults to http://localize.drupal.org
 *   l10n server = localize.drupal.org
 *   (This should be enough if the server url, the one below, is defined somewhere else)
 *
 * - Metadata information for the localization server
 *   l10n url = http://ftp.drupal.org/files/translations/l10n_server.xml
 *   (We can fetch *all* the information we need from this single url)
 *
 * - Translation file URL template, will be used to build the file url to download
 *   l10n path = http://ftp.drupal.org/files/translations/%core/%project/%project-%release.%language.po
 *   (Alternatively you can use the %filename variable that will default to '%project-%release.%language.po')
 */

/**
 * Rebuild project list
 *
 * @return array
 */
function l10n_update_build_projects() {
  module_load_include('inc', 'l10n_update');
  // Get all stored projects, including disabled ones
  $current = l10n_update_get_projects(NULL, TRUE);
  // Now get the new project list, just enabled ones
  $projects = l10n_update_project_list();
  // Mark all previous projects as disabled and store new project data
  db_query("UPDATE {l10n_update_project} SET status = 0");
  $default_server = l10n_update_default_server();
  foreach ($projects as $name => $data) {
    $data += array(
      'version' => isset($data['info']['version']) ? $data['info']['version'] : '',
      'core' => isset($data['info']['core']) ? $data['info']['core'] : DRUPAL_CORE_COMPATIBILITY,
      // The project can have its own l10n server, we use default if not
      'l10n_server' => isset($data['info']['l10n server']) ? $data['info']['l10n server'] : NULL,
      // A project can provide the server url to fetch metadata, or the update url (path)
      'l10n_url' => isset($data['info']['l10n url']) ? $data['info']['l10n url'] : NULL,
      'l10n_path' => isset($data['info']['l10n path']) ? $data['info']['l10n path'] : NULL,
      'status' => 1,
    );
    $project = (object)$data;
    // Unless the project provides a full l10n path (update url), we try to build one
    if (!isset($project->l10n_path)) {
      $server = NULL;
      if ($project->l10n_server || $project->l10n_url) {
        $server = l10n_update_server($project->l10n_server, $project->l10n_url);
      }
      else {
        // Use the default server
        $server = l10n_update_server($default_server['name'], $default_server['server_url']);
      }
      if ($server) {
        // Build the update path for this project, with project name and release replaced
        $project->l10n_path = l10n_update_build_string($project, $server['update_url']);
      }
    }
    // Create / update project record
    $update = empty($current[$name]) ? array() : array('name');
    drupal_write_record('l10n_update_project', $project, $update);
    $projects[$name] = $project;
  }
  return $projects;
}

/**
 * Get update module's project list
 *
 * @return array
 */
function l10n_update_project_list() {
  $projects = array();
  $disabled = variable_get('l10n_update_check_disabled', 0);
  // Unlike update module, this one has no cache
  _l10n_update_project_info_list($projects, module_rebuild_cache(), 'module', $disabled);
  _l10n_update_project_info_list($projects, system_theme_data(), 'theme', $disabled);
  // Allow other modules to alter projects before fetching and comparing.
  drupal_alter('update_projects', $projects);
  return $projects;
}

/**
 * Refresh projects after enabling/disabling modules
 *
 * When new projects are installed, set a batch for locale import / update
 */
function l10n_update_project_refresh() {
  // Get previous enabled/disabled projects
  $prev_enabled = l10n_update_get_projects();
  $prev_all = l10n_update_get_projects(FALSE, TRUE);
  // Get all current projects, also rebuild table data
  $current_projects = l10n_update_build_projects();
  $enabled = array_diff(array_keys($current_projects), array_keys($prev_enabled));
  $installed = array_diff(array_keys($current_projects), array_keys($prev_all));
  // If there are newly installed projects, create batch for projects x languages
  if ($installed) {
    module_load_include('check.inc', 'l10n_update');
    foreach ($installed as $name) {
      $projects[$name] = $current_projects[$name];
    }
    if ($available = l10n_update_check_projects($projects)) {
      $history = l10n_update_get_history();
      if ($updates = l10n_update_build_updates($history, $available)) {
        module_load_include('batch.inc', 'l10n_update');
        // Filter out updates in other languages. If no languages, all of them will be updated
        $updates = _l10n_update_prepare_updates($updates);
        $batch = l10n_update_batch_multiple($updates, variable_get('l10n_update_import_mode', LOCALE_IMPORT_KEEP));
        batch_set($batch);
      }
    }
  }
}

/**
 * Populate an array of project data.
 *
 * Based on _update_process_info_list()
 *
 * @param $projects
 * @param $list
 * @param $project_type
 * @param $disabled
 *   TRUE to include disabled projects too
 */
function _l10n_update_project_info_list(&$projects, $list, $project_type, $disabled = FALSE) {
  foreach ($list as $file) {
    if (!$disabled && empty($file->status)) {
      // Skip disabled modules or themes.
      continue;
    }

    // Skip if the .info file is broken.
    if (empty($file->info)) {
      continue;
    }

    // If the .info doesn't define the 'project', try to figure it out.
    if (!isset($file->info['project'])) {
      $file->info['project'] = l10n_update_get_project_name($file);
    }

    // If we still don't know the 'project', give up.
    if (empty($file->info['project'])) {
      continue;
    }

    // If we don't already know it, grab the change time on the .info file
    // itself. Note: we need to use the ctime, not the mtime (modification
    // time) since many (all?) tar implementations will go out of their way to
    // set the mtime on the files it creates to the timestamps recorded in the
    // tarball. We want to see the last time the file was changed on disk,
    // which is left alone by tar and correctly set to the time the .info file
    // was unpacked.
    if (!isset($file->info['_info_file_ctime'])) {
      $info_filename = dirname($file->filename) .'/'. $file->name .'.info';
      $file->info['_info_file_ctime'] = filectime($info_filename);
    }

    $project_name = $file->info['project'];
    if (!isset($projects[$project_name])) {
      // Only process this if we haven't done this project, since a single
      // project can have multiple modules or themes.
      $projects[$project_name] = array(
        'name' => $project_name,
        'info' => $file->info,
        'datestamp' => isset($file->info['datestamp']) ? $file->info['datestamp'] : 0,
        'includes' => array($file->name => $file->info['name']),
        'project_type' => $project_name == 'drupal' ? 'core' : $project_type,
      );
    }
    else {
      $projects[$project_name]['includes'][$file->name] = $file->info['name'];
      $projects[$project_name]['info']['_info_file_ctime'] = max($projects[$project_name]['info']['_info_file_ctime'], $file->info['_info_file_ctime']);
    }
  }
}

/**
 * Given a $file object (as returned by system_get_files_database()), figure
 * out what project it belongs to.
 *
 * @param $file
 * @return string
 * @see system_get_files_database()
 */
function l10n_update_get_project_name($file) {
  $project_name = '';
  if (isset($file->info['project'])) {
    $project_name = $file->info['project'];
  }
  elseif (isset($file->info['package']) && (strpos($file->info['package'], 'Core -') !== FALSE)) {
    $project_name = 'drupal';
  }
  elseif (in_array($file->name, array('bluemarine', 'chameleon', 'garland', 'marvin', 'minnelli', 'pushbutton'))) {
    // Unfortunately, there's no way to tell if a theme is part of core,
    // so we must hard-code a list here.
    $project_name = 'drupal';
  }
  return $project_name;
}
