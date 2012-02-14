<?php

/**
 * @file
 * This include file implements coder functionality for Drupal Standards.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_style_reviews() {
  $br = 'br';
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => '\t',
      '#warning' => 'Use an indent of 2 spaces, with no tabs',
    ),
    array(
      '#type' => 'regex',
      '#never' => '<\?php',
      '#value' => '^ (  )*[^ \'".]',
      '#warning' => 'Use an indent of 2 spaces, with no tabs',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s(if|elseif|while|foreach|switch|case|return|for|catch)\(',
      '#warning' => 'Control statements should have one space between the control keyword and opening parenthesis',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](\w+)\s\(',
      '#not' => '^(if|elseif|while|foreach|switch|case|return|for|list|catch)$',
      '#warning' => 'Functions should be called with no spaces between the function name and opening parentheses',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\){',
      '#warning' => 'use a space between the closing parenthesis and the open bracket',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(\S=>|=>\S)',
      '#source' => 'php',
      '#warning' => 'Arrays should be formatted with a space separating each element and assignment operator',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(\.(?:|\s{2,})[^\)\=\s0-9]|[^\(\s0-9](?:|\s{2,})\.)',
      '#warning' => 'String concatenation should be formatted with a space separating the operators (dot .) and the surrounding terms',
    ),
    array(
      '#type' => 'regex',
      '#value' => '<\?(\w+)',
      '#not' => '^(php|xml)$',
      '#warning' => 'Always use &lt;?php ?&gt; to delimit PHP code, not the &lt;? ?&gt; shorthand',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'global\s+\$(\w+)(,\s\$(\w+))*',
      '#not' => '^_|^(' . _coder_review_style_core_global_regex() . ')$',
      '#warning' => 'global variables should start with a single underscore followed by the module and another underscore',
    ),
    array(
      '#type' => 'callback',
      '#source' => 'all',
      '#value' => '_coder_review_style_closing_php_callback',
      '#warning' => 'the final ?> should be omitted from all code files',
    ),
    array(
      '#type' => 'regex',
      '#value' => '}\s*else',
      '#warning' => 'else statements should begin on a new line',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[,][^ \n\r]',
      '#warning' => 'missing space after comma',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^\s*{',
      '#warning' => 'curly braces { should end a line, not start one',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(?-i)(function\s+|\$)(([a-z]+[A-Z]+([a-z]*[A-Z]*)*)|([A-Z]+[a-z]+([A-Z]*[a-z]*)*))',
      '#warning' => 'do not use mixed case (camelCase), use lower case and _',
      '#class-not' => '.+',
      '#filename-not' => array('test'), // Add this line too for patch files, where we don't have have class context.
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s(stdclass)\s*\(',
      '#not' => '^(?-i)stdClass$',
      '#warning' => "use stdClass caseCapitalization, it's the one exception to the mixed case style standard",
    ),
    array(
      '#type' => 'regex',
      '#source' => 'html',
      '#value' => '<' . $br . '>', // NOTE: use $br only to avoid a warning.
      '#warning' => 'use &lt;br /&gt; instead of &lt;br&gt;',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'html',
      '#value' => '(?-i)<[A-Z]+',
      '#warning_callback' => '_coder_review_style_xhtml_warning',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s(if|elseif|while|foreach|switch|return|for|catch)\s*\(.*\) \s*{\s*[^\s]+',
      '#warning' => 'The control statement should be on a separate line from the control conditional',
    ),
    array(
      '#type' => 'regex',
      '#filename' => array('tpl.php'),
      '#value' => '\s(if|elseif)\s*\(.*\) \s*{\s*[^\s]+',
      '#warning' => 'The control statement should use ":" syntax instead of curly braces.',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'all',
      '#value' => '[ \t]+$',
      '#warning' => 'There should be no trailing spaces',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](strlen|strtolower|strtoupper|substr|ucfirst)\s*\(',
      '#warning' => 'in most cases, replace the string function with the drupal_ equivalent string functions',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\[\s*[A-Za-z][A-Za-z0-9_]*\s*]',
      '#not' => '\[\s*[A-Z][A-Z0-9_]*\s*]',
      '#warning' => 'use quotes around a string literal array index, this is not only a style issue, but a known performance problem',
      '#case-sensitive' => TRUE,
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s=>]+(true|false|null)[\)\s;,\n\r]+',
      '#case-sensitive' => TRUE,
      '#warning' => 'Use uppercase for PHP constants, e.g. NULL, TRUE, FALSE',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s+else\s+if\s*\(',
      '#warning' => 'Use "elseif" in place of "else if"',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\s*[\'"]#value[\'"]\s*=>\s*t\s*\(\s*[\'"]Submit[\'"]\s*\)',
      '#source' => 'allphp',
      '#warning' => 'When labelling buttons, make it clear what the button does, "Submit" is too generic.',
      '#severity' => 'minor',
    ),
  );
  $review = array(
    '#title' => t('Drupal Coding Standards'),
    '#link' => 'http://drupal.org/node/318',
    '#rules' => $rules,
    '#description' => t('every developer should use'),
  );
  return array('style' => $review);
}

/**
 * Define the rule callbacks for style.
 */
function _coder_review_style_closing_php_callback(&$coder_args, $review, $rule, $lines, &$results) {
  for ($lineno = -1; $last = array_slice($lines, $lineno); $lineno --) {
    $lastline = $last[0][0];
    if (preg_match('/\S/', $lastline)) {
      break;
    }
  }
  if ($last && $lastline && preg_match('/\?>\s*$/i', $lastline)) {
    $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
    _coder_review_error($results, $rule, $severity_name, count($lines));
  }
}

function _coder_review_style_core_global_regex() {
  static $coreglobalregex, $coreglobalvars;
  if (!isset($coreglobalregex)) {
    // Note: there's a little extra overhead in formatting this list as an
    // array, but I think it makes it more readable and maintainable.
    $coreglobalvars = array(
      // From the Drupal 5 includes/ directory.
      'active_db',
      'base_path',
      'base_root',
      'base_url',
      'conf',
      'custom_theme',
      'db_prefix',
      'db_type',
      'db_url',
      'form_button_counter',
      'form_submitted',
      'form_values',
      'install_locale',
      'installed_profile',
      'language',
      'last_result',
      'locale',
      'multibyte',
      'pager_page_array',
      'pager_total',
      'pager_total_items',
      'profile',
      'queries',
      'sidebar_indicator',
      'theme',
      'theme_engine',
      'theme_key',
      'theme_path',
      'timers',
      'user',
      'xrds_services',
      'xrds_open_elements',
      'xrds_current_service',

      // From the Drupal 5 modules/ directory -
      // Note: IMHO these should not be allowed, but until we fix core,
      // other modules will need them.
      'channel',
      'element',
      'forum_topic_list_header',
      'id',
      'image',
      'item',
      'items',
      'last_change',
      'last_nid',
      'nid',
      'recent_activity',
      'tag',
    );
    $coreglobalregex = implode('|', $coreglobalvars);
  }
  return $coreglobalregex;
}

/**
 * Define the warning callbacks.
 */

function _coder_review_style_xhtml_warning() {
  return t('use lowercase html tags to comply with <a href="@xhtml">XHTML</a>',
    array(
      '@xhtml' => 'http://www.w3.org/TR/xhtml1/#h-4.2',
    )
  );
}
