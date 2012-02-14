<?php
// $Id: nodewords_custom_pages.nodewords.hooks.inc,v 1.1.2.1 2010/03/21 13:16:56 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
 * Implements hook_nodewords_delete_tags().
 */
function nodewords_custom_pages_nodewords_delete_tags($options) {
  if ($options['type'] == NODEWORDS_TYPE_PAGE) {
    db_query("DELETE FROM {nodewords_custom} WHERE pid = '%s'", $options['id']);
  }
}

/**
 * Implements hook_nodewords_type_id().
 */
function nodewords_custom_pages_nodewords_type_id(&$result, $arg) {
  foreach (_nodewords_custom_pages_load_data() as $page) {
    $path = $page->path;

    if (drupal_match_path($_GET['q'], $path)) {
      $result['type'] = NODEWORDS_TYPE_PAGE;
      $result['id'] = $page->pid;
      break;
    }

    $bool = (
      ($alias = drupal_get_path_alias($_GET['q'])) != $_GET['q'] &&
      drupal_match_path($alias, $path)
    );
    if ($bool) {
      $result['type'] = NODEWORDS_TYPE_PAGE;
      $result['id'] = $page->pid;
      break;
    }
  }
}
