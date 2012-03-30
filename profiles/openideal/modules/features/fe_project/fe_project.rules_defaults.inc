<?php
/**
 * @file
 * fe_project.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function fe_project_default_rules_configuration() {
  $items = array();
  $items['rules_wrap_with_project'] = entity_import('rules_config', '{ "rules_wrap_with_project" : {
      "LABEL" : "Wrap with Project",
      "PLUGIN" : "reaction rule",
      "REQUIRES" : [ "rules", "flag" ],
      "ON" : [ "flag_flagged_worth_checking" ],
      "IF" : [
        { "node_is_of_type" : {
            "node" : [ "flagged-node" ],
            "type" : { "value" : { "idea" : "idea" } }
          }
        },
        { "entity_has_field" : { "entity" : [ "flagged-node" ], "field" : "field_category" } },
        { "data_is_empty" : { "data" : [ "flagged-node:field-project-ref" ] } }
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
        { "data_set" : {
            "data" : [ "entity-created:language" ],
            "value" : [ "flagged-node:language" ]
          }
        }
      ]
    }
  }');
  return $items;
}
