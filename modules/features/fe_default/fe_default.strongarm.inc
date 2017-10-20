<?php
/**
 * @file
 * fe_default.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fe_default_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'date_format_mini';
  $strongarm->value = 'd/m/y';
  $export['date_format_mini'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'node_admin_theme';
  $strongarm->value = '1';
  $export['node_admin_theme'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'statistics_count_content_views';
  $strongarm->value = 1;
  $export['statistics_count_content_views'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_content_types';
  $strongarm->value = array(
    'idea' => 'idea',
    'article' => 0,
    'page' => 0,
    'challenge' => 0,
    'faq' => 0,
    'news' => 0,
    'webform' => 0,
  );
  $export['watcher_content_types'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_default_settings';
  $strongarm->value = array(
    'watcher_automatic_enable_notifications' => 'watcher_automatic_enable_notifications',
    'watcher_notifications_updates' => 'watcher_notifications_updates',
    'watcher_notifications_new_comments' => 'watcher_notifications_new_comments',
    'watcher_autowatch_commented_on' => 'watcher_autowatch_commented_on',
    'watcher_autowatch_posted' => 'watcher_autowatch_posted',
    'watcher_share_binder' => 'watcher_share_binder',
  );
  $export['watcher_default_settings'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_display_intro_text';
  $strongarm->value = 'It\'s sometimes difficult to keep track of posts that are interesting or which you have been posting comments on. By <em>watching</em> posts that you want to keep an eye on you can see (and be notified) when they\'re updated or changed or when new comments are posted. This page lists all posts that you are currently watching. Click Help to learn more.
';
  $export['watcher_display_intro_text'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_display_last_updated';
  $strongarm->value = 0;
  $export['watcher_display_last_updated'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_display_node_type';
  $strongarm->value = 0;
  $export['watcher_display_node_type'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_display_post_author';
  $strongarm->value = 1;
  $export['watcher_display_post_author'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_email_notifications_comment_excerpt_len';
  $strongarm->value = '200chars';
  $export['watcher_email_notifications_comment_excerpt_len'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_email_notifications_enabled';
  $strongarm->value = 1;
  $export['watcher_email_notifications_enabled'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_email_notifications_method';
  $strongarm->value = 'cron';
  $export['watcher_email_notifications_method'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_email_notifications_send_time_limit';
  $strongarm->value = '5';
  $export['watcher_email_notifications_send_time_limit'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_help_text';
  $strongarm->value = '<div id="watcher_help_page">
<p>It\'s sometimes difficult to keep track of posts that are interesting or which you have been posting comments on. By <em>watching</em> posts that you want to keep an eye on you can see (and be notified) when they\'re updated or changed or when new comments are posted.</p>

<h3>Basic use</h3>
<ul>
<li>To start watching a post, click the link you see by the post on the site. The post is now added to your list of watched posts. Click again to stop watching the post.</li>
<li>In your watched posts list you can see all posts that you are watching. You can also see the number of comments and how many of those are new (not yet read by you).</li>
<li>If the site owner has enabled email notifications, next to each post is an icon that tells you whether changes to this post results in notifications by email. Click the icon to enable/disable email notifications. Emails are sent to the email address you have entered in your site account details.</li>
<li>To stop watching posts, either click the trash can icon next to the post in your watched posts list or click the link by the post.</li>
</ul>


<h3>Settings</h3>
<p>The settings page allows you to customize how and when posts are watched and notifications are sent. The site owner may have set default settings that will apply to all users who have not modified their settings. The default settings will apply until you have modified your settings.</p>
<dl>
<dt>Email Notification Settings</dt>
<dd>The email notification settings allow you to change your preferences regarding how and when email notifications are sent.
  <dl>
    <dt>Automatically enable email notifications for posts that I start watching</dt>
    <dd>When you start watching a post, email notifications are automatically enabled for it as well. You can disable email notifications by clicking the email icon in your watched posts list.</dd>
    <dt>Send email notification when a post is updated or edited</dt>
    <dd>An email notification is sent when the post is changed, either by its original author or someone else. This is useful for being notified about changes to posts that many users may edit. You will not be notified if you edited the post yourself.</dd>
    <dt>Send email notification when a post receives a new comment</dt>
    <dd>An email notification will be sent when a new comment is posted to the post you\'re watching. You will not be notified about comments you have made yourself.</dd>
  </dl>
</dd>
<dt>Watch Automatically</dt>
<dd>These settings control when posts are watched automatically without you having to manually click "watch this post".
  <dl>
    <dt>Automatically watch posts that I comment on</dt>
    <dd>When you post a comment on a post, the post will be added to your watched posts list without you having to manually add it. This is handy if you always want to watch all posts you have commented on and discuss with other users.</dd>
    <dt>Automatically watch posts that I make</dt>
    <dd>Posts that you have made will automatically be added to your watched posts list. This helps you keep track of what changes are made to your posts and the comments that are posted about your posts.</dd>
  </dl>
</dd>
<dt>Sharing</dt>
<dd>Your list of watched posts can be shared with other users.
  <dl>
    <dt>Share my list of watched posts</dt>
    <dd>If you want other users to be able see what posts you watch, enable this setting. When you\'re sharing your list, a link will appear in your profile allowing other users to browse your list of watched posts.</dd>
  </dl>
</dd>
</dl></div>';
  $export['watcher_help_text'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_notifications_templates_body_cmt';
  $strongarm->value = '[comment:author:name] wrote a new comment about \'[node:title]\':
[node:url]

Comment excerpt:
"[comment-excerpt]..."

To read the rest, view the entire comment by clicking the link below:
[comment:url]

Click the link below to reply to this comment:
[comment-reply-url]
';
  $export['watcher_notifications_templates_body_cmt'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_notifications_templates_body_confirm';
  $strongarm->value = 'A request to watch the following page has been made:
[node:url]

As long as the page is being watched you will receive email messages when
new comments or updates are made, depending on what you have chosen to
watch for.

If this request was made in error, please click either of the links below.';
  $export['watcher_notifications_templates_body_confirm'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_notifications_templates_body_node';
  $strongarm->value = 'The following post has been updated:
[node:title]

Click the link below to view it:
[node:url]
';
  $export['watcher_notifications_templates_body_node'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_notifications_templates_footer';
  $strongarm->value = '

------------------------------------------

To stop watching this post, click the link below:
[stop-watching-url]

If you believe you have received this message in error, please contact the site administrator at the following email address:
[site:mail]

Sincerely,

The staff at [site:name] - [site:slogan]
';
  $export['watcher_notifications_templates_footer'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_notifications_templates_footer_anonymous_notify';
  $strongarm->value = '-----------------------------------------

To stop watching THIS post, click here:
[stop-watching-url]

To stop watching ALL posts on this site, click here:
[stop-watching-all-url]

If you have any further questions, please contact the site administrator at the following email address:
[site:mail]

Sincerely,

The staff at [site:name] - [site:slogan]
';
  $export['watcher_notifications_templates_footer_anonymous_notify'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_notifications_templates_footer_confirm';
  $strongarm->value = '-----------------------------------------

The request was made from IP address: [user-ip]

To stop watching THIS post, click here:
[stop-watching-url]

To stop watching ALL posts on this site, click here:
[stop-watching-all-url]

If you have any further questions, please contact the site administrator at the following email address:
[site:mail]

Sincerely,

The staff at [site:name] - [site:slogan]
';
  $export['watcher_notifications_templates_footer_confirm'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_notifications_templates_header';
  $strongarm->value = '[recipient-username],

Your are receiving this email because you have chosen to receive email notifications from [site:name] ([site:url]). To change this, click the link below to go to your personal settings and disable email notifications:
[recipient-user-settings-url]

------------------------------------------

';
  $export['watcher_notifications_templates_header'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'watcher_toggle_link_in_teaser';
  $strongarm->value = 1;
  $export['watcher_toggle_link_in_teaser'] = $strongarm;

  return $export;
}
