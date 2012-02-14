<?php
// $Id: heartbeat_example.rules_defaults.inc,v 1.1.2.3 2010/02/21 22:45:26 stalski Exp $

/**
 * @file
 *   Default rules as example. They can be imported into production
 *   although this site is not ment to be enabled in production sites.
 */

/**
 * Implementation of hook_rules_defaults().
 *
 * @return configured rules array
 */
function heartbeat_example_rules_defaults() {
  return array(
    'rules' =>
    array(


      'heartbeat_custom_rules_1' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'When article is posted in a group',
        '#active' => 1,
        '#weight' => '0',
        '#categories' =>
        array(
          0 => 'heartbeat',
          1 => 'og_activity',
        ),
        '#status' => 'default',
        '#conditions' =>
        array(
          0 =>
          array(
            '#weight' => 0,
            '#info' =>
            array(
              'label' => 'Created content is published',
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
            '#name' => 'rules_condition_content_is_published',
            '#settings' =>
            array(
              '#argument map' =>
              array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),
          1 =>
          array(
            '#type' => 'condition',
            '#settings' =>
            array(
              '#argument map' =>
              array(
                'group' => 'node',
              ),
            ),
            '#name' => 'og_rules_condition_content_is_group_post',
            '#info' =>
            array(
              'label' => 'Content is a group post',
              'arguments' =>
              array(
                'group' =>
                array(
                  'type' => 'node',
                  'label' => 'Group post',
                ),
              ),
              'module' => 'Organic groups',
            ),
            '#weight' => 0,
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
              'nid_target_param' => '[node:og-id]',
              'message_id_param' => 'og_add_group_content',
              'variables_param' => '@user=|=[author:user-name-url]-|-@article=|=[node:title-link]-|-@group=|=[node:ogname-title-link]-|-',
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
                  'nid_target_param' =>
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
              'label' => 'Logs user activity when group post is created',
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



      'heartbeat_custom_rules_2' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_node_update',
        '#label' => 'When a group article is updated',
        '#active' => 1,
        '#weight' => '0',
        '#categories' =>
        array(
          0 => 'heartbeat',
          1 => 'og_activity',
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
                'group' => 'node',
              ),
            ),
            '#name' => 'og_rules_condition_content_is_group_post',
            '#info' =>
            array(
              'label' => 'Content is a group post',
              'arguments' =>
              array(
                'group' =>
                array(
                  'type' => 'node',
                  'label' => 'Group post',
                ),
              ),
              'module' => 'Organic groups',
            ),
            '#weight' => 0,
          ),
          1 =>
          array(
            '#weight' => 0,
            '#info' =>
            array(
              'label' => 'Updated content is published',
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
            '#name' => 'rules_condition_content_is_published',
            '#settings' =>
            array(
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
              'nid_target_param' => '[node:og-id]',
              'message_id_param' => 'og_edit_group_content',
              'variables_param' => '@user=|=[author:user-name-url]-|-@article=|=[node:title-link]-|-@group=|=[node:ogname-title-link]-|-',
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
                  'nid_target_param' =>
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
              'label' => 'Logs activity when a group post is updated',
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



      'heartbeat_custom_rules_3' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_og_user_insert',
        '#label' => 'When user joins a group',
        '#active' => 1,
        '#weight' => '0',
        '#categories' =>
        array(
          0 => 'og_activity',
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
            '#type' => 'action',
            '#settings' =>
            array(
              'uid_param' => '[account:uid]',
              'uid_target_param' => '0',
              'nid_param' => '[group:nid]',
              'nid_target_param' => '0',
              'message_id_param' => 'og_join_group',
              'variables_param' => '@user=|=[account:user-name-url]-|-@group=|=[group:title-link]-|-',
              '#eval input' =>
              array(
                'token_rules_input_evaluator' =>
                array(
                  'uid_param' =>
                  array(
                    0 => 'account',
                  ),
                  'nid_param' =>
                  array(
                    0 => 'group',
                  ),
                  'variables_param' =>
                  array(
                    0 => 'account',
                    1 => 'group',
                  ),
                ),
              ),
            ),
            '#name' => 'heartbeat_rules_default_action',
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
          ),
        ),
        '#version' => 6003,
      ),




      'heartbeat_custom_rules_4' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_node_insert',
        '#label' => 'When article is created',
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
            '#weight' => 0,
            '#info' =>
            array(
              'label' => 'Created content is published',
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
            '#name' => 'rules_condition_content_is_published',
            '#settings' =>
            array(
              '#argument map' =>
              array(
                'node' => 'node',
              ),
            ),
            '#type' => 'condition',
          ),

          1 =>
          array(
            '#type' => 'condition',
            '#settings' =>
            array(
              'type' =>
              array(
                'article' => 'article',
              ),
              '#argument map' =>
              array(
                'node' => 'node',
              ),
            ),
            '#name' => 'rules_condition_content_is_type',
            '#info' =>
            array(
              'label' => 'Content has type article',
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
              'nid_target_param' => '0',
              'message_id_param' => 'og_add_article_content',
              'variables_param' => '@user=|=[author:user-name-url]-|-@article=|=[node:title-link]-|-',
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
                  'nid_target_param' =>
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
              'label' => 'Logs user activity when article is created',
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


    ),
  );
}