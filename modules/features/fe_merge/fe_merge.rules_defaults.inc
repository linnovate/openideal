<?php
/**
 * @file
 * fe_merge.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function fe_merge_default_rules_configuration() {
  $items = array();
  $items['fe_idea_node_re_open_commenting'] = entity_import('rules_config', '{ "fe_idea_node_re_open_commenting" : {
      "LABEL" : "Node Re-open Commenting",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "openideal-new-merge-rules" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [ { "data_set" : { "data" : [ "node:comment" ], "value" : "2" } } ]
    }
  }');
  $items['rules_close_discussion_when_an_idea_is_merged'] = entity_import('rules_config', '{ "rules_close_discussion_when_an_idea_is_merged" : {
      "LABEL" : "Close discussion when an idea is merged",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "openideal-new-merge-rules" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_presave--idea" : { "bundle" : "idea" } },
      "IF" : [ { "NOT data_is_empty" : { "data" : [ "node:field-merge-to" ] } } ],
      "DO" : [ { "component_rules_node_close_commenting" : { "node" : [ "node" ] } } ]
    }
  }');
  $items['rules_merged_to_merged_ideas'] = entity_import('rules_config', '{ "rules_merged_to_merged_ideas" : {
      "LABEL" : "Merged to merged ideas",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "TAGS" : [ "Merged Idea" ],
      "REQUIRES" : [ "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [
        { "entity_query" : {
            "USING" : {
              "type" : "node",
              "property" : "field_merge_to",
              "value" : [ "node" ],
              "limit" : [ "" ]
            },
            "PROVIDE" : { "entity_fetched" : { "merged_ideas" : "Merged Ideas" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "merged-ideas" ] },
            "ITEM" : { "merged_idea" : "Current merged idea" },
            "DO" : [
              { "component_rules_merged_to_merged_to" : { "node" : [ "merged-idea" ] } }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_merged_to_merged_to'] = entity_import('rules_config', '{ "rules_merged_to_merged_to" : {
      "LABEL" : "merged to merged to",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "php" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "node_is_of_type" : {
            "node" : [ "node:field-merge-to" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        }
      ],
      "DO" : [
        { "php_eval" : { "code" : "\\/\\/ Set reference to the referenced - reference.\\r\\n$wrp = entity_metadata_wrapper(\\u0027node\\u0027, $node);\\r\\n$wrp-\\u003Efield_merge_to-\\u003Eset($wrp-\\u003Efield_merge_to-\\u003Efield_merge_to-\\u003Evalue());\\r\\n$wrp-\\u003Esave();" } }
      ]
    }
  }');
  $items['rules_node_hide_voting'] = entity_import('rules_config', '{ "rules_node_hide_voting" : {
      "LABEL" : "Node Hide Voting",
      "PLUGIN" : "action set",
      "OWNER" : "rules",
      "REQUIRES" : [ "php", "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [
        { "php_eval" : { "code" : "\\/\\/ Remove the voting widget;\\r\\n$node-\\u003Econtent[\\u0027field_vote\\u0027][0][\\u0027#markup\\u0027] = \\u0027\\u0027;" } }
      ]
    }
  }');
  $items['rules_pass_radioactivity_to_master_idea'] = entity_import('rules_config', '{ "rules_pass_radioactivity_to_master_idea" : {
      "LABEL" : "Pass Radioactivity to Master idea",
      "PLUGIN" : "rule",
      "OWNER" : "rules",
      "TAGS" : [ "Merged Idea" ],
      "REQUIRES" : [ "rules", "radioactivity" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "node_is_of_type" : {
            "node" : [ "node:field-merge-to" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        }
      ],
      "DO" : [
        { "radioactivity_maximum" : {
            "USING" : { "data" : [ "node:field-blast-idea" ] },
            "PROVIDE" : { "maximum_energy" : { "blast_energy" : "Blast Energy" } }
          }
        },
        { "radioactivity_maximum" : {
            "USING" : { "data" : [ "node:field-buzz-idea" ] },
            "PROVIDE" : { "maximum_energy" : { "buzz_energy" : "Buzz Energy" } }
          }
        },
        { "radioactivity_maximum" : {
            "USING" : { "data" : [ "node:field-groove-idea" ] },
            "PROVIDE" : { "maximum_energy" : { "groove_energy" : "Groove Energy" } }
          }
        },
        { "radioactivity_emit" : {
            "data" : [ "node:field-merge-to:field-blast-idea" ],
            "value" : [ "blast-energy" ]
          }
        },
        { "radioactivity_emit" : {
            "data" : [ "node:field-merge-to:field-buzz-idea" ],
            "value" : [ "buzz-energy" ]
          }
        },
        { "radioactivity_set" : {
            "data" : [ "node:field-merge-to:field-groove-idea" ],
            "value" : [ "groove-energy" ]
          }
        }
      ]
    }
  }');
  $items['rules_reopen_discussion_when_an_idea_is_un_merged'] = entity_import('rules_config', '{ "rules_reopen_discussion_when_an_idea_is_un_merged" : {
      "LABEL" : "Reopen discussion when an idea is un-merged",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "openideal-new-merge-rules" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_presave--idea" : { "bundle" : "idea" } },
      "IF" : [ { "data_is_empty" : { "data" : [ "node:field-merge-to" ] } } ],
      "DO" : [
        { "component_fe_idea_node_re_open_commenting" : { "node" : [ "node" ] } }
      ]
    }
  }');
  $items['rules_set_as_merged_idea'] = entity_import('rules_config', '{ "rules_set_as_merged_idea" : {
      "LABEL" : "Set as merged idea",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Merged Idea" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_presave" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "NOT data_is" : {
            "data" : [ "node-unchanged:field-merge-to" ],
            "value" : [ "node:field-merge-to" ]
          }
        },
        { "NOT data_is_empty" : { "data" : [ "node:field-merge-to" ] } }
      ],
      "DO" : [
        { "component_rules_merged_to_merged_ideas" : { "node" : [ "node" ] } },
        { "component_rules_pass_radioactivity_to_master_idea" : { "node" : [ "node" ] } }
      ]
    }
  }');
  return $items;
}
