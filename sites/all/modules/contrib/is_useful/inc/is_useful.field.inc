<?php

/**
 * @file
 *
 * Contains all Field API related code for the is_useful module.
 */

/**
 * Implementation of hook_field_info().
 */
function is_useful_field_info() {
  return array(
    'is_useful' => array(
      'label' => t('Is Useful'),
      'description' => t('Store a simple Yes/No/Maybe for this piece of content.'),
      'default_widget' => 'is_useful_exposed',
      'default_formatter' => 'is_useful_formatter_default',
      'settings' => array(
        'axis' => 'usefulness',
      ),
    ),
  );
}


/**
 * Implementation of hook_field_settings_form().
 */
function is_useful_field_settings_form($field, $instance) {
  $form['axis'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => 'Voting Tag',
    '#options' => is_useful_get_tags(),
    '#description' => t('The tag this rating will affect. Enter a property on which that this rating will affect, such as <em>quality</em>, <em>satisfaction</em>, <em>overall</em>, etc.'),
    '#default_value' => isset($field['settings']['axis']) ? $field['settings']['axis'] : '',
    '#disabled' => field_has_data($field),
  );

  return $form;
}

function is_useful_form_field_ui_field_edit_form_alter(&$form, $form_state) {
  $field = $form['#field'];
  $instance = $form['#instance'];
  if ($field['type'] == 'is_useful') {
    // Multiple values is not supported with is_useful.
    $form['field']['cardinality']['#access'] = FALSE;
    $form['field']['cardinality']['#value'] = 1;
    $form['instance']['default_value_widget']['#access'] = FALSE;
  }
}

/**
 * Implementation of hook_field_widget_info().
 */
function is_useful_field_widget_info() {
  return array(
    'is_useful_exposed' => array(
      'label' => t('Yes / No / Maybe'),
      'field types' => array('is_useful'),
      'behaviors' => array('multiple values' => FIELD_BEHAVIOR_NONE),
    ),
  );
}

/**
 * Implementation of hook_field_widget_form().
 */
function is_useful_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
}


function is_useful_field_formatter_info() {
  return array(
    'is_useful_formatter_default' => array(
      'label' => t('Default'),
      'field types' => array('is_useful'),
    ),
  );
}

/**
 * Implementation of hook_field_formatter_view().
 */
function is_useful_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
  $element = array();
  list($entity_id, $entity_vid, $bundle) = entity_extract_ids($entity_type, $entity);
  $settings = array(
    'entity_type' => $entity_type,
    'entity_id' => $entity_id,
    'axis' => $field['settings']['axis'],
  );

  $element[0] = drupal_get_form('is_useful_exposed_formatter', $settings);
  return $element;
}
