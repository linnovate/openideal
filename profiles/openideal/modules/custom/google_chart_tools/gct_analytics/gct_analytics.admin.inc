<?php
/**
 * Provide admin settings form
 */
function gtc_analytics_admin_settings($form, &$form_state) {
  $form = array();
  // Get available charts
  $options = array();
  $charts = gct_get_charts();
  foreach ($charts as $chart) {
    $options[$chart['id']] = $chart['title'];
  }

  // Generate default charts selection
  $default = array();
  if (!($default = variable_get('chart_visible', array()))) {
    if (empty($default)) {
      foreach ($options as $id => $label) {
        $default[] = $id;
      }
    }
  }
  
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['display']['chart_visible'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Show analytics for the following site items'),
    '#default_value' => $default,
    '#options' => $options,
    '#description' => t('The checked charts will be shown on the analytics page.'),
  );
  
  return system_settings_form($form);
}