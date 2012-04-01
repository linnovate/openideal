<?php

/**
 * @file
 *
 * @author greenSkin
 */

/*******************************************************************************
 * Callback Functions, Forms, and Tables
 ******************************************************************************/

/**
 * Settings form for module filter.
 */
function module_filter_settings() {
  $form['module_filter_tabs'] = array(
    '#type' => 'checkbox',
    '#title' => t('Tabs'),
    '#description' => t('Divide module groups into tabbed list.'),
    '#default_value' => variable_get('module_filter_tabs', 1)
  );
  $form['tabs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Tabs'),
    '#description' => t('Settings used with the tabs view of the modules page.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE
  );
  $form['tabs']['module_filter_count_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Number of enabled modules'),
    '#description' => t('Display the number of enabled modules in the active tab along with the total number of modules.'),
    '#default_value' => variable_get('module_filter_count_enabled', 1)
  );
  $form['tabs']['module_filter_visual_aid'] = array(
    '#type' => 'checkbox',
    '#title' => t('Visuals for newly enabled and disabled modules'),
    '#description' => t("Adds a basic count to tabs of modules being enabled/disabled and colors the module row pending it's being enabled or disabled"),
    '#default_value' => variable_get('module_filter_visual_aid', 1)
  );
  $form['tabs']['module_filter_dynamic_save_position'] = array(
    '#type' => 'checkbox',
    '#title' => t('Save dynamic positioning'),
    '#description' => t("DEVELOPMENTAL: For sites with lots of tabs, enable to help keep the 'Save configuration' button more accessible."),
    '#default_value' => variable_get('module_filter_dynamic_save_position', 0)
  );
  return system_settings_form($form);
}
