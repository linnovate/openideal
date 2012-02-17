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
      "TAGS" : [ "radioactivity" ],
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
  return $items;
}
