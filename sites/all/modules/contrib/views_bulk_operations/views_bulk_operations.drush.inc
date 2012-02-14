<?php
// $Id: views_bulk_operations.drush.inc,v 1.1.2.6 2010/02/23 21:12:37 kratib Exp $

/**
 * Implementation of hook_drush_help().
 */
function views_bulk_operations_drush_help($section) {
  switch ($section) {
    case 'drush:vbo-list':
      return dt('List all Views Bulk Operations (VBO) views, along with the operations associated with each.');
    case 'drush:vbo-execute':
      return dt('Execute a bulk operation based on a Views Bulk Operations (VBO) view.');
  }
}

/**
 * Implementation of hook_drush_command().
 */
function views_bulk_operations_drush_command() {
  $items['vbo-list'] = array(
    'callback' => 'views_bulk_operations_drush_list',
    'description' => 'List all Views Bulk Operations (VBO) views, along with the operations associated with each.',
  );
  $items['vbo-execute'] = array(
    'callback' => 'views_bulk_operations_drush_execute',
    'description' => 'Execute a bulk operation based on a Views Bulk Operations (VBO) view.',
    'arguments' => array(
      'vid' => 'ID or name of the view to be executed.',
      'operation' => 'Callback name of the operation to be applied on the view results.',
      'type:[name=]value ...' => 'Parameters to be passed as view input filters, view arguments or operation arguments, where type is respectively {input, argument, operation}.',
    ),
    'examples' => array(
      '$ drush vbo-execute admin_content node_publish_action' => 
        'Publish nodes returned by view admin_content.',
      '$ drush vbo-execute 44 node_assign_owner_action operation:owner_uid=3' => 
        'Change node ownership on nodes returned by view #44, passing argument owner_uid=3 to the action.',
      '$ drush vbo-execute admin_content node_unpublish_action input:type=expense argument:3' => 
        'Unpublish nodes returned by view admin_content, filtering results of type expense and passing value 3 as first view argument.',
    ),
  );
  return $items;
}

/**
 * Implementation of 'vbo list' command.
 */
function views_bulk_operations_drush_list() {
  // Impersonate admin.
  global $user;
  $user = user_load(array('uid' => '1'));
  session_save_session(FALSE);

  // Find all VBO views and their associated operations.
  $rows = array(array(sprintf('%5s', dt('VID')), dt('NAME'), dt('DESCRIPTION'), dt('OPERATIONS')));
  foreach (views_get_all_views() as $name => $view) {
    foreach (array_keys($view->display) as $display) {
      $display_options = &$view->display[$display]->display_options;
      if (isset($display_options['style_plugin']) && $display_options['style_plugin'] == 'bulk') {
        $view->build($display);
        $operations = array();
        foreach (array_filter($display_options['style_options']['selected_operations']) as $operation) {
          if (isset($view->style_plugin->options['all_operations'][$operation])) {
            $operations[] = $view->style_plugin->options['all_operations'][$operation]['label'] .' ('. $operation .')';
          }
        }
        $operations[] = "---------------";
        $rows[] = array(
          sprintf('%5d', $view->vid),
          $view->name,
          $view->description,
          implode("\n", $operations),
        );
      }
    }
  }
  drush_print_table($rows, TRUE);
}

/**
 * Implementation of 'vbo execute' command.
 */ 
function views_bulk_operations_drush_execute($vid = NULL, $operation = NULL) {
  // Parse arguments.
  if (is_null($vid)) {
    drush_set_error('VIEWS_BULK_OPERATIONS_MISSING_VID', dt('Please specify a view ID to execute.'));
    return;
  }
  if (is_null($operation)) {
    drush_set_error('VIEWS_BULK_OPERATIONS_MISSING_OPERATION', dt('Please specify an operation to execute.'));
    return;
  }
  $args = func_get_args();
  $view_exposed_input = array();
  $operation_arguments = array();
  $view_arguments = array();
  if (count($args) > 2) for ($i=2; $i<count($args); $i++) {
    $parts = array();
    if (FALSE === preg_match('/^(?<type>\w+):(?:(?<name>\w+)=)?(?<value>(.*?))$/', $args[$i], $parts)) {
      drush_set_error('VIEWS_BULK_OPERATIONS_INVALID_PARAMETER', dt('The parameter %arg should be of the form type:[name=]value where type in {input, argument, operation}.', array('%arg' => $args[$i])));
      return;
    }
    switch ($parts['type']) {
      case 'input':
        $view_exposed_input[$parts['name']] = $parts['value'];
        break;
      case 'operation':
        $operation_arguments[$parts['name']] = $parts['value'];
        break;
      case 'argument':
        $view_arguments[] = $parts['value'];
        break;
      default:
        drush_set_error('VIEWS_BULK_OPERATIONS_UNKNOWN_PARAMETER', dt('The parameter type %type is unknown. Please specify either input, argument or operation.', array('%type' => $parts['type'])));
        return;
    }
  }

  // Impersonate admin.
  global $user;
  $user = user_load(array('uid' => '1'));
  session_save_session(FALSE);

  // Execute the VBO.
  views_bulk_operations_execute($vid, $operation, $operation_arguments, $view_exposed_input, $view_arguments);
}

