<?php
// $Id: argument_selector.action.inc,v 1.1.2.2 2010/03/17 02:56:50 kratib Exp $

/**
 * @file 
 * Passes selected objects as arguments to a page or view.
 */

/**
 * Implementation of hook_action_info().
 */
function views_bulk_operations_argument_selector_action_info() {
  return array(
    'views_bulk_operations_argument_selector_action' => array(
      'description' => t('Pass objects as arguments to a page'),
      'type' => 'system',
      'aggregate' => TRUE,
      'configurable' => TRUE,
      'hooks' => array('any' => TRUE),
    ),
  );
}

/**
* Implementation of a Drupal action.
* Passes selected nodes as arguments to a view.
*/
function views_bulk_operations_argument_selector_action($objects, $context = array()) {
  $base_url = $context['url'];
  // $objects is an array of object IDs, since the action includes aggregate.
  $arguments = implode(',', $objects);
  drupal_goto($base_url . "/" . $arguments);
}

function views_bulk_operations_argument_selector_action_form($context) {
  $form['url'] = array(
    '#title' => t('URL'),
    '#type' => 'textfield',
    '#description' => t('Enter a URL that the user will be sent to.'),
    '#default_value' => @$context['url'],
    '#required' => TRUE,
  );
  return $form;
}

function views_bulk_operations_argument_selector_action_submit($form, $form_state) {
  return array('url' => $form_state['values']['url']);
}

