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
  $items['rules_idea_become_popular'] = entity_import('rules_config', '{ "rules_idea_become_popular" : {
      "LABEL" : "Idea_Become_Popular",
      "PLUGIN" : "reaction rule",
      "ACTIVE" : false,
      "TAGS" : [ "Popular" ],
      "REQUIRES" : [ "rules", "voting_rules", "ideal_main", "flag" ],
      "ON" : [ "flag_flagged_popular_idea" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "flagged-node" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        },
        { "AND" : [
            { "entity_has_field" : { "entity" : [ "flagged-node" ], "field" : "field_in_action" } },
            { "data_is_empty" : { "data" : [ "flagged-node:field-in-action" ] } }
          ]
        },
        { "AND" : [
            { "entity_has_field" : { "entity" : [ "flagged-node" ], "field" : "field_project_ref" } },
            { "data_is_empty" : { "data" : [ "flagged-node:field-project-ref" ] } }
          ]
        },
        { "voting_rules_condition_check_vote_value" : {
            "vote" : [ "flagged-node:votes:0" ],
            "operator" : "\\u003E",
            "value" : "-1"
          }
        }
      ],
      "DO" : [
        { "entity_create" : {
            "USING" : {
              "type" : "node",
              "param_type" : "project",
              "param_title" : [ "flagged-node:title" ],
              "param_author" : "1"
            },
            "PROVIDE" : { "entity_created" : { "entity_created" : "Created entity" } }
          }
        },
        { "ideal_action_close_comments" : { "node" : [ "flagged-node" ] } },
        { "data_set" : { "data" : [ "flagged-node:field-in-action" ], "value" : "Under Review" } },
        { "data_set" : {
            "data" : [ "flagged-node:field-project-ref" ],
            "value" : [ "entity-created" ]
          }
        },
        { "data_set" : {
            "data" : [ "entity-created:field-in-action" ],
            "value" : "Under Review"
          }
        },
        { "variable_add" : {
            "USING" : { "type" : "vote_results", "value" : [ "flagged-node:vote-results" ] },
            "PROVIDE" : { "variable_added" : { "flagged_node_resulet" : "Flagged node resulet" } }
          }
        },
        { "ideal_action_set_content_vote" : { "node" : [ "entity-created" ], "vote" : [ "flagged-node-resulet:sum" ] } }
      ]
    }
  }');
  $items['rules_idea_get_flag_on_radioactivity_'] = entity_import('rules_config', '{ "rules_idea_get_flag_on_radioactivity_" : {
      "LABEL" : "Idea Get Flag On Radioactivity ",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Radioactivity" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_radioactivity" } },
        { "data_is" : {
            "data" : [ "node:field-radioactivity" ],
            "op" : "\\u003E",
            "value" : "3000"
          }
        }
      ],
      "DO" : [
        { "flag_flagnode" : {
            "flag" : "popular_idea",
            "node" : [ "node" ],
            "flagging_user" : [ "node:author" ],
            "permission_check" : 0
          }
        },
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "5" : "5" } },
            "subject" : "The idea [node:title] become popular.\\u000D\\u000A",
            "message" : "The idea [node:title] become popular.\\u000D\\u000APlease check it out: [node:url]."
          }
        }
      ]
    }
  }');
  $items['rules_idea_got_new_comment'] = entity_import('rules_config', '{ "rules_idea_got_new_comment" : {
      "LABEL" : "Idea got new comment",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Radioactivity" ],
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
        { "radioactivity_maximum" : {
            "USING" : { "data" : [ "comment:node:field-radioactivity" ] },
            "PROVIDE" : { "maximum_energy" : { "maximum_energy" : "Maximum energy" } }
          }
        },
        { "radioactivity_emit" : {
            "data" : [ "comment:node:field-radioactivity" ],
            "value" : { "select" : "maximum-energy", "num_offset" : { "value" : "100" } }
          }
        }
      ]
    }
  }');
  $items['rules_idea_got_new_vote'] = entity_import('rules_config', '{ "rules_idea_got_new_vote" : {
      "LABEL" : "Idea got new vote",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Radioactivity" ],
      "REQUIRES" : [ "rules", "radioactivity", "voting_rules" ],
      "ON" : [ "voting_rules_insert_node" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } }
      ],
      "DO" : [
        { "radioactivity_maximum" : {
            "USING" : { "data" : [ "node:field-radioactivity" ] },
            "PROVIDE" : { "maximum_energy" : { "maximum_energy" : "Maximum energy" } }
          }
        },
        { "radioactivity_emit" : {
            "data" : [ "node:field-radioactivity" ],
            "value" : { "select" : "maximum-energy", "num_offset" : { "value" : "50" } }
          }
        }
      ]
    }
  }');
  $items['rules_user_add_comment'] = entity_import('rules_config', '{ "rules_user_add_comment" : {
      "LABEL" : "User add comment",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Userpoints" ],
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
      "TAGS" : [ "Userpoints" ],
      "REQUIRES" : [ "rules", "userpoints_rules" ],
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
        }
      ]
    }
  }');
  $items['rules_user_idea_in_action'] = entity_import('rules_config', '{ "rules_user_idea_in_action" : {
      "LABEL" : "User Idea In Action",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Userpoints" ],
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
        }
      ]
    }
  }');
  $items['rules_user_idea_launch'] = entity_import('rules_config', '{ "rules_user_idea_launch" : {
      "LABEL" : "User Idea Launch",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Userpoints" ],
      "REQUIRES" : [ "rules", "userpoints_rules" ],
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
        }
      ]
    }
  }');
  $items['rules_user_vote'] = entity_import('rules_config', '{ "rules_user_vote" : {
      "LABEL" : "User Vote",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Userpoints" ],
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
