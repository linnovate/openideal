<?php 

/**
 * Form for input term ID or term name. 
 * @param $settings
 * @param $form
 * @param $form_state
 */
function ideal_condition_content_has_term_form($settings, &$form, $form_state) {
  $form['settings']['term'] = array(
    '#type' => 'textfield',
    '#title' => t('Term ID or term name'),
    '#default_value' => !empty($settings['term']) ? $settings['term'] : '',
    '#description' => t("Provide a term ID (tid) or a term name matching text (case is ignore)."),
    '#required' => TRUE,
  );
}

/**
 * Action "Send a mail to a user" configuration form
 */
function ideal_action_send_brand_email_form($settings = array(), &$form) {
//  $settings += array('from' => '', 'subject' => '', 'message' => '');
//  $form['settings']['from'] = array(
//    '#type' => 'textfield',
//    '#title' => t('Sender'),
//    '#default_value' => $settings['from'],
//    '#description' => t("The mail's from address. Leave it empty to use the site-wide configured address."),
//  );
//  $form['settings']['subject'] = array(
//    '#type' => 'textfield',
//    '#title' => t('Subject'),
//    '#default_value' => $settings['subject'],
//    '#description' => t("The mail's subject."),
//  );
//  $form['settings']['message'] = array(
//    '#type' => 'textarea',
//    '#title' => t('Message'),
//    '#default_value' => $settings['message'],
//    '#description' => t("The mail's message body."),
//  );
  rules_action_mail_to_user_form($settings, $form);
}