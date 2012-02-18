<?php
/**
 * @file
 * message_example.features.inc
 */

/**
 * Implementation of hook_default_message_type().
 */
function message_example_default_message_type() {
  $items = array();
  $items['create_content'] = entity_import('message_type', '{
    "name" : "create_content",
    "description" : "Create content",
    "argument_keys" : [ "@url @title" ],
    "category" : "message_type",
    "message_text" : { "und" : [
        {
          "value" : "created \\u003ca href=\\"@url\\"\\u003e@title\\u003c\\/a\\u003e",
          "format" : "filtered_html",
          "safe_value" : "\\u003cp\\u003ecreated \\u003ca href=\\"@url\\"\\u003e@title\\u003c\\/a\\u003e\\u003c\\/p\\u003e\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  return $items;
}
