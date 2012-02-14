<?php
// $Id: heartbeat_views.views.inc,v 1.1.2.2.2.17 2010/10/16 09:16:36 stalski Exp $

/**
 * @file
 *   Views integration for heartbeat.
 */

/**
 * Implementation of views hook hook_views_data().
 *
 * @see http://views.doc.logrus.com/
 * @see http://www.lullabot.com/articles/hacking-views-part-1-basic-concepts
 * @return array views data
 */
function heartbeat_views_data() {

  //heartbeat messages
  $data['heartbeat_messages'] = array(
      // Table to join
      'table' => array(
        'group' => t('Heartbeat messages'),
      ),
      'message_id' => array(
        'title' => t('Heartbeat message id'),
        'help' => t('Message Id of the hearbeat message'),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
      ),
      'message' => array(
        'title' => t('Heartbeat message'),
        'help' => t('Hearbeat message'),
      ),
      'hid' => array(
        'title' => t('heartbeat message id'),
        'help' => t('Numeric heartbeat message id'),
      ),
      'perms' => array(
        'title' => t('Permissions'),
        'help' => t('Permissions of the message'),
        'filter' => array(
          'handler' => 'views_handler_filter_heartbeat_perms',
        ),
      ),
  );

  // Heartbeat activity table
  $data['heartbeat_activity'] = array(

      // Table to join
      'table' => array(

        'group' => t('Heartbeat activity'),

        'base'  => array(
          'field' => 'message_id',
          'title' => t('Heartbeat activity messages'),
          'help'  => t("All activity logged by heartbeat"),
        ),
      ),

      // Fields to use or as filter, arguments, relationships ...
      'uid' => array(
        'title' => t('Message owner'),
        'help' => t('The owner of the heartbeat message.'),
        'relationship' => array(
          'label' => t('Heartbeat message owner'),
          'base' => 'users',
          'base field' => 'uid',
          'left_field' => 'uid',
          'field' => 'uid',
        ),
      ),
      'message_id' => array(
        'title' => t('Heartbeat message id'),
        'help' => t('The id of the hearbeat message.'),
        'relationship' => array(
          'label' => t('Activity message id'),
          'base' => 'heartbeat_messages',
          'base field' => 'message_id',
          'left_field' => 'message_id',
          'field' => 'message_id',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_string',
        ),
        'field' => array(
          'handler' => 'views_handler_field_markup',
          'format' => FILTER_FORMAT_DEFAULT,
        ),
      ),
      'uid_current' => array(
        'real field' => 'uid',
        'title' => t('The user who\'s activity is beeing displayed'),
        'help' => t('Sets the displayed user to the currently logged in user or by uid in url'),
        'filter' => array(
          'handler' => 'views_handler_filter_user_current',
        ),
      ),
      'uid_requested' => array(
        'real field' => 'uid',
        'title' => t('Currently requested user (url argument) carried out the activity'),
        'help' => t('The user who did the activity is third argument in url heartbeat/user/% '),
        'argument' => array(
          'handler' => 'views_handler_argument_user_uid',
        ),
      ),
      'message' => array(
        'title' => t('Message'),
        'help' => t('Logged Message'),
        'field' => array(
          'handler' => 'views_handler_field_markup',
          'format' => FILTER_FORMAT_DEFAULT,
        ),
      ),
      // Language field
      'language' => array(
        'title' => t('Display language is current language(usertime)'),
        'help' => t('The language the content is in.'),
        'filter' => array(
          'handler' => 'views_handler_filter_heartbeat_language',
        ),
      ),
      'timestamp' => array(
        'title' => t('Action\'s timestamp'),
        'help' => t('When the action happened'),
        'field' => array(
          'handler' => 'views_handler_field_date',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_date',
        ),
      ),
      'access_type' => array(
        'real field' => 'uid',
        'title' => t('Heartbeat user access'),
        'help' => t('Restict with access types'),
        'filter' => array(
          'handler' => 'views_handler_filter_heartbeat_access',
        ),
      ),

      'nid' => array(
        'title' => t('Node'),
        'help' => t('The node where the activity occurred.'),
        'argument' => array(
          'handler' => 'views_handler_argument_node_nid',
        ),
        'relationship' => array(
          'label' => t('Message node id'),
          'base' => 'node',
          'base field' => 'nid',
          'left_field' => 'nid',
          'field' => 'nid',
        ),
      ),

      'nid_target' => array(
        'title' => t('Node Target'),
        'help' => t('The target node the activity occured.'),
        'argument' => array(
          'handler' => 'views_handler_argument_node_nid',
        ),
        'relationship' => array(
          'label' => t('Message node target id'),
          'base' => 'node',
          'base field' => 'nid',
          'left_field' => 'nid_target',
          'field' => 'nid_target',
        ),
      ),

  );
  if (module_exists('og_views')) {
    $data['heartbeat_activity']['og_rel'] = array(
      'group' => t('Heartbeat'),
      'title' => t('Organic group'),
      'help' => t('Create a relationship to nodes organic groups.'),
      'relationship' => array(
        'relationship table' => 'heartbeat_activity',
        'relationship field' => 'nid_target',
        'handler' => 'views_handler_relationship',
        'base' => 'og_ancestry',
        'base field' => 'nid',
        'label' => t('Group'),
      ),
    );
  }

  return $data;
}

/**
 * Implementation of hook_views_query_alter().
 */
function heartbeat_views_views_query_alter(&$view, &$query) {

  // Add all necessairy fields when activity is loaded
  if ($view->base_table == 'heartbeat_activity') {

    foreach (_heartbeat_get_fields('heartbeat_activity') as $field => $alias) {
      $query->add_field('heartbeat_activity', $field, $field);
    }

    // If relation is set with heartbeat_messages, add all fields for that table as well
    if (!empty($view->relationship['message_id'])) {
      foreach (_heartbeat_get_fields('heartbeat_messages') as $field => $alias) {
        $query->add_field($view->relationship['message_id']->alias, $field, $alias);
      }
      $query->add_field($view->relationship['message_id']->alias, 'variables', 'variables_orig');
      $query->add_field($view->relationship['message_id']->alias, 'message', 'message_orig');
      $query->add_field($view->relationship['message_id']->alias, 'message_concat', 'message_concat_orig');
    }
    else {
      drupal_set_message(t('Heartbeat views need to be joined over the heartbeat_messages table.'), 'error');
    }

  }

  $view->pager['_items_per_page'] = $view->pager['items_per_page'];
}

/**
 * Implementation of hook_views_handlers().
 *
 * @return unknown
 */
function heartbeat_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'heartbeat') . '/views/handlers',
    ),
    'handlers' => array(

      // Handler to fetch message for current language
      'views_handler_filter_heartbeat_language' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),

      // Handler to fetch message for only this permissions
      'views_handler_filter_heartbeat_perms' => array(
        'parent' => 'views_handler_filter_equality',
      ),

      // Handler to handle the access state of the messages
      'views_handler_filter_heartbeat_access' => array(
        'parent' => 'views_handler_filter_numeric',
      ),

      // TODO what does this have to do really?
      'views_handler_filter_heartbeat_user_requested' => array(
        'parent' => 'views_handler_filter_boolean_operator',
      ),
    ),
  );
}


