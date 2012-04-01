<?php
/**
 * @file
 * Admin options for top_searches module
 */
 
/**
 * Admin UI. Allow to limit number of items in the results list and allow to clear the results
 */
function top_searches_admin_form() {
  $form = array();

  $form['stats'] = array(
    //'#type' => 'markup',
    '#markup' => t('There are currently @rows items in the Top Searches tables.', array('@rows' => top_searches_count_rows())),
  );

  $form['top_searches_block'] = array(
      '#type' => 'fieldset',
      '#title' => t('Top Searches block configuration'),
  );

  $form['top_searches_block']['top_searches_block_items'] = array(
    '#type' => 'textfield',
    '#maxlength' => 2,
    '#size' => 2,
    '#title' => t('Maximum number of items to show in Top searches block'),
    '#default_value' => variable_get('top_searches_block_items', 50),
  );

  $form['top_searches_block']['top_searches_show_counters'] = array(
    '#type' => 'radios',
    '#title' => t('Should counters be presented next to the items in the block?'),
    '#default_value' => variable_get('top_searches_show_counters', 0),
    '#options' => array(t('No'), t('Yes')),
  );

  $form['clear_searches'] = array(
    '#type' => 'submit',
    '#value' => t('Reset search counters'),
    '#weight' => 10,
  );

  return system_settings_form($form);
}

function top_searches_admin_form_validate($form, &$form_state) {
  // In case we need to clear the DB table, redirect:
  if ($form_state['values']['op'] == t('Reset search counters')) {
    drupal_goto('admin/config/search/top_searches/clear');
  }
}

function top_searches_form_clear_confirm() {
  return confirm_form(array(), t('Are you sure you want to reset the Top Searches counters?'), 'admin/config/search/top_searches', t('This operation cannot be undone!'));
}

function top_searches_form_clear_confirm_submit(&$form, &$form_state) {
  if ($form['confirm']) {
    top_searches_form_clear();
  }
}

/**
 * Clears the Top Searches table
 */
function top_searches_form_clear() {
  // We first set the message, so we have the right number of rows
  drupal_set_message(t("The Top Searches counters were reset. @number records were deleted", array('@number' => top_searches_count_rows())));
  db_query("TRUNCATE {top_searches}");
  drupal_goto('admin/config/search/top_searches');
}
