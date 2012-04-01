<?php

/**
 * @file
 * This include file implements coder functionality for 5.x -> 6.x upgrades.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_6x_reviews() {
  $argex = '(((\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
  $rules = array(
    array(
      '#type' => 'regex',
      '#function' => '_menu$',
      '#value' => '\$items\[\]\s*=|if\s*\(\$may_cache\)',
      '#warning_callback' => '_coder_review_6x_new_menu_system_warning',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_menu$',
      '#value' => '(for|foreach|while)\s*\(',
      '#warning_callback' => '_coder_review_6x_new_menu_loop_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_form_alter\s*\(\$',
      '#warning_callback' => '_coder_review_6x_form_alter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_link_alter\s*\(\$',
      '#warning_callback' => '_coder_review_6x_link_alter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_profile_alter\s*\(\$',
      '#warning_callback' => '_coder_review_6x_profile_alter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_mail_alter\s*\([^,]+,',
      '#warning_callback' => '_coder_review_6x_mail_alter_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]l\s*\((' . $argex . '\s*,\s*){3,}(' . $argex . ')\s*\)',
      '#warning_callback' => '_coder_review_6x_url_l_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]url\s*\((' . $argex . '\s*,\s*){2,}(' . $argex . ')\s*\)',
      '#warning_callback' => '_coder_review_6x_url_l_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'global \$locale',
      '#warning' => '$locale became $language',
    ),
    array(
      '#type' => 'regex',
      '#value' => '(taxonomy_node_get_terms|taxonomy_node_get_terms_by_vocabulary|taxonomy_node_save|taxonomy_node_delete)\s*\(\s*(\d+|\$(nid|node->nid))',
      '#warning_callback' => '_coder_review_6x_taxonomy_node_get_terms_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]strtr\s*\(\s*format_plural\s*\(',
      '#warning_callback' => '_coder_review_6x_format_plural_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'watchdog\s*\(\s*\'\',\s*t\(\'',
      '#warning_callback' => '_coder_review_6x_watchdog_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]cache_set\s*\(\s*[^,]+,\s*([\'"]|\$table)',
      '#warning_callback' => '_coder_review_6x_cache_set_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]cache_set\s*\(\s*[^\)]+,\s*serialize\s*\(\s*',
      '#warning_callback' => '_coder_review_6x_cache_serialize_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]unserialize\s*\(\s*cache_get\s*\(',
      '#warning_callback' => '_coder_review_6x_cache_serialize_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\$_SERVER\s*\[\s*[\'"]REMOTE_ADDR[\'"]\s*\]',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_6x_remote_addr_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]file_check_upload\s*\(',
      '#warning_callback' => '_coder_review_6x_file_check_upload_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]file_save_upload\s*\([^,]+\s*,\s*(\$dest|\$destination|\'\'|"")\s*',
      '#never' => 'function\s+file_save_upload',
      '#warning_callback' => '_coder_review_6x_file_save_upload_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '\{file_revisions\}',
      '#warning' => 'The {' . 'file_revisions} table is now {upload}',
    ),
    array(
      '#type' => 'callback',
      '#value' => '_coder_review_6x_callback',
      // @NOTE: This is not used.  It only exists to catch potential errors in this code.
      '#warning_callback' => '_coder_review_6x_callback_warning',
    ),
    // FAPI Rules.
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '(\$form\s*\[\s*[\'"]#base[\'"]|[\'"]#base[\'"]\s*=>)',
      '#warning' => 'FormAPI\'s #base property is gone, manually populate the $form[\'#submit\'], $form[\'#validate\'], and $form[\'#theme\'] elements with the proper function names',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '\$form\s*\[\s*[\'"]#validate[\'"]\]\[[\'"]',
      '#warning_callback' => '_coder_review_6x_formapi_element_validate_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#function' => '_elements$',
      '#value' => '\s*[\'"]#validate[\'"]\s*=>\s*',
      '#warning_callback' => '_coder_review_6x_formapi_element_validate_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '(\$form\s*\[\s*[\'"]#multistep[\'"]|[\'"]#multistep[\'"]\s*=>)',
      '#warning' => '#multistep is gone, use $form_state instead',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(validate|submit)\s*\(\s*\$form_id',
      '#warning_callback' => '_coder_review_6x_form_validate_and_submit_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(validate|submit)\s*\(\s*\$form\s*,\s*\$form_state\s*\)[^;]',
      '#warning_callback' => '_coder_review_6x_form_validate_and_submit_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(prerender)\s*\(\s*\$form_id',
      '#warning_callback' => '_coder_review_6x_form_prerender_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '\$form\[[\'"]#submit[\'"]\]\[[\'"]',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_6x_formapi_custom_params_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]form_set_value\s*\([^,]+(,\s*[^,\)]+){0,1}\)',
      '#warning_callback' => '_coder_review_6x_form_set_value_warning',
      '#not' => 'array\s*\(',
    ),
/*  From Delete API, which has been reverted.
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]confirm_form\s*\([^,]+(,\s*[^,]+){4,}',
      '#warning_callback' => '_coder_review_6x_confirm_form_warning',
    ),
 */
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]custom_url_rewrite\s*\(',
      '#warning_callback' => '_coder_review_6x_custom_url_rewrite_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(info)\s*\(\s*\$field',
      '#warning_callback' => '_coder_review_6x_hook_info_auth_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '_(auth)\s*\(\s*\$user',
      '#warning_callback' => '_coder_review_6x_hook_info_auth_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'function\s+[a-z0-9_]+_(help)\s*\(\s*[^,]+\)',
      '#warning_callback' => '_coder_review_6x_hook_help_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '\$form(_values)?\s*\[\s*[\'"]?op[\'"]?\s*\]',
      '#warning' => 'replace $form[\'op\'] with $form_state[\'values\'][\'op\']',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#function' => '_(un)?install$|_update_[0-9]+$',
      '#value' => '\$GLOBALS\[[\'"]?db_type[\'"]?\]',
      '#warning_callback' => '_coder_review_6x_schema_api_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#function' => '_submit$|_validate$',
      '#value' => '\$form(_values)?\[[\'"][A-Za-z]',
      '#never' => '[\s\(]form_error\(',
      '#warning' => 'use $form_state[\'values\'] instead, which is where the values are stored after a submit',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]theme_get_function\s*\(',
      '#warning_callback' => '_coder_review_6x_theme_get_function_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]db_num_rows\s*\(',
      '#warning_callback' => '_coder_review_6x_db_num_rows_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]drupal_retrieve_form\s*\(' . $argex . '\s*\)',
      '#warning_callback' => '_coder_review_6x_drupal_retrieve_form_warning',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_menu$',
      '#value' => 'node\/add\/',
      '#source' => 'all',
      '#warning_callback' => '_coder_review_6x_menu_node_add_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'variable_(get|set|del)\(\s*[\'"]comment_form_location[\'"]\s*(,|\))',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_6x_comment_form_location_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '=\s*[\'"]comment_form_location[\'"]\s*;',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_6x_comment_form_location_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'admin\/content\/comment\/settings',
      '#source' => 'quote',
      '#warning_callback' => '_coder_review_6x_comment_form_location_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_access_rebuild\s*\(',
      '#function' => '_disable$|_enable$',
      '#warning_callback' => '_coder_review_6x_node_access_rebuild_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'admin\/logs',
      '#source' => 'all',
      '#warning_callback' => '_coder_review_6x_admin_logs_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'user_access\s*\(\s*\'\s*administer\s+access\s+control\s*\'\s*\)|admin\/user\/access',
      '#source' => 'all',
      '#warning_callback' => '_coder_review_6x_administer_access_control_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '(\$form\s*\[\s*[\'"]#DANGEROUS_SKIP_CHECK[\'"]|[\'"]#DANGEROUS_SKIP_CHECK[\'"]\s*=>)',
      '#warning_callback' => '_coder_review_6x_dangerous_skip_check_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]locale_refresh_cache\s*\(',
      '#warning_callback' => '_coder_review_6x_locale_refresh_cache_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]db_next_id\s*\(',
      '#warning_callback' => '_coder_review_6x_db_next_id_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]menu_set_location\s*\(',
      '#warning_callback' => '_coder_review_6x_menu_set_location_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]taxonomy_get_vocabulary\s*\(',
      '#warning_callback' => '_coder_review_6x_taxonomy_get_vocabulary_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]db_result\s*\((' . $argex . ')\s*,\s*' . $argex . '\);$',
      '#warning_callback' => '_coder_review_6x_db_result_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'watchdog\s*\(\s*[\'"]debug[\'"]\s*,\s*',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_6x_watchdog_debug_warning',
    ),
    // Book module rules -  start.
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_admin_orphan\s*\(',
      '#warning_callback' => '_coder_review_6x_book_admin_orphan_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_content\s*\(',
      '#warning_callback' => '_coder_review_6x_book_content_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_form\s*\(',
      '#warning_callback' => '_coder_review_6x_book_form_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_insert\s*\(',
      '#warning_callback' => '_coder_review_6x_book_insert_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_location\s*\(',
      '#warning_callback' => '_coder_review_6x_book_location_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_location_down\s*\(',
      '#warning_callback' => '_coder_review_6x_book_location_down_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_node_visitor_html_post\s*\(',
      '#warning_callback' => '_coder_review_6x_book_node_visitor_html_post_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_node_visitor_html_pre\s*\(',
      '#warning_callback' => '_coder_review_6x_book_node_visitor_html_pre_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_recurse\s*\(',
      '#warning_callback' => '_coder_review_6x_book_recurse_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_toc_recurse\s*\(',
      '#warning_callback' => '_coder_review_6x_book_toc_recurse_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_tree\s*\(',
      '#warning_callback' => '_coder_review_6x_book_tree_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_tree_recurse\s*\(',
      '#warning_callback' => '_coder_review_6x_book_tree_recurse_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_admin_edit\s*\((' . $argex . '\s*)\)',
      '#warning_callback' => '_coder_review_6x_book_admin_edit_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](book_toc\s*\((' . $argex . '\s*)\)|book_toc\s*\(\s*\))',
      '#warning_callback' => '_coder_review_6x_book_toc_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_export_html\s*\((' . $argex . '\s*,\s*)',
      '#warning_callback' => '_coder_review_6x_book_export_html_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](book_export\s*\((' . $argex . '\s*)\)|book_export\s*\(\s*\))',
      '#warning_callback' => '_coder_review_6x_book_export_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_outline\s*\(\$(nid|node->nid)',
      '#warning_callback' => '_coder_review_6x_book_outline_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_prev\s*\(\$node',
      '#warning_callback' => '_coder_review_6x_book_prev_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]book_next\s*\(\$node',
      '#warning_callback' => '_coder_review_6x_book_next_warning',
    ),
    // Book module rules -  end.
    array(
      '#type' => 'regex',
      '#value' => '\s*[\'"]#value[\'"]\s*=>\s*t\s*\(\s*[\'"]Submit[\'"]\s*\)',
      '#source' => 'allphp',
      '#warning' => 'When labelling buttons, make it clear what the button does, "Submit" is too generic.',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_nodeapi$',
      '#source' => 'allphp',
      '#value' => '(case\s+[\'"]submit[\'"]\s*:)|(==\s*[\'"]submit[\'"])',
      '#warning_callback' => '_coder_review_6x_nodeapi_submit_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '[\s\(]drupal_mail\s*\(\s*[^,]+,\s*(\$to|\$recipient|\$[a-zA-Z_]+?\->mail|(\'|").+@.+(\'|"))',
      '#warning_callback' => '_coder_review_6x_drupal_mail_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^(create\s+table\s+|alter\s+table\s+|drop\s+table\s+)',
      '#source' => 'quote',
      '#warning_callback' => '_coder_review_6x_drupal_install_schema_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]user_authenticate\s*\((' . $argex . '\s*,\s*)(' . $argex . ')\s*\)',
      '#warning_callback' => '_coder_review_6x_user_authenticate_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'function\s+[a-z0-9_]+_(access)\s*\((' . $argex . '\s*,\s*)(' . $argex . ')\s*\)',
      '#warning_callback' => '_coder_review_6x_hook_access_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '(node|user|menu|menu_link|taxonomy_vocabulary|contact|filter_format|forum_term|aggregator_feed|aggregator_category)_load\(arg\([0-9]\)\)',
      '#warning_callback' => '_coder_review_6x_menu_get_object_warning',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_submit$',
      '#value' => '\s+return\s+(' . $argex . ');',
      '#warning_callback' => '_coder_review_6x_form_submit_warning',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_menu$',
      '#source' => 'allphp',
      '#value' => '\'title\'\s*=>\s*t\(|\'description\'\s*=>\s*t\(',
      '#warning_callback' => '_coder_review_6x_menu_localization_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'function\s+[a-z0-9_]+_(forms)\s*\(\s*[^,]+\)',
      '#warning_callback' => '_coder_review_6x_hook_forms_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '\$form\s*\[\s*[\'"]#pre_render[\'"]\s*\]\[\s*[\'"].*[\'"]\s*\]\s*=',
      '#warning_callback' => '_coder_review_6x_formapi_prerender_warning',
    ),
    array(
      '#type' => 'regex',
      '#function' => '_menu$',
      '#value' => '[\'"]access callback.*=.*\(',
      '#source' => 'allphp',
      '#warning_callback' => '_coder_review_6x_menu_access_callback_warning',
    ),
  );
  $review = array(
    '#title' => t('Converting 5.x modules to 6.x'),
    '#link' => 'http://drupal.org/node/114774',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade6x' => $review);
}

/**
 * Define the rule callbacks for 6.x, see do_coder_review_callback().
 */
function _coder_review_6x_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $ignores = $coder_args['#ignore_lines'];
  $ignores = (empty($ignores) ? array() : $ignores[$review['#review_name']]);
  // Only perform this check on module's (not includes).
  $filename = $coder_args['#filename'];
  if (drupal_substr($filename, -7) == '.module') {
    // If there are any theme functions, make sure that a hook_theme exists.
    $theme = FALSE;
    $hook_theme = FALSE;
    foreach ($lines as $lineno => $line) {
      if (preg_match('/function theme_/', $line[0])) {
        if (!$theme) {
          $theme = TRUE;
          $theme_line = $line[0];
          $theme_lineno = $lineno;
        }
      }
      if (preg_match('/function \w+_theme\(/', $line[0])) {
        $hook_theme = TRUE;
      }
    }
    if ($theme && !$hook_theme) {
      $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
      $tmprule = $rule;
      $tmprule['#warning_callback'] = '_coder_review_6x_hook_theme_warning';
      _coder_review_error($results, $tmprule, $severity_name, $theme_lineno, $theme_line, $ignores);
    }

    // Read the .info file.
    $filename = drupal_substr(realpath($filename), 0, -7) . '.info';
    if (file_exists($filename)) {
      if ($lines = file($filename)) {
        foreach ($lines as $lineno => $line) {
          if (preg_match('/^dependencies\s*=/', $line)) {
            $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
            $tmprule = $rule;
            $tmprule['#warning_callback'] = '_coder_review_6x_dependency_warning';
            _coder_review_error($results, $tmprule, $severity_name, $lineno, $line, $ignores);
          }
          if (preg_match('/^core\s*=/', $line)) {
            $core = TRUE;
          }
        }
        if (!isset($core)) {
          $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
          $tmprule = $rule;
          $tmprule['#warning_callback'] = '_coder_review_6x_core_warning';
          _coder_review_error($results, $tmprule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }

    // Check that there is a translations directory and not a po directory.
    $dir = dirname($filename);
    if (file_exists($dir . '/po') && !file_exists($dir . '/translations')) {
      $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
      $tmprule = $rule;
      $tmprule['#warning_callback'] = '_coder_review_6x_translations_warning';
      _coder_review_error($results, $tmprule, $severity_name, -1, $dir);
    }
  }
}

/**
 * Define the warning callbacks.
 */

function _coder_review_6x_core_warning() {
  return array(
    '#warning' => t('New syntax for .info files files requires core=6.x'),
    '#link' => 'http://drupal.org/node/114774#info',
  );
}

function _coder_review_6x_dependency_warning() {
  return array(
    '#warning' => t('New syntax for .info files, use dependencies[]'),
    '#link' => 'http://drupal.org/node/114774#info',
  );
}

function _coder_review_6x_translations_warning() {
  return array(
    '#warning' => t('Translations moved from ./po to ./translations'),
    '#link' => 'http://drupal.org/node/114774#translations_directory',
  );
}

function _coder_review_6x_new_menu_system_warning() {
  return array(
    '#warning' => t('The menu system has been completely over-hauled in 6.x.'),
    '#link' => 'http://drupal.org/node/103114',
  );
}

function _coder_review_6x_new_menu_loop_warning() {
  return array(
    '#warning' => t('The menu system has been completely over-hauled in 6.x, and if you are looping, you are probably doing something wrong.'),
    '#link' => 'http://drupal.org/node/103114',
  );
}

function _coder_review_6x_form_alter_warning() {
  return array(
    '#warning' => t('!hook_form_alter() parameters have changed',
      array(
        '!hook_form_alter' => theme('drupalapi', array('function' => 'hook_form_alter', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/144132#form-alter',
  );
}

function _coder_review_6x_link_alter_warning() {
  return array(
    '#warning' => t('!hook_link_alter() parameters have changed',
      array(
        '!hook_link_alter' => theme('drupalapi', array('function' => 'hook_link_alter', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#link-alter',
  );
}

function _coder_review_6x_profile_alter_warning() {
  return array(
    '#warning' => t('!hook_profile_alter() parameters have changed',
      array(
        '!hook_profile_alter' => theme('drupalapi', array('function' => 'hook_profile_alter', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#profile-alter',
  );
}

function _coder_review_6x_mail_alter_warning() {
  return array(
    '#warning' => t('!hook_mail_alter() parameters have changed',
      array(
        '!hook_mail_alter' => theme('drupalapi', array('function' => 'hook_mail_alter', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#mail-alter',
  );
}

function _coder_review_6x_hook_theme_warning() {
  return array(
    '#warning' => t('new !hook_theme() function is required to register theme_ functions',
      array(
        '!hook_theme' => theme('drupalapi', array('function' => 'hook_theme', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#theme_registry',
  );
}

function _coder_review_6x_url_l_warning() {
  return array(
    '#warning' => t('!url() and !l() arguments changed, if you have a lot of these use <a href="@script">this conversion script</a>',
      array(
        '!url' => theme('drupalapi', array('function' => 'url', 'version' => '6')),
        '!l' => theme('drupalapi', array('function' => 'l', 'version' => '6')),
        '@script' => 'http://drupal.org/files/issues/replace.php_.txt',
      )
    ),
    '#link' => 'http://drupal.org/node/114774#url',
  );
}

function _coder_review_6x_taxonomy_node_get_terms_warning() {
  return array(
    '#warning' => t('!taxonomy_node_get_terms(), !taxonomy_node_get_terms_by_vocabulary(), !taxonomy_node_save and !taxonomy_node_delete() now take a full $node object, not just a nid (node id).',
      array(
        '!taxonomy_node_get_terms' => theme('drupalapi', array('function' => 'taxonomy_node_get_terms', 'version' => '6')),
        '!taxonomy_node_get_terms_by_vocabulary' => theme('drupalapi', array('function' => 'taxonomy_node_get_terms_by_vocabulary', 'version' => '6')),
        '!taxonomy_node_save' => theme('drupalapi', array('function' => 'taxonomy_node_save', 'version' => '6')),
        '!taxonomy_node_delete' => theme('drupalapi', array('function' => 'taxonomy_node_delete', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#taxonomy-revisions',
  );
}

function _coder_review_6x_format_plural_warning() {
  return array(
    '#warning' => t('!format_plural() accepts replacements, you no longer need to use !strtr()',
      array(
        '!format_plural' => theme('drupalapi', array('function' => 'format_plural', 'version' => '6')),
        '!strtr' => theme('phpapi', array('function' => 'strtr')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#format-plural',
  );
}

function _coder_review_6x_watchdog_warning() {
  return array(
    '#warning' => t('Parameters of !watchdog() changed, you <em>must</em> remove calls to !t()',
      array(
        '!watchdog' => theme('drupalapi', array('function' => 'watchdog', 'version' => '6')),
        '!t' => theme('drupalapi', array('function' => 't', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#watchdog_parameters',
  );
}

function _coder_review_6x_cache_set_warning() {
  return array(
    '#warning' => t('Changes to !cache_set() parameter order',
      array(
        '!cache_set' => theme('drupalapi', array('function' => 'cache_set', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#cache-set-parameter-order',
  );
}

function _coder_review_6x_cache_serialize_warning() {
  return array(
    '#warning' => t('!cache_set() and !cache_get() automatically (un)serialize complex data types',
      array(
        '!cache_set' => theme('drupalapi', array('function' => 'cache_set', 'version' => '6')),
        '!cache_get' => theme('drupalapi', array('function' => 'cache_get', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#cache-data-parameter',
  );
}

function _coder_review_6x_remote_addr_warning() {
  return array(
    '#warning' => t('Use new !ip_address() function instead of $_SERVER[\'REMOTE_ADDR\']',
      array(
        '!ip_address' => theme('drupalapi', array('function' => 'ip_address', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#ip-address',
  );
}

function _coder_review_6x_file_check_upload_warning() {
  return array(
    '#warning' => t('!file_check_upload() merged into !file_save_upload()',
      array(
        '!file_check_upload' => theme('drupalapi', array('function' => 'file_check_upload', 'version' => '5')),
        '!file_save_upload' => theme('drupalapi', array('function' => 'file_save_upload', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#file-check-upload',
  );
}

function _coder_review_6x_file_save_upload_warning() {
  return array(
    '#warning' => t('Parameters for !file_save_upload() have changed',
      array(
        '!file_save_upload' => theme('drupalapi', array('function' => 'file_save_upload', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#file-check-upload',
  );
}

function _coder_review_6x_formapi_element_validate_warning() {
  return array(
      '#warning' => 'Validation for specific form elements now uses the #element_validate property',
      '#link' => 'http://drupal.org/node/144132#element-validate',
  );
}

function _coder_review_6x_form_validate_and_submit_warning() {
  return array(
    '#warning' => t('The parameters for form validation and submission functions have changed to $form, &$form_state.'),
    '#link' => 'http://drupal.org/node/144132#process-params',
  );
}

function _coder_review_6x_form_prerender_warning() {
  return array(
    '#warning' => t('The parameters for form prerender functions have changed to $form.'),
    '#link' => 'http://drupal.org/node/144132#pre-render',
  );
}

function _coder_review_6x_formapi_prerender_warning() {
  return array(
    '#warning' => t('The form API for the #pre_render property has changed structure.'),
    '#link' => 'http://drupal.org/node/144132#pre-render',
  );
}

function _coder_review_6x_formapi_custom_params_warning() {
  return array(
    '#warning' => '$form[\'#submit\'] and $form[\'#validate\'] no longer support custom parameters',
    '#link' => 'http://drupal.org/node/144132#custom-params',
  );
}

function _coder_review_6x_form_set_value_warning() {
  return array(
    '#warning' => t('!form_set_value() parameters have changed',
      array(
        '!form_set_value' => theme('drupalapi', array('function' => 'form_set_value', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/144132#set-value',
  );
}

function _coder_review_6x_confirm_form_warning() {
  return array(
    '#warning' => t('The arguments to !confirm_form() have changed',
      array(
        '!confirm_form' => theme('drupalapi', array('function' => 'confirm_form', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#confirm-form-args',
  );
}

function _coder_review_6x_custom_url_rewrite_warning() {
  return array(
    '#warning' => t('In place of !custom_url_rewrite(), use !custom_url_rewrite_inbound() or !custom_url_rewrite_outbound()',
      array(
        '!custom_url_rewrite' => theme('drupalapi', array('function' => 'custom_url_rewrite', 'version' => '5')),
        '!custom_url_rewrite_inbound' => theme('drupalapi', array('function' => 'custom_url_rewrite_inbound', 'version' => '6')),
        '!custom_url_rewrite_outbound' => theme('drupalapi', array('function' => 'custom_url_rewrite_outbound', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#custom-url-rewrite',
  );
}

function _coder_review_6x_hook_info_auth_warning() {
  return array(
    '#warning' => t('hook no longer exists, use !hook_form_alter() to swap your own validation handler',
      array(
        '!hook_form_alter' => theme('drupalapi', array('function' => 'hook_form_alter', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#dist-auth',
  );
}

function _coder_review_6x_hook_help_warning() {
  return array(
    '#warning' => t('The arguments to !hook_help() have changed',
      array(
        '!hook_help' => theme('drupalapi', array('function' => 'hook_help', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#hook-help',
    '#description' => t('Change the first two lines to <code>function yourmodule_help($path, $arg) {' . "\n" . ' switch ($path) { ...</code>.'),
  );
}

function _coder_review_6x_schema_api_warning() {
  return array(
    '#warning' => t('A new schema API has been added in 6.x'),
    '#link' => 'http://drupal.org/node/146843',
    '#description' => t('This patch caused changes to the format of hook_install(), hook_uninstall(), and hook_update_N(). No longer are switch statements done on $GLOBALS[\'db_type\']; instead, use the variety of schema API functions to perform table manipulation.'),
  );
}

function _coder_review_6x_theme_get_function_warning() {
  return t('!theme_get_function() has been deprecated because of template theming; see !theme_get_registry()',
    array(
      '!theme_get_function' => theme('drupalapi', array('function' => 'theme_get_function', 'version' => '5')),
      '!theme_get_registry' => theme('drupalapi', array('function' => 'theme_get_registry', 'version' => '6')),
    )
  );
}

function _coder_review_6x_db_num_rows_warning() {
  return array(
    '#warning' => t('!db_num_rows() has been deprecated',
      array(
        '!db_num_rows' => theme('drupalapi', array('function' => 'db_num_rows', 'version' => '5')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#db-num-rows',
    '#description' => t('The db_num_rows() method was removed from the database abstraction layer in 6.x core, as it was a database dependent method.  Use SELECT COUNT(*) or some other appropriate solutions'),
  );
}

function _coder_review_6x_drupal_retrieve_form_warning() {
  return array(
    '#warning' => t('Parameters for !drupal_retrieve_form() have changed, add $form_state as a second argument',
      array(
        '!drupal_retrieve_form' => theme('drupalapi', array('function' => 'drupal_retrieve_form', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/144132#retrieve-form',
  );
}

function _coder_review_6x_menu_node_add_warning() {
  return array(
    '#warning' => t('The node/add/$type menu items are now auto-generated by the menu system in 6.x. You should not declare them in your menu hook. See <a href="@url">node/add is now menu generated</a>.',
      array(
        '@url' => url('http://drupal.org/node/114774#node_add_summary'),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#node_add_summary',
  );
}

function _coder_review_6x_comment_form_location_warning() {
  return array(
    '#warning' => t('Comment settings are now per node type.'),
    '#link' => 'http://drupal.org/node/114774#comment_settings_per_node_type',
  );
}

function _coder_review_6x_node_access_rebuild_warning() {
  return array(
    '#warning' => t('!node_access_rebuild() should not be called from !hook_enable() or !hook_disable() functions any more.',
      array(
        '!node_access_rebuild' => theme('drupalapi', array('function' => 'node_access_rebuild', 'version' => '6')),
        '!hook_enable' => theme('drupalapi', array('function' => 'hook_enable', 'version' => '6')),
        '!hook_disable' => theme('drupalapi', array('function' => 'hook_disable', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#node_access_rebuild_batch',
  );
}

function _coder_review_6x_admin_logs_warning() {
  return array(
    '#warning' => t('admin' . '/logs has been renamed to admin/reports, including all subpages.'),
    '#link' => 'http://drupal.org/node/114774#locale',
  );
}

function _coder_review_6x_administer_access_control_warning() {
  return array(
    '#warning' => t('The "administer access control" permission has been renamed to "administer permissions", and the path \'admin/' . 'user/access\' to \'admin/user/permissions\'.'),
    '#link' => 'http://drupal.org/node/114774#permissions',
  );
}

function _coder_review_6x_dangerous_skip_check_warning() {
  return array(
    '#warning' => t('Support for form property #DANGEROUS_SKIP_CHECK has been removed.'),
    '#link' => 'http://drupal.org/node/114774#choice_check',
  );
}

function _coder_review_6x_locale_refresh_cache_warning() {
  return array(
    '#warning' => t('!locale_refresh_cache() is deprecated.  Use !cache_clear_all() instead.',
      array(
        '!locale_refresh_cache' => theme('drupalapi', array('function' => 'locale_refresh_cache', 'version' => '5')),
        '!cache_clear_all' => theme('drupalapi', array('function' => 'cache_clear_all', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#locale',
  );
}

function _coder_review_6x_db_next_id_warning() {
  return array(
    '#warning' => t('!db_next_id() is deprecated.  Use !db_last_insert_id() instead.',
      array(
        '!db_next_id' => theme('drupalapi', array('function' => 'db_next_id', 'version' => '5')),
        '!db_last_insert_id' => theme('drupalapi', array('function' => 'db_last_insert_id', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#db_last_insert_id',
  );
}

function _coder_review_6x_menu_set_location_warning() {
  return array(
    '#warning' => t('!menu_set_location() is deprecated.  Use !drupal_set_breadcrumb() to set a custom breadcrumb or !menu_set_item() to set the current location in the menu tree.',
      array(
        '!menu_set_location' => theme('drupalapi', array('function' => 'menu_set_location', 'version' => '5')),
        '!drupal_set_breadcrumb' => theme('drupalapi', array('function' => 'drupal_set_breadcrumb', 'version' => '6')),
        '!menu_set_item' => theme('drupalapi', array('function' => 'menu_set_item', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#menu_set_location',
  );
}

function _coder_review_6x_taxonomy_get_vocabulary_warning() {
  return array(
    '#warning' => t('!taxonomy_get_vocabulary() is deprecated.  Use !taxonomy_vocabulary_load() instead.',
      array(
        '!taxonomy_get_vocabulary' => theme('drupalapi', array('function' => 'taxonomy_get_vocabulary', 'version' => '5')),
        '!taxonomy_vocabulary_load' => theme('drupalapi', array('function' => 'taxonomy_vocabulary_load', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/114774#taxonomy-load',
  );
}

function _coder_review_6x_watchdog_debug_warning() {
  return array(
    '#warning' => t('Parameters of !watchdog() changed, you use $severity WATCHDOG_DEBUG instead of $type \'debug\'',
      array(
        '!watchdog' => theme('drupalapi', array('function' => 'watchdog', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#watchdog_logging',
  );
}

function _coder_review_6x_book_admin_orphan_warning() {
  return array(
    '#warning' => t('!book_admin_orphan() is deprecated',
      array(
        '!book_admin_orphan' => theme('drupalapi', array('function' => 'book_admin_orphan', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_content_warning() {
  return array(
    '#warning' => t('!book_content() is deprecated',
      array(
        '!book_content' => theme('drupalapi', array('function' => 'book_content', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_form_warning() {
  return array(
    '#warning' => t('!book_form() is deprecated',
      array(
        '!book_form' => theme('drupalapi', array('function' => 'book_form', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_insert_warning() {
  return array(
    '#warning' => t('!book_insert() is deprecated',
      array(
        '!book_insert' => theme('drupalapi', array('function' => 'book_insert', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_location_warning() {
  return array(
    '#warning' => t('!book_location() is deprecated',
      array(
        '!book_location' => theme('drupalapi', array('function' => 'book_location', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_location_down_warning() {
  return array(
    '#warning' => t('!book_location_down() is deprecated',
      array(
        '!book_location_down' => theme('drupalapi', array('function' => 'book_location_down', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_node_visitor_html_post_warning() {
  return array(
    '#warning' => t('!book_node_visitor_html_post() is deprecated',
      array(
        '!book_node_visitor_html_post' => theme('drupalapi', array('function' => 'book_node_visitor_html_post', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_node_visitor_html_pre_warning() {
  return array(
    '#warning' => t('!book_node_visitor_html_pre() is deprecated',
      array(
        '!book_node_visitor_html_pre' => theme('drupalapi', array('function' => 'book_node_visitor_html_pre', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_recurse_warning() {
  return array(
    '#warning' => t('!book_recurse() is deprecated',
      array(
        '!book_recurse' => theme('drupalapi', array('function' => 'book_recurse', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_toc_recurse_warning() {
  return array(
    '#warning' => t('!book_toc_recurse() is deprecated',
      array(
        '!book_toc_recurse' => theme('drupalapi', array('function' => 'book_toc_recurse', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_tree_warning() {
  return array(
    '#warning' => t('!book_tree() is deprecated',
      array(
        '!book_tree' => theme('drupalapi', array('function' => 'book_tree', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_tree_recurse_warning() {
  return array(
    '#warning' => t('!book_tree_recurse() is deprecated',
      array(
        '!book_tree_recurse' => theme('drupalapi', array('function' => 'book_tree_recurse', 'version' => '5'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_admin_edit_warning() {
  return array(
    '#warning' => t('Parameters to !book_admin_edit() have changed',
      array(
        '!book_admin_edit' => theme('drupalapi', array('function' => 'book_admin_edit', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_toc_warning() {
  return array(
    '#warning' => t('Parameters to !book_toc() have changed',
      array(
        '!book_toc' => theme('drupalapi', array('function' => 'book_toc', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_export_html_warning() {
  return array(
    '#warning' => t('Parameters to !book_export_html() have changed',
      array(
        '!book_export_html' => theme('drupalapi', array('function' => 'book_export_html', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_export_warning() {
  return array(
    '#warning' => t('Parameters to !book_export() have changed',
      array(
        '!book_export' => theme('drupalapi', array('function' => 'book_export', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_outline_warning() {
  return array(
    '#warning' => t('Parameters to !book_outline() have changed',
      array(
        '!book_outline' => theme('drupalapi', array('function' => 'book_outline', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_prev_warning() {
  return array(
    '#warning' => t('Parameters to !book_prev() have changed',
      array(
        '!book_prev' => theme('drupalapi', array('function' => 'book_prev', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_book_next_warning() {
  return array(
    '#warning' => t('Parameters to !book_next() have changed',
      array(
        '!book_next' => theme('drupalapi', array('function' => 'book_next', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#book_module',
  );
}

function _coder_review_6x_nodeapi_submit_warning() {
  return array(
    '#warning' => t('!hook_nodeapi(\'submit\') has been replaced by op=\'presave\'',
      array(
        '!hook_nodeapi' => theme('drupalapi', array('function' => 'hook_nodeapi', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#nodeapi_presave',
  );
}

function _coder_review_6x_db_result_warning() {
  return array(
    '#warning' => t('Remove $row argument from !db_result() method',
      array(
        '!db_result' => theme('drupalapi', array('function' => 'db_result', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#db-result',
  );
}

function _coder_review_6x_drupal_mail_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_mail() have changed',
      array(
        '!drupal_mail' => theme('drupalapi', array('function' => 'drupal_mail', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/189367',
  );
}

function _coder_review_6x_drupal_install_schema_warning() {
  return array(
    '#warning' => t('Use the new Schema API to create, alter and drop tables rather than db_query()'),
    '#link' => 'http://drupal.org/node/146862',
  );
}

function _coder_review_6x_user_authenticate_warning() {
  return array(
    '#warning' => t('Parameters to !user_authenticate have changed',
      array(
        '!user_authenticate' => theme('drupalapi', array('function' => 'user_authenticate', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#user_authenticate',
  );
}

function _coder_review_6x_hook_access_warning() {
  return array(
    '#warning' => t('Parameters to !hook_access() have changed',
      array(
        '!hook_access' => theme('drupalapi', array('function' => 'hook_access', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#hook_access',
  );
}

function _coder_review_6x_menu_get_object_warning() {
  return array(
    '#warning' => t('Use !menu_get_object() to get an object based on your path',
      array(
        '!menu_get_object' => theme('drupalapi', array('function' => 'menu_get_object', 'version' => '6'))
      )
    ),
    '#link' => 'http://drupal.org/node/114774#menu_get_object',
  );
}

function _coder_review_6x_form_submit_warning() {
  return array(
    '#warning' => t('Submit handlers use $form_state[\'redirect\'] rather than returning urls'),
    '#link' => 'http://drupal.org/node/144132#redirect',
  );
}


function _coder_review_6x_menu_localization_warning() {
  return array(
    '#warning' => t('Menu item titles and descriptions should now no longer be wrapped in t() calls.'),
    '#link' => 'http://drupal.org/node/140311',
  );
}

function _coder_review_6x_hook_forms_warning() {
  return array(
    '#warning' => t('The arguments to !hook_forms() have changed',
      array(
        '!hook_forms' => theme('drupalapi', array('function' => 'hook_forms', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/144132#hook-forms',
  );
}

function _coder_review_6x_menu_access_callback_warning() {
  return array(
    '#warning' => t("The value for the 'access callback' must always be a string which is the the name of the function - never a function call. It may also be assigned the value TRUE or FALSE if the callback is always (or never) accessible."),
    '#link' => 'http://drupal.org/node/109157',
  );
}

