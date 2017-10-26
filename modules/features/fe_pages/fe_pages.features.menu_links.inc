<?php
/**
 * @file
 * fe_pages.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function fe_pages_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: menu-footer-menu:<front>
  $menu_links['menu-footer-menu:<front>'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => '<front>',
    'router_path' => '',
    'link_title' => 'Help',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '1',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-46',
  );
  // Exported menu link: menu-footer-menu:challenges
  $menu_links['menu-footer-menu:challenges'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'challenges',
    'router_path' => 'challenges',
    'link_title' => 'Challenges',
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
  // Exported menu link: menu-footer-menu:community
  $menu_links['menu-footer-menu:community'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'community',
    'router_path' => 'community',
    'link_title' => 'Community',
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
    'weight' => '-47',
  );
  // Exported menu link: menu-footer-menu:ideas
  $menu_links['menu-footer-menu:ideas'] = array(
    'menu_name' => 'menu-footer-menu',
    'link_path' => 'ideas',
    'router_path' => 'ideas',
    'link_title' => 'Ideas',
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
  t('Challenges');
  t('Community');
  t('Help');
  t('Ideas');


  return $menu_links;
}
