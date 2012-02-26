<?php
/**
 * @file
 * ideas_projects_manager.features.inc
 */

/**
 * Implements hook_views_api().
 */
function ideas_projects_manager_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
