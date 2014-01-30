<?php
/**
 * @file
 * fe_admin.layouts.inc
 */

/**
 * Implements hook_default_panels_layout().
 */
function fe_admin_default_panels_layout() {
  $export = array();

  $layout = new stdClass();
  $layout->disabled = FALSE; /* Edit this to true to make a default layout disabled initially */
  $layout->api_version = 1;
  $layout->name = 'openideal_admin';
  $layout->admin_title = 'OpenideaL admin';
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
          0 => 'menu_region',
          1 => 'center',
        ),
        'parent' => 'main',
      ),
      'center' => array(
        'type' => 'region',
        'title' => 'Content',
        'bootstrap_class' => array(
          'xs' => 'col-xs-12',
          'sm' => 'col-sm-12',
          'md' => 'col-md-8',
          'lg' => 'col-lg-8',
        ),
        'parent' => 'main-row',
        'class' => '',
      ),
      'menu_region' => array(
        'type' => 'region',
        'title' => 'Menu region',
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
  $export['openideal_admin'] = $layout;

  return $export;
}
