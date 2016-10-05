<?php
/**
 * @file
 * fe_idea.quicktabs.inc
 */

/**
 * Implements hook_quicktabs_default_quicktabs().
 */
function fe_idea_quicktabs_default_quicktabs() {
  $export = array();

  $quicktabs = new stdClass();
  $quicktabs->disabled = FALSE; /* Edit this to true to make a default quicktabs disabled initially */
  $quicktabs->api_version = 1;
  $quicktabs->machine_name = 'all_ideas';
  $quicktabs->ajax = 0;
  $quicktabs->hide_empty_tabs = FALSE;
  $quicktabs->default_tab = 0;
  $quicktabs->title = 'All ideas';
  $quicktabs->tabs = array(
    0 => array(
      'bid' => 'views_delta_ideas_all-block_3',
      'hide_title' => 1,
      'title' => 'Ideas',
      'weight' => '-100',
      'type' => 'block',
    ),
    1 => array(
      'bid' => 'views_delta_ideas_all-block_2',
      'hide_title' => 1,
      'title' => 'Latest',
      'weight' => '-99',
      'type' => 'block',
    ),
    2 => array(
      'bid' => 'views_delta_ideas_all-block_1',
      'hide_title' => 1,
      'title' => 'Popular',
      'weight' => '-98',
      'type' => 'block',
    ),
  );
  $quicktabs->renderer = 'quicktabs';
  $quicktabs->style = 'nostyle';
  $quicktabs->options = array();

  // Translatables
  // Included for use with string extractors like potx.
  t('All ideas');
  t('Ideas');
  t('Latest');
  t('Popular');

  $export['all_ideas'] = $quicktabs;

  $quicktabs = new stdClass();
  $quicktabs->disabled = FALSE; /* Edit this to true to make a default quicktabs disabled initially */
  $quicktabs->api_version = 1;
  $quicktabs->machine_name = 'ideas_by_category';
  $quicktabs->ajax = 0;
  $quicktabs->hide_empty_tabs = FALSE;
  $quicktabs->default_tab = 0;
  $quicktabs->title = 'Ideas by Category';
  $quicktabs->tabs = array(
    0 => array(
      'bid' => 'views_delta_ideas_by_category-block_1',
      'hide_title' => 1,
      'title' => 'Popular Ideas',
      'weight' => '-100',
      'type' => 'block',
    ),
    1 => array(
      'bid' => 'views_delta_ideas_by_category-block_2',
      'hide_title' => 1,
      'title' => 'Latest Ideas',
      'weight' => '-99',
      'type' => 'block',
    ),
    2 => array(
      'bid' => 'views_delta_ideas_by_category-block_3',
      'hide_title' => 1,
      'title' => 'Ideas in Action',
      'weight' => '-98',
      'type' => 'block',
    ),
  );
  $quicktabs->renderer = 'quicktabs';
  $quicktabs->style = 'nostyle';
  $quicktabs->options = array();

  // Translatables
  // Included for use with string extractors like potx.
  t('Ideas by Category');
  t('Ideas in Action');
  t('Latest Ideas');
  t('Popular Ideas');

  $export['ideas_by_category'] = $quicktabs;

  $quicktabs = new stdClass();
  $quicktabs->disabled = FALSE; /* Edit this to true to make a default quicktabs disabled initially */
  $quicktabs->api_version = 1;
  $quicktabs->machine_name = 'in_action';
  $quicktabs->ajax = 0;
  $quicktabs->hide_empty_tabs = TRUE;
  $quicktabs->default_tab = 0;
  $quicktabs->title = 'In Action';
  $quicktabs->tabs = array(
    0 => array(
      'bid' => 'views_delta_ideas_in_action-block_3',
      'hide_title' => 1,
      'title' => 'All',
      'weight' => '-100',
      'type' => 'block',
    ),
    1 => array(
      'bid' => 'views_delta_ideas_in_action-block_2',
      'hide_title' => 1,
      'title' => 'Under Review',
      'weight' => '-99',
      'type' => 'block',
    ),
    2 => array(
      'bid' => 'views_delta_ideas_in_action-block_4',
      'hide_title' => 1,
      'title' => 'Reviewed',
      'weight' => '-98',
      'type' => 'block',
    ),
    3 => array(
      'bid' => 'views_delta_ideas_in_action-block_5',
      'hide_title' => 1,
      'title' => 'Coming Soon',
      'weight' => '-97',
      'type' => 'block',
    ),
    4 => array(
      'bid' => 'views_delta_ideas_in_action-block_6',
      'hide_title' => 1,
      'title' => 'Launched',
      'weight' => '-96',
      'type' => 'block',
    ),
    5 => array(
      'bid' => 'views_delta_ideas_in_action-block_7',
      'hide_title' => 1,
      'title' => 'Next Time',
      'weight' => '-95',
      'type' => 'block',
    ),
  );
  $quicktabs->renderer = 'quicktabs';
  $quicktabs->style = 'nostyle';
  $quicktabs->options = array();

  // Translatables
  // Included for use with string extractors like potx.
  t('All');
  t('Coming Soon');
  t('In Action');
  t('Launched');
  t('Next Time');
  t('Reviewed');
  t('Under Review');

  $export['in_action'] = $quicktabs;

  return $export;
}
