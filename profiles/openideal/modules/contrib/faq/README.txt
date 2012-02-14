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
the 'faq-page' page.  The 'faq-page' page is automatically generated from the
FAQ nodes configured and the layout of this page can be modified on the settings
page.  Users will need the 'view faq page' permission to view the 'faq-page'
page.

An alternative to the built-in 'faq-page' is to use one of the example Views
layouts provided which you can easily customise to your needs using the Views
UI.  Note, the configuration settings for the module do not apply to the Views
layouts.


INSTALLATION
------------
1. Copy faq folder to modules directory.
2. At admin/modules enable the faq module.
3. Enable permissions at admin/people/permissions.
4. Configure the module at admin/config/content/faq - not used for Views
   layouts.
5. You can use the default faq page at "faq-page" or enable one of the page
   layouts in the example Views.  For the Views pages you can change the url if
   needed, but if you wish to change the url for the built-in page (faq-page)
   you need to create a url alias at admin/config/search/path.


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

