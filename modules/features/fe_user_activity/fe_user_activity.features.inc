<?php
/**
 * @file
 * fe_user_activity.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function fe_user_activity_ctools_plugin_api($module = NULL, $api = NULL) {
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function fe_user_activity_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_default_message_type().
 */
function fe_user_activity_default_message_type() {
  $items = array();
  $items['new_challenge'] = entity_import('message_type', '{
    "name" : "new_challenge",
    "description" : "New challenge",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 1 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "\\u003Ca href=\\u0022[message:field-challenge:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:field-challenge:title]\\u003C\\/strong\\u003E\\u003C\\/a\\u003E was posted by \\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E.",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003E\\u003Ca href=\\u0022[message:field-challenge:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:field-challenge:title]\\u003C\\/strong\\u003E\\u003C\\/a\\u003E was posted by \\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E.\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  $items['new_comment'] = entity_import('message_type', '{
    "name" : "new_comment",
    "description" : "New comment",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 1 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "en",
    "arguments" : null,
    "message_text" : {
      "en" : [
        {
          "value" : "\\u003Cdiv class=\\u0022user-image\\u0022\\u003E\\u003Cimg src=\\u0022[message:user:picture:url]\\u0022 \\/\\u003E\\u003C\\/div\\u003E\\u003Cdiv class=\\u0022user-text\\u0022\\u003E\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:user:name]\\u003C\\/strong\\u003E\\u003C\\/a\\u003E\\u003Cdiv class=\\u0022comment-body\\u0022\\u003E[message:field-comment:body]\\u003C\\/div\\u003E\\u003C\\/div\\u003E\\r\\n",
          "format" : "full_html",
          "safe_value" : "\\u003Cdiv class=\\u0022user-image\\u0022\\u003E\\u003Cimg src=\\u0022[message:user:picture:url]\\u0022 \\/\\u003E\\u003C\\/div\\u003E\\n\\u003Cdiv class=\\u0022user-text\\u0022\\u003E\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:user:name]\\u003C\\/strong\\u003E\\u003C\\/a\\u003E\\u003Cbr \\/\\u003E\\u003Cdiv class=\\u0022comment-body\\u0022\\u003E[message:field-comment:body]\\u003C\\/div\\u003E\\n\\u003C\\/div\\u003E\\n"
        }
      ],
      "und" : [
        {
          "value" : "\\u003Cdiv class=\\u0022user-image\\u0022\\u003E\\u003Cimg src=\\u0022[message:user:picture:url]\\u0022\\u003E\\u003C\\/img\\u003E\\u003C\\/div\\u003E\\u003Cdiv class=\\u0022user-text\\u0022\\u003E\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:user:name]\\u003C\\/strong\\u003E\\u003C\\/a\\u003E [message:field-comment:subject]\\u003C\\/div\\u003E",
          "format" : "full_html",
          "safe_value" : "\\u003Cdiv class=\\u0022user-image\\u0022\\u003E\\u003Cimg src=\\u0022[message:user:picture:url]\\u0022 \\/\\u003E\\u003C\\/div\\u003E\\n\\u003Cdiv class=\\u0022user-text\\u0022\\u003E\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:user:name]\\u003C\\/strong\\u003E\\u003C\\/a\\u003E [message:field-comment:subject]\\u003C\\/div\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  $items['new_idea'] = entity_import('message_type', '{
    "name" : "new_idea",
    "description" : "New idea",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 1 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "\\u003Ca href=\\u0022[message:field-posted-idea:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:field-posted-idea:title]\\u003Cstrong\\u003E\\u003C\\/a\\u003E was posted by \\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E in the challenge \\u003Ca href=\\u0022[message:field-challenge:url]\\u0022\\u003E[message:field-challenge:title]\\u003C\\/a\\u003E.",
          "format" : "full_html",
          "safe_value" : "\\u003Cp\\u003E\\u003Ca href=\\u0022[message:field-posted-idea:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:field-posted-idea:title]\\u003Cstrong\\u003E\\u003C\\/strong\\u003E\\u003C\\/strong\\u003E\\u003C\\/a\\u003E was posted by \\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E[message:user:name]\\u003C\\/a\\u003E in the challenge \\u003Ca href=\\u0022[message:field-challenge:url]\\u0022\\u003E[message:field-challenge:title]\\u003C\\/a\\u003E.\\u003C\\/p\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  $items['new_user'] = entity_import('message_type', '{
    "name" : "new_user",
    "description" : "New user",
    "argument_keys" : [],
    "argument" : [],
    "category" : "message_type",
    "data" : {
      "token options" : { "clear" : 1 },
      "purge" : { "override" : 0, "enabled" : 0, "quota" : "", "days" : "" }
    },
    "language" : "",
    "arguments" : null,
    "message_text" : { "und" : [
        {
          "value" : "\\u003Cdiv class=\\u0022user-image\\u0022\\u003E\\u003Cimg src=\\u0022[message:user:picture:url]\\u0022\\u003E\\u003C\\/img\\u003E\\u003C\\/div\\u003E\\u003Cdiv class=\\u0022user-text\\u0022\\u003E\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:user:name]\\u003C\\/strong\\u003E\\u003C\\/a\\u003E has joined OpenideaL.\\u003C\\/div\\u003E",
          "format" : "full_html",
          "safe_value" : "\\u003Cdiv class=\\u0022user-image\\u0022\\u003E\\u003Cimg src=\\u0022[message:user:picture:url]\\u0022 \\/\\u003E\\u003C\\/div\\u003E\\n\\u003Cdiv class=\\u0022user-text\\u0022\\u003E\\u003Ca href=\\u0022[message:user:url]\\u0022\\u003E\\u003Cstrong\\u003E[message:user:name]\\u003C\\/strong\\u003E\\u003C\\/a\\u003E has joined OpenideaL.\\u003C\\/div\\u003E\\n"
        }
      ]
    },
    "rdf_mapping" : []
  }');
  return $items;
}
