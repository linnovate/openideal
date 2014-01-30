<?php
/**
 * @file
 * fe_admin.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function fe_admin_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-administration:administration
  $menu_links['menu-administration:administration'] = array(
    'menu_name' => 'menu-administration',
    'link_path' => 'administration',
    'router_path' => 'administration',
    'link_title' => 'Overview',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-50',
  );
  // Exported menu link: menu-administration:administration/analytics
  $menu_links['menu-administration:administration/analytics'] = array(
    'menu_name' => 'menu-administration',
    'link_path' => 'administration/analytics',
    'router_path' => 'administration',
    'link_title' => 'Analytics',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-49',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Analytics');
  t('Overview');


  return $menu_links;
}
