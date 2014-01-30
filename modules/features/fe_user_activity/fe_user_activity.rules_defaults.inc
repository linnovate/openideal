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
  $items['rules_create_message_for_new_challenge'] = entity_import('rules_config', '{ "rules_create_message_for_new_challenge" : {
      "LABEL" : "Create message for new challenge",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Message" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "node" ],
            "type" : { "value" : { "challenge" : "challenge" } }
          }
        }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "new_challenge",
              "param_user" : [ "node:author" ]
            },
            "PROVIDE" : { "entity_created" : { "message" : "Message" } }
          }
        },
        { "data_set" : { "data" : [ "message:field-challenge" ], "value" : [ "node" ] } },
        { "entity_save" : { "data" : [ "message" ] } }
      ]
    }
  }');
  $items['rules_create_message_for_new_comment'] = entity_import('rules_config', '{ "rules_create_message_for_new_comment" : {
      "LABEL" : "Create message for new comment",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Message" ],
      "REQUIRES" : [ "rules", "comment" ],
      "ON" : [ "comment_insert" ],
      "IF" : [
        { "data_is" : {
            "data" : [ "comment:node:type" ],
            "op" : "IN",
            "value" : { "value" : { "challenge" : "challenge", "idea" : "idea", "news" : "news" } }
          }
        }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "new_comment",
              "param_user" : [ "comment:author" ]
            },
            "PROVIDE" : { "entity_created" : { "message" : "Message" } }
          }
        },
        { "data_set" : { "data" : [ "message:field-comment" ], "value" : [ "comment" ] } },
        { "entity_save" : { "data" : [ "message" ] } }
      ]
    }
  }');
  $items['rules_create_message_for_new_idea'] = entity_import('rules_config', '{ "rules_create_message_for_new_idea" : {
      "LABEL" : "Create message for new idea",
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
              "param_type" : "new_idea",
              "param_user" : [ "node:author" ]
            },
            "PROVIDE" : { "entity_created" : { "message" : "Message" } }
          }
        },
        { "data_set" : { "data" : [ "message:field-posted-idea" ], "value" : [ "node" ] } },
        { "data_set" : {
            "data" : [ "message:field-challenge" ],
            "value" : [ "node:field-challenge-ref" ]
          }
        },
        { "entity_save" : { "data" : [ "message" ] } }
      ]
    }
  }');
  $items['rules_create_message_for_new_user'] = entity_import('rules_config', '{ "rules_create_message_for_new_user" : {
      "LABEL" : "Create message for new user",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules" ],
      "ON" : [ "user_insert" ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "message",
              "param_type" : "new_user",
              "param_user" : [ "account" ]
            },
            "PROVIDE" : { "entity_created" : { "message" : "Message" } }
          }
        },
        { "entity_save" : { "data" : [ "message" ] } }
      ]
    }
  }');
  return $items;
}
