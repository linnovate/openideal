<?php
// $Id: logintoboggan_rules.rules.inc,v 1.1.2.1 2010/03/25 20:03:15 thehunmonkgroup Exp $

/**
 * Implement hook_rules_event_info().
 * @ingroup rules
 */

function logintoboggan_rules_rules_event_info() {
  return array(
    'logintoboggan_validated' => array(
      'label' => t("When the user account is validated."),
      'module' => 'LoginToboggan',
      'arguments' => array(
        'account' => array(
          'type' => 'user', 
          'label' => t("The validated user's account"),
        ),
      ),
    ),
  );
}
