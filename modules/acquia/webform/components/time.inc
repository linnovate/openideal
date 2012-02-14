<?php
// $Id: time.inc,v 1.16.2.12 2010/07/27 06:16:50 quicksketch Exp $

/**
 * @file
 *   Webform module time component.
 */

/**
 * Create a default time component.
 */
function _webform_defaults_time() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'email' => 1,
    'extra' => array(
      'timezone' => 'site',
      'check_daylight_savings' => 0,
      'hourformat' => '12-hour',
      'description' => '',
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form for editing this component.
 * Use care naming the form items, as this correlates directly to the database schema.
 * The component "Name" and "Description" fields are added to every component type and
 * are not necessary to specify here (although they may be overridden if desired).
 * @return An array of form items to be displayed on the edit component page
 */
function _webform_edit_time($currfield) {
  $edit_fields = array();
  $edit_fields['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $currfield['value'],
    '#description' => t('The default value of the field.') .'<br />'. t('Accepts a time in any <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>. Strings such as now, +2 hours, and 10:30pm are all valid.'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
    '#element_validate' => array('webform_validate_time_string'),
  );
  $edit_fields['extra']['timezone'] = array(
    '#type' => 'radios',
    '#title' => t('Timezone'),
    '#default_value' => empty($currfield['extra']['timezone']) ? 'site' : $currfield['extra']['timezone'],
    '#description' => t('Adjust the time according to a specific timezone. Website timezone is defined in the <a href="!settings">Site Settings</a> and is the default.', array('!settings' => url('admin/settings/date-time'))),
    '#options' => array('site' => 'Website Timezone', 'user' => 'User Timezone', 'gmt' => 'GMT'),
    '#weight' => 0,
  );
$edit_fields['extra']['check_daylight_savings'] = array(
  '#type' => 'checkbox',
  '#title' => t('Observe Daylight Savings'),
  '#default_value' => $currfield['extra']['check_daylight_savings'],
  '#checked_value' => 1,
  '#description' => t('Automatically adjust the time during daylight savings.'),
  '#weight' => 1,
);
  $edit_fields['extra']['hourformat'] = array(
    '#type' => 'radios',
    '#title' => t('Time Format'),
    '#default_value' => isset($currfield['extra']['hourformat']) ? $currfield['extra']['hourformat'] : '12-hour',
    '#description' => t('Format the display of the time in 12 or 24 hours.'),
    '#options' => array('12-hour' => '12-hour (am/pm)', '24-hour' => '24-hour'),
    '#weight' => 2,
  );
  return $edit_fields;
}

/**
 * Build a form item array containing all the properties of this component
 * @param $component An array of information describing the component, directly correlating to the webform_component database schema
 * @return An array of a form item to be displayed on the client-side webform
 */
function _webform_render_time($component) {
  if (drupal_strlen($component['value']) > 0) {
    // Calculate the timestamp in GMT.
    $timestamp = strtotime($component['value']);
    if ($component['extra']['timezone'] == 'user') {
      // Use the users timezone.
      global $user;
      $timestamp += (int)$user->timezone;
    }
    elseif ($component['extra']['timezone'] == 'gmt') {
      // Use GMT.
      $timestamp += 0;
    }
    else {
      // Use the Drupal site time.
      $timestamp += (int)variable_get('date_default_timezone', 0);
    }

    // Check for daylight savings time.
    if ($component['extra']['check_daylight_savings'] && date('I')) {
      $timestamp += 3600;
    }
  }

  if ($component['extra']['hourformat'] == '12-hour') {
    $first_hour = 1;
    $last_hour = 12;
    $hour_format = 'g';
  }
  else {
    $first_hour = 0;
    $last_hour = 23;
    $hour_format = 'G';
  }

  if (drupal_strlen($component['value']) > 0) {
    $hour = gmdate($hour_format, $timestamp);
    $minute = gmdate('i', $timestamp);
    $am_pm = gmdate('a', $timestamp);
  }

  // Generate the choices for drop-down selects.
  $hours[''] = t('hour');
  $minutes[''] = t('minute');
  for ($i = $first_hour; $i <= $last_hour; $i++) $hours[$i] = $i;
  for ($i = 0; $i <= 59; $i++) $minutes[$i < 10 ? "0$i" : $i] = $i < 10 ? "0$i" : $i;
  $am_pms = array('am' => t('am'), 'pm' => t('pm'));

  $form_item = array(
    '#title' => $component['name'],
    '#required' => $component['mandatory'],
    '#weight' => $component['weight'],
    '#description' => _webform_filter_descriptions($component['extra']['description']),
    '#prefix' => '<div class="webform-component-'. $component['type'] .'" id="webform-component-'. $component['form_key'] .'">',
    '#suffix' => '</div>',
    '#theme' => 'webform_time',
    '#element_validate' => array('webform_validate_time'),
    '#webform_component' => $component,
  );

  $form_item['hour'] = array(
    '#prefix' => '',
    '#type' => 'select',
    '#default_value' => $hour,
    '#options' => $hours,
  );
  $form_item['minute'] = array(
    '#prefix' => ':',
    '#type' => 'select',
    '#default_value' => $minute,
    '#options' => $minutes,
  );
  if ($component['extra']['hourformat'] == '12-hour') {
    $form_item['ampm'] = array(
      '#type' => 'radios',
      '#default_value' => $am_pm,
      '#options' => $am_pms,
    );
  }

  return $form_item;
}

function webform_validate_time($form_item, $form_state) {
  $form_key = $form_item['#webform_component']['form_key'];
  $name = $form_item['#webform_component']['name'];

  // Check if the user filled the required fields.
  foreach ($form_item['#webform_component'] == '12-hour' ? array('hour', 'minute', 'ampm') : array('hour', 'minute') as $field_type) {
    if (!is_numeric($form_item[$field_type]['#value']) && $form_item['#required']) {
      form_set_error($form_key, t('%field field is required.', array('%field' => $name)));
      return;
    }
  }

  // Check for a valid time.
  if ($form_item['hour']['#value'] !== '' || $form_item['minute']['#value'] !== '' || (isset($form_item['ampm']) && $form_item['ampm']['#value'] !== '')) {
    if (!is_numeric($form_item['hour']['#value']) || !is_numeric($form_item['minute']['#value']) || (isset($form_item['ampm']) && $form_item['ampm']['#value'] === '')) {
      form_set_error($form_key, t('Entered %name is not a valid time.', array('%name' => $name)));
      return;
    }
  }
}

/**
 * Display the result of a textfield submission. The output of this function
 * will be displayed under the "results" tab then "submissions".
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
function _webform_submission_display_time($data, $component, $enabled = FALSE) {
  $form_item = _webform_render_time($component);
  $form_item['minute']['#default_value'] = $data['value']['1'];
  $form_item['minute']['#disabled'] = !$enabled;
  $form_item['hour']['#disabled'] = !$enabled;
  $form_item['ampm']['#disabled'] = !$enabled;

  // Match the hourly data to the hour format.
  if ($data['value']['1']) {
    $timestamp = strtotime($data['value']['0'] .':'. $data['value']['1'] . (isset($data['value']['2']) ? $data['value']['2'] : ''));
    if ($component['extra']['hourformat'] == '24-hour') {
      $form_item['hour']['#default_value'] = date('H', $timestamp);
    }
    else {
      $form_item['hour']['#default_value'] = date('g', $timestamp);
      $form_item['ampm']['#default_value'] = $data['value']['2'];
    }
  }
  return $form_item;
}

/**
 * Format the output of emailed data for this component
 *
 * @param $data
 *   A string or array of the submitted data
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output to be included in the email.
 */
function theme_webform_mail_time($data, $component) {
  $output = $component['name'] .':';
  if ($data['hour'] && $data['minute']) {
    if ($component['extra']['hourformat'] == '24-hour') {
      $output .= ' '. $data['hour'] .':'. $data['minute'];
    }
    else {
      $output .= ' '. $data['hour'] .':'. $data['minute'] .' '. $data['ampm'];
    }
  }
  return $output;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_time($section) {
  switch ($section) {
    case 'admin/settings/webform#time_description':
      return t('Presents the user with hour and minute fields. Optional am/pm fields.');
  }
}

/**
 * Module specific instance of hook_theme().
 */
function _webform_theme_time() {
  return array(
    'webform_time' => array(
      'arguments' => array('element' => NULL),
    ),
    'webform_mail_time' => array(
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
 *   the webform_component database schema.
 * @param $sids
 *   An optional array of submission IDs (sid). If supplied, the analysis will be limited
 *   to these sids.
 * @return
 *   An array of data rows, each containing a statistic for this component's
 *   submissions.
 */
function _webform_analysis_rows_time($component, $sids = array()) {
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (".implode(",", $placeholders).")" : "";
  $query = 'SELECT no,data '.
    ' FROM {webform_submitted_data} '.
    ' WHERE nid = %d '.
    ' AND  cid = %d '. $sidfilter.
    ' ORDER BY sid,no ASC ';

  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  // build an array of timestamps from entered values.
  $timestamps = array();
  $submissions = 0;
  while ($row = db_fetch_array($result)) {
    if ($row['no'] == '0') {
      $submissions++;
      $hour = $row['data'];
      if ($row = db_fetch_array($result)) {
        if ($row['no'] == '1') {
          $minute = $row['data'];
          if ($row = db_fetch_array($result)) {
            if ($row['no'] == '2') {
              $ampm = $row['data'];
              // Build the full timestamp.
              if (drupal_strlen($hour) > 0 && drupal_strlen($minute) > 0 ) {
                $timestamps[] = strtotime($hour .':'. $minute .' '. $ampm);
              }
            }
            else {
              // Build military time.
              $timestamps[] = strtotime($hour .':'. $minute);
            }
          }
        }
      }
    }
  }

  // Display stats.
  // TODO: display date statistics in javascript tabs.
  $nonblanks = count($timestamps);
  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);
  return $rows;
}

/**
 * Return the result of this component's submission for display in a table. The
 * output of this function will be displayed under the "results" tab then "table".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_table_data_time($data, $component) {
  if (drupal_strlen($data['value']['0']) > 0 && drupal_strlen($data['value']['1']) > 0) {
    $timestamp = strtotime($data['value']['0'] .':'. $data['value']['1'] . $data['value']['2']);
    if ($component['extra']['hourformat'] == '24-hour') {
      return check_plain(date('H:i', $timestamp));
    }
    else {
      return check_plain(date('g:i a', $timestamp));
    }
  }
  else {
    return '';
  }
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
function _webform_csv_headers_time($component) {
  $header = array();
  $header[0] = '';
  $header[1] = '';
  $header[2] = $component['name'];
  return $header;
}

/**
 * Return the result of a textfield submission. The output of this function will
 * be displayed under the "results" tab then "submissions".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @return
 *   Textual output formatted for CSV, not including either prefixed or trailing
 *   commas.
 */
function _webform_csv_data_time($data, $component) {
  if (drupal_strlen($data['value']['0']) > 0 && drupal_strlen($data['value']['1']) > 0) {
    $timestamp = strtotime($data['value']['0'] .':'. $data['value']['1'] . $data['value']['2']);
    if ($component['extra']['hourformat'] == '24-hour') {
      return date('H:i', $timestamp);
    }
    else {
      return date('g:i a', $timestamp);
    }
  }
  else {
    return '';
  }
}

/**
 * Theme a webform time element.
 */
function theme_webform_time($element) {
  $element['#type'] = 'element';
  $element['#children'] = '<div class="container-inline">'. drupal_render($element['hour']) . drupal_render($element['minute']) . drupal_render($element['ampm']) .'</div>';

  return theme('form_element', $element, $element['#children']);
}
