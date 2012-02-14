<?php
// $Id: rules_forms.rules_forms.inc,v 1.1.2.5 2010/03/18 11:18:10 klausi Exp $

/**
 * @file
 * Contains forms for rules forms actions.
 */

/**
 * Action redirect configuration form.
 */
function rules_forms_action_redirect_form($settings, &$form) {
  $settings += array('path' => '', 'query' => '', 'fragment' => '');

  $form['settings']['path'] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#prefix' => '<br /><div class="container-inline">',
    '#title' => t('To'),
    '#suffix' => '?',
    '#default_value' => $settings['path'],
  );
  $form['settings']['query'] = array(
    '#type' => 'textfield',
    '#size' => 12,
    '#default_value' => $settings['query'],
    '#suffix' => '#',
  );
  $form['settings']['fragment'] = array(
    '#type' => 'textfield',
    '#size' => 12,
    '#default_value' => $settings['fragment'],
    '#suffix' => '</div>',
  );
}

/**
 * Action adjust weight configuration form.
 */
function rules_forms_action_adjust_weight_form($settings, &$form) {
  $form['settings']['weight']['#type'] = 'weight';
  $form['settings']['weight']['#delta'] = 50;
  if (isset($settings['weight'])) {
    $form['settings']['weight']['#default_value'] = $settings['weight'];
  }
  else {
    $form['settings']['weight']['#default_value'] = 0;
  }
}

/**
 * Action prefix/suffix code configuration form.
 */
function rules_forms_action_prefix_suffix_html_form($settings, &$form) {
  $form['settings']['element']['#required'] = FALSE;
  $form['settings']['element']['#description'] = t('ID of the form element to be targeted. Leave empty to apply prefix/suffix code to the whole form.');
  $form['settings']['prefix']['#type'] = 'textarea';
  $form['settings']['prefix']['#required'] = FALSE;
  $form['settings']['suffix']['#type'] = 'textarea';
  $form['settings']['suffix']['#required'] = FALSE;
}

/**
 * Action set error configuration form.
 */
function rules_forms_action_set_error_form($settings, &$form) {
  $form['settings']['element']['#required'] = FALSE;
  $form['settings']['message']['#type'] = 'textarea';
}

/**
 * Condition form element value configuration form.
 */
function rules_forms_condition_element_value_form($settings, &$form) {
  $form['settings']['value']['#type'] = 'textarea';
}

/**
 * Validation of the prefix/suffix action configuration form.
 */
function rules_forms_action_prefix_suffix_html_validate($form, $form_state) {
  if (empty($form_state['values']['settings']['prefix']) && empty($form_state['values']['settings']['suffix'])) {
    form_set_error('settings][prefix', t('You have to specify at least the prefix or suffix field.'));
  }
}

/**
 * Action set element configuration form.
 */
function rules_forms_action_set_element_form($settings, &$form) {
  $form['settings']['value']['#type'] = 'textarea';
  $form['settings']['value']['#required'] = FALSE;
}

/**
 * Label callback for adjusting weights action.
 */
function rules_forms_action_adjust_weight_label($settings, $argument_labels) {
  return t("Adjust weight of form element '@element' to @weight", array('@element' => $settings['element'], '@weight' => $settings['weight']));
}

/**
 * Label callback for hiding form elements action.
 */
function rules_forms_action_hide_element_label($settings, $argument_labels) {
  return t("Hide form element '@element'", array('@element' => $settings['hide']));
}

/**
 * Label callback for redirect action.
 */
function rules_forms_action_redirect_label($settings, $argument_labels) {
  $redirect = $settings['path'];
  if (!empty($settings['query'])) {
    $redirect .= '?'. $settings['query'];
  }
  if (!empty($settings['fragment'])) {
    $redirect .= '#'. $settings['fragment'];
  }
  return t("Set form redirect target to '@redirect'", array('@redirect' => $redirect));
}

/**
 * Label callback for prefix/suffix action.
 */
function rules_forms_action_prefix_suffix_html_label($settings, $argument_labels) {
  if (empty($settings['element'])) {
    return t('Insert HTML prefix/suffix code on form');
  }
  else {
    return t("Insert HTML prefix/suffix code on '@element'", array('@element' => $settings['element']));
  }
}

/**
 * Label callback for set error action.
 */
function rules_forms_action_set_error_label($settings, $argument_labels) {
  if (empty($settings['element'])) {
    return t('Set a form error');
  }
  return t("Set form error on element '@element'", array('@element' => $settings['element']));
}

/**
 * Label callback for set default value action.
 */
function rules_forms_action_set_element_label($settings, $argument_labels) {
  return t("Set default value on form element '@element'", array('@element' => $settings['element']));
}

/**
 * Label callback for element value condition.
 */
function rules_forms_condition_element_value_label($settings, $argument_labels) {
  return t("Form element '@element' value check", array('@element' => $settings['element']));
}
