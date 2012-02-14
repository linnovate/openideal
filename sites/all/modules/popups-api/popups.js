(function ($) {
/**
 * Popup Modal Dialog API
 *
 * Provide an API for building and displaying JavaScript, in-page, popups modal dialogs.
 * Modality is provided by a fixed, semi-opaque div, positioned in front of the page contents.
 *
 */

/*
 * TODO
 * * Return key in add node form not working.
 * * Tabledrag breaking after ahah reload.
 */

// ***************************************************************************
// Popups Namespace **********************************************************
// ***************************************************************************
/**
 * The Popups namespace contains:
 * * An ordered stack of Popup objects,
 * * The state of the original page,
 * * Functions for managing both of the above.
 */
var Popups = function(){};

/**
 * Static variables in the Popups namespace.
 */
Popups.popupStack = [];
Popups.addedCSS = {};
Popups.addedJS = {};
Popups.originalCSS = {};
Popups.originalJS = {};
Popups.originalSettings = null; // The initial popup options of the page.
/**
 * Each popup object gets it's own set of options.
 * These are the defaults.
 */
Popups.defaultOptions = {
  doneTest: null, // null, *path*, *regexp*. how do we know when a multiform flow is done?
  updateMethod: 'ajax', // none, ajax, reload, *callback*
  updateSource: 'initial', // initial, final. Only used if updateMethod != none.
  onUpdate: '', // Only used if updateMethod == callback.
  href: null,
  width: null, // Override the width specified in the css.
  targetSelectors: null, // Hash of jQuery selectors that define the content to be swapped out.
  titleSelectors: null, // Array of jQuery selectors to place the new page title.
  reloadOnError: false, // Force the entire page to reload if the popup href is unaccessable.
  noMessage: false, // Don't show drupal_set_message messages.
  skipDirtyCheck: false, // If true, this popup will not check for edits on the originating page.
  hijackDestination: true // Use the destiination param to force a form submit to return to the originating page.
};

// ***************************************************************************
// Popups.Popup Object *******************************************************
// ***************************************************************************
/**
 * A Popup is a single modal dialog.
 * The popup object encapslated all the info about a single popup.
 */
Popups.Popup = function() {
  this.id = 'popups-' + Popups.nextCounter();

  // These properties are needed if the popup contains a form that will be ajax submitted.
  this.parent = null; // The popup that spawned this one. If parent is null, this popup was spawned by the original page.
  this.path = null; // If popup is showing content from a url, this is that path.
  this.element = null; // The DOM element that was clicked to launch this popup.
  this.options = null; // An option array that control how the popup behaves.  See Popups.defaultOptions for explainations.
};
Popups.Popup.prototype.$popup = function() {
  return $('#' + this.id);
};
Popups.Popup.prototype.$popupBody = function() {
  return $('#' + this.id + ' .popups-body');
};
Popups.Popup.prototype.$popupClose = function() {
  return $('#' + this.id + ' .popups-close');
};
Popups.Popup.prototype.$popupTitle = function() {
  return $('#' + this.id + ' .popups-title');
};
Popups.Popup.prototype.$popupButtons = function() {
  return $('#' + this.id + ' .popups-buttons');
};
Popups.Popup.prototype.$popupFooter = function() {
  return $('#' + this.id + ' .popups-footer');
};

/**
 * Create the jQuery wrapped html at the heart of the popup object.
 *
 * @param title
 *   String
 * @param body
 *   String/HTML
 * @param buttons
 *   Hash/Object
 * @return
 *   The $popup.
 */
Popups.Popup.prototype.fill = function(title, body, buttons) {
  return $(Drupal.theme('popupDialog', this.id, title, body, buttons));
};

/**
 * Hide the popup by pushing it off to the side.
 * Just making it display:none causes flash in FF2.
 */
Popups.Popup.prototype.hide = function() {
  this.$popup().css('left', '-9999px');
};

Popups.Popup.prototype.show = function() {
  Popups.resizeAndCenter(this);
};

Popups.Popup.prototype.open = function(title, body, buttons, width){
  return Popups.open(this, title, body, buttons, width);
};

Popups.Popup.prototype.removePopup = function() {
  Popups.removePopup(this);
};

/**
 * Remove everything.
 */
Popups.Popup.prototype.close = function() {
  return Popups.close(this);
};

/**
 * Set the focus on the popups to the first visible, enabled form element, or the close link.
 */
Popups.Popup.prototype.refocus = function() {
  // Select the first visible enabled input element.
  var $popup = this.$popup();
  var $focus = $popup.find(':input:visible:enabled:first');
  if (!$focus.length) {
    // There is no visible enabled input element, so select the close link.
    $focus = $popup.find('.popups-close a');
  }
  $focus.focus();
};

/**
 * Return a selector that will find target content on the layer that spawned this popup.
 * This is needed for the popup to do ajax updates.
 */
Popups.Popup.prototype.targetLayerSelector = function() {
  if (this.parent === null) {
    return 'body'; // Select content in the original page.
  }
  else {
    return '#' + this.parent.id; // Select content in the parent popup.
  }
};

/**
 * Determine if we are at an end point of a form flow, or just moving from one popups to another.
 *
 * @param path
 *   The path of the page that the form flow has moved to.
 *   This path is relative to the base_path.
 *   Ex: node/add/story, not http://localhost/drupal6/node/add/story or drupa6/node/add/story.
 * @return bool
 */
Popups.Popup.prototype.isDone = function(path) {
  var done;
  if (this.options.doneTest) {
    // Test if we are at the path specified by doneTest.
    done = (path === this.options.doneTest || path.match(this.options.doneTest));
  }
  else {
    if (this.parent) {
       // Test if we are back to the parent popup's path.
      done = (path === this.parent.path);
    }
    else {
       // Test if we are back to the original page's path.
      done = (path === Popups.originalSettings.popups.originalPath);
    }
  }
  return done;
};


// ***************************************************************************
// Popups Functions **********************************************************
// ***************************************************************************

/**
 * Test if the param has been set.
 * Used to distinguish between a value set to null or false and on not yet unset.
 */
Popups.isset = function(v) {
  return (typeof(v) !== 'undefined');
};

/**
 * Get the currently active popup in the page.
 * Currently it is the only one visible, but that could change.
 */
Popups.activePopup = function() {
  if (Popups.popupStack.length) {
    return Popups.popupStack[Popups.popupStack.length - 1]; // top of stack.
  }
  else {
    return null;
  }
};

/**
 * Manage the page wide popupStack.
 */
Popups.push = function(popup) {
  Popups.popupStack.push(popup);
};
// Should I integrate this with popupRemove??
Popups.pop = function(popup) {
  return Popups.popupStack.pop();
};

/**
 * Build an options hash from defaults.
 *
 * @param overrides
 *   Hash of values to override the defaults.
 */
Popups.options = function(overrides) {
  var defaults = Popups.defaultOptions;
  return Popups.overrideOptions(defaults, overrides);
};

/**
 * Build an options hash.
 * Also maps deprecated options to current options.
 *
 * @param defaults
 *   Hash of default values
 * @param overrides
 *   Hash of values to override the defaults with.
 */
Popups.overrideOptions = function(defaults, overrides) {
  var options = {}, option;
  for (option in defaults) {
    if (defaults.hasOwnProperty(option)) {
      if (Popups.isset(overrides[option])) {
        options[option] = overrides[option];
      }
      else {
        options[option] = defaults[option];
      }
    }
  }
  // Map deprecated options.
  if (overrides.noReload || overrides.noUpdate) {
    options.updateMethod = 'none';
  }
  if (overrides.reloadWhenDone) {
    options.updateMethod = 'reload';
  }
  if (overrides.afterSubmit) {
    options.updateMethod = 'callback';
    options.onUpdate = overrides.afterSubmit;
  }
  if (overrides.forceReturn) {
    options.doneTest = overrides.forceReturn;
  }
  return options;
};

/**
 * Attach the popups behavior to all elements inside the context that match the selector.
 *
 * @param context
 *   Chunk of html to search.
 * @param selector
 *   jQuery selector for elements to attach popups behavior to.
 * @param options
 *   Hash of options associated with these links.
 */
Popups.attach = function(context, selector, options) {
  $(selector, context).not('.popups-processed').each(function() {
    var $element = $(this);

    // Mark the element as processed.
    $element.addClass('popups-processed');

    // Append note to link title.
    var title = '';
    if ($element.attr('title')) {
      title = $element.attr('title') + ' ';
    }
    title += Drupal.t('[Popup]');
    $element.attr('title', title);

    // Attach the on-click popup behavior to the element.
    $element.click(function(event){
      return Popups.clickPopupElement(this, options);
    });
  });
};

/**
 * Respond to click by opening a popup.
 *
 * @param element
 *   The element that was clicked.
 * @param options
 *   Hash of options associated with the element.
 */
Popups.clickPopupElement = function(element, options) {
  Popups.saveSettings();

  // If the element contains a on-popups-options attribute, override default options param.
  if ($(element).attr('on-popups-options')) {
    var overrides = Drupal.parseJson($(element).attr('on-popups-options'));
    options = Popups.overrideOptions(options, overrides);
  }

  // The parent of the new popup is the currently active popup.
  var parent = Popups.activePopup();

  // If the option is distructive, check if the page is already modified, and offer to save.
  var willModifyOriginal = !(options.updateMethod === 'none' || options.skipDirtyCheck);
  if (willModifyOriginal && Popups.activeLayerIsEdited()) {
    // The user will lose modifications, so show dialog offering to save current state.
    Popups.offerToSave(element, options, parent);
  }
  else {
    // Page is clean, or popup is safe, so just open it.
    Popups.openPath(element, options, parent);
  }
  return false;
};

/**
 * Test if the active layer been edited.
 * Active layer is either the original page, or the active Popup.
 */
Popups.activeLayerIsEdited = function() {
  var layer = Popups.activePopup();
  var $context = Popups.getLayerContext(layer);
  // TODO: better test for edited page, maybe capture change event on :inputs.
  var edited = $context.find('span.tabledrag-changed').length;
  return edited;
};

/**
 * Show dialog offering to save form on parent layer.
 *
 * @param element
 *   The DOM element that was clicked.
 * @param options
 *   The options associated with that element.
 * @param parent
 *   The layer that has the unsaved edits.  Null means the underlying page.
 */
Popups.offerToSave = function(element, options, parent) {
  var popup = new Popups.Popup();
  var body = Drupal.t("There are unsaved changes in the form, which you will lose if you continue.");
  var buttons = {
   'popup_save': {title: Drupal.t('Save Changes'), func: function(){Popups.saveFormOnLayer(element, options, parent);}},
   'popup_submit': {title: Drupal.t('Continue'), func: function(){popup.removePopup(); Popups.openPath(element, options, parent);}},
   'popup_cancel': {title: Drupal.t('Cancel'), func: function(){popup.close();}}
  };
  popup.open(Drupal.t('Warning: Please Confirm'), body, buttons);
};

/**
 * Generic dialog builder.
 * Adds the newly built popup into the DOM.
 *
 * TODO: capture the focus if it tabs out of the dialog.
 *
 * @param popup
 *   Popups.Popup object to fill with content, place in the DOM, and show on the screen.
 * @param String title
 *   String: title of new dialog.
 * @param body (optional)
 *   String: body of new dialog.
 * @param buttons (optional)
 *   Hash of button parameters.
 * @param width (optional)
 *   Width of new dialog.
 *
 * @return popup object
 */
Popups.open = function(popup, title, body, buttons, width){
  Popups.addOverlay();

  if (Popups.activePopup()) {
    // Hiding previously active popup.
    Popups.activePopup().hide();
  }

  if (!popup) {
    // Popup object was not handed in, so create a new one.
    popup = new Popups.Popup();
  }
  Popups.push(popup); // Put this popup at the top of the stack.

  // Create the jQuery wrapped html for the new popup.
  var $popup = popup.fill(title, body, buttons);
  popup.hide(); // Hide the new popup until it is finished and sized.

  if (width) {
    $popup.css('width', width);
  }

  // Add the new popup to the DOM.
  $('body').append($popup);

  // Add button function callbacks.
  if (buttons) {
    $.each(buttons, function(id, button){
      $('#' + id).click(button.func);
    });
  }

  // Add the default click-to-close behavior.
  popup.$popupClose().click(function(){
    return Popups.close(popup);
  });

  Popups.resizeAndCenter(popup);

  // Focus on the first input element in the popup window.
  popup.refocus();

  // TODO - this isn't the place for this - should mirror addLoading calls.
  // Remove the loading image.
  Popups.removeLoading();

  return popup;
};

/**
 * Adjust the popup's height to fit it's content.
 * Move it to be centered on the screen.
 * This undoes the effects of popup.hide().
 *
 * @param popup
 */
Popups.resizeAndCenter = function(popup) {
  var $popup = popup.$popup();

  // center on the screen, adding in offsets if the window has been scrolled
  var popupWidth = $popup.width();
  var windowWidth = Popups.windowWidth();
  var left = (windowWidth / 2) - (popupWidth / 2) + Popups.scrollLeft();

  // Get popups's height on the page.
  $popup.css('height', 'auto'); // Reset height.
  var popupHeight = $popup.height();
  $popup.height(popupHeight);
  var windowHeight = Popups.windowHeight();

  if (popupHeight > (0.9 * windowHeight) ) { // Must fit in 90% of window.
    popupHeight = 0.9 * windowHeight;
    $popup.height(popupHeight);
  }
  var top = (windowHeight / 2) - (popupHeight / 2) + Popups.scrollTop();

  $popup.css('top', top).css('left', left); // Position the popups to be visible.
};


/**
 *  Create and show a simple popup dialog that functions like the browser's alert box.
 */
Popups.message = function(title, message) {
  message = message || '';
  var popup = new Popups.Popup();
  var buttons = {
    'popup_ok': {title: Drupal.t('OK'), func: function(){popup.close();}}
  };
  popup.open(title, message, buttons);
  return popup;
};

/**
 * Handle any special keys when popups is active.
 */
Popups.keyHandle = function(e) {
  if (!e) {
    e = window.event;
  }
  switch (e.keyCode) {
    case 27: // esc
      Popups.close();
      break;
    case 191: // '?' key, show help.
      if (e.shiftKey && e.ctrlKey) {
        var $help = $('a.popups.more-help');
        if ($help.size()) {
          $help.click();
        }
        else {
          Popups.message(Drupal.t("Sorry, there is no additional help for this page"));
        }
      }
      break;
  }
};

/*****************************************************************************
 * Appearence Functions (overlay, loading graphic, remove popups)     *********
 *****************************************************************************/

/**
 * Add full page div between the page and the dialog, to make the popup modal.
 */
Popups.addOverlay = function() {
  var $overlay = $('#popups-overlay');
  if (!$overlay.length) { // Overlay does not already exist, so create it.
    $overlay = $(Drupal.theme('popupOverlay'));
    $overlay.css('opacity', '0.4'); // for ie6(?)
    // Doing absolute positioning, so make overlay's size equal the entire body.
    var $doc = $(document);
    $overlay.width($doc.width()).height($doc.height());
    $('body').prepend($overlay);
  }
};

/**
 * Remove overlay if popupStack is empty.
 */
Popups.removeOverlay = function() {
  if (!Popups.popupStack.length) {
    $('#popups-overlay').remove();
  }
};

/**
 * Add a "Loading" message while we are waiting for the ajax response.
 */
Popups.addLoading = function() {
  var $loading = $('#popups-loading');
  if (!$loading.length) { // Loading image does not already exist, so create it.
    $loading = $(Drupal.theme('popupLoading'));
    $('body').prepend($loading); // Loading div is initially display:none.
    var width = $loading.width();
    var height = $loading.height();
    var left = (Popups.windowWidth() / 2) - (width / 2) + Popups.scrollLeft();
    var top = (Popups.windowHeight() / 2) - (height / 2) + Popups.scrollTop();
    $loading.css({'top': top, 'left': left, 'display': 'block'}); // Center it and make it visible.
  }
};

Popups.removeLoading = function() {
  $('#popups-loading').remove();
};

// Should I fold this function into Popups.pop?
Popups.removePopup = function(popup) {
  if (!Popups.isset(popup)) {
    popup = Popups.activePopup();
  }
  if (popup) {
    popup.$popup().remove();
    Popups.popupStack.splice($.inArray(popup,Popups.popupStack), 1); // Remove popup from stack.  Probably should rework into .pop()
  }
};

/**
 * Remove everything.
 */
Popups.close = function(popup) {
  if (!Popups.isset(popup)) {
    popup = Popups.activePopup();
  }
  Popups.removePopup(popup);  // Should this be a pop??
  Popups.removeLoading();
  if (Popups.activePopup()) {
    Popups.activePopup().show();
    Popups.activePopup().refocus();
  }
  else {
    Popups.removeOverlay();
    Popups.restorePage();
  }
  return false;
};

/**
 * Save the page's original Drupal.settings.
 */
Popups.saveSettings = function() {
  if (!Popups.originalSettings) {
    Popups.originalSettings = Drupal.settings;
  }
};

/**
 * Restore the page's original Drupal.settings.
 */
Popups.restoreSettings = function() {
  Drupal.settings = Popups.originalSettings;
};

/**
 * Remove as much of the effects of jit loading as possible.
 */
Popups.restorePage = function() {
  var i;
  Popups.restoreSettings();
  // Remove the CSS files that were jit loaded for popup.
  for (i in Popups.addedCSS) {
    if (Popups.addedCSS.hasOwnProperty(i)) {
      $('link[href='+ Popups.addedCSS[i] + ']').remove();
    }
  }
  Popups.addedCSS = [];
};


/****************************************************************************
 * Utility Functions   ******************************************************
 ****************************************************************************/

/**
 * Get the position of the left side of the browser window.
 */
Popups.scrollLeft = function() {
  return Math.max(document.documentElement.scrollLeft, document.body.scrollLeft);
};

/**
 * Get the position of the top of the browser window.
 */
Popups.scrollTop = function() {
  return Math.max(document.documentElement.scrollTop, document.body.scrollTop);
};

/**
 * Get the height of the browser window.
 * Fixes jQuery & Opera bug - http://drupal.org/node/366093
 */
Popups.windowHeight = function() {
  if ($.browser.opera && $.browser.version > "9.5" && $.fn.jquery <= "1.2.6") {
    return document.documentElement.clientHeight;
  }
  return $(window).height();
};

/**
 * Get the height of the browser window.
 * Fixes jQuery & Opera bug - http://drupal.org/node/366093
 */
Popups.windowWidth = function() {
  if ($.browser.opera && $.browser.version > "9.5" && $.fn.jquery <= "1.2.6") {
    return document.documentElement.clientWidth;
  }
  return $(window).width();
};

Popups.nextCounter = function() {
  if (this.counter === undefined) {
    this.counter = 0;
  }
  else {
    this.counter++;
  }
  return this.counter;
};

/****************************************************************************
 * Ajax Functions   ******************************************************
 ****************************************************************************/

/**
 * Add additional CSS to the page.
 */
Popups.addCSS = function(css) {
  var type, file, link, href;
  Popups.addedCSS = [];
  for (type in css) {
    if (css.hasOwnProperty(type)) {
      for (file in css[type]) {
        if (css[type].hasOwnProperty(file)) {
          link = css[type][file];
          href = $(link).attr('href');
          // Does the page already contain this stylesheet?
          if (!Popups.originalCSS[href.replace(/^(\/.+)\?\w$/, '$1')] && !Popups.addedCSS[href]) {
            $('head').append(link);
            Popups.addedCSS[href] = 1; // Keep a list, so we can remove them later.
          }
        }
      }
    }
  }
};

/**
 * Add additional Javascript to the page.
 */
Popups.addJS = function(js) {
  // Parse the json info about the new context.
  var inlines = [], src, type, file,
    evalOnSuccess = function(script) { eval(script); };

  for (type in js) {
    if (js.hasOwnProperty(type) && type !== 'setting') {
      for (file in js[type]) {
        if (js[type].hasOwnProperty(file)) {
          if (type === 'inline') {
            inlines.push($(js[type][file]).text());
          }
          else {
            src = $(js[type][file]).attr('src');
            var b = src.replace(/^(\/.+)\?\w$/, '$1');
           var a = Popups.originalJS[b];

            if (!a && !Popups.addedJS[src]) {
              // Get the script from the server and execute it.
              $.ajax({
                type: 'GET',
                url: src,
                dataType: 'script',
                async : false,
                success: evalOnSuccess,
              });
              // Mark the js as added to the underlying page.
              Popups.addedJS[src] = 1;
            }
          }
        }
      }
    }
  }

  // Add new JS settings to the page, needed for #ahah properties to work.
  $.extend(true, Drupal.settings, js.setting);

  return inlines;
};

/**
 * Execute the jit loaded inline scripts.
 * Q: Do we want to re-excute the ones already in the page?
 *
 * @param inlines
 *   Array of inline scripts.
 */
Popups.addInlineJS = function(inlines) {
  var i, l;
  // Load the inlines into the page.
  for (i = 0, l = inlines.length; i < l; i++) {
    // If the script is not already in the page, execute it.
    //if (!$('script:not([src]):contains(' + inlines[n] + ')').length) {
      eval(inlines[i]);
    //}
  }
};

Popups.beforeSend = function(xhr) {
  xhr.setRequestHeader("X-Drupal-Render-Mode", 'json/popups');
};

/**
 * Do before the form in the popups is submitted.
 */
Popups.beforeSubmit = function(formData, $form, options) {
  Popups.removePopup(); // Remove just the dialog, but not the overlay.
  Popups.addLoading();
};


/****************************************************************************
 * Page & Form in popups functions                                         ***
 ****************************************************************************/

/**
 * Use Ajax to open a link in a popups window.
 *
 * @param element
 *   Element that was clicked to open the popups.
 * @param options
 *   Hash of options controlling how the popups interacts with the underlying page.
 * @param parent
 *   If path is being opened from inside another popup, that popup is the parent.
 */
Popups.openPath = function(element, options, parent) {
  Popups.saveSettings();

  // Let the user know something is happening.
  $('body').css("cursor", "wait");

  // TODO - get nonmodal working.
  if (!options.nonModal) {
    Popups.addOverlay();
  }
  Popups.addLoading();

  var href = options.href ? options.href : element.href;
  $(document).trigger('popups_open_path', [element, href]); // Broadcast Popup Open Path event.

  var params = {};
  // Force the popups to return back to the orignal page when forms are done, unless hijackDestination option is set to FALSE.
  if (options.hijackDestination) {
    var returnPath;
    if (parent) {
      returnPath = parent.path;
    }
    else { // No parent, so bring flow back to original page.
      returnPath = Popups.originalSettings.popups.originalPath;
    }
    href = href.replace(/destination=[^;&]*[;&]?/, ''); // Strip out any existing destination param.
    params.destination = returnPath; // Set the destination to return to the parent's path.
  }

  var ajaxOptions = {
    url: href,
    dataType: 'json',
    data: params,
    beforeSend: Popups.beforeSend,
    success: function(json) {
      // Add additional CSS to the page.
      Popups.addCSS(json.css);
      var inlines = Popups.addJS(json.js);
      var popup = Popups.openPathContent(json.path, json.title, json.messages + json.content, element, options, parent);
      Popups.addInlineJS(inlines);
      // Broadcast an event that the path was opened.
      $(document).trigger('popups_open_path_done', [element, href, popup]);
    },
    complete: function (response, status) {
    	$('body').css("cursor", "auto"); // Return the cursor to normal state.
    	if (status == 'error' || status == 'parsererror') {
          return ajax.error(response, ajax.url);
        }
    	
    }
  };

  if (options.reloadOnError) {
    ajaxOptions.error = function() {
      location.reload(); // Reload on error. Is this working?
    };
  }
  else {
    ajaxOptions.error = function() {
      Popups.message("Unable to open: " + href);
    };
  }
  $.ajax(ajaxOptions);

  return false;
};

/**
 * Open path's content in an ajax popups.
 *
 * @param title
 *   String title of the popups.
 * @param content
 *   HTML to show in the popups.
 * @param element
 *   A DOM object containing the element that was clicked to initiate the popup.
 * @param options
 *   Hash of options controlling how the popups interacts with the underlying page.
 * @param parent
 *   Spawning popup, or null if spawned from original page.
 */
Popups.openPathContent = function(path, title, content, element, options, parent) {
  var popup = new Popups.Popup();
  Popups.open(popup, title, content, null, options.width);

  // Set properties on new popup.
  popup.parent = parent;
  popup.path = path;
  popup.options = options;
  popup.element = element;

  // Add behaviors to content in popups.
  delete Drupal.behaviors.tableHeader; // Work-around for bug in tableheader.js (http://drupal.org/node/234377)
  delete Drupal.behaviors.teaser; // Work-around for bug in teaser.js (sigh).
  Drupal.attachBehaviors(popup.$popupBody());
  // Adding collapse moves focus.
  popup.refocus();

  // If the popups contains a form, capture submits.
  var $form = $('form', popup.$popupBody());
  if ($form.length) {
    $form.ajaxForm({
      dataType: 'json',
      beforeSubmit: Popups.beforeSubmit,
      beforeSend: Popups.beforeSend,
      success: function(json, status) {
        Popups.formSuccess(popup, json);
      },
      error: function() {
        Popups.message("Bad Response form submission");
      }
    });
  }
  return popup;
};

/**
 * The form in the popups was successfully submitted
 * Update the originating page.
 * Show any messages in a popups.
 *
 * @param popup
 *   The popup object that contained the form that was just submitted.
 * @param data
 *   JSON object from server with status of form submission.
 */
Popups.formSuccess = function(popup, data) {
  // Determine if we are at an end point, or just moving from one popups to another.
  var done = popup.isDone(data.path);
  if (!done) { // Not done yet, so show new page in new popups.
    Popups.removeLoading();
    Popups.openPathContent(data.path, data.title, data.messages + data.content, popup.element, popup.options, popup.parent);
  }
  else { // We are done with popup flow.
    // Execute the onUpdate callback if available.
    if (popup.options.updateMethod === 'callback' && popup.options.onUpdate) {
      var result = eval(popup.options.onUpdate +'(data, popup.options, popup.element)');
      if (result === false) { // Give onUpdate callback a chance to skip normal processing.
        return;
      }
    }

    if (popup.options.updateMethod === 'reload') { // Force a complete, non-ajax reload of the page.
      if (popup.options.updateSource === 'final') {
        location.href = Drupal.settings.basePath + data.path; // TODO: Need to test this.
      }
      else { // Reload originating page.
        location.reload();
      }
    }
    else { // Normal, targeted ajax, reload behavior.
      var showingMessagePopup = false;
      // Show messages in dialog and embed the results in the original page.
      // TODO - should seperate these two functions.
//      var showMessage = data.messages.length && !popup.options.noMessage;
      if (data.messages.length) {
        // If we just dismissed the last popup dialog.
        if (!Popups.activePopup() && !popup.options.noMessage) {
          // Show drupal_set_message in message popup.
          var messagePopup = Popups.message(data.messages);
          if (Popups.originalSettings.popups.autoCloseFinalMessage) {
            setTimeout(function(){Popups.close(messagePopup);}, 2500); // Autoclose the message box in 2.5 seconds.
          }
          showingMessagePopup = true;
        }


        // Insert the message into the parent layer, above the content.
        // Might not be the standard spot, but it is the easiest to find.
        var $next;
        if (popup.targetLayerSelector() === 'body') {
          $next = $('body').find(Popups.originalSettings.popups.defaultTargetSelector);
        }
        else {
          $next = $(popup.targetLayerSelector()).find('.popups-body');
        }
        $next.parent().find('div.messages').remove(); // Remove the existing messages.
        $next.before(data.messages); // Insert new messages.
      }

      // Update the content area (defined by 'targetSelectors').
      if (popup.options.updateMethod !== 'none') {
        Popups.testContentSelector(); // Kick up warning message if selector is bad.

        Popups.restoreSettings(); // Need to restore original Drupal.settings.popups.links before running attachBehaviors.  This probably has CSS side effects!
        if (popup.options.targetSelectors) { // Pick and choose what returned content goes where.
          $.each(popup.options.targetSelectors, function(t_new, t_old) {
            if (!isNaN(t_new)) {
              t_new = t_old; // handle case where targetSelectors is an array, not a hash.
            }
            var new_content = $(t_new, data.content);
            var $c = $(popup.targetLayerSelector()).find(t_old).html(new_content); // Inject the new content into the original page.

            Drupal.attachBehaviors($c);
          });
        }
        else { // Put the entire new content into default content area.
          var $c = $(popup.targetLayerSelector()).find(Popups.originalSettings.popups.defaultTargetSelector).html(data.content);
          Drupal.attachBehaviors($c);
        }
      }

      // Update the title of the page.
      if (popup.options.titleSelectors) {
        $.each(popup.options.titleSelectors, function() {
          $(this).html(data.title);
        });
      }

      // Done with changes to the original page, remove effects.
      Popups.removeLoading();
      if (!showingMessagePopup) {
        // If there is not a messages popups, pop the stack.
        // Sending in null to Popups.close reveales the next popup in the stack.
        // If the stack is empty, it will remove the overlay.
        Popups.close(null);
      }
    }

    // Broadcast an event that popup form was done and successful.
    $(document).trigger('popups_form_success', [popup]);

  }  // End of updating spawning layer.
};


/**
 * Get a jQuery object for the content of a layer.
 * @param layer
 *   Either a popup, or null to signify the original page.
 */
Popups.getLayerContext = function(layer) {
  var $context;
  if (!layer) {
    $context = $('body').find(Popups.originalSettings.popups.defaultTargetSelector);
  }
  else {
    $context = layer.$popupBody();
  }
  return $context;
};

/**
 * Submit the page and reload the results, before popping up the real dialog.
 *
 * @param element
 *   Element that was clicked to open a new popup.
 * @param options
 *   Hash of options controlling how the popups interacts with the underlying page.
 * @param layer
 *   Popup with form to save, or null if form is on original page.
 */
Popups.saveFormOnLayer = function(element, options, layer) {
  var $context = Popups.getLayerContext(layer);
  var $form = $context.find('form');
  var ajaxOptions = {
    dataType: 'json',
    beforeSubmit: Popups.beforeSubmit,
    beforeSend: Popups.beforeSend,
    success: function(response, status) {
      // Sync up the current page contents with the submit.
      var $c = $context.html(response.content); // Inject the new content into the page.
      Drupal.attachBehaviors($c);
      // The form has been saved, the page reloaded, now safe to show the triggering link in a popup.
      Popups.openPath(element, options, layer);
    }
  };
  $form.ajaxSubmit(ajaxOptions); // Submit the form.
};

/**
 * Warn the user if ajax updates will not work
 *   due to mismatch between the theme and the theme's popup setting.
 */
Popups.testContentSelector = function() {
  var target = Popups.originalSettings.popups.defaultTargetSelector;
  var hits = $(target).length;
  if (hits !== 1) { // 1 is the corrent answer.
    var msg = Drupal.t('The popup content area for this theme is misconfigured.') + '\n';
    if (hits === 0) {
      msg += Drupal.t('There is no element that matches ') + '"' + target + '"\n';
    }
    else if (hits > 1) {
      msg += Drupal.t('There are multiple elements that match: ') + '"' + target + '"\n';
    }
    msg += Drupal.t('Go to admin/build/themes/settings, select your theme, and edit the "Content Selector" field');
    alert(msg);
  }
};


// ***************************************************************************
// DRUPAL Namespace
// ***************************************************************************

/**
 * Attach the popups bevior to the all the requested links on the page.
 *
 * @param context
 *   The jQuery object to apply the behaviors to.
 */

Drupal.behaviors.popups = {
  attach: function (context, settings) {
	  Popups.saveSettings();
	
	  var $body = $('body'),
	    $popit;
	
	  if (!$body.hasClass('popups-processed')) {
	    $body.addClass('popups-processed');
	    $(document).bind('keydown', Popups.keyHandle);
	    $popit = $('#popit');
	    if ($popit.length) {
	      $popit.remove();
	      Popups.message($popit.html());
	    }
	
	    // Make note of all the CSS and JS on the page so when we load a popup we
	    // don't try to add them a second time.
	    $('link[rel="stylesheet"][href]').each(function(i, v) {
	      Popups.originalCSS[$(this).attr('href').replace(/^(\/.+)\?\w$/, '$1')] = 1;
	    });
	    if (Drupal.settings.popups && Drupal.settings.popups.originalCSS) {
	      $.extend(Popups.originalCSS, Drupal.settings.popups.originalCSS);
	    }
	    $('script[src]').each(function(i, v) {
	      Popups.originalJS[$(this).attr('src').replace(/^(\/.+)\?\w$/, '$1')] = 1;
	    });
	    if (Drupal.settings.popups && Drupal.settings.popups.originalJS) {
	      $.extend(Popups.originalJS, Drupal.settings.popups.originalJS);
	    }
	  }
	
	  // Add the popups-link-in-dialog behavior to links defined in Drupal.settings.popups.links array.
	  // Get these from current Drupal.settings, not Popups.originalSettings, as each page has it's own hooks.
	  if (Drupal.settings.popups && Drupal.settings.popups.links) {
	    $.each(Drupal.settings.popups.links, function (link, options) {
	      Popups.attach(context, link, Popups.options(options));
	    });
	  }
	
	  Popups.attach(context, '.popups', Popups.options({updateMethod: 'none'}));
	  Popups.attach(context, '.popups-form', Popups.options({updateMethod: 'ajax'})); // ajax reload.
	  Popups.attach(context, '.popups-form-reload', Popups.options({updateMethod: 'reload'})); // whole page reload.
	  Popups.attach(context, '.popups-form-noupdate', Popups.options({updateMethod: 'none'}));  // no reload at all.
	}
};

// ****************************************************************************
// * Theme Functions   ********************************************************
// ****************************************************************************

Drupal.theme.prototype.popupLoading = function() {
  var loading = '<div id="popups-loading">';
  loading += '<img src="'+ Drupal.settings.basePath + Popups.originalSettings.popups.modulePath + '/ajax-loader.gif" />';
  loading += '</div>';
  return loading;
};

Drupal.theme.prototype.popupOverlay = function() {
  return '<div id="popups-overlay"></div>';
};

Drupal.theme.prototype.popupButton = function(title, id) {
  return '<input type="button" value="'+ title +'" id="'+ id +'" />';
};

Drupal.theme.prototype.popupDialog = function(popupId, title, body, buttons) {
  var template = Drupal.theme('popupTemplate', popupId);
  var popups = template.replace('%title', title).replace('%body', body);

  var themedButtons = '';
  if (buttons) {
    $.each(buttons, function (id, button) {
      themedButtons += Drupal.theme('popupButton', button.title, id);
    });
  }
  popups = popups.replace('%buttons', themedButtons);
  return popups;
};

Drupal.theme.prototype.popupTemplate = function(popupId) {
  var template;
  template += '<div id="'+ popupId + '" class="popups-box">';
  template += '  <div class="popups-title">';
  template += '    <div class="popups-close"><a href="#">' + Drupal.t('Close') + '</a></div>';
  template += '    <div class="title">%title</div>';
  template += '    <div class="clear-block"></div>';
  template += '  </div>';
  template += '  <div class="popups-body">%body</div>';
  template += '  <div class="popups-buttons">%buttons</div>';
  template += '  <div class="popups-footer"></div>';
  template += '</div>';
  return template;
};
/*jslint devel: true, onevar: false, browser: true, evil: true, undef: true, maxerr: 50, indent: 2 */

})(jQuery);