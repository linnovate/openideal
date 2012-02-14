// $Id: README.txt,v 1.2.2.2 2009/06/20 19:25:54 dvessel Exp $

This "framework" theme is based on the 960 Grid System created by Nathan Smith.
It is being proposed for the core base theme in Drupal 7. The CSS framework is
licenced under GPL and MIT. This Drupal theme is licensed under GPL.

See these pages:
  Issue queue:       http://drupal.org/node/293540
  g.d.o discussion:  http://groups.drupal.org/node/16200
  960 post on g.d.o: http://groups.drupal.org/node/16457

Homepage and download for 960:
  http://960.gs/

Background information on 960:
  http://sonspring.com/journal/960-grid-system

Write-up from DivitoDesign:
  http://www.divitodesign.com/2008/12/960-css-framework-learn-basics/

Write-up from Nettus:
  http://nettuts.com/videos/screencasts/a-detailed-look-at-the-960-css-framework/
  http://nettuts.com/tutorials/html-css-techniques/prototyping-with-the-grid-960-css-framework/

General idea behind a CSS framework:
  http://www.alistapart.com/articles/frameworksfordesigners

Extensive overview on working within grids:
  http://www.smashingmagazine.com/2007/04/14/designing-with-grid-based-approach/

==============================================================================
Modifications:

- The 960 Grid System package has been modified so it conforms to Drupal's
  coding standards. Tabs were removed for double spaces and underscores
  changed to hyphens.

- Additional .push-x and .pull-x classes have been added.

- Right-to-left languages are supported. It's not part of the 960 download.
  It has been extended to support Drupal's rtl language system.

- Removed ".clear-fix" and ".clear" classes from 960.css. Drupal works with
  ".clear-block" which uses the same technique used in .clear-fix. The .clear
  class on the other hand is too commonly used as a class name and the
  properties can cause confusion since anything with that property will
  disappear. A standard <br /> tag can be used in its place.

- Removed the "outline:0;" rule from reset.css. Adding it back manually
  prevents OS specific outline styles from being used. Specifically Webkit and
  possibly others. Also removed the "a:focus {outline: 1px dotted invert;}"
  from text.css where it's reapplied.

- Removed the large left margin for list items inside text.css. It interferes
  in many places.

==============================================================================
Notes and rules to play nice with the grids:

- The class .container-[N] ([N] being a numeric value) is a subdivision of the
  overall width (960 pixels). It can either be .container-12 or .container-16.
  Depending on which is used, each grid unit (.grid-[N] class) will either be
  in multiples of 80 pixels for 12 subdivisions or in multiple of 60 pixels for
  16 subdivisions. All grid blocks include a 10 pixel margin on the left and
  right side.

- Add a .show-grid class to the body tag to see the grid. It will add a
  background graphic to guide you. This theme includes a printable sketch
  sheet and templates for Photoshop and various other formats to guide you
  from start to finish. See the "extras" folder within this theme.

- Do not add left or right margins or padding to anything that's assigned a
  grid class or it may throw off the alignment.

- Use the .push-[N] and .pull-[N] classes so the order the layout is presented
  does not depend on source order. For example, if the source order was this:
  [content][side-1][side-2], and the desired presentation order was this:
  [side-1][content][side-2]. Adding a .pull-[N] class to #side-1 with the same
  numeric grid value as #content and adding a .push-[N] class to #content with
  the same numeric grid value of #side-1 will swap their display. These classes
  can also be used for the general shifting of content when needed.

  <div class="container-16">
    <div id="content" class="grid-9 push-4">
      ...                         \  /
    </div>                         \/ Match numeric values to swap.
                                   /\
    <div id="side-1"  class="grid-4 pull-9">
      ...
    </div>

    <div id="side-2" class="grid-3">
      ...
    </div>
  </div>

  Any element assigned a .push-[N] or .pull-[N] class should also have a
  .grid-[N] class. As an alternative, a "position:relative;" rule can be
  applied to the element.

- Use the .prefix-[N] and .suffix-[N] classes if you need to fill empty space.
  Do not confuse this with the .push/pull-[N] classes which can overlap
  adjacent areas.

- Right to left languages will have the page reflow automatically. See
  960-rtl.css inside the "styles" folder. As long as the theme uses the
  positioning classes from this framework, it will be automatic.

- When embedding a grid within a grid, use .alpha and .omega classes. The
  first block must be assigned .alpha and the last, .omega. This will chop off
  the 10px margin so it fits neatly into the grid. This depends more on the
  visual order than the source order when using the push/pull classes.

As long as you stay within the framework, any browser positioning quirks
_especially from IE_ should be minimal. 

==============================================================================
To-dos:

- * Update: see next section *
  Helper function to calculate grid classes based on context. The standard
  $body_classes within page.tpl.php which contains the state of the sidebars
  will be less useful in this grid system. It's often used to change the width
  of the content region depending on the presence of sidebars. This new helper
  function will need to replace that functionality. It also gives us an
  opportunity to have a more flexible system. Instead of focusing on sidebars,
  it can be more fine grained in its awareness of adjacent grids and push out
  classes appropriately. This helper function must be as simple as possible to
  *work with*.

- Separate issue but this can help add grid classes easier.
  Add $classes to hook templates: http://drupal.org/node/306358

==============================================================================
Adding classes based on context:

The function "ns()" can be used to add classes contextually. The first argument
should be the default class with the highest possible unit. The rest of the
arguments are paired by a variable and its unit used in an adjacent block of
markup.


<div class="container-16">
  <div id="main" class="<?php print ns('grid-16', $neighbor_a, 3, $neighbor_b, 4); ?>">
    <?php print $main; ?>              [default]      |        |      |        |
  </div>                                              |- pair -|      |- pair -|
                                                      |        |      |        |
  <?php if ($neighbor_a): ?> <!-----------------------/        |      |        |
  <div id="neighbor-a" class="grid-3"> <!----------------------/      |        |
    <?php print $neighbor_a; ?>                                       |        |
  </div>                                                              |        |
  <?php endif; ?>                                                     |        |
                                                                      |        |
  <?php if ($neighbor_b)?> <!-----------------------------------------/        |
  <div id="neighbor-b" class="grid-4"> <!--------------------------------------/
    <?php print $neighbor_b; ?>
  </div>
  <?php endif; ?>
</div>

  |------------ .container-16 -----------------------|
  |---------------------------|---------|------------|
  |                           |         |            |
  |              #main.grid-9 <.grid-12 <.grid-16    |
  |                        -7 |      -4 |[default]   |
  |---------------------------|---------|------------|


Note that the *default class* (first parameter) is the largest possible of
"grid-16" since it's the immediate child of "container-16". The variables
$neighbor_a and $neighbor_b can be any variable that exists inside a template.
The number arguments immediately after the variable argument is it's default
grid value placed in their own markup.

The function only checks if the variable contains anything and subtracts the
next numeric parameter from the default class. With the above example, if
$neighbor_a contains anything it will subtract "3" from "grid-16". Same with
$neighbor_b which will subtract "4". If both variables exists, "grid-9" will
output to make space for the adjacent areas.

There are no limits to the number of parameters but it must always be done in
pairs. The first part of the pair being the variable and second, the number to
subtract from the default class. This pairing excludes the first parameter.

If needed, subtraction can take place when the variable *is empty* by using
an exclamation mark before it. This is useful for pull/push and prefix/suffix
classes in some contexts.


<div class="container-16">
  <div id="main" class="grid-10 <?php print ns('suffix-6', !$neighbor_c, 6); ?>">
    ...                                        [default]        |        |
  </div>                                                        |- pair -|
                                                                |        |
  <?php if ($neighbor_c): ?> <!---------------------------------/        |
  <div id="neighbor-c" class="grid-6"> <!--------------------------------/
    <?php print $neighbor_c; ?>
  </div>
  <?php endif; ?>
</div>

  |--------------- .container-16 --------------------|
  |------------------------------|-------------------|
  |                              |                   |
  |                #main.grid-10 >.suffix-6          |
  |                              |[padding fill]     |
  |------------------------------|-------------------|


The main points to remember are these:
  - The first parameter (default class) starts at the largest value. The latter
    parameters always work to subtract from the first.
  - The first parameter can be any type of class. As long as it's delimited by
    a hyphen "-" and ends in a numeric. (grid-[N], pull-[N], suffix-[N], etc.)
  - Starting from the second parameter, variables and numeric parameters must
    be set in pairs.
  - If the variable contains a value, subtraction will occur by default. Use an
    exclamation mark before the variable to subtract when it *does not* contain
    a value.

==============================================================================
Problem with .clearfix and .clear-block in IE:

The .clearfix (aka .clear-block) method of clearing divs does not always play
well with 960.gs in IE6 (haven't tried IE7). The problem is twofold:

   1. If a div is assigned a container-X class, you must add the clearfix class
      after it, like so:
      <div class="container-12 clearfix">

   2. If a div is assigned a grid-X class, adding clearfix will break your
      layout. You must used an "inner" div instead, like so:
      <div class="grid-8"><div class="clearfix">My content</div></div>

This problem has been seen in IE6. IE7 has not yet been tested. Please see
this issue for more information: http://drupal.org/node/422240

==============================================================================

If you have any questions or suggestions. Please post in the group discussion.
http://groups.drupal.org/node/16457 or contact me @ joon at dvessel dot com.

