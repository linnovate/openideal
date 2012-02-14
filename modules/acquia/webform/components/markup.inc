<?php
// $Id: markup.inc,v 1.5.2.8 2010/02/10 03:19:41 quicksketch Exp $

/**
 * @file
 *   Webform module markup component.
 */

/**
 * Create a default markup component.
 */
function _webform_defaults_markup() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'value' => '',
    'extra' => array(
      'format' => FILTER_FORMAT_DEFAULT,
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
function _webform_edit_markup($currfield) {
  $edit_fields = array();
  $edit_fields['advanced']['mandatory'] = array(); // Do not render the mandatory checkbox.
  $edit_fields['markup']['value'] = array(
    '#type' => 'textarea',
    '#title' => t('Value'),
    '#default_value' => $currfield['value'],
    '#description' => t('Markup allows you to enter custom HTML or PHP logic into your form.') . theme('webform_token_help'),
    '#weight' => -1,
    '#parents' => array('value'),
  );
  // Add the filter form.
  $edit_fields['markup']['format'] = filter_form($currfield['extra']['format'], 0, array('extra', 'format'));

  // No description for markup.
  $edit_fields['extra']['description'] = array();

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
function _webform_render_markup($component) {
  // We don't want users to be able to execute filters outside of their permissions on preview.
  if ($_POST['op'] == t('Preview')) {
    $check_filter = TRUE;
  }
  else {
    $check_filter = FALSE;
  }

  $form_item = array(
    '#type'   => 'markup',
    '#weight' => $component['weight'],
    '#value'  => _webform_filter_values(check_markup($component['value'], $component['extra']['format'], $check_filter), NULL, NULL, FALSE),
    '#prefix' => '<div class="webform-component-'. $component['type'] .'" id="webform-component-'. $component['form_key'] .'">',
    '#suffix' => '</div>',
  );
  return $form_item;
}

/**
 * Display the markup  in the results. The output of this function will be
 * displayed under the "results" tab then "submissions".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_submission_display_markup($data, $component, $enabled = FALSE) {
  $form_item = _webform_render_markup($component);
  return $form_item;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_markup($section) {
  switch ($section) {
    case 'admin/settings/webform#markup_description':
      return t('Displays text as HTML in the form; does not render a field.');
  }
}
