<?php
/**
 * @file
 * fe_default.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function fe_default_default_rules_configuration() {
  $items = array();
  $items['rules_user_get_flag_on_userpoints'] = entity_import('rules_config', '{ "rules_user_get_flag_on_userpoints" : {
      "LABEL" : "User Get Flag On Userpoints",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Userpoints" ],
      "REQUIRES" : [ "flag", "rules" ],
      "ON" : [ "node_view" ],
      "DO" : [
        { "flag_flaguser" : {
            "flag" : "extreme_user",
            "user" : [ "site:current-user" ],
            "flagging_user" : [ "site:current-user" ],
            "permission_check" : 0
          }
        }
      ]
    }
  }');
  return $items;
}
