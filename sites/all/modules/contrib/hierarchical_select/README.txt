$Id: README.txt,v 1.42 2011/02/06 13:57:18 wimleers Exp $

Description
-----------
This module defines the "hierarchical_select" form element, which is a greatly
enhanced way for letting the user select items in a hierarchy.

Hierarchical Select has the ability to save the entire lineage of a selection
or only the "deepest" selection. You can configure it to force the user to
make a selection as deep as possible in the tree, or allow the user to select
an item anywhere in the tree. Levels can be labeled, you can configure limit
the number of items that can be selected, configure a title for the dropbox,
choose a site-wide animation delay, and so on. You can even create new items
and levels through Hierarchical Select!


Integrates with
---------------
* Forum (Drupal core)
* Menu (Drupal core)
* Taxonomy (Drupal core)
* Content Taxonomy (http://drupal.org/project/content_taxonomy)
* Views


Installation
------------
1) Place this module directory in your "modules" folder (this will usually be
"sites/all/modules/"). Don't install your module in Drupal core's "modules"
folder, since that will cause problems and is bad practice in general. If
"sites/all/modules" doesn't exist yet, just create it.

2) Enable the module.

3) If you want to use it for one or more of your vocabularies, go to
admin/content/taxonomy and click the "edit" link for a vocabulary. Now scroll
down and you'll find a whole range of Hierarchical Select settings. All
settings are explained there as well.


Troubleshooting
---------------
If you ever have problems, make sure to go through these steps:

1) Go to admin/logs/status (i.e. the Status Report). Ensure that the status
   of the Hierarchical Select module is ok.

2) Ensure that the page isn't being served from your browser's cache. Use
   CTRL+R in Windows/Linux browsers, CMD+R in Mac OS X browsers to enforce the
   browser to reload everything, preventing it from using its cache.

3) When you're getting a JS alert with the following message: "Received an
   invalid response from the server.", ensure that the page (of which this
   form is a part) is *not* being cached.

4) When Hierarchical Select seems to be misbehaving in a certain use case in
   which terms with multiple parents are being used, make sure to enable the
   "Save term lineage" setting.
   Note: you may have to repeat this for every configuration in which the
   vocabulary with terms that have multiple parents are being used. E.g. if
   such a vocabulary is called "A", then go to 
      admin/settings/hierarchical_select/configs
   and edit all configuration that have "A" in the "Hierarchy" column.

In case of problems, don't forget to try a hard refresh in your browser!


Limitations
-----------
- Creating new items in the hierarchy in a multiple parents hierarchy (more
  scientifically: a directed acyclic graph) is *not* supported.
- Not the entire scalability problem can be solved by installing this set of
  modules; read the maximum scalability section for details.
- The child indicators only work in Firefox. This *cannot* be supported in
  Safari or IE. See http://drupal.org/node/180691#comment-1044691.
- The special [save-lineage-termpath] token only works with content_taxonomy
  fields as long as you have the "Save option" set to either "Tag" or "Both".
- In hierarchies where items can have multiple parent items and where you have
  enabled Hierarchical Select's "save lineage" setting, it is impossible to
  remember individual hierarchies, unless the underlying module supports it.
  So far, no module supports this. Hierarchical Select is just a form element,
  not a system for storing hierarchies.
  For example, if you have created a multiple parent vocabulary through the
  Taxonomy module, and you have terms like this:
   A -> C
   A -> D
   B -> C
   B -> D
  If you then save any two lineages in which all four terms exist, all four
  lineages will be rendered by Hierarchical Select, because only the four
  terms are stored and thus there is no way to recover the originally selected
  two lineages.
- You can NOT expect the Hierarchical Select Taxonomy module to automagically
  fix all existing nodes when you enable or disable the "save lineage" setting
  and neither can you expect it to keep working properly when you reorganize
  the term hierarchy. There's nothing I can do about this. Hierarchical Select
  is merely a form element, it can't be held responsible for features that
  Drupal core lacks or supports poorly.
  See the following issues:
  * http://drupal.org/node/1023762#comment-4054386
  * http://drupal.org/node/976394#comment-4054456


Maximum scalability
-------------------
hs_taxonomy takes advantage of the taxonomy_override_selector variable to
improve scalability: the whole tree is no longer loaded by Drupal core.
While the hs_book, hs_menu and hs_subscriptions modules override existing form
items, those form items are *still* being generated. This can cause scalability
issues.
If you want to fix this, you *will* have to modify the original modules (so
that includes Drupal core modules). Simply move the changes from the
hook_form_alter() implementations into the corresponding form definitions.


Hierarchical Select Taxonomy: using the [save-lineage-termpath] token
---------------------------------------------------------------------
When you're using the Token module, and likely the Pathauto module, and you've
configured your Hierarchical Select to save the lineage, you may want to show
the saved lineage (or the first lineage in case you've also enabled the
dropbox) in your URL. That's possible through the [save-lineage-termpath]
token (and other similar tokens). However, by default it uses the separator
you've configured Pathauto to use (if you aren't using Pathauto then it will
default to a dash). You can override this by setting the hs_taxonomy_separator
variable. Also, when you're using Pathauto and it seems to be stripping the
separator you've configured, then you may want to configure that character in
Pathauto's Punctuation settings to "No action (do not replace)".


Using the dropbox in Views exposed filters
------------------------------------------
This can be very tricky, due to a combination of the respective limitations of
Taxonomy and Views exposed filters.
See http://drupal.org/node/346033.


Rendering hierarchy lineages when viewing content
-------------------------------------------------
Hierarchical Select is obviously only used for input. Hence it is only used on
the create/edit forms of content.
Combine that with the fact that Hierarchical Select is the only module capable
of restoring the lineage of saved items (e.g. Taxonomy terms). None of the
Drupal core modules is capable of storing the lineage, but Hierarchical Select
can reconstruct it relatively efficiently. However, this lineage is only
visible when creating/editing content, not when viewing it.
To allow you to display the lineages of stored items, I have provided a
theming function that you can call from within e.g. your node.tpl.php file:
the theme_hierarchical_select_selection_as_lineages($selection, $config)
function.

Sample usage (using Taxonomy and Hierarchical Select Taxonomy):
  <?php if ($taxonomy):
    require_once(drupal_get_path('module', 'hierarchical_select') . '/includes/common.inc');
    $vid = 2;                                                    // Vocabulary ID. CHANGE THIS!
    $config_id = "taxonomy-$vid";                                // Generate the config ID.
    $config = hierarchical_select_common_config_get($config_id); // Get the Hierarchical Select configuration through the config ID.
    $config['module'] = 'hs_taxonomy';                           // Set the module.
    $config['params']['vid'] = $vid;                             // Set the parameters.
  ?>
    <div class="terms"><?php print theme('hierarchical_select_selection_as_lineages', $node->taxonomy, $config); ?></div>
  <?php endif; ?>

This will automatically render all lineages for vocabulary 2 (meaning that if
you want to render the lineages of multiple vocabularies, you'll have to clone
this piece of code once for every vocabulary). It will also automatically get
the current Hierarchical Select configuration for that vocabulary.

Alternatively, you could provide the $config array yourself. Only three keys
are required: 1) module, 2) params, 3) save_lineage. For example:
  <?php if ($taxonomy):
    $vid = 2;                          // Vocabulary ID. CHANGE THIS!
    $config['module'] = 'hs_taxonomy'; // Set the module.
    $config['params']['vid'] = $vid;   // Set the parameters.
    $config['save_lineage'] = 1;       // save_lineage setting is enabled. CHANGE THIS!
  ?>
    <div class="terms"><?php print theme('hierarchical_select_selection_as_lineages', $node->taxonomy, $config); ?></div>
  <?php endif; ?>

If you don't like how the lineage is displayed, simply override the
theme_hierarchical_select_selection_as_lineages() function from within your
theme, create e.g. garland_hierarchical_select_selection_as_lineages().


Setting a fixed size
--------------------
When you don't want users to be able to resize a hierarchical select
themselves, you can set a fixed size in advance yourself
Setting #size to >1 does *not* generate #multiple = TRUE selects! And the
opposite is also true. #multiple sets the "multiple" HTML attribute. This
enables the user to select multiple options of a select. #size just controls
the "size" HTML attribute. This increases the vertical size of selects,
thereby showing more options.
See http://www.w3.org/TR/html401/interact/forms.html#adef-size-SELECT.


Sponsors
--------
* Initial development:
   Paul Ektov of http://autobin.ru.
* Abstraction, to let other modules than taxonomy hook in:
   Etienne Leers of http://creditcalc.biz.
* Support for saving the term lineage:
   Paul Ektov of http://autobin.ru.
* Multiple select support:
   Marmaladesoul, http://marmaladesoul.com.
* Taxonomy Subscriptions support:
   Mr Bidster Inc.
* Ability to create new items/levels:
   The Worx Company, http://www.worxco.com.
* Ability to only show items that are associated with at least one entity:
   Merge, http://merge.nl.
* Views 2 support:
   Merge, http://merge.nl.


Author
------
Wim Leers

* website: http://wimleers.com/
* contact: http://wimleers.com/contact

The author can be contacted for paid development on this module. This can vary
from new features to Hierarchical Select itself, to new implementations (i.e.
support for new kinds of hierarchies).
