<?php
// $Id: htmlmail.admin.inc,v 1.1.2.15 2010/03/01 03:00:25 chrisherberte Exp $

/**
 * @file
 * Admin forms for HTML Mail
 */

function htmlmail_admin_settings() {
  $form['htmlmail_autop'] = array(
    '#type' => 'checkbox',
    '#title' => t('Line break converter'),
    '#default_value' => variable_get('htmlmail_autop', '1'),
    '#description' => t('Converts line breaks into HTML (i.e. &lt;br&gt; and &lt;p&gt; tags).'),
  );
  $form['htmlmail_urlfilter'] = array(
    '#type' => 'checkbox',
    '#title' => t('URL Filter'),
    '#default_value' => variable_get('htmlmail_urlfilter', '1'),
    '#description' => t('Turns web and e-mail addresses into clickable links. URLs longer than 72 characters will be truncated.'),
  );
  $form['htmlmail_emogrifier'] = array(
    '#type' => 'checkbox',
    '#title' => t('Emogrifier'),
    '#default_value' => variable_get('htmlmail_emogrifier', '0'),
    '#description' => t('Insert your CSS definitions as inline styles into HTML tags for Outlook 2007 and Google Gmail.<br /><em>Checkbox will be disabled if DOM extension is not loaded.</em>'),
    '#disabled' => !extension_loaded('dom'),
  );
  $form['htmlmail_token'] = array(
    '#type' => 'checkbox',
    '#title' => t('Token support'),
    '#default_value' => variable_get('htmlmail_token', '0'),
    '#description' => t('Make use of the <strong><a href="http://drupal.org/project/token">Token</a></strong> module in order to tune up your mail header and footer.<br /><em>You need to have the module installed.</em>'),
    '#disabled' => !module_exists('token'),
  );
  $form['htmlmail_rel_to_abs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Convert relative links to absolute'),
    '#default_value' => variable_get('htmlmail_rel_to_abs', '0'),
    '#description' => t('Make use of the <strong><a href="http://drupal.org/project/rel_to_abs">Relative Path to Absolute URLs</a></strong> module to fix broken image links when using a WYSIWYG, untick if you want control over this.<br /><em>You need to have the module installed.</em>'),
    '#disabled' => !module_exists('rel_to_abs'),
  );
  $form['htmlmail_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug'),
    '#default_value' => variable_get('htmlmail_debug', '0'),
    '#description' => t('Add mail key to bottom of email. Useful for theme template suggestion or exclusion settings.'),
  );
  $form['htmlmail_plain'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plain Text Settings'),
    '#collapsible' => TRUE,
  );
  $form['htmlmail_plain']['htmlmail_plain_type'] = array(
    '#type' => 'radios',
    '#title' => t('Process with HTML Mail'),
    '#options' => array(
      t('Exclude e-mail sent from the listed modules.'),
      t('Incude e-mail send from the listed modules.'),
    ),
    '#default_value' => variable_get('htmlmail_plain_type', 0),
  );

  $form['htmlmail_plain']['htmlmail_plain_settings'] = array(
    '#type' => 'textarea',
    '#title' => t('Modules or mail keys'),
    '#default_value' => variable_get('htmlmail_plain_settings', ''),
    '#description' => t('Enter module name. eg: "user" (without quotes) for all user module emails. <em>Note there is a bug in Drupal 6 core which may make this unreliable.</em>'),
  );
  return system_settings_form($form);
}

function htmlmail_template_settings() {
  $form['htmlmail_header'] = array(
    '#type' => 'textarea',
    '#title' => t('Header HTML'),
    '#rows' => 6,
    '#default_value' => variable_get('htmlmail_header', ''),
  );
  if (module_exists('token') && variable_get('htmlmail_token', '0')) {
    $form['htmlmail_header_tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => theme('htmlmail_token_help'),
    );
  }
  $form['htmlmail_footer'] = array(
    '#type' => 'textarea',
    '#title' => t('Footer HTML'),
    '#rows' => 6,
    '#default_value' => variable_get('htmlmail_footer', ''),
  );
  if (module_exists('token') && variable_get('htmlmail_token', '0')) {
    $form['htmlmail_footer_tokens'] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns'),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
      '#description' => theme('htmlmail_token_help'),
    );
  }
  $form['htmlmail_css'] = array(
    '#type' => 'textarea',
    '#title' => t('CSS'),
    '#default_value' => variable_get('htmlmail_css', ''),
  );
  return system_settings_form($form);
}

function htmlmail_test_form($form_values = NULL) {
  $defaults = variable_get('htmlmail_test', array());
  $form['to'] = array(
    '#type' => 'textfield',
    '#title' => t('To'),
    '#default_value' => $defaults['to'],
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $defaults['subject'],
    '#maxlength' => 128,
    '#required' => TRUE,
  );
  $form['body'] = array(
    '#type' => 'textarea',
    '#title' => t('Body'),
    '#rows' => 20,
    '#default_value' => $defaults['body'],
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
  );
  return $form;
}

function htmlmail_test_form_submit($form, &$form_state) {
  // Save form values
  $defaults = array(
    'to' => $form_state['values']['to'],
    'subject' => $form_state['values']['subject'],
    'body' => $form_state['values']['body'],
  );
  variable_set('htmlmail_test', $defaults);
  // Send email
  $params = array(
    'subject' => $form_state['values']['subject'],
    'body' => $form_state['values']['body'],
  );
  if (drupal_mail('htmlmail', 'test', $form_state['values']['to'], language_default(), $params)) {
    drupal_set_message(t('HTML Mail test message sent.'));
  }
}

