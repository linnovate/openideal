<?php
// $Id: fieldset.inc,v 1.4.2.7 2009/01/09 08:31:06 quicksketch Exp $

/**
 * @file
 *   Webform module fieldset component.
 */

/**
 * Create a default fieldset component.
 */
function _webform_defaults_fieldset() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(
      'collapsible' => 0,
      'collapsed' => 0,
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
function _webform_edit_fieldset($currfield) {
  $edit_fields = array();
  $edit_fields['extra']['collapsible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsible'),
    '#default_value' => $currfield['extra']['collapsible'],
    '#description' => t('If this fieldset is collapsible, the user may open or close the fieldset.'),
    '#weight' => 0,
  );
  $edit_fields['extra']['collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Collapsed by Default'),
    '#default_value' => $currfield['extra']['collapsed'],
    '#description' => t('Collapsible fieldsets are "open" by default. Select this option to default the fieldset to "closed."'),
    '#weight' => 3,
  );
  $edit_fields['advanced']['mandatory'] = array();
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
function _webform_render_fieldset($component) {
  $form_item = array(
    '#type'          => $component['type'],
    '#title'         => htmlspecialchars($component['name'], ENT_QUOTES),
    '#weight'        => $component['weight'],
    '#description'   => _webform_filter_descriptions($component['extra']['description']),
    '#collapsible'   => $component['extra']['collapsible'],
    '#collapsed'     => $component['extra']['collapsed'],
    '#attributes'    => array('class' => 'webform-component-'. $component['type'], 'id' => 'webform-component-'. $component['form_key']),
  );

  return $form_item;
}

/**
 * Display the result of a fieldset submission. The output of this function will
 * be displayed under the "results" tab then "submissions".
 * @param $data
 *   An array of information containing the submission result, directly
 *   correlating to the webform_submitted_data database schema.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output formatted for human reading.
 */
function _webform_submission_display_fieldset($data, $component, $enabled = FALSE) {
  $form_item = _webform_render_fieldset($component);
  return $form_item;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_fieldset($section) {
  switch ($section) {
    case 'admin/settings/webform#fieldset_description':
      return t('Fieldsets allow you to organize multiple fields into groups.');
  }
}
