<?php
/**
 * @file
 * Heartbeat Views support. Declares the required tables for Views.
 **/

/**
 * Implements hook_views_data().
 */
function heartbeat_views_data() {

  $tables['heartbeat_activity'] = array(
    'table' => array(
      'group' => t('Heartbeat activity'),
      'join' => array(
        'users' => array(
          'table' => 'heartbeat_activity',
          'left_field' => 'uid',
          'field' => 'uid'
         ),
         'node' => array(
           'table' => 'heartbeat_activity',
           'left_field' => 'nid',
           'field' => 'nid'
         ),
       ),
      'base' => array(
        'field' => 'rid',
        'title' => t('Heartbeat activity'),
        'help' => t('Heartbeat activity content to form a news feed or heartbeat stream.'),
        'weight' => 0,
      ),
    ),
    'message' => array(
      'title' => t('Activity message'),
      'help' => t('The singular activity message'),
      'real field' => 'variables',
      'field' => array(
        'handler' => 'views_handler_field_heartbeat_message',
        'click sortable' => TRUE
      ),
    ),
    'uaid' => array(
      'title' => t('User activity ID'),
      'help' => t('The user activity ID'),
      //'real field' => 'uaid',
      'argument' => array(
        'handler' => 'views_handler_argument_numeric'
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'uid' => array(
      'title' => t('User ID'),
      'help' => t('The user ID'),
      //'real field' => 'uid',
      'argument' => array(
        'handler' => 'views_handler_argument_numeric'
      ),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
    ),
    'nid' => array(
      'title' => t('Node ID'),
      'help' => t('Node ID used in the activity message'),
      'field' => array(
        'handler' => 'views_handler_field_node_link',
        'click sortable' => TRUE
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_node_nid',
        'name field' => 'title', // the field to display in the summary.
        'numeric' => TRUE,
        'validate type' => 'nid',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array(
        'base' => 'node',
        'field' => 'nid',
        'label' => 'content',
        'help' => t('Bring in data about the main node.'),
      ),
    ),
    'nid_target' => array(
      'title' => t('Node target ID'),
      'help' => t('Node target ID used in the activity message'),
      'field' => array(
        'handler' => 'views_handler_field_numeric',
        'click sortable' => TRUE
      ),
      'argument' => array(
        'handler' => 'views_handler_argument_node_nid',
        'name field' => 'title', // the field to display in the summary.
        'numeric' => TRUE,
        'validate type' => 'nid',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_numeric',
      ),
      'sort' => array(
        'handler' => 'views_handler_sort',
      ),
      'relationship' => array( // this relationship brings in all fields the node table offers
        'base' => 'node',
        'field' => 'nid',
        'label' => 'content',
        'help' => t('Bring in data about the node being addressed.'),
      ),
    ),
    'created_at' => array(
      'title' => t('Creation date'),
      'real field' => 'timestamp',
      'help' => t('Date the activity was registered'),
      'field' => array(
        'handler' => 'views_handler_field_date',
        'click sortable' => TRUE
      ),
      'sort' => array(
        'handler' => 'views_handler_sort_date',
      ),
      'filter' => array(
        'handler' => 'views_handler_filter_date',
      ),
    ),
    'attachments' => array(
      'title' => t('Attachments'),
      'help' => t('Attachments for the activity message'),
      'real field' => 'uaid',
      'field' => array(
        'handler' => 'views_handler_field_heartbeat_attachments',
        'click sortable' => TRUE
      ),
    ),

  );

  return $tables;

}

/**
 * Implementation of hook_views_handlers().
 */
function heartbeat_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'heartbeat') . '/views',
    ),
    'handlers' => array(
      'views_handler_field_heartbeat_message' => array(
        'parent' => 'views_handler_field',
        'file' => 'views_handler_field_heartbeat_message.inc',
      ),
      'views_plugin_row_heartbeat_view' => array(
        'parent' => 'views_plugin_row',
        'file' => 'views_plugin_row_heartbeat_view.inc',
      ),
    ),
  );
}

/**
 * Implements hook_views_plugins().
 */
function heartbeat_views_plugins() {
  return array(
    'module' => 'heartbeat', // This just tells our themes are elsewhere.
    'row' => array(
      'heartbeat_activity' => array(
        'title' => t('Heartbeat activity'),
        'help' => t('Display the message of the activity view.'),
        'handler' => 'views_plugin_row_heartbeat_view',
        'path' => drupal_get_path('module', 'heartbeat') . '/views',
        //'theme path' => drupal_get_path('module', 'heartbeat') . '/theme',
        'theme' => 'views_view_row_heartbeat',
        'base' => array('heartbeat_activity'), // only works with 'heartbeat_activity' as base.
        'uses options' => TRUE,
        'type' => 'normal',
        'help topic' => 'style-heartbeat',
      ),
    ),
  );
}

/**
 * Template helper for theme_views_view_row_heartbeat.
 */
function template_preprocess_views_view_row_heartbeat(&$vars) {

  $options = $vars['options'];

  // Make sure the variables are defined.
  $vars['heartbeat_activity'] = '';
  $vars['comments'] = '';
  $row_plugin = $vars['view']->style_plugin->row_plugin;

  if (!empty($vars['row']->{$vars['field_alias']})) {
    $uaid = $vars['row']->{$vars['field_alias']};
  }
  // Invalid messages, messages restricted by access or grouped messages
  // result in the absense of a  user activity ID.
  if (!is_numeric($uaid) || !isset($row_plugin->activities[$uaid])) {
    return;
  }

  $heartbeat_activity = $row_plugin->activities[$uaid];

  // This needs to represent what
  // $build = heartbeat_stream_view($heartbeatStream);
  // does for built-in blocks and pages.
  if ($row_plugin->stream->needsModal()) {
    // Add CTools modal requirements.
    heartbeat_ctools_modal_prepare();
  }

  if (variable_get('heartbeat_debug', 0)) {
    drupal_set_message($row_plugin->stream->getErrors());
  }

  // Add the javascript and css files.
  drupal_add_js(drupal_get_path('module', 'heartbeat') . '/js/heartbeat.js');
  drupal_add_css(drupal_get_path('module', 'heartbeat') . '/css/heartbeat.css');

  $heartbeat_activity->view = $vars['view'];
  // Detect which build module should be used.
  $heartbeat_activity->view_mode = (!empty($options['view_mode'])) ? $options['view_mode'] : 'full';
  $content = heartbeat_activity_view($heartbeat_activity);
  //$content = $content['heartbeat_activities'][$heartbeat_activity->uaid];

  $vars['heartbeat_activity'] = drupal_render($content);

}

