README.txt
==========
Plus1

A module that allows users to cast a +1 vote.
This module depends on the Voting API module.
The Voting API provides:
- Its own table to store the votes.
- Helper functions to store votes and retrieve results.
- Integration with the Views module.

Be sure to grant appropriate roles the permission to 'rate content'.

INSTALLATION
=============

To install this module:

- First install the Voting API module, following the install instructions provided with that module.
- Then copy the plus1 folder to your sites/all/modules directory.
- Enable the module on the page ?q=admin/build/modules/list.
- Be sure to grant appropriate roles the permission to 'rate content'.

INFORMATION FOR THEMERS
========================

First off, you can change the 'You voted' text on the module admin page ?q=admin/settings/plus1.

Additionally, you can override the theming function theme_plus1_widget in your theme.

From plus1.module theme_plus1_widget function header:

/**
* Theme for the voting widget.
*
* You are free to load your own CSS and JavaScript files in your
* theming function override, instead of the ones provided by default.
*
* This function adds information to the Drupal.settings.plus1 JS object,
* concerning class names used for the voting widget.
* If you override this theming function but choose to use the
* default JavaScript file, simply assign different values to
* the following variables:
*    $widget_class   (The wrapper element for the voting widget.)
*    $link_class     (The anchor element to cast a vote.)
*    $message_class  (The wrapper element for the anchor element. May contain feedback when the vote has been cast.)
*    $score_class    (The placeholder element for the score.)
* The JavaScript looks for these CSS hooks to
* update the voting widget after a vote is cast.
* Of course you may choose to write your own JavaScript.
* The JavaScript adds presentation, ie: fade in.
*
*/

AUTHOR/MAINTAINER
==================
-Caroline Schnapp at http://11heavens.com (chill35 on http://drupal.org)
