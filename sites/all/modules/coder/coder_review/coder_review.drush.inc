<?php

/**
 * @file
 * Command line utility for coder_review.
 */

/**
 * Implements hook_drush_help().
 */
function coder_review_drush_help($section) {
  switch ($section) {
    case 'drush:coder-review':
      return dt("usage: drush coder-review [options] [severity] [review] [what]"
                . "\n  options:"
                . "\n    summary - display summary information only, no warning output"
                . "\n    no-empty - hide results that are empty"
                . "\n    xml - output results as xml"
                . "\n    checkstyle - output results in Checkstyle xml format"
                . "\n  severity: show warnings at or above severity level"
                . "\n     major|minor|critical"
                . "\n  review: one or more reviews"
                . "\n     upgrade7x|comment|i18n|security|sql|style"
                . "\n       * style is the default"
                . "\n  what:"
                . "\n    all - review all modules and themes"
                . "\n      * this is the default if nothing is explicitly specified"
                . "\n    active - review all active modules and themes"
                . "\n    core - review all of core, modules, themes, and includes"
                . "\n    contrib - review all non-core modules"
                . "\n    default - review 'default' modules and themes, as defined by settings"
                . "\n    <name> - review specified module or theme name, i.e. taxonomy"
                . "\n    no-<name> - exclude named modules and themes, i.e. no-taxonomy"
                . "\n     combine with active|core|contrib|all|default"
                . "\n    <patch-url> - URL to patch file"
                . "\n    <file-path> - path to file or multiple files, relative to Drupal installation"
    );
  }
}

/**
 * Implements hook_drush_command().
 */
function coder_review_drush_command() {
  $items['coder-review'] = array(
    'callback' => 'coder_review_drush_review',
    'description' => dt('Run code reviews'),
    'drupal dependencies' => array('coder_review'),
  );
  return $items;
}

function _coder_review_set_form_modules(&$settings) {
  if (isset($settings['coder_modules'])) {
    foreach ($settings['coder_modules'] as $key => $value) {
      if ($value == 1) {
        $settings['coder_modules-' . $key] = 1;
      }
    }
    unset($settings['coder_modules']);
  }
}

/**
 * Do the actual review.
 */
function coder_review_drush_review() {
  _coder_review_drush_set_option('drush');

  // Process command line arguments.
  $args = func_get_args();
  $reviews = array();
  $modules = array();
  $output = array();
  $settings = _coder_review_get_default_settings();
  if (count($args)) {
    $avail_reviews = _coder_review_reviews();
    foreach ($args as $arg) {
      switch ($arg) {
        case 'summary':
        case 'no-empty':
        case 'xml':
        case 'checkstyle':
          _coder_review_drush_set_option($arg);
          break;
        case 'active':
        case 'core':
        case 'contrib':
        case 'all':
        case 'default':
          $settings = _coder_review_get_default_settings($arg);
          $settings['coder_includes'] = TRUE;
          break;
        case 'major':
        case 'minor':
        case 'critical':
          $severity_name = $arg;
          break;
        default:
          if (isset($avail_reviews[$arg])) {
            $reviews[$arg] = $arg;
          }
          elseif (drupal_substr($arg, 0, 3) == 'no-') {
            _coder_review_set_form_modules($settings);
            unset($settings['coder_modules-' . drupal_substr($arg, 3)]);
          }
          elseif (strpos($arg, '*') !== FALSE || strpos($arg, '%') !== FALSE) {
            $result = db_query('SELECT name FROM {system} WHERE name LIKE :name', array(':name' => str_replace('*', '%', $arg)));
            foreach ($result as $system) {
              $settings['coder_modules-' . $system->name] = 1;
              $match = TRUE;
            }
            if (!isset($match)) {
              _coder_review_drush_print(dt('no matches found for @name', array('@name' => $arg)));
              return;
            }
            unset($settings['coder_active_modules']);
            unset($settings['coder_core']);
            unset($settings['coder_all']);
            unset($settings['coder_modules']);
            $settings['coder_includes'] = TRUE;
          }
          else {
            if (file_exists($arg)) {
              $settings['coder_files'] = 1;
              if (empty($settings['coder_file_list'])) {
                $settings['coder_file_list'] = '';
              }
              $settings['coder_file_list'] .= $arg . "\n";
            }
            elseif (_coder_review_drush_is_patch_arg($arg)) {
              $settings['coder_patches'] = 1;
              $settings['coder_patch_link'] = $arg;
            }
            else {
              $settings['coder_modules-' . $arg] = 1;
              $settings['coder_includes'] = TRUE;
            }
            unset($settings['coder_active_modules']);
            unset($settings['coder_core']);
            unset($settings['coder_all']);
            unset($settings['coder_modules']);
          }
          break;
      }
    }
    if (_coder_review_drush_is_option('checkstyle')) {
      _coder_review_drush_xml_output_header('checkstyle');
    }
    elseif (_coder_review_drush_is_option('xml')) {
      _coder_review_drush_xml_output_header('xml');
    }
    if (isset($severity_name)) {
      if (_coder_review_drush_is_option('xml')) {
        _coder_review_drush_xml_output_severity($severity_name);
      }
      $output[] = dt('Severity @severity_name', array('@severity_name' => $severity_name));
      $settings['coder_severity'] = _coder_review_severity($severity_name);
    }
    if (count($reviews)) {
      if (_coder_review_drush_is_option('xml') && !_coder_review_drush_is_option('checkstyle')) {
        _coder_review_drush_xml_output_reviews($reviews, $avail_reviews);
      }
      else {
        foreach ($reviews as $review) {
          $output[] = $avail_reviews[$review]['#title'];
        }
      }
      $settings['coder_reviews'] = $reviews;
    }
    if (count($output) && !_coder_review_drush_is_option('summary') && !_coder_review_drush_is_option('xml') && !_coder_review_drush_is_option('checkstyle')) {
      _coder_review_drush_print(implode(', ', $output) . "\n");
    }
  }
  _coder_review_set_form_modules($settings);
  $settings['op'] = 'drush';

  $form_state['storage'] = $settings;
  coder_review_page_form(array(), $form_state);

  if (_coder_review_drush_is_option('checkstyle')) {
    _coder_review_drush_xml_output_footer('checkstyle');
  }
  elseif (_coder_review_drush_is_option('xml')) {
    _coder_review_drush_xml_output_footer('xml');
  }
}

/**
 * Handle patch reviews.
 */
function _coder_review_drush_is_patch_arg(&$arg) {
  if (drupal_substr($arg, 0, 7) == 'http://' || is_file(realpath($arg))) {
    return TRUE;
  }
  // @NOTE: relies on http://drupal.org/node/297611
  if (isset($_SERVER['OLDPWD'])) {
    $path = $_SERVER['OLDPWD'] . '/' . $arg;
    if (is_file($path)) {
      $arg = $path;
      return TRUE;
    }
  }
}

/**
 * Theme printing of results and filenames.
 */
function theme_drush_coder_review($variables) {
  $name = $variables['name'];
  $filename = $variables['filename'];
  $results = $variables['results'];
  if (!_coder_review_drush_is_option('summary') && !empty($results) && ((count($results) && !isset($results[0])) || !_coder_review_drush_is_option('no-empty'))) {
    if (_coder_review_drush_is_option('checkstyle') || _coder_review_drush_is_option('xml')) {
      _coder_review_drush_xml_output_results($filename, $results);
    }
    else {
      _coder_review_drush_print($filename . ":\n " . implode("\n ", $results) . "\n");
    }
  }
}

/**
 * Theme warning message, including source snippet.
 */
function theme_drush_coder_review_warning($variables) {
  $warning = $variables['warning'];
  $severity_name = $variables['severity_name'];
  $lineno = isset($variables['lineno']) ? $variables['lineno'] : 0;
  $line = isset($variables['line']) ? $variables['line'] : '';
  $checkstyle_levels = array(
    'minor' => 'info',
    'normal' => 'warning',
    'critical' => 'error',
  );
  if (_coder_review_drush_is_option('xml') || _coder_review_drush_is_option('checkstyle')) {
    $attr = array(
      'line' => $lineno,
      'column' => 0,
      'severity' => $severity_name,
      'message' => $warning,
      'source' => $line,
    );
    if (_coder_review_drush_is_option('checkstyle')) {
      $attr['severity'] = $checkstyle_levels[$severity_name];
    }
    $output = '<error '. drupal_attributes($attr) . ' />';
    return $output;
  }
  else {
    $output  = $lineno ? '+' . $lineno . ': ' : '';
    $output .= '[' . $severity_name . '] ';
    $output .= is_array($warning) ? $warning['#warning'] : $warning;
    return _coder_review_drush_output($output);
  }
}

/**
 * Handle printing of drupal_set_message() messages.
 */
function coder_review_print_drush_messages() {
  foreach (drupal_get_messages() as $type => $messages) {
    $output = _coder_review_drush_output(implode("\n ", $messages));
    if (_coder_review_drush_is_option('xml') && !_coder_review_drush_is_option('checkstyle')) {
      _coder_review_drush_print('<status type="' . $type . '">' . $output . '</status>');
    }
    elseif (!_coder_review_drush_is_option('checkstyle')) {
      _coder_review_drush_print(dt(drupal_ucfirst($type) . ' Messages') . ":\n " . $output . "\n");
    }
  }
}

/**
 * Strip HTML tags from drush output.
 */
function _coder_review_drush_output($output) {
  return html_entity_decode(strip_tags($output));
}

/**
 * Helper function to set a command line option.
 */
function _coder_review_drush_set_option($option) {
  global $_coder_drush_options;
  if (!isset($_coder_drush_options)) {
    $_coder_drush_options = array();
  }
  $_coder_drush_options[$option] = TRUE;
}

/**
 * Helper function to determine if an option is set.
 */
function _coder_review_drush_is_option($option) {
  global $_coder_drush_options;
  return isset($_coder_drush_options[$option]);
}

/**
 * Switch between print and pipe print output.
 */
function _coder_review_drush_print($message) {
  if (drush_get_context('DRUSH_PIPE')) {
    drush_print_pipe($message);
  }
  else {
    drush_print($message);
  }
}




/**
 * XML output functions.
 */

/**
 * XML: output appropriate header tag.
 */
function _coder_review_drush_xml_output_header($type = 'xml') {
  // Put in an extra concatenation so syntax highlighting in vim doesn't break.
  _coder_review_drush_print('<?xml version="1.0" encoding="UTF-8"?' . '>');
  switch ($type) {
    case 'checkstyle':
      _coder_review_drush_print('<checkstyle version="1.3.0RC1">');
      break;
    case 'xml':
    default:
      _coder_review_drush_print('<coderreview version="1.1">');
      break;
  }
}

/**
 * XML: output appropriate footer tag.
 */
function _coder_review_drush_xml_output_footer($type = 'xml') {
  switch ($type) {
    case 'checkstyle':
      _coder_review_drush_print('</checkstyle>');
      break;
    case 'xml':
    default:
      _coder_review_drush_print('</coderreview>');
      break;
  }
}

/**
 * XML: output severity level if provided.
 */
function _coder_review_drush_xml_output_severity($severity_name) {
  _coder_review_drush_print('<severity>' . $severity_name . '</severity>');
}

/**
 * XML: output list of chosen reviews.
 */
function _coder_review_drush_xml_output_reviews($reviews, $avail_reviews) {
  _coder_review_drush_print('<reviews>');
  foreach ($reviews as $review) {
    _coder_review_drush_print('<review>' . $avail_reviews[$review]['#title'] . '</review>');
  }
  _coder_review_drush_print('</reviews>');
}

/**
 * XML: output filename and its results (if any).
 */
function _coder_review_drush_xml_output_results($filename, $results) {
  if (empty($results) || (count($results) == 1 && isset($results[0]))) {
    _coder_review_drush_print('<file name = "' . $filename . '" />');
  }
  else {
    _coder_review_drush_print('<file name = "' . $filename . '">' . "\n" . implode("\n ", $results) . "\n" . '</file>');
  }
}

