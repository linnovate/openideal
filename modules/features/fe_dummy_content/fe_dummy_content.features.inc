<?php
/**
 * @file
 * fe_dummy_content.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_dummy_content_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}
