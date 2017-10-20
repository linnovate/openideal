<?php
/**
 * @file
 * fe_notifications.rules_defaults.inc
 */

/**
 * Implements hook_default_rules_configuration().
 */
function fe_notifications_default_rules_configuration() {
  $items = array();
  $items['rules_send_mail_for_idea_status_change'] = entity_import('rules_config', '{ "rules_send_mail_for_idea_status_change" : {
      "LABEL" : "Send mail for idea status change",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Notifications" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_update" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "NOT data_is" : {
            "data" : [ "node-unchanged:field-in-action" ],
            "value" : [ "node:field-in-action" ]
          }
        },
        { "data_is" : { "data" : [ "node:author:field-content-notifications" ], "value" : 1 } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "node:author:mail" ],
            "subject" : "The status of your idea [node:title] has changed .",
            "message" : "Hello,\\r\\n\\r\\nThe status of your idea [node:title] in the challenge [node:field-challenge-ref:title] has changed.\\r\\nClick here to view the idea: [node:url]\\r\\n\\r\\nRegards,\\r\\n[site:name]",
            "from" : "[site:mail]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_send_mail_for_new_idea'] = entity_import('rules_config', '{ "rules_send_mail_for_new_idea" : {
      "LABEL" : "Send mail for new idea",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Notifications" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "node_insert" : [] },
      "IF" : [
        { "node_is_of_type" : { "node" : [ "node" ], "type" : { "value" : { "idea" : "idea" } } } },
        { "data_is" : {
            "data" : [ "node:field-challenge-ref:author:field-content-notifications" ],
            "value" : 1
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "node:field-challenge-ref:author:mail" ],
            "subject" : "New idea in your challenge [node:field-challenge-ref:title].",
            "message" : "Hello,\\r\\n\\r\\nThe idea [node:title] was created in your challenge [node:field-challenge-ref:title].\\r\\nClick here to view the idea: [node:url]\\r\\n\\r\\nRegards,\\r\\n[site:name]",
            "from" : "[site:mail]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_send_mail_for_new_user'] = entity_import('rules_config', '{ "rules_send_mail_for_new_user" : {
      "LABEL" : "Send mail for new user",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Notifications" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "user_insert" : [] },
      "DO" : [
        { "mail_to_users_of_role" : {
            "roles" : { "value" : { "3" : "3", "5" : "5" } },
            "subject" : "A new user registered on [site:name].",
            "message" : "Hello,\\r\\n\\r\\nThe user [account:name] has registered on the site [site:name]\\r\\nClick here to view the user: [account:url]\\r\\n\\r\\nRegards,\\r\\n[site:name]",
            "from" : "[site:mail]"
          }
        }
      ]
    }
  }');
  $items['rules_send_mail_notification_about_the_mention'] = entity_import('rules_config', '{ "rules_send_mail_notification_about_the_mention" : {
      "LABEL" : "Send mail notification about the mention",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "TAGS" : [ "Notifications" ],
      "REQUIRES" : [ "rules" ],
      "ON" : { "mentions_insert" : [] },
      "IF" : [
        { "data_is" : { "data" : [ "mention:user:field-mention-notifications" ], "value" : 1 } }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "mention:user:mail" ],
            "subject" : "[mention:author] mentioned you on [site:name]",
            "message" : "Hello,\\r\\n\\r\\n[mention:author] mentioned you on [mention:node]\\r\\nClick here to read more: [mention:node_url]\\r\\n\\r\\nRegards,\\r\\n[site:name]",
            "from" : "[site:mail]",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  $items['rules_send_mail_to_idea_author_on_comment'] = entity_import('rules_config', '{ "rules_send_mail_to_idea_author_on_comment" : {
      "LABEL" : "send_mail_to_idea_author_on_comment",
      "PLUGIN" : "reaction rule",
      "OWNER" : "rules",
      "REQUIRES" : [ "rules", "comment" ],
      "ON" : { "comment_insert--comment_node_idea" : { "bundle" : "comment_node_idea" } },
      "IF" : [
        { "data_is" : {
            "data" : [ "comment:node:author:field-comment-notficiations" ],
            "value" : "1"
          }
        }
      ],
      "DO" : [
        { "mail" : {
            "to" : [ "comment:node:author:mail" ],
            "subject" : "New comments for your idea: [comment:node:title]",
            "message" : "Hi [comment:node:author]\\r\\n\\r\\n\\u003Cstrong\\u003E[comment:author]\\u003C\\/strong\\u003E added a new comment to your idea \\u003Cstrong\\u003E[comment:node:title]\\u003C\\/strong\\u003E:\\r\\n[comment:body]\\r\\n\\r\\n\\r\\nContinue the discussion at: [comment:url]\\r\\n\\r\\nRegards,\\r\\n[site:name]\\r\\n",
            "language" : [ "" ]
          }
        }
      ]
    }
  }');
  return $items;
}
