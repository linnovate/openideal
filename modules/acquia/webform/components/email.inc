<?php
// $Id: email.inc,v 1.19.2.12 2009/03/04 05:05:12 quicksketch Exp $

/**
 * @file
 *   Webform module email component.
 */

/**
 * Create a default email component.
 */
function _webform_defaults_email() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'mandatory' => 0,
    'email' => 1,
    'extra' => array(
      'width' => '',
      'disabled' => 0,
      'email' => 0,
      'description' => '',
      'attributes' => array(),
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
function _webform_edit_email($currfield) {
  $edit_fields['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Default value'),
    '#default_value' => $currfield['value'],
    '#description' => t('The default value of the field.') . theme('webform_token_help'),
    '#size' => 60,
    '#maxlength' => 127,
    '#weight' => 0,
    '#attributes' => ($currfield['value'] == '%useremail' && count(form_get_errors()) == 0) ? array('disabled' => TRUE) : array(),
    '#id' => 'email-value',
  );
  $edit_fields['user_email'] = array(
    '#type' => 'checkbox',
    '#title' => t('User email as default'),
    '#default_value' => $currfield['value'] == '%useremail' ? 1 : 0,
    '#description' => t('Set the default value of this field to the user email, if he/she is logged in.'),
    '#attributes' => array('onclick' => 'getElementById("email-value").value = (this.checked ? "%useremail" : ""); getElementById("email-value").disabled = this.checked;'),
    '#weight' => 0,
    '#element_validate' => array('_webform_edit_email_validate'),
  );
  $edit_fields['extra']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $currfield['extra']['width'],
    '#description' => t('Width of the textfield.') .' '. t('Leaving blank will use the default size.'),
    '#size' => 5,
    '#maxlength' => 10,
  );
  $edit_fields['extra']['email'] = array(
    '#type' => 'checkbox',
    '#title' => t('E-mail a submission copy'),
    '#return_value' => 1,
    '#default_value' => $currfield['extra']['email'],
    '#description' => t('Check this option if this component contains an e-mail address that should get a copy of the submission. Emails are sent individually so other emails will not be shown to the recipient.'),
  );
  $edit_fields['extra']['disabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disabled'),
    '#return_value' => 1,
    '#description' => t('Make this field non-editable. Useful for setting an unchangeable default value.'),
    '#weight' => 3,
    '#default_value' => $currfield['extra']['disabled'],
  );
  return $edit_fields;
}

/**
 * Validation function for the email edit form.
 */
function _webform_edit_email_validate($element, &$form_state) {
  if ($form_state['values']['user_email']) {
    $form_state['values']['value'] = '%useremail';
  }
}

/**
 * Build a form item array containing all the properties of this component.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   An array of a form item to be displayed on the client-side webform.
 */
function _webform_render_email($component) {
  global $user;
  $form_item = array(
    '#type'          => 'textfield',
    '#title'         => $component['name'],
    '#default_value' => _webform_filter_values($component['value']),
    '#required'      => $component['mandatory'],
    '#weight'        => $component['weight'],
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
    '#attributes'    => $component['extra']['attributes'],
    '#prefix'        => '<div class="webform-component-'. $component['type'] .'" id="webform-component-'. $component['form_key'] .'">',
    '#suffix'        => '</div>',
    '#element_validate'  => array('_webform_validate_email'),
    '#webform_component' => $component,
  );

  if ($component['extra']['disabled']) {
    $form_item['#attributes']['readonly'] = 'readonly';
  }

  // Change the 'width' option to the correct 'size' option.
  if ($component['extra']['width'] > 0) {
    $form_item['#size'] = $component['extra']['width'];
  }

  return $form_item;
}

/**
 * A Drupal Forms API Validation function. Validates the entered values from
 * email components on the client-side form.
 * @param $form_element
 *   The e-mail form element.
 * @param $form_state
 *   The full form state for the webform.
 * @return
 *   None. Calls a form_set_error if the e-mail is not valid.
 */
function _webform_validate_email($form_element, $form_state) {
  $component = $form_element['#webform_component'];
  if (!empty($form_element['#value']) && !valid_email_address($form_element['#value'])) {
    form_error($form_element, t('%value is not a valid email address.', array('%value' => $form_element['#value'])));
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
function _webform_submission_display_email($data, $component, $enabled = FALSE) {
  $form_item = _webform_render_email($component);
  $form_item['#default_value'] = $data['value']['0'];
  if (!$enabled) {
    $form_item['#attributes']['readonly'] = 'readonly';
  }
  return $form_item;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_email($section) {
  switch ($section) {
    case 'admin/settings/webform#email_description':
      return t("A textfield that automatically fills in a logged-in user's e-mail.");
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
function _webform_analysis_rows_email($component, $sids = array()) {
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
    if (drupal_strlen(trim($data['data'])) > 0) {
      $nonblanks++;
      $wordcount += str_word_count(trim($data['data']));
    }
    $submissions++;
  }

  $rows[0] = array(t('Left Blank'), ($submissions - $nonblanks));
  $rows[1] = array(t('User entered value'), $nonblanks);
  $rows[2] = array(t('Average submission length in words (ex blanks)'), ($nonblanks != 0 ? number_format($wordcount/$nonblanks, 2) : '0'));
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
function _webform_table_data_email($data) {
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
function _webform_csv_headers_email($component) {
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
function _webform_csv_data_email($data) {
  return empty($data['value']['0']) ? '' : $data['value']['0'];
}
