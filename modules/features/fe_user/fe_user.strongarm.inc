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
  $strongarm->name = 'gravatar_default';
  $strongarm->value = '3';
  $export['gravatar_default'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'gravatar_rating';
  $strongarm->value = 'G';
  $export['gravatar_rating'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'gravatar_size';
  $strongarm->value = '100';
  $export['gravatar_size'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'gravatar_url';
  $strongarm->value = 'http://www.gravatar.com/avatar/';
  $export['gravatar_url'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'gravatar_url_ssl';
  $strongarm->value = 'https://secure.gravatar.com/avatar/';
  $export['gravatar_url_ssl'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_picture_default';
  $strongarm->value = 'http://upload.wikimedia.org/wikipedia/commons/d/d3/User_Circle.png';
  $export['user_picture_default'] = $strongarm;

  return $export;
}
