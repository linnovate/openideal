<?php
/**
 * @file
 * fe_user_activity.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function fe_user_activity_default_rules_configuration() {
  $items = array();
  $items['rules_log_new_comment'] = entity_import('rules_config', '{ "rules_log_new_comment" : {
      "LABEL" : "Log new comment",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Message" ],
      "REQUIRES" : [ "rules", "comment" ],
      "ON" : [ "comment_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "comment:node" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "log_new_comment",
              "param_user" : [ "comment:author" ]
            },
            "PROVIDE" : { "entity_created" : { "new_comment_log" : "Log new comment" } }
          }
        },
        { "data_set" : {
            "data" : [ "new-comment-log:field-message-content" ],
            "value" : [ "comment:node" ]
          }
        },
        { "data_set" : {
            "data" : [ "new-comment-log:field-message-comment" ],
            "value" : [ "comment" ]
          }
        }
      ]
    }
  }');
  $items['rules_log_new_content'] = entity_import('rules_config', '{ "rules_log_new_content" : {
      "LABEL" : "Log new content",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Message" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "log_new_content",
              "param_user" : [ "node:author" ]
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Created entity" } }
          }
        },
        { "data_set" : {
            "data" : [ "entity-created:field-message-content" ],
            "value" : [ "node" ]
          }
        }
      ]
    }
  }');
  $items['rules_log_new_vote'] = entity_import('rules_config', '{ "rules_log_new_vote" : {
      "LABEL" : "Log new vote",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Message" ],
      "REQUIRES" : [ "rules", "voting_rules" ],
      "ON" : [ "voting_rules_insert_node" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "log_new_vote",
              "param_user" : [ "vote:user" ]
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Created entity" } }
          }
        },
        { "data_set" : {
            "data" : [ "entity-created:field-message-content" ],
            "value" : [ "node" ]
          }
        }
      ]
    }
  }');
  return $items;
}
