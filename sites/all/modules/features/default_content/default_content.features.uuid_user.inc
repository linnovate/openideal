<?php

/**
 * Implementation of hook_uuid_features_default_user().
 */
function default_content_uuid_features_default_user() {
  $users = array();

  $users[] = array(
    'name' => 'Bryan',
    'pass' => '4a4a0abc4f2a49f584bf2dd37b7396e5',
    'mail' => 'Bryan@idealmail.com',
    'mode' => '0',
    'sort' => '0',
    'threshold' => '0',
    'theme' => '',
    'signature' => '',
    'signature_format' => '0',
    'created' => '1304710444',
    'access' => '0',
    'login' => '0',
    'status' => '1',
    'timezone' => NULL,
    'language' => '',
    'picture' => '',
    'init' => '',
    'data' => 'a:0:{}',
    'timezone_name' => '',
    'roles' => array(
      '2' => 'authenticated user',
    ),
    'uuid' => 'fd85555a-78ab-11e0-ad89-c44619ee2e3d',
    'nodewords' => array(
      'metatags' => array(),
    ),
    'heartbeat_relations' => array(
      '8' => '8',
    ),
  );
  $users[] = array(
    'name' => 'Eleanor',
    'pass' => 'f44fe29983b4ea8765bf541b3445e29e',
    'mail' => 'Eleanor@idealmail.com',
    'mode' => '0',
    'sort' => '0',
    'threshold' => '0',
    'theme' => '',
    'signature' => '',
    'signature_format' => '0',
    'created' => '1304710444',
    'access' => '0',
    'login' => '0',
    'status' => '1',
    'timezone' => NULL,
    'language' => '',
    'picture' => '',
    'init' => '',
    'data' => 'a:0:{}',
    'timezone_name' => '',
    'roles' => array(
      '2' => 'authenticated user',
    ),
    'uuid' => 'fd86a549-78ab-11e0-ad89-c44619ee2e3d',
    'nodewords' => array(
      'metatags' => array(),
    ),
    'heartbeat_relations' => array(
      '10' => '10',
    ),
  );
  $users[] = array(
    'name' => 'David',
    'pass' => '0d02ad8a47fcff976a3d0b4a84e2f678',
    'mail' => 'David@idealmail.com',
    'mode' => '0',
    'sort' => '0',
    'threshold' => '0',
    'theme' => '',
    'signature' => '',
    'signature_format' => '0',
    'created' => '1304710444',
    'access' => '0',
    'login' => '0',
    'status' => '1',
    'timezone' => NULL,
    'language' => '',
    'picture' => '',
    'init' => '',
    'data' => 'a:0:{}',
    'timezone_name' => '',
    'roles' => array(
      '2' => 'authenticated user',
    ),
    'uuid' => 'fd8729ea-78ab-11e0-ad89-c44619ee2e3d',
    'nodewords' => array(
      'metatags' => array(),
    ),
    'heartbeat_relations' => array(
      '11' => '11',
    ),
  );
  $users[] = array(
    'name' => 'admin',
    'pass' => '202cb962ac59075b964b07152d234b70',
    'mail' => 'raz@linnovate.net',
    'mode' => '0',
    'sort' => '0',
    'threshold' => '0',
    'theme' => '',
    'signature' => '',
    'signature_format' => '0',
    'created' => '1304710418',
    'access' => '1304882643',
    'login' => '1304710484',
    'status' => '1',
    'timezone' => NULL,
    'language' => '',
    'picture' => '',
    'init' => 'raz@linnovate.net',
    'data' => 'a:0:{}',
    'timezone_name' => '',
    'roles' => array(
      '2' => 'authenticated user',
    ),
    'uuid' => 'fd884c67-78ab-11e0-ad89-c44619ee2e3d',
    'nodewords' => array(
      'metatags' => array(),
    ),
    'heartbeat_relations' => array(
      '1' => '1',
    ),
  );
  return $users;
}
