<?php
/**
 * @file
 * fe_user.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function fe_user_default_rules_configuration() {
  $items = array();
  $items['rules_bronze_user'] = entity_import('rules_config', '{ "rules_bronze_user" : {
      "LABEL" : "Bronze User",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules", "flag", "userpoints_rules" ],
      "ON" : [ "userpoints_event_points_awarded_after" ],
      "IF" : [
        { "data_is" : {
            "data" : [ "userpoints-transaction:points" ],
            "op" : "\\u003E",
            "value" : "1000"
          }
        }
      ],
      "DO" : [
        { "flag_flaguser" : {
            "flag" : "bronze_user",
            "user" : [ "userpoints-transaction:user" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        }
      ]
    }
  }');
  $items['rules_gold_user'] = entity_import('rules_config', '{ "rules_gold_user" : {
      "LABEL" : "Gold User",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules", "flag", "userpoints_rules" ],
      "ON" : [ "userpoints_event_points_awarded_after" ],
      "IF" : [
        { "data_is" : {
            "data" : [ "userpoints-transaction:points" ],
            "op" : "\\u003E",
            "value" : "5000"
          }
        }
      ],
      "DO" : [
        { "flag_flaguser" : {
            "flag" : "gold_user",
            "user" : [ "userpoints-transaction:user" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        }
      ]
    }
  }');
  $items['rules_silver_user'] = entity_import('rules_config', '{ "rules_silver_user" : {
      "LABEL" : "Silver User",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "User" ],
      "REQUIRES" : [ "rules", "flag", "userpoints_rules" ],
      "ON" : [ "userpoints_event_points_awarded_after" ],
      "IF" : [
        { "data_is" : {
            "data" : [ "userpoints-transaction:points" ],
            "op" : "\\u003E",
            "value" : "2000"
          }
        }
      ],
      "DO" : [
        { "flag_flaguser" : {
            "flag" : "silver_user",
            "user" : [ "userpoints-transaction:user" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        }
      ]
    }
  }');
  return $items;
}
