When formatting an email message with a given `$module` and `$key`,
[HTML Mail](http://drupal.org/project/htmlmail)
will use the first template file it finds from the following list:

1.  `htmlmail--$module--$key.tpl.php`
2.  `htmlmail--$module.tpl.php`
3.  `htmlmail.tpl.php`

For each filename,
[HTML Mail](http://drupal.org/project/htmlmail)
looks first in the chosen *Email theme* directory, then in its own
module directory, before proceeding to the next filename.

For example, if `example_module` sends mail with:

    drupal_mail("example_module", "outgoing_message" ...)

the possible template file names would be:

1.  `htmlmail--example_module--outgoing_message.tpl.php`
2.  `htmlmail--example_module.tpl.php`
3.  `htmlmail.tpl.php`

Template files are cached, so remember to clear the cache by visiting
<u>admin/config/development/performance</u>
after changing any `.tpl.php` files.

The following variables available in this template:

**`$body`**
:   The message body text.

**`$module`**
:   The first argument to
    [`drupal_mail()`](http://api.drupal.org/api/drupal/includes--mail.inc/function/drupal_mail/7),
    which is, by convention, the machine-readable name of the sending module.

**`$key`**
:   The second argument to
    [`drupal_mail()`](http://api.drupal.org/api/drupal/includes--mail.inc/function/drupal_mail/7),
    which should give some indication of why this email is being sent.

**`$message_id`**
:   The email message id, which should be equal to `"{$module}_{$key}"`.

**`$headers`**
:   An array of email `(name => value)` pairs.

**`$from`**
:   The configured sender address.

**`$to`**
:   The recipient email address.

**`$subject`**
:   The message subject line.

**`$body`**
:   The formatted message body.

**`$language`**
:   The language object for this message.

**`$params`**
:   Any module-specific parameters.

**`$template_name`**
:   The basename of the active template.

**`$template_path`**
:   The relative path to the template directory.

**`$template_url`**
:   The absolute URL to the template directory.

**`$theme`**
:   The name of the *Email theme* used to hold template files. If the
    [Echo](http://drupal.org/project/echo) module is enabled this theme will
    also be used to transform the message body into a fully-themed webpage.

**`$theme_path`**
:   The relative path to the selected *Email theme* directory.

**`$theme_url`**
:   The absolute URL to the selected *Email theme* directory.

**`$debug`**
:   `TRUE` to add some useful debugging info to the bottom of the message.

Other modules may also add or modify theme variables by implementing a
`MODULENAME_preprocess_htmlmail(&$variables)`
[hook function](http://api.drupal.org/api/drupal/modules--system--theme.api.php/function/hook_preprocess_HOOK/7).
