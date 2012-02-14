## [HTML Mail](http://drupal.org/project/htmlmail)
Lets you theme your messages the same way you theme the rest of your website.

### [Requirement](http://www.dict.org/bin/Dict?Form=Dict2&Database=*&Query=requirement)

*   [Mail System 7.x-2.x](http://drupal.org/project/mailsystem)

### [Installation](http://drupal.org/documentation/install/modules-themes/modules-7)

The following additional modules, while not required, are highly recommended:

*   [Echo](http://drupal.org/project/echo)

    :   Wraps your messages in a drupal theme.  Now you can "brand" your
         messages with the same logo, header, fonts, and styles as your website.

*   [Emogrifier](http://drupal.org/project/emogrifier)

    :   Converts stylesheets to inline style rules, for consistent display on
        mobile devices and webmail.

*   [Mail MIME](http://drupal.org/project/mailmime)

    :   Provides a text/plain alternative to text/html emails, and automatically
        converts image references to inline image attachments.

*   [Pathologic](http://drupal.org/project/pathologic)

    :   Converts urls from relative to absolute, so clickable links in your
        email messages work as intended.

*   [Transliteration](http://drupal.org/project/filter_transliteration)

    :   Converts non-ASCII characters to their US-ASCII equivalents, such
        as from Microsoft "smart-quotes" to regular quotes.

    :   *Also available as a [patch](http://drupal.org/node/1095278#comment-4219530).*

### [Updating from previous versions](http://drupal.org/node/250790)

The [7.x-2.x](http://drupal.org/node/1106064) branch shares 94% of its code
with the [6.x-2.x](http://drupal.org/node/1119548) branch, but only 15% of
its code with the [7.x-1.x](http://drupal.org/node/355250) branch, and a tiny
8% of its code with the [6.x-1.x](http://drupal.org/node/329828) branch.

Let your compatibility expectations be adjusted accordingly.

*   Check the module dependencies, as they have changed.  The latest version of
    [HTML Mail](http://drupal.org/project/htmlmail) depends on the
    [Mail System](http://drupal.org/project/mailsystem) module (7.x-2.2 or later)
    and will not work without it.

*   Run `update.php` *immediately* after uploading new code.

*   The user-interface for adding email header and footer text has been removed.
    Headers and footers may be added by template files and/or by enabling the
    [Echo](http://drupal.org/project/echo) module.

*   Any customized filters should be carefully tested, as some of the template
    variables have changed.  Full documentation is provided both on the module
    configuration page (Click on the <u>Instructions</u> link) and as comments
    within the `htmlmail.tpl.php` file itself.

*   The following options have been removed from the module settings page.  In
    their place, any combination of
    [over 200 filter modules](http://drupal.org/project/modules/?filters=type%3Aproject_project%20tid%3A63%20hash%3A1hbejm%20-bs_project_sandbox%3A1%20bs_project_has_releases%3A1)
    may be used to create an email-specific
    [text format](http://drupal.org/node/778976)
    for post-template filtering.

    *   [Line break converter](http://api.drupal.org/api/drupal/modules--filter--filter.module/function/_filter_autop/7)
    *   [URL Filter](http://api.drupal.org/api/drupal/modules--filter--filter.module/function/_filter_url/7)
    *   [Relative Path to Absolute URLs](http://drupal.org/project/rel_to_abs)
    *   [Emogrifier](http://www.pelagodesign.com/sidecar/emogrifier/)
    *   [Token support](http://drupal.org/project/token)

*   Full MIME handling, including automatic generation of a plaintext
    alternative part and conversion of image references to inline image
    attachments, is available simply by enabling the
    [Mail MIME](http://drupal.org/project/mailmime) module.

### [Configuration](http://drupal.org/files/images/htmlmail_settings_2.thumbnail.png)

Visit the [Mail System](http://drupal.org/project/mailsystem) settings page at
<u>admin/config/system/mailsystem</u>
to select which parts of Drupal will use
[HTML Mail](http://drupal.org/project/htmlmail)
instead of the
[default](http://api.drupal.org/api/drupal/modules--system--system.mail.inc/class/DefaultMailSystem/7)
[mail system](http://api.drupal.org/api/drupal/includes--mail.inc/function/drupal_mail_system/7).

Visit the [HTML Mail](http://drupal.org/project/htmlmail) settings page at
<u>admin/config/system/htmlmail</u>
to select a theme and post-filter for your messages.

### [Theming](http://drupal.org/documentation/theme)

The email message text goes through three transformations before sending:

1.  <h3>Template File</h3>

    A template file is applied to your message header, subject, and body text.
    The default template is the included `htmlmail.tpl.php` file.  You may copy
    this file to your <cite>email theme</cite> directory (selected below), and
    use it to customize the contents and formatting of your messages. The
    comments within that file contain complete documentation on its usage.

2.  <h3>Theming</h3>

    You may choose a theme that will hold your templates from Step 1 above. If
    the [Echo](http://drupal.org/project/echo) module is installed, this theme
    will also be used to wrap your templated text in a webpage.  You use any one
    of [over 800](http://drupal.org/project/themes) themes to style your
    messages, or [create your own](http://drupal.org/documentation/theme) for
    even more power and flexibility.

3.  <h3>Post-filtering</h3>

    You may choose a
    [text format](http://drupal.org/node/778976)
    to be used for filtering email messages *after* theming.
    This allows you to use any combination of
    [over 200 filter modules](http://drupal.org/project/modules/?filters=type%3Aproject_project%20tid%3A63%20hash%3A1hbejm%20-bs_project_sandbox%3A1%20bs_project_has_releases%3A1)
    to make final changes to your message before sending.

    Here is a recommended configuration:

    *   [Emogrifier](http://drupal.org/project/emogrifier)
        Converts stylesheets to inline style rules for consistent display on
        mobile devices and webmail.

    *   [Transliteration](http://drupal.org/project/filter_transliteration)
        Converts non-ASCII text to US-ASCII equivalents.  This helps prevent
        Microsoft "smart-quotes" from appearing as question-marks in
        Mozilla Thunderbird.

    *   [Pathologic](http://drupal.org/project/pathologic)
        Converts relative URLS to absolute URLS so that clickable links in
        your message will work as intended.

### Troubleshooting
 
*   Check the [online documentation](http://drupal.org/node/1124376),
    especially the [screenshots](http://drupal.org/node/1124934).

*   There is a special documentation page for
    [Using HTML Mail together with SMTP Authentication Support](http://drupal.org/node/1200142).

*   [Simplenews](http://drupal.org/project/simplenews) users attempting advanced
    theming should read [this page](http://drupal.org/node/1260178).

*   Double-check the [Mail System](http://drupal.org/project/mailsystem)
    module settings and and make sure you selected
    <u><code>HTMLMailSystem</code></u> for your
    <u>Site-wide default mail system</u>.

*   Try selecting the <u><code>[ ]</code> *(Optional)* Debug</u> checkbox
    at the [HTML Mail](http://drupal.org/project/htmlmail) module
    settings page and re-sending your message.

*   Clear your cache after changing any <u><code>.tpl.php</code></u>
    files.

*   If you use a post-filter, make sure your filter settings page looks like
    [this](http://drupal.org/node/1130960).

*   Visit the [issue queue](http://drupal.org/project/issues/htmlmail)
    for support and feature requests.

### Related Modules

**Echo**
:   http://drupal.org/project/echo

**Emogrifier**
:   http://drupal.org/project/emogrifier

**HTML Purifier**
:   http://drupal.org/project/htmlpurifier

**htmLawed**
:   http://drupal.org/project/htmlawed

**Mail MIME**
:   http://drupal.org/project/mailmime

**Mail System**
:   http://drupal.org/project/mailsystem

**Pathologic**
:   http://drupal.org/project/pathologic

**Transliteration**
:   http://drupal.org/project/transliteration

### [Documentation](http://drupal.org/project/documentation)
 
**[HTML Mail](http://drupal.org/node/1124376)

**[filter.module](http://api.drupal.org/api/drupal/modules--filter--filter.module/6)**
:   [api.drupal.org/api/drupal/modules--filter--filter.module](http://api.drupal.org/api/drupal/modules--filter--filter.module/7)
:   [api.drupal.org/api/drupal/modules--filter--filter.module/group/standard_filters/7](http://api.drupal.org/api/drupal/modules--filter--filter.module/group/standard_filters/7)

**[Installing contributed modules](http://drupal.org/documentation/install/modules-themes/modules-7)**
:   [drupal.org/documentation/install/modules-themes/modules-7](http://drupal.org/documentation/install/modules-themes/modules-7)

**[Theming guide](http://drupal.org/documentation/theme)**
:   [drupal.org/documentation/theme](http://drupal.org/documentation/theme)

### Original Author

*   [Chris Herberte](http://drupal.org/user/1171)

### Current Maintainer

*   [Bob Vincent](http://drupal.org/user/36148)
