<?php
function ideal_suggestions_default_rules_configuration() {
  $configs = array();
  $rule = '{ "rules_send_email_notification_about_the_mention" : {
    "LABEL" : "Send email notification about the mention",
    "PLUGIN" : "reaction rule",
    "REQUIRES" : [ "rules", "mentions" ],
    "ON" : [ "mentions_insert" ],
    "DO" : [
      { "mail" : {
          "to" : "[mention:user_email]",
          "subject" : "[mention:author] mentioned you on [site:name]",
          "message" : "Hello,\r\n[mention:author] mentioned you on [mention:node]\r\nClick here to read more: [mention:node_url]\r\n\r\nRegards,\r\n[site:name]",
          "from" : "[site:mail]",
          "language" : [ "" ]
        }
      }
    ]
  }
}';
  $configs['rules_send_email_notification_about_the_mention'] = rules_import($rule);
  return $configs;
}
