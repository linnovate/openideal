<?php
// $Id: emvideo.theme.inc,v 1.1.2.38.2.11 2010/10/20 20:33:21 aaron Exp $

/**
 * @file
 * This defines the various theme functions for Embedded Video Field (emvideo).
 */

/**
 * Formatter for emvideo_video_embed.
 * This will return the 'embed code', typically used to embed media in an
 * external site or blog.
 */
function theme_emvideo_video_embed($field, $item, $formatter, $node, $options = array()) {
  /*
    Note you can use this in node.tpl.php, substituting the proper field type:
    $field_type = 'field_video';
    $system_types = _content_type_info();
    $field = $system_types['fields'][$field_type];
    $field['widget'] = $system_types['content types'][$node->type]['fields'][$field_type]['widget'];
    print theme('emvideo_video_embed', $field, $node->{$field_type}[0], 'emvideo_embed', $node);

    or you can set $field to NULL and just use the options array
   */
  $output = '';
  if (isset($item['value']) && isset($item['provider'])) {
    $output .= drupal_get_form('emvideo_embed_form', $field, $item, $formatter, $node, $options);
  }

  return $output;
}

function theme_emvideo_video_thumbnail_no_link($field, $item, $formatter, $node, $options = array()) {
  return theme('emvideo_video_thumbnail', $field, $item, $formatter, $node, TRUE, $options);
}
function theme_emvideo_video_thumbnail_provider_link($field, $item, $formatter, $node, $options = array()) {
  $options['link_url'] = isset($options['link_url']) ? $options['link_url'] : emfield_include_invoke('emvideo', $item['provider'], 'embedded_link', $item['value'], $item['data']);
  return theme('emvideo_video_thumbnail', $field, $item, $formatter, $node, FALSE, $options);
}

/**
 * Returns the relative path to the video thumbnail.
 */
function theme_emvideo_video_thumbnail_path($field, $item, $formatter, $node, $options = array()) {
  $options['return_url'] = TRUE;
  return theme('emvideo_video_thumbnail', $field, $item, $formatter, $node, TRUE, $options);
}

/**
 * Returns the absolute URL to the video thumbnail.
 */
function theme_emvideo_video_thumbnail_url($field, $item, $formatter, $node, $options = array()) {
  $options['return_url'] = TRUE;
  $options['raw'] = TRUE;
  $path = theme('emvideo_video_thumbnail', $field, $item, $formatter, $node, TRUE, $options);
  return url($path, array('absolute' => TRUE));
}

/**
 * This will return a provided thumbnail image for a video.
 *
 * @param $field
 *   This is the field providing settings for the video thumbnail.
 * @param $item
 *   This is the data returned by the field. It requires at the least to be an array with 'value' and 'provider'.
 *   $item['value'] will be the video code, and $item['provider'] will be the provider, such as youtube.
 * @param $formatter
 *   This is the formatter for the view. This will nearly always be video_thumbnail.
 * @param $node
 *   This is the node object containing the field.
 * @param $no_link
 *   Optional. If FALSE, then we provide a link to the node.
 *   (In retrospect, this should have been $link, defaulting to TRUE.
 *   TODO: fix? problem though is that this goes deeper up the tree.)
 * @param $options
 *   Optional array. This is to pass optional overrides. currently:
 *     $options['width'] and $options['height'], if provided, will override any field settings for the thumbnail w/h.
 *     $options['link_url'], if provided, will cause the thumbnail link to go to another URL other than node/nid. $no_link must be FALSE.
 *     $options['title'], if provided, will set the title of the link and image.
 *     $options['link_title'], if provided, will set the title of the link when no image is provided. otherwise, it defaults to 'See video'.
 *     $options['image_title'], if provided, will set the title attribute of the href link, defaulting to $options['link_title'].
 *     $options['image_alt'], if provided, will set the alt attribute of the href link, defaulting to $options['link_title'].
 *     $options['thumbnail_url'], if provided, will completely override the thumbnail image entirely.
 *     $options['attributes'], if provided, sets the attributes for the displayed image.
 *     $options['return_url'], if provided, simply returns the URL to the thumbnail.
 *     $options['raw'], if TRUE, will return the raw URL, assuming $options['return_url'] is also true.
 * @return
 *   The thumbnail output.
 */
function theme_emvideo_video_thumbnail($field, $item, $formatter, $node, $no_link = FALSE, $options = array()) {
  // Thickbox requires some output, so make sure we have at least a blank string.
  $output = '';
  $options['node'] = $node;
  $width = isset($options['width']) ? $options['width'] : ($field['widget']['thumbnail_width'] ? $field['widget']['thumbnail_width'] : variable_get('emvideo_default_video_width', EMVIDEO_DEFAULT_THUMBNAIL_WIDTH));
  $height = isset($options['height']) ? $options['height'] : ($field['widget']['thumbnail_height'] ? $field['widget']['thumbnail_height'] : variable_get('emvideo_default_video_height', EMVIDEO_DEFAULT_THUMBNAIL_HEIGHT));

  // Get a thumbnail URL. This can be an override through $options['thumbnail_url'],
  // defined by the provider (the usual case), or a default image.
  if (isset($options['thumbnail_url']) || ($item['value'] && $item['provider'])) {
    // If we've set $options['thumbnail_url'], then we'll just use that.
    $thumbnail_url = isset($options['thumbnail_url']) ? $options['thumbnail_url'] : '';

    // Otherwise, if we have emthumb installed, then give it a chance to override our thumbnail.
    if (empty($thumbnail_url) && function_exists('emthumb_thumbnail_path')) {
      $thumbnail_url = emthumb_thumbnail_path($item);
    }

    // If we don't have a custom thumbnail, then see if the provider gives us a thumbnail.
    if (empty($thumbnail_url)) {
      $thumbnail_url = emfield_include_invoke('emvideo', $item['provider'], 'thumbnail', $field, $item, $formatter, $node, $width, $height, $options);
    }
  }

  // If we still don't have a thumbnail, then apply a default thumbnail, if it exists.
  if (empty($thumbnail_url)) {
    $default_thumbnail_url = $field['widget']['thumbnail_default_path'] ? $field['widget']['thumbnail_default_path'] : variable_get('emvideo_default_thumbnail_path', NULL);
    if ($default_thumbnail_url) {
      $thumbnail_url = $default_thumbnail_url;
    }
  }

  // Define the thumbnail link's path.
  $link_url = isset($options['link_url']) ? $options['link_url'] : 'node/'. $node->nid;

  // Define the title/alt to display for the link hover.
  $link_title = isset($options['link_title']) ? $options['link_title'] : (isset($options['title']) ? $options['title'] : (isset($field['widget']['thumbnail_link_title']) ? $field['widget']['thumbnail_link_title'] : (isset($item['title']) ? $item['title'] : variable_get('emvideo_default_thumbnail_link_title', t('See video')))));
  if (module_exists('token')) {
    // Allow the editor to use [title] tokens.
    $link_title = token_replace($link_title, 'global', $node);
  }
  $image_title = isset($options['image_title']) ? $options['image_title'] : (isset($item['title']) ? $item['title'] : $link_title);
  $image_alt = isset($options['image_alt']) ? $options['image_alt'] : (isset($item['description']) ? $item['description'] : $link_title);

  if (isset($thumbnail_url)) {
    if (!empty($options['absolute'])) {
      $thumbnail_url = url($thumbnail_url, array('absolute' => $options['absolute']));
    }
    if (isset($options['return_url'])) {
      return $options['raw'] ? $thumbnail_url : url($thumbnail_url);
    }
    $width = isset($options['width']) ? $options['width'] : NULL;
    $width = isset($width) ? $width : ($field['widget']['thumbnail_width'] ? $field['widget']['thumbnail_width'] : variable_get('emvideo_default_thumbnail_width', EMVIDEO_DEFAULT_THUMBNAIL_WIDTH));
    $height = isset($options['height']) ? $options['height'] : NULL;
    $height = isset($height) ? $height : ($field['widget']['thumbnail_height'] ? $field['widget']['thumbnail_height'] : variable_get('emvideo_default_thumbnail_height', EMVIDEO_DEFAULT_THUMBNAIL_HEIGHT));
    $attributes = isset($options['attributes']) ? $options['attributes'] : array();
    $attributes['width'] = isset($attributes['width']) ? $attributes['width'] : $width;
    $attributes['height'] = isset($attributes['height']) ? $attributes['height'] : $height;
    $image = theme('image', $thumbnail_url, $image_alt, $image_title, $attributes, FALSE);
    if ($no_link) {
      // Thickbox requires the thumbnail returned without the link.
      $output = $image;
    }
    else {
      $output = l($image, $link_url, array('html'=> TRUE));
    }
  }
  else {
    if ($options['return_url']) {
      return $options['raw'] ? $thumbnail_url : url($link_url);
    }

    // If all else fails, then just print a 'see video' type link.
    if (!$no_link) {
      $output = l($link_title, $link_url);
    }
  }

  return $output;
}

function theme_emvideo_video_video($field, $item, $formatter, $node, $options = array()) {
  $output = '';

  if (isset($item['value']) && isset($item['provider'])) {
    $embed = $item['value'];
    $width = isset($options['width']) ? $options['width'] : ($field['widget']['video_width'] ? $field['widget']['video_width'] : variable_get('emvideo_default_video_width', EMVIDEO_DEFAULT_VIDEO_WIDTH));
    $height = isset($options['height']) ? $options['height'] : ($field['widget']['video_height'] ? $field['widget']['video_height'] : variable_get('emvideo_default_video_height', EMVIDEO_DEFAULT_VIDEO_HEIGHT));
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $field['widget']['video_autoplay'];
    $options['node'] = $node;
    $output = emfield_include_invoke('emvideo', $item['provider'], 'video', $embed, $width, $height, $field, $item, $node, $autoplay, $options);
    $output = '<div class="emvideo emvideo-video emvideo-'. check_plain($item['provider']) .'">'. $output .'</div>';
  }

  return $output;
}

function theme_emvideo_default($field, $item, $formatter, $node, $options = array()) {
  return theme('emvideo_video_video', $field, $item, $formatter, $node, $options);
}

function theme_emvideo_video_link($field, $item, $formatter, $node, $options = array()) {
  $title = isset($options['link_title']) ? $options['link_title'] : (isset($options['title']) ? $options['title'] : (isset($field['widget']['thumbnail_link_title']) ? $field['widget']['thumbnail_link_title'] : variable_get('emvideo_default_thumbnail_link_title', t('See video'))));
  $url = emfield_include_invoke('emvideo', $item['provider'], 'embedded_link', $item['value'], $item['data']);
  return l($title, $url);
}
function theme_emvideo_video_url($field, $item, $formatter, $node, $options = array()) {
  $url = emfield_include_invoke('emvideo', $item['provider'], 'embedded_link', $item['value'], $item['data']);
  return url($url);
}
function theme_emvideo_video_duration($field, $item, $formatter, $node, $options = array()) {
  return '<div class="emvideo-duration">'. _emvideo_seconds_to_time($item['duration']) .'</div>';
}

function theme_emvideo_video_preview($field, $item, $formatter, $node, $options = array()) {
  $output = '';
  if ($item['value'] && $item['provider']) {
    $embed = $item['value'];
    $width = isset($options['width']) ? $options['width'] : (isset($field['widget']['preview_width']) ? $field['widget']['preview_width'] : variable_get('emvideo_default_preview_width', EMVIDEO_DEFAULT_PREVIEW_WIDTH));
    $height = isset($options['height']) ? $options['height'] : (isset($field['widget']['preview_height']) ? $field['widget']['preview_height'] : variable_get('emvideo_default_preview_height', EMVIDEO_DEFAULT_PREVIEW_HEIGHT));
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : (isset($field['widget']['preview_autoplay']) ? $field['widget']['preview_autoplay'] : FALSE);
    $options['node'] = $node;
    $output = emfield_include_invoke('emvideo', $item['provider'], 'preview', $embed, $width, $height, $field, $item, $node, $autoplay, $options);
    $output = '<div class="emvideo emvideo-preview emvideo-'. check_plain($item['provider']) .'">'. $output .'</div>';
  }

  return $output;
}

function theme_emvideo_modal_generic($field, $item, $formatter, $node, $options = array()) {
  $title = isset($options['title']) ? $options['title'] : (isset($item['title']) ? $item['title'] : (isset($node->title) ? $node->title : (isset($node->node_title) ? $node->node_title : (isset($field['widget']['thumbnail_link_title']) ? $field['widget']['thumbnail_link_title'] : variable_get('emvideo_default_thumbnail_link_title', t('See video'))))));
  $thumbnail = isset($options['thumbnail']) ? $options['thumbnail'] : theme('emvideo_video_thumbnail', $field, $item, 'video_thumbnail', $node, TRUE, $options);
  $width = ($formatter == 'video_replace_preview') ? $field['widget']['preview_width'] : $field['widget']['video_width'];
  $height = ($formatter == 'video_replace_preview') ? $field['widget']['preview_height'] : $field['widget']['video_height'];
  $destination = 'emvideo/modal/'. $node->nid .'/'. $width .'/'. $height .'/'. $field['field_name'] .'/'. $item['provider'] .'/'. $item['value'];
  $link_class = 'emvideo-thumbnail-replacement emvideo-modal-' . $options['modal'] .' '. $options['modal'];
  $attributes = array(
    'attributes' => array(
      'title' => $title,
      'class' => $link_class,
    ),
    'query' => NULL,
    'fragment' => NULL,
    'absolute' => FALSE,
    'html' => TRUE,
  );
  if ($options['modal'] == 'lightbox2') {
    $attributes['attributes']['rel'] = 'lightframe['. $field['type_name'] .'|width:'. ($width + 16) .'; height:'. ($height + 16) .';]';
  }
  else if ($options['modal'] == 'shadowbox') {
    $attributes['attributes']['rel'] = 'shadowbox['. $field['type_name'] .'];width='. ($width + 5) .';height='. ($height + 5);
    // Shadowbox needs a .php extension so it knows to open as an iFrame.
    $destination .= '/index.php';
  }
  else if ($options['modal'] == 'colorbox-load') {
    $attributes['query'] = 'width='. ($width + 4) .'&height='. ($height + 7) .'&iframe=true';
  }

  if (($options['modal'] == 'lightbox2') && function_exists('lightbox2_add_files')) {
    lightbox2_add_files();
  }

  static $added_js;
  if (!isset($added_js)) {
    // Add the play button image overlay.
    drupal_add_js(array('emvideo' => array('thumbnail_overlay' => variable_get('emfield_thumbnail_overlay', TRUE))), 'setting');
    drupal_add_js(drupal_get_path('module', 'emvideo') . '/emvideo.thumbnail-replacement.js');
    drupal_add_css(drupal_get_path('module', 'emvideo') . '/emvideo.thumbnail-replacement.css');
    $added_js = TRUE;
  }
  $class = 'emvideo-modal emvideo-'. $options['modal'];
  if ($options['wrapper-class']) {
    $class .= ' '. $options['wrapper-class'];
  }
  $display_title = '';
  // Create "title" container
  if (isset($options['title'])) {
    $display_title = '<div class="item-title">'. $options['title'] ."</div>";
  } else if (isset($item['title'])) {
    $display_title = '<div class="item-title">'. $item['title']. "</div>";
  }
  $output = '<div class="'. $class .'">'. l($thumbnail, $destination, $attributes) . $display_title .'</div>';
  return $output;
}

/**
 *  Display the video as a Thickbox modal popup. Requires the Thickbox module.
 */
function theme_emvideo_thickbox($field, $item, $formatter, $node, $options = array()) {
  $options['modal'] = 'thickbox';
  return theme('emvideo_modal_generic', $field, $item, $formatter, $node, $options);
}

/**
 *  Display the video as a Colorbox modal popup. Requires the Colorbox module.
 */
function theme_emvideo_colorbox($field, $item, $formatter, $node, $options = array()) {
  $options['modal'] = 'colorbox-load';
  return theme('emvideo_modal_generic', $field, $item, $formatter, $node, $options);
}

/**
 *  Display the video as a Lightbox2 modal popup. Requires the Lightbox2 module.
 */
function theme_emvideo_lightbox2($field, $item, $formatter, $node, $options = array()) {
  $options['modal'] = 'lightbox2';
  return theme('emvideo_modal_generic', $field, $item, $formatter, $node, $options);
}

/**
 *  Display the video as a Shadowbox modal popup. Requires the Shadowbox module.
 */
function theme_emvideo_shadowbox($field, $item, $formatter, $node, $options = array()) {
  $options['modal'] = 'shadowbox';
  return theme('emvideo_modal_generic', $field, $item, $formatter, $node, $options);
}

function theme_emvideo_video_replace($field, $item, $formatter, $node, $options = array()) {
  $options['modal'] = 'emvideo';
  $options['width'] = isset($options['width']) ? $options['width'] : $field['widget']['video_width'];
  $options['height'] = isset($options['height']) ? $options['height'] : $field['widget']['video_height'];
  $options['wrapper-class'] = isset($options['wrapper-class']) ? $options['wrapper-class'] . ' emvideo-thumbnail-replace-full' : 'emvideo-thumbnail-replace-full';
  return theme('emvideo_modal_generic', $field, $item, $formatter, $node, $options);
}
function theme_emvideo_video_replace_preview($field, $item, $formatter, $node, $options = array()) {
  $options['modal'] = 'emvideo';
  $options['width'] = isset($options['width']) ? $options['width'] : $field['widget']['preview_width'];
  $options['height'] = isset($options['height']) ? $options['height'] : $field['widget']['preview_height'];
  $options['wrapper-class'] = isset($options['wrapper-class']) ? $options['wrapper-class'] . ' emvideo-thumbnail-replace-preview' : 'emvideo-thumbnail-replace-preview';
  return theme('emvideo_modal_generic', $field, $item, $formatter, $node, $options);
}

/**
 *  Formatter theme functions.
 */
function theme_emvideo_formatter_video_video($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_embed($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_link($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_url($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_duration($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_default($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_preview($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_thumbnail($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_flash($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_colorbox($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_thickbox($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_lightbox2($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_shadowbox($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_replace($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_replace_preview($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_thumbnail_no_link($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_thumbnail_provider_link($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_thumbnail_path($element) {
  return _emvideo_formatter_theme_helper($element);
}
function theme_emvideo_formatter_video_thumbnail_url($element) {
  return _emvideo_formatter_theme_helper($element);
}

/**
 *  This ultimately calls theme('emvideo_[formatter]').
 */
function _emvideo_formatter_theme_helper($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return emvideo_field_formatter($field, $element['#item'], $element['#formatter'], $element['#node']);
}

/**
 * Convert time from field into seconds.
 *
 * @param $time
 *    Raw timemarker input from field.
 */
function _emvideo_convert_to_seconds($time) {
  $time_in_seconds = 0;
  $time_split = split(':', $time);
  $count = count($time_split);

  // If it is already in seconds then don't do anything.
  if ($count == 1) return intval($time);

  foreach ($time_split as $unit) {
    $count--;
    // If hours or minutes multiply by appropriate amount.
    if ($count > 0) {
      // If it is hours multiply by 36000 if minutes multiply by 60.
      $time_in_seconds += $unit*(pow(60, $count));
    }
    else {
      // If we are on the seconds unit then we do not need to do anything.
      $time_in_seconds += $unit;
    }
  }

  return $time_in_seconds;
}

/**
 *  Display seconds as HH:MM:SS, with leading 0's.
 *
 *  @param $seconds
 *    The number of seconds to display.
 */
function _emvideo_seconds_to_time($seconds) {
  // Number of seconds in an hour.
  $unith =3600;
  // Number of seconds in a minute.
  $unitm =60;

  // '/' given value by num sec in hour... output = HOURS
  $hh = intval($seconds / $unith);

  // Multiply number of hours by seconds, then subtract from given value.
  // Output = REMAINING seconds.
  $ss_remaining = ($seconds - ($hh * 3600));

  // Take remaining seconds and divide by seconds in a min... output = MINS.
  $mm = intval($ss_remaining / $unitm);
  // Multiply number of mins by seconds, then subtract from remaining seconds.
  // Output = REMAINING seconds.
  $ss = ($ss_remaining - ($mm * 60));

  $output = '';

  // If we have any hours, then prepend that to our output.
  if ($hh) {
    $output .= "$hh:";
  }

  // Create a safe-for-output MM:SS.
  $output .= check_plain(sprintf($hh ? "%02d:%02d" : "%d:%02d", $mm, $ss));

  return $output;
}

/**
 *  Display the video in an iFrame.
 *
 *  @param $content
 *    The video to display.
 *  @return
 *    The output to print directly, including HTML/Head/Body tags.
 */
function template_preprocess_emvideo_modal_iframe(&$variables) {
  $variables['language']          = $GLOBALS['language'];
  $variables['language']->dir     = $GLOBALS['language']->direction ? 'rtl' : 'ltr';
  // Construct page title
  if (drupal_get_title()) {
    $variables['head_title'] = array(strip_tags(drupal_get_title()), variable_get('site_name', 'Drupal'));
  }
  else {
    $variables['head_title'] = array(variable_get('site_name', 'Drupal'));
    if (variable_get('site_slogan', '')) {
      $variables['head_title'][] = variable_get('site_slogan', '');
    }
  }
  $variables['head_title']        = implode(' | ', $variables['head_title']);
  $variables['head']              = drupal_get_html_head();
  $variables['css']               = drupal_add_css();
  $variables['styles']            = drupal_get_css();
  $variables['scripts']           = drupal_get_js();
  $variables['closure']           = theme('closure');
}
