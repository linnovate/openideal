<?php

/**
 * @file
 * This include file implements coder functionality for Drupal Standards.
 *
 * @todo The rules for this review are not yet complete.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_security_reviews() {
  $argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
  $allphp_argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'[^\']+\'|"[^"]+")';
  $sanitize_argex = '((t|st|\$t|check_plain|format_plural|check_markup|filter_xss|filter_xss_admin)\s*\([^\)]+?\))';
  $table = '\{[A-Za-z_]+\}'; // table-regex
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]trigger_error\s*\(\s*[^\$]+.+\$',
      '#never' => '(^function\s|trigger_error\s*\(\s*(((st|t|\$t)\s*\()|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_trigger_error_filter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]trigger_error\s*\(\s*(st|t|\$t)\s*\(\s*((.*?\$)|([\'"].*?!\w+.*?[\'"]\s*,)|(.*?array\(.*?!\w+.*?\)))',
      '#never' => '(^function\s|trigger_error\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_trigger_error_filter_t_warning',
    ),
    /*
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_title\s*\(\s*[^\$)]+.+\$',
      '#never' => '(^function\s|drupal_set_title\s*\(\s*(((st|t|\$t)\s*\()|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_drupal_set_title_filter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_title\s*\(\s*(st|t|\$t)\s*\(\s*((.*?\$)|([\'"].*?!\w+.*?[\'"]\s*,)|(.*?array\(.*?!\w+.*?\)))',
      '#never' => '(^function\s|drupal_set_title\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_drupal_set_title_filter_t_warning',
    ),
    */
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_message\s*\(\s*[^\$)]+.+\$',
      '#never' => '(^function\s|drupal_set_message\s*\(\s*(((st|t|\$t)\s*\()|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_drupal_set_message_filter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_set_message\s*\(\s*(st|t|\$t)\s*\(\s*((.*?\$)|([\'"].*?!\w+.*?[\'"]\s*,)|(.*?array\(.*?!\w+.*?\)))',
      '#never' => '(^function\s|drupal_set_message\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_drupal_set_message_filter_t_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](form_set_error|form_error)\s*\(\s*' . $argex . '\s*,\s*[^\$)]+.+\$',
      '#never' => '(^function\s|(form_set_error|form_error)\s*\(\s*' . $argex . '\s*,\s*(((st|t|\$t)\s*\()|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_form_set_error_filter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](form_set_error|form_error)\s*\(\s*' . $argex . '\s*,\s*(st|t|\$t)\s*\(\s*((.*?\$)|([\'"].*?!\w+.*?[\'"]\s*,)|(.*?array\(.*?!\w+.*?\)))',
      '#never' => '(^function\s|(form_set_error|form_error)\s*\(\s*' . $argex . '\s*,\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_form_set_error_filter_t_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]confirm_form\s*\(\s*' . $argex . '\s*,\s*[^\$\s]+.+\$[^,]+,\s*' . $argex . '\s*\)',
      '#never' => '(^function\s|confirm_form\s*\(\s*' . $argex . '\s*,\s*(((st|t|\$t)\s*\()|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_confirm_form_filter_warning',
    ),
    // confirm_form($form, t( ($taint | "abc $taint " | 'abcs '. $taint | stuff), array('!taint' => $taint)), $path);
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]confirm_form\s*\(\s*' . $argex . '\s*,\s*(st|t|\$t)\s*\(\s*((\$\w+[,\)])|("[^"]+?\$\w+.*?"\s*[,\)])|([\'"].*?[\'"]\s*\.\s*\$\w)|([\'"].*?!\w+.*?[\'"]\s*,)|(.*?array\(.*?!\w+.*?\)))',
      '#never' => '(^function\s|confirm_form\s*\(\s*' . $argex . '\s*,\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_confirm_form_filter_t_warning',
    ),
    // confirm_form 4th, 5th and 6th args
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]confirm_form\s*\(\s*((' . $allphp_argex . '|' . $sanitize_argex . ')\s*,\s*){3,5}([^\$\s]+.+\$[^,\)]+)(' . $allphp_argex . ')??\)\s*;',
      '#never' => '(^function\s|confirm_form\s*\(\s*((' . $allphp_argex . '|' . $sanitize_argex . ')\s*,\s*){3,5}((st|t|\$t|format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\([^\)\$]+\)).*?(' . $allphp_argex . ')??\)\s*;)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_confirm_form_filter_warning',
    ),
    // confirm_form($form, "safe string", $path, t( ($taint | "abc $taint " | 'abcs '. $taint | stuff), array('!taint' => $taint)), ...);
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]confirm_form\s*\(\s*((' . $allphp_argex . '|' . $sanitize_argex . ')\s*,\s*){3,5}(st|t|\$t)\s*\(\s*((\$\w+[,\)])|("[^"]+?\$\w+.*?"\s*[,\)])|([\'"].*?[\'"]\s*\.\s*\$\w)|([\'"].*?!\w+.*?[\'"]\s*,)|(.*?array\(.*?!\w+.*?\))).*?(' . $allphp_argex . ')??\)\s*;',
      '#never' => '(^function\s|confirm_form\s*\(\s*((' . $allphp_argex . '|' . $sanitize_argex . ')\s*,\s*){3,5}(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\([^\)\$]+\)).*?(' . $allphp_argex . ')??\)\s*;)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_confirm_form_filter_t_warning',
    ),
    array(
      '#type' => 'regex',
      '#severity' => 'minor',
      '#value' => '[\s\(]l\(check_plain\(.*',
      '#never' => '[\'"]html[\'"]\s*=>\s*(TRUE|1)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_l_check_plain_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(?-i)\$REQUEST_URI',
      '#warning_callback' => '_coder_review_security_request_uri_warning',
      '#function-not' => '^(request_uri|drupal_detect_baseurl)$',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '(?-i)\"REQUEST_URI\"|\'REQUEST_URI\'',
      '#warning_callback' => '_coder_review_security_request_uri_warning',
      '#function-not' => '^(request_uri|drupal_detect_baseurl)$',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+' . $table . '|insert\s+into\s+' . $table . '|update\s+' . $table . '\s+set|delete\s+from\s+' . $table . ')\s+.*\$[a-z0-9_]+',
      '#not' => '\$placeholder',
      '#source' => 'quote',
      '#warning_callback' => '_coder_review_security_sql_var_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(select\s+.*\s+from\s+' . $table . '|insert\s+into\s+' . $table . '|update\s+' . $table . '\s+set|delete\s+from\s' . $table . ')\s+[^\']*?(\s+|\(|=|,)\%s',
      '#source' => 'quote',
      '#warning' => 'SQL query handling data in a potentially insecure way by using the %%s placeholder without wrapping it in single quotes.  This is a potential source of SQL injection attacks when the value can come from user data.',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp', // allow us to look inside the regex string
      '#value' => '\bpreg_replace\s*\(\s*(\'(.)([^\'\\\\]|\\\\.)*\\2([^\'\\\\]|\\\\.)*|"(.)([^"\\\\]|\\\\.)*\\5([^"\\\\]|\\\\.)*)e',
      '#warning' => "Use preg_replace_callback() instead of the 'e' modifier to preg_replace()",
      '#severity' => 'critical',
    ),
    /*
    array(
      '#type' => 'regex',
      '#value' => '.*[\'"]SELECT\s+.*\s+(FROM|JOIN)\s+\{node\}',
      '#never' => '([\s\(]db_rewrite_sql\s*\(|\s\$\w+\s*=\s*[\'"]SELECT|[\'"]SELECT\s+COUNT\(|[\s\.]nid\s*=\s*%d)',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_db_rewrite_sql_warning',
      '#function-not' => '_cron$',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]db_rewrite_sql\(\s*[\'"]SELECT\s+.*\s+(FROM|JOIN)\s+\{node\}([,\'"]+|\s+(ON|WHERE|HAVING|LIMIT|ORDER|GROUP)\s+)',
      '#never' => '[\s\(]db_rewrite_sql\(\s*[\'"]SELECT\s+(COUNT\([^\)]+\))*.*\s+(FROM|JOIN)\s+\{node\}([,\'"]+|\s+(ON|WHERE|HAVING|LIMIT|ORDER|GROUP)\s+).*?,\s*[\'"]\{node\}[\'"]',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_db_rewrite_sql_warning',
      '#function-not' => '_cron$',
    ),
    */
    array(
      '#type' => 'regex',
      '#value' => '[\'"]access callback.*=.*\(',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_menu_access_callback_warning',
      '#function' => '_menu$',
    ),
    array(
      '#type' => 'regex',
      '#severity' => 'minor',
      '#value' => '\$_(POST)\[.+?\]',
      '#never' => '((((==|!=|>=|<=|>|<)\s*|[!\s\(](form_get_cache|form_set_cache|format_plural|filter_xss|filter_xss_admin|check_plain|check_markup|isset|empty|foreach|while|if|elseif)\s*\(\s*)\$_(POST)\[.+?\])|\$_(POST)\[.+?\]\s*(\.=|=|!=))',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_post_get_request_filter_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'html',
      '#value' => '<form[\s\'"]',
      '#warning_callback' => '_coder_review_security_form_tag_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](drupal_)*?eval\s*\([^\)]*?\$',
      '#never' => '(^function\s)',
      '#source' => 'php',
      '#warning_callback' => '_coder_review_security_eval_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\[][\'"]subject[\'"](\])*?\s*=(>)*?\s*\$',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_security_block_title_warning',
      '#function' => '_block$',
    ),
    array(
      '#type' => 'callback',
      '#value' => '_coder_review_security_callback',
    ),
  );
  $review = array(
    '#title' => 'Drupal Security Checks',
    '#link' => 'http://drupal.org/node/28984',
    '#rules' => $rules,
    '#severity' => 'critical',
    '#description' => t('very basic, needs work, errs on the side of caution so may give false positives'),
  );
  return array('security' => $review);
}


/**
 * Define the rule callbacks.
 */

function _coder_review_security_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
  $allphp_argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'[^\']+\'|"[^"]+")';
  $sanitize_argex = '((t|st|\$t|check_plain|format_plural|check_markup|filter_xss|filter_xss_admin)\s*\([^\)]+?\))';

  $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
  $ignores = $coder_args['#ignore_lines'];
  $ignores = (empty($ignores) || !isset($ignores[$review['#review_name']])) ? array() : $ignores[$review['#review_name']];
  /*
  if (!isset($coder_args['#tokens'])) {
    $source = implode('', $lines);
    $coder_args['#tokens'] = token_get_all($source);
  }
  */

  // Check for calls to trigger_error(), confirm_form(), drupal_set_title(),
  // drupal_set_message(), form_error() and form_set_error() on $vars without a
  // sanitizing function. If found, look back up within current function.
  $this_function = '';
  $function_paren = 0;
  $forward_matches = array();
  $function_name = '';
  foreach ($coder_args['#all_lines'] as $lineno => $line) {
    // Start of a function, store the line.
    if (preg_match('/function (\w+)\(/', $line, $match)) {
      $this_function = $line;
      $function_name = $match[1];
    }
    // Within a function.
    elseif ($function_paren > 0) {
      $this_function .= $line;
    }
    // Check to see if we're still in a function by counting number of {} chars.
    $tmp_line = preg_replace(array('/([^\\\])\'.+?[^\\\]\'/', '/([^\\\])".+?[^\\\]"/'), '$1', $line);
    if (preg_match('/([{}])/', $tmp_line, $match)) {
      $function_paren += ($match[0] == '{') ? 1 : -1;

      // If we've just exited a function, run forward match checks.
      if ($function_paren <= 0) {
        // db_rewrite_sql()
        /*
        if (isset($forward_matches['db_rewrite_sql'])) {
          $after_never_regex = '/[\s\(]db_rewrite_sql\s*\(\s*\$' . $forward_matches['db_rewrite_sql'] . '[\s,\)]/';
          if (!preg_match($after_never_regex, $this_function, $sanitized_matches)) {
            $rule = _coder_review_security_db_rewrite_sql_warning();
            _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
          }
        }
        */
        // FAPI #value
        if (isset($forward_matches['fapi_markup_value'])) {
          $find_match = '/[\s=]array\s*\(\s*([^\)]*?([\'"]#value[\'"]\s*=>\s*.*?\$' . $forward_matches['fapi_markup_value'] . '.*?))\);/';
          if (preg_match($find_match, $this_function, $sanitized_matches)) {
            if (!preg_match('/[\'"]#type[\'"]/', $sanitized_matches[1], $markup_matches) || preg_match('/[\'"]#type[\'"]\s*=>\s*[\'"]markup[\'"]/', $sanitized_matches[1], $markup_matches)) {
              $rule = _coder_review_security_fapi_markup_value_warning();
              _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
            }
          }
        }
        $forward_matches = array();
        $function_name = '';
      }
    }
    // If we're not in a function, continue.
    if ($function_paren < 0 || $this_function == '') {
      continue;
    }

    // Run our multi-line reviews.
    // trigger_error()
    $regex = '/[\s\(]trigger_error\s*\(\s*\$(\w+)\s*[,\)]/';
    $never_regex = '/(^function\s|trigger_error\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)/';
    if (preg_match($regex, $line, $matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[1] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $rule = _coder_review_security_trigger_error_filter_warning();
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    // drupal_set_title()
    /*
    $regex = '/[\s\(]drupal_set_title\s*\(\s*\$(\w+)\s*[,\)]/';
    $never_regex = '/(^function\s|drupal_set_title\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)/';
    if (preg_match($regex, $line, $matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[1] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $rule = _coder_review_security_drupal_set_title_filter_warning();
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    */
    // drupal_set_message()
    $regex = '/[\s\(]drupal_set_message\s*\(\s*\$(\w+)\s*[,\)]/';
    $never_regex = '/(^function\s|drupal_set_message\s*\(\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)/';
    if (preg_match($regex, $line, $matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[1] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $rule = _coder_review_security_drupal_set_message_filter_warning();
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    // form_set_error() and form_error()
    $regex = '/[\s\(](form_set_error|form_error)\s*\(\s*' . $argex . '\s*,\s*\$(\w+)\s*[,\)]/';
    $never_regex = '/(^function\s|(form_set_error|form_error)\s*\(\s*' . $argex . '\s*,\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)/';
    if (preg_match($regex, $line, $matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[8] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $rule = _coder_review_security_form_set_error_filter_warning();
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    // confirm_form()
    $regex = '/[\s\(]confirm_form\s*\(\s*' . $argex . '\s*,\s*\$(\w+)\s*[,\)]/';
    $never_regex = '/(^function\s|confirm_form\s*\(\s*' . $argex . '\s*,\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)/';
    if (preg_match($regex, $line, $matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[7] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $rule = _coder_review_security_confirm_form_filter_warning();
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    // confirm_form() - 4th, 5th and 6th args
    $regex = '/[\s\(]confirm_form\s*\(\s*((' . $allphp_argex . '|' . $sanitize_argex . ')\s*,\s*){3,5}\$(\w+)\s*[,\)]/';
    $never_regex = '/(^function\s|confirm_form\s*\(\s*((' . $allphp_argex . '|' . $sanitize_argex . ')\s*,\s*){3,5}(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*$)/';
    if (preg_match($regex, $line, $matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[11] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $rule = _coder_review_security_confirm_form_filter_warning();
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    // db_rewrite_sql()
    $regex = '/\$(\w+)\s*=\s*[\'"]SELECT\s+(.*)\s+(FROM|JOIN)\s+\{node\}(.*)?/';
    if (preg_match($regex, $line, $matches) && !preg_match('/COUNT\s*\(/', $matches[2]) && !preg_match('/[\s\.]nid\s*=\s*%d/', $matches[4])) {
      $forward_matches['db_rewrite_sql'] = $matches[1];
    }
    // FAPI #title and #description
    $regex = '/[\'"]#(title|description)[\'"]\s*=>\s*.*?\$(\w+)\s*.*?[,\)]/';
    $never_regex = '/([\'"]#(title|description)[\'"]\s*=>\s*((((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*))/';
    if (preg_match($regex, $line, $matches) && preg_match('/_form(_alter)*$/', $function_name, $function_matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[1] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $rule = _coder_review_security_fapi_title_description_warning();
          _coder_review_error($results, $rule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
    // FAPI #value
    $regex = '/[\'"]#value[\'"]\s*=>\s*.*?\$(\w+)\s*.*?[,\)]/';
    $never_regex = '/([\'"]#value[\'"]\s*=>\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\().*)/';
    if (preg_match($regex, $line, $matches) && preg_match('/_form(_alter)*$/', $function_name, $function_matches)) {
      if (!preg_match($never_regex, $coder_args['#all_lines'][$lineno])) {
        $before_never_regex = '/[\s]\$' . $matches[1] . '\s*=\s*(((st|t|\$t)\s*\(((\s*[\'"][^!]+?[\'"]\s*,)|(.*?array\([^!]+\))))|(format_plural|filter_xss|filter_xss_admin|check_plain|check_markup)\s*\()/';
        if (!preg_match($before_never_regex, $this_function, $sanitized_matches)) {
          $forward_matches['fapi_markup_value'] = $matches[1];
        }
      }
    }
  }
}

/**
 * Define the warning callbacks.
 */

function _coder_review_security_trigger_error_filter_warning() {
  return array(
    '#warning' => t('Potential problem: !trigger_error() only accepts filtered text, be sure to use !check_plain(), !filter_xss() or similar to ensure your $variable is fully sanitized.',
      array(
        '!trigger_error' => theme('phpapi', array('function' => 'trigger_error')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_trigger_error_filter_t_warning() {
  return array(
    '#warning' => t('Potential problem: !trigger_error() only accepts filtered text, be sure all !placeholders for $variables in !t() are fully sanitized using !check_plain(), !filter_xss() or similar.',
      array(
        '!trigger_error' => theme('phpapi', array('function' => 'trigger_error')),
        '!t' => theme('drupalapi', array('function' => 't')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

/*
function _coder_review_security_drupal_set_title_filter_warning() {
  return array(
    '#warning' => t('Potential problem: !drupal_set_title() only accepts filtered text, be sure to use !check_plain(), !filter_xss() or similar to ensure your $variable is fully sanitized.',
      array(
        '!drupal_set_title' => theme('drupalapi', array('function' => 'drupal_set_title')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_drupal_set_title_filter_t_warning() {
  return array(
    '#warning' => t('Potential problem: !drupal_set_title() only accepts filtered text, be sure all !placeholders for $variables in !t() are fully sanitized using !check_plain(), !filter_xss() or similar.',
      array(
        '!drupal_set_title' => theme('drupalapi', array('function' => 'drupal_set_title')),
        '!t' => theme('drupalapi', array('function' => 't')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}
*/

function _coder_review_security_drupal_set_message_filter_warning() {
  return array(
    '#warning' => t('Potential problem: !drupal_set_message() only accepts filtered text, be sure to use !check_plain(), !filter_xss() or similar to ensure your $variable is fully sanitized.',
      array(
        '!drupal_set_message' => theme('drupalapi', array('function' => 'drupal_set_message')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_drupal_set_message_filter_t_warning() {
  return array(
    '#warning' => t('Potential problem: !drupal_set_message() only accepts filtered text, be sure all !placeholders for $variables in !t() are fully sanitized using !check_plain(), !filter_xss() or similar.',
      array(
        '!drupal_set_message' => theme('drupalapi', array('function' => 'drupal_set_message')),
        '!t' => theme('drupalapi', array('function' => 't')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_form_set_error_filter_warning() {
  return array(
    '#warning' => t('Potential problem: !form_set_error() and !form_error() only accept filtered text, be sure to use !check_plain(), !filter_xss() or similar to ensure your $variable is fully sanitized.',
      array(
        '!form_set_error' => theme('drupalapi', array('function' => 'form_set_error')),
        '!form_error' => theme('drupalapi', array('function' => 'form_error')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_form_set_error_filter_t_warning() {
  return array(
    '#warning' => t('Potential problem: !form_set_error() and !form_error() only accept filtered text, be sure all !placeholders for $variables in !t() are fully sanitized using !check_plain(), !filter_xss() or similar.',
      array(
        '!form_set_error' => theme('drupalapi', array('function' => 'form_set_error')),
        '!form_error' => theme('drupalapi', array('function' => 'form_error')),
        '!t' => theme('drupalapi', array('function' => 't')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_confirm_form_filter_warning() {
  return array(
    '#warning' => t('Potential problem: !confirm_form() only accepts filtered text, be sure to use !check_plain(), !filter_xss() or similar to ensure your $variable is fully sanitized.',
      array(
        '!confirm_form' => theme('drupalapi', array('function' => 'confirm_form')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_confirm_form_filter_t_warning() {
  return array(
    '#warning' => t('Potential problem: !confirm_form() only accepts filtered text, be sure all !placeholders for $variables in !t() are fully sanitized using !check_plain(), !filter_xss() or similar.',
      array(
        '!confirm_form' => theme('drupalapi', array('function' => 'confirm_form')),
        '!t' => theme('drupalapi', array('function' => 't')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_l_check_plain_warning() {
  return array(
    '#warning' => t('!l() already contains a !check_plain() call by default. See http://drupal.org/node/28984',
      array(
        '!l' => theme('drupalapi', array('function' => 'l')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
      )
    ),
  );
}

function _coder_review_security_request_uri_warning() {
  return array(
    '#warning' => t('the use of REQUEST_URI is prone to XSS exploits and does not work on IIS; use !request_uri() instead',
      array(
        '!request_uri' => theme('drupalapi', array('function' => 'request_uri')),
      )
    ),
  );
}

function _coder_review_security_sql_var_warning() {
  return array(
    '#warning' => t('In SQL strings, Use !db_query() placeholders in place of variables.  This is a potential source of SQL injection attacks when the variable can come from user data.',
      array(
        '!db_query' => theme('drupalapi', array('function' => 'db_query')),
      )
    ),
    '#link' => 'http://drupal.org/writing-secure-code',
    '#description' => t('Use %s and %d variable substitution.  When inserting an array of values use <code>$placeholders = implode(\',\', array_fill(0, count($args), "\'%s\'"));</code>'),
  );
}

/*
function _coder_review_security_db_rewrite_sql_warning() {
  return array(
    '#warning' => t('Potential problem: "SELECT FROM {node}" statements should probably be wrapped in !db_rewrite_sql and with the alias for {node} table defined (e.g. {node} n)',
      array(
        '!db_rewrite_sql' => theme('drupalapi', array('function' => 'db_rewrite_sql')),
      )
    ),
  );
}
*/

function _coder_review_security_menu_access_callback_warning() {
  return array(
    '#warning' => t("The value for the 'access callback' must always be a string which is the the name of the function - never a function call. It may also be assigned the value TRUE or FALSE if the callback is always (or never) accessible."),
    '#link' => 'http://drupal.org/node/109157',
  );
}

function _coder_review_security_post_get_request_filter_warning() {
  return array(
    '#warning' => t('Potential problem: use the Form API to prevent against CSRF attacks. If you need to use $_POST variables, ensure they are fully sanitized if displayed by using !check_plain(), !f
ilter_xss() or similar.',
      array(
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/178896',
  );
}

function _coder_review_security_eval_warning() {
  return array(
    '#warning' => t("Using !eval() or !drupal_eval() in your module's code could have a security risk if the PHP input provided to the function contains malicious code.",
      array(
        '!eval' => theme('phpapi', array('function' => 'eval')),
        '!drupal_eval' => theme('drupalapi', array('function' => 'drupal_eval')),
      )
    ),
    '#link' => url('http://drupal.org/node/715010'),
  );
}

function _coder_review_security_form_tag_warning() {
  return array(
    '#warning' => t('Use the Form API to build forms to help prevent against CSRF attacks.'),
    '#link' => 'http://drupal.org/node/178896',
  );
}

function _coder_review_security_block_title_warning() {
  return array(
    '#warning' => t('Potential problem: !hook_block() only accepts filtered text as the block title, be sure to use !check_plain(), !filter_xss() or similar to ensure your $variable is fully sanitized.',
      array(
        '!hook_block' => theme('drupalapi', array('function' => 'hook_block')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_fapi_title_description_warning() {
  return array(
    '#warning' => t("Potential problem: FAPI elements '#title' and '#description' only accept filtered text, be sure to use !check_plain(), !filter_xss() or similar to ensure your \$variable is fully sanitized.",
      array(
        '!hook_form' => theme('drupalapi', array('function' => 'hook_form')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}

function _coder_review_security_fapi_markup_value_warning() {
  return array(
    '#warning' => t("Potential problem: when FAPI element '#type' is set to 'markup' (default), '#value' only accepts filtered text, be sure to use !check_plain(), !filter_xss() or similar to ensure your \$variable is fully sanitized.",
      array(
        '!hook_form' => theme('drupalapi', array('function' => 'hook_form')),
        '!check_plain' => theme('drupalapi', array('function' => 'check_plain')),
        '!filter_xss' => theme('drupalapi', array('function' => 'filter_xss')),
      )
    ),
    '#link' => 'http://drupal.org/node/28984',
  );
}
