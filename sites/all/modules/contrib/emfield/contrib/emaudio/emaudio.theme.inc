<?php
// $Id: emaudio.theme.inc,v 1.1.2.6 2008/12/19 18:17:14 aaron Exp $

/**
 * @file
 * This defines the various theme functions for Embedded Audio Field (emaudio).
 */

function theme_emaudio_audio_embed($field, $item, $formatter, $node, $options = array()) {
  /*
    Note you can use this in node.tpl.php, substituting the proper field type:
    $field_type = 'field_audio';
    $system_types = _content_type_info();
    $field = $system_types['fields'][$field_type];
    $field['widget'] = $system_types['content types'][$node->type]['fields'][$field_type]['widget'];
    print theme('emaudio_audio_embed', $field, $node->{$field_type}[0], 'emaudio_embed', $node);
   */
  if ($item['value'] && $item['provider']) {
    $output = drupal_get_form('emaudio_embed_form', $field, $item, $formatter, $node, $options);
  }

  return $output;
}

function theme_emaudio_audio_thumbnail($field, $item, $formatter, $node, $no_link = FALSE, $options = array()) {
  if ($item['value'] && $item['provider']) {
    // If we've set $options['thumbnail_url'], then we'll just use that.
    // Otherwise, if we have emthumb installed, then give it a chance to override our thumbnail
    $thumbnail_url = $options['thumbnail_url'] ? $options['thumbnail_url'] : module_invoke('emthumb', 'thumbnail_url', $item);

    // if we don't have a custom thumbnail, then see if the provider gives us a thumbnail
    $thumbnail_url = $thumbnail_url ? $thumbnail_url : module_invoke('emfield', 'include_invoke', 'emaudio', $item['provider'], 'thumbnail', $field, $item, $formatter, $node, $width, $height, $options);

    // If we still don't have a thumbnail, then apply a default thumbnail, if it exists.
    if (!$thumbnail_url) {
      $default_thumbnail_url = $field['widget']['thumbnail_default_path'] ? $field['widget']['thumbnail_default_path'] : variable_get('emaudio_default_thumbnail_path', NULL);

      if ($default_thumbnail_url) {
        $thumbnail_url = base_path() . $default_thumbnail_url;
      }
    }
  }
  else {
    // Seems to be an unknown audio. Apply a default thumbnail, if it exists.
    if (!$thumbnail_url) {
      $default_thumbnail_url = $field['widget']['thumbnail_default_path'] ? $field['widget']['thumbnail_default_path'] : variable_get('emaudio_default_thumbnail_path', NULL);
      if ($default_thumbnail_url) {
        $thumbnail_url = base_path() . $default_thumbnail_url;
      }
    }
  }

  $link_url = isset($options['link_url']) ? $options['link_url'] : 'node/'. $node->nid;
  $link_title = isset($options['link_title']) ? $options['link_title'] : t('See audio');
  $image_title = isset($options['image_title']) ? $options['image_title'] : $link_title;
  $image_alt = isset($options['image_alt']) ? $options['image_alt'] : $link_title;

  if ($thumbnail_url) {
    $width = isset($options['width']) ? $options['width'] : NULL;
    $width = isset($width) ? $width : ($field['widget']['thumbnail_width'] ? $field['widget']['thumbnail_width'] : variable_get('emaudio_default_thumbnail_width', EMAUDIO_DEFAULT_THUMBNAIL_WIDTH));
    $height = isset($options['height']) ? $options['height'] : NULL;
    $height = isset($height) ? $height : ($field['widget']['thumbnail_height'] ? $field['widget']['thumbnail_height'] : variable_get('emaudio_default_thumbnail_height', emaudio_DEFAULT_THUMBNAIL_HEIGHT));
    if ($no_link) { //thickbox requires the thumbnail returned without the link
      $output = '<img src="'. $thumbnail_url .'" width="'. $width  .'" height="'. $height  .'" alt="'. $image_alt .'" title="'. $image_title .'" />';
    }
    else {
      $output = l('<img src="'. $thumbnail_url .'" width="'. $width  .'" height="'. $height  .'" alt="'. $image_alt .'" title="'. $image_title .'" />', $link_url, array('html'=> true));
    }
  }
  else {
     // if all else fails, then just print a 'see audio' link.
    if ($no_link) {
      $output = ''; //thickbox won't work without a thumbnail
    }
    else {
      $output = l($link_title, $link_url);
    }
  }

  return $output;
}

function theme_emaudio_audio_audio($field, $item, $formatter, $node, $options = array()) {
  if ($item['value'] && $item['provider']) {
    $embed = $item['value'];
    $width = isset($options['width']) ? $options['width'] : (isset($field['widget']['audio_width']) ? $field['widget']['audio_width'] : variable_get('emaudio_default_audio_width', EMAUDIO_DEFAULT_AUDIO_WIDTH));
    $height = isset($options['height']) ? $options['height'] : (isset($field['widget']['audio_height']) ? $field['widget']['audio_height'] : variable_get('emaudio_default_audio_height', EMAUDIO_DEFAULT_AUDIO_HEIGHT));
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $field['widget']['audio_autoplay'];
    $output = module_invoke('emfield', 'include_invoke', 'emaudio', $item['provider'], 'audio', $embed, $width, $height, $field, $item, $node, $autoplay, $options);
  }

  return $output;
}

function theme_emaudio_default($field, $item, $formatter, $node, $options = array()) {
  return theme('emaudio_audio_audio', $field, $item, $formatter, $node, $options);
}

function theme_emaudio_audio_preview($field, $item, $formatter, $node, $options = array()) {
  if ($item['value'] && $item['provider']) {
    $embed = $item['value'];
    $width = isset($options['width']) ? $options['width'] : (isset($field['widget']['audio_width']) ? $field['widget']['audio_width'] : variable_get('emaudio_default_audio_width', EMAUDIO_DEFAULT_PREVIEW_WIDTH));
    $height = isset($options['height']) ? $options['height'] : (isset($field['widget']['audio_height']) ? $field['widget']['audio_height'] : variable_get('emaudio_default_audio_height', EMAUDIO_DEFAULT_PREVIEW_HEIGHT));
    $autoplay = isset($options['autoplay']) ? $options['autoplay'] : $field['widget']['audio_autoplay'];
    $output = module_invoke('emfield', 'include_invoke', 'emaudio', $item['provider'], 'preview', $embed, $width, $height, $field, $item, $node, $autoplay, $options);
  }

  return $output;
}

function theme_emaudio_formatter_audio_flash($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emaudio', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}
function theme_emaudio_formatter_audio_embed($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emaudio', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emaudio_formatter_audio_thumbnail($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emaudio', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emaudio_formatter_default($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emaudio', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emaudio_formatter_audio_preview($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emaudio', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emaudio_formatter_audio_audio($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emaudio', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}