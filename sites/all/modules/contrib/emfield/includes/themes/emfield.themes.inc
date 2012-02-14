<?php
// $Id: emfield.themes.inc,v 1.1.2.1 2009/10/05 19:05:11 aaron Exp $

/**
 *  @file
 *  Basic theme functions for Embedded Media Field.
 */

 /**
 * Theme function to add a flash file to the page.
 *
 * The actual returned HTML from this function is minimal.  Its main purpose is
 * to setup the Javascript properly on content which is cached by input filters
 *
 * @ingroup themeable
 * @param $url
 *   A web accessible url to the flash file.
 * @param $params
 *   An associative array of parameters that describe the SWF.
 * @param $vars
 *   An associative array of variables to pass through to the SWF flashvars value.
 * @param $id
 *   An id to appened to the so object.
 * @param $attributes
 *   Associative array of attributes to apply to the resulting flash tag (embed, object, etc.)
 */
function theme_emfield_swfobject($url, $params = array(), $vars = array(), $id = NULL, $attributes = array()) {
  // Defer to the SWFObject API module.
  if (module_exists('swfobject_api')) {
    return theme('swfobject_api', $url, $params, $vars, $id, $attributes);
  }

  // We're in trouble if we don't have swfobject.js installed at this point.
  if (!variable_get('emfield_swfobject_location', '')) {
    return '';
  }

  // keep track of each div ID
  static $swfobject_id = 1;

  // Build the base params.
  $base_params = array(
    'width' => '100%',
    'height' => '100%',
    'no_flash' => t('Sorry, you need to install flash to see this content.'),
    'version' => variable_get('swfoa_settings_version', '6'),
    'type' => 'movie',
    'bgcolor' => '#FFFFFF',
    'express_redirect' => FALSE,
//     'express_redirect' => variable_get('swfoa_settings_express', TRUE) ? drupal_get_path('module', 'swfobject_api') .'/expressinstall.swf' : 'false',
    'class' => '',
  );

  // Merge in default parameters.
  $params += $base_params;

  // Increment the div id to allow for multiple players on the page
  $div_id = empty($id) ? 'emfield-swfobject-'. $swfobject_id++ : $id;

  // are the no_flash parameters being passed with a filter?
  if (is_array($params['no_flash'])) {
    $params['no_flash'] = check_markup($params['no_flash']['text'], $params['no_flash']['filter']);
  }

  // assign param data to the specific parameters
  $height = $params['height'];
  $width = $params['width'];
  $express_redirect = $params['express_redirect'];
  $version = $params['version'];
  $bgcolor = $params['bgcolor'];
  $no_flash = $params['no_flash'];
  $class = implode(' ', array($params['class'], 'swfobject'));
  unset($params['height'], $params['width'], $params['express_redirect'], $params['version'], $params['bg_color'], $params['no_flash'], $params['class']);

  $settings['swfobject_api']['files'][$div_id] = array(
    'url' => $url,
    'params' => $params,
    'flashVars' => $vars,
    'attributes' => $attributes,
    'height' => $height,
    'width' => $width,
    'express_redirect' => $express_redirect,
    'version' => $version,
    'bgcolor' => $bg_color,
  );

  // add the JS to the page
  _emfield_swfobject_api_ensure_swfobject($settings);

  // Return the placeholder HTML that will normally get
  // replaced with flash content.
  return "<div id=\"$div_id\" class=\"{$class}\">{$no_flash}</div>\n";
}

/**
 * Ensure that the necessary JS files are loaded.
 *
 * This function automatically checks to make sure that the JS files are only
 * loaded once.
 * @param array $settings
 */
function _emfield_swfobject_api_ensure_swfobject($settings = array()) {
  static $done = FALSE;
  if (! $done) {
    // Load the swfobject library and our add-on to activate it.
    drupal_add_js(variable_get('emfield_swfobject_location', ''), 'module', 'header');
    // Load the JS which creates loads the params into the page
    drupal_add_js(drupal_get_path('module', 'emfield') .'/includes/js/swfobject_api.js', 'module', 'footer');
    $done = TRUE;
  }
  drupal_add_js($settings, 'setting', 'footer', FALSE, TRUE, FALSE);
}
