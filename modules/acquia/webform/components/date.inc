<?php
// $Id: date.inc,v 1.15.2.24 2010/07/27 06:16:50 quicksketch Exp $

/**
 * @file
 *   Webform module date component.
 */

/**
 * Create a default date component.
 */
function _webform_defaults_date() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'email' => 1,
    'extra' => array(
      'check_daylight_savings' => 0,
      'year_start' => '1900',
      'year_end' => '2050',
      'year_textfield' => 0,
      'description' => '',
    ),
  );
}

/**
 * Create a set of form items to be displayed on the form for editing this
 * component. Use care naming the form items, as this correlates directly to the
 * database schema. The component "Name" and "Description" fields are added to
 * every component type and are not necessary to specify here (although they may
 * be overridden if desired).
 * @return
 *   An array of form items to be displayed on the edit component page.
 */
function _webform_edit_date($currfield) {
  $edit_fields = array();
  $edit_fields['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $currfield['value'],
    '#description' => t('The default value of the field.') .'<br />'. t('Accepts any date in any <a href="http://www.gnu.org/software/tar/manual/html_chapter/Date-input-formats.html">GNU Date Input Format</a>. Strings such as today, +2 months, and Dec 9 2004 are all valid.'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
  );
  $edit_fields['extra']['timezone'] = array(
    '#type' => 'radios',
    '#title' => t('Timezone'),
    '#default_value' => empty($currfield['extra']['timezone']) ? 'site' : $currfield['extra']['timezone'],
    '#description' => t('Adjust the date according to a specific timezone. Website timezone is defined in the <a href="!settings">Site Settings</a> and is the default.', array('!settings' => url('admin/settings/date-time'))),
    '#options' => array('site' => t('Website timezone'), 'user' => t('User timezone'), 'gmt' => t('GMT')),
    '#weight' => 1,
  );
  $edit_fields['extra']['check_daylight_savings'] = array(
    '#type' => 'checkbox',
    '#title' => t('Observe Daylight Savings'),
    '#default_value' => $currfield['extra']['check_daylight_savings'],
    '#checked_value' => 1,
    '#description' => t('Automatically adjust the time during daylight savings.'),
    '#weight' => 2,
  );

  $edit_fields['extra']['year_start'] = array(
    '#type' => 'textfield',
    '#title' => t('Start year'),
    '#default_value' => empty($currfield['extra']['year_start']) ? '1900' : $currfield['extra']['year_start'],
    '#description' => t('The first year that is allowed to be entered.'),
    '#size' => 10,
    '#maxlength' => 4,
    '#weight' => 3,
  );
  $edit_fields['extra']['year_end'] = array(
    '#type' => 'textfield',
    '#title' => t('End year'),
    '#default_value' => empty($currfield['extra']['year_end']) ? '2050' : $currfield['extra']['year_end'],
    '#description' => t('The last year that is allowed to be entered.'),
    '#size' => 10,
    '#maxlength' => 4,
    '#weight' => 4,
  );
  $edit_fields['extra']['year_textfield'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use a textfield for year'),
    '#default_value' => $currfield['extra']['year_textfield'],
    '#description' => t('If checked, the generated date field will use a textfield for the year. Otherwise it will use a select list.'),
    '#weight' => 5,
  );

  return $edit_fields;
}

/**
 * Build a form item array containing all the properties of this component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   An array of a form item to be displayed on the client-side webform.
 */
function _webform_render_date($component) {
  $form_item = array(
    '#title' => $component['name'],
    '#weight' => $component['weight'],
    '#type' => 'date',
    '#description' => _webform_filter_descriptions($component['extra']['description']),
    '#prefix' => '<div class="webform-component-'. $component['type'] .'" id="webform-component-'. $component['form_key'] .'">',
    '#suffix' => '</div>',
    '#required' => $component['mandatory'],
    '#webform_component' => $component,
    '#process' => array('webform_expand_date'),
    '#element_validate' => array('webform_validate_date'),
  );
  return $form_item;
}

function webform_expand_date($element) {
  $component = $element['#webform_component'];

  // Set defaults according to existing #default_value (set by Form API)
  if (isset($element['month']['#default_value']) || isset($element['day']['#default_value']) || isset($element['year']['#default_value'])) {
    $default_values = array(
      'month' => $element['month']['#default_value'],
      'day' => $element['day']['#default_value'],
      'year' => $element['year']['#default_value'],
    );
    // Remove the default values, or the elements will be forced into month/day/year order.
    unset($element['month']);
    unset($element['day']);
    unset($element['year']);
  }
  // Or, if none, use set the defaults of the component.
  elseif (drupal_strlen($component['value']) > 0) {
    // Calculate the timestamp in GMT.
    $timestamp = strtotime(_webform_filter_values($component['value']));

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
      $timestamp += variable_get('date_default_timezone', 0);
    }

    // Check for daylight savings time.
    if ($component['extra']['check_daylight_savings'] && date('I')) {
      $timestamp += 3600;
    }

    $default_values = array(
      'day' => gmdate('j', $timestamp),
      'month' => gmdate('n', $timestamp),
      'year' => gmdate('Y', $timestamp),
    );
  }
  else {
    $default_values = array(
      'day' => NULL,
      'month' => NULL,
      'year' => NULL,
    );
  }

  // Let Drupal do it's normal expansion.
  $element = expand_date($element);

  // Set default values.
  foreach ($default_values as $type => $value) {
    switch ($type) {
      case 'month':
        $none = t('Month');
        break;
      case 'day':
        $none = t('Day');
        break;
      case 'year':
        $none = t('Year');
        break;
    }
    unset($element[$type]['#value']);
    $element[$type]['#default_value'] = isset($default_values[$type]) ? $default_values[$type] : NULL;
    $element[$type]['#options'] = array('' => $none) + $element[$type]['#options'];
  }

  // Tweak the year field.
  if ($component['extra']['year_textfield']) {
    $element['year']['#type'] = 'textfield';
    $element['year']['#size'] = 5;
    $element['year']['#maxlength'] = 4;
    unset($element['year']['#options']);
  }
  elseif (is_numeric($component['extra']['year_start']) && is_numeric($component['extra']['year_end'])) {
    $element['year']['#options'] = array('' => t('Year')) + drupal_map_assoc(range($component['extra']['year_start'], $component['extra']['year_end']));
  }

  return $element;
}

/**
 * Element validation for Webform date fields.
 */
function webform_validate_date($form_item, $form_state) {
  $component = $form_item['#webform_component'];
  $form_key = $component['form_key'];
  $name = $component['name'];

  // Check if the user filled the required fields.
  foreach (array('day', 'month', 'year') as $field_type) {
    if (!is_numeric($form_item[$field_type]['#value']) && $form_item['#required']) {
      form_set_error($form_key, t('!name field is required.', array('!name' => $name)));
      return;
    }
  }
  // Check for a valid date.
  if ($form_item['month']['#value'] !== '' || $form_item['day']['#value'] !== '' || $form_item['year']['#value'] !== '') {
    if (!checkdate((int)$form_item['month']['#value'], (int)$form_item['day']['#value'], (int)$form_item['year']['#value'])) {
      form_set_error($form_key, t('Entered !name is not a valid date.', array('!name' => $name)));
      return;
    }
  }
  // Check the date is between allowed years.
  if ($form_item['year']['#value'] !== '' && is_numeric($component['extra']['year_start']) && is_numeric($component['extra']['year_end'])) {
    if ($form_item['year']['#value'] < $component['extra']['year_start'] || $form_item['year']['#value'] > $component['extra']['year_end']) {
      form_set_error($form_key .'][year', t('The entered date needs to be between the years @start and @end.', array('@start' => $component['extra']['year_start'], '@end' => $component['extra']['year_end'])));
      return;
    }
  }
}

/**
 * Convert internationalized Date formats into the US order expected by Webform.
 *
 * @param $data
 *   The POST data associated with the component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Nothing.
 */
function _webform_submit_date(&$data, $component) {
  // Webform stores dates in month/day/year rows.
  // Ensure consistency when using international date formats.
  $data = array($data['month'], $data['day'], $data['year']);
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
function _webform_submission_display_date($data, $component, $enabled = FALSE) {
  $form_item = _webform_render_date($component);
  $form_item['month']['#default_value'] = $data['value']['0'];
  $form_item['day']['#default_value'] = $data['value']['1'];
  $form_item['year']['#default_value'] = $data['value']['2'];

  $form_item['#disabled'] = !$enabled;
  return $form_item;
}

/**
 * Format the output of emailed data for this component
 *
 * @param $data
 *   A string or array of the submitted data
 * @param $component
 *   An array of information describing the component,
 * directly correlating to the webform_component database schema.
 * @return string
 *   Textual output to be included in the email.
 */
function theme_webform_mail_date($data, $component) {
  $output = $component['name'] .':';
  if ($data[0] && $data[1]) {
    $timestamp = strtotime($data[0] .'/'. $data[1] .'/'. $data[2]);
    $format = webform_date_format('medium');
    $output .= ' '. format_date($timestamp, 'custom', $format, 0);
  }

  return $output;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_date($section) {
  switch ($section) {
    case 'admin/settings/webform#date_description':
      return t('Presents month, day, and year fields.');
  }
}

/**
 * Module specific instance of hook_theme().
 */
function _webform_theme_date() {
  return array(
    'webform_date' => array(
      'arguments' => array('element' => NULL),
    ),
    'webform_mail_date' => array(
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
function _webform_analysis_rows_date($component, $sids = array()) {
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
      $month = $row['data'];
      if ($row = db_fetch_array($result)) {
        if ($row['no'] == '1') {
          $day = $row['data'];
          if ($row = db_fetch_array($result)) {
            if ($row['no'] == '2') {
              $year = $row['data'];
              // Build the full timestamp.
              if (drupal_strlen($month) > 0  && drupal_strlen($day) > 0  && drupal_strlen($year) > 0) {
                $timestamp = strtotime($month .'/'. $day .'/'. $year);
                // Add usefull information about this date into an array.
                $timestamps[$timestamp] = array(
                  date('l', $timestamp), // Day of the week (Monday, Tuesday, etc.).
                  date('F', $timestamp), // Full Month name (January, February, etc.).
                  $year, // Year.
                  $day,  // Day of the month (1,2,...,31).
                );
              }
            }
          }
        }
      }
    }
  }

  // Display stats.
  // TODO: display date statistics in javascript tabs.
  $nonblanks = count($timestamps);
  $rows[0] = array( t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array( t('User entered value'), $nonblanks);
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
function _webform_table_data_date($data) {
  if (drupal_strlen($data['value']['0']) > 0 && drupal_strlen($data['value']['1']) > 0 && drupal_strlen($data['value']['2']) > 0) {
    $timestamp = strtotime($data['value']['0'] .'/'. $data['value']['1'] .'/'. $data['value']['2']);
    $format = webform_date_format('short');
    return date($format, $timestamp);
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
function _webform_csv_headers_date($component) {
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
function _webform_csv_data_date($data) {
  if (drupal_strlen($data['value']['0']) > 0 && drupal_strlen($data['value']['1']) > 0 && drupal_strlen($data['value']['2']) > 0) {
    $timestamp = strtotime($data['value']['0'] .'/'. $data['value']['1'] .'/'. $data['value']['2']);
    $format = webform_date_format('short');
    return date($format, $timestamp);
  }
  else {
    return '';
  }
}

/**
 * Get a date format according to the site settings.
 *
 * @param $size
 *   A choice of 'short', 'medium', or 'long' date formats.
 */
function webform_date_format($size = 'medium') {
    // Format date according to site's given format.
    $format = variable_get('date_format_'. $size, 'D, m/d/Y - H:i');
    $time = 'aABgGhHisueIOPTZ';
    $day_of_week = 'Dlw';
    $special = ',-: ';
    return trim($format, $time . $day_of_week . $special);
}
