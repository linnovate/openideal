<?php
// $Id: nodewords_verification_tags.nodewords.hooks.inc,v 1.1.2.4 2010/04/17 15:19:11 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
 * Implements hook_nodewords_tags_info().
 */
function nodewords_verification_tags_nodewords_tags_info() {
  $tags = array(
    'bing_webmaster_center' => array(
      'callback' => 'nodewords_verification_tags_bing_webmaster_center',
      'label' => t('Bing Webmaster Center verification code'),
      'templates' => array(
        'head' => array(
          'msvalidate.01' => NODEWORDS_META,
        ),
      ),
    ),
    'google_webmaster_tools' => array(
      'callback' => 'nodewords_verification_tags_google_webmaster_tools',
      'label' => t('Google Webmaster Tools verification code'),
      'templates' => array(
        'head' => array(
          'google-site-verification' => NODEWORDS_META,
        ),
      ),
    ),
    'yahoo_site_explorer' => array(
      'callback' => 'nodewords_verification_tags_yahoo_site_explorer',
      'label' => t('Yahoo! Site Explorer verification code'),
      'templates' => array(
        'head' => array(
          'y_key' => NODEWORDS_META,
        ),
      ),
    ),
  );

  return $tags;
}
