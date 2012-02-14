<?php
// $Id: og_activity.rules_defaults.inc,v 1.1.2.2.2.5 2010/02/21 22:45:26 stalski Exp $

/**
 * @file
 *   Default rules for organic group activity
 */

/**
 * Implementation of hook_rules_defaults().
 *
 * @return configured rules array
 */
function og_activity_rules_defaults() {

  return array(
    'rules' =>
    array(

      'og_activity_rules_1' =>
      array(
        '#type' => 'rule',
        '#set' => 'event_og_become_co_admin',
        '#categories' =>
        array(
          0 => 'og_activity',
          1 => 'heartbeat'
        ),
        '#label' => 'User becomes co-admin of a group',
        '#active' => 0,
        '#weight' => '0',
        '#status' => 'default',
        '#conditions' =>
        array(
        ),
        '#actions' =>
        array(
          0 =>
          array(
            '#weight' => 1,
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
              'uid_param' => '[user:uid]',
              'uid_target_param' => '0',
              'nid_param' => '[node:nid]',
              'nid_target_param' => '[node:og-id]',
              'message_id_param' => 'og_become_co_admin',
              'variables_param' => '@username=|=[user:user-name-url]-|-@group=|=[node:title-link]-|-',
              '#eval input' =>
              array(
                'token_rules_input_evaluator' =>
                array(
                  'uid_param' =>
                  array(
                    0 => 'user',
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
                    1 => 'user',
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
