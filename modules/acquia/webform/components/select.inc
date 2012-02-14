<?php
// $Id: select.inc,v 1.22.2.31 2010/02/07 04:48:36 quicksketch Exp $

/**
 * @file
 *   Webform module multiple select component.
 */

/**
 * Create a default select component.
 */
function _webform_defaults_select() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'email' => 1,
    'mandatory' => 0,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'items' => '',
      'email' => 0,
      'multiple' => NULL,
      'aslist' => NULL,
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
function _webform_edit_select($currfield) {
  $edit_fields = array();
  $edit_fields['extra']['items'] = array(
    '#type' => 'textarea',
    '#title' => t('Options'),
    '#default_value' => $currfield['extra']['items'],
    '#description' => t('A list of selectable options. One option per line. Key-value pairs may be entered seperated by pipes, such as "safe_key|Some readable option". Option groups for lists and menus may be specified with &lt;Group Name&gt;. &lt;&gt; can be used to insert items at the root of the menu after specifying a group.') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => -2,
    '#required' => TRUE,
    '#element_validate' => array('_webform_edit_validate_select'),
  );
  $edit_fields['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $currfield['value'],
    '#description' => t('The default value of the field. For multiple selects use commas to separate multiple defaults.') . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 256,
    '#weight' => 0,
  );
  $edit_fields['extra']['multiple'] = array(
    '#type' => 'checkbox',
    '#title' => t('Multiple'),
    '#return_value' => 'Y',
    '#default_value' => $currfield['extra']['multiple'],
    '#description' => t('Check this option if the user should be allowed to choose multiple values.'),
  );
  $edit_fields['extra']['aslist'] = array(
    '#type' => 'checkbox',
    '#title' => t('Listbox'),
    '#return_value' => 'Y',
    '#default_value' => $currfield['extra']['aslist'],
    '#description' => t('Check this option if you want the select component to be of listbox type instead of radiobuttons or checkboxes.'),
  );
  $edit_fields['extra']['email'] = array(
    '#type' => 'checkbox',
    '#title' => t('E-mail a submission copy'),
    '#return_value' => 1,
    '#default_value' => $currfield['extra']['email'],
    '#description' => t('Check this option if this component contains an e-mail address that should get a copy of the submission. Emails are sent individually so other emails will not be shown to the recipient.') .' '.
                      t('To use the option with a select component, you must use key-value pairs seperated by pipes. i.e. user@example.com|Sample user.'),
  );
  return $edit_fields;
}

/**
 * Element validation callback. Ensure keys are not duplicated.
 */
function _webform_edit_validate_select($element, $form_state) {
  // TODO: Validate e-mail addresses when used as keys?\

  // Check for duplicate key values to prevent unexpected data loss.
  if (!empty($element['#value'])) {
    $lines = explode("\n", $element['#value']);
    $existing_keys = array();
    $duplicate_keys = array();
    $group = '';
    foreach ($lines as $line) {
      $matches = array();
      $line = trim($line);
      if (preg_match('/^\<([^>]*)\>$/', $line, $matches)) {
        $group = $matches[1];
        $key = NULL; // No need to store group names.
      }
      elseif (preg_match('/^([^|]+)\|(.*)$/', $line, $matches)) {
        $key = $matches[1];
      }
      else {
        $key = $line;
      }

      if (isset($key)) {
        if (isset($existing_keys[$group][$key])) {
          $duplicate_keys[$key] = $key;
        }
        else {
          $existing_keys[$group][$key] = $key;
        }
      }
    }

    if (!empty($duplicate_keys)) {
      form_error($element, t('Options within the select list must be unique. The following keys have been used multiple times:') . theme('item_list', $duplicate_keys));
    }

  }

  return TRUE;
}

/**
 * Build a form item array containing all the properties of this component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   An array of a form item to be displayed on the client-side webform.
 */
function _webform_render_select($component) {
  $form_item = array(
    '#title'         => $component['name'],
    '#required'      => $component['mandatory'],
    '#weight'        => $component['weight'],
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
    '#prefix'        => '<div class="webform-component-'. $component['type'] .'" id="webform-component-'. $component['form_key'] .'">',
    '#suffix'        => '</div>',
  );

  // Convert the user-entered options list into an array.
  $default_value = _webform_filter_values($component['value'], NULL, NULL, FALSE);
  $options = _webform_select_options($component['extra']['items'], $component['extra']['aslist'] !== 'Y');

  if ($component['extra']['aslist'] === 'Y' && $component['extra']['multiple'] !== 'Y' && !($component['mandatory'] && !empty($component['value']))) {
    $options = array('' => t('select...')) + $options;
  }

  // Set the component options.
  $form_item['#options'] = $options;

  // Set the default value.
  if ($default_value != '') {
    // Convert default value to a list if necessary.
    if ($component['extra']['multiple'] === 'Y') {
      $varray = array_filter(explode(',', $default_value));
      foreach ($varray as $key => $v) {
        $form_item['#default_value'][] = $v;
      }
    }
    else {
      $form_item['#default_value'] = $default_value;
    }
  }

  if ($component['extra']['aslist'] === 'Y') {
    // Set display as a select list:
    $form_item['#type'] = 'select';
    if ($component['extra']['multiple'] === 'Y') {
      $form_item['#multiple'] = TRUE;
    }
  }
  else {
    if ($component['extra']['multiple'] === 'Y') {
      // Set display as a checkbox set.
      $form_item['#type'] = 'checkboxes';
      // Drupal 6 hack to properly render on multipage forms.
      $form_item['#process'] = array('webform_expand_checkboxes');
    }
    else {
      // Set display as a radio set.
      $form_item['#type'] = 'radios';
    }
  }
  return $form_item;
}

/**
 * Drupal 6 hack that properly *renders* checkboxes in multistep forms. This is
 * different than the value hack needed in Drupal 5, which is no longer needed.
 */
function webform_expand_checkboxes($element) {
  // Elements that have a value set are already in the form structure cause
  // them not to be written when the expand_checkboxes function is called.
  $default_value = array();
  foreach (element_children($element) as $key) {
    if (isset($element[$key]['#default_value'])) {
      $default_value[$key] = $element[$key]['#default_value'];
      unset($element[$key]);
    }
  }

  $element = expand_checkboxes($element);
  foreach ($default_value as $key=>$val) {
    $element[$key]['#default_value'] = $val;
  }
  return $element;
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
function _webform_submission_display_select($data, $component, $enabled = FALSE) {
  $form_item = _webform_render_select($component);
  if ($component['extra']['multiple'] === 'Y') {
    // Set the value as an array.
    $form_item['#default_value'] = array();
    foreach ((array)$data['value'] as $key => $value) {
      $form_item['#default_value'][] = $value;
    }
  }
  else {
    // Set the value as a single string.
    $form_item['#default_value'] = '';
    foreach ((array)$data['value'] as $value) {
      $form_item['#default_value'] = $value;
    }
  }
  $form_item['#disabled'] = !$enabled;
  return $form_item;
}


/**
 * Convert FAPI 0/1 values into something saveable.
 *
 * @param $data
 *   The POST data associated with the component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Nothing.
 */
function _webform_submit_select(&$data, $component) {
  $options = drupal_map_assoc(array_flip(_webform_select_options($component['extra']['items'], TRUE)));

  if (is_array($data)) {
    foreach ($data as $key => $value) {
      if ($value != '') {
        $data[$key] = $options[$key];
      }
      // Checkboxes submit a value of 0 when not checked.
      elseif ($value == 0 && $component['extra']['aslist'] !== 'Y' && $component['extra']['multiple'] === 'Y') {
        unset($data[$key]);
      }
      else {
        unset($data[$key]);
      }
    }
  }
}

/**
 * Format the output of emailed data for this component.
 *
 * @param $data
 *   A string or array of the submitted data.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output to be included in the email.
 */
function theme_webform_mail_select($data, $component) {
  // Convert submitted 'safe' values to un-edited, original form.
  $options = _webform_select_options($component['extra']['items'], TRUE);

  // Generate the output.
  $output = '';
  if ($component['extra']['multiple']) {
    $output .= $component['name'] .":\n";
    foreach ((array)$data as $value) {
      if ($value) {
        if ($options[$value]) {
          $output .= '    - '. $options[$value] ."\n";
        }
      }
    }
  }
  else {
    if ($data !== '' && $options[$data]) {
      $output .= $component['name'] .": ". $options[$data] ."\n";
    }
  }
  return $output;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_select($section) {
  switch ($section) {
    case 'admin/settings/webform#select_description':
      return t('Allows creation of checkboxes, radio buttons, or select menus.');
  }
}

/**
 * Module specific instance of hook_theme().
 */
function _webform_theme_select() {
  return array(
    'webform_mail_select' => array(
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
function _webform_analysis_rows_select($component, $sids = array()) {
  $options = _webform_select_options($component['extra']['items'], TRUE);
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (".implode(",", $placeholders).")" : "";

  $query = 'SELECT data, count(data) as datacount '.
    ' FROM {webform_submitted_data} '.
    ' WHERE nid = %d '.
    ' AND cid = %d '.
    " AND data != '0' AND data != '' $sidfilter ".
    ' GROUP BY data ';
  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  $rows = array();
  while ($data = db_fetch_array($result)) {
    if (isset($options[$data['data']])) {
      $display_option = $options[$data['data']];
    }
    else {
      $display_option = $data['data'];
    }
    $rows[] = array($display_option, $data['datacount']);
  }
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
function _webform_table_data_select($data) {
  $output = '';
  // Set the value as a single string.
  if (is_array($data['value'])) {
    foreach ($data['value'] as $value) {
      if ($value !== '0') {
        $output .= check_plain($value) .'<br />';
      }
    }
  }
  else {
    $output .= check_plain(empty($data['value']['0']) ? '' : $data['value']['0']);
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
function _webform_csv_headers_select($component) {
  $headers = array(
    0 => array(),
    1 => array(),
    2 => array(),
  );

  if ($component['extra']['multiple']) {
    $headers[0][] = '';
    $headers[1][] = $component['name'];
    $items = _webform_select_options($component['extra']['items'], TRUE);
    $count = 0;
    foreach ($items as $key => $item) {
      // Empty column per sub-field in main header.
      if ($count != 0) {
        $headers[0][] = '';
        $headers[1][] = '';
      }
      $headers[2][] = $key;
      $count++;
    }
  }
  else {
    $headers[0][] = '';
    $headers[1][] = '';
    $headers[2][] = $component['name'];
  }
  return $headers;
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
function _webform_csv_data_select($data, $component) {
  $value = _webform_filter_values($component['value'], NULL, NULL, FALSE);
  $options = _webform_select_options($component['extra']['items'], TRUE);
  $return = array();

  if ($component['extra']['multiple']) {
    foreach ($options as $key => $item) {
      
      if (in_array($key, (array)$data['value']) === TRUE) {
        $return[] = 'X';
      }
      else {
        $return[] = '';
      }
    }
  }
  else {
    $return = $data['value'][0];
  }
  return $return;
}

/**
 * Utility function to split user-entered values from new-line seperated
 * text into an array of options.
 *
 * @param $text
 *   Text to be converted into a select option array.
 * @param $flat
 *   Optional. If specified, return the option array and exclude any optgroups.
 */
function _webform_select_options($text, $flat = FALSE) {
  $options = array();
  $rows = array_filter(explode("\n", trim($text)));
  $group = NULL;
  foreach ($rows as $option) {
    $option = trim($option);
    /**
     * If the Key of the option is within < >, treat as an optgroup
     *
     * <Group 1>
     *   creates an optgroup with the label "Group 1"
     *
     * <>
     *   Unsets the current group, allowing items to be inserted at the root element.
     */

    if (preg_match('/^\<([^>]*)\>$/', $option, $matches)) {
      if (empty($matches[1])) {
        unset($group);
      }
      elseif (!$flat) {
        $group = _webform_filter_values($matches[1], NULL, NULL, FALSE);
      }
    }
    elseif (preg_match('/^([^|]+)\|(.*)$/', $option, $matches)) {
      $key = _webform_filter_values($matches[1], NULL, NULL, FALSE);
      $value = _webform_filter_values($matches[2], NULL, NULL, FALSE);
      isset($group) ? $options[$group][$key] = $value : $options[$key] = $value;
    }
    else {
      $filtered_option = _webform_filter_values($option, NULL, NULL, FALSE);
      isset($group) ? $options[$group][$filtered_option] = $filtered_option : $options[$filtered_option] = $filtered_option;
    }
  }
  return $options;
}
