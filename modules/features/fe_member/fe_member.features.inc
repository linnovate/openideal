<?php
/**
 * @file
 * fe_member.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_member_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
}
