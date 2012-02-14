/* $Id: README.txt,v 1.1.6.6.2.1 2010/10/05 20:29:11 alexua Exp $ */

/*********************/
 Embedded Media Field
/*********************/

Author: Aaron Winborn (aaron)
Maintainers: Aaron Winborn (aaron) + Alex Urevick-Ackelsberg (Alex UA)
Development Began 2007-06-13

Requires: Drupal 5 or 6, Content (CCK)
Optional: Views, FeedAPI/FeedAPI Element Mapper (see this for instructions for importing Embedded Video Feeds: http://zivtech.com/blog/module-mashup-creating-a-feed-embedded-videos-using-emfield-feedapi-and-feedapimapper ), Media Actions, Asset, & Thickbox.

The Embedded Media Field creates a field for nodes created with CCK to accept pasted URL's or embed code from various third party media content providers, such as YouTube and Flickr. It will then parse the URL to determine the provider, and display the media appropriately.

Currently, the module ships with Embedded Video Field, Embedded Image Field, and Embedded Audio Field. In addition, it has Embedded Media Import, to import photosets and playlists into individual nodes, when allowed by specific providers. Finally, it also ships with Embedded Media Thumbnail, which allows editors to override provider-supplied thumbnails with their own custom image thumbnails.

The module also allows field & provider specific settings and overrides, such as autoplay, resized thumbnails or videos for teasers, RSS support, and YouTube's 'related videos'. You can turn off individual provider support on a field or global basis.

/***************/
 Refreshing Data
/***************/

There are some instances in which your third-party data may need to be refreshed. That should be the first step to take if you notice media not working (that used to work before). You can refresh an individual node's third-party multimedia data by editing its node and resubmitting the info. You can also do this for multiple nodes by visiting the Content Management Administration page, by browsing to Administer > Content Management > Content (at /admin/content/node). Then select the nodes you wish to refresh, checking their respective check boxes, selecting Reload Embedded Media Data from the Update options drop-down, and pressing the Update button.

If you have the Job Queue module enabled (from http://drupal.org/project/job_queue), you will be able to similarly refresh all nodes on your site, by visiting the 'Reload data' tab that will then appear on the Embedded Media Field configuration page, by browsing to Administer > Content management > Embedded Media Field configuration > Reload data (at /admin/content/emfield/reload). Then select the content type(s) you wish to refresh and press the Submit button. All nodes will then be refreshed on your next cron batch (or several crons if you have a lot of nodes on your site).

/*********/
 Providers
/*********/

All provider files now must be installed seperately! Please see the emfield project page for a list of relevant Media: Modules.


You can:

    * Administer emfield's general settings at administer >> content >> emfield
    * Add embedded media fields to your content types at administer >> content >> types >> %YourType% >> add_field
    * Manage teaser and full node display settings at administer >> content >> types >> %YourType% >> fields

For the most up-to-date documentation, please see http://drupal.org/node/184346