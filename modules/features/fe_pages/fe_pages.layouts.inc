<?php
/**
 * @file
 * fe_pages.layouts.inc
 */

/**
 * Implements hook_default_panels_layout().
 */
function fe_pages_default_panels_layout() {
  $export = array();

  $layout = new stdClass();
  $layout->disabled = FALSE; /* Edit this to true to make a default layout disabled initially */
  $layout->api_version = 1;
  $layout->name = 'home';
  $layout->admin_title = 'Home';
  $layout->admin_description = '';
  $layout->category = 'OpenideaL';
  $layout->plugin = 'bootstrap';
  $layout->settings = array(
    'items' => array(
      'canvas' => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 'main',
        ),
        'parent' => NULL,
      ),
      'main' => array(
        'type' => 'column',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-12',
          'lg' => 'col-lg-12',
        ),
        'children' => array(
          0 => 'main-row',
          1 => 1,
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(),
        'parent' => 'main',
      ),
      1 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'content_views',
          1 => 'sidebar',
        ),
        'parent' => 'main',
        'class' => 'Content',
      ),
      'content_views' => array(
        'type' => 'region',
        'title' => 'Content',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-8',
          'lg' => 'col-lg-8',
        ),
        'parent' => '1',
        'class' => 'oi-views',
      ),
      'sidebar' => array(
        'type' => 'region',
        'title' => 'Sidebar',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-4',
          'lg' => 'col-lg-4',
        ),
        'parent' => '1',
        'class' => 'oi-sidebar',
      ),
    ),
  );
  $export['home'] = $layout;

  $layout = new stdClass();
  $layout->disabled = FALSE; /* Edit this to true to make a default layout disabled initially */
  $layout->api_version = 1;
  $layout->name = 'ideal_two_column';
  $layout->admin_title = 'Ideal two column';
  $layout->admin_description = '';
  $layout->category = 'OpenideaL';
  $layout->plugin = 'bootstrap';
  $layout->settings = array(
    'items' => array(
      'canvas' => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 'main',
        ),
        'parent' => NULL,
      ),
      'main' => array(
        'type' => 'column',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-12',
          'lg' => 'col-lg-12',
        ),
        'children' => array(
          0 => 'main-row',
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center',
          1 => 'sidebar',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Center',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-8',
          'lg' => 'col-lg-8',
        ),
        'parent' => 'main-row',
      ),
      'sidebar' => array(
        'type' => 'region',
        'title' => 'Sidebar',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-4',
          'lg' => 'col-lg-4',
        ),
        'parent' => 'main-row',
        'class' => '',
      ),
    ),
  );
  $export['ideal_two_column'] = $layout;

  $layout = new stdClass();
  $layout->disabled = FALSE; /* Edit this to true to make a default layout disabled initially */
  $layout->api_version = 1;
  $layout->name = 'latest_and_greatest';
  $layout->admin_title = 'Latest and greatest';
  $layout->admin_description = 'Three column bootstrap layout';
  $layout->category = 'OpenideaL';
  $layout->plugin = 'bootstrap';
  $layout->settings = array(
    'items' => array(
      'canvas' => array(
        'type' => 'row',
        'contains' => 'column',
        'children' => array(
          0 => 'main',
          1 => 1,
          2 => 2,
        ),
        'parent' => NULL,
      ),
      'main' => array(
        'type' => 'column',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-4',
          'lg' => 'col-lg-4',
        ),
        'children' => array(
          0 => 'main-row',
        ),
        'parent' => 'canvas',
      ),
      'main-row' => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Column 1',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-12',
          'lg' => 'col-lg-12',
        ),
        'parent' => 'main-row',
        'class' => '',
      ),
      1 => array(
        'type' => 'column',
        'parent' => 'canvas',
        'children' => array(
          0 => 3,
        ),
        'class' => '',
        'bootstrap_class' => array(
          'md' => 'col-md-4',
          'lg' => 'col-lg-4',
        ),
      ),
      2 => array(
        'type' => 'column',
        'parent' => 'canvas',
        'children' => array(
          0 => 4,
        ),
        'class' => '',
        'bootstrap_class' => array(
          'md' => 'col-md-4',
          'lg' => 'col-lg-4',
        ),
      ),
      3 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'column_2',
        ),
        'parent' => '1',
        'class' => '',
      ),
      4 => array(
        'type' => 'row',
        'contains' => 'region',
        'children' => array(
          0 => 'column_3',
        ),
        'parent' => '2',
        'class' => '',
      ),
      'column_2' => array(
        'type' => 'region',
        'title' => 'Column 2',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-12',
          'lg' => 'col-lg-12',
        ),
        'parent' => '3',
        'class' => '',
      ),
      'column_3' => array(
        'type' => 'region',
        'title' => 'Column 3',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-12',
          'lg' => 'col-lg-12',
        ),
        'parent' => '4',
        'class' => '',
      ),
    ),
  );
  $export['latest_and_greatest'] = $layout;

  $layout = new stdClass();
  $layout->disabled = FALSE; /* Edit this to true to make a default layout disabled initially */
  $layout->api_version = 1;
  $layout->name = 'openideal_admin';
  $layout->admin_title = 'OpenideaL admin';
  $layout->admin_description = '';
  $layout->category = 'OpenideaL';
  $layout->plugin = 'bootstrap';
  $layout->settings = array();
  $export['openideal_admin'] = $layout;

  return $export;
}
