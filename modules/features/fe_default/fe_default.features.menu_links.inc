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

  // Exported menu link: main-menu_challenges:challenges.
  $menu_links['main-menu_challenges:challenges'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'challenges',
    'router_path' => 'challenges',
    'link_title' => 'Challenges',
    'options' => array(
      'identifier' => 'main-menu_challenges:challenges',
      'attributes' => array(
        'title' => 'All Challenges',
      ),
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -50,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_community:community.
  $menu_links['main-menu_community:community'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'community',
    'router_path' => 'community',
    'link_title' => 'Community',
    'options' => array(
      'identifier' => 'main-menu_community:community',
      'attributes' => array(
        'title' => 'Community members list',
      ),
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -42,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_ideas:ideas.
  $menu_links['main-menu_ideas:ideas'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'ideas',
    'router_path' => 'ideas',
    'link_title' => 'Ideas',
    'options' => array(
      'attributes' => array(
        'title' => 'All Ideas',
      ),
      'identifier' => 'main-menu_ideas:ideas',
    ),
    'module' => 'system',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -43,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_news:news.
  $menu_links['main-menu_news:news'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'news',
    'router_path' => 'news',
    'link_title' => 'News',
    'options' => array(
      'attributes' => array(
        'title' => '',
      ),
      'identifier' => 'main-menu_news:news',
    ),
    'module' => 'menu',
    'hidden' => 0,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -46,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );
  // Exported menu link: main-menu_news:news-list.
  $menu_links['main-menu_news:news-list'] = array(
    'menu_name' => 'main-menu',
    'link_path' => 'news-list',
    'router_path' => 'news-list',
    'link_title' => 'News',
    'options' => array(
      'identifier' => 'main-menu_news:news-list',
    ),
    'module' => 'system',
    'hidden' => 1,
    'external' => 0,
    'has_children' => 0,
    'expanded' => 0,
    'weight' => -41,
    'customized' => 1,
    'language' => 'und',
    'menu_links_customized' => 1,
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Challenges');
  t('Community');
  t('Ideas');
  t('News');

  return $menu_links;
}
