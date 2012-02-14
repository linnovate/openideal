<?php
// $Id: friendlist_activity.rules_defaults.inc,v 1.1.2.2.2.14 2010/04/05 23:18:42 stalski Exp $

/**
 * @file
 * Rules' pre-configured rules.
 */

/**
 * Implementation of hook_rules_defaults();
 *
 * @return array rules structure
 */
function friendlist_activity_rules_defaults() {

  $rules = array('rules' => array());

  if (module_exists('friendlist_rules')) {

    $rules['rules'] += array(

      'rule_heartbeat_friendlist_add' =>
      array(
        '#type' => 'rule',
        '#categories' =>
        array(
          0 => 'friendlist_activity',
          1 => 'heartbeat',
        ),
        '#set' => 'event_friendlist_add',
        '#label' => 'Heartbeat - user has one way relation to another user',
        '#active' => 0,
        '#weight' => '0',
        '#status' => 'default',
        '#conditions' =>
        array(
          0 =>
          array(
            '#weight' => 0,
            '#info' =>
            array(
              'label' => 'Compare with a status.',
              'arguments' =>
              array(
                'relation_status' =>
                array(
                  'label' => 'Relation status',
                  'type' => 'relation_status',
                  'description' => 'The relation to compare',
                ),
              ),
              'module' => 'Friendlist Rules',
            ),
            '#name' => 'rules_condition_relation_status_compare',
            '#settings' =>
            array(
              'status_param' => 'OW_1_TO_2',
              '#argument map' =>
              array(
                'relation_status' => 'status_post',
              ),
            ),
            '#type' => 'condition',
          ),
        ),
        '#actions' =>
        array(
          0 =>
          array(
            '#info' =>
            array(
              'label' => 'Logs activity for a single user to a static user',
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
              'uid_param' => '[user1:uid]',
              'uid_target_param' => '[user2:uid]',
              'nid_param' => '0',
              'nid_target_param' => '0',
              'message_id_param' => 'heartbeat_add_FL_OW_relation',
              'variables_param' => '@user1=|=[user1:user-name-url]-|-@relation=|=[rtid:name]-|-@user2=|=[user2:user-name-url]-|-',
              '#eval input' =>
              array(
                'token_rules_input_evaluator' =>
                array(
                  'uid_param' =>
                  array(
                    0 => 'user1',
                  ),
                  'uid_target_param' =>
                  array(
                    0 => 'user2',
                  ),
                  'variables_param' =>
                  array(
                    0 => 'user1',
                    1 => 'user2',
                    2 => 'rtid',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
            '#weight' => 0,
          ),
        ),
        '#version' => 6003,
      ),

      'rule_heartbeat_friendlist_accepted' =>
      array(
        '#type' => 'rule',
        '#categories' =>
        array(
          0 => 'friendlist_activity',
          1 => 'heartbeat',
        ),
        '#set' => 'event_friendlist_add',
        '#label' => 'Heartbeat - users accepted relation with another user',
        '#active' => 1,
        '#weight' => '3',
        '#status' => 'default',
        '#conditions' =>
        array(
          1 =>
          array(
            '#weight' => 0,
            '#settings' =>
            array(
              'status_param' => 'TW_BOTH',
              '#argument map' =>
              array(
                'relation_status' => 'status_post',
              ),
            ),
            '#info' =>
            array(
              'label' => 'Request has been accepted',
              'label callback' => FALSE,
              'arguments' =>
              array(
                'relation_status' =>
                array(
                  'label' => 'Relation status',
                  'type' => 'relation_status',
                  'description' => 'The relation to compare',
                ),
              ),
              'help' => 'TRUE is returned if the relation status equals the string in the input box',
              'module' => 'Friendlist Rules',
            ),
            '#name' => 'rules_condition_relation_status_compare',
            '#type' => 'condition',
          ),
        ),
        '#actions' =>
        array(
          0 =>
          array(
            '#weight' => 0,
            '#info' =>
            array(
              'label' => 'Logs relational activity for two users (1->2)',
              'label callback' => FALSE,
              'eval input' =>
              array(
                0 => 'uid_param',
                1 => 'uid_target_param',
                2 => 'message_id_param',
                3 => 'variables_param',
              ),
              'module' => 'Friendlist Rules',
            ),
            '#name' => 'heartbeat_rules_users_action',
            '#settings' =>
            array(
              'uid_param' => '[user1:uid]',
              'uid_target_param' => '[user2:uid]',
              'message_id_param' => 'heartbeat_become_friends',
              'variables_param' => '@user1=|=[user1:user-name-url]-|-#relation_type=|=[rtid:name_p]-|-@user2=|=[user2:user-name-url]-|-',
              '#eval input' =>
              array(
                'token_rules_input_evaluator' =>
                array(
                  'uid_param' =>
                  array(
                    0 => 'user1',
                  ),
                  'uid_target_param' =>
                  array(
                    0 => 'user2',
                  ),
                  'variables_param' =>
                  array(
                    0 => 'user1',
                    1 => 'user2',
                    2 => 'rtid',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),

          1 =>
          array(
            '#weight' => 0,
            '#info' =>
            array(
              'label' => 'Logs relational activity for two users (2->1)',
              'label callback' => FALSE,
              'eval input' =>
              array(
                0 => 'uid_param',
                1 => 'uid_target_param',
                2 => 'message_id_param',
                3 => 'variables_param',
              ),
              'module' => 'Friendlist Rules',
            ),
            '#name' => 'heartbeat_rules_users_action',
            '#settings' =>
            array(
              'uid_param' => '[user2:uid]',
              'uid_target_param' => '[user1:uid]',
              'message_id_param' => 'heartbeat_become_friends',
              'variables_param' => '@user1=|=[user2:user-name-url]-|-#relation_type=|=[rtid:name_p]-|-@user2=|=[user1:user-name-url]-|-',
              '#eval input' =>
              array(
                'token_rules_input_evaluator' =>
                array(
                  'uid_param' =>
                  array(
                    0 => 'user2',
                  ),
                  'uid_target_param' =>
                  array(
                    0 => 'user1',
                  ),
                  'variables_param' =>
                  array(
                    0 => 'user2',
                    1 => 'user1',
                    2 => 'rtid',
                  ),
                ),
              ),
            ),
            '#type' => 'action',
          ),


        ),

        '#version' => 6003,
      ),

    );

  }// end friendlist module

  return $rules;
}