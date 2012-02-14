<?php

/**
 * @file
 * Theme callbacks for the metatag module.
 */

/**
 * Theme callback for a normal meta tag.
 *
 * The format is:
 * <meta name="[name]" content="[value]" />
 */
function theme_metatag($variables) {
  $element = &$variables['element'];
  element_set_attributes($element, array('name', '#value' => 'content'));
  unset($element['#value']);
  return theme('html_tag', $variables);
}

/**
 * Theme callback for a normal meta tag.
 *
 * The format is:
 * <meta http-equiv="[name]" content="[value]" />
 */
function theme_metatag_http_equiv($variables) {
  $element = &$variables['element'];
  element_set_attributes($element, array('#name' => 'http-equiv', '#value' => 'content'));
  unset($element['#value']);
  return theme('html_tag', $variables);
}

/**
 * Theme callback for a rel link tag.
 *
 * The format is:
 * <link rel="[name]" href="[value]" />
 */
function theme_metatag_link_rel($variables) {
  $element = &$variables['element'];
  element_set_attributes($element, array('#name' => 'rel', '#value' => 'href'));
  unset($element['#value']);
  return theme('html_tag', $variables);
}

/**
 * Theme callback for a rev link tag.
 *
 * The format is:
 * <link rev="[name]" href="[value]" />
 */
function theme_metatag_link_rev($variables) {
  $element = &$variables['element'];
  element_set_attributes($element, array('#name' => 'rev', '#value' => 'href'));
  unset($element['#value']);
  return theme('html_tag', $variables);
}
