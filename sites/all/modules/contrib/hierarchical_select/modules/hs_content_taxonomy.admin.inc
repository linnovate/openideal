<?php
// $Id: hs_content_taxonomy.admin.inc,v 1.5 2010/03/01 00:49:43 wimleers Exp $

/**
 * Form definition; configuration form for Hierarchical Select as the widget
 * for a content_taxonomy field.
 *
 * @param $content_type_name
 *   Name of a content type. Provides necessary context.
 * @param $field_name
 *   Name of a field. Provides necessary context.
 */
function hs_content_taxonomy_config_form($form_state, $content_type_name, $field_name) {
  require_once(drupal_get_path('module', 'hierarchical_select') .'/includes/common.inc');

  drupal_add_css(drupal_get_path('module', 'hs_content_taxonomy') .'/hs_content_taxonomy.css');

  $content_type = content_types($content_type_name);

  $field = $content_type['fields'][$field_name];

  // Extract the necessary context from the $field array.
  $vid = $field['vid'];
  $tid = (isset($field['tid'])) ? $field['tid'] : NULL;
  $depth = (empty($field['depth'])) ? 0 : $field['depth'];

  // Add the Hierarchical Select config form.
  $module = 'hs_content_taxonomy';
  $params = array(
    'vid'   => $vid,
    'tid'   => $tid,
    'depth' => $depth,
  );
  $config_id = "content-taxonomy-$field_name";
  $vocabulary = taxonomy_vocabulary_load($vid);
  $defaults = array(
  // Enable the save_lineage setting by default if the multiple parents
  // vocabulary option is enabled.
    'save_lineage' => (int) ($vocabulary->hierarchy == 2),
    'editability' => array(
      'max_levels' => min($depth, _hs_taxonomy_hierarchical_select_get_depth($vid)),
    ),
  );
  // If this config is being created (not edited), then enable the dropbox if
  // this is a "multiple values" field. This allows for an intuitive
  // transition to a Hierarchical Select widget.
  if (variable_get('hs_config_'. $config_id, FALSE) === FALSE) {
    $defaults['dropbox']['status'] = $field['multiple'];
  }
  $strings = array(
    'hierarchy'   => t('vocabulary'),
    'hierarchies' => t('vocabularies'),
    'item'        => t('term'),
    'items'       => t('terms'),
    'item_type'   => t('term type'),
    'entity'      => t('node'),
    'entities'    => t('nodes'),
  );
  $max_hierarchy_depth = min(($depth == 0) ? 9 : $depth, _hs_taxonomy_hierarchical_select_get_depth($vid));
  $preview_is_required = $field['required'];
  $form['hierarchical_select_config'] = hierarchical_select_common_config_form($module, $params, $config_id, $defaults, $strings, $max_hierarchy_depth, $preview_is_required);

  $form['link'] = array(
    '#value' => l('Back to the field configuration', 'admin/content/node-type/'. str_replace('_', '-', $content_type['type']) .'/fields/'. $field_name),
    '#prefix' => '<div class="cck-hierarchical-select-back-link">',
    '#suffix' => '</div>',
    '#weight' => -5,
  );

  $form['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['#content_type_name'] = $content_type_name;
  $form['#field_name'] = $field_name;

  // Add the the submit handler for the Hierarchical Select config form.
  $parents = array('hierarchical_select_config');
  $form['#submit'][] = 'hierarchical_select_common_config_form_submit';
  $form['#hs_common_config_form_parents'] = $parents;

  $form['#submit'][] = 'hs_content_taxonomy_common_config_form_submit';

  return $form;
}

/**
 * Additional submit callback to update the multiple values field setting.
 */
function hs_content_taxonomy_common_config_form_submit( &$form, &$form_state) {
  $config = $form_state['values']['hierarchical_select_config'];
  $multiple_values = ($config['save_lineage'] | $config['dropbox']['status']);
  require_once(drupal_get_path('module', 'content') . '/includes/content.crud.inc');
  $fields = content_field_instance_read(array('type_name' => $form['#content_type_name'], 'field_name' => $form['#field_name']));
  foreach ($fields as $field) {
    $field['multiple'] = $multiple_values;
    content_field_instance_update($field);
  }
}
