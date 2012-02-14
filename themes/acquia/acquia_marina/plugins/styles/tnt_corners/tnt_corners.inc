<?php
// $Id: tnt_corners.inc,v 1.1.2.2 2009/06/05 08:00:57 jwolf Exp $

/**
 * @file styles/tnt_corners/tnt_corners.inc
 * Definition of the Acquia Marina corners panel style.
 * 
 * Based  on rounded_corner.inc from panels.module
 * 
 */

// ---------------------------------------------------------------------------
// Panels hooks.

/**
 * Implementation of THEME_STYLENAME_panels_style().
 * 
 * Where THEME is the theme name, and STYLENAME matches this inc filename.
 */
function acquia_marina_tnt_corners_panels_styles() {
  return array(
    'tnt_corners' => array(
      'title' => t('Acquia Marina Rounded Corners'),
      'description' => t('Presents the panes or panels with a themed rounded corner box around them'),
      'render panel' => 'acquia_marina_tnt_corners_style_render_panel',
      'render pane' => 'acquia_marina_tnt_corners_style_render_pane',
      'settings form' => 'acquia_marina_tnt_corners_style_settings_form',
      'hook theme' => array(
        'acquia_marina_tnt_corners_box' => array(
          'arguments' => array('content' => NULL),
          'path' => panels_get_path('plugins/styles/tnt_corners'),
          'template' => 'acquia-marina-tnt-corners-box',
        ),
      ),
    ),
  );
}

// ---------------------------------------------------------------------------
// Panels style plugin callbacks.

/**
 * Render callback.
 * 
 * Declared by name in THEME_STYLENAME_panels_style() above.
 *
 * @ingroup themeable
 */
function theme_acquia_marina_tnt_corners_style_render_panel($display, $panel_id, $panes, $settings) {
  $output = '';

  // Determine where to put the box. If empty or 'pane' around each pane. If
  // 'panel' then just around the whole panel.
  $where = empty($settings['tnt_corner_location']) ? 'pane' : $settings['tnt_corner_location'];

  $print_separator = FALSE;
  foreach ($panes as $pane_id => $pane) {
    $text = panels_render_pane($pane, $display->content[$pane_id], $display);
    if ($text) {
      // Add the separator if we've already displayed a pane.
      if ($print_separator) {
        $output .= '<div class="panel-separator"></div>';
      }

      if ($where == 'pane') {
        $output .= theme('acquia_marina_tnt_corners_box', $text);
      }
      else {
        $output .= $text;
        $print_separator = TRUE;
      }
    }
  }

  if ($where == 'panel') {
    $output = theme('acquia_marina_tnt_corners_box', $output);
  }

  acquia_marina_add_tnt_corners_css();

  return $output;
}

/**
 * Render callback for a single pane.
 */
function theme_acquia_marina_tnt_corners_style_render_pane($content, $pane, $display) {
  $output = theme('panels_pane', $content, $pane, $display);

  if (!$output) {
    return;
  }

  // Just stick a box around the standard theme_panels_pane.
  $output = theme('acquia_marina_tnt_corners_box', $output);
  acquia_marina_add_tnt_corners_css();
  return $output;
}

function acquia_marina_add_tnt_corners_css() {
  static $css_loaded;
  if (!$css_loaded == TRUE) {
    $filename = drupal_get_path('theme', 'acquia_marina') . '/plugins/styles/tnt_corners/tnt_corners.css';
    drupal_add_css($filename, 'module', 'all', FALSE);
    $css_loaded = TRUE;
  }
}

/**
 * Settings form callback. 
 */
function acquia_marina_tnt_corners_style_settings_form($style_settings) {
  $form['tnt_corner_location'] = array(
    '#type' => 'select',
    '#title' => t('Box around'),
    '#options' => array(
      'pane' => t('Each pane'),
      'panel' => t('Each panel'),
    ),
    '#default_value' => (isset($style_settings['tnt_corner_location'])) ? $style_settings['tnt_corner_location'] : 'pane',
    '#description' => t('Choose whether to include the box around each pane (piece of content) or panel (each column or region)'),
  );

  return $form;
}
