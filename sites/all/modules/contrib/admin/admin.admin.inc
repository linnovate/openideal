<?php
// $Id: admin.admin.inc,v 1.1.2.3.2.7 2010/08/01 05:19:03 yhahn Exp $

/**
 * System settings form for admin toolbar.
 */
function admin_settings_form() {  
  $form['admin_toolbar'] = array(
    '#tree' => TRUE,
    '#theme' => 'admin_settings_form'
  );
  $form['admin_toolbar']['layout'] = array(
    '#type' => 'select',
    '#title' => t('Layout'),
    '#description' => t('Choose a layout for the admin toolbar. Vertical works well with large, wide screens. Horizontal works well on screens with limited real estate.'),
    '#options' => array(
      'horizontal' => t('Horizontal'),
      'vertical' => t('Vertical'),
    ),
    '#default_value' => admin_get_settings('layout'),
  );
  $form['admin_toolbar']['position'] = array(
    '#type' => 'select',
    '#title' => t('Position'),
    '#description' => t('Choose a position for the admin toolbar that will not collide with other elements in your current theme.'),
    '#options' => array(
      'ne' => t('Top right'),
      'nw' => t('Top left'),
      'se' => t('Bottom right'),
      'sw' => t('Bottom left'),
    ),
    '#default_value' => admin_get_settings('position'),
  );
  $form['admin_toolbar']['behavior'] = array(
    '#type' => 'select',
    '#title' => t('State on new pages'),
    '#description' => t('Choose toolbar behavior on new page loads.'),
    '#options' => array(
      'df' => t('Remember from previous page'),
      'ah' => t('Collapsed'),
    ),
    '#default_value' => admin_get_settings('behavior'),
  );

  $block_info = array();
  foreach (module_implements('block') as $module) {
    $module_blocks = module_invoke($module, 'block', 'list');
    if ($module_blocks) {
      foreach ($module_blocks as $delta => $info) {
        $block_info["{$module}-{$delta}"] = $info;
      }
    }
  }
  // Store block info for use in form processing.
  $form['#block_info'] = $block_info;

  // Get default block options.
  $options = array();
  $enabled = array_filter(admin_get_settings('blocks')) + admin_get_default_blocks(TRUE);
  foreach ($block_info as $bid => $info) {
    if (!empty($enabled[$bid])) {
      $options[$bid] = $info['info'];
    }
  }
  asort($options);

  $form['admin_toolbar']['blocks'] = array(
    '#type' => 'checkboxes',
    '#options' => $options,
    '#default_value' => drupal_map_assoc(array_keys(array_filter(admin_get_settings('blocks')))),
  );

  // Grab an array of human-readable module names. It hurts that we need to do this.
  $result = db_query("SELECT name,type,info FROM {system} WHERE type = 'module' AND status = 1");
  $modules = array();
  while ($row = db_fetch_object($result)) {
    $info = unserialize($row->info);
    $modules[$row->name] = isset($info['name']) ? $info['name'] : $row->name;
  }
  foreach (array_diff_key($block_info, $options) as $bid => $info) {
    $module = array_shift(explode('-', $bid));
    $module = isset($modules[$module]) ? $modules[$module] : $module;
    $custom_options[$module][$bid] = $info['info'];
  }
  $custom_options = $custom_options + array(-1 => '<'. t('Choose a block') .'>');
  ksort($custom_options);
  $form['admin_toolbar']['custom'] = array(
    '#type' => 'select',
    '#options' => $custom_options,
    '#default_value' => -1,
    '#element_validate' => array('admin_settings_form_custom_validate'),
    '#description' => t('Enable other blocks for use in the admin toolbar.'),
  );
  $form['admin_toolbar']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add block'),
  );

  $form = system_settings_form($form);

  // Add a submit handler for expanding block information.
  array_unshift($form['#submit'], 'admin_settings_form_submit');

  return $form;
}

/**
 * Submit handler for admin_settings_form().
 *
 * Retrieves cache type for each block and saves that instead of `1` for each
 * enabled block.
 */
function admin_settings_form_submit($form, &$form_state) {
  if (isset($form['#block_info'], $form_state['values']['admin_toolbar']['blocks'])) {
    $blocks = array();
    foreach (array_keys(array_filter($form_state['values']['admin_toolbar']['blocks'])) as $bid) {
      $blocks[$bid] = isset($form['#block_info'][$bid]['cache']) ? $form['#block_info'][$bid]['cache'] : BLOCK_NO_CACHE;
    }
    $form_state['values']['admin_toolbar']['blocks'] = $blocks;
  }
}

/**
 * Element validator for custom block adder. Moves values within
 * $form_state['values'] and prevents additional variables from being saved.
 */
function admin_settings_form_custom_validate($element, &$form_state) {
  if ($form_state['values']['admin_toolbar']['custom'] != -1) {
    $bid = $form_state['values']['admin_toolbar']['custom'];
    $form_state['values']['admin_toolbar']['blocks'][$bid] = $bid;
  }
  unset($form_state['values']['admin_toolbar']['custom']);
  unset($form_state['values']['admin_toolbar']['add']);
}

/**
 * Theme function for the admin settings form.
 */
function theme_admin_settings_form($form) {
  $rows = array();

  // Admin blocks
  $header = array(array('data' => t('Administrative blocks'), 'colspan' => 2));
  foreach (array_keys(admin_get_default_blocks(TRUE)) as $block) {
    $rows[] = array(array(
        'data' => drupal_render($form['blocks'][$block]),
        'colspan' => 2
    ));
  }

  // "Custom" blocks
  $rows[] = array(array('data' => t('Other blocks'), 'colspan' => 2, 'header' => TRUE));
  foreach (element_children($form['blocks']) as $block) {
    if (empty($form['blocks'][$block]['#printed'])) {
      $rows[] = array(array(
        'data' => drupal_render($form['blocks'][$block]),
        'colspan' => 2
      ));
    }
  }
  $rows[] = array(drupal_render($form['custom']), drupal_render($form['add']));
  $form['blocks']['#children'] = theme('table', $header, $rows);
  return drupal_render($form);
}

/**
 * Rebuild form.
 */
function admin_settings_rebuild($form_state) {
  $form = array();
  $form['rebuild'] = array(
    '#type' => 'fieldset',
    '#title' => t('Wipe and rebuild'),
    '#description' => t('If the administrative menu is not working properly, you may need to wipe and rebuild it from scratch. <strong>This option will reset any customizations made to your administrative menu items.</strong>'),
  );
  $form['rebuild']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Rebuild'),
    '#submit' => array('admin_settings_rebuild_submit'),
  );
  return $form;
}

/**
 * Wipe and rebuild the admin menu.
 */
function admin_settings_rebuild_submit($form, &$form_state) {
  db_query("DELETE FROM {menu_links} WHERE menu_name = 'admin'");
  menu_rebuild();
  cache_clear_all('admin', 'cache_block', TRUE);
}
