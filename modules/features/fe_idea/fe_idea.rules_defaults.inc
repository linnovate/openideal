<?php
/**
 * @file
 * fe_idea.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function fe_idea_default_rules_configuration() {
  $items = array();
  $items['rules_close_idea in action'] = entity_import('rules_config', '{ "rules_close_idea in action" : {
      "LABEL" : "Close idea in action",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "In Action" ],
      "REQUIRES" : [ "rules" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "NOT data_is_empty" : { "data" : [ "node:field-in-action" ] } }
      ],
      "DO" : [
        { "component_rules_node_close_voting" : { "node" : [ "node" ] } },
        { "component_rules_node_close_commenting" : { "node" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_idea_got_new_comment'] = entity_import('rules_config', '{ "rules_idea_got_new_comment" : {
      "LABEL" : "Idea got new comment",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "Radioactivity" ],
      "REQUIRES" : [ "rules", "radioactivity", "comment" ],
      "ON" : [ "comment_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "comment:node" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        }
      ],
      "DO" : [
        { "radioactivity_emit" : { "data" : [ "comment:node:field-blast-idea" ], "value" : "100" } },
        { "radioactivity_emit" : { "data" : [ "comment:node:field-buzz-idea" ], "value" : "100" } },
        { "radioactivity_emit" : { "data" : [ "comment:node:field-groove-idea" ], "value" : "100" } }
      ]
    }
  }');
  $items['rules_idea_got_new_vote'] = entity_import('rules_config', '{ "rules_idea_got_new_vote" : {
      "LABEL" : "Idea got new vote",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "Radioactivity" ],
      "REQUIRES" : [ "rules", "radioactivity", "voting_rules" ],
      "ON" : [ "voting_rules_insert_node" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } }
      ],
      "DO" : [
        { "radioactivity_emit" : { "data" : [ "node:field-blast-idea" ], "value" : "50" } },
        { "radioactivity_emit" : { "data" : [ "node:field-buzz-idea" ], "value" : "50" } },
        { "radioactivity_emit" : { "data" : [ "node:field-groove-idea" ], "value" : "50" } }
      ]
    }
  }');
  $items['rules_idea_is_blast'] = entity_import('rules_config', '{ "rules_idea_is_blast" : {
      "LABEL" : "Idea is Blast",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "Radioactivity" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_blast_idea" } },
        { "data_is" : {
            "data" : [ "node:field-blast-idea" ],
            "op" : "\\u003E",
            "value" : "3000"
          }
        }
      ],
      "DO" : [
        { "flag_flagnode" : {
            "flag" : "blast_idea",
            "node" : [ "node" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "5" : "5" } },
            "subject" : "The idea [node:title] is a blast.",
            "message" : "The idea [node:title] is a blast.\\r\\nPlease check it out: [node:url]."
          }
        }
      ]
    }
  }');
  $items['rules_idea_is_buzz'] = entity_import('rules_config', '{ "rules_idea_is_buzz" : {
      "LABEL" : "Idea is Buzz",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "Radioactivity" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_buzz_idea" } },
        { "data_is" : {
            "data" : [ "node:field-blast-idea" ],
            "op" : "\\u003E",
            "value" : "3000"
          }
        }
      ],
      "DO" : [
        { "flag_flagnode" : {
            "flag" : "buzz_idea",
            "node" : [ "node" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "5" : "5" } },
            "subject" : "The idea [node:title] is a buzz.",
            "message" : "The idea [node:title] is a Buzz.\\r\\nPlease check it out: [node:url]."
          }
        }
      ]
    }
  }');
  $items['rules_idea_is_groove'] = entity_import('rules_config', '{ "rules_idea_is_groove" : {
      "LABEL" : "Idea is Groove",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "Radioactivity" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_blast_idea" } },
        { "data_is" : {
            "data" : [ "node:field-blast-idea" ],
            "op" : "\\u003E",
            "value" : "3000"
          }
        }
      ],
      "DO" : [
        { "flag_flagnode" : {
            "flag" : "groove_idea",
            "node" : [ "node" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "5" : "5" } },
            "subject" : "The idea [node:title] is a groove.",
            "message" : "The idea [node:title] is a Groove.\\r\\nPlease check it out: [node:url]."
          }
        }
      ]
    }
  }');
  $items['rules_node_close_commenting'] = entity_import('rules_config', '{ "rules_node_close_commenting" : {
      "LABEL" : "Node Close Commenting",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [ { "data_set" : { "data" : [ "node:comment" ], "value" : "1" } } ]
    }
  }');
  $items['rules_node_close_voting'] = entity_import('rules_config', '{ "rules_node_close_voting" : {
      "LABEL" : "Node Close Voting",
      "PLUGIN" : "action set",
      "REQUIRES" : [ "php", "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [
        { "php_eval" : { "code" : "$tag = variable_get(\\u0027vud_tag\\u0027, \\u0027vote\\u0027);\\r\\n$widget = variable_get(\\u0027vud_node_widget\\u0027, \\u0027updown\\u0027);\\r\\n    \\r\\n$theme_vars = array(\\r\\n  \\u0027entity_id\\u0027 =\\u003E $node-\\u003Enid,\\r\\n  \\u0027type\\u0027 =\\u003E \\u0027node\\u0027,\\r\\n  \\u0027widget_theme\\u0027 =\\u003E \\u0027alternate\\u0027,\\r\\n  \\u0027tag\\u0027 =\\u003E $tag,\\r\\n  \\u0027readonly\\u0027 =\\u003E TRUE\\r\\n);\\r\\n   \\r\\n$node-\\u003Econtent[\\u0027field_vote\\u0027][0][\\u0027#markup\\u0027] = theme(\\u0027vud_widget\\u0027, $theme_vars);" } }
      ]
    }
  }');
  $items['rules_user_add_comment'] = entity_import('rules_config', '{ "rules_user_add_comment" : {
      "LABEL" : "User add comment",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "Userpoints" ],
      "REQUIRES" : [ "rules", "userpoints_rules", "comment" ],
      "ON" : [ "comment_insert" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "comment:node" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        }
      ],
      "DO" : [
        { "userpoints_action_grant_points" : {
            "user" : [ "comment:author" ],
            "points" : "50",
            "tid" : "0",
            "entity" : [ "comment:author" ],
            "operation" : "Insert",
            "display" : 0,
            "moderate" : "default"
          }
        }
      ]
    }
  }');
  $items['rules_user_add_idea'] = entity_import('rules_config', '{ "rules_user_add_idea" : {
    "LABEL" : "User Add Idea",
    "PLUGIN" : "reaction rule",
    "TAGS" : [ "Idea", "Userpoints" ],
    "REQUIRES" : [ "rules", "userpoints_rules", "php" ],
    "ON" : [ "node_insert" ],
    "IF" : [
      { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } }
    ],
    "DO" : [
      { "userpoints_action_grant_points" : {
          "user" : [ "node:author" ],
          "points" : "100",
          "tid" : "0",
          "entity" : [ "node:author" ],
          "operation" : "Insert",
          "display" : 0,
          "moderate" : "default"
        }
      },
      { "php_eval" : { "code" : "$_SESSION[\u0027newnode\u0027] = 1;" } }
    ]
  }
}');
  $items['rules_user_idea_in_action'] = entity_import('rules_config', '{ "rules_user_idea_in_action" : {
      "LABEL" : "User Idea In Action",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "In Action", "Userpoints" ],
      "REQUIRES" : [ "rules", "userpoints_rules" ],
      "ON" : [ "node_update" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_in_action" } },
        { "data_is_empty" : { "data" : [ "node-unchanged:field-in-action" ] } },
        { "OR" : [
            { "data_is" : { "data" : [ "node:field-in-action" ], "value" : "Under Review" } },
            { "data_is" : { "data" : [ "node:field-in-action" ], "value" : "Reviewed" } },
            { "data_is" : { "data" : [ "node:field-in-action" ], "value" : "Coming Soon" } }
          ]
        }
      ],
      "DO" : [
        { "userpoints_action_grant_points" : {
            "user" : [ "node:author" ],
            "points" : "50",
            "tid" : "0",
            "entity" : [ "node:author" ],
            "operation" : "Insert",
            "display" : 0,
            "moderate" : "default"
          }
        },
        { "mail" : {
            "to" : [ "node:author:mail" ],
            "subject" : "Your idea [node:title] caught our eyes !!!",
            "message" : "Hello [node:author], \\r\\n\\r\\nYour idea [node:title] caught our eyes, and we started a review process.\\r\\n\\r\\n\\r\\nThank you,\\r\\n[site:name] team.",
            "from" : "[site:mail]",
            "language" : [ "" ]
          }
        },
        { "entity_query" : {
            "USING" : { "type" : "node", "property" : "field_merge_to", "value" : [ "node" ] },
            "PROVIDE" : { "entity_fetched" : { "merged_ideas" : "Merged Ideas" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "merged-ideas" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "userpoints_action_grant_points" : {
                  "user" : [ "list-item:author" ],
                  "points" : "50",
                  "tid" : "0",
                  "entity" : [ "list-item:author" ],
                  "operation" : "insert",
                  "display" : 0,
                  "moderate" : "default"
                }
              },
              { "mail" : {
                  "to" : [ "list-item:author:mail" ],
                  "subject" : "Your idea [node:title] caught our eyes !!!",
                  "message" : "Hello [node:author], \\r\\n\\r\\nYour idea [node:title] caught our eyes, and we started a review process.\\r\\n\\r\\n\\r\\nThank you,\\r\\n[site:name] team.",
                  "from" : "[site:mail]",
                  "language" : [ "" ]
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_user_idea_launch'] = entity_import('rules_config', '{ "rules_user_idea_launch" : {
      "LABEL" : "User Idea Launch",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "Userpoints" ],
      "REQUIRES" : [ "rules", "userpoints_rules", "flag" ],
      "ON" : [ "node_update" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_in_action" } },
        { "NOT data_is" : { "data" : [ "node-unchanged:field-in-action" ], "value" : "Launched" } },
        { "data_is" : { "data" : [ "node:field-in-action" ], "value" : "Launched" } }
      ],
      "DO" : [
        { "userpoints_action_grant_points" : {
            "user" : [ "node:author" ],
            "points" : "100",
            "tid" : "0",
            "entity" : [ "node:author" ],
            "operation" : "Insert",
            "display" : 0,
            "moderate" : "default"
          }
        },
        { "flag_flaguser" : {
            "flag" : "has_launched_idea",
            "user" : [ "node:author" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        },
        { "mail" : {
            "to" : [ "node:author:mail" ],
            "subject" : "Your idea [node:title] is LAUNCHED !!!",
            "message" : "Hello [node:author], \\r\\n\\r\\nWe Happy to tell you that we launched your idea [node:title].\\r\\n\\r\\nThank you for your contribution,\\r\\n[site:name] team.",
            "from" : "[site:mail]",
            "language" : [ "" ]
          }
        },
        { "entity_query" : {
            "USING" : { "type" : "node", "property" : "field_merge_to", "value" : [ "node" ] },
            "PROVIDE" : { "entity_fetched" : { "merged_ideas" : "Merged Ideas" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "merged-ideas" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "userpoints_action_grant_points" : {
                  "user" : [ "list-item:author" ],
                  "points" : "100",
                  "tid" : "0",
                  "entity" : [ "list-item:author" ],
                  "operation" : "insert",
                  "display" : 0,
                  "moderate" : "default"
                }
              },
              { "flag_flaguser" : {
                  "flag" : "has_launched_idea",
                  "user" : [ "list-item:author" ],
                  "flagging_user" : "1",
                  "permission_check" : 0
                }
              }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_user_vote'] = entity_import('rules_config', '{ "rules_user_vote" : {
      "LABEL" : "User Vote",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Idea", "Userpoints" ],
      "REQUIRES" : [ "rules", "userpoints_rules", "voting_rules" ],
      "ON" : [ "voting_rules_insert_node" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } }
      ],
      "DO" : [
        { "userpoints_action_grant_points" : {
            "user" : [ "vote:user" ],
            "points" : "10",
            "tid" : "0",
            "entity" : [ "vote:user" ],
            "operation" : "Insert",
            "display" : 1,
            "moderate" : "default"
          }
        }
      ]
    }
  }');
  return $items;
}
