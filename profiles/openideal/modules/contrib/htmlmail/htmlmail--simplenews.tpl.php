<?php

/**
 * @file
 * Sample template for sending Simplenews messages with HTML Mail.
 *
 * The following variables are available in this template:
 *
 *  - $message_id: The email message id, or "simplenews_$key"
 *  - $module: The sending module, which is 'simplenews'.
 *  - $key: The simplenews action, which may be any of the following:
 *    - node: Send a newsletter to its subscribers.
 *    - subscribe: New subscriber confirmation message.
 *    - test: Send a test newsletter to the test address.
 *    - unsubscribe: Unsubscribe confirmation message.
 *  - $headers: An array of email (name => value) pairs.
 *  - $from: The configured sender address.
 *  - $to: The recipient subscriber email address.
 *  - $subject: The message subject line.
 *  - $body: The formatted message body.
 *  - $language: The language object for this message.
 *  - $params: An array containing the following keys:
 *    - context:  An array containing the following keys:
 *      - account: The recipient subscriber account object, which contains
 *        the following useful properties:
 *        - snid: The simplenews subscriber id, or NULL for test messages.
 *        - name: The subscriber username, or NULL.
 *        - activated: The date this subscription became active, or NULL.
 *        - uid: The subscriber user id, or NULL.
 *        - mail: The subscriber email address; same as $message['to'].
 *        - language: The subscriber language code.
 *        - tids: An array of taxonomy term ids.
 *        - newsletter_subscription: An array of subscription ids.
 *      - node: The simplenews newsletter node object, which contains the
 *        following useful properties:
 *        - changed: The node last-modified date, as a unix timestamp.
 *        - created: The node creation date, as a unix timestamp.
 *        - name: The username of the node publisher.
 *        - nid: The node id.
 *        - title: The node title.
 *        - uid: The user ID of the node publisher.
 *      - newsletter: The simplenews newsletter object, which contains the
 *        following useful properties:
 *        - nid: The node ID of the newsletter node.
 *        - name: The short name of the newsletter.
 *        - description: The long name or description of the newsletter.
 *  - $template_path: The relative path to the template directory.
 *  - $template_url: The absolute url to the template directory.
 *  - $theme: The name of the selected Email theme.
 *  - $theme_path: The relative path to the Email theme directory.
 *  - $theme_url: The absolute url to the Email theme directory.
 */
  $template_name = basename(__FILE__);
  $current_path = realpath(NULL);
  $current_len = strlen($current_path);
  $template_path = realpath(dirname(__FILE__));
  if (!strncmp($template_path, $current_path, $current_len)) {
    $template_path = substr($template_path, $current_len + 1);
  }
  $template_url = url($template_path, array('absolute' => TRUE));
?>
<?php if ($key == 'node' || $key == 'test'): ?>
<div class="htmlmail-simplenews-link">
  <a href="<?php echo url('node/' . $params['context']['node']->nid, array('absolute' => TRUE)); ?>">
    Click here to view this message on the web.
  </a>
</div>
<?php endif; ?>
<div class="htmlmail-simplenews-body htmlmail-body">
<?php echo $body; ?>
</div>
<?php if ($debug):
  $module_template = 'htmlmail--simplenews.tpl.php';
  $message_template = "htmlmail--simplenews--$key.tpl.php";
?>
<hr />
<div class="htmlmail-simplenews-debug htmlmail-debug">
  <dl><dt><p>
    To customize your simplenews messages:
  </p></dt><dd><ol><li><p><?php if (empty($theme)): ?>
    Visit <u>admin/config/system/htmlmail</u>
    and select a theme to hold your custom email template files.
  </p></li><li><p><?php elseif (empty($theme_path)): ?>
    Visit <u>admin/appearance</u>
    to enable your selected <u><?php echo drupal_ucfirst($theme); ?></u> theme.
  </p></li><li><?php endif;
if ("$template_path/$template_name" == "$theme_path/$message_template"): ?><p>
    Edit your<br />
    <u><code><?php echo "$template_path/$template_name"; ?></code></u>
    <br />file.
  </p></li><li><?php
else:
  if (!file_exists("$theme_path/htmlmail.tpl.php")): ?><p>
    Copy<br />
    <u><code><?php echo "$module_path/htmlmail.tpl.php"; ?></code></u>
    <br />to<br />
    <u><code><?php echo "$theme_path/htmlmail.tpl.php"; ?></code></u>
  </p></li><li><?php
  endif;
  if (!file_exists("$theme_path/$module_template")): ?><p>
    For general Simplenews message customization, copy<br />
    <u><code><?php echo "$module_path/htmlmail.tpl.php"; ?></code></u>
    <br />to<br />
    <code><?php echo "$theme_path/$module_template"; ?></code>
  </p></li><li><?php
  endif;
  if (!file_exists("$theme_path/$message_template")): ?><p>
    For message-specific customization, copy<br />
    <u><code><?php echo "$module_path/htmlmail.tpl.php"; ?></code></u>
    <br />to one of the following:
  </p><ul><li><dl><dt><p>
    <u><code>htmlmail--simplenews--node.tpl.php</code></u>
  </p></dt><dd><p>
    Regular newsletter template.
  </p></dd></dl></li><li><dl><dt><p>
    <u><code>htmlmail--simplenews--subscribe.tpl.php</code></u>
  </p></dt><dd><p>
    New subscriber confirmation message.
  </p></dd></dl></li><li><dl><dt><p>
    <u><code>htmlmail--simplenews--test.tpl.php</code></u>
  </p></dt><dd><p>
    Test newsletter.
  </p></dd></dl></li><li><dl><dt><p>
    <u><code>htmlmail--simplenews--unsubscribe.tpl.php</code></u>
  </p></dt><dd><p>
    Unsubscribe confirmation message.
  </p></dd></dl></li></ul></li><li><?php
  endif; ?><p>
    Edit the copied file.
  </p></li><li><?php
endif; ?><p>
    Send a test message to make sure your customizations worked.
  </p></li><li><p>
    If you think your customizations would be of use to others,
    please contribute your file as a feature request in the
    <a href="http://drupal.org/node/add/project-issue/htmlmail">issue queue</a>.
  </p></li></ol></dd><dt><p>
    The simplenews module sets the <u><code>$params</code></u> variable.
    For this message,
  </p></dt><dd><p><code><pre>
$params = <?php echo check_plain(print_r($params, 1)); ?>
  </pre></code></p></dd></dl>
</div>
<?php endif;
