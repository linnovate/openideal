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
  $items['rules_close_commnting_hide_voting_widget_on_merge'] = entity_import('rules_config', '{ "rules_close_commnting_hide_voting_widget_on_merge" : {
      "LABEL" : "Close Commnting \\u0026 hide Voting widget on merge",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "flag", "rules" ],
      "ON" : [ "node_view" ],
      "IF" : [
        { "NOT flag_threshold_node" : {
            "flag" : "merged_idea",
            "node" : [ "node" ],
            "number" : "1",
            "operator" : "\\u003C"
          }
        }
      ],
      "DO" : [ { "component_rules_node_hide_voting" : { "node" : [ "node" ] } } ]
    }
  }');
  $items['rules_merged_to_merged_ideas'] = entity_import('rules_config', '{ "rules_merged_to_merged_ideas" : {
      "LABEL" : "Merged to merged ideas",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Merged Idea" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "flag_flagged_merged_idea" ],
      "DO" : [
        { "entity_query" : {
            "USING" : {
              "type" : "node",
              "property" : "field_merge_to",
              "value" : [ "flagged-node" ],
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
      "REQUIRES" : [ "php", "rules" ],
      "USES VARIABLES" : { "node" : { "label" : "Node", "type" : "node" } },
      "ACTION SET" : [
        { "php_eval" : { "code" : "\\/\\/ Remove the voting widget;\\r\\n$node-\\u003Efield_vote = NULL;\\r\\nnode_save($node);" } }
      ]
    }
  }');
  $items['rules_pass_radioactivity_to_master_idea'] = entity_import('rules_config', '{ "rules_pass_radioactivity_to_master_idea" : {
      "LABEL" : "Pass Radioactivity to Master idea",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Merged Idea" ],
      "REQUIRES" : [ "rules", "radioactivity", "flag" ],
      "ON" : [ "flag_flagged_merged_idea" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "flagged-node" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        },
        { "node_is_of_type" : {
            "node" : [ "flagged-node:field-merge-to" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        }
      ],
      "DO" : [
        { "radioactivity_maximum" : {
            "USING" : { "data" : [ "flagged-node:field-blast-idea" ] },
            "PROVIDE" : { "maximum_energy" : { "blast_energy" : "Blast Energy" } }
          }
        },
        { "radioactivity_maximum" : {
            "USING" : { "data" : [ "flagged-node:field-buzz-idea" ] },
            "PROVIDE" : { "maximum_energy" : { "buzz_energy" : "Buzz Energy" } }
          }
        },
        { "radioactivity_maximum" : {
            "USING" : { "data" : [ "flagged-node:field-groove-idea" ] },
            "PROVIDE" : { "maximum_energy" : { "groove_energy" : "Groove Energy" } }
          }
        },
        { "radioactivity_emit" : {
            "data" : [ "flagged-node:field-merge-to:field-blast-idea" ],
            "value" : [ "blast-energy" ]
          }
        },
        { "radioactivity_emit" : {
            "data" : [ "flagged-node:field-merge-to:field-buzz-idea" ],
            "value" : [ "buzz-energy" ]
          }
        },
        { "radioactivity_set" : {
            "data" : [ "flagged-node:field-merge-to:field-groove-idea" ],
            "value" : [ "groove-energy" ]
          }
        }
      ]
    }
  }');
  $items['rules_set_as_merged_idea'] = entity_import('rules_config', '{ "rules_set_as_merged_idea" : {
      "LABEL" : "Set as merged idea",
      "PLUGIN" : "reaction rule",
      "TAGS" : [ "Merged Idea" ],
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "node_presave" ],
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
        { "flag_flagnode" : {
            "flag" : "merged_idea",
            "node" : [ "node" ],
            "flagging_user" : "1",
            "permission_check" : 0
          }
        }
      ]
    }
  }');
  return $items;
}
