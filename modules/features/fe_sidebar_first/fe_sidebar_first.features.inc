<?php
/**
 * @file
 * fe_sidebar_first.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_sidebar_first_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "panels_mini" && $api == "panels_default") {
    return array("version" => "1");
  }
}
