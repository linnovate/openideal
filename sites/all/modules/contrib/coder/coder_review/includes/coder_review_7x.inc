<?php

/**
 * @file
 * This include file implements coder functionality for 6.x -> 7.x upgrades.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_7x_reviews() {
  $argex = '(((&?\$?)[a-zA-Z_]+((\([^)]*\))|\[[^\]]*\])?)|[0-9]+(\.[0-9]*)?|\'\'|"")';
  $taxonomy_tables = '\{(term_data|term_hierarchy|term_node|term_relation|term_synonym|vocabulary|vocabulary_node_types)\}';

  $rules = array(
    // Module Info / Install
      // N/A - http://drupal.org/node/224333#module_file_during_install --- .module file available during install
      // N/A - http://drupal.org/node/224333#module_version_dependencies --- Module .info files can now optionally specify the version number of the module it depends on
      // N/A - http://drupal.org/node/224333#update_dependencies --- New update dependency system, affecting the order in which module updates are run
      // N/A - http://drupal.org/node/224333#configure_info --- Module .info files can have configure line

      // NO TESTS - http://drupal.org/node/224333#afile --- Add Doxygen @file tag to all install files
      array(
        '#type' => 'callback',
        '#value' => '_coder_review_7x_comment_doxygen_file_review_callback',
        // @NOTE: this warning_callback isn't used.  It only exists to catch
        // potential errors in the code.
        '#warning_callback' => '_coder_review_7x_comment_doxygen_file_review_callback_warning',
      ),
      // NO TESTS - http://drupal.org/node/224333#registry
      // and http://drupal.org/node/224333 --- Info file should specify core = 7.x
      array(
        '#type' => 'callback',
        '#value' => '_coder_review_7x_info_file_review_callback',
        // @NOTE: this warning_callback isn't used.  It only exists to catch
        // potential errors in the code.
        '#warning_callback' => '_coder_review_7x_info_file_review_callback_warning',
      ),
      // http://drupal.org/node/224333#update_sql --- Update hooks now return strings or throw exceptions, and update_sql() is no more
      array(
        '#type' => 'regex',
        '#source' => 'php',
        '#value' => '[\s=\(]update_sql\s*\(',
        '#warning_callback' => '_coder_review_7x_update_sql_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'php',
        '#value' => 'return\s+(array|\$ret)',
        '#warning_callback' => '_coder_review_7x_update_hook_ret_warning',
        '#function' => '_update_[0-9]+$',
        '#filename' => array('install'),
      ),

      // http://drupal.org/node/224333#update_php --- Update functions in .install files must include a Doxygen style comment


    // System
      // N/A - http://drupal.org/node/224333#variable_get_default_null --- Default parameter when getting variables
      // N/A - http://drupal.org/node/224333#hook_modules_action --- New hooks: hook_modules_installed, hook_modules_enabled, hook_modules_disabled, and hook_modules_uninstalled</a>
      // N/A - http://drupal.org/node/224333#static_variable_api --- Standardized API for static variables and resetting them
      // N/A - http://drupal.org/node/224333#archive-tar -- New tar archive library added
      // N/A - http://drupal.org/node/224333#user-1 --- User 1 is now called site maintenance account
      // N/A - http://drupal.org/node/224333#hook_module_implements_alter --- New hook_module_implements_alter
      // N/A - http://drupal.org/node/224333#drupal_static_advanced --- Functions called very often that need a drupal_static() variable can use an optimized way of calling that function
      // N/A - http://drupal.org/node/224333#hook_update_index --- hook_update_index() only runs when searching enabled for a given module

      // http://drupal.org/node/224333#module_implements_not_module_list --- use module_implements not module_list when calling hook implementations
      array(
        '#type' => 'callback',
        '#value' => '_coder_review_7x_module_implements_review_callback',
        // @NOTE: this warning_callback isn't used.  It only exists to catch
        // potential errors in the code.
        '#warning_callback' => '_coder_review_7x_module_implements_callback_warning',
      ),

      // http://drupal.org/node/224333#absolute_includes
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '((require|include)(_once)?).+[\'"]\.\/',
        '#warning_callback' => '_coder_review_7x_absolute_includes_warning',
      ),
      // http://drupal.org/node/224333#drupal_set_session
      array(
        '#type' => 'grep',
        '#value' => '$_SESSION',
        '#warning_callback' => '_coder_review_7x_session_warning',
        '#severity' => 'minor',
      ),
      // http://drupal.org/node/224333#time
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]time\s*\([\w\d\'"]*\)',
        '#warning_callback' => '_coder_review_7x_time_warning',
      ),
      // http://drupal.org/node/224333#rebuild_functions
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_rebuild_code_registry\s*\(',
        '#warning_callback' => '_coder_review_7x_code_registry_rebuild_function_warning',
      ),
      // http://drupal.org/node/224333#drupal_uninstall_modules
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_uninstall_module\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_uninstall_modules_warning',
      ),
      // http://drupal.org/node/224333#drupal_http_request_parameters
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_http_request\s*\(\s*(' . $argex . '\s*,\s*){2,}(' . $argex . ')\s*\)',
        '#warning_callback' => '_coder_review_7x_drupal_http_request_parameters_warning',
      ),
      // http://drupal.org/node/224333#moved_statistics_settings
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => 'admin\/reports\/settings',
        '#warning_callback' => '_coder_review_7x_moved_statistics_settings_warning',
      ),
      // http://drupal.org/node/224333#system_get_module_data
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]module_rebuild_cache\s*\(',
        '#warning_callback' => '_coder_review_7x_module_rebuild_cache_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]system_theme_data\s*\(',
        '#warning_callback' => '_coder_review_7x_system_theme_data_warning',
      ),
      // http://drupal.org/node/224333#drupal_set_html_head
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_set_html_head\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_set_html_head_warning',
      ),
      // http://drupal.org/node/224333#php_eval
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_eval\s*\(',
        '#warning_callback' => '_coder_review_7x_php_eval_warning',
      ),
      // http://drupal.org/node/224333#drupal_set_content
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_set_content\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_set_content_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_get_content\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_get_content_warning',
      ),
      // NO TESTS - http://drupal.org/node/224333#cache_implementation
      array(
        '#type' => 'callback',
        '#value' => '_coder_review_7x_cache_implementation_review_callback',
        // @NOTE: this warning_callback isn't used.  It only exists to catch
        // potential errors in the code.
        '#warning_callback' => '_coder_review_7x_cache_implementation_review_callback_warning',
      ),
      // http://drupal.org/node/224333#time_limit
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]set_time_limit\s*\(',
        '#function-not' => '^drupal_set_time_limit$',
        '#warning_callback' => '_coder_review_7x_time_limit_warning',
      ),
      // http://drupal.org/node/224333#drupal_set_header_renamed --- drupal_set_header() and drupal_get_headers() renamed to drupal_add_http_header() and drupal_get_http_header()
      array(
        '#type' => 'regex',
        '#value' => '[\s\(](drupal_set_header|drupal_get_headers)\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_set_get_header_warning',
      ),
      // http://drupal.org/node/224333#watchdog_emerg --- WATCHDOG_EMERG was renamed to WATCHDOG_EMERGENCY
      array(
        '#type' => 'regex',
        '#value' => '[\s\(,]WATCHDOG_EMERG.*$',
        '#not' => 'WATCHDOG_EMERGENCY',
        '#warning_callback' => '_coder_review_7x_watchdog_emerg_warning',
      ),
      // http://drupal.org/node/224333#remove-drupal-urlencode --- drupal_urlencode() replaced by drupal_encode_path()
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_urlencode\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_urlencode_warning',
      ),
      // http://drupal.org/node/224333#search-api --- API for modules providing search has changed
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_search\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_search_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]do_search\s*\(',
        '#warning_callback' => '_coder_review_7x_do_search_warning',
      ),

      // http://drupal.org/node/224333#drupal_alter ---   drupal_alter() now takes at most 3 parameters by reference
      // http://drupal.org/node/224333#email-html --- All e-mails are considered to originate as HTML
      // http://drupal.org/node/224333#system_retrieve_file ---  system_retrieve_file() API cleanup


    // Permissions and Access
      // N/A - http://drupal.org/node/224333#sorting_permissions --- Permissions are no longer sorted alphabetically
      // N/A - http://drupal.org/node/224333#node_access_alter_hooks --- Node access hooks now have drupal_alter() functions
      // N/A - http://drupal.org/node/224333#user-roles-api --- Added API functions for creating, loading, updating, and deleting user roles and permissions
      // N/A - http://drupal.org/node/224333#permissions_restrict_access --- New 'restrict access' parameter in hook_permission() for labeling unsafe permissions
      // N/A - in system http://drupal.org/node/224333#moved_statistics_settings --- Moved statistics settings from admin/reports/settings to admin/config/system/statistics and added a new 'administer statistics' permission

      // UNCLEAR - http://drupal.org/node/224333#hook_node_access_records --- hook_node_access_records() now applies to unpublished nodes; 'view own unpublished content' permission added
      // UNCLEAR - http://drupal.org/node/224333#bypass_node_access --- "administer nodes" permission split into "administer nodes" and "bypass node access"

      // http://drupal.org/node/224333#hook_permission
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(perm)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_perm_warning',
      ),
      // http://drupal.org/node/224333#descriptions_permissions
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_permission$',
        '#value' => '\s+array\s*\(\s*([\'"][a-zA-Z\s]+[\'"]\s*[,\)]\s*){1,}\s*;',
        '#warning_callback' => '_coder_review_7x_descriptions_permissions_warning',
      ),
      // http://drupal.org/node/224333#permission_tables
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '^(select\s+.*\s+from\s+{permission}|insert\s+into\s+{permission}|update\s+{permission}\s+set|delete\s+from\s+{permission})',
        '#warning_callback' => '_coder_review_7x_permission_tables_warning',
      ),
      // http://drupal.org/node/224333#php_permissions --- "use PHP for settings" permission should be used for all PHP settings rights (replaces "use PHP for block visibility")
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '^use PHP for block visibility$',
        '#warning_callback' => '_coder_review_7x_php_permissions_warning',
      ),
      // http://drupal.org/node/224333#hook_node_access --- hook_access() removed in favor of hook_node_access()
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(access)\s*\(',
        '#function-not' => '^\w+_node_access$',
        '#warning_callback' => '_coder_review_7x_hook_access_warning',
      ),


    // Database
      // N/A - http://drupal.org/node/224333#schema_date_time --- Schema API now supports date and time types natively
      // N/A - http://drupal.org/node/224333#foreign-keys-added --- Foreign keys added to core database table schema

      // http://drupal.org/node/224333#dbtng --- A completely new database API has been added

      // http://drupal.org/node/224333#schema_translation
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_(schema|update_\d+)$',
        '#value' => '[\'"]description[\'"]\s*=>\s*(t|st)\(',
        '#warning_callback' => '_coder_review_7x_schema_translation_warning',
      ),
      // http://drupal.org/node/224333#db_rewrite_sql
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]db_rewrite_sql\s*\(',
        '#warning_callback' => '_coder_review_7x_db_rewrite_sql_warning',
      ),
      // http://drupal.org/node/224333#schema_html --- Schema descriptions are now plain text instead of HTML
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_(schema|update_\d+)$',
        '#value' => '[\'"]description[\'"]\s*=>\s*.*?&lt;.*?&gt;',
        '#warning_callback' => '_coder_review_7x_schema_html_warning',
      ),
      // http://drupal.org/node/224333#select_count --- Do not use SELECT COUNT(*) to check for existence of rows
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => 'db_query\s*\(\s*[\'"]select\s+count\s*\(\s*\*\s*\)\s+from\s+',
        '#warning_callback' => '_coder_review_7x_select_count_warning',
        '#severity' => 'minor'
      ),
      // http://drupal.org/node/224333#install-schema --- Database schema (un)installed automatically
      array(
        '#type' => 'regex',
        '#source' => 'php',
        '#value' => '[\s\(]drupal_(un)*install_schema\s*\(',
        '#warning_callback' => '_coder_review_7x_install_schema_warning',
        '#function-not' => '^system_install$',
        '#filename' => array('install'),
      ),
      // http://drupal.org/node/224333#schema_ret --- The $ret parameter has been removed from all Schema operations
      // http://drupal.org/node/224333#block_tables_renamed --- Block tables renamed
      // http://drupal.org/node/224333#block_deltas_renamed --- Block deltas are now specified as strings
      // http://drupal.org/node/224333#nomorelower --- New pattern for cross-database, performant, case-insensitive comparisons
      // http://drupal.org/node/224333#moderate_column --- Remove moderate column from node_schema()
      // http://drupal.org/node/224333#db_result ---  db_result() has been removed; use ->fetchField() instead
      // http://drupal.org/node/224333#db_column --- Rename db_column_exists() to db_field_exists()  (no link)
      // http://drupal.org/node/224333#db_is_active --- db_is_active() has been removed


    // Menu
      // N/A - http://drupal.org/node/224333#international_item --- Added a new top level admin item, 'international'.
      // N/A - in comment section - http://drupal.org/node/224333#comment_load --- _comment_load() is now comment_load()
      // N/A - http://drupal.org/node/224333#menu-link-hooks --- CRUD hooks for menu links: hook_menu_link_insert(), hook_menu_link_update(), hook_menu_link_delete()

      // http://drupal.org/node/224333#menu_callback_array --- Menu callbacks should return an array; hello hook_page_alter()

      // http://drupal.org/node/224333#hook_menu_link_alter
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(menu_link_alter)\s*\(\s*(' . $argex . '\s*,\s*){1,}(' . $argex . ')\s*\)',
        '#warning_callback' => '_coder_review_7x_hook_menu_link_alter_warning',
      ),
      // http://drupal.org/node/224333#admin_path_changes --- Many paths to admin screens have changed
      // http://drupal.org/node/224333#custom_menu_api --- Custom menu API
      // http://drupal.org/node/224333#clean_urls_search_paths --- Changed Clean URLs and Search settings page path
      // http://drupal.org/node/224333#menu_tree_data --- Function menu_tree_data() now expects an array of links instead of a query results
      // http://drupal.org/node/224333#menu_default_node_menu --- menu_default_node_menu replaced with per-content type settings



    // Block
      // N/A - http://drupal.org/node/224333#hook_block_info_alter --- Added hook_block_info_alter()
      // N/A - in database section - http://drupal.org/node/224333#block_tables_renamed --- Block tables renamed
      // N/A - in database section - http://drupal.org/node/224333#block_deltas_renamed --- Block deltas are now specified as strings

      // http://drupal.org/node/224333#block_optional --- Block module now optional
      array(
        '#type' => 'callback',
        '#source' => 'allphp',
        '#value' => '_coder_review_7x_optional_block_review_callback',
        '#warning_callback' => '_coder_review_7x_optional_block_warning',
      ),

      // http://drupal.org/node/224333#remove_op
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(block)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_block_op_warning',
      ),

      // http://drupal.org/node/224333#custom_block --- Boxes" have been renamed to "custom blocks"



    // Comment
      // N/A - http://drupal.org/node/224333#comment_load_multiple --- New comment_load_multiple()
      // N/A - http://drupal.org/node/224333#comment_save_refactor --- comment_save() now supports programatic saving
      // N/A - http://drupal.org/node/224333#comment_presave_hook --- New hook_comment_presave() for comments

      // http://drupal.org/node/224333#comment_load
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]_comment_load\s*\(',
        '#warning_callback' => '_coder_review_7x_comment_load_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_menu$|_menu_alter$',
        '#value' => '%_comment',
        '#warning_callback' => '_coder_review_7x_comment_wildcard_warning',
      ),
      // http://drupal.org/node/224333#comment_status
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '\{comments\}\s+WHERE\s+.*?\s*status\s*(!=|=|<>|>|<)\s*(0|1)',
        '#warning_callback' => '_coder_review_7x_comment_status_warning',
      ),
      // http://drupal.org/node/224333#comment_validate_removed
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]comment_validate\s*\(',
        '#warning_callback' => '_coder_review_7x_comment_validate_warning',
      ),
      // http://drupal.org/node/224333#comment_node_url
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]comment_node_url\s*\(',
        '#warning_callback' => '_coder_review_7x_comment_node_url_warning',
      ),
      // http://drupal.org/node/224333#comment_created_changed ---  Comment.timestamp split into 'created' and 'changed'
      // http://drupal.org/node/224333 (no link) --- hook_comment removed and replaced with family of related functions
      // http://drupal.org/node/224333#comment_render --- Comment rendering overhaul


    // Input Sanitization & Input Formats
      // http://drupal.org/node/224333#check_markup_params
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]check_markup\s*\(\s*(' . $argex . '\s*,\s*){2}(\$check|FALSE|TRUE)',
        '#warning_callback' => '_coder_review_7x_check_markup_warning',
      ),
      // http://drupal.org/node/224333#drupal_set_title
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]drupal_set_title\s*\(\s*(check_plain\s*\(|t\s*\(\s*[\'"][^@%]*(@\w|\%\w))',
        '#never' => 'PASS_THROUGH',
        '#warning_callback' => '_coder_review_7x_drupal_set_title_warning',
      ),
      // http://drupal.org/node/224333#hook_filter_info
      array(
        '#type' => 'regex',
        '#value' => 'function\s+\w+_(filter)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_filter_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'function\s+\w+_(filter_tips)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_filter_tips_warning',
      ),
      // http://drupal.org/node/224333#text_format
      array(
        '#type' => 'regex',
        '#value' => '\$form\[.+?=\s*filter_form\(',
        '#warning_callback' => '_coder_review_7x_text_format_warning',
      ),
      // http://drupal.org/node/224333#filter_urls
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => 'admin\/settings\/filters(\/.*?)?',
        '#warning_callback' => '_coder_review_7x_filter_urls_warning',
      ),
      // http://drupal.org/node/224333#default-text-formats
      // http://drupal.org/node/224333#text-format-permissions
      // http://drupal.org/node/224333#filter_formats_parameters


    // Taxonomy
      // N/A - http://drupal.org/node/224333#taxonomy_term_load_multiple --- taxonomy_term_load() and taxonomy_term_load_multiple()
      // N/A - http://drupal.org/node/224333#taxonomy_hooks --- New hooks: hook_taxonomy_term_load(), hook_taxonomy_term_insert(), hook_taxonomy_term_update(), hook_taxonomy_term_delete() and hook_taxonomy_vocabulary_load(), hook_taxonomy_vocabulary_insert(), hook_taxonomy_vocabulary_update(), hook_taxonomy_vocabulary_delete()
      // N/A - http://drupal.org/node/224333#vocabulary_load_multiple --- Added taxonomy_vocabulary_load_multiple()

      // http://drupal.org/node/224333#taxonomy_get_tree
      array(
        '#type' => 'regex',
        '#source' => 'php',
        '#value' => '[\s\(]taxonomy_get_tree\s*\(\s*(' . $argex . '\s*,\s*){2}(\$depth|-1)\s*[,\)]',
        '#warning_callback' => '_coder_review_7x_taxonomy_get_tree_warning',
      ),
      // http://drupal.org/node/224333#taxonomy_tables
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '^(select\s+.*\s+from\s+' . $taxonomy_tables . '|insert\s+into\s+' . $taxonomy_tables . '|update\s+' . $taxonomy_tables . '\s+set|delete\s+from\s+' . $taxonomy_tables . ')',
        '#warning_callback' => '_coder_review_7x_taxonomy_tables_warning',
      ),
      // http://drupal.org/node/224333#taxonomy_crud
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_get_term\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_get_term_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_save_term\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_save_term_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_term_save\s*\(\s*array\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_save_term_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_del_term\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_del_term_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_save_vocabulary\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_save_vocabulary_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_vocabulary_save\s*\(\s*array\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_save_vocabulary_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_del_vocabulary\s*\(',
        '#warning_callback' => '_coder_review_7x_taxonomy_del_vocabulary_warning',
      ),
      // http://drupal.org/node/224333#taxonomy_paths
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => 'taxonomy\/term\/\d+(\+\d+|\,\d+|\/\d+)',
        '#warning_callback' => '_coder_review_7x_taxonomy_paths_warning',
      ),
      // http://drupal.org/node/224333#related_terms
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]taxonomy_get_related\s*\(',
        '#warning_callback' => '_coder_review_7x_related_terms_warning',
      ),
      // http://drupal.org/node/224333#taxonomy_node --- All taxonomy functions relating to nodes have been removed or refactored
      // http://drupal.org/node/224333#taxonomy_form_all --- taxonomy_form_all() removed
      // http://drupal.org/node/224333#taxonomy_term_view --- taxonomy_term_view() and taxonomy-term.tpl.php for term display
      // http://drupal.org/node/224333#no-synonyms-taxonomy --- Taxonomy synonyms have been removed


    // Javascript
      // N/A - http://drupal.org/node/224333#jquery_ui --- jQuery UI (1.7) was added into core
      // N/A - http://drupal.org/node/224333#attached_js --- Attached JavaScript and CSS for forms
      // N/A - http://drupal.org/node/224333#drupal_add_library --- Ability to add multiple JavaScript/CSS files at once
      // N/A - http://drupal.org/node/224333#drupal_add_js_css_reset --- Ability to reset JavaScript/CSS
      // N/A - http://drupal.org/node/224333#local_settings_behaviors --- Settings passed locally to JavaScript Behaviors

      // NO TESTS - http://drupal.org/node/224333#drupal_behaviors
      array(
        '#type' => 'regex',
        '#value' => 'Drupal\.behaviors\..+?\s*=\s*function\s*\(',
        '#filename' => array('js'),
        '#warning_callback' => '_coder_review_7x_drupal_behaviors_warning',
      ),
      array(
        '#type' => 'callback',
        '#value' => '_coder_review_7x_drupal_behaviors_review_callback',
        '#filename' => array('js'),
        // @NOTE: this warning_callback isn't used.  It only exists to catch
        // potential errors in the code.
        '#warning_callback' => '_coder_review_7x_drupal_behaviors_warning',
      ),
      // http://drupal.org/node/224333#drupal_add_js_weight
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]drupal_add_js\s*\(\s*' . $argex . '\s*,\s*[\'"](module|theme|core)[\'"]\s*[,\)]',
        '#warning_callback' => '_coder_review_7x_drupal_add_js_weight_warning',
      ),
      // http://drupal.org/node/224333#drupal_add_js_options
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_add_js\s*\(\s*(' . $argex . '\s*,\s*){2,}(' . $argex . ')\s*\)',
        '#warning_callback' => '_coder_review_7x_drupal_add_js_parameters_warning',
      ),
      // http://drupal.org/node/224333#hook_js_alter
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\'"]scripts[\'"]',
        '#function' => '_preprocess_page$',
        '#warning_callback' => '_coder_review_7x_hook_js_alter_warning',
      ),
      // NO TESTS - http://drupal.org/node/224333#javascript_compatibility
      array(
        '#type' => 'grep_invert',
        '#value' => '(function ($) {',
        '#filename' => array('js'),
        '#warning_callback' => '_coder_review_7x_javascript_compatibility_warning',
      ),
      // http://drupal.org/node/224333#drupal_add_js_external
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]drupal_add_html_head\s*\(\s*[\'"].*?<script\s.*?src\s*=\s*[\'"][a-zA-Z:\/\._\-0-9]+?\.js',
        '#warning_callback' => '_coder_review_7x_drupal_add_js_external_warning',
      ),
      // NO TESTS - http://drupal.org/node/224333#jquery_13
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '\[.*?@[a-zA-Z]+\s*(\^=|=|!=|\$=|\*=)\s*.+?\]',
        '#filename' => array('js'),
        '#warning_callback' => '_coder_review_7x_jquery_13_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '(\$|jQuery)\.(boxModel|browser.version|browser)',
        '#filename' => array('js'),
        '#warning_callback' => '_coder_review_7x_jquery_13_warning',
      ),
      // NO TESTS - http://drupal.org/node/224333#no-jsenabled
      array(
        '#type' => 'regex',
        '#value' => 'Drupal.jsEnabled',
        '#filename' => array('js'),
        '#warning_callback' => '_coder_review_7x_no_jsenabled_warning',
      ),
      // http://drupal.org/node/224333#jquery_once
      // http://drupal.org/node/224333#rename-drupal-to-js


    // CSS
      // N/A - http://drupal.org/node/224333#attached_js --- Attached JavaScript and CSS for forms
      // N/A - http://drupal.org/node/224333#drupal_add_css_inline --- Inline cascading style sheets from drupal_add_css()
      // N/A - http://drupal.org/node/224333#drupal_add_library --- Ability to add multiple JavaScript/CSS files at once
      // N/A - http://drupal.org/node/224333#drupal_add_js_css_reset --- Ability to reset JavaScript/CSS
      // N/A - http://drupal.org/node/224333#add-css-external --- drupal_add_css() now supports external CSS files

      // http://drupal.org/node/224333#drupal_add_js_options
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_add_css\s*\(\s*(' . $argex . '\s*,\s*){2,}(' . $argex . ')\s*\)',
        '#warning_callback' => '_coder_review_7x_drupal_add_css_parameters_warning',
      ),
      // http://drupal.org/node/224333#drupal_add_css_weight
      // http://drupal.org/node/224333#class_attribute_array
      // http://drupal.org/node/224333#form_clean_id --- form_clean_id() has been renamed to drupal_clean_css_identifier()


    // Theming
      // http://drupal.org/node/224333#rebuild_functions
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_rebuild_theme_registry\s*\(',
        '#warning_callback' => '_coder_review_7x_theme_rebuild_function_warning',
      ),
      // http://drupal.org/node/224333#theme_page
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]theme\s*\(\s*[\'"]page[\'"]',
        '#warning_callback' => '_coder_review_7x_theme_page_warning',
      ),
      // http://drupal.org/node/224333#hook_footer
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]theme\s*\(\s*[\'"]closure[\'"]',
        '#warning_callback' => '_coder_review_7x_hook_footer_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(closure|footer)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_footer_warning',
      ),
      // @NOTE: no simpletest for this because of #filename setting.
      array(
        '#type' => 'regex',
        '#value' => '\$closure[^a-zA-Z_]',
        '#filename' => array('tpl.php'),
        '#warning_callback' => '_coder_review_7x_closure_warning',
      ),
      // http://drupal.org/node/224333#element_theme_properties
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_elements$',
        '#value' => '[\'"]#type[\'"]',
        '#warning_callback' => '_coder_review_7x_element_theme_properties_warning',
      ),

      // http://drupal.org/node/224333#drupal_render_children --- Element theme functions should call drupal_render_children()
      // http://drupal.org/node/224333#theme-links-param --- theme_links() has a new parameter $heading for accessibility
      // http://drupal.org/node/224333#theme_changes --- theme() now takes only takes two arguments.
      // http://drupal.org/node/224333#hook_theme_render_changes --- hook_theme() requires "variables" or "render element" instead of "arguments" to better integrate with drupal_render()
      // http://drupal.org/node/224333#custom_theme --- New method for altering the theme used to display a page (global $custom_theme variable removed)
      // http://drupal.org/node/224333#placeholder --- theme('placeholder') replaced by drupal_placeholder()
      // http://drupal.org/node/224333#theme_hook_suggestions_1 --- A theme hook name followed by a double underscore ('__') is a default 'pattern'
      // http://drupal.org/node/224333#theme_hook_suggestions_2 --- Preprocess functions need to now specify "theme_hook_suggestion(s)" instead of "template_file(s)"
      // http://drupal.org/node/224333#theme_links_with_context --- Use #theme='links__MODULE' or #theme='links__MODULE_EXTRA_CONTEXT' when adding links to a render array
      // http://drupal.org/node/224333#theme_pager --- theme_pager() no longer takes limit parameter
      // http://drupal.org/node/224333#theme_username --- theme_username() parameters changed



    // Form API
      // http://drupal.org/node/224333#poundtheme --- #theme recommended for specifying theme function

      // http://drupal.org/node/224333#node_form
      array(
        '#type' => 'regex',
        '#function' => '_form_alter$',
        '#source' => 'allphp',
        '#value' => '\$form\[[\'"]type[\'"]\]\[[\'"]\#value[\'"]\]\s*\.\s*[\'"]_node_form[\'"]',
        '#warning_callback' => '_coder_review_7x_node_form_warning',
      ),
      // No link - parameter change to hook_form_alter().
      array(
        '#type' => 'regex',
        '#function' => '_form_alter$',
        '#value' => 'function\s+[a-z0-9_]+_(form_alter)\s*\(\s*&\$form,\s*\$form_state,\s*\$form_id\s*\)',
        '#warning_callback' => '_coder_review_7x_hook_form_alter_warning',
      ),
      // http://drupal.org/node/224333#process_functions
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\'"]#process[\'"](\])?\s*=(>)?\s*[\'"](expand_password_confirm|expand_date|expand_radios|form_expand_ahah|expand_checkboxes|process_weight)[\'"]',
        '#warning_callback' => '_coder_review_7x_process_functions_warning',
      ),

      // http://drupal.org/node/224333#markup --- Use '#markup' not '#value' for markup.

      // http://drupal.org/node/224333#drupal_execute_drupal_form_submit
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_execute\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_execute_renamed_warning',
      ),

      // http://drupal.org/node/224333#unrendered --- drupal_get_form() returns an array

      // http://drupal.org/node/224333#ahah-now-ajax
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\'"]#ahah[\'"]',
        '#warning_callback' => '_coder_review_7x_ahah_now_ajax_warning',
      ),

      // http://drupal.org/node/224333#hook_forms_signature --- The signature of the callback from drupal_get_form() changed to add $form
      // http://drupal.org/node/224333#operation_links --- Use #type='link' for adding a single link to a render array, particularly for tables that include operation links like 'edit', 'delete', etc
      // http://drupal.org/node/224333#entity_prepare_view --- Added entity_prepare_view() and hook_entity_prepare_view()
      // http://drupal.org/node/224333#attached_js --- Attached JavaScript and CSS for forms
      // http://drupal.org/node/224333#hook_element_info --- hook_elements() renamed to hook_element_info()
      // http://drupal.org/node/224333#fapi_changes --- Removal of FAPI $form['#redirect'] and $_REQUEST['destination']
      // http://drupal.org/node/224333#actions_buttons --- Form submit buttons consistently grouped in actions array

    // File API
      // N/A - http://drupal.org/node/224333#file_load_multiple --- file_load_multiple()
      // N/A - http://drupal.org/node/224333#file_check_directory_recursive --- file_check_directory() will now recursively create directories

      // UNCLEAR - http://drupal.org/node/224333#file_validate_extensions --- file_validate_extensions() enforces check for uid=1
      // UNCLEAR - http://drupal.org/node/224333#file_scan_directory_property_names --- file_scan_directory() now uses same property names as file_load()
      // UNCLEAR - http://drupal.org/node/224333#unmanaged_files --- File operations that don't affect the database have been renamed
      // UNCLEAR - http://drupal.org/node/224333#user_picture --- User pictures are now managed files

      // http://drupal.org/node/224333#file_scan_directory_array-itize
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]file_scan_directory\s*\(\s*((' . $argex . '\s*,\s*){4,}(' . $argex . ')\s*\)|(' . $argex . '\s*,\s*){2,}\$nomask)',
        '#warning_callback' => '_coder_review_7x_file_scan_directory_array_itize_warning',
      ),
      // http://drupal.org/node/224333#file_scan_directory_nomask
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]file_scan_directory\s*\(\s*(\s*.*?,\s*){2,}array\(([\'"].*?[\'"](,)*){1,}\)\s*[,\)]',
        '#warning_callback' => '_coder_review_7x_file_scan_directory_nomask_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]file_scan_directory\s*\(\s*(\s*.*?,\s*){2,}array\(.+?[\'"]nomask[\'"]\s*=>\s*array\(([\'"].*?[\'"](,)*){1,}\)\s*[,\)]',
        '#warning_callback' => '_coder_review_7x_file_scan_directory_nomask_warning',
      ),
      // http://drupal.org/node/224333#file_set_status
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]file_set_status\s*\(',
        '#warning_callback' => '_coder_review_7x_file_set_status_warning',
      ),
      // http://drupal.org/node/224333#remove_FILE_STATUS_TEMPORARY
      array(
        '#type' => 'grep',
        '#value' => 'FILE_STATUS_TEMPORARY',
        '#case-sensitive' => TRUE,
        '#warning_callback' => '_coder_review_7x_file_status_temporary_warning',
      ),
      // http://drupal.org/node/224333#preg_match
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]file_scan_directory\s*\(\s*([^,]+?\s*,\s*){1}[\'"].*?[\'"]\s*[,\)]',
        '#never' => '[\s\(]file_scan_directory\s*\(\s*[^,]+?,\s*[\'"]\/.*?\/[\'"]\s*[,\)]',
        '#warning_callback' => '_coder_review_7x_file_scan_directory_preg_match_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]drupal_system_listing\s*\(\s*[\'"][^\/].*?[^\/][\'"]\s*,',
        '#never' => '[\s\(]drupal_system_listing\s*\(\s*[\'"]\/.*?\/[\'"]\s*,',
        '#warning_callback' => '_coder_review_7x_drupal_system_listing_preg_match_warning',
      ),
      // http://drupal.org/node/224333#file_prepare_directory --- file_check_directory() renamed to file_prepare_directory()
      // http://drupal.org/node/224333#hook_file_url_alter --- New hook: hook_file_url_alter()


    // User API
      // N/A - http://drupal.org/node/224333#specified_ids --- Save new users and nodes with specified IDS

      // http://drupal.org/node/224333#user_cancel
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]user_delete\s*\(',
        '#warning_callback' => '_coder_review_7x_user_delete_warning',
      ),
      // http://drupal.org/node/224333#remove_op
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(user)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_user_op_warning',
      ),
      // http://drupal.org/node/719612 --- Changed log out path from 'logout' to 'user/logout' for consistency
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\'"]logout[\'"]',
        '#warning_callback' => '_coder_review_7x_user_logout_warning',
      ),
      // http://drupal.org/node/224333#user_load_multiple
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]user_load\s*\(\s*(array\s*\(|\(\s*array\s*\))',
        '#warning_callback' => '_coder_review_7x_user_load_warning',
      ),
      // http://drupal.org/node/224333#user_authenticate
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]user_authenticate\s*\(\s*' . $argex . '\s*\)',
        '#warning_callback' => '_coder_review_7x_user_authenticate_warning',
      ),
      // http://drupal.org/node/224333#hook-user-changes
      array(
        '#type' => 'regex',
        '#value' => 'function\s+\w+_profile_alter\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_profile_alter_warning',
      ),
      // http://drupal.org/node/224333#format_username --- New API function: format_username() and new hook: hook_username_alter()
      // http://drupal.org/node/224333#hook_user_form_register_gone --- hook_user_form(), hook_user_register() are gone
      // http://drupal.org/node/224333#hook_user_validate_gone --- hook_user_validate() and hook_user_submit() are gone
      // http://drupal.org/node/224333#hook_user_after_update --- hook_user_after_update() replaced by hook_user_update(), amended by hook_user_presave() for common operations


    // Node API
      // N/A - http://drupal.org/node/224333#delete_multiple --- Add node_delete_multiple()
      // N/A - http://drupal.org/node/224333#specified_ids --- Save new users and nodes with specified IDS

      // No link - node_revisions table name changed.
      array(
        '#type' => 'regex',
        '#source' => 'quote',
        '#value' => '^(select\s+.*\s+from\s+{node_revisions}|insert\s+into\s+{node_revisions}|update\s+{node_revisions}\s+set|delete\s+from\s+{node_revisions})',
        '#warning_callback' => '_coder_review_7x_node_revisions_table_warning',
      ),

      // http://drupal.org/node/224333#node_links
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => 'function\s+[a-z0-9_]+_(link)\s*\(\s*([\'"](node|taxonomy terms)[\'"]|' . $argex . '\s*,\s*\$node\s*,)',
        '#warning_callback' => '_coder_review_7x_node_links_warning',
      ),
      // http://drupal.org/node/224333#node_load_multiple
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#value' => '[\s\(]node_load\s*\(\s*(\$param\s*[,\)]|array\s*\()',
        '#warning_callback' => '_coder_review_7x_node_load_warning',
      ),
      // http://drupal.org/node/224333#node_type_base
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_node_info$',
        '#value' => '[\'"](module)[\'"]\s*=>\s*[\'"].*?[\'"]',
        '#warning_callback' => '_coder_review_7x_hook_node_info_module_warning',
      ),
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_node_info$',
        '#value' => '[\'"](module|base)[\'"]\s*=>\s*[\'"]node[\'"]',
        '#warning_callback' => '_coder_review_7x_hook_node_info_node_content_warning',
      ),
      // http://drupal.org/node/224333#remove_op
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(nodeapi)\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_nodeapi_op_warning',
      ),
      // http://drupal.org/node/224333#node_invoke_nodeapi
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]node_invoke_nodeapi\s*\(',
        '#warning_callback' => '_coder_review_7x_node_invoke_nodeapi_removed_warning',
      ),
      // http://drupal.org/node/224333#node_view
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]node_view\s*\(',
        '#warning_callback' => '_coder_review_7x_node_view_warning',
      ),
      // http://drupal.org/node/224333#node_build_rss
      array(
        '#type' => 'regex',
        '#source' => 'allphp',
        '#function' => '_nodeapi$',
        '#value' => '[\'"]rss item[\'"]',
        '#warning_callback' => '_coder_review_7x_node_build_rss_warning',
      ),
      // http://drupal.org/node/224333#node_type_get_functions
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]node_get_types\s*\(',
        '#warning_callback' => '_coder_review_7x_node_type_get_functions_warning',
      ),
      // http://drupal.org/node/224333#build_mode
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]\$node->build_mode',
        '#warning_callback' => '_coder_review_7x_build_mode_warning',
      ),
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]NODE_BUILD_PREVIEW',
        '#warning_callback' => '_coder_review_7x_node_build_preview_warning',
      ),
      // http://drupal.org/node/224333#hook_node_xxx
      array(
        '#type' => 'regex',
        '#value' => 'function\s+[a-z0-9_]+_(nodeapi)_[a-z]+\s*\(',
        '#warning_callback' => '_coder_review_7x_hook_node_xxx_warning',
      ),
      // http://drupal.org/node/224333#view_mode --- $teaser parameter changed to $view_mode in node viewing functions and hooks, $node->build_mode property removed
      // http://drupal.org/node/224333#hook_load_signature --- hook_load() signature and return value change


    // Multilingual
      // http://drupal.org/node/224333#locale_context --- Added string context support to t() and format_plural(), changed parameters
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]t\s*\(\s*(.*?\s*,\s*){2,}([\'"].*?[\'"])\s*\)',
        '#value' => '[\s\(]t\s*\(\s*(' . $argex . '\s*,\s*){2,}([\'"].*?[\'"])\s*\)',
        '#warning_callback' => '_coder_review_7x_t_signature_warning',
      ),

      array(
        '#type' => 'regex',
        '#value' => '[\s\(]format_plural\s*\(\s*(' . $argex . '\s*,\s*){4,}([\'"].*?[\'"])\s*\)',
        '#warning_callback' => '_coder_review_7x_format_plural_signature_warning',
      ),
      // http://drupal.org/node/224333#drupal_language_none --- Language neutral content now has an explicit language associated with it
      // http://drupal.org/node/224333#language_negotiation --- New language negotiation API introduced


    // Misc
      // N/A - http://drupal.org/node/224333#sticky_headers --- Make sticky tableheaders optional
      // N/A - http://drupal.org/node/224333#implementation_hook_comment --- Commenting style - use 'Implement hook_foo().' when documenting hooks.

      // http://drupal.org/node/224333#book_toc_parameters
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]book_toc\s*\(\s*' . $argex . '\s*,\s*(array\s*\(|(\$?)[a-zA-Z_]+\s*,\s*\d+)',
        '#warning_callback' => '_coder_review_7x_book_toc_warning',
      ),
      // http://drupal.org/node/224333#referer_uri
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]referer_uri\s*\(',
        '#warning_callback' => '_coder_review_7x_referer_uri_warning',
      ),
      // http://drupal.org/node/224333#drupal_clone
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]drupal_clone\s*\(',
        '#warning_callback' => '_coder_review_7x_drupal_clone_warning',
      ),
      // http://drupal.org/node/224333#actions_synchronize
      array(
        '#type' => 'regex',
        '#value' => '[\s\(]actions_synchronize\s*\((\$actions|[^,]+,)',
        '#warning_callback' => '_coder_review_7x_actions_synchronize_warning',
      ),
      // http://drupal.org/node/224333#distauth --- Login validation change for distributed authentication modules
      // http://drupal.org/node/224333#trigger_overhaul --- Trigger and Actions API overhaul
      // http://drupal.org/node/224333#hook_entity_load --- Added hook_entity_load().
      // http://drupal.org/node/224333#api_php --- Code documentation to module.api.php
      // http://drupal.org/node/224333#php_version --- Drupal 7 requires PHP 5.2 or higher
      // http://drupal.org/node/224333#hook_url_outbound_alter --- Replaced taxonomy_term_path(), hook_term_path(), language_url_rewrite(), and custom_url_alter_outbound() with hook_url_outbound_alter()
      // http://drupal.org/node/224333#hook_url_inbound_alter --- Replaced custom_url_rewrite_inbound() with hook_url_inbound_alter()
      // http://drupal.org/node/224333#url_is_external --- Renamed menu_path_is_external() to url_is_external()
      // http://drupal.org/node/224333#entity_info_cache_clear --- New entity_info_cache_clear() API function
      // http://drupal.org/node/224333#hook_admin_paths --- New hooks: hook_admin_paths() and hook_admin_paths_alter()
      // http://drupal.org/node/224333#drupal_valid_path --- The function menu_valid_path() has been renamed to drupal_valid_path(), and its inputs have changed
      // http://drupal.org/node/224333#remove-blogapi --- Blog API module removed from Drupal core
      // http://drupal.org/node/224333#drupal_goto_params --- drupal_goto() follows parameters of url()
      // http://drupal.org/node/224333#format_date --- Format date types "small" and "large" have been changed to "short" and "long"
      // http://drupal.org/node/224333#url_query_parameter --- url() 'query' field must be array
      // http://drupal.org/node/224333#date_default_timezone --- User-configured time zones now serve as the default time zone for PHP date/time functions
  );
  $review = array(
    '#title' => t('Converting 6.x modules to 7.x'),
    '#link' => 'http://drupal.org/node/394070',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade7x' => $review);
}

/**
 * Define the rule callbacks for 7.x, see do_coder_review_callback().
 */
function _coder_review_7x_info_file_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $ignores = $coder_args['#ignore_lines'];
  $ignores = (empty($ignores) ? array() : $ignores[$review['#review_name']]);
  // Only perform this check for module's (not includes).
  $filename = $coder_args['#filename'];
  if (drupal_substr($filename, -7) == '.module') {
    // @NOTE: no simpletest for this.
    // Read the .info file.
    $files_line = FALSE;
    $core = FALSE;
    $filename = drupal_substr(realpath($filename), 0, -7) . '.info';
    if (file_exists($filename)) {
      if ($lines = file($filename)) {
        foreach ($lines as $lineno => $line) {
          if (preg_match('/^files\s*\[\s*\]\s*=/', $line)) {
            $files_line = TRUE;
          }
          if (preg_match('/^core\s*=\s*7/', $line)) {
            $core = TRUE;
          }
        }
        if (empty($files_line)) {
          $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
          $tmprule = $rule;
          $tmprule['#warning_callback'] = '_coder_review_7x_info_files_warning';
          _coder_review_error($results, $tmprule, $severity_name, $lineno, $line, $ignores);
        }
        if (!$core) {
          $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
          $tmprule = $rule;
          $tmprule['#warning_callback'] = '_coder_review_7x_core_warning';
          _coder_review_error($results, $tmprule, $severity_name, $lineno, $line, $ignores);
        }
      }
    }
  }
}

/**
 * Define the rule callbacks for 7.x, see do_coder_review_callback().
 */
function _coder_review_7x_module_implements_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $ignores = $coder_args['#ignore_lines'];
  $ignores = (empty($ignores) ? array() : $ignores[$review['#review_name']]);
  $depth = 0;
  foreach ($lines as $lineno => $line) {
    if ($depth == 0) {
      if (preg_match('/^\s+foreach\s*\(\s*module_list\s*\(\s*\)/', $line[0])) {
        $depth = 1;
      }
    }
    else {
      if (preg_match('/module_invoke\s*\(/', $line[0])) {
        $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
        $tmprule = $rule;
        $tmprule['#warning_callback'] = '_coder_review_7x_module_implements_warning';
        _coder_review_error($results, $tmprule, $severity_name, $lineno, $line[0], $ignores);
      }
      if (strpos($line[0], '{') !== FALSE) {
        $depth --;
      }
      if (strpos($line[0], '}') !== FALSE) {
        $depth ++;
      }
    }
  }
}

function _coder_review_7x_cache_implementation_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // @NOTE: no simpletest for this.
  $ignores = $coder_args['#ignore_lines'];
  $ignores = (empty($ignores) ? array() : $ignores[$review['#review_name']]);
  $filename = $coder_args['#filename'];
  if ($filename == 'includes/cache-install.inc' || $filename == 'includes/cache.inc') {
    return;
  }
  if (file_exists($filename)) {
    // Only do it for php files.
    if (drupal_substr($filename, -7) == '.module' || drupal_substr($filename, -4) == '.php' || drupal_substr($filename, -4) == '.inc') {
      if ($lines = file($filename)) {
        foreach ($lines as $lineno => $line) {
          if (preg_match('/function\s+(cache_set|cache_get|cache_clear_all)\s*\(/', $line)) {
            $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
            $tmprule = $rule;
            $tmprule['#warning_callback'] = '_coder_review_7x_cache_implementation_warning';
            _coder_review_error($results, $tmprule, $severity_name, $lineno, $line, $ignores);
          }
        }
      }
    }
  }
}

function _coder_review_7x_drupal_behaviors_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // @NOTE: no simpletest for this.
  $ignores = $coder_args['#ignore_lines'];
  $ignores = (empty($ignores) ? array() : $ignores[$review['#review_name']]);
  $filename = $coder_args['#filename'];
  if (file_exists($filename)) {
    // Only do it for js files.
    if (drupal_substr($filename, -3) == '.js') {
      if ($lines = file($filename)) {
        $detach = FALSE;
        foreach ($lines as $lineno => $line) {
          // New function, reset detach boolean.
          if (preg_match('/(^|[\s\(=])function\s*([a-zA-Z_]+?|\()/', $line)) {
            $detach = FALSE;
          }
          if (preg_match('/Drupal\.detachBehaviors\s*\(/', $line)) {
            $detach = TRUE;
          }
          if (!$detach && preg_match('/(jQuery\.|\$\.|\$\([a-zA-Z_#\'"\-]+?\)\.)(ajax|get|getJSON|post|load)/', $line)) {
            $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
            $tmprule = $rule;
            $tmprule['#warning_callback'] = '_coder_review_7x_drupal_behaviors_warning';
            _coder_review_error($results, $tmprule, $severity_name, $lineno, $line, $ignores);
          }
        }
      }
    }
  }
}

function _coder_review_7x_comment_doxygen_file_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  // @NOTE: no simpletest for this.
  // Only perform this check on install files.
  $ignores = $coder_args['#ignore_lines'];
  $ignores = (empty($ignores) ? array() : $ignores[$review['#review_name']]);
  $filename = $coder_args['#filename'];
  if (drupal_substr($filename, -7) == '.install') {
    $file_found = 0;
    $invalid_file_message = 0;
    foreach ($lines as $lineno => $line) {
      if (preg_match('/^ * @file/', $line[0])) {
        $file_found = 1;
      }
      // Immediately reset $file_found on the next line, and check if the
      // comment matches the required format.
      elseif ($file_found == 1) {
        if (!preg_match('/^ * Install, update and uninstall functions for the \w+ module./', $line[0])) {
          $invalid_file_message = 1;
        }
        $file_found = 0;
      }
    }
    if ($invalid_file_message) {
      $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
      $tmprule = $rule;
      $tmprule['#warning_callback'] = '_coder_review_7x_comment_doxygen_file_review_callback_warning';
      _coder_review_error($results, $tmprule, $severity_name, $theme_lineno, $theme_line, $ignores);
    }
  }
}

/**
 * Make sure user defined block module as a dependency if using block functions.
 *
 * @see http://drupal.org/node/224333#block_optional
 */
function _coder_review_7x_optional_block_review_callback(&$coder_args, $review, $rule, $lines, &$results) {
  $ignores = $coder_args['#ignore_lines'];
  $ignores = (empty($ignores) ? array() : $ignores[$review['#review_name']]);
  $functions_to_check = array('block_list', 'block_custom_block_get', 'block_page_build');
  $filename = $coder_args['#filename'];

  // Skip for unit tests as there is no physical file.
  if ($filename == 'snippet.php') {
    return;
  }

  // Find out the info filename and load it.
  $path_info = pathinfo($filename);
  list($modulename) = explode('.', $path_info['basename'], 2);
  $path = realpath($path_info['dirname'] . '/' . $modulename . '.info');
  if (!$path || !is_file($path)) {
    return;
  }
  $ini = drupal_parse_info_file($path);

  // Check if the dependency has been declared.
  $dependency_declared = (isset($ini['dependencies']) && is_array($ini['dependencies'])) ? in_array('block', $ini['dependencies']) : FALSE;

  if (file_exists($filename) && $modulename != 'block' && !$dependency_declared) {
    if ($lines = file($filename)) {
      foreach ($lines as $lineno => $line) {
        foreach ($functions_to_check as $function) {
          if (preg_match("/[\s\(]$function\(/", $line)) {
            $severity_name = _coder_review_severity_name($coder_args, $review, $rule);
            $tmprule = $rule;
            $tmprule['#warning_callback'] = '_coder_review_7x_optional_block_warning';
            _coder_review_error($results, $tmprule, $severity_name, $lineno, $line, $ignores);
          }
        }
      }
    }
  }
}

/**
 * Define the warning callbacks.
 */
function _coder_review_7x_core_warning() {
  return array(
    '#warning' => t('Info file should specify core=7.x'),
  );
}

function _coder_review_7x_comment_doxygen_file_review_callback_warning() {
  return array(
    '#warning' => t('For .install files, the @file description should be of the format "Install, update and uninstall functions for the XXX module.".'),
    '#link' => 'http://drupal.org/node/224333#afile',
  );
}

function _coder_review_7x_referer_uri_warning() {
  return array(
    '#warning' => t('!referer_uri() has been removed and replaced with the PHP global $_SERVER[\'HTTP_REFERER\']',
      array(
        '!referer_uri' => theme('drupalapi', array('function' => 'referer_uri', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#referer_uri',
  );
}

function _coder_review_7x_drupal_clone_warning() {
  return array(
    '#warning' => t('!drupal_clone() has been removed and the PHP 5 !clone should be used instead.',
      array(
        '!drupal_clone' => theme('drupalapi', array('function' => 'drupal_clone', 'version' => '6')),
        '!clone' => l(t('!clone', array('!clone' => 'clone')), 'http://us.php.net/language.oop5.cloning'),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_clone',
  );
}

function _coder_review_7x_node_invoke_nodeapi_removed_warning() {
  return array(
    '#warning' => t('!node_invoke_nodeapi() has been removed and !module_invoke_all(\'node_\' . $hook, $node); should be used instead.',
      array(
        '!node_invoke_nodeapi' => theme('drupalapi', array('function' => 'node_invoke_nodeapi', 'version' => '6')),
        '!module_invoke_all' => theme('drupalapi', array('function' => 'module_invoke_all', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_invoke_nodeapi',
  );
}

function _coder_review_7x_session_warning() {
  return array(
    '#warning' => t('Note that it is important that modules try not to use $_SESSION for anonymous users.'),
    '#link' => 'http://drupal.org/node/224333#drupal_set_session',
  );
}

function _coder_review_7x_db_rewrite_sql_warning() {
  return array(
    '#warning' => t('!db_rewrite_sql() replaced with !hook_query_alter()',
      array(
        '!db_rewrite_sql()' => theme('drupalapi', array('function' => 'db_rewrite_sql', 'version' => '6')),
        '!hook_query_alter()' => theme('drupalapi', array('function' => 'hook_query_alter', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#db_rewrite_sql',
  );
}

function _coder_review_7x_schema_translation_warning() {
  return array(
    '#warning' => t('Schema descriptions are no longer translated in !hook_schema()',
      array(
        '!hook_schema()' => theme('drupalapi', array('function' => 'hook_schema', 'version' => '6')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#schema_translation',
  );
}

function _coder_review_7x_comment_load_warning() {
  return array(
    '#warning' => t('!_comment_load() has been renamed to !comment_load()',
      array(
        '!_comment_load()' => theme('drupalapi', array('function' => '_comment_load', 'version' => '6')),
        '!comment_load()' => theme('drupalapi', array('function' => 'comment_load', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#comment_load',
  );
}

function _coder_review_7x_comment_wildcard_warning() {
  return array(
    '#warning' => t('%_comment wildcard has been renamed to %comment'),
    '#link' => 'http://drupal.org/node/224333#comment_load',
  );
}

function _coder_review_7x_taxonomy_tables_warning() {
  return array(
    '#warning' => t("Taxonomy DB table names have changed to begin with 'taxonomy_'"),
    '#link' => 'http://drupal.org/node/224333#taxonomy_tables',
  );
}

function _coder_review_7x_file_set_status_warning() {
  return array(
    '#warning' => t('!file_set_status() has been removed and !file_save() should be used in its place.',
      array(
        '!file_set_status()' => theme('drupalapi', array('function' => 'file_set_status', 'version' => '6')),
        '!file_save()' => theme('drupalapi', array('function' => 'file_save', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#file_set_status',
  );
}

function _coder_review_7x_user_delete_warning() {
  return array(
    '#warning' => t('!user_delete() replaced by !user_cancel()',
      array(
        '!user_delete()' => theme('drupalapi', array('function' => 'user_delete', 'version' => '6')),
        '!user_cancel()' => theme('drupalapi', array('function' => 'user_cancel', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#user_cancel',
  );
}

function _coder_review_7x_hook_user_op_warning() {
  return array(
    '#warning' => t('!hook_user() has been removed in favour of hook_user_$op functions, for example !hook_user_validate().',
      array(
        '!hook_user()' => theme('drupalapi', array('function' => 'hook_user', 'version' => '6')),
        '!hook_user_validate()' => theme('drupalapi', array('function' => 'hook_user_validate', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#remove_op',
  );
}

function _coder_review_7x_hook_nodeapi_op_warning() {
  return array(
    '#warning' => t('!hook_nodeapi() has been split into a number of smaller hooks.',
      array(
        '!hook_nodeapi()' => theme('drupalapi', array('function' => 'hook_nodeapi', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#remove_op',
  );
}

function _coder_review_7x_hook_node_info_node_content_warning() {
  return array(
    '#warning' => t('For content types managed by the node module, change "node" to "node_content" in !hook_node_info()',
      array(
        '!hook_node_info()' => theme('drupalapi', array('function' => 'hook_node_info', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_type_base',
  );
}

function _coder_review_7x_hook_node_info_module_warning() {
  return array(
    '#warning' => t('Change "module" to "base" in !hook_node_info()',
      array(
        '!hook_node_info()' => theme('drupalapi', array('function' => 'hook_node_info', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_type_base',
  );
}

function _coder_review_7x_hook_block_op_warning() {
  return array(
    '#warning' => t('!hook_block() has been split into a number of smaller hooks.',
      array(
        '!hook_block()' => theme('drupalapi', array('function' => 'hook_block', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#remove_op',
  );
}

function _coder_review_7x_user_logout_warning() {
  return array(
    '#warning' => t('Log out path changed from \'logout\' to \'user/logout\''),
    '#link' => 'http://drupal.org/node/224333#logout_path',
  );
}

function _coder_review_7x_time_warning() {
  return array(
    '#warning' => t('For improved performance, it is highly recommended that time() is replaced with REQUEST_TIME',
      array(
        '!time()' => theme('phpapi', array('function' => 'time')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#time',
  );
}

function _coder_review_7x_actions_synchronize_warning() {
  return array(
    '#warning' => t('Parameters for !actions_synchronize() have changed',
      array(
        '!actions_synchronize()' => theme('drupalapi', array('function' => 'actions_synchronize', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#actions_synchronize',
  );
}

function _coder_review_7x_check_markup_warning() {
  return array(
    '#warning' => t('Parameters for !check_markup() have changed',
      array(
        '!check_markup()' => theme('drupalapi', array('function' => 'check_markup', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#check_markup_params',
  );
}

function _coder_review_7x_drupal_set_title_warning() {
  return array(
    '#warning' => t('!drupal_set_title() now uses !check_plain() by default',
      array(
        '!drupal_set_title()' => theme('drupalapi', array('function' => 'drupal_set_title', 'version' => 7)),
        '!check_plain()' => theme('drupalapi', array('function' => 'check_plain', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_title',
  );
}

function _coder_review_7x_hook_filter_warning() {
  return array(
    '#warning' => t('!hook_filter() has been removed in favour of !hook_filter_info().',
      array(
        '!hook_filter_info()' => theme('drupalapi', array('function' => 'hook_filter_info', 'version' => 7)),
        '!hook_filter()' => theme('drupalapi', array('function' => 'hook_filter', 'version' => 6)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_filter_info',
  );
}

function _coder_review_7x_hook_filter_tips_warning() {
  return array(
    '#warning' => t('!hook_filter_tips() has been removed in favour of !hook_filter_info().',
      array(
        '!hook_filter_info()' => theme('drupalapi', array('function' => 'hook_filter_info', 'version' => 7)),
        '!hook_filter_tips()' => theme('drupalapi', array('function' => 'hook_filter_tips', 'version' => 6)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_filter_info',
  );
}

function _coder_review_7x_text_format_warning() {
  return array(
    '#warning' => t('New #text_format to assign text format selection to fields.'),
    '#link' => 'http://drupal.org/node/224333#text_format',
  );
}

function _coder_review_7x_comment_status_warning() {
  return array(
    '#warning' => t('Comment status values in the database have flipped so they match node status. We recommend you use constants like COMMENT_NOT_PUBLISHED in the future.'),
    '#link' => 'http://drupal.org/node/224333#comment_status',
  );
}

function _coder_review_7x_file_status_temporary_warning() {
  return array(
    '#warning' => t('Removed FILE_STATUS_TEMPORARY constant.'),
    '#link' => 'http://drupal.org/node/224333#remove_FILE_STATUS_TEMPORARY',
  );
}

function _coder_review_7x_theme_rebuild_function_warning() {
  return array(
    '#warning' => t('!drupal_rebuild_theme_registry() function has been renamed to !drupal_theme_rebuild()',
      array(
        '!drupal_rebuild_theme_registry()' => theme('drupalapi', array('function' => 'drupal_rebuild_theme_registry', 'version' => 6)),
        '!drupal_theme_rebuild()' => theme('drupalapi', array('function' => 'drupal_theme_rebuild', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#rebuild_functions',
  );
}

function _coder_review_7x_code_registry_rebuild_function_warning() {
  return array(
    '#warning' => t('!drupal_rebuild_code_registry() function has been renamed to !registry_rebuild()',
      array(
        '!drupal_rebuild_code_registry()' => theme('drupalapi', array('function' => 'drupal_rebuild_code_registry', 'version' => 6)),
        '!registry_rebuild()' => theme('drupalapi', array('function' => 'registry_rebuild', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#rebuild_functions',
  );
}

function _coder_review_7x_taxonomy_get_term_warning() {
  return array(
    '#warning' => t('!taxonomy_get_term() function has been renamed to !taxonomy_term_load()',
      array(
        '!taxonomy_get_term()' => theme('drupalapi', array('function' => 'taxonomy_get_term', 'version' => 6)),
        '!taxonomy_term_load()' => theme('drupalapi', array('function' => 'taxonomy_term_load', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_taxonomy_save_term_warning() {
  return array(
    '#warning' => t('!taxonomy_save_term() function has been renamed to !taxonomy_term_save() and takes a term object as a parameter instead of any array.',
      array(
        '!taxonomy_save_term()' => theme('drupalapi', array('function' => 'taxonomy_save_term', 'version' => 6)),
        '!taxonomy_term_save()' => theme('drupalapi', array('function' => 'taxonomy_term_save', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_taxonomy_del_term_warning() {
  return array(
    '#warning' => t('!taxonomy_del_term() function has been renamed to !taxonomy_term_delete()',
      array(
        '!taxonomy_del_term()' => theme('drupalapi', array('function' => 'taxonomy_del_term', 'version' => 6)),
        '!taxonomy_term_delete()' => theme('drupalapi', array('function' => 'taxonomy_term_delete', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_taxonomy_save_vocabulary_warning() {
  return array(
    '#warning' => t('!taxonomy_save_vocabulary() function has been renamed to !taxonomy_vocabulary_save() and takes a vocabulary object as a parameter instead of any array.',
      array(
        '!taxonomy_save_vocabulary()' => theme('drupalapi', array('function' => 'taxonomy_save_vocabulary', 'version' => 6)),
        '!taxonomy_vocabulary_save()' => theme('drupalapi', array('function' => 'taxonomy_vocabulary_save', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_taxonomy_del_vocabulary_warning() {
  return array(
    '#warning' => t('!taxonomy_del_vocabulary() function has been renamed to !taxonomy_vocabulary_delete()',
      array(
        '!taxonomy_del_vocabulary()' => theme('drupalapi', array('function' => 'taxonomy_del_vocabulary', 'version' => 6)),
        '!taxonomy_vocabulary_delete()' => theme('drupalapi', array('function' => 'taxonomy_vocabulary_delete', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_crud',
  );
}

function _coder_review_7x_info_files_warning() {
  return array(
    '#warning' => t('Module .info files must now specify all loadable code files explicitly.'),
    '#link' => 'http://drupal.org/node/224333#registry',
  );
}

function _coder_review_7x_drupal_execute_renamed_warning() {
  return array(
    '#warning' => t('!drupal_execute() has been renamed to !drupal_submit_form()',
      array(
        '!drupal_execute()' => theme('drupalapi', array('function' => 'drupal_execute', 'version' => 6)),
        '!drupal_submit_form()' => theme('drupalapi', array('function' => 'drupal_submit_form', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_execute_drupal_form_submit',
  );
}

function _coder_review_7x_node_view_warning() {
  return array(
    '#warning' => t('!node_view() has been removed and !node_build() should be used instead.',
      array(
        '!node_view' => theme('drupalapi', array('function' => 'node_view', 'version' => 6)),
        '!node_build()' => theme('drupalapi', array('function' => 'node_build', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_view',
  );
}

function _coder_review_7x_comment_validate_warning() {
  return array(
    '#warning' => t('!comment_validate() has been removed and !comment_form_validate() should be used instead.',
      array(
        '!comment_validate' => theme('drupalapi', array('function' => 'comment_validate', 'version' => 6)),
        '!comment_form_validate()' => theme('drupalapi', array('function' => 'comment_form_validate', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#comment_validate_removed',
  );
}

function _coder_review_7x_comment_node_url_warning() {
  return array(
    '#warning' => t('!comment_node_url() has been removed. Use "node/" .  $comment->nid to build URLs instead.',
      array(
        '!comment_node_url' => theme('drupalapi', array('function' => 'comment_node_url', 'version' => 6)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#comment_node_url',
  );
}

function _coder_review_7x_node_load_warning() {
  return array(
    '#warning' => t('!node_load() now only takes nid and vid as parameters.',
      array(
        '!node_load' => theme('drupalapi', array('function' => 'node_load', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_load_multiple',
  );
}

function _coder_review_7x_hook_perm_warning() {
  return array(
    '#warning' => t('!hook_perm() has been renamed to !hook_permission().',
      array(
        '!hook_perm' => theme('drupalapi', array('function' => 'hook_perm', 'version' => 6)),
        '!hook_permission' => theme('drupalapi', array('function' => 'hook_permission', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_permission',
  );
}

function _coder_review_7x_absolute_includes_warning() {
  return array(
    '#warning' => t('Use absolute path, constructed from DRUPAL_ROOT, when including a file.'),
    '#link' => 'http://drupal.org/node/224333#absolute_includes',
  );
}

function _coder_review_7x_filter_urls_warning() {
  return array(
    '#warning' => t('Administrative URLs in the Filter module have been renamed from plural to singular.'),
    '#link' => 'http://drupal.org/node/224333#filter_urls',
  );
}

function _coder_review_7x_taxonomy_get_tree_warning() {
  return array(
    '#warning' => t('!taxonomy_get_tree()\'s $depth and $max_depth parameters have changed positions.',
      array(
        '!taxonomy_get_tree' => theme('drupalapi', array('function' => 'taxonomy_get_tree', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#taxonomy_get_tree',
  );
}

function _coder_review_7x_taxonomy_paths_warning() {
  return array(
    '#warning' => t('Support for taxonomy term paths with multiple tids and depth specification has been removed.'),
    '#link' => 'http://drupal.org/node/224333#taxonomy_paths',
  );
}

function _coder_review_7x_book_toc_warning() {
  return array(
    '#warning' => t('!book_toc()\'s $depth_limit and $exclude parameters have changed positions.',
      array(
        '!book_toc' => theme('drupalapi', array('function' => 'book_toc', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#book_toc_parameters',
  );
}

function _coder_review_7x_node_type_get_functions_warning() {
  return array(
    '#warning' => t('!node_get_types() function has been replaced by node_type_get_$op() functions.',
      array(
        '!node_get_types' => theme('drupalapi', array('function' => 'node_get_types', 'version' => 6)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_type_get_functions',
  );
}

function _coder_review_7x_node_build_rss_warning() {
  return array(
    '#warning' => t('Removed $op "rss item" from !hook_nodeapi() in favor of NODE_BUILD_RSS.',
      array(
        '!hook_nodeapi()' => theme('drupalapi', array('function' => 'hook_nodeapi', 'version' => '7')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_build_rss',
  );
}

function _coder_review_7x_build_mode_warning() {
  return array(
    '#warning' => t('$node->build_mode property removed.'),
    '#link' => 'http://drupal.org/node/224333#build_mode',
  );
}

function _coder_review_7x_node_build_preview_warning() {
  return array(
    '#warning' => t('NODE_BUILD_PREVIEW has been replaced by the $node->in_preview boolean flag.'),
    '#link' => 'http://drupal.org/node/224333#build_mode',
  );
}

function _coder_review_7x_node_links_warning() {
  return array(
    '#warning' => t('Node and taxonomy links are no longer emitted by !hook_link() and !hook_link_alter().',
      array(
        '!hook_link()' => theme('drupalapi', array('function' => 'hook_link', 'version' => 7)),
        '!hook_link_alter()' => theme('drupalapi', array('function' => 'hook_link_alter', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_links',
  );
}

function _coder_review_7x_drupal_uninstall_modules_warning() {
  return array(
    '#warning' => t('!drupal_uninstall_module() has been renamed to !drupal_uninstall_modules() and now takes an array of module names.',
      array(
        '!drupal_uninstall_module()' => theme('drupalapi', array('function' => 'drupal_uninstall_module', 'version' => 6)),
        '!drupal_uninstall_modules()' => theme('drupalapi', array('function' => 'drupal_uninstall_modules', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_uninstall_modules',
  );
}

function _coder_review_7x_drupal_http_request_parameters_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_http_request() have changed.',
      array(
        '!drupal_http_request()' => theme('drupalapi', array('function' => 'drupal_http_request', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_http_request_parameters',
  );
}

function _coder_review_7x_moved_statistics_settings_warning() {
  return array(
    '#warning' => t('Statistics settings have moved to "admin/config/system/statistics" and there is a new "administer statistics" permission.'),
    '#link' => 'http://drupal.org/node/224333#moved_statistics_settings',
  );
}

function _coder_review_7x_drupal_set_html_head_warning() {
  return array(
    '#warning' => t('!drupal_set_html_head() has been renamed to !drupal_add_html_head().',
      array(
        '!drupal_set_html_head()' => theme('drupalapi', array('function' => 'drupal_set_html_head', 'version' => 6)),
        '!drupal_add_html_head()' => theme('drupalapi', array('function' => 'drupal_add_html_head', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_html_head',
  );
}

function _coder_review_7x_php_eval_warning() {
  return array(
    '#warning' => t("!drupal_eval() has been renamed to !php_eval() and should be wrapped in a module_exists('php') check.",
      array(
        '!drupal_eval()' => theme('drupalapi', array('function' => 'drupal_eval', 'version' => 6)),
        '!php_eval()' => theme('drupalapi', array('function' => 'php_eval', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#php_eval',
  );
}

function _coder_review_7x_module_rebuild_cache_warning() {
  return array(
    '#warning' => t('!module_rebuild_cache() has been renamed to !system_get_module_data().',
      array(
        '!module_rebuild_cache()' => theme('drupalapi', array('function' => 'module_rebuild_cache', 'version' => 6)),
        '!system_get_module_data()' => theme('drupalapi', array('function' => 'system_get_module_data', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#system_get_module_data',
  );
}

function _coder_review_7x_system_theme_data_warning() {
  return array(
    '#warning' => t('!system_theme_data() has been renamed to !system_get_theme_data().',
      array(
        '!system_theme_data()' => theme('drupalapi', array('function' => 'system_theme_data', 'version' => 6)),
        '!system_get_theme_data()' => theme('drupalapi', array('function' => 'system_get_theme_data', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#system_get_module_data',
  );
}

function _coder_review_7x_drupal_set_content_warning() {
  return array(
    '#warning' => t('!drupal_set_content() has been renamed to !drupal_add_region_content().',
      array(
        '!drupal_set_content()' => theme('drupalapi', array('function' => 'drupal_set_content', 'version' => 6)),
        '!drupal_add_region_content()' => theme('drupalapi', array('function' => 'drupal_add_region_content', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_content',
  );
}

function _coder_review_7x_drupal_get_content_warning() {
  return array(
    '#warning' => t('!drupal_get_content() has been renamed to !drupal_get_region_content().',
      array(
        '!drupal_get_content()' => theme('drupalapi', array('function' => 'drupal_get_content', 'version' => 6)),
        '!drupal_get_region_content()' => theme('drupalapi', array('function' => 'drupal_get_region_content', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_set_content',
  );
}

function _coder_review_7x_http_header_functions_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_set_header() have changed and it has been renamed to !drupal_add_http_header().',
      array(
        '!drupal_set_header()' => theme('drupalapi', array('function' => 'drupal_set_header', 'version' => 6)),
        '!drupal_add_http_header()' => theme('drupalapi', array('function' => 'drupal_add_http_header', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#http_header_functions',
  );
}

function _coder_review_7x_cache_implementation_warning() {
  return array(
    '#warning' => t('Alternative cache implementations have changed.'),
    '#link' => 'http://drupal.org/node/224333#cache_implementation',
  );
}

// No link!
function _coder_review_7x_node_revisions_table_warning() {
  return array(
    '#warning' => t("The 'node_revisions' table has been renamed to 'node_revision'."),
    //'#link' => 'http://drupal.org/node/224333#taxonomy_tables',
  );
}

function _coder_review_7x_user_load_warning() {
  return array(
    '#warning' => t('!user_load() now only takes a user ID as its argument.',
      array(
        '!user_load()' => theme('drupalapi', array('function' => 'user_load', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#user_load_multiple',
  );
}

function _coder_review_7x_hook_footer_warning() {
  return array(
    '#warning' => t('!hook_footer() and !theme_closure() were removed.',
      array(
        '!hook_footer()' => theme('drupalapi', array('function' => 'hook_footer', 'version' => 6)),
        '!theme_closure()' => theme('drupalapi', array('function' => 'theme_closure', 'version' => 6)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_footer',
  );
}

function _coder_review_7x_closure_warning() {
  return array(
    '#warning' => t('$closure has been changed to $page_bottom.'),
    '#link' => 'http://drupal.org/node/224333#hook_footer',
  );
}

function _coder_review_7x_theme_page_warning() {
  return array(
    '#warning' => t('Instead of "theme(\'page\')" use !drupal_set_page_content()',
      array(
        '!drupal_set_page_content()' => theme('drupalapi', array('function' => 'drupal_set_page_content', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#theme_page',
  );
}

function _coder_review_7x_drupal_add_js_parameters_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_add_js() have changed.',
      array(
        '!drupal_add_js()' => theme('drupalapi', array('function' => 'drupal_add_js', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_add_js_options',
  );
}

function _coder_review_7x_drupal_add_css_parameters_warning() {
  return array(
    '#warning' => t('Parameters to !drupal_add_css() have changed.',
      array(
        '!drupal_add_css()' => theme('drupalapi', array('function' => 'drupal_add_css', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_add_js_options',
  );
}

function _coder_review_7x_jquery_13_warning() {
  return array(
    '#warning' => t('jQuery has been upgraded to version 1.3.'),
    '#link' => 'http://drupal.org/node/224333#jquery_13',
  );
}

function _coder_review_7x_hook_js_alter_warning() {
  return array(
    '#warning' => t('There is a new !hook_js_alter() to alter JavaScript.',
      array(
        '!hook_js_alter()' => theme('drupalapi', array('function' => 'hook_js_alter', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_js_alter',
  );
}

function _coder_review_7x_drupal_add_js_weight_warning() {
  return array(
    '#warning' => t("Replace 'core', 'module' and 'theme' with 'file' in !drupal_add_js()",
      array(
        '!drupal_add_js()' => theme('drupalapi', array('function' => 'drupal_add_js', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_add_js_weight',
  );
}

function _coder_review_7x_drupal_behaviors_warning() {
  return array(
    '#warning' => t('Changed Drupal.behaviors to objects having the methods "attach" and "detach".'),
    '#link' => 'http://drupal.org/node/224333#drupal_behaviors',
  );
}

function _coder_review_7x_drupal_add_js_external_warning() {
  return array(
    '#warning' => t('External JavaScript can now be referenced through !drupal_add_js().',
      array(
        '!drupal_add_js()' => theme('drupalapi', array('function' => 'drupal_add_js', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#drupal_add_js_external',
  );
}

function _coder_review_7x_javascript_compatibility_warning() {
  return array(
    '#warning' => t('JavaScript should be compatible with other libraries than jQuery.'),
    '#link' => 'http://drupal.org/node/224333#javascript_compatibility',
  );
}

function _coder_review_7x_file_scan_directory_array_itize_warning() {
  return array(
    '#warning' => t('Parameters to !file_scan_directory() have changed.',
      array(
        '!file_scan_directory()' => theme('drupalapi', array('function' => 'file_scan_directory', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#file_scan_directory_array_itize',
  );
}

function _coder_review_7x_file_scan_directory_nomask_warning() {
  return array(
    '#warning' => t('!file_scan_directory() now uses a preg regular expression for the nomask parameter.',
      array(
        '!file_scan_directory()' => theme('drupalapi', array('function' => 'file_scan_directory', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#file_scan_directory_nomask',
  );
}

function _coder_review_7x_drupal_system_listing_preg_match_warning() {
  return array(
    '#warning' => t('!drupal_system_listing() now uses preg regular expressions.',
      array(
        '!drupal_system_listing()' => theme('drupalapi', array('function' => 'drupal_system_listing', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#preg_match',
  );
}

function _coder_review_7x_file_scan_directory_preg_match_warning() {
  return array(
    '#warning' => t('!file_scan_directory() now uses preg regular expressions.',
      array(
        '!file_scan_directory()' => theme('drupalapi', array('function' => 'file_scan_directory', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#preg_match',
  );
}

function _coder_review_7x_permission_tables_warning() {
  return array(
    '#warning' => t("The 'permission' table has been renamed to 'role_permission' and has a different structure."),
    '#link' => 'http://drupal.org/node/224333#permission_tables',
  );
}

function _coder_review_7x_descriptions_permissions_warning() {
  return array(
    '#warning' => t('Permissions are required to have titles and descriptions in !hook_permission().',
      array(
        '!hook_permission()' => theme('drupalapi', array('function' => 'hook_permission', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#descriptions_permissions',
  );
}

function _coder_review_7x_php_permissions_warning() {
  return array(
    '#warning' => t('The "use PHP for block visibility" permission has been replaced by the more generic "use PHP for settings" permission.'),
    '#link' => 'http://drupal.org/node/224333#php_permissions',
  );
}

function _coder_review_7x_hook_menu_link_alter_warning() {
  return array(
    '#warning' => t('The parameters to !hook_menu_link_alter() have changed.',
      array(
        '!hook_menu_link_alter()' => theme('drupalapi', array('function' => 'hook_menu_link_alter', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_menu_link_alter',
  );
}

function _coder_review_7x_time_limit_warning() {
  return array(
    '#warning' => t('Use !drupal_set_time_limit() instead of !set_time_limit().',
      array(
        '!drupal_set_time_limit()' => theme('drupalapi', array('function' => 'drupal_set_time_limit', 'version' => 7)),
        '!set_time_limit()' => theme('phpapi', array('function' => 'set_time_limit')),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#time_limit',
  );
}

function _coder_review_7x_drupal_urlencode_warning() {
  return array(
    '#warning' => t('Use !drupal_encode_path() instead of !drupal_urlencode().',
      array(
        '!drupal_encode_path()' => theme('drupalapi', array('function' => 'drupal_encode_path', 'version' => 7)),
        '!drupal_urlencode()' => theme('dr)upalapi', array('function' => 'drupal_urlencode', 'version' => 6)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#remove-drupal-urlencode',
  );
}

function _coder_review_7x_user_authenticate_warning() {
  return array(
    '#warning' => t('Parameters to !user_authenticate() have changed.',
      array(
        '!user_authenticate()' => theme('drupalapi', array('function' => 'user_authenticate', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#user_authenticate',
  );
}

function _coder_review_7x_hook_node_xxx_warning() {
  return array(
    '#warning' => t('hook_nodeapi_xxx() has been renamed to hook_node_xxx().'),
    '#link' => 'http://drupal.org/node/224333#hook_node_xxx',
  );
}

function _coder_review_7x_related_terms_warning() {
  return array(
    '#warning' => t('The related terms functionality has been removed from the taxonomy module.'),
    '#link' => 'http://drupal.org/node/224333#related_terms',
  );
}

function _coder_review_7x_schema_html_warning() {
  return array(
    '#warning' => t('Schema descriptions are now plain text instead of HTML.'),
    '#link' => 'http://drupal.org/node/224333#schema_html',
  );
}

function _coder_review_7x_node_form_warning() {
  return array(
    '#warning' => t('Easier check for node form during !hook_form_alter().',
      array(
        '!hook_form_alter()' => theme('drupalapi', array('function' => 'hook_form_alter', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#node_form',
  );
}

function _coder_review_7x_hook_form_alter_warning() {
  return array(
    '#warning' => t('Parameters to !hook_form_alter() have changed.',
      array(
        '!hook_form_alter()' => theme('drupalapi', array('function' => 'hook_form_alter', 'version' => 7)),
      )
    ),
  //  '#link' => 'http://drupal.org/node/224333#node_form',
  );
}

function _coder_review_7x_process_functions_warning() {
  return array(
    '#warning' => t('!expand_password_confirm(), !expand_date(), !expand_radios(), !form_expand_ahah(), !expand_checkboxes(), !process_weight()  have been renamed.',
      array(
        '!expand_password_confirm()' => theme('drupalapi', array('function' => 'expand_password_confirm', 'version' => 6)),
        '!expand_date()' => theme('drupalapi', array('function' => 'expand_date', 'version' => 6)),
        '!expand_radios()' => theme('drupalapi', array('function' => 'expand_radios', 'version' => 6)),
        '!form_expand_ahah()' => theme('drupalapi', array('function' => 'form_expand_ahah', 'version' => 6)),
        '!expand_checkboxes()' => theme('drupalapi', array('function' => 'expand_checkboxes', 'version' => 6)),
        '!process_weight()' => theme('drupalapi', array('function' => 'process_weight', 'version' => 6)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#process_functions',
  );
}

function _coder_review_7x_no_jsenabled_warning() {
  return array(
    '#warning' => t('JavaScript variable Drupal.jsEnabled has been removed.'),
    '#link' => 'http://drupal.org/node/224333#no-jsenabled',
  );
}

function _coder_review_7x_optional_block_warning() {
  return array(
    '#warning' => t("Block module is now optional, make sure you declare it as a dependency in your module's .info file."),
    '#link' => 'http://drupal.org/node/224333#block_optional',
  );
}

function _coder_review_7x_module_implements_warning() {
  return array(
    '#warning' => t('Use module_implements not module_list when calling hook implementations.'),
    '#link' => 'http://drupal.org/node/224333#module_implements_not_module_list',
  );
}

function _coder_review_7x_ahah_now_ajax_warning() {
  return array(
    '#warning' => t('#ahah has changed to #ajax and the call details have changed.'),
    '#link' => 'http://drupal.org/node/224333#ahah-now-ajax',
  );
}

function _coder_review_7x_select_count_warning() {
  return array(
    '#warning' => t('You may not want to use SELECT COUNT(*), if all you want to do is check for the existence of any rows, rather than actually counting the number of rows.'),
    '#link' => 'http://drupal.org/node/224333#select_count',
  );
}

function _coder_review_7x_hook_access_warning() {
  return array(
    '#warning' => t('!hook_access() removed in favor of !hook_node_access().',
      array(
        '!hook_access()' => theme('drupalapi', array('function' => 'hook_access', 'version' => 6)),
        '!hook_node_access()' => theme('drupalapi', array('function' => 'hook_node_access', 'version' => 7)),
      )
    ),
    '#link' => 'http://drupal.org/node/224333#hook_node_access',
  );
}

function _coder_review_7x_hook_profile_alter_warning() {
  return array(
    '#warning' => t('!hook_profile_alter() has been removed in favour of !hook_user_view().',
    array(
      '!hook_profile_alter()' => theme('drupalapi', array('function' => 'hook_profile_alter', 'version' => 6)),
      '!hook_user_view()' => theme('drupalapi', array('function' => 'hook_user_view', 'version' => 7)),
    )),
  );
}

function _coder_review_7x_t_signature_warning() {
  return array(
    '#warning' => t('Context has been added to !t() as the third parameter, locale has to be an element in the array. Example: array("context" => "frontpage", "locale" => "de").',
      array(
        '!t()' => theme('drupalapi', array('function' => 't', 'version' => 7)),
      )),
    '#link' => 'http://drupal.org/node/224333#locale_context',
  );
}

function _coder_review_7x_format_plural_signature_warning() {
  return array(
    '#warning' => t('Context has been added to !format_plural() as the fifth parameter, locale has to be an element in the array. Example: array("context" => "frontpage", "locale" => "de").',
      array(
        '!format_plural()' => theme('drupalapi', array('function' => 'format_plural', 'version' => 7)),
      )),
    '#link' => 'http://drupal.org/node/224333#locale_context',
  );
}

function _coder_review_7x_drupal_set_get_header_warning() {
  return array(
    '#warning' => t('!drupal_set_header() has been renamed to !drupal_add_http_header() and its parameters changed, and !drupal_get_headers() renamed to !drupal_get_http_header()',
      array(
        '!drupal_set_header()' => theme('drupalapi', array('function' => 'drupal_set_header', 'version' => 6)),
        '!drupal_get_headers()' => theme('drupalapi', array('function' => 'drupal_get_headers', 'version' => 6)),
        '!drupal_add_http_header()' => theme('drupalapi', array('function' => 'drupal_add_http_header', 'version' => 7)),
        '!drupal_get_http_header()' => theme('drupalapi', array('function' => 'drupal_get_http_header', 'version' => 7)),
      )),
    '#link' => 'http://drupal.org/node/224333#drupal_set_header_renamed',
  );
}

function _coder_review_7x_watchdog_emerg_warning() {
  return array(
    '#warning' => t('WATCHDOG_EMERG was renamed to WATCHDOG_EMERGENCY'),
    '#link' => 'http://drupal.org/node/224333#watchdog_emerg',
  );
}

function _coder_review_7x_update_sql_warning() {
  return array(
    '#warning' => t('!update_sql() is now deprecated',
      array(
        '!update_sql()' => theme('drupalapi', array('function' => 'update_sql', 'version' => 6)),
      )),
    '#link' => 'http://drupal.org/node/224333#update_sql',
  );
}

function _coder_review_7x_update_hook_ret_warning() {
  return array(
    '#warning' => t('Update hooks now return translated strings or throw exceptions'),
    '#link' => 'http://drupal.org/node/224333#update_sql',
  );
}

function _coder_review_7x_hook_search_warning() {
  return array(
    '#warning' => t('!hook_search() has been separated out into several other hooks: !hook_search_info(), !hook_search_admin(), !hook_search_execute(), !hook_search_reset(), !hook_search_status()',
    array(
      '!hook_search()' => theme('drupalapi', array('function' => 'hook_search', 'version' => 6)),
      '!hook_search_info()' => theme('drupalapi', array('function' => 'hook_search_info', 'version' => 7)),
      '!hook_search_admin()' => theme('drupalapi', array('function' => 'hook_search_admin', 'version' => 7)),
      '!hook_search_execute()' => theme('drupalapi', array('function' => 'hook_search_execute', 'version' => 7)),
      '!hook_search_reset()' => theme('drupalapi', array('function' => 'hook_search_reset', 'version' => 7)),
      '!hook_search_status()' => theme('drupalapi', array('function' => 'hook_search_status', 'version' => 7)),
    )),
    '#link' => 'http://drupal.org/node/224333#search-api',
  );
}

function _coder_review_7x_do_search_warning() {
  return array(
    '#warning' => t('!do_search() has been deprecated and instead modules should perform the search query using the "SearchQuery" extension.',
    array(
      '!do_search()' => theme('drupalapi', array('function' => 'do_search', 'version' => 6)),
    )),
    '#link' => 'http://drupal.org/node/224333#search-api',
  );
}

function _coder_review_7x_install_schema_warning() {
  return array(
    '#warning' => t('A module no longer should explicitly install or uninstall its database schema in !hook_install() or !hook_uninstall().',
    array(
      '!hook_install()' => theme('drupalapi', array('function' => 'hook_install', 'version' => 7)),
      '!hook_uninstall()' => theme('drupalapi', array('function' => 'hook_uninstall', 'version' => 7)),
    )),
    '#link' => 'http://drupal.org/node/224333#install-schema',
  );
}

