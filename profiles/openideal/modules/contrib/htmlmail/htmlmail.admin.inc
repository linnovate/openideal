<?php

/**
 * @file
 * Admin forms for HTML Mail
 */

/**
 * Implements hook_admin_settings().
 */
function htmlmail_admin_settings() {
  $formats = array('0' => t('Unfiltered'));
  foreach (filter_formats() as $id => $filter) {
    $formats[$id] = $filter->name;
  }
  $form['template'] = array(
    '#type' => 'fieldset',
    '#title' => '<h3>' . t('Step 1') . '</h3>',
    '#collapsible' => FALSE,
  );
  $form['template']['htmlmail_template'] = array(
    '#type' => 'fieldset',
    '#prefix' => '<strong>' . t('Template file') . ':</strong><br />'
    . t('A template file is applied to your message header, subject, and body text.  You may copy the <a href="!uri">!template</a> file to your default theme directory and use it to customize your messages.',
      array(
        '!uri' => url('http://drupalcode.org/project/htmlmail.git/blob_plain/refs/heads/7.x-2.x:/htmlmail.tpl.php'),
        '!template' => '<code>htmlmail.tpl.php</code>'
      )
    ),
    '#title' => t('Instructions'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['template']['htmlmail_template']['instructions'] = array(
    '#type' => 'item',
    '#suffix' => t('!Instructions
<p>When formatting an email message with a given <code>$module</code> and <code>$key</code>, <a href="http://drupal.org/project/htmlmail">HTML Mail</a> will use the first template file it finds from the following list:</p>
<ol style="list-style-type: decimal;">
  <li><code>htmlmail--$module--$key.tpl.php</code></li>
  <li><code>htmlmail--$module.tpl.php</code></li>
  <li><code>htmlmail.tpl.php</code></li>
</ol>
<p>For each filename, <a href="http://drupal.org/project/htmlmail">HTML Mail</a> looks first in the chosen <em>Email theme</em> directory, then in its own module directory, before proceeding to the next filename.</p>
<p>For example, if <code>example_module</code> sends mail with:</p>
<pre>
<code>drupal_mail("example_module", "outgoing_message" ...)
</code>
</pre>
<p>the possible template file names would be:</p>
<ol style="list-style-type: decimal;">
  <li><code>htmlmail--example_module--outgoing_message.tpl.php</code></li>
  <li><code>htmlmail--example_module.tpl.php</code></li>
  <li><code>htmlmail.tpl.php</code></li>
</ol>
<p>Template files are cached, so remember to clear the cache by visiting <u>admin/config/development/performance</u> after changing any <code>.tpl.php</code> files.</p>
<p>The following variables available in this template:</p>
<dl>
  <dt><strong><code>$body</code></strong></dt>
  <dd>
    <p>The message body text.</p>
  </dd>
  <dt><strong><code>$module</code></strong></dt>
  <dd>
    <p>The first argument to <a href="http://api.drupal.org/api/drupal/includes--mail.inc/function/drupal_mail/7"><code>drupal_mail()</code></a>, which is, by convention, the machine-readable name of the sending module.</p>
  </dd>
  <dt><strong><code>$key</code></strong></dt>
  <dd>
    <p>The second argument to <a href="http://api.drupal.org/api/drupal/includes--mail.inc/function/drupal_mail/7"><code>drupal_mail()</code></a>, which should give some indication of why this email is being sent.</p>
  </dd>
  <dt><strong><code>$message_id</code></strong></dt>
  <dd>
    <p>The email message id, which should be equal to <code>"{$module}_{$key}"</code>.</p>
  </dd>
  <dt><strong><code>$headers</code></strong></dt>
  <dd>
    <p>An array of email <code>(name =&gt; value)</code> pairs.</p>
  </dd>
  <dt><strong><code>$from</code></strong></dt>
  <dd>
    <p>The configured sender address.</p>
  </dd>
  <dt><strong><code>$to</code></strong></dt>
  <dd>
    <p>The recipient email address.</p>
  </dd>
  <dt><strong><code>$subject</code></strong></dt>
  <dd>
    <p>The message subject line.</p>
  </dd>
  <dt><strong><code>$body</code></strong></dt>
  <dd>
    <p>The formatted message body.</p>
  </dd>
  <dt><strong><code>$language</code></strong></dt>
  <dd>
    <p>The language object for this message.</p>
  </dd>
  <dt><strong><code>$params</code></strong></dt>
  <dd>
    <p>Any module-specific parameters.</p>
  </dd>
  <dt><strong><code>$template_name</code></strong></dt>
  <dd>
    <p>The basename of the active template.</p>
  </dd>
  <dt><strong><code>$template_path</code></strong></dt>
  <dd>
    <p>The relative path to the template directory.</p>
  </dd>
  <dt><strong><code>$template_url</code></strong></dt>
  <dd>
    <p>The absolute URL to the template directory.</p>
  </dd>
  <dt><strong><code>$theme</code></strong></dt>
  <dd>
    <p>The name of the <em>Email theme</em> used to hold template files. If the <a href="http://drupal.org/project/echo">Echo</a> module is enabled this theme will also be used to transform the message body into a fully-themed webpage.</p>
  </dd>
  <dt><strong><code>$theme_path</code></strong></dt>
  <dd>
    <p>The relative path to the selected <em>Email theme</em> directory.</p>
  </dd>
  <dt><strong><code>$theme_url</code></strong></dt>
  <dd>
    <p>The absolute URL to the selected <em>Email theme</em> directory.</p>
  </dd>
  <dt><strong><code>$debug</code></strong></dt>
  <dd>
    <p><code>TRUE</code> to add some useful debugging info to the bottom of the message.</p>
  </dd>
</dl>
<p>Other modules may also add or modify theme variables by implementing a <code>MODULENAME_preprocess_htmlmail(&amp;$variables)</code> <a href="http://api.drupal.org/api/drupal/modules--system--theme.api.php/function/hook_preprocess_HOOK/7">hook function</a>.</p>',
      array('!Instructions' => '')
    ),
  );
  $form['template']['htmlmail_debug'] = array(
    '#type' => 'checkbox',
    '#prefix' => '<br />',
    '#title' => '<em>' . t('(Optional)') . '</em> ' . t('Debug'),
    '#default_value' => variable_get('htmlmail_debug', '0'),
    '#description' => t('Add debugging info (Set <code>$debug</code> to <code>TRUE</code>).'),
  );
  $form['theme'] = array(
    '#type' => 'fieldset',
    '#title' => '<h3>' . t('Step 2') . '</h3>',
    '#collapsible' => FALSE,
  );
  $form['theme']['htmlmail_theme'] = array(
    '#type' => 'select',
    '#title' => t('Email theme'),
    '#default_value' => variable_get('htmlmail_theme', ''),
    '#options' => htmlmail_get_allowed_themes(),
    '#suffix' => '<p>'
    . t('Choose the theme that will hold your customized templates from Step 1 above.')
    . '</p><p>'
    . (module_exists('echo') ?
      t('The templated text will be styled by your chosen theme.  This lets you use any one of <a href="!themes">over 800</a> themes to style your messages.  Creating an email-specific sub-theme lets you use the full power of the <a href="!theme_system">drupal theme system</a> to format your messages.',
        array(
          '!themes' => 'http://drupal.org/project/themes',
          '!theme_system' => 'http://drupal.org/documentation/theme',
        )
      ) :
      t('If you install and enable the <a href="!echo">Echo</a> module, the theme you select will also be used to style your messages as if they were pages on your website.')
    )
    . '</p><p>'
    . (module_exists('mailmime') ?
      t('Since you have the <a href="!mailmime">Mail MIME</a> module installed, your images will be automatically converted to inline attachments, and a plain-text alternative will be available to recipients who prefer it.',
        array('!mailmime' => 'http://drupal.org/project/mailmime')
      ) :
      t('If you install the <a href="!mailmime">Mail MIME</a> module, images in your emails will be automatically converted to inline attachments, and a plain-text alternative will be made available. This prevents your recipients from seeing broken image links and scary security warnings when they don\'t have the sender\'s address in their email addressbook.  <a href="!mailmime">Mail MIME</a> also allows <a href="!htmlmail">HTML Mail</a> to handle MIME-formatted messages sent by other modules such as <a href="!print">Send by-email</a>.',
        array(
          '!mailmime' => 'http://drupal.org/project/mailmime',
          '!print' => 'http://drupal.org/project/print',
        )
      )
    )
    . '</p>',
  );
  $form['filter'] = array(
    '#type' => 'fieldset',
    '#title' => '<h3>' . t('Step 3') . '</h3>',
    '#collapsible' => FALSE,
  );
  $form['filter']['htmlmail_postfilter'] = array(
    '#type' => 'select',
    '#title' => t('Post-filtering'),
    '#default_value' => variable_get('htmlmail_postfilter', ''),
    '#options' => $formats,
    '#suffix' => '<p>'
    . t('You may choose a <a href="!formats">text format</a> to be used for filtering email messages <em>after</em> theming.  This allows you to use any combination of <a href="!filters">over 200 filter modules</a> to make final changes to your message before sending.',
      array(
        '!formats' => url('admin/config/content/formats'),
        '!filters' => url('http://drupal.org/project/modules/?filters=type%3Aproject_project%20tid%3A63%20hash%3A1hbejm%20-bs_project_sandbox%3A1%20bs_project_has_releases%3A1'),
      )
    )
    . '</p><p>'
    . t('Here is a recommended configuration:')
    . '</p><ul><li><dl><dt>'
    . t('<a href="!emogrifier">Emogrifier</a>',
      array('!emogrifier' => url('http://drupal.org/project/emogrifier'))
    )
    . '</dt><dd>'
    . t('Converts stylesheets to inline style rules for consistent display on mobile devices and webmail.')
    . '</dd></dl></li><li><dl><dt>'
    . t('<a href="!transliteration">Transliteration</a>',
      array('!transliteration' => url('http;//drupal.org/project/filter_transliteration'))
    )
    . '</dt><dd>'
    . t('Converts non-ASCII text to US-ASCII equivalents. This helps prevent Microsoft <q>smart-quotes</q> from appearing as question-marks in Mozilla Thunderbird.'
    )
    . '</dd></dl></li><li><dl><dt>'
    . t('<a href="!pathologic">Pathologic</a>',
      array('!pathologic' => url('http://drupal.org/project/pathologic'))
    )
    . '</dt><dd>'
    . t('Converts relative URLS to absolute URLS so that clickable links in your message will work as intended.')
    . '</dd></dl></ul>'
  );
  return system_settings_form($form);
}

/**
 * Builds a form for sending a test message.
 */
function htmlmail_test_form($form_values = NULL) {
  $defaults = variable_get(
    'htmlmail_test',
    array(
      'to' => variable_get('site_mail', 'user@example.com'),
      'subject' => 'test',
      'body' => array(
        'value' => 'test',
      ),
    )
  );
  $defaults['body']['format'] = filter_fallback_format();
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
    '#type' => 'text_format',
    '#title' => t('Body'),
    '#rows' => 20,
    '#default_value' => $defaults['body']['value'],
    '#format' => $defaults['body']['format'],
    '#required' => TRUE,
  );
  $mailsystem = mailsystem_get();
  if (empty($mailsystem['htmlmail'])) {
    $mailsystem['htmlmail'] = 'HTMLMailSystem';
  }
  $form['class'] = array(
    '#type' => 'select',
    '#title' => t('Test mail sending class'),
    '#default_value' => $mailsystem['htmlmail'],
    '#options' => array_combine(mailsystem_get_classes(), mailsystem_get_classes()),
    '#description' => 'Select the MailSystemInterface implementation to be tested.',
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Send test message'),
  );
  return $form;
}

/**
 * Sends the test messsage and saves the contents for re-use.
 */
function htmlmail_test_form_submit($form, &$form_state) {
  // Get the form values.
  $defaults = array(
    'to' => $form_state['values']['to'],
    'subject' => $form_state['values']['subject'],
    'body' => $form_state['values']['body'],
  );
  // Set the defaults for reuse.
  variable_set('htmlmail_test', $defaults);
  // Set the mail sending class.
  mailsystem_set(array('htmlmail' => $form_state['values']['class']));
  // Send the email.
  $params = array(
    'subject' => $form_state['values']['subject'],
    'body' => check_markup(
      $form_state['values']['body']['value'],
      $form_state['values']['body']['format']
    ),
  );
  if (
    drupal_mail(
      'htmlmail',
      'test',
      $form_state['values']['to'],
      language_default(),
      $params
    )
  ) {
    drupal_set_message(t('HTML Mail test message sent.'));
  }
}
