<?php
// $Id: emfield.php,v 1.1.2.3 2009/10/29 18:29:43 aaron Exp $

/**
 *  @file
 *  Document various hooks for Embedded Media Field.
 *
 *  These hooks generally follow the standards for hook invocations with
 *  Drupal, with hook_[module]_[hookname]. To invoke a hook, you would
 *  replace hook_ with your invoking module name.
 *
 *  For instance, to implement hook_emfield_system_list with a module named
 *  'mymodule', you would create function mymodule_emfield_system_list().
 *
 *  However, Embedded Media Field also provides a unique hook structure for
 *  its provider files, following the pattern:
 *  hook [emmodule]_[provider]_[hookname]. For instance, if you create a
 *  provider for the http://example.com, with a provider name of 'example',
 *  you might implement hook emvideo_PROVIDER_settings as
 *  function emvideo_example_settings().
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 *  Return the information about a specific provider.
 *
 *  Each provider module must implement this hook to be recognized.
 *
 *  @return
 *    An associative array with the following information:
 *      'provider' => The machine name, should be same as the provider filename.
 *      'name' => The provider's human readable name.
 *      'url' => The URL to the provider's main page.
 *      'settings_description' => This will be displayed above the provider
 *        information on the administration page.
 *      'supported_features' => An array of cells to be passed to a table
 *        on the administration page, under the headers of 'Feature',
 *        'Supported', and 'Notes'.
 */
function EMMODULE_PROVIDER_info() {
  $features = array(
    array(t('Autoplay'), t('Yes'), ''),
    array(t('RSS Attachment'), t('Yes'), ''),
    array(t('Thumbnails'), t('Yes'), t('')),
    array(t('Duration'), t('Yes'), ''),
    array(t('Full screen mode'), t('Yes'), t('You may customize the player to enable or disable full screen playback. Full screen mode is enabled by default.')),
    array(t('Use JW FLV Media Player'), t('Yes'), t("You may opt to use the !flvplayer to play example.com videos if it's installed on your server.", array('!flvplayer' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/')))),
  );
  return array(
    'provider' => 'example',
    'name' => t('Example.com'),
    'url' => 'http://example.com/',
    'settings_description' => t('These settings specifically affect videos displayed from <a href="@example" target="_blank">Example.com</a>.', array('@example' => 'http://example.com')),
    'supported_features' => $features,
  );
}

/**
 *  Allow a provider to add its own elements to the content creation form.
 */
function EMMODULE_PROVIDER_form($field, $item) {
}

/**
 *  Parse the URL or embed code provided by an editor.
 *
 *  @param $embed
 *    The raw URL or embed code pasted into the text field by the editor.
 *  @param $field
 *    The field data for the emvideo, emaudio, etc.
 *  @return
 *    If the hook returns a non-empty string, then it is assumed to have been
 *    parsed and matched by this provider. If the hook returns an array of
 *    strings, then each string is assumed to be a regex pattern, and will
 *    be checked for a match in turn. Otherwise, it is assumed there is no
 *    match.
 */
function EMMODULE_PROVIDER_extract($embed, $field) {
  return array(
    '@example\.com/video/(*+)@i',
    '@example\.com/rss/video/(*+)@i',
  );
}

/**
 *  Validate the provider's embedded code.
 *
 *  This allows each provider to determine whether a pasted URL that otherwise
 *  matches a parse attempt from EMMODULE_PROVIDER_extract is valid.
 *  For instance, the provider file might check against the third party
 *  provider's API to ensure a video still exists. If the validation is to
 *  fail, the function should call form_set_error with the provided
 *  $error_field.
 *
 *  @param $code
 *    The unique identifier for the third party media.
 *  @param $error_field
 *    The form field to use with form_set_error().
 */
function EMMODULE_PROVIDER_validate($code, $error_field) {
  if (!_example_call_api('video_data', $code)) {
    form_set_error($error_field, t('That video no longer exists on example.com.'));
  }
}

function EMMODULE_PROVIDER_data($field, $item) {

}

function EMMODULE_PROVIDER_data_version($field, $item) {

}

/**
 * Returns a link to view the original media at the provider's site.
 *  @param $code
 *    The unique identifier for the third party media.
 *  @param $data
 *    The original data array collected for the media.
 *  @return
 *    A string containing the URL to view the original media.
 */
function EMMODULE_PROVIDER_embedded_link($code, $data = array()) {
  return 'http://example.com/video/'. $code;
}

/**
 *  @TODO
 *  Not sure why this is there -- provided for already w/ hook_emfield_subtheme.
 */
function EMMODULE_PROVIDER_subtheme() {
}

/**
 *  Returns any theme functions required for this provider.
 */
function EMMODULE_PROVIDER_emfield_subtheme() {
  return array(
    'emvideo_example_flash'  => array(
        'arguments' => array('embed' => NULL, 'width' => NULL, 'height' => NULL, 'autoplay' => NULL, 'options' => array()),
        'file' => 'providers/example.inc',
        'path' => drupal_get_path('module', 'media_example'),
    ),
  );
}

/**
 *  @TODO
 *  Not sure why this is there -- provided for already w/ hook_emfield_submenu.
 */
function EMMODULE_PROVIDER_submenu() {
}

/**
 *  Returns any menu pages required by this provider.
 */
function EMMODULE_PROVIDER_emfield_submenu() {
}

/**
 *  Generate random media for the devel module.
 *
 *  If the devel module (at http://drupal.org/project/devel) is installed,
 *  it can be used to generate placeholder content, useful for development.
 *
 *  This function should return an array of URLs to parse. One will be selected
 *  randomly from the array.
 */
function EMMODULE_PROVIDER_content_generate() {
  return array(
    'http://www.example.com/video/drupal-song',
    'http://www.example.com/video/about-emfield',
    'http://www.example.com/video/love-drupal',
  );
}

/**
 *  Build a list of provider files that serve the invoking module.
 *
 *  @param $module
 *    The contributed Embedded Media Field module in question, such as
 *    Embedded Video Field or Embedded Google Wave.
 *  @param $provider
 *    If provided, then we expect the single provider file named.
 *    Otherwise, we expect an array of all provider files supported.
 *  @return
 *    A listing of files built with drupal_system_listing().
 */
function hook_emfield_providers($module, $provider = NULL) {
  return drupal_system_listing("$provider\.inc$", drupal_get_path('module', 'media_example') ."/providers/$module", 'name', 0);
}

/**
 *  Generally used for metadata, this will add an array of database column
 *  information to be saved when present in the field. Note that this hook
 *  is used to return columns to be saved for all Embedded Media Fields,
 *  and is currently unused by the core Embedded Media Field modules.
 */
function hook_emfield_field_columns_extra($field) {
  return array(
    'author' => array(
      'description' => 'The original author of the third party media.',
      'type' => 'varchar',
      'length' => 255,
      'not null' => TRUE,
      'default' => '',
    ),
  );
}

/**
 *  This passes on the original implementation of CCK's hook_widget_settings.
 */
function hook_emfield_widget_settings_extra($op, $widget) {
}

/**
 *  This passes on the original implementation of CCK's hook_widget.
 */
function hook_emfield_widget_extra($form, $form_state, $field, $items, $delta, $module) {
}

/**
 *  Alter the data array of a media item before it is stored in the database.
 */
function hook_emfield_data_alter(&$data, $module, $delta = 0, $node = NULL, $field = NULL, $items = array()) {
}
