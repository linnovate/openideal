CONTENTS OF THIS FILE
---------------------
 * Introduction
 * Features
 * Installation
 * Useful Information
 * Known Issues
 * Authors
 * Sponsors


INTRODUCTION
-----------------------

Sasson is a powerful base theme intended for advanced drupal theming, aiming at bringing the fun back to theming.
It is a collection of many open source goodies combined together in a modular structure, everything is optional and we keep it so that what you don't use won't leave any trace in your output code.

Clean and simple code, lightweight structure, latest technologies, 100% open-source and the best DX (developer experience) we could think of is what we hope you will find in this new drupal base theme.


FEATURES
---------------

 * It includes a Sass compiler & Compass framework (via a modified PHamlP, optional) - no extra requirements, simply enable and start writing sass/scss.
 * It's mobile friendly - with responsive, content-first layout, out of the box. optional mobile-first responsive layout, media queries break-points are configurable.
 * It converts the core template files to HTML5 markup - <header>, <footer>, <article> for nodes, <section> for blocks, <aside> for sidebars, <nav> for menus etc.(thanks Boron - http://drupal.org/project/boron)
 * It includes a perfectly semantic grid system (based on 960gs http://960.gs/ via Compass - http://compass-style.org).
 * It includes an HTML5-friendly CSS Reset (normalize), cross-browser styling compatibility improvements and other tweaks & best practices from HTML5Boilerplate v3.0.1 (http://html5boilerplate.com)
 * It *doesn't* give you a pile of CSS rules you will have to override.


MORE FEATURES
---------------

 * Adaptive grid - on your theme settings you may choose width, # of columns, gutter width, we (well, SASS) do the math.
 * Ready made sub-theme. just copy, rename, and start theming
 * Google web-fonts support (http://www.google.com/webfonts)
 * FireSASS support (https://addons.mozilla.org/en-US/firefox/addon/firesass-for-firebug)
 * HTML5 support in oldIEs via HTML5Shiv v3 (http://code.google.com/p/html5shiv/)
 * HTML5 doctype and meta content-type
 * Blocks marked up with section elements
 * Search form uses the new input type="search" attribute
 * WAI-ARIA accessibility roles added to primary elements
 * Many extra body and node classes for easy theming
 * Responsive menus (thanks to https://github.com/joggink/jquerymobiledropdown)
 * Optional blueprint grid system integration, no more vendor prefixes, simple IE fixes, and many more - all thanks to compass (http://compass-style.org/blog/2011/04/24/v011-release/)
 * Grid background "image", for easy element aligning, made with CSS3 and SASS to fit every grid you can imagine. 
 * Draggable overlay image you can lay over your HTML for easy visual comparison. you may also set different overlay opacity values.
 * Bi-directionality support for RTL and LTR (Right-To-Left and Left-To-Right) - Sasson can actually auto-flip your scss, you only have to @flip it.


INSTALLATION
----------------------

 * Bad way - Extract the theme in your sites/all/themes/ directory, enable it and start hacking

 * Good way - 

      * Extract the theme in your sites/all/themes/ directory
      * Move SUBTHEME into its own folder in your themes directory
      * Optional but recommended - Rename at least your folder and .info file
      * Enable your sub-theme and start hacking

 * Even better - you can use drush to create your sub-theme(s) - 

      # drush sns "My theme"


USEFUL INFORMATION
----------------------------------

 * Out of the box Sasson will give you a 960 pixel grid system, you may change grid properties in your theme settings

 * Sasson gives you a responsive layout - that means your site adapts itself to the browser it is displayed on. you may set the layout breakpoints or disable this behaviour via theme settings
 
 * The default responsive layout takes a desktop-first approach, you can go mobile-first with a click in your theme settings.
 
 * While you develop, you should keep the development mode turned on (see theme settings page), this will compile your SASS on every page load, and will give you FireSASS support (https://addons.mozilla.org/en-US/firefox/addon/firesass-for-firebug/). 

 * When not developing, turn development mode off, this will keep your CSS output light as a feather, in fact, the output of our semantic version of 960gs is much slimmer then the original css grid system.

 * Sasson allows you to write CSS3 properties (like 'border-radius', 'box-shadow' etc.) in the standard form, vendor specific prefixes will be added for you. see hook_prefixes_alter() if you want to add more.

 * Sasson passes variables from the theme settings form and into the sass compiler, you can do this in your sub-theme as well, see hook_sasson_alter().

 * Sasson will force latest IE rendering engine (even in intranet) & Chrome Frame, you may disable that via theme settings

 * Sasson will set mobile viewport initial scale to 100%. with a responsive layout, this will give your mobile users the best experience, no need to zoom on every page load, you may disable that via theme settings
 
 * sass/scss files are compiled to css files with the same name, when manually creating multiple sub-themes, you should avoid having two sass/scss files with the same name because they will override each other, if using drush sns to create sub-theme we take care of that for you.
 
 * When loading style-sheets in your .info file Sasson allows you to specify settings like media queries, browsers, weight and any option available to drupal_add_css(), for example :

		styles[styles/sasson.scss][options][weight] = 1
		styles[styles/sasson.scss][options][media] = screen and (max-width:400px)
		styles[styles/sasson.scss][options][browsers][IE] = lte IE 7
		styles[styles/sasson.scss][options][browsers][!IE] = FALSE

 This will load sasson.scss with an extra weight for screen only (not print) on browsers wider then 400px and on IE7 or older only, you get the point.

 * Sasson applies classes to the <html> tag to enable easier styling for Internet Exporer :

   - html.ie9 #selector { ... } /* IE9 only rules */
   - htm.lte-ie8 #selector { ... } /* IE8 and below rules */

 * Bi-directionality - Many sites need to have both LTR (left-to-right e.g. English) and RTL (right-to-left e.g. Hebrew, Arabic) support for different sections of the site. Drupal allows you to add '-rtl' to your CSS filenames for style sheets that will be loaded for RTL pages only. Sasson allows you to use '-ltr' too (e.g. mytheme-ltr.scss) for files that will load on LTR pages only.

 * New feature: Auto Directionality Flipping - You can simply put '@flip {file: "filename.scss"};' in your 'filename-rtl.scss' or 'filename-ltr.scss' file and Sasson will replace this line with a flipped (RTLed or LTRed) version of 'filename.scss'. if needed you are free to insert fixes to the auto-generated code right after this line. (see example and check the output of sasson-rtl.scss to see it in action)


KNOWN ISSUES
------------------------

* Our Sass compiler works really well (and constantly improving) but the compass version included is quite old, if you need the latest compass features we suggest using Sasson with the native Ruby compiler (http://compass-style.org/install/).


AUTHORS
--------------

* Tsachi Shlidor (tsi) - http://drupal.org/user/322980 & http://rtl-themes.co.il
* Raz konforti (konforti) - http://drupal.org/user/99548
* And many others...


SPONSORS
----------------

This project is made possible by :

* Linnovate (http://linnovate.net)
