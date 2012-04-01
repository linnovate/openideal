<?php

/**
 * @file
 * Theme function used by Service Links.
 */

/**
 * Build a single link following the style rules.
 */
function theme_service_links_build_link($variables) {
  $text = $variables['text'];
  $url = $variables['url'];
  $image = $variables['image'];
  $nodelink = $variables['nodelink'];
  $style = $variables['style'];
  $attributes = $variables['attributes'];

  if ($nodelink) {
    $query = isset($url[1]) ? $url[1] : NULL;
 
    switch ($style) {
      case SERVICE_LINKS_STYLE_TEXT:
        $link = array(
          'title' => $text,
          'href' => $url[0],
          //'query' => array($url[1]),
          'query' => $query,
          'attributes' => $attributes,
        );
        break;
      case SERVICE_LINKS_STYLE_IMAGE:
        $link = array(
          'title' => theme('image', array('path' => service_links_expand_path($image), 'alt' => $text)),
          'href' => $url[0],
          'query' => $query,
          'attributes' => $attributes,
          'html' => TRUE,
        );
        break;
      case SERVICE_LINKS_STYLE_IMAGE_AND_TEXT:
        $link = array(
          'title' => theme('image', array('path' => service_links_expand_path($image), 'alt' => $text)) .' '. $text,
          'href' => $url[0],
          'query' => $query,
          'attributes' => $attributes,
          'html' => TRUE,
        );
        break;
      case SERVICE_LINKS_STYLE_EMPTY:
        $link = array(
          'title' => '<span class="element-invisible">' . $text . '</span>',
          'href' => $url[0],
          'query' => $query,
          'attributes' => $attributes,
          'html' => TRUE,
        );
        break;
      default:
        $link = theme($style, $variables);
    }
  }
  else {
    $attributes = array('attributes' => $attributes);
    if (isset($url[1])) {
      $attributes['query'] = $url[1];
    }

    switch ($style) {
      case SERVICE_LINKS_STYLE_TEXT:
        $link = l($text, $url[0], $attributes);
        break;
      case SERVICE_LINKS_STYLE_IMAGE:
        $attributes = array_merge($attributes, array('html' => TRUE));
        $link = l(theme('image', array('path' => service_links_expand_path($image), 'alt' => $text)), $url[0], $attributes);
        break;
      case SERVICE_LINKS_STYLE_IMAGE_AND_TEXT:
        $attributes = array_merge($attributes, array('html' => TRUE));
        $link = l(theme('image', array('path' => service_links_expand_path($image), 'alt' => $text)) .' '. $text, $url[0], $attributes);
        break;
      case SERVICE_LINKS_STYLE_EMPTY:
        $attributes = array_merge($attributes, array('html' => TRUE));
        $link = l('<span class="element-invisible">' . $text . '</span>', $url[0], $attributes);
        break;
      case SERVICE_LINKS_STYLE_FISHEYE:
        $attributes['attributes']['class'] = isset($attributes['attributes']['class']) ? array_merge($attributes['attributes']['class'], array('fisheyeItem')) : array('fisheyeItem');
        $attributes = array_merge($attributes, array('html' => TRUE));
        $link = l(theme('image', array('path' => service_links_expand_path($image, 'fisheye'), 'alt' => $text, 'getsize' => FALSE)) .'<span>'. $text .'</span>', $url[0], $attributes);
        break;
      default:
        $link = theme($style, $variables);
    }
  }

  return $link;
}

/**
 * Format the items shown in the node.
 */
function theme_service_links_node_format($variables) {
  $links = $variables['links'];
  $label = $variables['label'];

  if (isset($label) && !empty($label)) {
    return '<div class="service-links"><div class="service-label">'. t('@label', array('@label' => $label)) .' </div>'. theme('links', array('links' => $links)) .'</div>';
  }
  else {
    return '<div class="service-links">'. theme('links', array('links' => $links)) .'</div>';
  }
}

/**
 * Format the items shown in one of the provided blocks.
 */
function theme_service_links_block_format($variables) {
  $items = $variables['items'];
  $style = $variables['style'];

  if (empty($items)) {
    return;
  }

  switch ($style) {
    case SERVICE_LINKS_STYLE_IMAGE:
      $output = implode(' ', $items);
      break;
    default:
      $output = theme('item_list', array('items' => array_values($items)));
      break;
  }

  return '<div class="service-links">' . $output . '</div>';
}

/**
 * Format the items shown in the Fisheye block.
 */
function theme_service_links_fisheye_format($variables) {
  $items = $variables['items'];

  drupal_add_js(service_links_expand_path('interface.js', 'javascript'), array('weight' => 90));
  drupal_add_js(service_links_expand_path('service_links_fisheye.js', 'javascript'), array('weight' => 90));
  drupal_add_css(service_links_expand_path('service_links_fisheye.css', 'css'));

  return "<div class=\"fisheye\"><div class=\"fisheyeContainer\">\r\n". implode("\r\n", $items) ."\r\n</div></div>";
}
