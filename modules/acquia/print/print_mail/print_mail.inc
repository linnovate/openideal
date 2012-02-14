<?php
// $Id: print_mail.inc,v 1.2.2.32 2010/08/12 08:43:50 jcnventura Exp $

/**
 * @file
 * Displays and processes the mail send form.
 *
 * This file is included by the print_mail module and includes the
 * mail form display, validation and submit hooks.
 *
 * @ingroup print
 */

require_once(drupal_get_path('module', 'print') .'/print.pages.inc');

// Include MIME library
@include_once('Mail/mime.php');

/**
 * Menu callback for the send by e-mail form.
 *
 * @ingroup forms
 */
function print_mail_form($form_state) {
  global $user;

  $print_mail_hourly_threshold = variable_get('print_mail_hourly_threshold', PRINT_MAIL_HOURLY_THRESHOLD);

  if ((!user_access('administer print')) && (!flood_is_allowed('print_mail', $print_mail_hourly_threshold))) {
    $form['flood'] = array(
      '#type' => 'markup',
      '#value' => '<p>'. t('You cannot send more than %number messages per hour. Please try again later.', array('%number' => $print_mail_hourly_threshold)) .'</p>',
    );
    return $form;
  }

  $print_mail_teaser_default = variable_get('print_mail_teaser_default', PRINT_MAIL_TEASER_DEFAULT_DEFAULT);
  $print_mail_teaser_choice = variable_get('print_mail_teaser_choice', PRINT_MAIL_TEASER_CHOICE_DEFAULT);
  $form = array();

  // Remove the printmail/ prefix
  $path = explode('/', $_GET['q']);
  unset($path[0]);
  $path = implode('/', $path);
  if (is_numeric($path)) {
    $path = 'node/'. $path;
  }
  $cid = isset($_GET['comment']) ? (int)$_GET['comment'] : NULL;
  $title = _print_get_title($path);

  if (count($form_state['post']) == 0) {
    $nodepath = drupal_get_normal_path($path);
    db_query("UPDATE {print_mail_page_counter} SET totalcount = totalcount + 1, timestamp = %d WHERE path = '%s'", time(), $nodepath);
    // If we affected 0 rows, this is the first time viewing the node.
    if (!db_affected_rows()) {
      // We must create a new row to store counters for the new node.
      db_query("INSERT INTO {print_mail_page_counter} (path, totalcount, timestamp) VALUES ('%s', 1, %d)", $nodepath, time());
    }
  }

  $form['path'] = array('#type' => 'value', '#value' => $path);
  $form['cid'] =  array('#type' => 'value', '#value' => $cid);
  $form['title'] = array('#type' => 'value', '#value' => $title);

  $form['fld_from_addr'] = array(
    '#type' => 'textfield',
    '#title' => t('Your e-mail'),
    '#size' => 62,
    '#required' => TRUE,
  );
  $form['fld_from_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Your name'),
    '#size' => 62,
  );
  $form['txt_to_addrs'] = array(
    '#type' => 'textarea',
    '#title' => t('Send to'),
    '#rows' => 3,
    '#resizable' => FALSE,
    '#description' => t('Enter multiple addresses separated by commas and/or different lines.'),
    '#required' => TRUE,
  );
  $form['fld_subject'] = array(
    '#type' => 'textfield',
    '#title' => t('Subject'),
    '#size' => 62,
    '#required' => TRUE,
  );
  if (!empty($title)) {
    $form['fld_title'] = array(
      '#type' => 'item',
      '#title' => t('Page to be sent'),
      '#value' => l($title, $path, array('attributes' => array('title' => t('View page')))),
    );
  }
  $form['fld_send_option'] = array(
    '#type' => 'select',
    '#title' => t('Send page as'),
    '#default_value' => variable_get('print_mail_send_option_default', PRINT_MAIL_SEND_OPTION_DEFAULT),
    '#options' => array(
      'sendlink' => t('Link'),
      'sendpage' => t('Inline HTML'),
    ),
  );
  if (class_exists('Mail_mime')) {
    $form['fld_send_option']['#options']['inline-attachment'] = t('Inline HTML with Attachment');
    $form['fld_send_option']['#options']['plain-attachment'] = t('Plain Text with Attachment');
  }
  $form['txt_message'] = array(
    '#type' => 'textarea',
    '#title' => t('Your message'),
    '#rows' => 6,
    '#required' => TRUE,
  );

  if ($print_mail_teaser_choice) {
    $form['chk_teaser'] = array(
      '#type' => 'checkbox',
      '#title' => t('Send only the teaser'),
      '#default_value' => $print_mail_teaser_default,
    );
  }
  else {
    $form['chk_teaser'] = array('#type' => 'value', '#value' => $print_mail_teaser_default);
  }

  $form['btn_submit'] = array(
    '#name' => 'submit',
    '#type' => 'submit',
    '#value' => t('Send e-mail'),
  );
  $form['btn_clear'] = array(
    '#type' => 'markup',
    '#value' => '<input type="reset" name="clear" value="'. t('Clear form') .'" class="form-submit" /> ',
  );
  $form['btn_cancel'] = array(
    '#name' => 'cancel',
    '#type' => 'submit',
    '#value' => t('Cancel'),
  );

  if ($user->uid != 0) {
    $user_name = check_plain(strip_tags(theme('username', $user)));
    $form['fld_from_addr']['#default_value'] = $user->mail;
    $form['fld_from_addr']['#disabled'] = TRUE;
    $form['fld_from_addr']['#value'] = $user->mail;
    $form['fld_from_name']['#default_value'] = $user_name;
  }
  else {
    $user_name = t('Someone');
  }
  $site_name = variable_get('site_name', t('an interesting site'));
  $print_mail_text_subject = filter_xss(variable_get('print_mail_text_subject', t('!user has sent you a message from !site')));
  $form['fld_subject']['#default_value'] = t($print_mail_text_subject, array('!user' => $user_name, '!site' => $site_name, '!title' => $title));
  $print_mail_text_content = filter_xss(variable_get('print_mail_text_content', ''));
  $form['txt_message']['#default_value'] = t($print_mail_text_content);

  return $form;
}

/**
 * Theme function for the send by-email form submission.
 *
 * Adds a class to the form labels. This class is used to place the label on
 * the left of the input fields.
 *
 * @ingroup forms
 */
function theme_print_mail_form($form) {
  drupal_add_css(drupal_get_path('module', 'print') .'/css/printlinks.css');
  $content = '';
  foreach (element_children($form) as $key) {
    $tmp = drupal_render($form[$key]);
    switch ($key) {
      case 'fld_from_addr':
      case 'fld_from_name':
      case 'txt_to_addrs':
      case 'fld_subject':
      case 'fld_title':
      case 'fld_send_option':
        $tmp = str_replace('<label', '<label class ="printmail-label"', $tmp);
        break;
    }
    $content .= $tmp;
  }
  return $content;
}

/**
 * Validate the send by-email form submission.
 *
 * @ingroup forms
 */
function print_mail_form_validate($form, &$form_state) {
  if (array_key_exists('cancel', $form['#post'])) {
    return;
  }
  $from_addr = trim($form_state['values']['fld_from_addr']);
  $test = user_validate_mail($from_addr);
  if ($test) {
    form_set_error('fld_from_addr', $test);
  }

  // All new-lines are replaced by commas
  $to_addrs = preg_replace('![\r|\n|,]+!', ',', trim($form_state['values']['txt_to_addrs']));
  // Create an array from the string
  $to_array = explode(',', $to_addrs);
  // Verify each element of the array
  foreach ($to_array as $key => $address) {
    $address = trim($address);
    if (preg_match('/(.*?) <(.*)>/s', $address, $matches)) {
      // Address is of the type User Name <user@domain.tld>
      $test = user_validate_mail($matches[2]);
      $to_array[$key] = trim($matches[1]) .' <'. $matches[2] .'>';
    }
    else {
      // Address must be user@domain.tld
      $test = user_validate_mail($address);
    }
    if ($test) {
      form_set_error('txt_to_addrs', $test);
    }
  }

  $print_mail_hourly_threshold = variable_get('print_mail_hourly_threshold', PRINT_MAIL_HOURLY_THRESHOLD);

  if ((!user_access('administer print')) && (!flood_is_allowed('print_mail', $print_mail_hourly_threshold - count($to_array) + 1))) {
    form_set_error('txt_to_addrs',  t('You cannot send more than %number messages per hour. Please reduce the number of recipients.', array('%number' => $print_mail_hourly_threshold)));
  }

  // In all fields, prevent insertion of custom headers
  foreach ($form_state['values'] as $key => $string) {
    if ( (substr($key, 0, 4) == 'fld_') && ((strpos($string, "\n") !== FALSE) || (strpos($string, "\r") !== FALSE)) ) {
      form_set_error($key, 'Found invalid character');
    }
  }

  $form_state['values']['fld_from_addr'] = $from_addr;
  $form_state['values']['fld_from_name'] = trim($form_state['values']['fld_from_name']);
  // Re-create the string from the re-organized array
  $form_state['values']['txt_to_addrs'] = implode(', ', $to_array);
}

/**
 * Process the send by-email form submission.
 *
 * @ingroup forms
 */
function print_mail_form_submit($form, &$form_state) {
  if (!array_key_exists('cancel', $form_state['values'])) {
    if (!empty($form_state['values']['fld_from_name'])) {
      $from = '"'. $form_state['values']['fld_from_name'] .'" <'. $form_state['values']['fld_from_addr'] .'>';
    }
    else {
      $from = $form_state['values']['fld_from_addr'];
    }
    $cid = isset($form_state['values']['cid']) ? $form_state['values']['cid'] : NULL;

    $print_mail_text_message = filter_xss_admin(variable_get('print_mail_text_message', t('Message from sender')));
    $sender_message = $print_mail_text_message .':<br /><br /><em>'. nl2br(check_plain($form_state['values']['txt_message'])) .'</em>';

    $print = print_controller($form_state['values']['path'], $cid, PRINT_MAIL_FORMAT, $form_state['values']['chk_teaser'], $sender_message);

    // Spaces in img URLs must be replaced with %20
    $pattern = '!<(img\s[^>]*?)>!is';
    $print['content'] = preg_replace_callback($pattern, '_print_mail_encode_urls', $print['content']);

    if ($print !== FALSE) {
      $params = array();
      $params['subject'] = $form_state['values']['fld_subject'];
      $params['message'] = $form_state['values']['txt_message'];
      $params['link'] = $print['url'];
      $params['title'] = $form_state['values']['title'];

      $node = $print['node'];
      $params['body'] = theme('print_page', $print, PRINT_MAIL_FORMAT, $node);
      $params['body'] = drupal_final_markup($params['body']);

      $ok = FALSE;
      if (function_exists('job_queue_add') && variable_get('print_mail_job_queue', PRINT_MAIL_JOB_QUEUE_DEFAULT)) {
        $use_job_queue = TRUE;
        $this_file = drupal_get_path('module', 'print_mail') .'/print_mail.inc';
      }
      else {
        $use_job_queue = FALSE;
      }

      $addresses = explode(', ', $form_state['values']['txt_to_addrs']);
      foreach ($addresses as $to) {
        // Call to hook_print_mail_before_send in order to know if the mail can be sent
        // Handlers must return TRUE or FALSE
        $can_send = module_invoke_all('print_mail_before_send', $node, $to, $from, $params);

        if (!in_array(FALSE, $can_send)) {
          if ($use_job_queue) {
            // Use job queue to send mails during cron runs
            job_queue_add('drupal_mail', t('print_mail: From %from', array('%from' => $from)), array('print_mail', $form_state['values']['fld_send_option'], $to, language_default(), $params, $from, TRUE), $this_file, TRUE);
          }
          else {
            // Send mail immediately using Drupal's mail handler
            $ret = drupal_mail('print_mail', $form_state['values']['fld_send_option'], $to, language_default(), $params, $from, TRUE);
          }
          if ($ret['result'] || $use_job_queue) {
            // Call to hook_print_mail_after_send in order to provide information to other modules.
            module_invoke_all('print_mail_after_send', $node, $to, $from, $params);

            flood_register_event('print_mail');
            $ok = TRUE;
          }
        }
      }
      if ($ok) {
        watchdog('print_mail', '%name [%from] sent %page to [%to]', array('%name' => $form_state['values']['fld_from_name'], '%from' => $form_state['values']['fld_from_addr'], '%page' => $form_state['values']['path'], '%to' => $form_state['values']['txt_to_addrs']));
        $site_name = variable_get('site_name', t('us'));
        $print_mail_text_confirmation = variable_get('print_mail_text_confirmation', t('Thank you for spreading the word about !site.'));
        drupal_set_message(check_plain(t($print_mail_text_confirmation, array('!site' => $site_name))));

        $nodepath = drupal_get_normal_path($form_state['values']['path']);
        db_query("UPDATE {print_mail_page_counter} SET sentcount = sentcount + %d, sent_timestamp = %d WHERE path = '%s'", count($addresses), time(), $nodepath);
      }
    }
  }

  $form_state['redirect'] = preg_replace('!^book/export/html/!', 'node/', $form_state['values']['path']);
}

/**
 * Implementation of hook_mail().
 */
function print_mail_mail($key, &$message, $params) {
  $message['subject'] = $params['subject'];

  switch ($key) {
    case 'sendpage':
      $message['body'] = $params['body'];
      $message['headers']['Content-Type'] = 'text/html; charset=utf-8';
      break;
    case 'sendlink':
      // Generate plain-text and html versions of message with link
      $sendlink_plain = $params['message'] .'\n\n'. $params['link'];
      $sendlink_html = $params['message'] .'<br/><br/>'. l($params['title'], $params['link']);

      // Send HTML-only version if MIME library not present
      if (!class_exists('Mail_mime')) {
        $message['body'] = $sendlink_html;
        $message['headers']['Content-Type'] = 'text/html; charset=utf-8';
        break;
      }
      // no break on purpose
    case 'plain-attachment':
    case 'inline-attachment':
      // Configure new MIME object
      $mime = new Mail_mime("\n");
      $mime_params['html_encoding'] = '7bit';

      // Pass message contents into MIME object
      switch ($key) {
        case 'sendlink':
          $mime->setTXTBody($sendlink_plain);
          $mime->setHTMLBody($sendlink_html);
          break;
        case 'inline-attachment':
          $mime->setHTMLBody($params['body']);
          // no break on purpose
        case 'plain-attachment':
          $mime->setTXTBody($params['message']);
          $mime->addAttachment($params['body'], 'text/html', 'Attachment.html', FALSE);
          break;
      }

      // Store MIME message output in message array
      $message['body'] = $mime->get($mime_params);
      $message['headers'] = $mime->headers($message['headers']);

      // Strip special characters from Content-Type header
      // Required to prevent mime_header_encode() from disrupting Content-Type header
      $message['headers']['Content-Type'] = preg_replace('/[^\x20-\x7E]/', '', $message['headers']['Content-Type']);
      break;
  }
}

/**
 * Process the send by-email form cancel submission.
 *
 * @ingroup forms
 */
function print_mail_form_cancel($form, &$form_state) {
  $form_state['redirect'] = preg_replace('!^book/export/html/!', 'node/', $form_state['values']['path']);
}

/**
 * Callback function for the preg_replace_callback replacing spaces with %20
 *
 * Replace spaces in URLs with %20
 *
 * @param $matches
 *   array with the matched tag patterns, usually <a...>+text+</a>
 * @return
 *   tag with re-written URL
 */
function _print_mail_encode_urls($matches) {
  // first, split the html into the different tag attributes
  $pattern = '!\s*(\w+\s*=\s*"(?:\\\"|[^"])*")\s*|\s*(\w+\s*=\s*\'(?:\\\\\'|[^\'])*\')\s*|\s*(\w+\s*=\s*\w+)\s*|\s+!';
  $attribs = preg_split($pattern, $matches[1], -1, PREG_SPLIT_NO_EMPTY | PREG_SPLIT_DELIM_CAPTURE);
  foreach ($attribs as $key => $value) {
    $attribs[$key] = preg_replace('!(\w)\s*=\s*(.*)!', '$1=$2', $value);
  }

  $size = count($attribs);
  for ($i=1; $i < $size; $i++) {
    // If the attribute is href or src, we may need to rewrite the URL in the value
    if (preg_match('!^(?:href|src)\s*?=(.*)!i', $attribs[$i], $urls) > 0) {
      $url = trim($urls[1], " \t\n\r\0\x0B\"'");
      $new_url = str_replace(' ', '%20', $url);
      $matches[1] = str_replace($url, $new_url, $matches[1]);
    }
  }

  $ret = '<'. $matches[1] .'>';
  if (count($matches) == 4) {
    $ret .= $matches[2] . $matches[3];
  }

  return $ret;
}
