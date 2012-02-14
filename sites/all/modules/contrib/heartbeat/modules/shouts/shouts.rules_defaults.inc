<?php
// $Id: shouts.rules_defaults.inc,v 1.1.2.2.2.6 2010/04/12 20:10:14 stalski Exp $

/**
 * @file
 *   Default rules for the shouts module
 */

/**
 * Implementation of hook_rules_defaults().
 *
 * @return configured rules array
 */
function shouts_rules_defaults() {
  return array(
    'rules' =>
    array(


      'shout_rule_0' =>
      array(
        '#type' => 'rule',
        '#categories' => array('heartbeat'),
        '#set' => 'event_shout_post',
        '#label' => 'heartbeat: After shouting, log shout message',
        '#active' => 1,
        '#weight' => '6',
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
              'uid_param' => '[user:uid]',
              'uid_target_param' => '0',
              'nid_param' => '',
              'nid_target_param' => '',
              'message_id_param' => 'heartbeat_shout',
              'variables_param' => '@user=|=[user:user-name-url]-|-@message=|=[shout:shout-body]-|-',
              '#eval input' =>
              array(
                'token_rules_input_evaluator' =>
                array(
                  'uid_param' =>
                  array(
                    0 => 'user',
                  ),
                  'variables_param' =>
                  array(
                    0 => 'user',
                    1 => 'shout',
                  ),
                ),
              ),
            ),
            '#name' => 'heartbeat_rules_default_action',
            '#info' =>
            array(
              'label' => 'Logs relational user activity',
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