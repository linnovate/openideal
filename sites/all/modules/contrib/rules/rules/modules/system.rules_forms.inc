<?php
// $Id: system.rules_forms.inc,v 1.1.2.11 2009/07/31 10:21:43 fago Exp $


/**
 * @file Rules configuration forms for the system module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Action drupal message configuration form.
 */
function rules_action_drupal_message_form($settings, &$form) {
  $settings += array('message' => '', 'error' => '');

  $form['settings']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $settings['message'],
    '#description' => t('The message that should be displayed.'),
  );
  $form['settings']['error'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display as error message'),
    '#default_value' => $settings['error'],
  );
}

/**
 * Action "Set breadcrumb" configuration form
 */
function rules_action_set_breadcrumb_form($settings = array(), &$form) {
  $settings += array('titles' => '', 'paths' => '');
  $form['settings']['titles'] = array(
    '#type' => 'textarea',
    '#title' => t('Titles'),
    '#required' => TRUE,
    '#description' => t('A list of titles for the breadcrumb links, one on each line.'),
    '#default_value' => $settings['titles'],
  );
  $form['settings']['paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths'),
    '#required' => TRUE,
    '#description' => t('A list of Drupal paths for the breadcrumb links, one on each line.'),
    '#default_value' => $settings['paths'],
  );
}

/**
 * Action "Send a mail to a user" configuration form
 */
function rules_action_mail_to_user_form($settings = array(), &$form) {
  $settings += array('from' => '', 'subject' => '', 'message' => '');
  $form['settings']['from'] = array(
    '#type' => 'textfield',
    '#title' => t('Sender'),
    '#default_value' => $settings['from'],
    '#description' => t("The mail's from address. Leave it empty to use the site-wide configured address."),
  );
  $form['settings']['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#default_value' => $settings['subject'],
    '#description' => t("The mail's subject."),
  );
  $form['settings']['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#default_value' => $settings['message'],
    '#description' => t("The mail's message body."),
  );
}

/**
 * Action "Send a mail to an arbitrary mail address" configuration form
 */
function rules_action_mail_form($settings = array(), &$form) {
  $form['settings']['to'] = array(
    '#type' => 'textfield',
    '#title' => t('Recipient'),
    '#default_value' => isset($settings['to']) ? $settings['to'] : '',
    '#description' => t("The mail's recipient address. You may separate multiple addresses with ','."),
    '#required' => TRUE,
    '#weight' => -1,
  );
  rules_action_mail_to_user_form($settings, $form);
}

/**
 * Action "Send a mail to users of a role" configuration form
 */
function rules_action_mail_to_users_of_role_form($settings = array(), &$form) {
  // Select only non-anonymous user roles because anonymous users won't have emails.
  $roles = array_map('filter_xss_admin', user_roles(TRUE));

  $form['settings']['recipients'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Recipient roles'),
    '#prefix' => t('WARNING: This may cause problems if there are too many users of these roles on your site, as your server may not be able to handle all the mail requests all at once.'),
    '#required' => TRUE,
    '#default_value' => isset($settings['recipients']) ? $settings['recipients'] : array(),
    '#options' => $roles,
    '#description' => t('Select the roles whose users should receive this email.'),
  );
  rules_action_mail_to_user_form($settings, $form);
}

function rules_action_mail_to_users_of_role_submit(&$settings, $form, $form_state) {
  $settings['recipients'] = array_filter(array_keys(array_filter($settings['recipients'])));
}

/**
 * Action "Page redirect" configuration form
 */
function rules_action_drupal_goto_form($settings = array(), &$form, $form_state) {
  $settings += array('path' => '', 'query' => '', 'fragment' => '', 'force' => 0);
  // For settings backward compatibility respect the override setting as default
  $settings += array('immediate' => isset($settings['override']) && !$settings['override']);

  $form['settings']['path'] = array(
    '#type' => 'textfield',
    '#size' => 30,
    '#prefix' => '<br /><div class="container-inline">',
    '#title' => t('To'),
    '#default_value' => $settings['path'],
  );
  $form['settings'][] = array(
    '#value' => '?',
  );
  $form['settings']['query'] = array(
    '#type' => 'textfield',
    '#size' => 12,
    '#default_value' => $settings['query'],
  );
  $form['settings'][] = array(
    '#value' => '#',
  );
  $form['settings']['fragment'] = array(
    '#type' => 'textfield',
    '#size' => 12,
    '#default_value' => $settings['fragment'],
    '#suffix' => '</div>',
  );
  $form['settings']['force'] = array(
    '#type' => 'checkbox',
    '#title' => t('Force redirecting to the given path, even if a destination parameter is given'),
    '#description' => t("Per default drupal doesn't redirect to the given path, if a destination parameter is set. Instead it redirects to the given destination parameter. Most times, the destination parameter is set by appending it to the URL, e.g. !example_url", array('!example_url' => 'http://example.com/user/login?destination=node/2')),
    '#default_value' => $settings['force'],
    '#weight' => 7,
  );
  $form['settings']['immediate'] = array(
    '#type' => 'checkbox',
    '#title' => t('Immediately issue the page redirect'),
    '#description' => t("Use this with <em>caution</em>! If checked, the path redirect is issued immediately, so the normal execution flow is interrupted."),
    '#default_value' => $settings['immediate'],
    '#weight' => 8,
  );
}

/**
 * Action "Watchdog" configuration form
 */
function rules_action_watchdog_form($settings = array(), &$form) {
  $form['settings']['severity'] = array(
    '#type' => 'select',
    '#title' => t('Severity'),
    '#options' => watchdog_severity_levels(),
    '#default_value' => isset($settings['severity']) ? $settings['severity'] : WATCHDOG_NOTICE,
    '#required' => TRUE,
  );
  $form['settings']['type'] = array(
    '#type' => 'textfield',
    '#title' => t('Category'),
    '#default_value' => isset($settings['type']) ? $settings['type'] : 'rules',
    '#description' => t('The category to which this message belongs.'),
    '#required' => TRUE,
  );
  $form['settings']['message'] = array(
    '#type' => 'textfield',
    '#title' => t('Message'),
    '#default_value' => isset($settings['message']) ? $settings['message'] : '',
    '#description' => t('The message to log.'),
    '#required' => TRUE,
  );
  $form['settings']['link'] = array(
    '#type' => 'textfield',
    '#title' => t('Link (optional)'),
    '#default_value' => isset($settings['link']) ? $settings['link'] : '',
    '#description' => t('A link to associate with the message.'),
  );
}

/**
 * @}
 */
