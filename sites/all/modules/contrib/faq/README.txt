CONTENTS OF THIS FILE
----------------------

  * Introduction
  * Installation
  * Configuration
  * Known Issues
    - No FAQs appear after module upgrade
    - <p> tags appear in FAQ question text
    - Clicking on category links takes user to front page


INTRODUCTION
------------
Maintainer: Stella Power (http://drupal.org/user/66894)

Documentation: http://drupal.org/node/129209

The Frequently Asked Questions (faq) module allows users with the 'administer
faq' permission to create question and answer pairs which they want displayed on
the 'faq' page.  The 'faq' page is automatically generated from the FAQ nodes
configured and the layout of this page can be modified on the settings page.
Users will need the 'view faq' permission to view the 'faq' page.

There are 2 blocks included in this module, one shows a list of FAQ categories
while the other can show a configurable number of recent FAQs added.

Note the function theme_faq_highlights(), which shows the last X recently
created FAQs, used by one of the blocks, can also be called in a php-filtered
node if desired.


INSTALLATION
------------
1. Copy faq folder to modules directory.
2. At admin/build/modules enable the faq module.
3. Enable permissions at admin/user/permissions.
4. Configure the module at admin/settings/faq.


UPGRADE NOTICE
---------------
When using categorized FAQ nodes, the disabling of the FAQ module causes the
vocabulary to lose the association with the FAQ content type. This results in no
FAQ nodes being displayed when you re-enable the FAQ module. Before opening an
issue, please verify that this setting is still in place.


CONFIGURATION
-------------
Once the module is activated, you can create your question and answer pairs by
creating FAQ nodes (Create content >> FAQ).  This allows you to edit the
question and answer text.  In addition, if the 'Taxonomy' module is enabled and
there are some terms configured for the FAQ node type, it will also be possible
to put the questions into different categories when editing.


KNOWN ISSUES
-------------
No FAQs appear after module upgrade
-----------------------------------
When using categorized FAQ nodes, the disabling of the FAQ module causes the
vocabulary to lose the association with the FAQ content type. This results in no
FAQ nodes being displayed when you re-enable the FAQ module. Before opening an
issue, please verify that this setting is still in place.

<p> tags appear in FAQ question text
------------------------------------
When using WYSIWYG editors, such as TinyMCE and FCKeditor, <p> and other HTML
tags may appear in the displayed question text. This is because the faq title or
question input box is a textarea and not a textfield, so the faq module can
accommodate longer question texts. The p-tags come from the WYSIWYG editor used
and not the FAQ module. This is because TinyMCE, and other WYSIWYG editors,
attach themselves to all textareas on a given page.  Details on how to prevent
this can be found at http://drupal.org/node/294708

Clicking on category links takes user to front page
---------------------------------------------------
Reported for Drupal 5, unconfirmed for Drupal 6.
Instead of being taken to the categorized faq page, the front page is displayed
when the user clicks on a faq category. This is something to do with the
pathauto module and can be easily fixed by doing a bulk update of paths for the
faq vocabulary.

