<?php
/**
 * @file
 * openideal_leader_board.features.menu_links.inc
 */

/**
 * Implements hook_menu_default_menu_links().
 */
function openideal_leader_board_menu_default_menu_links() {
  $menu_links = array();

  // Exported menu link: main-menu:leader-board
  $menu_links['main-menu:leader-board'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'leader-board',
    'router_path' => 'leader-board',
    'link_title' => 'Leader Board',
    'options' => array(
      'attributes' => array(
        'title' => 'See all users with their current points balance',
      ),
    ),
    'module' => 'system',
    'hidden' => '0',
    'external' => '0',
    'has_children' => '0',
    'expanded' => '0',
    'weight' => '0',
  );
  // Translatables
  // Included for use with string extractors like potx.
  t('Leader Board');


  return $menu_links;
}
