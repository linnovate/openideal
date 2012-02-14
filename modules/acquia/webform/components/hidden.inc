<?php
// $Id: hidden.inc,v 1.12.2.11 2009/03/04 05:05:12 quicksketch Exp $

/**
 * @file
 *   Webform module hidden component.
 */

/**
 * Create a default hidden component.
 */
function _webform_defaults_hidden() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'email' => 0,
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
function _webform_edit_hidden($currfield) {
  $edit_fields = array();
  $edit_fields['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Default value'),
    '#default_value' => $currfield['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#cols' => 60,
    '#rows' => 5,
    '#weight' => 0,
  );
  $edit_fields['advanced']['mandatory'] = array(
    '#type' => 'hidden',
    '#value' => 1,
  );
  $edit_fields['extra']['email'] = array(
    '#type' => 'checkbox',
    '#title' => t('E-mail a submission copy'),
    '#return_value' => 1,
    '#default_value' => $currfield['extra']['email'],
    '#description' => t('Check this option if this component contains an e-mail address that should get a copy of the submission. Emails are sent individually so other emails will not be shown to the recipient.'),
  );
  $edit_fields['extra']['description'] = array(); // Hide the description box.
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
function _webform_render_hidden($component) {
  $form_item = array(
    '#type'          => 'hidden',
    '#default_value' => _webform_filter_values($component['value']),
    '#weight'        => $component['weight'],
  );

  return $form_item;
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
function _webform_submission_display_hidden($data, $component, $enabled = FALSE) {
  $form_item = _webform_render_hidden($component);
  // Only allow administrators that can view or edit all submissions to view or edit hidden fields.
  if (user_access('edit webform submissions') || user_access('access webform results')) {
    unset($form_item['#value']);
    $form_item['#default_value'] = $data['value']['0'];
    $form_item['#type'] = 'textfield';
    $form_item['#title'] = t('@name (hidden)', array('@name' => $component['name']));
    if (!$enabled) {
      $form_item['#attributes']['readonly'] = 'readonly';
    }
  }
  return $form_item;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_hidden($section) {
  switch ($section) {
    case 'admin/settings/webform#hidden_description':
      return t('A field which is not visible to the user, but is recorded with the submission.');
  }
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
function _webform_analysis_rows_hidden($component, $sids = array()) {
  $placeholders = count($sids) ? array_fill(0, count($sids), "'%s'") : array();
  $sidfilter = count($sids) ? " AND sid in (".implode(",", $placeholders).")" : "";
  $query = 'SELECT data '.
    ' FROM {webform_submitted_data} '.
    ' WHERE nid = %d '.
    ' AND cid = %d '. $sidfilter;
  $nonblanks = 0;
  $submissions = 0;
  $wordcount = 0;

  $result = db_query($query, array_merge(array($component['nid'], $component['cid']), $sids));
  while ($data = db_fetch_array($result)) {
    if ( strlen(trim($data['data'])) > 0 ) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }

  $rows[0] = array( t('Empty'), ($submissions - $nonblanks));
  $rows[1] = array( t('Non-empty'), $nonblanks);
  $rows[2] = array( t('Average submission length in words (ex blanks)'),
                    ($nonblanks !=0 ? number_format($wordcount/$nonblanks, 2) : '0'));
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
function _webform_table_data_hidden($data) {
  return check_plain(empty($data['value']['0']) ? '' : $data['value']['0']);
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
function _webform_csv_headers_hidden($component) {
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
function _webform_csv_data_hidden($data) {
  return empty($data['value']['0']) ? '' : $data['value']['0'];
}
