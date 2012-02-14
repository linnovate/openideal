<?php

/**
 * @file
 * Provides documentation for the Service Links API.
 */

/**
 * Obtains all available service links.
 * @return
 *   An array containing all service links, keyed by name.
 */
function hook_service_links() {
  $links = array();

  $links['myservice'] = array(
    // The name of the service.
    'name' => 'MyService',
    // A short description for the link.
    'description' => t('Share this post on MyService'),
    // The link's URL. Available values are:
    //   <encoded-url>, <encoded-title>, <encoded-teaser>, <teaser>, <short-url>,
    // <source>, <node-id>, <url>, and <title>.
    'link' => 'http://example.com/?url=<encoded-url>&title=<encoded-title>&summary=<encoded-teaser>',
    // The service's small share icon. This is the relative path from Drupal's
    // base path, or the absolute URL.
    'icon' => drupal_get_path('module', 'myservice') .'/myservice.png',
    // Any additional attributes to apply to the element.
    'attributes' => array(
      'class' => 'myservice-special-class', // A special class.
      'style' => 'text-decoration: underline;', // Apply any special inline styles.
    ),
    // JavaScript that is added when this link is processed.
    'javascript' => drupal_get_path('module', 'myservice') .'/myservice.js',
    // CSS that is added when this link is processed.
    'css' => drupal_get_path('module', 'myservice') .'/myservice.css',
    // A PHP function callback that is invoked when the link is created.
    'callback' => 'myservice_callback',
  );

  return $links;
}

/**
 * Example callback from the Service Links.
 *
 * @param $service
 *   The service that is being used.
 * @param $context
 *   An array containing all information about the item being shared.
 */
function myservice_callback($service, $context) {

}

/**
 * Allows alteration of the Service Links.
 *
 * @param $links
 *   The constructed array of service links.
 */
function hook_service_links_alter(&$links) {
  if (isset($links['myservice'])) {
    // Change the icon of MyService.
    $links['myservice']['icon'] = 'http://drupal.org/misc/favicon.ico';
  }
}
