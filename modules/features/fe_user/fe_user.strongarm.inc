<?php
/**
 * @file
 * fe_user.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function fe_user_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_default';
  $strongarm->value = 'http://upload.wikimedia.org/wikipedia/commons/d/d3/User_Circle.png';
  $export['user_picture_default'] = $strongarm;

  return $export;
}
