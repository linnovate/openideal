<?php

/**
 * @file
 * Theme functions for userpoints.module
 */

/**
 * Theme implementation to display a userpoints category.
 */
function theme_userpoints_view_category($variables) {
  $element = $variables['element'];
  $element += array(
    '#attributes' => array(),
  );

  $output = '';
  if (!empty($element['#title'])) {
    $output .= '<h3 ' . drupal_attributes(($element['#attributes'])) . '>' . $element['#title'] . '</h3>';
  }

  $output .= '<dl ' . drupal_attributes(($element['#attributes'])) . '>';
  $output .= drupal_render_children($element);
  $output .= '</dl>';

  return $output;
}

/**
 * Theme implementation to display a userpoints item.
 */
function theme_userpoints_view_item($variables) {
  $element = $variables['element'];
  $element += array(
    '#attributes' => array(),
  );

  $output = '<dt ' . drupal_attributes(($element['#attributes'])) . '>' . $element['#title'] . '</dt>';
  $output .= '<dd ' . drupal_attributes(($element['#attributes'])) . '>' . $element['#value'] . '</dd>';

  return $output;
}
