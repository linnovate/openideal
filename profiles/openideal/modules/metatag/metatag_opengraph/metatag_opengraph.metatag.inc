<?php

/**
 * @file
 * Metatag integration for the metatag_opengraph module.
 */

/**
 * Implements hook_metatag_config_default_alter().
 */
function metatag_opengraph_metatag_config_default_alter(array &$configs) {
  foreach ($configs as &$config) {
    switch ($config->instance) {
      case 'global':
        $config->config += array(
          'og:type' => array('value' => 'article'),
          'og:title' => array('value' => '[current-page:title]'),
          'og:site_name' => array('value' => '[site:name]'),
          'og:url' => array('value' => '[current-page:url]'),
        );
        break;
      case 'global:frontpage':
        $config->config += array(
          'og:type' => array('value' => 'website'),
          'og:title' => array('value' => '[site:name]'),
          'og:url' => array('value' => '[site:url]'),
        );
        break;
      case 'node':
        $config->config += array(
          'og:title' => array('value' => '[node:title]'),
          'og:description' => array('value' => '[node:summary]'),
        );
        break;
      case 'taxonomy_term':
        $config->config += array(
          'og:title' => array('value' => '[term:name]'),
          'og:description' => array('value' => '[term:description]'),
        );
        break;
      case 'user':
        $config->config += array(
          'og:type' => array('value' => 'profile'),
          'og:title' => array('value' => '[user:name]'),
        );
        if (variable_get('user_pictures')) {
          $config->config += array(
            'og:image' => array('value' => '[user:picture:url]'),
          );
        }

        break;
    }
  }
}

/**
 * Implements hook_metatag_info().
 */
function metatag_opengraph_metatag_info() {
  $info['groups']['open-graph'] = array(
    'label' => t('Open Graph'),
  );

  $info['tags']['og:title'] = array(
    'label' => t('Open Graph title'),
    'description' => t('The title of your object as it should appear within the graph, e.g., <em>The Rock</em>.'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'open-graph',
    'element' => array(
      '#theme' => 'metatag_opengraph',
    ),
  );
  $info['tags']['og:type'] = array(
    'label' => t('Open Graph type'),
    'description' => t('The type of your object, e.g., <em>movie</em>.'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'open-graph',
    'form' => array(
      '#type' => 'select',
      '#options' => _metatag_opengraph_type_options(),
      '#empty_option' => t('- None -'),
    ),
    'element' => array(
      '#theme' => 'metatag_opengraph',
    ),
  );
  //if (module_exists('select_or_other')) {
  //  $info['tags']['og:type']['form']['#type'] = 'select_or_other';
  //}
  $info['tags']['og:image'] = array(
    'label' => t('Open Graph image'),
    'description' => t('An image URL which should represent your object within the graph. The image must be at least 50px by 50px and have a maximum aspect ratio of 3:1. We support PNG, JPEG and GIF formats.'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'open-graph',
    'element' => array(
      '#theme' => 'metatag_opengraph',
    ),
  );
  $info['tags']['og:url'] = array(
    'label' => t('Open Graph URL'),
    'description' => t('The canonical URL of your object that will be used as its permanent ID in the graph, e.g., <em>http://www.imdb.com/title/tt0117500/</em>.'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'open-graph',
    'element' => array(
      '#theme' => 'metatag_opengraph',
    ),
  );
  $info['tags']['og:site_name'] = array(
    'label' => t('Open Graph site name'),
    'description' => t('A human-readable name for your site, e.g., <em>IMDb</em>.'),
    'class' => 'DrupalTextMetaTag',
    'group' => 'open-graph',
    'context' => array('global'),
    'element' => array(
      '#theme' => 'metatag_opengraph',
    ),
  );
  $info['tags']['og:description'] = array(
    'label' => t('Open Graph description'),
    'description' => t('A one to two sentence description of your page.'),
    'group' => 'open-graph',
    'class' => 'DrupalTextMetaTag',
    'element' => array(
      '#theme' => 'metatag_opengraph',
    ),
  );

  return $info;
}

function _metatag_opengraph_type_options() {
  $options = array(
    t('Activities') => array(
      'activity' => t('Activity'),
      'sport' => t('Sport'),
    ),
    t('Businesses') => array(
      'bar' => t('Bar'),
      'company' => t('Company'),
      'cafe' => t('Cafe'),
      'hotel' => t('Hotel'),
      'restaurant' => t('Restaurant'),
    ),
    t('Groups') => array(
      'cause' => t('Cause'),
      'sports_league' => t('Sports league'),
      'sports_team' => t('Sports team'),
    ),
    t('Organizations') => array(
      'band' => t('Band'),
      'government' => t('Government'),
      'non_profit' => t('Non-profit'),
      'school' => t('School'),
      'university' => t('University'),
    ),
    t('People') => array(
      'actor' => t('Actor'),
      'athlete' => t('Athlete'),
      'author' => t('Author'),
      'director' => t('Director'),
      'musician' => t('Musician'),
      'politician' => t('Politician'),
      'profile' => t('Profile'),
      'public_figure' => t('Public figure'),
    ),
    t('Places') => array(
      'city' => t('City'),
      'country' => t('Country'),
      'landmark' => t('Landmark'),
      'state_province' => t('State or province'),
    ),
    t('Products and Entertainment') => array(
      'album' => t('Album'),
      'book' => t('Book'),
      'drink' => t('Drink'),
      'food' => t('Food'),
      'game' => t('Game'),
      'movie' => t('Movie'),
      'product' => t('Product'),
      'song' => t('Song'),
      'tv_show' => t('TV show'),
    ),
    t('Websites') => array(
      'blog' => t('Blog'),
      'website' => t('Website'),
      'article' => t('Article'),
    ),
  );

  return $options;
}
