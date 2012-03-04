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
  $items['rules_log_activity_for_a_new_node_being_published_oi'] = entity_import('rules_config', '{ "rules_log_activity_for_a_new_node_being_published_oi" : {
      "LABEL" : "Log activity for a new node being published",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "heartbeat_rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [ { "node_is_published" : { "node" : [ "node" ] } } ],
      "DO" : [
        { "heartbeat_activity_heartbeat_add_node_oi" : {
            "nid" : "[node:nid]",
            "uid" : "[node:author:uid]",
            "nid_target" : "0",
            "uid_target" : "0",
            "message_id" : { "value" : { "heartbeat_add_node_oi" : "heartbeat_add_node_oi" } },
            "!username" : "[node:author:link]",
            "!node_title" : "[node:link]",
            "!types" : "[node:type]s"
          }
        }
      ]
    }
  }');
  $items['rules_log_activity_when_a_node_is_commented_oi'] = entity_import('rules_config', '{ "rules_log_activity_when_a_node_is_commented_oi" : {
      "LABEL" : "Log activity when a node is commented",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Heartbeat" ],
      "REQUIRES" : [ "rules", "heartbeat_rules", "comment" ],
      "ON" : [ "comment_update", "comment_insert" ],
      "DO" : [
        { "heartbeat_activity_heartbeat_add_comment_oi" : {
            "nid" : "[comment:node:nid]",
            "uid" : "[comment:author:uid]",
            "nid_target" : "0",
            "uid_target" : "0",
            "message_id" : { "value" : { "heartbeat_add_comment_oi" : "heartbeat_add_comment_oi" } },
            "!username" : "[comment:author:link]",
            "!title" : "[comment:node:link]"
          }
        }
      ]
    }
  }');
  $items['rules_log_activity_when_a_node_is_voted'] = entity_import('rules_config', '{ "rules_log_activity_when_a_node_is_voted" : {
      "LABEL" : "Log activity when a node is voted",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Heartbeat" ],
      "REQUIRES" : [ "rules", "heartbeat_rules", "voting_rules" ],
      "ON" : [ "voting_rules_insert_node" ],
      "DO" : [
        { "heartbeat_activity_heartbeat_vote_on_idea" : {
            "nid" : [ "vote:vote-id" ],
            "uid" : "[vote:uid]",
            "nid_target" : [ "0" ],
            "uid_target" : [ "0" ],
            "message_id" : { "value" : { "heartbeat_vote_on_idea" : "heartbeat_vote_on_idea" } },
            "!username" : "[site:current-user:link]",
            "!node_title" : "[node:link]",
            "!types" : [ "node:type" ]
          }
        }
      ]
    }
  }');
  $items['rules_user_get_flag_on_userpoints'] = entity_import('rules_config', '{ "rules_user_get_flag_on_userpoints" : {
      "LABEL" : "User Get Flag On Userpoints",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Userpoints" ],
      "REQUIRES" : [ "ideal_main", "flag", "rules" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "userpoints_condition_check_points" : {
            "user" : [ "site:current-user" ],
            "tid" : "0",
            "threshold" : "3000",
            "comparison" : "ge"
          }
        }
      ],
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
