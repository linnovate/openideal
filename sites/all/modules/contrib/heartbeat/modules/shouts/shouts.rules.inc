<?php
// $Id: shouts.rules.inc,v 1.1.2.2 2010/02/21 12:31:33 stalski Exp $

/**
 * @file
 *   shouts.rules.inc.
 */


/**
 * Implementation of hook_rules_event_info().
 * @ingroup rules
 */
function shouts_rules_event_info() {
  return array(
    'shout_post' => array(
      'label' => t('User posts a new shout'),
      'module' => 'Shouts',
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('User who posts a new shout.')),
        'shout' => array('type' => 'shout', 'label' => t('Shout that has been posted.')),
      ),
      'redirect' => TRUE,
    ),
  );
}

/**
 * Implementation of hook_rules_data_type_info().
 */
function shouts_rules_data_type_info() {
  return array(
    'shout' => array(
      'label' => t('shout'),
      'class' => 'rules_data_type_shout',
      'savable' => FALSE,
      'identifiable' => TRUE,
      'module' => 'shouts',
    ),
  );
}

/**
 * Defines the shout data type.
 */
class rules_data_type_shout extends rules_data_type {
  function load($shout_id) {
    return shout_load($shout_id);
  }

  function get_identifier() {
    return $this->get()->shout_id;
  }
}