<?php

/**
 * Implementation of hook_ctools_plugin_api().
 */
function fe_news_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => 3);
  }
  elseif ($module == "strongarm" && $api == "strongarm") {
    return array("version" => 1);
  }
}

/**
 * Implementation of hook_node_info().
 */
function fe_news_node_info() {
  $items = array(
    'news' => array(
      'name' => t('News'),
      'module' => 'features',
      'description' => t('News and updates'),
      'has_title' => '1',
      'title_label' => t('title'),
      'has_body' => '1',
      'body_label' => t('body'),
      'min_word_count' => '0',
      'help' => '',
    ),
  );
  return $items;
}

/**
 * Implementation of hook_views_api().
 */
function fe_news_views_api() {
  return array(
    'api' => '2',
  );
}
