<?php
/**
 * @file
 * fe_user.quicktabs.inc
 */

/**
 * Implements hook_quicktabs_default_quicktabs().
 */
function fe_user_quicktabs_default_quicktabs() {
  $export = array();

  $quicktabs = new stdClass();
  $quicktabs->disabled = FALSE; /* Edit this to true to make a default quicktabs disabled initially */
  $quicktabs->api_version = 1;
  $quicktabs->machine_name = 'user_ideas';
  $quicktabs->ajax = 0;
  $quicktabs->hide_empty_tabs = FALSE;
  $quicktabs->default_tab = 0;
  $quicktabs->title = 'Ideas';
  $quicktabs->tabs = array(
    0 => array(
      'bid' => 'views_delta_user_ideas-block',
      'hide_title' => 1,
      'title' => 'My ideas',
      'weight' => '-100',
      'type' => 'block',
    ),
    1 => array(
      'bid' => 'views_delta_my_commented_ideas-block',
      'hide_title' => 1,
      'title' => 'My commented ideas',
      'weight' => '-99',
      'type' => 'block',
    ),
    2 => array(
      'bid' => 'views_delta_my_voted_ideas-block',
      'hide_title' => 1,
      'title' => 'My voted ideas',
      'weight' => '-98',
      'type' => 'block',
    ),
  );
  $quicktabs->renderer = 'quicktabs';
  $quicktabs->style = 'nostyle';
  $quicktabs->options = array();

  // Translatables
  // Included for use with string extractors like potx.
  t('Ideas');
  t('My commented ideas');
  t('My ideas');
  t('My voted ideas');

  $export['user_ideas'] = $quicktabs;

  return $export;
}
