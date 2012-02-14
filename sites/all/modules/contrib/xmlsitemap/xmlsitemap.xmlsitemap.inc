<?php
// $Id: xmlsitemap.xmlsitemap.inc,v 1.5.2.2 2010/04/29 16:22:09 davereid Exp $

/**
 * @file
 * XML sitemap integration functions for xmlsitemap.module.
 */

/**
 * Implements hook_xmlsitemap_link_info().
 */
function xmlsitemap_xmlsitemap_link_info() {
  return array(
    'frontpage' => array(
      'label' => t('Frontpage'),
      'xmlsitemap' => array(
        'settings callback' => 'xmlsitemap_link_frontpage_settings',
      ),
    ),
  );
}

/**
 * XML sitemap link type settings callback for frontpage link entity.
 */
function xmlsitemap_link_frontpage_settings(&$form) {
  module_load_include('admin.inc', 'xmlsitemap');
  if (user_access('administer site configuration')) {
    $form['#description'] = t('The front page path can be changed at <a href="@url-frontpage">@url-frontpage</a>.', array('@url-frontpage' => url('admin/settings/site-information')));
  }
  $form['xmlsitemap_frontpage_priority'] = array(
    '#type' => 'select',
    '#title' => t('Priority'),
    '#options' => xmlsitemap_get_priority_options(),
    '#default_value' => variable_get('xmlsitemap_frontpage_priority', 1.0),
  );
  $form['xmlsitemap_frontpage_changefreq'] = array(
    '#type' => 'select',
    '#title' => t('Change frequency'),
    '#options' => xmlsitemap_get_changefreq_options(),
    '#default_value' => variable_get('xmlsitemap_frontpage_changefreq', XMLSITEMAP_FREQUENCY_DAILY),
  );
  return $form;
}

/**
 * Implements hook_xmlsitemap_link_alter().
 */
function xmlsitemap_xmlsitemap_link_alter(&$link) {
  // Alter the frontpage priority.
  if ($link['type'] == 'frontpage' || $link['loc'] == '' || $link['loc'] == drupal_get_normal_path(variable_get('site_frontpage', 'node'))) {
    $link['priority'] = variable_get('xmlsitemap_frontpage_priority', 1.0);
    $link['changefreq'] = variable_get('xmlsitemap_frontpage_changefreq', XMLSITEMAP_FREQUENCY_DAILY);
  }
}

/**
 * Implements hook_xmlsitemap_links().
 */
function xmlsitemap_xmlsitemap_links() {
  // Frontpage link.
  $links[] = array(
    'type' => 'frontpage',
    'id' => 0,
    'loc' => '',
  );
  return $links;
}

/**
 * Implements hook_xmlsitemap_sitemap_operations().
 */
function xmlsitemap_xmlsitemap_sitemap_operations() {
  $operations['update'] = array(
    'label' => t('Update cached files'),
    'action past' => t('Updated'),
    'callback' => 'xmlsitemap_sitemap_multiple_update',
  );
  return $operations;
}

/**
 * XML sitemap operation callback; regenerate sitemap files using the batch API.
 *
 * @param $smids
 *   An array of XML sitemap IDs.
 *
 * @see xmlsitemap_regenerate_batch()
 */
function xmlsitemap_sitemap_multiple_update(array $smids) {
  module_load_include('generate.inc', 'xmlsitemap');
  $batch = xmlsitemap_regenerate_batch($smids);
  batch_set($batch);
}
