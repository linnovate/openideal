<?php
// $Id: node.rules_forms.inc,v 1.1.2.20 2009/08/25 17:28:58 fago Exp $


/**
 * @file Rules configuration forms for the node module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Condition: Check for content types - Configuration form
 */
function rules_condition_content_is_type_form($settings, &$form) {
  $form['settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Content types'),
    '#options' => node_get_types('names'),
    '#multiple' => TRUE,
    '#default_value' => isset($settings['type']) ? $settings['type'] : array(),
    '#required' => TRUE,
  );
}

/**
 * Some label callbacks for the conditions
 */
function rules_condition_content_is_type_label($settings, $argument_labels) {
  $names = array_intersect_key(node_get_types('names'), $settings['type']);
  return t('@node is @type', $argument_labels + array('@type' => implode(' '. t('or') .' ', $names)));
}

function rules_condition_content_is_published_label($settings, $argument_labels) {
  return t('@node is published', $argument_labels);
}

function rules_condition_content_is_sticky_label($settings, $argument_labels) {
  return t('@node is sticky', $argument_labels);
}

function rules_condition_content_is_promoted_label($settings, $argument_labels) {
  return t('@node is promoted to frontpage', $argument_labels);
}

function rules_condition_content_is_new_label($settings, $argument_labels) {
  return t('@node is new', $argument_labels);
}

/**
 * Action set node author - Label callback
 */
function rules_action_node_set_author_label($settings, $argument_labels) {
  return t('Set the author of @node to @author', $argument_labels);
}

/**
 * Action load node author label callback
 */
function rules_action_node_load_author_label($settings, $argument_labels) {
  return t('Load the @node author', $argument_labels);
}

/**
 * Action load node author variable label callback
 */
function rules_action_node_load_author_variable_label($settings, $argument_labels) {
  return t('@node author', $argument_labels);
}

/**
 * Action set node title - Label callback
 */
function rules_action_set_node_title_label($settings, $argument_labels) {
  return t("Set @node's title", $argument_labels);
}

/**
 * Action "Add a node" configuration form
 */
function rules_action_add_node_form($settings, &$form) {
  $form['settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Content type to be used'),
    '#options' => node_get_types('names'),
    '#default_value' => isset($settings['type']) ? $settings['type'] : '',
    '#description' => t('Select a content type that will be created when this action is invoked.'),
    '#required' => TRUE,
  );
  $form['settings']['node_access'] = array(
    '#type' => 'checkbox',
    '#title' => t('Create content only if the given author has access permission to do so'),
    '#default_value' => isset($settings['node_access']) ? $settings['node_access'] : 0,
  );
}

function rules_action_add_node_variable_label($settings) {
  return t('New content of type @type', array('@type' => $settings['type']));
}

/**
 * Action "Load a node" variable label callback
 */
function rules_action_load_node_variable_label($settings) {
  return t('Content with id @id', array('@id' => $settings['nid']));
}

/**
 * Label callback for the node core actions
 */
function rules_core_node_label_callback($settings, $argument_labels, &$element) {
  if (!isset($element['#info']['label_skeleton'])) {
    $element['#info']['label_skeleton'] = str_replace(t('content'), '@node', $element['#info']['label']);
  }
  return t($element['#info']['label_skeleton'], $argument_labels);
}

/**
 * Action "Delete content" - Label callback
 */
function rules_action_delete_node_label($settings, $argument_labels) {
  return t('Delete @node', $argument_labels);
}

/**
 * The following functions help converting node actions
 * when upgrading from workflow-ng
 */
function workflow_ng_action_node_publish_upgrade(&$element) {
  $element['#name'] = $element['#settings']['published'] ? 'rules_core_node_publish_action' : 'rules_core_node_unpublish_action';
  $element['#settings'] = array('auto_save' => TRUE);
}

function workflow_ng_action_node_promote_upgrade(&$element) {
  $element['#name'] = $element['#settings']['promote'] ? 'rules_core_node_promote_action' : 'rules_core_node_unpromote_action';
  $element['#settings'] = array('auto_save' => TRUE);
}

function workflow_ng_action_node_sticky_upgrade(&$element) {
  $element['#name'] = $element['#settings']['sticky'] ? 'rules_core_node_make_sticky_action' : 'rules_core_node_make_unsticky_action';
  $element['#settings'] = array('auto_save' => TRUE);
}

/**
 * Implementation of hook_rules_upgrade_event_map().
 */
function node_rules_upgrade_event_map() {
  return array('node_submit' => 'node_presave');
}

/**
 * @}
 */
