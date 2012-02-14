<?php

/**
 * @file
 * This include file implements coder_review functionality for 4.7 -> 5.x upgrades.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_5x_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]user_mail\s*\(',
      '#warning_callback' => '_coder_review_5x_user_mail_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]user_mail_wrapper\s*\(',
      '#warning_callback' => '_coder_review_5x_user_mail_wrapper_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]message_na\s*\(',
      '#warning_callback' => '_coder_review_5x_message_na_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]module_exist\s*\(',
      '#warning_callback' => '_coder_review_5x_module_exist_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_call_js\s*\(',
      '#warning_callback' => '_coder_review_5x_drupal_call_js_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]system_listing\s*\(',
      '#warning_callback' => '_coder_review_5x_system_listing_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_get_name\s*\(',
      '#warning_callback' => '_coder_review_5x_node_get_name_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '\$_POST\s*\[\s*[\'"]?op[\'"]?\s*\]',
      '#warning' => '$_POST[\'op\'] deprecated in favor of $form_values[\'op\']',
      '#severity' => 'normal',
    ),
    array(
      '#type' => 'callback',
      '#value' => '_coder_review_5x_callback',
      '#warning_callback' => '_coder_review_5x_callback_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '[\s\(]theme_add_style\s*\(',
      '#warning_callback' => '_coder_review_5x_theme_add_style_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '[\s\(]form_render\s*\(',
      '#warning_callback' => '_coder_review_5x_form_render_warning',
    ),
  );
  $review = array(
    '#title' => t('Converting 4.7.x modules to 5.x'),
    '#link' => 'http://drupal.org/node/64279',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade5x' => $review);
}

/**
 * Define the rule callbacks for 5.x, see do_coder_review_review_callback().
 */
function _coder_review_5x_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // Only perform this check on module's (not includes).
  $filename = $coder_args['#filename'];
  if (drupal_substr($filename, -7) == '.module') {
    // Make sure that a .info file exists.
    $filename = drupal_substr(realpath($filename), 0, -7) . '.info';
    if (!file_exists($filename)) {
      $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
      _coder_review_error($results, $rule, $severity_name);
    }
  }
}

/**
 * Define the warning callbacks.
 */

function _coder_review_5x_callback_warning() {
  return t('All modules now need to have a <a href="@info">modulename.info file</a>', array('@info' => 'http://drupal.org/node/101009'));
}

function _coder_review_5x_user_mail_warning() {
  return t('!user_mail() is replaced by !drupal_mail()',
    array(
      '!user_mail' => theme('drupalapi', array('function' => 'user_mail', 'version' => '4.7')),
      '!drupal_mail' => theme('drupalapi', array('function' => 'drupal_mail', 'version' => '5')),
    )
  );
}

function _coder_review_5x_user_mail_wrapper_warning() {
  return t('!user_mail_wrapper() changed to !drupal_mail_wrapper()',
    array(
      '!user_mail_wrapper' => theme('drupalapi', array('function' => 'user_mail_wrapper', 'version' => '4.7')),
      '!drupal_mail_wrapper' => theme('drupalapi', array('function' => 'drupal_mail_wrapper', 'version' => '5')),
    )
  );
}

function _coder_review_5x_message_na_warning() {
  return t('The function !message_na() was removed, remove it from your modules as well and replace it with !t(\'n/a\')',
    array(
      '!message_na' => theme('drupalapi', array('function' => 'message_na', 'version' => '4.7')),
      '!t' => theme('drupalapi', array('function' => 't', 'version' => '5')),
    )
  );
}

function _coder_review_5x_module_exist_warning() {
  return t('!module_exist() is now !module_exists()',
    array(
      '!module_exist' => theme('drupalapi', array('function' => 'module_exist', 'version' => '4.7')),
      '!module_exists' => theme('drupalapi', array('function' => 'module_exists', 'version' => '5')),
    )
  );
}

function _coder_review_5x_drupal_call_js_warning() {
  return t('Remove !drupal_call_js(), use "!drupal_add_js(\'myCustomFunction(your, parameters, here)\', \'inline\');" instead',
    array(
      '!drupal_call_js' => theme('drupalapi', array('function' => 'drupal_call_js', 'version' => '4.7')),
      '!drupal_add_js' => theme('drupalapi', array('function' => 'drupal_add_js', 'version' => '5')),
    )
  );
}

function _coder_review_5x_system_listing_warning() {
  return t('!system_listing() is now !drupal_system_listing()',
    array(
      '!system_listing' => theme('drupalapi', array('function' => 'system_listing', 'version' => '4.7')),
      '!drupal_system_listing' => theme('drupalapi', array('function' => 'drupal_system_listing', 'version' => '5')),
    )
  );
}

function _coder_review_5x_theme_add_style_warning() {
  return t('Replace !theme_add_style() with !drupal_add_css()',
    array(
      '!theme_add_style' => theme('drupalapi', array('function' => 'theme_add_style', 'version' => '4.7')),
      '!drupal_add_css' => theme('drupalapi', array('function' => 'drupal_add_css', 'version' => '5')),
    )
  );
}

function _coder_review_5x_form_render_warning() {
  return t('Replace !form_render() with !drupal_render()',
    array(
      '!form_render' => theme('drupalapi', array('function' => 'form_render', 'version' => '4.7')),
      '!drupal_render' => theme('drupalapi', array('function' => 'drupal_render', 'version' => '5')),
    )
  );
}

function _coder_review_5x_node_get_name_warning() {
  return t('Replace !node_get_name($node) with !node_get_types(\'name\', $node)',
    array(
      '!node_get_name' => theme('drupalapi', array('function' => 'node_get_name', 'version' => '4.7')),
      '!node_get_types' => theme('drupalapi', array('function' => 'node_get_types', 'version' => '5')),
    )
  );
}
