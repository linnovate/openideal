<?php

function _flag_abuse_abuse_node_flags(&$flags) {
  // Exported flag: "Node Abuse".
  $flags[] = array(
    'content_type' => 'node',
    'name' => 'abuse_node',
    'title' => 'Node Abuse',
    'global' => 0,
    'types' => array(
      0 => 'story',
    ),
    'events' => array(
      0 => 'flag',
      1 => 'unflag',
      2 => 'reset',
    ),
    'flag_short' => 'Flag as offensive',
    'flag_long' => 'Flag this content as offensive.',
    'flag_message' => '',
    'unflag_short' => 'Flag as non-offensive',
    'unflag_long' => 'Flag this content as non-offensive.',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'show_on_page' => TRUE,
    'show_on_teaser' => FALSE,
    'show_on_form' => FALSE,
    'access_author' => '',
    'i18n' => 0,
    'flag_confirmation' => 'Are you sure you want to flag this content as offensive?',
    'unflag_confirmation' => 'Are you sure you want to flag this content as non-offensive?',
    'reset_short' => 'Reset flags',
    'reset_long' => 'Reset all flags on this content',
    'reset_confirmation' => 'Are you sure you want to reset all flags on this content?',
    'status' => FALSE,
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'module' => 'flag_abuse',
    'api_version' => 2,
  );
  return $flags;
}

function _flag_abuse_abuse_comment_flags(&$flags) {
  // Exported flag: "Comment Abuse".
  $flags[] = array(
    'content_type' => 'comment',
    'name' => 'abuse_comment',
    'title' => 'Comment Abuse',
    'global' => 0,
    'types' => array(
      0 => 'story',
    ),
    'events' => array(
      0 => 'flag',
      1 => 'unflag',
    ),
    'flag_short' => 'Flag as offensive',
    'flag_long' => 'Flag this content as offensive.',
    'flag_message' => '',
    'unflag_short' => 'Flag as non-offensive',
    'unflag_long' => 'Flag this content as non-offensive.',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'access_author' => '',
    'show_on_comment' => TRUE,
    'flag_confirmation' => 'Are you sure you want to flag this content as offensive?',
    'unflag_confirmation' => 'Are you sure you want to flag this content as non-offensive?',
    'reset_short' => 'Reset flags',
    'reset_long' => 'Reset all flags on this commnet',
    'reset_confirmation' => 'Are you sure you want to reset all flags on this comment?',
    'status' => FALSE,
    'show_on_teaser' => FALSE,
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'module' => 'flag_abuse',
    'api_version' => 2,
  );
  return $flags;
}

function _flag_abuse_abuse_user_flags(&$flags) {
  // Exported flag: "User Abuse".
  $flags[] = array(
    'content_type' => 'user',
    'name' => 'abuse_user',
    'title' => 'User Abuse',
    'global' => 0,
    'types' => array(
    ),
    'events' => array(
      0 => 'flag',
      1 => 'unflag',
    ),
    'flag_short' => 'Report User',
    'flag_long' => 'Report this user as offensive.',
    'flag_message' => '',
    'unflag_short' => 'Remove Report',
    'unflag_long' => 'Remove your report about this user.',
    'unflag_message' => '',
    'unflag_denied_text' => '',
    'link_type' => 'confirm',
    'roles' => array(
      'flag' => array(
        0 => '2',
      ),
      'unflag' => array(
        0 => '2',
      ),
    ),
    'show_on_profile' => TRUE,
    'access_uid' => '',
    'flag_confirmation' => 'Are you sure you want to report this user as offensive?',
    'unflag_confirmation' => 'Are you sure you want to remove your report of this user?',
    'reset_short' => 'Reset flags',
    'reset_long' => 'Reset all flags on this user',
    'reset_confirmation' => 'Are you sure you want to reset all flags on this user?',
    'status' => FALSE,
    'show_on_teaser' => FALSE,
    'locked' => array(
      'name' => 'name',
      'global' => 'global',
    ),
    'module' => 'flag_abuse',
    'api_version' => 2,
  );
  return $flags;
}
