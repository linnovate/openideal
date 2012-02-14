<?php

/**
 * Implementation of hook_quicktabs_default_quicktabs().
 */
function fe_main_quicktabs_default_quicktabs() {
  $export = array();
  $quicktabs = new stdClass;
  $quicktabs->disabled = FALSE; /* Edit this to true to make a default quicktabs disabled initially */
  $quicktabs->api_version = 1;
  $quicktabs->machine_name = 'brand_page';
  $quicktabs->ajax = 0;
  $quicktabs->hide_empty_tabs = 0;
  $quicktabs->default_tab = FALSE;
  $quicktabs->title = '<none>';
  $quicktabs->tabs = array(
    0 => array(
      'vid' => 'brand_page_ideas',
      'display' => 'page_1',
      'args' => '',
      'get_displays' => 'vdisp_0',
      'title' => 'Popular Ideas',
      'weight' => '-100',
      'type' => 'view',
    ),
    1 => array(
      'vid' => 'brand_page_ideas',
      'display' => 'page_2',
      'args' => '',
      'get_displays' => 'vdisp_1',
      'title' => 'Recent Ideas',
      'weight' => '-99',
      'type' => 'view',
    ),
    2 => array(
      'vid' => 'brand_page_ideas',
      'display' => 'page_4',
      'args' => '',
      'get_displays' => 'vdisp_2',
      'title' => 'Ideas In Action',
      'weight' => '-98',
      'type' => 'view',
    ),
  );
  $quicktabs->style = 'default';

  $export['brand_page'] = $quicktabs;
  $quicktabs = new stdClass;
  $quicktabs->disabled = FALSE; /* Edit this to true to make a default quicktabs disabled initially */
  $quicktabs->api_version = 1;
  $quicktabs->machine_name = 'ideas_index';
  $quicktabs->ajax = 0;
  $quicktabs->hide_empty_tabs = 0;
  $quicktabs->default_tab = TRUE;
  $quicktabs->title = 'Ideas index tabs';
  $quicktabs->tabs = array(
    0 => array(
      'vid' => 'ideas_index',
      'display' => 'page_1',
      'args' => '',
      'get_displays' => 'vdisp_0',
      'title' => 'Popular Ideas',
      'weight' => '-100',
      'type' => 'view',
    ),
    1 => array(
      'vid' => 'ideas_index',
      'display' => 'page_2',
      'args' => '',
      'get_displays' => 'vdisp_1',
      'title' => 'Recent Ideas',
      'weight' => '-99',
      'type' => 'view',
    ),
  );
  $quicktabs->style = 'default';

  $export['ideas_index'] = $quicktabs;
  $quicktabs = new stdClass;
  $quicktabs->disabled = FALSE; /* Edit this to true to make a default quicktabs disabled initially */
  $quicktabs->api_version = 1;
  $quicktabs->machine_name = 'in_action';
  $quicktabs->ajax = 0;
  $quicktabs->hide_empty_tabs = 0;
  $quicktabs->default_tab = FALSE;
  $quicktabs->title = '<none>';
  $quicktabs->tabs = array(
    0 => array(
      'vid' => 'ideas_in_action',
      'display' => 'page_2',
      'args' => '',
      'get_displays' => 'vdisp_0',
      'title' => 'All',
      'weight' => '-100',
      'type' => 'view',
    ),
    1 => array(
      'vid' => 'ideas_in_action',
      'display' => 'page_1',
      'args' => '',
      'get_displays' => 'vdisp_1',
      'title' => 'Under Review',
      'weight' => '-99',
      'type' => 'view',
    ),
    2 => array(
      'vid' => 'ideas_in_action',
      'display' => 'page_3',
      'args' => '',
      'get_displays' => 'vdisp_2',
      'title' => 'Reviewed',
      'weight' => '-98',
      'type' => 'view',
    ),
    3 => array(
      'vid' => 'ideas_in_action',
      'display' => 'page_4',
      'args' => '',
      'get_displays' => 'vdisp_3',
      'title' => 'Comming Soon',
      'weight' => '-97',
      'type' => 'view',
    ),
    4 => array(
      'vid' => 'launced_projects',
      'display' => 'page_1',
      'args' => '',
      'get_displays' => 'vdisp_4',
      'title' => 'Launched',
      'weight' => '-96',
      'type' => 'view',
    ),
  );
  $quicktabs->style = 'default';

  $export['in_action'] = $quicktabs;
  return $export;
}
