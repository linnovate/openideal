<?php
// $Id: emimage.theme.inc,v 1.1.2.6.2.5 2010/11/17 17:01:12 aaron Exp $

/**
 *  @file
 *  Theme functions for Embedded Media Image
 */

/**
 *  Helper theme for various emimage themes.
 */
function theme_emimage_image($field, $item, $formatter, $node, $code, $width = NULL, $height = NULL, $title = '', $link = NULL, $options = array()) {
  $url = emimage_image_url($field, $item, $formatter, $node, $code, $width, $height, $options);

  $attributes = array();
  if ($width) {
    $attributes['width'] = $width;
  }
  if ($height) {
    $attributes['height'] = $height;
  }
  if (!isset($width) || !isset($height)) {
    // Find out the size of the actual image file, and scale accordingly
    if ( $item['data']['width'] && $item['data']['height'] ) {
      // The image's width & height are known
      $scale_width = $item['data']['width'] / ($width ? $width : 1);
      $scale_height = $item['data']['height'] / ($height ? $height : 1);
      if ($scale_width > $scale_height) {
        $attributes['width'] = $width;
      }
      else {
        $attributes['height'] = $height;
      }
    }
    else {
      // We don't know the size of the image, so just make it fill the space available.
      // It will probably be stretched in one direction, making it look odd.
      $attributes['width'] = $width ? $width : NULL;
      $attributes['height'] = $height ? $height : NULL;
    }
  }
  if ($item['class']) {
      $attributes['class'] = $item['class'];
  }
  else if ($item['provider']) {
      $attributes['class'] = $item['provider'];
  }
  $output = theme('image', $url, $title, $title, $attributes, false);

  if ($link) {
    $output = l($output, $link, array('html' => true));
  }
  return $output;
}

/**
 *  Format the image using the Thumbnail presets.
 */
function theme_emimage_image_thumbnail($field, $item, $formatter, $node, $options = array()) {
  if ($item['value'] && $item['provider']) {
    $code = $item['value'];
    $width = isset($options['width']) ? $options['width'] : $field['widget']['thumbnail_width'];
    $height = isset($options['height']) ? $options['height'] : $field['widget']['thumbnail_height'];
    $link = isset($options['thumbnail_link']) ? $options['thumbnail_link'] : $field['widget']['thumbnail_link'];
    if ($link == EMIMAGE_LINK_CONTENT) {
      $link = 'node/'. $node->nid;
    }
    else if ($link == EMIMAGE_LINK_PROVIDER) {
      $link = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'embedded_link', $code, $item['data']);
    }
    else if (!is_string($link)) {
      $link = NULL;
    }
    if (isset($options['title'])) {
      $title = $options['title'];
    }
    else {
      $title = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'image_title', $code, $item['data']);
    }
    $output = theme('emimage_image', $field, $item, $formatter, $node, $code, $width, $height, $title, $link, $options);
  }
  return $output;
}

/**
 *  Format the image using the Full Image preset.
 */
function theme_emimage_image_full($field, $item, $formatter, $node, $options = array()) {
  if ($item['value'] && $item['provider']) {
    $code = $item['value'];
    $width = isset($options['width']) ? $options['width'] : $field['widget']['full_width'];
    $height = isset($options['height']) ? $options['height'] : $field['widget']['full_height'];
    $link = isset($options['full_link']) ? $options['full_link'] : $field['widget']['full_link'];
    if ($link == EMIMAGE_LINK_CONTENT) {
      $link = 'node/'. $node->nid;
    }
    else if ($link == EMIMAGE_LINK_PROVIDER) {
      $link = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'embedded_link', $code, $item['data']);
    }
    else if (!is_string($link)) {
      $link = NULL;
    }
    if (isset($options['title'])) {
      $title = $options['title'];
    }
    else {
      $title = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'image_title', $code, $item['data']);
    }
    $output = theme('emimage_image', $field, $item, $formatter, $node, $code, $width, $height, $title, $link, $options);
  }
  return $output;
}

function theme_emimage_formatter_image_link($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emimage', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

/**
 *  Format the image using the Default (Full Image) preset.
 */
function theme_emimage_default($field, $item, $formatter, $node, $options = array()) {
  return theme('emimage_image_full', $field, $item, $formatter, $node, $options);
}

function theme_emimage_image_link($field, $item, $formatter, $node, $options = array()) {
  return module_invoke('emimage', $item['provider'] .'_embedded_link', $item['value']);
}

/**
 *  Format the image using the Preview Image preset.
 */
function theme_emimage_image_preview($field, $item, $formatter, $node, $options = array()) {
  if ($item['value'] && $item['provider']) {
    $code = $item['value'];
    $width = isset($options['width']) ? $options['width'] : $field['widget']['preview_width'];
    $height = isset($options['height']) ? $options['height'] : $field['widget']['preview_height'];
    $link = isset($options['preview_link']) ? $options['preview_link'] : $field['widget']['preview_link'];
    if ($link == EMIMAGE_LINK_CONTENT) {
      $link = 'node/'. $node->nid;
    }
    else if ($link == EMIMAGE_LINK_PROVIDER) {
      $link = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'embedded_link', $code, $item['data']);
    }
    else if (!is_string($link)) {
      $link = NULL;
    }
    if (isset($options['title'])) {
      $title = $options['title'];
    }
    else {
      $title = module_invoke('emfield', 'include_invoke', 'emimage', $item['provider'], 'image_title', $code, $item['data']);
    }
    $output = theme('emimage_image', $field, $item, $formatter, $node, $code, $width, $height, $title, $link, $options);
  }
  return $output;
}

function theme_emimage_formatter_image_image($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emimage', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emimage_formatter_image_embed($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emimage', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emimage_formatter_default($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emimage', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emimage_formatter_image_full($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emimage', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emimage_formatter_image_preview($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emimage', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

function theme_emimage_formatter_image_thumbnail($element) {
  $field = content_fields($element['#field_name'], $element['#type_name']);
  return module_invoke('emimage', 'field_formatter', $field, $element['#item'], $element['#formatter'], $element['#node']);
}

/**
 *  Place the 'embed' code for the image in a form element.
 */
function theme_emimage_image_embed($field, $item, $formatter, $node, $options = array()) {
  if ($item['value'] && $item['provider']) {
    $output = drupal_get_form('emimage_embed_form', $field, $item, $formatter, $node, $options);
  }
  return $output;
}
