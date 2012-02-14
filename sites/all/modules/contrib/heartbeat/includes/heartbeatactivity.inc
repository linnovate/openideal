<?php
// $Id: heartbeatactivity.inc,v 1.1.2.1.2.41 2010/10/16 09:16:35 stalski Exp $

/**
 * @file
 *   HeartbeatActivity object
 *   Defines one heartbeat activity object.
 */

/**
 * Class defines an activity message object
 */
class HeartbeatActivity {

  // DB fields
  public $uaid = 0;
  public $message_id = '';
  public $nid_info = '';
  public $nid_target_info = '';
  public $uid = 0;
  public $uid_access = TRUE;
  public $uid_target = 0;
  public $nid = 0;
  public $nid_access = TRUE;
  public $nid_target = 0;
  public $nid_target_access = TRUE;
  public $timestamp = 0;
  public $language = 'en';
  public $message = '';
  public $message_concat = '';
  public $access = HEARTBEAT_PUBLIC_TO_ALL;
  public $variables = array();
  public $additions = NULL;
  public $classes = '';
  // User object of the uid, being the actor of activity.
  public $actor = NULL;
  // Array of associated user activity ID's.
  public $uaids = array();

  // Template composite
  public $template = '';

  // Global properties on message base.
  public $show_time = 0;
  public $count = 0; // count for same instance
  public $target_count = 0; // count for same instance that is merged by target
  public $display_time = TRUE;
  public $time_info_grouped = 0;
  public $delete_access = FALSE;
  public $actor_access = FALSE;

  // Template variables
  public $name = 'heartbeatactivity';
  public $type = 'heartbeatactivity';
  public $build_mode = 'none';
  public $content = array();

  /**
   * constructor
   * @param $data Array or Object with fields that can match this object
   *   Data for the message, some are converted.
   *   It's possible to give a sql-result set as data which will
   *   adapt to a heartbeatActivity object.
   * @param $template HeartbeatMessageTemplate
   *   holds the template data.
   */
  function __construct($data, HeartbeatMessageTemplate $template) {

    $this->template = $template;
    $this->additions = new stdClass();
    $this->message_id = $this->template->message_id;

    if (is_object($data)) {
      $data = (array) $data;
    }

    if (isset($data['uaid'])) {
      $this->uaid = $data['uaid'];
    }
    if (isset($data['count'])) {
      $this->count = $data['count'];
    }
    $this->uid = $data['uid'];
    $this->actor = heartbeat_user_load($data['uid']);
    $this->uid_target = $data['uid_target'];
    $this->nid = $data['nid'];
    $this->nid_info = !empty($data['nid_info']) ? unserialize($data['nid_info']) : array();
    $this->nid_target = isset($data['nid_target']) ? (int)$data['nid_target'] : 0;
    $this->nid_target_info = !empty($data['nid_target_info']) ? unserialize($data['nid_target_info']) : array();
    $this->timestamp = !empty($data['timestamp']) ? $data['timestamp'] : $_SERVER['REQUEST_TIME'];
    // For performance reasons, one activity has knowlegde of the comment count.
    $this->additions->comment_count = isset($data['uaid_comments']) ? $data['uaid_comments'] : 0;
    $this->access = (int)$data['access'];

    if (isset($data['message'])) {
      $this->message = $data['message'];
    }
    else {
      $this->message = $this->template->message;
    }

    if (!empty($data['message_concat'])) {
      $this->message_concat = $data['message_concat'];
    }
    else {
      $this->message_concat = $this->template->message_concat;
    }

    $this->set_variables($data['variables']);

    if (isset($data['extra']['duplicate'])) {
      $this->variables['duplicate'] = $data['extra']['duplicate'];
    }
  }

  /**
   * Method gets a heartbeat_activity variable
   *
   * @desc The magic getter method fetches a variable
   *       in members. If not found there, it will always
   *       check the content holder as well
   */
  public function __get($variable) {
    return $this->variables[$variable];
  }

  /**
   * Method sets a heartbeat_activity property
   *
   * @desc The magic setter method sets a variable to member
   *  If not found, just stash in the content array
   */
  public function __set($variable, $value) {
    $this->variables[$variable] = $value;
  }

  /*
   * Rebuild the message with given candidate variables
   */
  public function rebuild_message($concat = FALSE) {

    if (count($this->variables) > 0) {
      $variables = array();
      foreach ($this->variables as $key => $value) {
        $key[0] = '!';
        $variables[$key] = $value;
      }
    }

    if ($concat) {
      return t($this->template->message_concat, $variables);
    }
    return t($this->template->message, $variables);
  }

  /*
   * Rebuild the message with given candidate variables
   */
  public function create_grouped_message($candidates, $max_items_to_group = 5) {

    global $base_url;

    $message_template = $this->rebuild_message(TRUE);
    $message_extension = '';
    $message_template = str_replace("%times%", $this->target_count, $message_template);
    $message_template = str_replace("%count%", $this->target_count, $message_template);

    // Prepare the merged factors from the stored messages
    $merged_string = '';

    $remains = array();
    $target = $this->template->concat_args['group_target'];
    $beat = 'beat-item-' . $this->uaid;
    $unique = $candidates['variables'];
    $count = $candidates['count'];//count($candidates['variables']);

    // Limit the number of displayed (grouped) messages by message
    // or left as default global configuration
    if (!empty($this->template->concat_args['group_num_max'])) {
      $max_items_to_group = $this->template->concat_args['group_num_max'];
    }

    // Reduce the variables for substitution to the maximum
    // Add the remains by keeping the others plus the last one,
    // in case the total exceeds the max group setting.
    if ($count > $max_items_to_group) {
      $count = $max_items_to_group;
      $unique = array_slice($unique, 0, $count);
      $remains = array_slice($candidates['variables'], $count - 1);
    }

    // Replacement of placeholder with group targets
    // If there is a match for %variable%
    // Try to replace the variable with the group_target variables
    if (preg_match_all("|\%(.*)\%(.*)|U", $message_template, $matches)) {

      if (count($remains) > 0) {
        $hidden_remains = '<ul id="' . $beat . '-remaining" style="display: none;">';
        foreach ($remains as $remain) {
          $hidden_remains .= '<li>' . $remain["@". $target] . '</li>';
        }
        $hidden_remains .= '</ul>';
      }

      // TODO Make a decision on how to handle the target on merged groups
      // This code in comment is/was a attempt to have more grouping words
      // The difficult and almost impossible part is the fact that you always
      // have to target something to group on (for the same node or same user)
      //foreach ($matches[1] as $target) {
        $placeholder = $matches[1][0];

        $i = 1;
        foreach ($unique as $stored_variables) {

          // limit them to the value given by the group variable setting
          if (isset($stored_variables["@". $target])) {
            if ($i == 1) {
              $merged_string .= ' ' . $stored_variables["@". $target];
            }
            elseif ($i < $count && $count > 2) {
              $merged_string .= ' ' . $this->template->concat_args['merge_separator'];
              $merged_string .= ' ' . $stored_variables["@". $target];
            }
            elseif ($i == $count || $count == 2) {
              $merged_string .= ' ' . $this->template->concat_args['merge_end_separator'];
              if (count($remains) >= 1 && $hidden_remains != '') {
                $attributes = array(
                  'attributes' => array(
                    'absolute' => TRUE,
                    'onclick' => 'javascript:$(\'#' . $beat . '-remaining\').toggle(\'slow\'); return false;'),
                    'fragment' => $beat);
                $merged_string .= ' ' . l(t('@num more', array('@num' => count($remains))), $base_url . request_uri(), $attributes);
                $message_extension .= $hidden_remains;
              }
              else {
                $merged_string .= ' ' . $stored_variables["@". $target];
              }
            }
          }
          $i++;
        }
        $message_template = str_replace("%". $placeholder ."%", $merged_string, $message_template);
        if (isset($message_extension)) {
          $message_template .= $message_extension;
        }
      //}
    }
    $this->message = $message_template;
  }

  /**
   * Public function to save activity to database
   * @param array raw argument to enforce as is (pre-renderd)
   */
  public function save($raw_args = array()) {

    $locale = module_exists('locale');

    if (!empty($this->nid)) {
      $node = node_load($this->nid);
      // Add extra node access information for nid.
      if (empty($this->nid_info)) {
        // Add extra node access information for nid.
        $this->nid_info = array(
          'uid' => $node->uid,
          'type' => $node->type,
          'format' => $node->format,
        );
      }
    }
    if (!empty($this->nid_target)) {
      $node_target = node_load($this->nid);
      // Add extra node access information for nid_target.
      $this->nid_target_info = array(
        'uid' => $node_target->uid,
        'type' => $node_target->type,
        'format' => $node_target->format,
      );
    }

    // if a user selected a language, then we follow i19n.
    if (isset($node) && !empty($node->language)) {
      return $this->_save($raw_args, $node->language);
    }

    if ($locale) {
      return $this->save_locale($raw_args);
    }
    else {
      return $this->_save($raw_args);
    }
  }

  /**
   * Sets the variables array
   * Data variables are stored in an array to use them to build real variables.
   * this means that when activity message objects get merged, the variables
   * will be filled with variables components from all instances.
   */
  private function set_variables($variables = NULL) {

    if (!empty($variables)) {

      if (is_string($variables)) {
        $this->variables = heartbeat_decode_message_variables($variables);
      }
      elseif (is_array($variables)) {
        $this->variables = $variables;
      }
    }
  }

  /**
   * Save activity log with multilingual content
   * and multilingual parts to pre-translate
   *
   * @param array $raw_args
   */
  private function save_locale($raw_args = array()) {

    $logged = FALSE;
    $args = $this->rebuild_arguments($raw_args, TRUE);
    $locale = $args['locale'];
    unset($args['locale']);

    // Save activity by logging a row for each active language
    // Translations only when locale exists
    $languages = locale_language_list();

    $tuaid = NULL;
    foreach ($languages as $language => $human_language) {

      // preprocess multilingual message "parts"
      // for all flagged token replacements
      foreach ($this->variables as $key => $value) {
        if (isset($locale[$key])) {
          $amp_token = str_replace("#", "!", $key);
          $args[$amp_token] = locale($locale[$key], $language);
        }
      }

      $uaid = $this->log_message($args, $language);

      // Each language has the first uaid (user activity id) as
      // tuaid (translated user activity id)
      if ($uaid && !isset($tuaid)) {
        $tuaid = $uaid;
      }
      db_query("INSERT INTO {heartbeat_translations} SET tuaid=%d, uaid=%d", $tuaid, $uaid);

    }

    $this->uaid = $tuaid;
    return $tuaid;
  }

  /**
   * Save activity log
   *
   * @param array $raw_args
   */
  private function _save($raw_args = array(), $lang = '') {
    // Rebuild arguments with tokens
    $args = $this->rebuild_arguments($raw_args);
    $this->uaid = $this->log_message($args, $lang);
    return $this->uaid;
  }

  /**
   * Logs a heartbeat message
   * @param string language optional
   *
   */
  private function log_message($args, $lang = '') {

    if (empty($lang)) {
      global $language;
      $lang = $language->language;
    }

    // Mis-usage of the t-function, but how could i fix this?
    $message = t($this->message, $args, $lang);
    $message_concat = t($this->message_concat, $args, $lang);

    // Checks if there should be logging what so ever
    if (empty($message)) {
      watchdog('heartbeat', 'Error in logging user activity: it is not possible to log empty message', array(), WATCHDOG_ERROR);
      return FALSE;
    }

    // Log relational message to user activity
    $logged = db_query("INSERT INTO {heartbeat_activity}
      SET uid=%d, uid_target=%d, nid=%d, nid_target=%d, message_id='%s', language='%s',
      access = %d, message ='%s',message_concat ='%s', timestamp=%d
      , nid_info='%s', nid_target_info='%s', uaid_comments=%d, variables='%s'",
      $this->uid,  $this->uid_target, $this->nid, $this->nid_target, $this->message_id, $lang,
      $this->access, $message, $message_concat, $this->timestamp,
      serialize($this->nid_info), serialize($this->nid_target_info), 0,
      heartbeat_encode_message_variables($this->variables));

    if (!$logged) {
      return 0;
    }
    $uaid = db_last_insert_id('heartbeat_activity', 'uaid');
    $this->uaids[] = $uaid;
    return $uaid;

  }

  /**
   * Rebuild the arguments for variables
   * to share within this object
   *
   * @param array $raw_input of arguments
   */
  private function rebuild_arguments($raw_input, $locale = FALSE) {
    $args = array();
    if ($locale) {
      // Variables that need to be pre-translated go here
      $args['locale'] = array();
    }

    $tags = heartbeat_allowed_tags();

    // Rebuild arguments with language tokens
    foreach ($this->variables as $key => $value) {

      $value = filter_xss($value, $tags);

      // Leave $key[0] == "!"  asis
      if ($key[0] != "@" && $key[0] != "#" ) {
        //continue; // bad argument
      }
      $oldkey = $key;
      // Reset the key of the arguments to ! to parse the next
      // tokenization asis.
      if ($key[0] == "@") $key[0] = "!";
      // # and @ token replacement prefixes are kept,
      // but set a flag for it in the raw_arguments
      if ($key[0] == "#") {
        // if it has to be translated ...
        if ($locale) $args['locale'][$key] = $value;
        // Now reset the key
        $key[0] = "!";
      }
      // if argument is prefilled, override
      if (isset($raw_input[$oldkey])) {
        $args[$key] = $raw_input[$oldkey];
        continue;
      }
      // Argument gets the value as in variables
      $args[$key] = $value;
    }

    return $args;
  }


} // eof class heartbeat_activity
