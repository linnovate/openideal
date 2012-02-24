<?php
/**
 * @file
 * add_idea_autocompleate.features.inc
 */

/**
 * Implements hook_views_api().
 */
function add_idea_autocompleate_views_api() {
  list($module, $api) = func_get_args();
  if ($module == "views" && $api == "views_default") {
    return array("version" => "3.0");
  }
}
