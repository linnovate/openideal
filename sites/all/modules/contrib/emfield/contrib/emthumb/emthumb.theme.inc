<?php
// $Id: emthumb.theme.inc,v 1.1.2.1.2.2 2010/10/20 20:21:22 aaron Exp $

/**
 * @file
 *  Theme functions for the Embedded Media Thumbnail module.
 */

/**
 *  Returns the imagecache image by itself.
 */
function theme_emthumb_imagecache_formatter_default($element) {
  return _emthumb_imagecache_formatter_default($element);
}

/**
 *  Returns the imagecache image linked to the node.
 */
function theme_emthumb_imagecache_formatter_linked($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $thumbnail = _emthumb_imagecache_formatter_default($element);
  return l($thumbnail, 'node/'. $options['node']->nid, array('html' => TRUE));
}

/**
 *  Returns the imagecache image linked to the original image.
 */
function theme_emthumb_imagecache_formatter_imagelink($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $thumbnail = _emthumb_imagecache_formatter_default($element);
  $options['return_url'] = $options['raw'] = TRUE;
  $original = '';
  switch ($options['module']) {
    case 'emvideo':
      $original = theme('emvideo_video_thumbnail', $options['field'], $options['item'], $options['formatter'], $options['node'], TRUE, $options);
      break;
  }
  return l($thumbnail, $original, array('html' => TRUE));
}

/**
 *  Returns the path to the imagecache image.
 */
function theme_emthumb_imagecache_formatter_path($element) {
  $options = _emthumb_formatter_theme_helper($element);
  if ($thumbnail = emthumb_thumbnail_path($options['item'])) {
    return imagecache_create_path($options['presetname'], $thumbnail);
  }
  $options['return_url'] = TRUE;
  switch ($options['module']) {
    case 'emvideo':
      return theme('emvideo_video_thumbnail', $options['field'], $options['item'], $options['formatter'], $options['node'], TRUE, $options);
  }
}

/**
 *  Returns the URL to the imagecache image.
 */
function theme_emthumb_imagecache_formatter_url($element) {
  $options = _emthumb_formatter_theme_helper($element);
  if ($thumbnail = emthumb_thumbnail_path($options['item'])) {
    return imagecache_create_url($options['presetname'], $thumbnail);
  }
  $options['return_url'] = $options['raw'] = TRUE;
  switch ($options['module']) {
    case 'emvideo':
      return theme('emvideo_video_thumbnail', $options['field'], $options['item'], $options['formatter'], $options['node'], TRUE, $options);
  }
}

/**
 *  Returns the imagecache image linked to the original provider file.
 */
function theme_emthumb_imagecache_formatter_provider_link($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $thumbnail = _emthumb_imagecache_formatter_default($element);
  $provider_link = emfield_include_invoke('emvideo', $options['item']['provider'], 'embedded_link', $options['item']['value'], $options['item']['data']);
  return l($thumbnail, $provider_link, array('html' => TRUE));
}

/**
 *  Returns the imagecache image linked to a full media display.
 */
function theme_emthumb_imagecache_formatter_full($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $options['thumbnail'] = _emthumb_imagecache_formatter_default($element);
  switch ($options['module']) {
    case 'emvideo':
      return theme('emvideo_video_replace', $options['field'], $options['item'], $options['formatter'], $options['node'], $options);
  }
}

/**
 *  Returns the imagecache image linked to a preview media display.
 */
function theme_emthumb_imagecache_formatter_preview($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $options['thumbnail'] = _emthumb_imagecache_formatter_default($element);
  switch ($options['module']) {
    case 'emvideo':
      return theme('emvideo_video_replace_preview', $options['field'], $options['item'], $options['formatter'], $options['node'], $options);
  }
}

/**
 *  Returns the imagecache image linked to a modal colorbox.
 */
function theme_emthumb_imagecache_formatter_colorbox($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $options['thumbnail'] = _emthumb_imagecache_formatter_default($element);
  switch ($options['module']) {
    case 'emvideo':
      return theme('emvideo_colorbox', $options['field'], $options['item'], $options['formatter'], $options['node'], $options);
  }
}

/**
 *  Returns the imagecache image linked to a modal thickbox.
 */
function theme_emthumb_imagecache_formatter_thickbox($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $options['thumbnail'] = _emthumb_imagecache_formatter_default($element);
  switch ($options['module']) {
    case 'emvideo':
      return theme('emvideo_thickbox', $options['field'], $options['item'], $options['formatter'], $options['node'], $options);
  }
}

/**
 *  Returns the imagecache image linked to a modal lightbox2.
 */
function theme_emthumb_imagecache_formatter_lightbox2($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $options['thumbnail'] = _emthumb_imagecache_formatter_default($element);
  switch ($options['module']) {
    case 'emvideo':
      return theme('emvideo_lightbox2', $options['field'], $options['item'], $options['formatter'], $options['node'], $options);
  }
}

/**
 *  Returns the imagecache image linked to a modal shadowbox.
 */
function theme_emthumb_imagecache_formatter_shadowbox($element) {
  $options = _emthumb_formatter_theme_helper($element);
  $options['thumbnail'] = _emthumb_imagecache_formatter_default($element);
  switch ($options['module']) {
    case 'emvideo':
      return theme('emvideo_shadowbox', $options['field'], $options['item'], $options['formatter'], $options['node'], $options);
  }
}

/**
 *  Returns the path to the imagecache image, or the original item if it's
 *  external.
 */
function _emthumb_imagecache_formatter_default($element, $options = NULL) {
  if (!isset($options)) {
    $options = _emthumb_formatter_theme_helper($element);
  }
  if ($options['item']['value']) {
    if ($thumbnail = emthumb_thumbnail_path($options['item'])) {
      return theme('imagecache', $options['presetname'], $thumbnail);
    }
    switch ($options['module']) {
      case 'emvideo':
        // Look for a local file first for imagecaching.
        $options['return_url'] = $options['raw'] = TRUE;
        $thumbnail = theme('emvideo_video_thumbnail', $options['field'], $options['item'], $options['formatter'], $options['node'], TRUE, $options);
        if (url($thumbnail) != url($thumbnail, array('absolute' => TRUE))) {
          return theme('imagecache', $options['presetname'], $thumbnail);
        }
        return theme('image', $thumbnail);
    }
  }
}

/**
 *  Sets up the options array with the proper node, field, etc.
 */
function _emthumb_formatter_theme_helper($element) {
  $options['field'] = content_fields($element['#field_name'], $element['#type_name']);
  $options['module'] = $options['field']['module'];
  $options['formatter'] = $element['#formatter'];
  $options['node'] = $element['#node'];
  $options['element'] = $element;
  $options['delta'] = $element['#delta'];
  $options['presetname'] = substr($element['#formatter'], 0, strrpos($element['#formatter'], '_'));

  // If we're coming from a preview, we need to extract our new embedded value.
  if (isset($element['#node']->in_preview)) {
    $element['#item'] = emfield_parse_embed($options['field'], $element['#item'], $options['module']);
  }

  // If we have no value, then return an empty string.
  if (!isset($element['#item'])) {
    return '';
  }
  $options['item'] = $element['#item'];

  // Unfortunately, when we come from a view, we don't get all the widget fields.
  if (!$options['node']->type) {
    $type = content_types($element['#type_name']);
    $options['field']['widget'] = $type['fields'][$field['field_name']]['widget'];
  }

  // Sometimes our data is still unserialized, again from views.
  if (!is_array($options['item']['data'])) {
    $options['item']['data'] = (array)unserialize($options['item']['data']);
  }

  return $options;
}
