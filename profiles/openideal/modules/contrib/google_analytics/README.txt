// $Id: README.txt,v 1.7.2.3 2010/06/03 01:40:24 hass Exp $

Module: Google Analytics
Author: Alexander Hass <http://drupal.org/user/85918>


Description
===========
Adds the Google Analytics tracking system to your website.

Requirements
============

* Google Analytics user account


Installation
============
* Copy the 'googleanalytics' module directory in to your Drupal
sites/all/modules directory as usual.


Usage
=====
In the settings page enter your Google Analytics account number.

You can also track the username and/or user ID who visits each page.
This data will be visible in Google Analytics as segmentation data.
If you enable the profile.module you can also add more detailed
information about each user to the segmentation tracking.

All pages will now have the required JavaScript added to the
HTML footer can confirm this by viewing the page source from
your browser.

New approach to page tracking in 5.x-1.5 and 6.x-1.1
====================================================
With 5.x-1.5 and 6.x-1.1 there are new settings on the settings page at
admin/settings/googleanalytics. The "Page specific tracking" area now
comes with an interface that copies Drupal's block visibility settings.

The default is set to "Add to every page except the listed pages". By
default the following pages are listed for exclusion:

admin
admin/*
user/*/*
node/add*
node/*/*

These defaults are changeable by the website administrator or any other
user with 'administer google analytics' permission.

Like the blocks visibility settings in Drupal core, there is now a
choice for "Add if the following PHP code returns TRUE." Sample PHP snippets
that can be used in this textarea can be found on the handbook page
"Overview-approach to block visibility" at http://drupal.org/node/64135.

A code snippet that creates opt-out by role functionality for unchecked roles
can be found in the Google Analytics handbook at http://drupal.org/node/261997.

Advanced Settings
=================
You can include additional JavaScript snippets in the custom javascript
code textarea. These can be found on the official Google Analytics pages
and a few examples at http://drupal.org/node/248699. Support is not
provided for any customisations you include.

To speed up page loading you may also cache the Analytics ga.js
file locally. You need to make sure the site file system is in public
download mode.