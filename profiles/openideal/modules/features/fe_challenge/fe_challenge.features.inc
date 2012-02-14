<?php

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_challenge_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implements hook_node_info().
 */
function fe_challenge_node_info() {
  $items = array(
    'challenge' => array(
      'name' => t('Challenge'),
      'base' => 'features',
      'description' => '',
      'has_title' => '1',
      'title_label' => t('Challenge'),
      'has_body' => '1',
      'body_label' => t('description'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implements hook_views_api().
 */
function fe_challenge_views_api() {
  return array(
    'api' => '2',
  );
}
