  This module gives Drupal the ability to easily change links into popup dialog boxes.

  IMPORTANT INSTRUCTIONS
  ------------------------------------------------------------------------------------
  Ajax updating only works with themes that have selectable content areas. 
  If you are not using garland, you will need to figure out the selector for your theme, 
  and enter it into the "Content Selector" field on the admin/build/themes/settings page
  for your theme. Open the page.tpl.php file for your theme, and search for "print $content".
  The $content should be surrounded by a div with an id. Ex:
    <div id="content-content">
      <?php print $content; ?>
    </div> <!-- /content-content -->
  In this case, just enter '#content-content' into the Content Selector field.
  Unfortunately, a lot of themes do not have well defined content areas.  Just add the div yourself,
  and then complain on the issue queue for the theme.  It is important that there are no other
  print statements inside the div.

  LIMITATIONS
  ------------------------------------------------------------------------------------  
  Does not work with tinymce. Unlikely to work with other WYSIWYG's. (Is this still true?)
  Conflicts with: 
    Devel Theme Developer module.

  HOW TO USE THE POPUPS API
  ------------------------------------------------------------------------------------  
  If you just want to use the built in admin links, just enable the Popups: Admin Links
  module and you are good to go.
  If you want to add popups behavior to new links, or incorporate popups into your module,
  there are a couple of ways to do it.
  
  #1) Attach popup behavior to a link with popups_add_popups() call.
  ----------------------------------------------------------------  
  <!-- In html or theme -->
  <a href="popups/test/response" id="mylink"> 
  <a href="popups/test/response" id="mylink2"> 
  
  // In your module
  popups_add_popups(array('#mylink', '#mylink2=>array('width'=>'200px')));  
  This is the simplest method if you want to pass in per-link options.
  The first key is a jQuery selector. It should select an 'a' element (unless you 
  are using the 'href' option). See http://docs.jquery.com/Selectors to learn more 
  about jQuery selectors.
  The array is a set of Options. See below for the list of options.
  No array means just use the defualts. 
  
  #2) Add the class="popup" to an existing link.
  -------------------------------------------
  And then either be sure popups_add_popups() is called sometime for the page,
  or use the "Scan all pages for popup links" checkbox on the popups settings page. 
  
  Example on the theme level ("Scan all pages for popups links" must be checked):
    <a href="popups/test/response" class="popups">

  Example in code:
    popups_add_popups();
    $output .= l("Pop up entire local page.", 'popups/test/response', array('attributes'=>array('class' => 'popups')));
  
  Here are the classes that you can use:
  class="popups" requests an informational popup (or a form that doesn't want ajax processing).
  class="popups-form" requests a popup with a form that modifies the content of the original page.
  class="popups-form-reload" requests a popup with a form, and reloads the entire page when done.
  class="popups-form-noupdate" requests a popup with a form, and leaves the original page as-is.
  
  You can use the pseudo-attribute, "on-popups-options" to send options, if you don't mind having non-validating HTML.
  Note: this attribute gets removed from user content by the HTML filter.
  Example:
    print l("Pop with options (width=200px).", 'popups/test/response', 
             array('attributes'=>array(array('class' => 'popups', 'on-popups-options' => '{width: "200px"}'))))
  See popups_test.module for more examples.    
  
  #3) Add a custom module that implements hook_popups().
  ---------------------------------------------------------------------
  hook_popups() returns an array of popup rules, keyed by the id of a form, 
  or the url of a page (which can use the wildcard '*').
  Each rule is an array of options, keyed by a jQuery selector.  
  Leaving off the options array is equal to a link with class="popup-form".
  This is equivent to using a series of popup_add_popups() calls.
  
  Rule Format Example:
    'admin/content/taxonomy' => array( // Act only on the links on this page. 
      'div#tabs-wrapper a:eq(1)',  // No options, so use defaults.
      'table td:nth-child(2) a' => array( 
        'noUpdate' => true, // Popup will not modify original page.
      ),
    );
  
  #4) Make your module alter the default popup rules with hook_popups_alter().
  ----------------------------------------------------------------------------
  hook_popups_alter() allows you to modify how the popup rules are
  registered. This is useful to modify the default behavior of some
  already existing popup rules.

  See hook_popups_alter() in popups.api.php for an example.


  LIST OF POPUP OPITIONS
  ------------------------------------------------------------------------------------ 
  DEPRECATED OPTIONS
//   noUpdate: Does the popup NOT modify the original page (Default: FALSE).
//   reloadWhenDone: Force the entire page to reload after the popup form is submitted (Default: FALSE)
//   nonModel: Not working.
//   forceReturn: url to force a stop to work flow (Advanced. Use in conjunction with noUpdate or targetSelectors).  
//   afterSubmit: function to call when updating after successful form submission.   
   
   doneTest: how do we know when the multiform flow is done?
     null: flow is done when returned path = original path (default).
     *path*: 
     *regexp*: done when returned path matches regexp.
   updateMethod:
     none: do not update the initial page 
     ajax: targeted replacement of parts of the initial page (default).
     reload: full replacement of initial page with new page.
     callback: use onUpdate(data, options, element).
   updateSource (only used if updateMethod is not none):
     initial: use the initial page (default).
     final: use the path returned at the end of the multiform flow.
   href: Override the href in the a element, or attach an href to a non-link element.
   width: Override the width specified in the css.
   targetSelectors: Hash of jQuery selectors that define the content to be swapped out.
   titleSelectors: Array of jQuery selectors to place the new page title.
   reloadOnError: Force the entire page to reload if the popup href is unaccessable (Default: FALSE)
   noMessage: Don't show drupal_set_message messages.
   onUpdate: function to call when updating after successful form submission.   
   skipDirtyCheck: If true, this popup will not check for edits on the originating page.  
                   Often used with custom target selectors. Redundant is noUpdate is true. (Default: FALSE)
   hijackDestination: Use the destiination param to force a form submit to return to the originating page.
                      Overwrites any destination already set one the link (Default: TRUE)
   
 