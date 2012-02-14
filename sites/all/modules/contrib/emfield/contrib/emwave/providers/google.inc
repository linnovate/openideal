<?php
// $Id: google.inc,v 1.1.2.1 2009/10/13 13:26:44 aaron Exp $

/**
 * @file
 * This include processes google.com wave files for use by emfield.module.
 */

define('EMWAVE_GOOGLE_MAIN_URL', 'http://wave.google.com/a/wavesandbox.com/');

/**
 * hook emwave_PROVIDER_info
 * this returns information relevant to a specific 3rd party wave provider
 * @return
 *   an array of strings requested by various admin and other forms
 *   'name' => The translated name of the provider.
 *   'url' => The url to the main page for the provider.
 *   'settings_description' => A description of the provider that will be posted in the admin settings form.
 *   'supported_features' => An array of rows describing the state of certain supported features by the provider.
 *      These will be rendered in a table, with the columns being 'Feature', 'Supported', 'Notes'.
 */

function emwave_google_info() {
  $features = array();
  return array(
    'provider' => 'google',
    'name' => t('Google'),
    'url' => EMWAVE_GOOGLE_MAIN_URL,
    'settings_description' => t('These settings specifically affect images displayed from <a href="@google" target="_blank">Google</a>.', array('@google' => EMWAVE_GOOGLE_MAIN_URL)),
    'supported_features' => $features,
  );
}

function emwave_google_data($field, $item) {
  $data = array();
  return $data;
}

function emwave_google_extract($embed = '') {
  return $embed;
}

/**
 * hook emwave_PROVIDER_embedded_link($code)
 * returns a link to view the content at the provider's site
 *  @param $code
 *    the string containing the content to watch
 *  @return
 *    a string containing the URL to view the wave at the original provider's site
 */
function emwave_google_embedded_link($code) {
  return "http://wave.google.com/a/wavesandbox.com/$code";
}

/**
 *  Implement emwave_PROVIDER_wave_url
 *
 *  @param $code
 *    the code of the wave
 *  @param $data
 *    any stored data for the wave, which may already have the title
 *  @return
 *    the url directly to the wave to display
 */
function emwave_google_wave_url($code, $data) {
  return "http://wave.google.com/a/wavesandbox.com/{$code}";
}
