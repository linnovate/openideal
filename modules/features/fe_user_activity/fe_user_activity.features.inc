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
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
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
          "value" : "\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E has \\u003Ca href=\\u0022[message:field-message-comment:url]\\u0022\\u003Ecommented\\u003C\\/a\\u003E to \\u003Ca href=\\u0022[message:field-message-content:url]\\u0022\\u003E[message:field-message-content:title]\\u003C\\/a\\u003E",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003E\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E has \\u003Ca href=\\u0022[message:field-message-comment:url]\\u0022\\u003Ecommented\\u003C\\/a\\u003E to \\u003Ca href=\\u0022[message:field-message-content:url]\\u0022\\u003E[message:field-message-content:title]\\u003C\\/a\\u003E\\u003C\\/p\\u003E\\n"
        }
      ]
    },
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
          "value" : "\\u003Ca href=\\u0022[message:field-message-content:url]\\u0022\\u003E[message:field-message-content:title]\\u003C\\/a\\u003E has been added by \\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E",
          "format" : "filtered_html",
          "safe_value" : "\\u003Cp\\u003E\\u003Ca href=\\u0022url]\\u0022\\u003E[message:field-message-content:title]\\u003C\\/a\\u003E has been added by \\u003Ca href=\\u0022url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E\\u003C\\/p\\u003E\\n"
        }
      ]
    },
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
          "value" : "\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E has been voted on \\u003Ca href=\\u0022[message:field-message-content:url]\\u0022\\u003E[message:field-message-content:title]\\u003C\\/a\\u003E",
          "format" : "filtered_html",
          "safe_value" : "\\u003Cp\\u003E\\u003Ca href=\\u0022url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E has been voted on \\u003Ca href=\\u0022url]\\u0022\\u003E[message:field-message-content:title]\\u003C\\/a\\u003E\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  return $items;
}
