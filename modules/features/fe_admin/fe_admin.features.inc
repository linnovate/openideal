<?php
/**
 * @file
 * fe_admin.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_admin_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "panels" && $api == "layouts") {
    return array("version" => "1");
  }
}
