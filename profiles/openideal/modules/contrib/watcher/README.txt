Watcher Module for Drupal 7.x

Feature overview

    * Lets users watch nodes for changes or new comments being posted without
      having to post themselves.+
    * Uses AJAX to toggle watching for a smooth user experience.
    * Allows both anonymous and registered users to watch nodes.
    * Supports email notifications being sent when both or either of the
      above occurs.
    * Template-based notification messages using the Token module - edit the
      templates to your liking!
    * Provides a binder with an overview of the nodes you watch. From the
      binder you can also toggle email notifications for each node you watch.
    * Users may choose to publish the binder so that other users can see what
      they're watching, providing a form of social bookmarking.
    * Uses a queue-based email dispatcher to handle large numbers of notifications.
    * Users can opt to automatically watch nodes they create or comment on.
    * User settings can have default values, set by the site owner, that can be
      customized and overridden by the user.
    * Highly customizable, configurable user interface and themeable.
    * Designed with usability and exceptional user experience as a high priority.

Watcher was developed to fill a gap that has existed for a long time. Even
though there are several subscription and notification modules out there,
none is both easy to use or particularly specialized and allows instant
(or near instant) notifications. Watcher is not a general solution to
notifications but caters to the needs of community sites.

Requirements
------------
To install this version of Watcher you need:

* Drupal 7.x
* Comments module (included in Drupal)
* Node module (included in Drupal)
* Token module (http://drupal.org/project/token)

There's a version for Drupal 5 and 6 as well, available from:
http://drupal.org/project/watcher


Installation
------------
To install Watcher, do the following:

  1. Download, install and configure the Token module, follow the
     instructions for that module.

  2. Download the latest stable version (however only dev available for D7 now)
     of Watcher from its project page: http://drupal.org/project/watcher

  3. Unpack the file you downloaded into sites/all/modules or the
     modules directory of your site.

  4. Go to Administration -> Modules and enable the module.

  5. Go to Configuration -> Watcher and select at least
     one content type for the module.

     NOTE: The module will not work and users will not be able to watch
     posts unless you select at least *one* content type.
     Click "Save configuration" to save your changes.


Upgrading from Drupal 6
-----------------------
Run the update.php script. After this no special steps will be needed.

Upgrading from Drupal 5
-----------------------
If you've been using a previous version of Watcher, make sure you go to update.php
and run the update script after uploading the new module files.


Issues
------
In case Watcher does not appear to work for anonymous users even though the
role Anonymous has been granted the necessary permissions, your users table
may be missing the anonymous user.

To fix this, first empty your site's cache. You can do that by going to
Configuration -> Development -> Performance

Secondly, make sure there's a row with uid 0 in your site's users table.
You can confirm that by executing this SQL query (applies to MySQL):
SELECT * FROM `users` WHERE uid = 0;

The query should return exactly one row. If no row is returned, the row is
missing and must be restored. Execute this query to add the row (applies to MySQL):
INSERT INTO `users` VALUES (0, '', '', '', 0, 0, 0, '', '', 0, 0, 0, 0, NULL, '', '', '', NULL);

See: http://drupal.org/node/370459


CAPTCHA support and preventing spam
-----------------------------------
Watcher hasn't tested to work with CAPTCHA 7.x yet. The following info has been left
from Watcher 6.x readme:

Since Watcher now supports anonymous users watching nodes, CAPTCHA support has
been added to help prevent spam submissions. Download CAPTCHA module:
http://www.drupal.org/project/captcha

Install CAPTCHA and set up its permissions as needed. Then go to:
admin/user/captcha/captcha/captcha_point

Enter "_watcher_watch_toggle_anonymous_form" (quotation marks omitted) in the
"Form ID" field. Select a challenge type and save the form. CAPTCHA will now display
the challenge you selected when anonymous users want to start watching a node.

To implement a first line of defense against spambots you can use the Bad Behavior
module. It uses heuristics to determine whether the browser is a genuine
human-controlled software application or an automated spambot:
http://drupal.org/project/badbehavior


Configuration
-------------
The module can be configured by going to:
Configuration -> Watcher

You can customize a wide range of options. Most of this is quite straight-forward
however some things may require an explanation.

Under "Settings for Email Notifications" you can choose what method to use for
sending emails. This will have an impact on how many messages are sent at once.
Watcher stores notifications to be sent in a message queue and every time messages
are sent they're taken off the queue. The method affects when the queue messages are
actually sent, it can either happen when cron and cron.php is being run or when a
user posts a comment.

If there are many messages in the queue, the best setting to use is cron and making
sure cron is run frequently enough that the entire queue is being processed. You can
change the time limit for message sending. The server will send as many messages as
possible during this interval. Any messages left unsent will remain in the queue
until it is processed again.

You may also modify the notification message templates that are used to generate the
notification messages. You can use placeholder tokens to insert the content that
changes, such as recipient name and comment excerpt.

Watcher also allows every user on your site to customize how the module works for them.
Users may enable or disable email notifications and make it so that they automatically
watch every post they make or comment on. Users who have not yet altered these settings
will be affected by the user settings defaults. These default settings apply until a
user goes to his or her settings page for Watcher and clicks "Save".

The page titled Statistics shows some basic statistics about the module which may be of
interest to you as a site owner. If there are unsent messages in the message queue,
these can be viewed here as well.

The Testing page allows you to do testing in case notification messages are not being
delivered. You can create test messages that are sent to your own email address.


Retroactively add existing posts
--------------------------------
If you have a large site and a large quantity of nodes/posts you may want to add the
existing nodes your users have made or commented on to their watched posts lists.

You can do this by running the SQL queries below. The queries below will do the
following:
- add all posts a user has created to his/her watched posts list
- add the nodes belonging to every comment a user has made to his/her watched posts list

CAUTION: Make a database dump to keep as a backup copy before attempting this!
CAUTION: The following has only been tested with MySQL 5!

INSERT IGNORE INTO watcher_nodes (uid, nid, send_email, added) SELECT uid, nid, 1, UNIX_TIMESTAMP() FROM node;
INSERT IGNORE INTO watcher_nodes (uid, nid, send_email, added) SELECT uid, nid, 1, UNIX_TIMESTAMP() FROM comments;


Theming
-------
Watcher is entirely themable. Open watcher.module in your editor to see what
theme functions are available to be overridden. The functions are documented in
detail in the code.

Themable functions of interest (please refer to function's comments for more
detailed description):
  * watcher_binder($vars)
  * watcher_binder_email_icon($vars)
  * watcher_binder_stop_watching_icon($vars)
  * watcher_node_toggle_watching_link($vars)
  * watcher_help_page($vars)

Watcher outputs the watcher link at the end of the node's body by default. A future
feature will be to make this configurable, allowing the link to be output in the
node's links section, at the beginning or end of body or in a separate template
variable. Until then, you can use the following method to separate the watcher link
from the node's body.

In node.tpl.php, to display the watcher link:

  <?php if($node->content['watcher']['#value']): ?>
    <div class="node_watcher">
      <?php print $node->content['watcher']['#value']; ?>
    </div>
  <?php endif; ?>

And the node's body:

  <div class="content">
    <div class="content_description"><?php print $node->content['body']['#value']; ?></div>
  </div>

For more info, please see: http://drupal.org/node/11816


Terminology
-----------
I have tried to consistently use the term "post" in the parts of the module that
the end users see as it makes way more sense to non-Drupallers than the word "node".
In the code comments and documentation, I've used the word "node" where I talk
about nodes.

The word "binder", is used interchangeable with the term "watched posts list". They're
exactly the same thing, "binder" is a term Hans Dekker of Wordsy.com suggested and it's
stayed in the module since its first versions.


Author
------
The module was developed by Jakob Persson <http://drupal.org/user/37564> of
NodeOne, Sweden's leading Drupal consulting firm. Our goal is to empower the
user by building usable, powerful and effective web solutions for our clients.
Visit us at http://www.nodeone.se

I am a developer that considers usability and user experience to be some of the
defining properties of successful software applications as well as devices and
appliances. I have a background in cognitive science and HCI and I work with
user experience and usability at NodeOne, building beautiful, usable
web sites and intranet applications for our clients.

The author can be contacted for paid customizations of this module as well as
Drupal consulting, installation, development and customizations.


Sponsors
--------
The development of this module has been sponsored by

  * Wordsy <http://www.wordsy.com>
  * NodeOne <http://www.nodeone.se>s


Thanks
------
To Hans Dekker <http://www.hansdekker.com> for the idea as well as suggestions
and feedback.

Some of the icons were either taken directly from the Tango Icon Library or were
derived from Tango icons.
http://tango.freedesktop.org/