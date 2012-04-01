<?php

/**
 * @file
 * This include file implements coder_review functionality for 4.6 -> 4.7 upgrades.
 */

/**
 * Implements hook_reviews().
 */
function coder_review_47_reviews() {
  $rules = array(
    array(
      '#type' => 'regex',
      '#value' => 'function\s+\w+(_node_name|_node_type)\s*\(',
      '#warning_callback' => '_coder_review_47_hook_node_info_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_load\s*\(\s*array\s*\(',
      '#warning_callback' => '_coder_review_47_node_load_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_list\s*\(',
      '#warning_callback' => '_coder_review_47_node_list_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]module_get_node_name\s*\(',
      '#warning_callback' => '_coder_review_47_module_get_name_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]format_name\s*\(',
      '#warning_callback' => '_coder_review_47_format_name_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '^\s*(taxonomy_save_vocabulary|taxonomy_save_term)\s*\(',
      '#warning' => 'check the return and display a status message',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]message_access\s*\(',
      '#warning_callback' => '_coder_review_47_message_access_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(](strlen|strtolower|strtoupper|substr|ucfirst)\s*\(',
      '#warning' => 'in most cases, replace the string function with the drupal_ equivalent string functions',
      '#severity' => 'minor',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]conf_url_rewrite\s*\(',
      '#warning_callback' => '_coder_review_47_conf_url_rewrite_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_delete\s*\(\s*array\s*\(',
      '#warning_callback' => '_coder_review_47_node_delete_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '[\s\(]variable_get\s*\(\s*[\'"]+file_directory_temp[\'"]+\s*,',
      '#warning_callback' => '_coder_review_47_file_directory_temp_warning',
    ),
    array(
      '#type' => 'regex',
      '#source' => 'allphp',
      '#value' => '[\s\(]variable_get\s*\(\s*[\'"]+file_directory_path[\'"]+\s*,',
      '#warning_callback' => '_coder_review_47_file_directory_path_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]array2object\s*\(',
      '#warning_callback' => '_coder_review_47_array2object_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => 'function\s+\w+_onload\s*\(',
      '#warning_callback' => '_coder_review_47_hook_onload_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]node_validate_title\s*\(',
      '#warning_callback' => '_coder_review_47_node_validate_title_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]tablesort_pager\s*\(',
      '#warning_callback' => '_coder_review_47_tablesort_pager_warning',
    ),
    array(
      '#type' => 'regex',
      '#value' => '[\s\(]form_(textfield|radio|group|select|checkbox|textarea)\s*\(',
      '#warning' => 'replace form_ functions with the forms api',
    ),
  );
  $review = array(
    '#title' => t('Converting 4.6.x modules to 4.7.x'),
    '#link' => 'http://drupal.org/node/22218',
    '#rules' => $rules,
    '#severity' => 'critical',
  );
  return array('upgrade47' => $review);
}

/**
 * Define the warning callbacks.
 */

function _coder_review_47_hook_node_info_warning() {
  return t('implement !hook_node_info() to create a module which defines node type(s)',
    array(
      '!hook_node_info' => theme('drupalapi', array('function' => 'hook_node_info', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_node_load_warning() {
  return t('use !node_load($nid) instead of !node_load(array(\'nid\' => $nid))',
    array(
      '!node_load' => theme('drupalapi', array('function' => 'node_load', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_node_list_warning() {
  return t('!node_list() became node_get_types and now returns an associative array about node types',
    array(
      '!node_list' => theme('drupalapi', array('function' => 'node_list', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_module_get_name_warning() {
  return t('!module_get_node_name() deprecated and now handled by !node_get_base().',
    array(
      '!module_get_node_name' => theme('drupalapi', array('function' => 'module_get_node_name', 'version' => '4.6')),
      '!node_get_base' => theme('drupalapi', array('function' => 'node_get_base', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_format_name_warning() {
  return t('!format_name() was renamed to !theme_username()',
    array(
      '!format_name' => theme('drupalapi', array('function' => 'format_name', 'version' => '4.6')),
      '!theme_username' => theme('drupalapi', array('function' => 'theme_username', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_message_access_warning() {
  return t('!message_access() was removed, replace with a nice case error message',
    array(
      '!message_access' => theme('drupalapi', array('function' => 'message_access', 'version' => '4.6')),
    )
  );
}

function _coder_review_47_conf_url_rewrite_warning() {
  return t('!conf_url_rewrite() became !custom_url_rewrite()',
    array(
      '!conf_url_rewrite' => theme('drupalapi', array('function' => 'conf_url_rewrite', 'version' => '4.6')),
      '!custom_url_rewrite' => theme('drupalapi', array('function' => 'custom_url_rewrite', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_node_delete_warning() {
  return t('use !node_delete($nid) instead of !node_delete(array(\'nid\' => $nid))',
    array(
      '!node_delete' => theme('drupalapi', array('function' => 'node_delete', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_file_directory_temp_warning() {
  return t('use !file_directory_temp() function instead of variable',
    array(
      '!file_directory_temp' => theme('drupalapi', array('function' => 'file_directory_temp', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_file_directory_path_warning() {
  return t('use !file_directory_path() function instead of variable',
    array(
      '!file_directory_path' => theme('drupalapi', array('function' => 'file_directory_path', 'version' => '4.7')),
    )
  );
}

function _coder_review_47_array2object_warning() {
  return t('!array2object() replaced by native PHP type conversion (typecase to (object)',
    array(
      '!array2object' => theme('drupalapi', array('function' => 'array2object', 'version' => '4.6')),
    )
  );
}

function _coder_review_47_hook_onload_warning() {
  return t('!hook_onload() replaced by javascript addLoadEvent()',
    array(
      '!hook_onload' => theme('drupalapi', array('function' => 'hook_onload', 'version' => '4.6')),
    )
  );
}

function _coder_review_47_node_validate_title_warning() {
  return t('!node_validate_title() was removed',
    array(
      '!node_validate_title' => theme('drupalapi', array('function' => 'node_validate_title', 'version' => '4.6')),
    )
  );
}

function _coder_review_47_node_tablesort_pager_warning() {
  return t('!tablesort_pager() was removed',
    array(
      '!tablesort_pager' => theme('drupalapi', array('function' => 'tablesort_pager', 'version' => '4.6')),
    )
  );
}
