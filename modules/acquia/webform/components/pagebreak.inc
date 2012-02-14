<?php
// $Id: pagebreak.inc,v 1.3.2.4 2009/01/09 08:31:06 quicksketch Exp $

/**
 * @file
 *   Webform module page break component.
 */

/**
 * Create a default pagebreak component.
 */
function _webform_defaults_pagebreak() {
  return array(
    'name' => '',
    'form_key' => NULL,
    'pid' => 0,
    'weight' => 0,
    'extra' => array(),
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
function _webform_edit_pagebreak($currfield) {
  drupal_set_message(t('Use caution when combining the pagebreak and file components. File components will not be submitted unless they are after the last pagebreak in the form.'), 'warning');

  $form = array();
  // No description.
  $form['extra']['description'] = array(
      '#type' => 'hidden',
      '#value' => '',
  );
  // Force the parent to always be root.
  $form['advanced']['pid'] = array(
      '#type' => 'hidden',
      '#value' => '0',
  );
  // Never mandatory.
  $form['advanced']['mandatory'] = array(
      '#type' => 'hidden',
      '#value' => '',
  );
  return $form;
}

/**
 * Module specific instance of hook_help().
 */
function _webform_help_pagebreak($section) {
  switch ($section) {
    case 'admin/settings/webform#pagebreak_description':
      return t('Break up a multi-page form.');
  }
}

function _webform_render_pagebreak($component) {
  $form = array();
  // Render page breaks as hidden elements so that they can be displayed in
  // emails as separators.
  $form = array(
    '#type' => 'hidden',
    '#value' => $component['name'],
    '#weight' => $component['weight'],
  );
  return $form;
}

/**
 * Module specific instance of hook_theme().
 */
function _webform_theme_pagebreak() {
  return array(
    'webform_mail_pagebreak' => array(
      'arguments' => array('data' => NULL, 'component' => NULL),
    ),
  );
}

/**
 * Format the output of e-mailed data for this component.
 *
 * @param $data
 *   A string or array of the submitted data.
 * @param $component
 *   An array of information describing the component, directly correlating to
 *   the webform_component database schema.
 * @return
 *   Textual output to be included in the email.
 */
function theme_webform_mail_pagebreak($data, $component) {
  return "\n-- ". $component['name'] ." --\n";
}
