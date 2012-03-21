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
  $items['rules_oi_log_activity_when_a_node_is_commented'] = entity_import('rules_config', '{ "rules_oi_log_activity_when_a_node_is_commented" : {
      "LABEL" : "OI Log activity when a node is commented",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Heartbeat" ],
      "REQUIRES" : [ "rules", "heartbeat_rules", "comment" ],
      "ON" : [ "comment_update", "comment_insert" ],
      "DO" : [
        { "heartbeat_activity_oi_add_comment" : {
            "nid" : "[comment:node:nid]",
            "uid" : "[comment:author:uid]",
            "nid_target" : "0",
            "uid_target" : "0",
            "message_id" : { "value" : { "oi_add_comment" : "oi_add_comment" } },
            "!username" : "[comment:author:link]",
            "!node_title" : "[comment:node:link]",
          }
        }
      ]
    }
  }');
  $items['rules_oi_log_activity_when_a_node_is_voted'] = entity_import('rules_config', '{ "rules_oi_log_activity_when_a_node_is_voted" : {
      "LABEL" : "OI Log activity when a node is voted",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Heartbeat" ],
      "REQUIRES" : [ "rules", "heartbeat_rules", "voting_rules" ],
      "ON" : [ "voting_rules_insert_node" ],
      "DO" : [
        { "heartbeat_activity_oi_vote_on_node" : {
            "nid" : [ "vote:vote-id" ],
            "uid" : "[vote:uid]",
            "nid_target" : [ "0" ],
            "uid_target" : [ "0" ],
            "message_id" : { "value" : { "oi_vote_on_node" : "oi_vote_on_node" } },
            "!node_title" : "[node:link]",
            "!username" : "[vote:user:link]",
            "!types" : [ "node:type" ]
          }
        }
      ]
    }
  }');
  $items['rules_oi_log_activity_when_a_node_is_voted_by_other'] = entity_import('rules_config', '{ "rules_oi_log_activity_when_a_node_is_voted_by_other" : {
      "LABEL" : "OI Log activity when a node is voted by other",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Heartbeat" ],
      "REQUIRES" : [ "rules", "heartbeat_rules", "voting_rules" ],
      "ON" : [ "voting_rules_insert_node" ],
      "IF" : [
        { "NOT data_is" : { "data" : [ "vote:user" ], "value" : [ "node:author" ] } }
      ],
      "DO" : [
        { "heartbeat_activity_oi_vote_on_my_content" : {
            "nid" : [ "vote:vote-id" ],
            "uid" : [ "node:author:uid" ],
            "nid_target" : "0",
            "uid_target" : "0",
            "message_id" : { "value" : { "oi_vote_on_my_content" : "oi_vote_on_my_content" } },
            "!otherusername" : "[vote:user:link]",
            "!node_title" : "[node:link]"
          }
        }
      ]
    }
  }');
  $items['rules_oi_log_activity_when_a_node_is_commented_by_other'] = entity_import('rules_config', '{ "rules_oi_log_activity_when_a_node_is_commented_by_other" : {
      "LABEL" : "OI Log activity when a node is commented by other",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Heartbeat" ],
      "REQUIRES" : [ "rules", "heartbeat_rules", "comment" ],
      "ON" : [ "comment_update", "comment_insert" ],
      "IF" : [
        { "NOT data_is" : { "data" : [ "comment:node:author" ], "value" : [ "comment:author" ] } }
      ],
      "DO" : [
        { "heartbeat_activity_oi_comment_on_my_content" : {
            "nid" : "[comment:node:nid]",
            "uid" : "[comment:author:uid]",
            "nid_target" : "0",
            "uid_target" : "0",
            "message_id" : { "value" : { "oi_comment_on_my_content" : "oi_comment_on_my_content" } },
            "!otherusername" : "[comment:author:link]",
            "!node_title" : "[comment:node:link]"
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
