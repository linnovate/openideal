<?php
// $Id: views_bulk_operations.rules.inc,v 1.1.2.3 2010/05/05 00:27:02 kratib Exp $

/**
 * Implementation of hook_rules_action_alter().
 */
function views_bulk_operations_rules_action_info_alter(&$actions) {
  foreach (module_invoke_all('views_bulk_operations_object_info') as $type => $info) {
    $actions['views_bulk_operations_action_' . $type] = array(
      'label' => t('Execute a VBO programmatically on !type', array('!type' => $type)),
      // Make sure there is something passed for $object to the action.
      'arguments' => array('object' => array('type' => $type, 'label' => t('Object'))),
      'module' => 'Views Bulk Operations',
      'eval input' => array(),
      // Let the rules system for executing core style actions execute it.
      'base' => 'rules_core_action_execute',
      'action_name' => 'views_bulk_operations_action',
      'configurable' => TRUE,
    );
  }
}

