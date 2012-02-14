<?php

/**
 * @file
 * This include file implements coder functionality to check for
 * Internationalization issues in translation files.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_i18n_po_reviews() {
  $argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
  $rules = array(
    array(
      '#type' => 'callback',
      '#filename' => array('po'),
      '#value' => '_coder_review_translation_callback',
    ),
  );
  $review = array(
    '#title' => t('Translation files'),
    '#rules' => $rules,
    '#description' => t('experimental'),
  );
  return array('i18n_po' => $review);
}

/**
 * Define the rule callbacks for style.
 */
function _coder_review_translation_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $severity_name = _coder_review_severity_name($coder_args, $review, $rule);

  // Parse the translation file into msgid/msgstr pairs.
  $translations = array();
  foreach ($coder_args['#all_lines'] as $lineno => $line) {
    if (preg_match('/^(msgid|msgstr) "(.*)"$/', $line, $matches)) {
      if ($matches[1] == 'msgid') {
        $msgid = $matches[2];
      }
      elseif (!empty($msgid)) {
        $translations[$lineno] = array('msgid' => $msgid, 'msgstr' => $matches[2]);
        unset($msgid);
      }
    }
  }

  // Check each translation.
  foreach ($translations as $lineno => $translation) {
    $msgid = $translation['msgid'];
    $msgstr = $translation['msgstr'];

    // Check the translation first capitable letter.
    if (!empty($msgstr)) {
      if (ctype_upper($msgid[0]) != ctype_upper($msgstr[0])) {
        $rule = array(
          '#warning' => "The first letter in the translation text should have the same capitalization as it's original text.",
        );
        _coder_review_error($results, $rule, $severity_name, $lineno, $msgstr);
      }

      // Check the translation trailing periods.
      if (drupal_substr($msgid, -1, 1) == '.' && !drupal_substr($msgstr, -1, 1) != '.') {
        $rule = array(
          '#warning' => 'The translation text should end in a period when the same original text also ends in a period.',
        );
        _coder_review_error($results, $rule, $severity_name, $lineno, $msgstr);
      }

      // Check punctuation characters.
      if (preg_match_all('/[\.,:;?!]/', $msgid, $matches)) {
        foreach ($matches[0] as $html) {
          if (stripos($msgstr, $html) === FALSE) {
            $rule = array(
              '#warning' => 'Punctuation characters (.,:;?!) from the original text should exist in the translation.',
            );
            _coder_review_error($results, $rule, $severity_name, $lineno, $msgstr);
          }
        }
      }
      if (preg_match_all('/[\.,:;?!]/', $msgstr, $matches)) {
        foreach ($matches[0] as $html) {
          if (stripos($msgid, $html) === FALSE) {
            $rule = array(
              '#warning' => 'Punctuations characters (.,:;?!) in the translation should also exist in the original text.',
            );
            _coder_review_error($results, $rule, $severity_name, $lineno, $msgstr);
          }
        }
      }

      // Check HTML tags.
      if (preg_match_all('/<[^>]*>/', $msgid, $matches)) {
        foreach ($matches[0] as $html) {
          if (stripos($msgstr, $html) === FALSE) {
            $rule = array(
              '#warning' => 'HTML from the original text should also exist in the translation.'
            );
            _coder_review_error($results, $rule, $severity_name, $lineno, $msgstr);
          }
        }
      }

      // Check placeholders.
      if (preg_match_all('/[\!\@\%]\w+/', $msgid, $matches)) {
        foreach ($matches[0] as $placeholder) {
          if (stripos($msgstr, $placeholder) === FALSE) {
            $rule = array(
              '#warning' => 'If placeholders like !name, @name or %name exist in the original text, they must also exist in the translation.',
            );
            _coder_review_error($results, $rule, $severity_name, $lineno, $msgstr);
          }
        }
      }

      // @TODO: Check translations for opening/closing tags if they contain HTML.
      // @TODO: Quotation checks.
      // @TODO: Parenthesis (()[]{}) checks.
    }
  }
}

