<?php
// $Id: hrules.rules_defaults.inc,v 1.1.2.2 2010/02/21 12:31:33 stalski Exp $

/**
 * @file
 *   Rules pre-configured rules for default drupal core behaviour.
 */

/**
 * Implementation of hook_rules_defaults().
 *
 * @return configured rules array
 */
function hrules_rules_defaults() {
  return array(
    'rules' =>
    array(


      'hrule_add_page' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'Heartbeat: when user adds a page, log user activity',
        '#active' => 1,
        '#weight' => '0',
        '#categories' =>
        array(
          0 => 'heartbeat',
        ),
        '#status' => 'default',
        '#conditions' =>
        array(
          0 =>
          array(
            '#type' => 'condition',
            '#settings' =>
            array(
              '#argument map' =>
              array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_published',
            '#info' =>
            array(
              'label' => 'page is published',
              'label callback' => FALSE,
              'arguments' =>
              array(
                'node' =>
                array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#weight' => 0,
          ),
          1 =>
          array(
            '#weight' => 0,
            '#info' =>
            array(
              'label' => 'Content has type page',
              'label callback' => FALSE,
              'arguments' =>
              array(
                'node' =>
                array(
                  'type' => 'node',
                  'label' => 'Content',
                ),
              ),
              'module' => 'Node',
            ),
            '#name' => 'rules_condition_content_is_type',
            '#settings' =>
            array(
              'type' =>
              array(
                'page' => 'page',
              ),
              '#argument map' =>
              array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' =>
        array(
          0 =>
          array(
            '#weight' => 0,
            '#type' => 'action',
            '#settings' =>
            array(
              'uid_param' => '[author:uid]',
              'uid_target_param' => '0',
              'nid_param' => '[node:nid]',
              'nid_target_param' => '',
              'message_id_param' => 'heartbeat_add_node',
              'variables_param' => '@username=|=[author:user-name-url]-|-@node_type=|=[node:type]-|-@node_title=|=[node:title-link]-|-@types=|=pages-|-',
              '#eval input' =>
              array(
                'token_rules_input_evaluator' =>
                array(
                  'uid_param' =>
                  array(
                    0 => 'author',
                  ),
                  'nid_param' =>
                  array(
                    0 => 'node',
                  ),
                  'variables_param' =>
                  array(
                    0 => 'node',
                    1 => 'author',
                  ),
                ),
              ),
            ),
            '#name' => 'heartbeat_rules_default_action',
            '#info' =>
            array(
              'label' => 'Logs activity for single users adding a page',
              'label callback' => FALSE,
              'eval input' =>
              array(
                0 => 'uid_param',
                1 => 'uid_target_param',
                2 => 'nid_param',
                3 => 'nid_target_param',
                4 => 'message_id_param',
                5 => 'variables_param',
              ),
              'module' => 'heartbeat',
            ),
          ),
        ),
        '#version' => 6003,
      ),


      'hrules_add_comment' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_comment_publish',
        '#label' => 'Heartbeat: When a user adds a comment',
        '#active' => 0,
        '#weight' => '0',
        '#categories' =>
        array(
          0 => 'heartbeat',
        ),
        '#status' => 'default',
        '#conditions' =>
        array(
        ),
        '#actions' =>
        array(
          0 =>
          array(
            '#weight' => 0,
            '#info' =>
            array(
              'label' => 'Logs user activity for single users',
              'eval input' =>
              array(
                0 => 'uid_param',
                1 => 'uid_target_param',
                2 => 'nid_param',
                3 => 'nid_target_param',
                4 => 'message_id_param',
                5 => 'variables_param',
              ),
              'module' => 'heartbeat',
            ),
            '#name' => 'heartbeat_rules_default_action',
            '#settings' =>
            array(
              'uid_param' => '[comment_author:uid]',
              'uid_target_param' => '[node_author:uid]',
              'nid_param' => '[node:nid]',
              'nid_target_param' => '0',
              'message_id_param' => 'heartbeat_add_comment',
              'variables_param' => '@username=|=[comment_author:user-name-url]-|-@title=|=[node:title-link]-|-@comment=|=[comment:comment-body-raw]-|-',
              '#eval input' =>
              array(
                'token_rules_input_evaluator' =>
                array(
                  'uid_param' =>
                  array(
                    0 => 'comment_author',
                  ),
                  'uid_target_param' =>
                  array(
                    0 => 'node_author',
                  ),
                  'nid_param' =>
                  array(
                    0 => 'node',
                  ),
                  'variables_param' =>
                  array(
                    0 => 'comment',
                    1 => 'comment_author',
                    2 => 'node',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),
        ),
        '#version' => 6003,
      ),

    ),
  );
}