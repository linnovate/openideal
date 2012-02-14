<?php
// $Id: rules_forms.admin.inc,v 1.1.2.2 2009/08/14 09:52:51 klausi Exp $


/**
 * @file
 * Implements forms events management screen.
 */

/**
 * Defines the forms events settings form
 */
function rules_forms_admin_events(&$form_state) {
  $form = array();
  $form['enable_form_activation_message'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable event activation messages on forms'),
    '#default_value' => isset($_SESSION['rules_forms_message']) ? $_SESSION['rules_forms_message'] : FALSE,
    '#description' => t('If checked, there will be a message on each form containing a link to activate events for the form. Only visible for your currently logged in user account.'),
  );
  $form['enable_form_element_ids'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display form element IDs'),
    '#default_value' => isset($_SESSION['rules_forms_element_ids']) ? $_SESSION['rules_forms_element_ids'] : FALSE,
    '#description' => t('If checked, the identifier of every single form element will be displayed on event-activated forms. Only visible for your currently logged in user account.'),
  );
  $form['settings_submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
    '#submit' => array('rules_forms_settings_submit'),
  );
  $form_events = variable_get('rules_forms_events', array());
  if (!empty($form_events)) {
    $form['form_events'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Forms where events are activated'),
      '#options' => $form_events,
      '#description' => t('Forms that currently invoke events. Select forms to deactivate events on them.'),
    );
    $form['events_deactivate'] = array(
      '#type' => 'submit',
      '#value' => t('Deactivate events'),
      '#submit' => array('rules_forms_events_deactivate_submit'),
    );
  }
  else {
    drupal_set_message('Enable the event activation messages below and go to the form you would like to activate events on.', 'status', FALSE);
  }
  return $form;
}

/**
 * Submit handler to save settings.
 */
function rules_forms_settings_submit($form_id, $form_values) {
  $_SESSION['rules_forms_message'] = (bool) $form_values['values']['enable_form_activation_message'];
  $_SESSION['rules_forms_element_ids'] = (bool) $form_values['values']['enable_form_element_ids'];
  drupal_set_message(t('The settings have been saved.'));
}

/**
 * Submit handler to deactivate form events.
 */
function rules_forms_events_deactivate_submit($form_id, $form_values) {
  $deactivate_events = array_filter($form_values['values']['form_events']);
  $form_events = variable_get('rules_forms_events', array());
  foreach ($deactivate_events as $key => $value) {
    unset($form_events[$key]);
  }
  variable_set('rules_forms_events', $form_events);
  drupal_set_message(t('The event settings have been saved.'));
}

/**
 * Activation page for a form ID.
 */
function rules_forms_activate($form_id_activate) {
  $form_events = variable_get('rules_forms_events', array());
  if (isset($form_events[$form_id_activate])) {
    return t('Events for %form_id have already been activated.', array('%form_id' => $form_id_activate));
  }
  return drupal_get_form('rules_forms_activate_form', $form_id_activate);
}

/**
 * Confirmation form to activate events on a form.
 */
function rules_forms_activate_form(&$form_state, $form_id_activate) {
  $default_form_label = drupal_ucfirst(str_replace('_', ' ', $form_id_activate));
  $form = array(
    'form_id_label' => array(
        '#type' => 'textfield',
        '#title' => t('Custom form label'),
        '#default_value' => $default_form_label,
        '#required' => TRUE,
        '#description' => t('Enter a custom label to better recognize the form in the administration user interface.'),
    ),
  );
  $form_state['form_id_activate'] = $form_id_activate;

  $path = array();
  $path['path'] = isset($_GET['destination']) ? $_GET['destination'] : RULES_ADMIN_FORMS_PATH;

  $title = t('Are you sure you want to activate events for %form?', array('%form' => $form_id_activate));
  $msg = t('Once the activation is confirmed, events on this form can be used to trigger rules.');
  return confirm_form($form, $title, $path, $msg, t('Activate'), t('Cancel'));
}

/**
 * Submit handler for activation of a form.
 */
function rules_forms_activate_form_submit($form, &$form_state) {
  $form_events = variable_get('rules_forms_events', array());
  $form_events[$form_state['form_id_activate']] = $form_state['values']['form_id_label'];
  variable_set('rules_forms_events', $form_events);
  drupal_set_message(t("%form has been activated.", array('%form' => $form_state['form_id_activate'])));
  $form_state['redirect'] = RULES_ADMIN_FORMS_PATH;
}
