<?php
// $Id: fbconnect_invite.rules.inc,v 1.1.2.1 2010/10/06 17:22:59 vectoroc Exp $

/**
 * @file rules integration for the fbconnect_invite module
 *
 * @addtogroup rules
 * @{
 */


/**
 * Implementation of hook_rules_event_info().
 */
function fbconnect_invite_rules_event_info() {
  return array(
    'fbconnect_invite_accepted' => array(
      'label' => t('Invitee User has accepted the invite'),
      'module' => 'fbconnect_invite',
      'arguments' => array(
        'inviter' => array('type' => 'user', 'label' => t('inviter')),
      ),
    ),
  );
}

/**
 * @}
 */
