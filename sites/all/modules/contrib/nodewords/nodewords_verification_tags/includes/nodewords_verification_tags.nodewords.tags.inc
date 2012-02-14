<?php
// $Id: nodewords_verification_tags.nodewords.tags.inc,v 1.1.2.2 2010/03/21 13:11:24 kiam Exp $

/**
 * @file
 * Nodewords support file.
 */

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_verification_tags_bing_webmaster_center_form(&$form, $content, $options) {
  $form['bing_webmaster_center'] = array(
    '#tree' => TRUE,
  );

  $form['bing_webmaster_center']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Bing Webmaster Center verification code'),
    '#description' => t('<a href="@bing-url">Bing Webmaster Center</a> will ask you to add a meta tag in order to verify you have write access to the pages of the web site you registered with their services.', array('@bing-url' => 'http://www.bing.com/webmaster')) . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_verification_tags_google_webmaster_tools_form(&$form, $content, $options) {
  $form['google_webmaster_tools'] = array(
    '#tree' => TRUE,
  );

  $form['google_webmaster_tools']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Google Webmaster Tools verification code'),
    '#description' => t('<a href="@google-url">Google Webmaster Tools</a> will ask you to add a meta tag in your web site to provide you with an easy way to make your site more <a href="http://google.com">Google</a>-friendly.', array('@google-url' => 'http://www.google.com/webmasters/tools')) . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the form fields used to implement the options for the meta tag.
 */
function nodewords_verification_tags_yahoo_site_explorer_form(&$form, $content, $options) {
  $form['yahoo_site_explorer'] = array(
    '#tree' => TRUE,
  );

  $form['yahoo_site_explorer']['value'] = array(
    '#type' => 'textfield',
    '#title' => t('Yahoo! Site Explorer'),
    '#description' => t('<a href="@yahoo-url">Yahoo! Site Explorer</a> will ask you to add a meta tag in your web site to allow you to explore all the web pages indexed by <a href="http://search.yahoo.com">Yahoo! Search</a>.', array('@yahoo-url' => 'https://siteexplorer.search.yahoo.com/')) . $options['description'],
    '#default_value' => empty($content['value']) ? '' : $content['value'],
    '#maxlength' => variable_get('nodewords_max_size', 350),
  );
}

/**
 * Set the meta tag content.
 */
function nodewords_verification_tags_bing_webmaster_center_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['bing_webmaster_center:msvalidate.01'] = $content['value'];
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_verification_tags_google_webmaster_tools_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['google_webmaster_tools:google-site-verification'] = $content['value'];
  }
}

/**
 * Set the meta tag content.
 */
function nodewords_verification_tags_yahoo_site_explorer_prepare(&$tags, $content, $options) {
  if (!empty($content['value'])) {
    $tags['yahoo_site_explorer:y_key'] = $content['value'];
  }
}
