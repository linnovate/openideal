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
        { "radioactivity_emit" : { "data" : [ "comment:node:field-blast-idea" ], "value" : "100" } },
        { "radioactivity_emit" : { "data" : [ "comment:node:field-buzz-idea" ], "value" : "100" } },
        { "radioactivity_emit" : { "data" : [ "comment:node:field-groove-idea" ], "value" : "100" } }
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
        { "radioactivity_emit" : { "data" : [ "node:field-blast-idea" ], "value" : "50" } },
        { "radioactivity_emit" : { "data" : [ "node:field-buzz-idea" ], "value" : "50" } },
        { "radioactivity_emit" : { "data" : [ "node:field-groove-idea" ], "value" : "50" } }
      ]
    }
  }');
  $items['rules_idea_is_blast'] = entity_import('rules_config', '{ "rules_idea_is_blast" : {
      "LABEL" : "Idea is Blast",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Radioactivity" ],
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
      "TAGS" : [ "Radioactivity" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_buzz_idea" } },
        { "data_is" : {
            "data" : [ "node:field-buzz-idea" ],
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
      "TAGS" : [ "Radioactivity" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "entity_has_field" : { "entity" : [ "node" ], "field" : "field_groove_idea" } },
        { "data_is" : {
            "data" : [ "node:field-groove-idea" ],
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
