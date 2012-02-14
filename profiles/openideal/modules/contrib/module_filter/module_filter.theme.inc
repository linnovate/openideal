<?php

/**
 * @file
 *
 * @author greenSkin
 */

/**
 * A router theme function.
 *
 * Appropriately themes the system modules page with alterations and based on
 * set variables
 *
 * @param $form
 *   An associative array containing the structure of the form.
 *
 * @return
 *   An output string.
 */
function theme_module_filter_system_modules($variables) {
  $form = $variables['form'];

  drupal_add_css(drupal_get_path('module', 'module_filter') .'/css/module_filter.css');

  $output = '';
  if (variable_get('module_filter_tabs', 1)) {
    drupal_add_css(drupal_get_path('module', 'module_filter') .'/css/module_filter_tab.css');
    drupal_add_js(drupal_get_path('module', 'module_filter') .'/js/module_filter_tab.js');

    drupal_add_js(array('moduleFilter' => array('visualAid' => variable_get('module_filter_visual_aid', 1))), 'setting');

    if (variable_get('module_filter_dynamic_save_position', 0)) {
      drupal_add_js(drupal_get_path('module', 'module_filter') .'/js/dynamic_position.js');
    }

    $form['module_filter']['#size'] = 45;
    $output .= theme('module_filter_system_modules_tabs', $form);
  }
  else {
    drupal_add_js(drupal_get_path('module', 'module_filter') .'/js/module_filter.js');
    $form['module_filter']['#prefix'] = '<div id="module-filter-wrapper" style="display: none;">';
    $form['module_filter']['#suffix'] = '</div>';
    $output = drupal_render($form['module_filter']);
    $form['#theme'] = 'system_modules';
    $output .= drupal_render($form['modules']);
    $output .= drupal_render($form['actions']);
  }
  return $output;
}

function theme_module_filter_modules_table($variables) {
  $form = $variables['form'];

  // Individual table headers.
  $rows = array();
  // Iterate through all the modules, which are
  // children of this fieldset.
  foreach (element_children($form) as $key) {
    // Stick it into $module for easier accessing.
    $module = $form[$key];
    $row = array();
    unset($module['enable']['#title']);
    $row[] = array('class' => array('checkbox'), 'data' => drupal_render($module['enable']));
    $label = '<label';
    if (isset($module['enable']['#id'])) {
      $label .= ' for="'. $module['enable']['#id'] .'"';
    }
    $row[] = $label .'><strong>' . drupal_render($module['name']) . '</strong></label>';
    $row[] = drupal_render($module['version']);
    // Add the description, along with any modules it requires.
    $description = drupal_render($module['description']);
    if ($module['#requires']) {
      $description .= '<div class="admin-requirements">' . t('Requires: !module-list', array('!module-list' => implode(', ', $module['#requires']))) . '</div>';
    }
    if ($module['#required_by']) {
      $description .= '<div class="admin-requirements">' . t('Required by: !module-list', array('!module-list' => implode(', ', $module['#required_by']))) . '</div>';
    }
    $row[] = array('data' => $description, 'class' => array('description'));
    // Display links (such as help or permissions) in their own columns.
    foreach (array('help', 'permissions', 'configure') as $key) {
      $row[] = array('data' => drupal_render($module['links'][$key]), 'class' => array($key));
    }

    $id = module_filter_get_id($module['#package']);
    $rows[] = array(
      'data' => $row,
      'class' => array($id .'-tab-content')
    );
  }

  return theme('table', array('header' => $form['#header'], 'rows' => $rows, 'attributes' => array('class' => array('package'))));
}

/**
 * Theme callback for the modules tabbed form.
 */
function theme_module_filter_system_modules_tabs($variables) {
  $form = $variables['form'];

  $count_enabled = variable_get('module_filter_count_enabled', 1);

  // Display packages.
  $all = t('All');
  $all_count = ($count_enabled) ? '<span class="counts">' . t('!enabled of !total', array('!enabled' => $form['#tab_counts'][$all]['enabled'], '!total' => $form['#tab_counts'][$all]['total'])) . '</span>' : '';
  $tabs = array('all' => '<li class="active"><a id="all-tab" class="project-tab" href="#all">' . $all . $all_count . '</a></li>');
  foreach ($form['#packages'] as $package) {
    $id = module_filter_get_id($package);

    $count = ($count_enabled) ? '<span class="counts">' . t('!enabled of !total', array('!enabled' => $form['#tab_counts'][$package]['enabled'], '!total' => $form['#tab_counts'][$package]['total'])) . '</span>' : '';
    $tabs[$id] = '<li><a id="' . $id . '-tab" class="project-tab" href="#' . str_replace('-', '_', $id) . '">' . $package . $count . '</a></li>';
  }

  $output = '<div id="module-filter-wrapper">';
  $output .= '<div id="module-filter-left">';
  $output .= '<div id="module-filter-tabs"><ul>'. implode($tabs) . '</ul></div>';
  $output .= '<div id="module-filter-submit">' . drupal_render($form['actions']) . '</div></div>';
  $output .= '<div id="module-filter-right"><div id="module-filter-squeeze">' . drupal_render($form['module_filter']);
  $output .= drupal_render($form['modules']) . '</div></div>';
  $output .= '<div class="clear-block"></div>';
  $output .= '</div>';
  $output .= drupal_render_children($form);
  return $output;
}
