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
  $items['rules_a_class_user'] = entity_import('rules_config', '{ "rules_a_class_user" : {
      "LABEL" : "A Class User",
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
            "flag" : "a_class_user",
            "user" : [ "userpoints-transaction:user" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        }
      ]
    }
  }');
  $items['rules_b_class_user'] = entity_import('rules_config', '{ "rules_b_class_user" : {
      "LABEL" : "B Class User",
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
            "flag" : "b_class_user",
            "user" : [ "userpoints-transaction:user" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        }
      ]
    }
  }');
  $items['rules_c_class_user'] = entity_import('rules_config', '{ "rules_c_class_user" : {
      "LABEL" : "C Class User",
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
            "flag" : "c_class_user",
            "user" : [ "userpoints-transaction:user" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        }
      ]
    }
  }');
  $items['rules_oi_log_activity_for_a_new_node_being_published'] = entity_import('rules_config', '{ "rules_oi_log_activity_for_a_new_node_being_published" : {
      "LABEL" : "OI Log activity for a new node being published",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "heartbeat_rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [ { "node_is_published" : { "node" : [ "node" ] } } ],
      "DO" : [
        { "heartbeat_activity_oi_add_node" : {
            "nid" : "[node:nid]",
            "uid" : "[node:author:uid]",
            "nid_target" : "0",
            "uid_target" : "0",
            "message_id" : { "value" : { "oi_add_node" : "oi_add_node" } },
            "!username" : "[node:author:link]",
            "!node_title" : "[node:link]",
            "!types" : "[node:type]s"
          }
        }
      ]
    }
  }');
  return $items;
}
