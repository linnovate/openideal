<?php
// $Id: print_mail.admin.inc,v 1.1.2.17 2010/08/12 08:43:50 jcnventura Exp $

/**
 * @file
 * Contains the administrative functions of the send by e-mail module.
 *
 * This file is included by the send by e-mail module, and includes the
 * settings form.
 *
 * @ingroup print
 */

// Include MIME library
@include_once('Mail/mime.php');

/**
 * Menu callback for the send by e-mail module settings form.
 *
 * @ingroup forms
 */
function print_mail_settings() {
  $form['settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Send by e-mail options'),
  );

  $form['settings']['print_mail_link_pos'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Send by e-mail link'),
    '#default_value' => variable_get('print_mail_link_pos', array(PRINT_MAIL_LINK_POS_DEFAULT => PRINT_MAIL_LINK_POS_DEFAULT)),
    '#options' => array('link' => t('Links area'), 'corner' => t('Content corner')),
    '#description' => t('Choose the location of the link(s) to the send by e-mail page. The Links area is usually below the node content, whereas the Content corner is placed in the upper-right corner of the node content. Unselect all options to disable the link. Even if the link is disabled, you can still send a node by e-mail by going to !path/nid where nid is the numeric id of the node.', array('!path' => PRINTMAIL_PATH)),
  );

  $form['settings']['print_mail_link_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Display send by e-mail link in teaser'),
    '#default_value' => variable_get('print_mail_link_teaser', PRINT_MAIL_LINK_TEASER_DEFAULT),
    '#description' => t('Enabling this will display the link in teaser mode.'),
  );

  $form['settings']['adv_link'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced link options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['settings']['adv_link']['print_mail_show_link'] = array(
    '#type' => 'radios',
    '#title' => t('Link style'),
    '#default_value' => variable_get('print_mail_show_link', PRINT_MAIL_SHOW_LINK_DEFAULT),
    '#options' => array(1 => t('Text only'), 2 => t('Icon only'), 3 => t('Icon and Text')),
    '#description' => t('Select the visual style of the link.'),
  );

  $form['settings']['adv_link']['print_mail_link_use_alias'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use URL alias instead of node ID'),
    '#default_value' => variable_get('print_mail_link_use_alias', PRINT_MAIL_LINK_USE_ALIAS_DEFAULT),
    '#description' => t('Enabling this will create the link using the URL alias instead of the node ID.'),
  );

  $form['settings']['adv_link']['print_mail_link_class'] = array(
    '#type' => 'textfield',
    '#title' => t('Link class'),
    '#default_value' => variable_get('print_mail_link_class', PRINT_MAIL_LINK_CLASS_DEFAULT),
    '#size' => 60,
    '#maxlength' => 250,
    '#description' => t('This can be used by themers to change the link style or by jQuery modules to open in a new window (e.g. greybox or thickbox). Multiple classes can be specified, separated by spaces.'),
  );

  $form['settings']['adv_link']['print_mail_node_link_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Link visibility'),
    '#default_value' => variable_get('print_mail_node_link_visibility', PRINT_MAIL_NODE_LINK_VISIBILITY_DEFAULT),
    '#options' => array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.')),
  );

  $form['settings']['adv_link']['print_mail_node_link_pages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('print_mail_node_link_pages', PRINT_MAIL_NODE_LINK_PAGES_DEFAULT),
    '#rows' => 3,
    '#description' => t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  $access = user_access('use PHP for link visibility');

  if ($form['settings']['adv_link']['print_mail_node_link_visibility']['#default_value'] == 2 && !$access) {
    $form['settings']['adv_link']['print_mail_node_link_visibility'] = array('#type' => 'value', '#value' => 2);
    $form['settings']['adv_link']['print_mail_node_link_pages'] = array('#type' => 'value', '#value' => $form['settings']['adv_link']['print_mail_node_link_pages']['#default_value']);
  }
  elseif ($access) {
    $form['settings']['adv_link']['print_mail_node_link_visibility']['#options'][] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
    $form['settings']['adv_link']['print_mail_node_link_pages']['#description'] .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
  }

  $form['settings']['adv_link']['print_mail_sys_link_visibility'] = array(
    '#type' => 'radios',
    '#title' => t('Show link in system (non-content) pages'),
    '#default_value' => variable_get('print_mail_sys_link_visibility', PRINT_MAIL_SYS_LINK_VISIBILITY_DEFAULT),
    '#options' => array(t('Show on every page except the listed pages.'), t('Show on only the listed pages.')),
  );

  $form['settings']['adv_link']['print_mail_sys_link_pages'] = array(
    '#type' => 'textarea',
    '#default_value' => variable_get('print_mail_sys_link_pages', PRINT_MAIL_SYS_LINK_PAGES_DEFAULT),
    '#rows' => 3,
    '#description' => t('Setting this option will add a send by e-mail link on pages created by Drupal or the enabled modules.') .'<br />'.
                      t("Enter one page per line as Drupal paths. The '*' character is a wildcard. Example paths are %blog for the blog page and %blog-wildcard for every personal blog. %front is the front page.", array('%blog' => 'blog', '%blog-wildcard' => 'blog/*', '%front' => '<front>')),
  );

  if ($form['settings']['adv_link']['print_mail_sys_link_visibility']['#default_value'] == 2 && !$access) {
    $form['settings']['adv_link']['print_mail_sys_link_visibility'] = array('#type' => 'value', '#value' => 2);
    $form['settings']['adv_link']['print_mail_sys_link_pages'] = array('#type' => 'value', '#value' => $form['settings']['adv_link']['print_mail_sys_link_pages']['#default_value']);
  }
  elseif ($access) {
    $form['settings']['adv_link']['print_mail_sys_link_visibility']['#options'][] = t('Show if the following PHP code returns <code>TRUE</code> (PHP-mode, experts only).');
    $form['settings']['adv_link']['print_mail_sys_link_pages']['#description'] .= ' '. t('If the PHP-mode is chosen, enter PHP code between %php. Note that executing incorrect PHP-code can break your Drupal site.', array('%php' => '<?php ?>'));
  }

  $form['settings']['adv_link']['print_mail_book_link'] = array(
    '#type' => 'radios',
    '#title' => t('Link in book hierarchy nodes'),
    '#default_value' => variable_get('print_mail_book_link', PRINT_MAIL_BOOK_LINK_DEFAULT),
    '#options' => array(t('No link'), t('Current page and sub-pages'), t('Current page only')),
  );

  $form['settings']['print_mail_hourly_threshold'] = array(
    '#type' => 'select',
    '#title' => t('Hourly threshold'),
    '#default_value' => variable_get('print_mail_hourly_threshold', PRINT_MAIL_HOURLY_THRESHOLD),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 20, 30, 40, 50)),
    '#description' => t('The maximum number of e-mails a user can send per hour.'),
  );

  $form['settings']['print_mail_teaser_default'] = array(
    '#type' => 'checkbox',
    '#title' => t('Send only the teaser'),
    '#default_value' => variable_get('print_mail_teaser_default', PRINT_MAIL_TEASER_DEFAULT_DEFAULT),
    '#description' => t("If selected, the default choice will be to send only the node's teaser instead of the full content."),
  );

  $form['settings']['print_mail_teaser_choice'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable teaser/full mode choice'),
    '#default_value' => variable_get('print_mail_teaser_choice', PRINT_MAIL_TEASER_CHOICE_DEFAULT),
    '#description' => t('If checked, the user will be able to choose between sending the full content or only the teaser at send time.'),
  );

  $form['settings']['print_mail_send_option_default'] = array(
    '#type' => 'select',
    '#title' => t('Default e-mail sending format'),
    '#default_value' => variable_get('print_mail_send_option_default', PRINT_MAIL_SEND_OPTION_DEFAULT),
    '#options' => array(
      'sendlink' => t('Link'),
      'sendpage' => t('Inline HTML'),
    ),
  );
  if (class_exists('Mail_mime')) {
    $form['settings']['print_mail_send_option_default']['#options']['inline-attachment'] = t('Inline HTML with Attachment');
    $form['settings']['print_mail_send_option_default']['#options']['plain-attachment'] = t('Plain Text with Attachment');
  }

  if (module_exists('job_queue')) {
    $form['settings']['print_mail_job_queue'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send e-mails using Job Queue'),
      '#default_value' => variable_get('print_mail_job_queue', PRINT_MAIL_JOB_QUEUE_DEFAULT),
      '#description' => t("Selecting this option, e-mail delivery will be performed by the Job Queue module during each cron run. Leaving this unselected, the e-mail will be sent immediately, but the site will take slightly longer to reply to the user."),
    );
  }

  return system_settings_form($form);
}

/**
 * Menu callback for the send by e-mail module text strings settings form.
 *
 * @ingroup forms
 */
function print_mail_strings_settings() {
  drupal_set_message(t("Saving these strings will disable their translation via Drupal's language system. Use the reset button to return them to the original state."), 'warning', FALSE);

  $form['print_mail_text'] = array(
    '#type' => 'fieldset',
    '#title' => t('Text strings'),
  );

  $form['print_mail_text']['print_mail_link_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Link text'),
    '#default_value' => variable_get('print_mail_link_text', t('Send to friend')),
    '#description' => t('Text used in the link to the send by-email form.'),
  );

  $form['print_mail_text']['print_mail_text_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Message Subject'),
    '#default_value' => variable_get('print_mail_text_subject', t('!user has sent you a message from !site')),
    '#description' => t("e-mail subject line. The sender's name will appear in place of !user in the subject. The web site name will be inserted in place of !site. The page title replaces !title."),
  );
  $form['print_mail_text']['print_mail_text_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Message Preamble'),
    '#default_value' => variable_get('print_mail_text_message', t('Message from sender')),
    '#description' => t('e-mail message preamble. The sender will be able to add their own message after this.'),
  );
  $form['print_mail_text']['print_mail_text_content'] = array(
    '#type' => 'textfield',
    '#title' => t('Message Content'),
    '#default_value' => variable_get('print_mail_text_content', ''),
    '#description' => t('Set the default contents of the message.'),
  );
  $form['print_mail_text']['print_mail_text_confirmation'] = array(
    '#type' => 'textfield',
    '#title' => t('Thank You Message'),
    '#default_value' => variable_get('print_mail_text_confirmation', t('Thank you for spreading the word about !site.')),
    '#description' => t('This message will be displayed after the user successfully submits the form.'),
  );

  return system_settings_form($form);
}
