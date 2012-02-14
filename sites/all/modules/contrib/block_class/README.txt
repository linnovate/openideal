=====
Block Class
http://drupal.org/project/block_class
-----
Block Class was developed and is maintained by Four Kitchens <http://fourkitchens.com>.


=====
Installation
-----

1. Enable the module
2. Insert some code into your theme (see the two methods below)
3. To add a class to a block, simply visit that block's configuration page at Admin > Site Building > Blocks


=====
Method 1: Using the module's block_class() function
-----

You will need to add this snippet to your theme's block.tpl.php inside the block's class definition:

<?php print block_class($block); ?>

Here's the first line of the Garland theme's block.tpl.php prior to adding the code:

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">

And here's what the code should look like after adding the snippet:

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?> <?php print block_class($block); ?>">

IMPORTANT: Remember to separate the PHP snippet from the existing markup with a single space. If you don't add the space, your CSS classes could run together like this: "block-modulecustomclass" instead of "block-module customclass".

-----
Checking if the function block_class() exists before calling it
-----

If there's a chance you may disable the Block Class module, you should consider placing the PHP snippet inside a conditional statement that checks to make sure the function actually exists before calling it: <?php if (function_exists(block_class)) print block_class($block); ?>. This will prevent a nasty "Call to undefined function" error.

Why use function_exists() instead of Drupal's handy module_exists() function? It's faster!


=====
Method 2: Using the $block_classes variable and template_preprocess_block()
-----
Step 1
-----
Add this snippet to your theme's block.tpl.php inside the block's class definition:

<?php print $block_classes; ?>

Here's the first line of the Garland theme's block.tpl.php prior to adding the code:

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?>">

And here's what the code should look like after adding the snippet:

<div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="clear-block block block-<?php print $block->module ?> <?php print $block_classes; ?>">

IMPORTANT: Remember to separate the PHP snippet from the existing markup with a single space. If you don't add the space, your CSS classes could run together like this: "block-modulecustomclass" instead of "block-module customclass".

-----
Step 2
-----
Modify your theme's template.php to include an implementation of template_preprocess_block(). This preprocess function should define the variable $block_classes. Here's an example:

/**
 * Implementation of template_preprocess_block().
 * 
 * The following is an example of how to automatically add classes to your
 * blocks by adding a $block_classes variable to to block.tpl.php
 */
function YOURTHEME_preprocess_block(&$variables) {
  if (isset($variables['block_class'])) {
    $variables['block_classes'] .= ' '. block_class($block);
  }
  else {
    $variables['block_classes'] = block_class($block);
  }
}
