<?php
/**
 * @file
 * Formats and sends mail using the MailMIME class.
 *
 * @see http://drupal.org/node/900794
 * @see http://api.drupal.org/api/drupal/includes--mail.inc/interface/MailSystemInterface/7
 * @see http://drupal.org/project/mailmime
 */

/**
 * Implements MailSystemInterface.
 */
class HTMLMailSystem implements MailSystemInterface {
  /**
   * Format emails according to module settings.
   *
   * Parses the message headers and body into a MailMIME object.  If another module
   * subsequently modifies the body, then format() should be called again before
   * sending.  This is safe because the $message['body'] is not modified.
   *
   * @param $message
   *   An associative array with at least the following parts:
   *   - headers: An array of (name => value) email headers.
   *   - body: The text/plain or text/html message part.
   *
   * @return
   *   The formatted $message, ready for sending.
   */
  public function format(array $message) {
    $eol = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
    // @todo Remove this when issue #209672 gets resolved.
    $default_from = variable_get('site_mail', ini_get('sendmail_from'));
    if ( !empty($message['headers']['From'])
      && $message['headers']['From'] == $default_from
      && valid_email_address($default_from)
    ) {
      $message['headers']['From'] = '"'
      . str_replace('"', '', variable_get('site_name', 'Drupal'))
        . '" <' . $default_from . '>';
    }
    // Collapse the message body array.
    if (module_exists('mailmime')) {
      $body = $this->formatMailMIME($message);
      $plain = $message['MailMIME']->getTXTBody();
    }
    else {
      if (is_array($message['body'])) {
        $message['body'] = implode("$eol$eol", $message['body']);
      }
      $body = theme('htmlmail', $message);
      if ($message['body'] && !$body) {
        watchdog(
          'htmlmail',
          'The %theme function did not return any text.  Please check your template file for errors.',
          array('%theme' => "theme('htmlmail', \$message)"),
          WATCHDOG_WARNING
        );
        $body = $message['body'];
      }
      // @todo Change to drupal_html_to_text when issue #299138 gets resolved.
      $plain = mailsystem_html_to_text($body);
      if ($body && !$plain) {
        watchdog(
          'htmlmail',
          'The %convert function did not return any text. Please report this error to the %mailsystem issue queue.',
          array('%convert' => 'mailsystem_html_to_text()', '%mailsystem' => 'Mail system'),
          WATCHDOG_WARNING,
          'http://drupal.org/node/add/project-issue/mailsystem'
        );
      }
    }
    // Check to see whether recipient allows non-plaintext.
    if ($body && htmlmail_is_allowed($message['to'])) {
      // Optionally apply the selected web theme.
      if (module_exists('echo') && $theme = htmlmail_get_selected_theme($message)) {
        $themed_body = echo_themed_page($message['subject'], $body, $theme);
        if ($themed_body) {
          $body = $themed_body;
        }
        else {
          watchdog(
            'htmlmail',
            'The %echo function did not return any text. Please check the page template of your %theme theme for errors.',
            array('%echo' => 'echo_themed_page()', '%theme' => $theme),
            WATCHDOG_WARNING
          );
        }
      }
      // Optionally apply the selected output filter.
      if ($filter = variable_get('htmlmail_postfilter')) {
        $filtered_body = check_markup($body, $filter);
        if ($filtered_body) {
          $body = $filtered_body;
        }
        else {
          watchdog(
            'htmlmail',
            'The %check function did not return any text. Please check your %filter output filter for errors.',
            array('%check' => 'check_markup()', '%filter' => $filter),
            WATCHDOG_WARNING
          );
        }
      }
      // Store the fully-themed HTML body.
      if (isset($message['MailMIME'])) {
        $mime = &$message['MailMIME'];
        $mime->setHTMLBody($body);
        list($message['headers'], $message['body']) = $mime->toEmail($message['headers']);
        if (!$message['body']) {
          watchdog(
            'htmlmail',
            'The %toemail function did not return any text. Please report this errot to the %mailmime issue queue.',
            array('%toemail' => 'MailMIME::toEmail()', '%mailmime' => 'Mail MIME'),
            WATCHDOG_WARNING,
            'http://drupal.org/node/add/project-issue/mailmime'
          );
        }
      }
      else {
        $message['headers']['Content-Type'] = 'text/html; charset=utf-8';
        $message['body'] = $body;
      }
    }
    else {
      if (isset($message['MailMIME'])) {
        $mime = &$message['MailMIME'];
        $mime->setHTMLBody('');
        $mime->setContentType('text/plain', array('charset' => 'utf-8'));
        list($message['headers'], $message['body']) = $mime->toEmail($message['headers']);
        if (!$message['body']) {
          watchdog(
            'htmlmail',
            'The %toemail function did not return any text. Please report this errot to the %mailmime issue queue.',
            array('%toemail' => 'MailMIME::toEmail()', '%mailmime' => 'Mail MIME'),
            WATCHDOG_WARNING,
            'http://drupal.org/node/add/project-issue/mailmime'
          );
        }
      }
      else {
        $message['body'] = $plain;
        $message['headers']['Content-Type'] = 'text/plain; charset=utf-8';
      }
    }
    return $message;
  }

  /**
   * Use the MailMime class to format the message body.
   *
   * @see http://drupal.org/project/mailmime
   */
  public function formatMailMIME(array &$message) {
    $eol = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
    $message['body'] = MailMIME::concat($message['body']);
    // Build a full email message string.
    $email = MailMIME::encodeEmail($message['headers'], $message['body']);
    // Parse it into MIME parts.
    if (!($mime = MailMIME::parse($email))) {
      watchdog(
        'HTMLMailSystem',
        'Could not parse email message.',
        array(),
        WATCHDOG_ERROR
      );
      return $message;
    }
    // Work on a copy so that the original $message['body'] remains unchanged.
    $email = $message;
    if ( !($email['body'] = $mime->getHTMLBody())
      && !($email['body'] = $mime->getTXTBody())
    ) {
      $email['body'] = '';
    }
    else {
      // Wrap formatted plaintext in <pre> tags.
      if ( $email['body'] === strip_tags($email['body']) // No html tags.
        && preg_match('/.' . $eol . './', $email['body']) // At least one embedded newline.
      ) {
        $email['body'] = '<pre>' . $email['body'] . '</pre>';
      }
    }
    // Theme with htmlmail.tpl.php.
    $body = theme('htmlmail', $email);
    $mime->setHTMLBody($body);
    // @todo Change to drupal_html_to_text when issue #299138 gets resolved.
    $mime->setTXTBody(mailsystem_html_to_text($body));
    $message['MailMIME'] = &$mime;
    return $body;
  }

  /**
   * Send an email message.
   *
   * @param $message
   *   An associative array containing at least:
   *   - headers: An associative array of (name => value) email headers.
   *   - body: The text/plain or text/html message body.
   *   - MailMIME: The message, parsed into a MailMIME object.
   */
  public function mail(array $message) {
    $eol = variable_get('mail_line_endings', MAIL_LINE_ENDINGS);
    // Ensure that subject is non-null.
    $message += array('subject' => t('(No subject)'));
    // Check for empty recipient.
    if (empty($message['to'])) {
      if (empty($message['headers']['To'])) {
        watchdog(
          'HTMLMailSystem',
          'Cannot send email about %subject without a recipient.',
          array('subject' => $message['subject']),
          WATCHDOG_ERROR
        );
        return FALSE;
      }
      $message['to'] = $message['headers']['To'];
    }
    if (class_exists('MailMIME')) {
      $mime = new MailMIME();
      $to = $mime->encodeHeader('to', $message['to']);
      $subject = $mime->encodeHeader('subject', $message['subject']);
      $txt_headers = $mime->txtHeaders($message['headers']);
    }
    else {
      $to = mime_header_encode($message['to']);
      $subject = mime_header_encode($message['subject']);
      $txt_headers = $this->txtHeaders($message['headers']);
    }
    $body = preg_replace('#(\r\n|\r|\n)#s', $eol, $message['body']);
    // Check for empty body.
    if (empty($body)) {
      watchdog(
        'HTMLMailSystem',
        'Refusing to send a blank email to %recipient about %subject.',
        array('%recipient' => $message['to'], '%subject' => $message['subject']),
        WATCHDOG_WARNING
      );
      return FALSE;
    }
    if (variable_get('htmlmail_debug', 0)) {
      $params = array(
        $to,
        $subject,
        drupal_substr($body, 0, min(80, strpos("\n", $body))) . '...',
        $txt_headers
      );
    }
    if (isset($message['headers']['Return-Path'])) {
      // A return-path was set.
      if (isset($_SERVER['WINDIR']) || strpos($_SERVER['SERVER_SOFTWARE'], 'Win32') !== FALSE) {
        // On Windows, PHP will use the value of sendmail_from for the
        // Return-Path header.
        $old_from = ini_get('sendmail_from');
        ini_set('sendmail_from', $message['headers']['Return-Path']);
        $result = @mail($to, $subject, $body, $txt_headers);
        ini_set('sendmail_from', $old_from);
      }
      elseif (ini_get('safe_mode')) {
        // If safe mode is in effect, passing the fifth parameter to @mail
        // will cause it to return FALSE and generate a PHP warning, even
        // if the parameter is NULL.
        $result = @mail($to, $subject, $body, $txt_headers);
      }
      else {
        // On most non-Windows systems, the "-f" option to the sendmail command
        // is used to set the Return-Path.
        $extra = '-f' . $message['headers']['Return-Path'];
        $result = @mail($to, $subject, $body, $txt_headers, $extra);
        if (variable_get('htmlmail_debug', 0)) {
          $params[] = $extra;
        }
      }
    }
    else {
      // No return-path was set.
      $result = @mail($to, $subject, $body, $txt_headers);
    }
    if (!$result && variable_get('htmlmail_debug', 0)) {
      $call = '@mail(' . implode(', ', $params) . ')';
      foreach ($params as $i => $value) {
        $params[$i] = var_export($value, 1);
      }
      if (defined('DEBUG_BACKTRACE_IGNORE_ARGS')) {
        $trace = print_r(debug_backtrace(DEBUG_BACKTRACE_IGNORE_ARGS), 1);
      }
      else {
        $trace = debug_backtrace(0);
        for ($i = count($trace) - 1; $i >= 0; $i--) {
          unset($trace[$i]['args']);
        }
        $trace = print_r($trace);
      }
      watchdog('htmlmail', 'Mail sending failed because:<br /><pre>@call</pre><br />returned FALSE.<br /><pre>@trace</pre>', array('@call' => $call, '@trace' => $trace));
    }
    return $result;
  }

  /**
   * Converts an array of email headers to a text string.
   *
   * @param $headers
   *   An associative array of ('HeaderName' => 'header value') pairs.
   *
   * @return
   *   The concatenated headers as a single string.
   */
  public function txtHeaders(array $headers) {
    $output = array();
    foreach ($headers as $name => $value) {
      if (is_array($value)) {
        foreach ($value as $val) {
          $output[] = "$name: $val";
        }
      }
      else {
        $output[] = "$name: $value";
      }
    }
    return implode("\n", $output);
  }
}
