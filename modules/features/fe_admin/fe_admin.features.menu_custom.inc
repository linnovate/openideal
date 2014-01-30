<?php
/**
 * @file
 * fe_admin.features.menu_custom.inc
 */

/**
 * Implements hook_menu_default_menu_custom().
 */
function fe_admin_menu_default_menu_custom() {
  $menus = array();

  // Exported menu: menu-administration.
  $menus['menu-administration'] = array(
    'menu_name' => 'menu-administration',
    'title' => 'Administration',
    'description' => 'Site admin menu',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Administration');
  t('Site admin menu');


  return $menus;
}
