<?php
/**
 * Implements hook_action_info().
 */
function ideal_main_action_info() {
  $actions['ideal_main_merge_ideas_action'] = array(
    'type' => 'node',
    'label' => t('Merge Ideas'),
    'configurable' => TRUE,
    );
  return $actions;
}

function ideal_main_merge_ideas_action($entity, $context) {
  dpm($entity);
}

function ideal_main_merge_ideas_action_form($context, &$form_state) {
  dpm($form_state);
  $form = array();
  $selection = $form_satate['selection'];
  $form = array(
    '#type' => 'radios',
    '#title' => t('Select the maser idea'),
    '#description' => t('The master idea is the one, the other ideas will be merged to.'),
    '#default_value' => NULL,
    '#requierd' => TRUE,
    '#options' => $selection, 
  );
  return $form;
}
  
function ideal_main_merge_ideas_action_submit($form, &$form_state) {
  dpm($form_state);
}