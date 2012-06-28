<?php
/**
 * Implements hook_action_info().
 */
function ideal_main_action_info() {
  $actions = array();
  $actions['ideal_main_merge_ideas_action'] = array(
    'type' => 'node',
    'label' => t('Merge Ideas'),
    'configurable' => TRUE,
  );
  $actions['ideal_main_set_status_action'] = array(
    'type' => 'node',
    'label' => t('Set Status (in action)'),
    'configurable' => TRUE,
  );
  
  return $actions;
}

//// SET IN ACTION STATUS ////

/**
 * The set status (in action) action to take.
 */
function ideal_main_set_status_action($entity, $context) {
  $status = $context['status'];
  $wrp = entity_metadata_wrapper('node', $entity);
  $wrp->field_in_action->set($status);
  $wrp->save();
}

/**
 * The set status form - select the status value.
 */
function ideal_main_set_status_action_form($context, &$form_state) {
  $field = field_info_field('field_in_action');
  $list = list_allowed_values($field);
  $form =array();
  $form['status'] = array(
    '#type' => 'select',
    '#titile' => t('Select status'),
    '#description' => t('The new In Action status to set.'),
    '#default_value' => 0,
    '#requierd' => TRUE,
    '#options' => $list, 
  ); 
  return $form;
}

/**
 * The set status from submit. 
 */
function ideal_main_set_status_action_submit($form, &$form_state) {
  return array(
    'status' => $form_state['values']['status'],
  );
}

//// MERGE IDEAS ////

/**
 * The action to take on merge ideas.
 */
function ideal_main_merge_ideas_action($entity, $context) {
  $master = $context['master'];
  if ($master != $entity->nid) {
    $master_wrp = entity_metadata_wrapper('node', $master);
    $merge_wrp = entity_metadata_wrapper('node', $entity);
    $merge_wrp->field_merge_to->set($master_wrp);
    $merge_wrp->save();
  }
}

/**
 * The merge ideas form - select the master idea.
 */
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
  
/**
 * The merge ideas form submit.
 */
function ideal_main_merge_ideas_action_submit($form, &$form_state) {
  return array(
    'master' => $form_state['values']['merge_idea'],
  );
}