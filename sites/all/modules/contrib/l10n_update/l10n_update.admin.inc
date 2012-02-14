<?php

/**
 * @file
 *   Admin settings and update page.
 */

/**
 * Project has a new release available.
 */
define('L10N_UPDATE_NOT_CURRENT', 4);

/**
 * Project is up to date.
 */
define('L10N_UPDATE_CURRENT', 5);

/**
 * Project's status cannot be checked.
 */
define('L10N_UPDATE_NOT_CHECKED', -1);

/**
 * No available update data was found for project.
 */
define('L10N_UPDATE_UNKNOWN', -2);

/**
 * There was a failure fetching available update data for this project.
 */
define('L10N_UPDATE_NOT_FETCHED', -3);

// Include l10n_update API
include_once 'l10n_update.check.inc';
// And project api
include_once 'l10n_update.project.inc';

/**
 * Page callback: Admin overview page.
 */
function l10n_update_admin_overview() {
  // For now we get package information provided by modules.
  $projects = l10n_update_get_projects();
  $languages = l10n_update_language_list('name');
  if ($projects && $languages) {
    $history = l10n_update_get_history();
    $available = l10n_update_available_releases();
    $updates = l10n_update_build_updates($history, $available);
    $output = theme('l10n_update_project_status', $projects, $languages, $history, $available, $updates);
    $output .= drupal_get_form('l10n_update_admin_import_form', $projects, $updates);
  }
  else {
    $output = t('No projects or languages to update.');
  }
  return $output;
}

/**
 * Translation update form.
 *
 * @todo selectable packages
 * @todo check language support in server
 * @todo check file update dates
 *
 * @param $form_state
 *   Form states array.
 * @param $projects
 *   @todo $projects are not used in the form.
 * @param $updates
 *   Updates to be displayed in the form.
 */
function l10n_update_admin_import_form($form_state, $projects, $updates) {
  //module_load_include('inc', 'l10n_update');
  // For now we get package information provided by modules
  $projects = l10n_update_get_projects();
  $languages = l10n_update_language_list('name');

  if ($projects && $languages) {
    $form['updates'] = array('#type' => 'value', '#value' => $updates);
    $form['lang'] = array(
      '#type' => 'fieldset',
      '#title' => t('Languages'),
      '#collapsible' => TRUE, '#collapsed' => TRUE,
      '#description' => t('Select one or more languages to download and update. If you select none, all of them will be updated.'),
    );
    $form['lang']['languages'] = array(
      '#type' => 'checkboxes',
      '#options' => $languages,
      '#default_value' => array(language_default('language')),

    );
    $form['mode'] = array('#type' => 'radios',
      '#title' => t('Update mode'),
      '#default_value' => variable_get('l10n_update_import_mode', LOCALE_IMPORT_KEEP),
      '#options' => _l10n_update_admin_import_options(),
    );
    $form['buttons']['download'] = array('#type' => 'submit', '#value' => t('Update translations'));
    $form['buttons']['refresh'] = array('#type' => 'submit', '#value' => t('Refresh information'));
  }
  else {
    $form['warning'] = array('#value' => t('No projects or languages to update.'));
  }
  return $form;
}

/**
 * Submit handler for Update form.
 *
 * Handles both submit buttons to update translations and to update the
 * form information.
 */
function l10n_update_admin_import_form_submit($form, $form_state) {
  $op = isset($form_state['values']['op']) ? $form_state['values']['op'] : '';
  $projects = l10n_update_get_projects();

  if ($op == t('Update translations')) {
    $languages = array_filter($form_state['values']['languages']);
    $updates = $form_state['values']['updates'];
    $mode = $form_state['values']['mode'];
    if ($projects && $updates) {
      module_load_include('batch.inc', 'l10n_update');
      // Filter out updates in other languages. If no languages, all of them will be updated
      $updates = _l10n_update_prepare_updates($updates, NULL, $languages);
      $batch = l10n_update_batch_multiple($updates, $mode);
      batch_set($batch);
    }
    else {
      drupal_set_message(t('Cannot find any translation updates.'), 'error');
    }
  }
  elseif ($op == t('Refresh information')) {
    l10n_update_build_projects();
    if ($available = l10n_update_available_releases(TRUE)) {
      drupal_set_message(t('Fetched information about available updates from the server'));
    }
    else {
      drupal_set_message(t('Failed to fetch information about available updates from the server'), 'error');
    }
  }
}

/**
 * Page callback: Settings form.
 */
function l10n_update_admin_settings_form() {
  $form['l10n_update_check_mode'] = array('#type' => 'radios',
    '#title' => t('Update source'),
    '#default_value' => variable_get('l10n_update_check_mode', L10N_UPDATE_CHECK_ALL),
    '#options' => _l10n_update_admin_check_options(),
  );
  $form['l10n_update_import_mode'] = array('#type' => 'radios',
    '#title' => t('Update mode'),
    '#default_value' => variable_get('l10n_update_import_mode', LOCALE_IMPORT_KEEP),
    '#options' => _l10n_update_admin_import_options(),
  );
  $form['l10n_update_check_frequency'] = array(
    '#type' => 'radios',
    '#title' => t('Check for updates'),
    '#default_value' => variable_get('l10n_update_check_frequency', 0),
    '#options' => array(
      0 => t('Never (manually)'),
      1 => t('Daily'),
      7 => t('Weekly'),
    ),
    '#description' => t('Select how frequently you want to automatically check for updated translations for installed modules and themes.'),
  );
  $form['l10n_update_check_disabled'] = array(
    '#type' => 'radios',
    '#title' => t('Update disabled modules'),
    '#options' => array(t('Disabled'), t('Enabled')),
    '#default_value' => variable_get('l10n_update_check_disabled', 0),
    '#description' => t('Enable if you want translations for disabled modules to be updated too. This comes with an important performance penalty so it is not recommended.'),
  );
  $form['l10n_update_download_store'] = array(
    '#title' => t('Store downloaded files'),
    '#type' => 'textfield',
    '#default_value' => variable_get('l10n_update_download_store', ''),
    '#description' => t('A path relative to the Drupal installation directory where translation files will be stored, e.g. sites/all/translations. Saved translation files can be reused by other installations. If left empty the downloaded translation will not be saved.'),
  );
  return system_settings_form($form);
}

/**
 * Additional validation handler for update settings.
 *
 * Check for existing files directory and creates one when required.
 */
function l10n_update_admin_settings_form_validate($form, &$form_state) {
  $form_values = $form_state['values'];
  if ( ! empty($form_values['l10n_update_download_store'])) {
    file_check_directory($form_values['l10n_update_download_store'], FILE_CREATE_DIRECTORY, 'l10n_update_download_store');
  }
}

/**
 * Get array of import options.
 *
 * The import options of the Locale module are used but the UI text is altered
 * to suit the Localization update cases.
 *
 * @return
 *   Keyed array of import options.
 */
function _l10n_update_admin_import_options() {
  return array(
    LOCALE_IMPORT_OVERWRITE => t('Translation updates replace existing ones, new ones are added'),
    LOCALE_IMPORT_KEEP => t('Edited translations are kept, only default ones (previously imported) are overwritten and new translations are added')
  );
}

/**
 * Get array of check options.
 *
 * @return
 *   Keyed array of source download options.
 */
function _l10n_update_admin_check_options() {
  return array(
    L10N_UPDATE_CHECK_ALL => t('Local files and remote server.'),
    L10N_UPDATE_CHECK_LOCAL => t('Local files only.'),
    L10N_UPDATE_CHECK_REMOTE => t('Remote server only.'),
  );
}

/**
 * Format project update status.
 *
 * @params array $projects
 *   Projects listed.
 * @params array $languages
 *   Languages listed.
 * @params array $history
 *   Project translation history.
 * @params array $available
 *   Available translation releases.
 * @params array $updates
 *   Applicable translation updates.
 *
 * @return string
 *   HTML output.
 */
function theme_l10n_update_project_status($projects, $languages, $history, $available, $updates) {
  // We use the core update module CSS
  drupal_add_css(drupal_get_path('module', 'update') .'/update.css');
  $output = '';
  //$header = array(t('Project'), t('Current version'), t('Available update'), '');
  $header = $rows = array();

  foreach ($projects as $name => $project) {
    $row = '<div class="version-status">';
    if (empty($available[$name])) {
      // Remote information not checked
      $class = 'unknown';
      $status = 'unknown';
    }
    elseif (empty($updates[$name])) {
      // No updates available
      $class = 'ok';
      $status = 'ok';
    }
    else {
      // Update available
      $class = 'warning';
      $status = 'update';
    }

    $row = theme('l10n_update_version_status', $status);

    $row .= "<div class=\"project $class\">";
    $title = isset($project->title) ? $project->title : $project->name;
    $row .= check_plain($title);
    $row .= ' ' . check_plain($project->version);
    if ($server = l10n_update_server($project->l10n_server)) {
      $row .= ' ' . l($server['name'], $server['link']);
    }
    $row .= "</div>\n";

    $row .= "<div class=\"versions\">\n";
    $versions = array();
    foreach ($languages as $lang => $language) {
      $current = isset($history[$name][$lang]) ? $history[$name][$lang] : NULL;
      $update = isset($updates[$name][$lang]) ? $updates[$name][$lang] : NULL;
      if ($update) {
        $status = 'update';
        $class = 'warning';
      }
      elseif ($current) {
        $status = $class = 'ok';
      }
      else {
        $status = $class ='unknown';
      }
      $version = array(
        array('data' => $language, 'class' => 'version-title'),
        $current ? theme('l10n_update_release', $current) : '',
        $update ? theme('l10n_update_release', $update) : '',
        theme('l10n_update_version_status', $status, $update ? $update->type : NULL),
      );
      $versions[] = array('data' => $version, 'class' => $class);
    }
    $row .= theme('table', array(), $versions);
    $row .= "</div>\n";
    $rows[] = array($row);
  }
  $output .= theme('table', $header, $rows, array('class' => 'update'));
  return $output;
}

/**
 * Format tag and release date.
 *
 * @param object $release
 *   Source file data.
 * @return string
 *   HTML output.
 */
function theme_l10n_update_release($release) {
  $name = $release->filename;
  $date = $release->timestamp;
  if (!empty($release->fileurl)) {
    // Remote file, straight link
    $link = l($name, $release->fileurl);
  }
  elseif (!empty($release->filepath)) {
    // Local file, try something
    $link = l($name, $release->filepath, array('absolute' => TRUE));
  }
  else {
    // No link
    $link = "<strong>$name</strong>";
  }
  return $link . '<br /><small><span class="version-date">' . format_date($date, 'short') . '</span></small>';
}

/**
 * Format version status with icon.
 *
 * @param string $status
 *   Version status: 'ok', 'update', 'unknown'.
 * @param string $type
 *   Update type: 'download', 'localfile'.
 *
 * @return sting
 *   HTML output.
 */
function theme_l10n_update_version_status($status, $type = NULL) {
  $output = '<div class="version-status">';
  switch ($status) {
    case 'ok':
      $icon = theme('image', 'misc/watchdog-ok.png', t('ok'), t('ok'));
      $msg = '<span class="current">'. t('Up to date') .'</span>';
      break;
    case 'update':
      $icon = theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning'));
      $txt = ($type == 'download') ? t('Remote update available') : t('Local update available');
      $msg = '<span class="not-current">'. $txt .'</span>';
      break;
    case 'unknown':
      $icon = theme('image', 'misc/watchdog-warning.png', t('warning'), t('warning'));
      $msg = '<span class="not-supported">'. t('No information') .'</span>';
      break;
  }
  $output .= $msg;
  $output .= '<span class="icon">'. $icon .'</span>';
  $output .= "</div>";
  return $output;
}
