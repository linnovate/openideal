<?php

/**
 * @file
 * Hooks provided by the Sasson theme.
 */

/**
 * Allows altering the data (string) of SASS or SCSS file
 * just before it's processed by the PhamlP parser.
 *
 * @param &$data
 *   The SASS or SCSS file content of $file (string) 
 *   that is going to be processed by the PhamlP parser.
 */
function hook_sasson_alter(&$data) {
  // Replaces '[oranges]' with 'apples' and '[apples]' with 'oranges'.
  $variables = array(
    '[oranges]' => 'apples',
    '[apples]' => 'oranges',
  );

  $data = str_replace(array_keys($variables), $variables, $data);
}

/**
 * Allows altering the data (string) of SASS or SCSS file
 * right after it comes back from the PhamlP parser.
 *
 * @param &$data
 *   The SASS or SCSS file content of $file (string) 
 *   that is returned from the PhamlP parser.
 */
function hook_sasson_post_parse_alter(&$data) {
  // Replaces '[oranges]' with 'apples' and '[apples]' with 'oranges'.
  $variables = array(
    '[oranges]' => 'apples',
    '[apples]' => 'oranges',
  );

  $data = str_replace(array_keys($variables), $variables, $data);
}


/**
 * Allows altering the vendor prefixes used for generating CSS3 properties used in your SASS or SCSS file
 * This way a property need only be written in the standard form and vendor specific versions will be added to the style sheet.
 *
 * @param &$pref
 *   An array listing all vendor prefixes
 */
function hook_prefixes_alter(&$pref) {
  $pref = array(
    'border-radius' => array(
      '-moz-border-radius',
      '-webkit-border-radius',
      '-khtml-border-radius'
    ),
    'border-top-right-radius' => array(
      '-moz-border-radius-topright',
      '-webkit-border-top-right-radius',
      '-khtml-border-top-right-radius'
    ),
    'border-bottom-right-radius' => array(
      '-moz-border-radius-bottomright', 
      '-webkit-border-bottom-right-radius',
      '-khtml-border-bottom-right-radius'
    ),
    'border-bottom-left-radius' => array(
      '-moz-border-radius-bottomleft',
      '-webkit-border-bottom-left-radius',
      '-khtml-border-bottom-left-radius'
    ),
    'border-top-left-radius' => array(
      '-moz-border-radius-topleft',
      '-webkit-border-top-left-radius',
      '-khtml-border-top-left-radius'
    ),
    'box-shadow' => array(
      '-moz-box-shadow', 
      '-webkit-box-shadow'
    ),
    'box-sizing' => array(
      '-moz-box-sizing', 
      '-webkit-box-sizing'
    ),
    'opacity' => array(
      '-moz-opacity', 
      '-webkit-opacity', 
      '-khtml-opacity'
    ),
    'transition' => array(
      '-webkit-transition', 
      '-moz-transition', 
      '-ms-transition', 
      '-o-transition'
    ),
    'transform' => array(
      '-webkit-transform', 
      '-moz-transform', 
      '-ms-transform', 
      '-o-transform'
    ),
  );
}
