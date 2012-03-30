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
  $items['rules_merged_to_merged_to'] = entity_import('rules_config', '{ "rules_merged_to_merged_to" : {
      "LABEL" : "merged to merged to",
      "PLUGIN" : "rule",
      "REQUIRES" : [ "rules" ],
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
        { "data_set" : {
            "data" : [ "node:field-merge-to" ],
            "value" : [ "node:field-merge-to:field-merge-to" ]
          }
        }
      ]
    }
  }');
  $items['rules_repoint_merged_ideas'] = entity_import('rules_config', '{ "rules_repoint_merged_ideas" : {
      "LABEL" : "Repoint merged ideas",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "views_bulk_operations", "rules", "flag" ],
      "ON" : [ "flag_flagged_merged_idea" ],
      "IF" : [
        { "views_bulk_operations_condition_result_count" : {
            "view" : "idea_merged_ideas|default",
            "args" : [ "flagged-node:nid" ],
            "minimum" : "1"
          }
        },
        { "node_is_of_type" : {
            "node" : [ "flagged-node" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        }
      ],
      "DO" : [
        { "views_bulk_operations_action_load_list" : {
            "USING" : { "view" : "idea_merged_ideas|default", "args" : [ "flagged-node:nid" ] },
            "PROVIDE" : { "entity_list" : { "merged_ideas_list" : "A list of merged ideas" } }
          }
        },
        { "LOOP" : {
            "USING" : { "list" : [ "merged_ideas_list" ] },
            "ITEM" : { "list_item" : "Current list item" },
            "DO" : [
              { "component_rules_merged_to_merged_to" : { "node" : [ "list-item" ] } }
            ]
          }
        }
      ]
    }
  }');
  $items['rules_set_as_merged_idea'] = entity_import('rules_config', '{ "rules_set_as_merged_idea" : {
      "LABEL" : "Set as merged idea",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "node_presave" ],
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "data_is_empty" : { "data" : [ "node-unchanged:field-merge-to" ] } },
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
