<?php

/**
 * Implementation of hook_uuid_features_default_comments().
 */
function default_content_uuid_features_default_comments() {
  $comments = array();

  $comment[] = array(
    'pid' => '0',
    'subject' => 'zxcxzc',
    'comment' => 'zxcxzc',
    'timestamp' => '1304779680',
    'status' => '0',
    'format' => '1',
    'thread' => '02/',
    'name' => 'admin',
    'mail' => '',
    'homepage' => '',
    'uuid' => '30ae192d-78b9-11e0-ad89-c44619ee2e3d',
    'uuid_node' => FALSE,
    'uuid_user' => 'fd8231e5-78ab-11e0-ad89-c44619ee2e3d',
  );
  $comment[] = array(
    'pid' => '0',
    'subject' => 'comments',
    'comment' => 'comments',
    'timestamp' => '1304774118',
    'status' => '0',
    'format' => '1',
    'thread' => '01/',
    'name' => 'admin',
    'mail' => '',
    'homepage' => '',
    'uuid' => '3cd26e2d-78ac-11e0-ad89-c44619ee2e3d',
    'uuid_node' => FALSE,
    'uuid_user' => 'fd8231e5-78ab-11e0-ad89-c44619ee2e3d',
  );
  return $comments;
}
