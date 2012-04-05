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
  $master = $context['master'];
  if ($master != $entity->nid) {
    $master_wrp = entity_metadata_wrapper('node', $master);
    $merge_wrp = entity_metadata_wrapper('node', $entity);
    $merge_wrp->field_merge_to->set($master_wrp);
    $merge_wrp->save();
  }
}

function ideal_main_merge_ideas_action_form($context, &$form_state) {
  $form = array();
  $selections = $form_state['selection'];
  if (count($selections) < 2) {
    return t("There need to select at least 2 ideas to be merge.");
  }
  foreach ($selections as $id) {
    $wrp = entity_metadata_wrapper('node', $id);
    $title = $wrp->title->value();
    $options[$id] = $title;
  }
  $form['merge_idea'] = array(
    '#type' => 'radios',
    '#title' => t('Select the master idea'),
    '#description' => t('The master idea is the one, the other ideas will be merged to.'),
    '#default_value' => 0,
    '#requierd' => TRUE,
    '#options' => $options, 
 
  );
  return $form;
}
  
function ideal_main_merge_ideas_action_submit($form, &$form_state) {
  return array(
    'master' => $form_state['values']['merge_idea'],
  );
}