<?php
/**
 * @file
 * fe_user_activity.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_user_activity_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
}

/**
 * Implements hook_views_api().
 */
function fe_user_activity_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_default_message_type().
 */
function fe_user_activity_default_message_type() {
  $items = array();
  $items['log_new_comment'] = entity_import('message_type', '{
    "name" : "log_new_comment",
    "description" : "Log new comment",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : { "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" } },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "[message:user:link] has \\u003Ca href=\\u0022[message:field-message-comment:url]\\u0022\\u003Ecommented\\u003C\\/a\\u003E to [message:field-message-content:link]",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003E[message:user:link] has \\u003Ca href=\\u0022[message:field-message-comment:url]\\u0022\\u003Ecommented\\u003C\\/a\\u003E to [message:field-message-content:link]\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "metatags" : [],
    "rdf_mapping" : []
  }');
  $items['log_new_content'] = entity_import('message_type', '{
    "name" : "log_new_content",
    "description" : "Log new content",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : { "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" } },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "[message:field-message-content:link] has been added by [message:user:link]",
          "format" : "filtered_html",
          "safe_value" : "\\u003Cp\\u003E[message:field-message-content:link] has been added by [message:user:link]\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "metatags" : [],
    "rdf_mapping" : []
  }');
  $items['log_new_vote'] = entity_import('message_type', '{
    "name" : "log_new_vote",
    "description" : "Log new vote",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : { "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" } },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "[message:user:link] has been voted on [message:field-message-content:link]",
          "format" : "filtered_html",
          "safe_value" : "\\u003Cp\\u003E[message:user:link] has been voted on [message:field-message-content:link]\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "metatags" : [],
    "rdf_mapping" : []
  }');
  return $items;
}
