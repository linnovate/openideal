<?php
/**
 * @file
 * fe_default.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function fe_default_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:ideas
  $menu_links['main-menu:ideas'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'ideas',
    'router_path' => 'ideas',
    'link_title' => 'Ideas',
    'options' => array(
      'attributes' => array(),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-47',
  );
  // Exported menu link: main-menu:in-action
  $menu_links['main-menu:in-action'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'in-action',
    'router_path' => 'in-action',
    'link_title' => 'Ideas in action',
    'options' => array(
      'attributes' => array(),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-46',
  );
  // Exported menu link: main-menu:node/add/idea
  $menu_links['main-menu:node/add/idea'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'node/add/idea',
    'router_path' => 'node/add/idea',
    'link_title' => 'Add Idea',
    'options' => array(
      'attributes' => array(),
    ),
    'module' => 'menu',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '-48',
  );
  // Exported menu link: main-menu:user/login
  $menu_links['main-menu:user/login'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'user/login',
    'router_path' => 'user/login',
    'link_title' => 'Login',
    'options' => array(
      'attributes' => array(),
      'alter' => TRUE,
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
  t('Add Idea');
  t('Ideas');
  t('Ideas in action');
  t('Login');


  return $menu_links;
}
