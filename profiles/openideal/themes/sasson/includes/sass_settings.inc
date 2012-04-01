<?php
/**
 * Implements hook_sasson_alter().
 *
 * This is how we pass variables from php to sass
 */
function sasson_sasson_alter(&$data) {
  $variables = array(
    // Grid settings
    '[show-grid]' => theme_get_setting('sasson_show_grid') == 1 ? '1' : '0',
    '[grid-width]' => theme_get_setting('sasson_grid_width'),
    '[columns]' => theme_get_setting('sasson_columns'),
    '[gutter-width]' => theme_get_setting('sasson_gutter_width'),
    '[sidebar-first]' => theme_get_setting('sasson_sidebar_first'),
    '[sidebar-second]' => theme_get_setting('sasson_sidebar_second'),
    // Responsive layouts
    '[responsive]' => theme_get_setting('sasson_responsive') == 1 ? 'true' : 'false',
    '[desktop-first]' => theme_get_setting('sasson_responsive_approach') == 'desktop_first' ? 'true' : 'false',
    '[mobile-first]' => theme_get_setting('sasson_responsive_approach') == 'mobile_first' ? 'true' : 'false',
    '[narrow]' => theme_get_setting('sasson_responsive_narrow'),
    '[narrower]' => theme_get_setting('sasson_responsive_narrower'),
    '[narrowest]' => theme_get_setting('sasson_responsive_narrowest'),
    '[small]' => theme_get_setting('sasson_responsive_mf_small'),
    '[medium]' => theme_get_setting('sasson_responsive_mf_medium'),
    '[large]' => theme_get_setting('sasson_responsive_mf_large'),
    // Fonts
    '[font-face]' => theme_get_setting('sasson_font') ? theme_get_setting('sasson_font') : 'false',
    '[font-fallback]' => theme_get_setting('sasson_font_fallback') ? ', ' . theme_get_setting('sasson_font_fallback') : '',
    '[font-selectors]' => theme_get_setting('sasson_font_selectors') ? theme_get_setting('sasson_font_selectors') : 'body',
    // Overlay image
    '[overlay]' => theme_get_setting('sasson_overlay') == 1 ? '1' : '0',
  );

  $data = str_replace(array_keys($variables), $variables, $data);
}

/**
 * Implements hook_sasson_post_parse_alter().
 */
function sasson_sasson_post_parse_alter(&$data) {
  // Break the file data into lines.
  $lines = preg_split('/$\R?^/m', $data);
  foreach ($lines as $k => $line) {
    // Look for @flip command.
    if (strpos($line, '@flip') !== FALSE) {
      $s = explode('"', $line);
      $file = trim($s[1]);
      // Replace the command line with the RTLer returns
      $output = '/** Starting Auto-Flip **/';
      $output .= sasson_flip($file);
      $output .= '/** Ending Auto-Flip **/';
      $lines[$k] = $output;
    }
  }
  // Build back the data out of the lines
  $data = implode("\n", $lines);
}

function sasson_flip($fname) {
  $pp = explode("/", $fname);
  $last = end($pp);
  $fp = explode(".", $last);
  $name = $fp[0];
  $ext = $fp[1];
  $directory = 'public://css';
  
  // If it's scss file, lets check for the compile css file in the public files directory.
  if ($ext == 'scss' || $ext == 'sass') {
    $path = "{$directory}/{$name}.css" ;
  }
  // If it's a css file, use the exact path to file.
  // @TODO: check how to pass the path to file.
  //  elseif ($ext == 'css') {
  //    $path = $fname;
  //  }

  if (isset($path) && file_exists($path)) { 
    require_once dirname(__FILE__) . '/flipper.inc';
    
    // Open and read the file.
    $c = file_get_contents($path);
    // create RTL object
    $Flipper = new Flipper;

    // do our job! LOL
    $flipped = $Flipper->parse_css($c);
    return $flipped;
  }
}
/**
 * Implements hook_prefixes_alter().
 *
 * This is how we define multiple vedor prefixes for our css
 */
function sasson_prefixes_alter(&$pref) {
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

