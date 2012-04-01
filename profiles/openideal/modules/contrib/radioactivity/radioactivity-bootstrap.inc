<?php
/**
 * This file is used both in drupal and in callback files.
 */

// prepare variables
if (!defined("DRUPAL_ROOT")) {
  // We're not in drupal but we need 'some' functionality so...
  _radioactivity_light_initialization();
} else {
  // We're in drupal, setup necessary vars
  define("VAR_RADIOACTIVITY_CHECKSUM_SALT", variable_get("radioactivity_checksum_salt", "undefined"));
  if (class_exists('Memcache')) {
    define("VAR_RADIOACTIVITY_MEMCACHED_HOST", variable_get("radioactivity_memcached_host", "localhost"));
    define("VAR_RADIOACTIVITY_MEMCACHED_PORT", variable_get("radioactivity_memcached_port", "11211"));
  }
}



/**
 * Get the config file path of R
 */
function _radioactivity_get_config_file_path() {
  $dir = dirname(__FILE__);
  $config_file = $dir . "/radioactivity-bootstrap.cfg.inc";
  return $config_file;
}

/** 
 * Do a light system initialization
 */
function _radioactivity_light_initialization() {
  if (!defined("RADIOACTIVITY_BOOTSTRAPPED")) {
     
    $config_file = _radioactivity_get_config_file_path();

    if (file_exists($config_file)) {
      
      include($config_file);
   
    } else {

      _radioactivity_require_bootstrapping();
      // Grab the checksum variable
      $var = variable_get('radioactivity_checksum_salt', 'undefined');
      define("VAR_RADIOACTIVITY_CHECKSUM_SALT", $var);
      variable_set("radioactivity_config_warning", true);
      if (class_exists('Memcache')) {
        define("VAR_RADIOACTIVITY_MEMCACHED_HOST", variable_get("radioactivity_memcached_host", "localhost"));
        define("VAR_RADIOACTIVITY_MEMCACHED_PORT", variable_get("radioactivity_memcached_port", "11211"));
      }
    }
    
    define("RADIOACTIVITY_BOOTSTRAPPED", true);
  }
}


/**
 * Do a light boostrap
 */
function _radioactivity_require_bootstrapping() {
  // use the VERSION to figure out if we've done bootstrapping already
  if (!defined("VERSION")) {

    include "radioactivity.module";
    
    if (!defined("VAR_RADIOACTIVITY_DRUPAL_ROOT")) {
      define("VAR_RADIOACTIVITY_DRUPAL_ROOT", $_SERVER['DOCUMENT_ROOT']);
    }
    define('DRUPAL_ROOT', VAR_RADIOACTIVITY_DRUPAL_ROOT);
    chdir(DRUPAL_ROOT);
    
    if (!file_exists('./includes/bootstrap.inc')) {
      die("Unable to figure out bootstrapping directory!");
    }
    
    require_once './includes/bootstrap.inc';
    drupal_bootstrap(DRUPAL_BOOTSTRAP_VARIABLES);
    variable_set("radioactivity_bootstrap_warning", true);
  }
}

/**
 * Generate a checksum for given data
 */
function _radioactivity_checksum_generate($data) {
  return md5(VAR_RADIOACTIVITY_CHECKSUM_SALT . $data);
}


/**
 * Validate checksum against given data
 */
function _radioactivity_checksum_validate($data, $checksum) {
  return strcmp(_radioactivity_checksum_generate($data), $checksum) === 0;
}



/**
 * Validate (and filter) a payload 
 */
function _radioactivity_validate_payload($pl) {
  if (!isset($pl['checksum']) || !isset($pl['data'])) {
    return false;
  }
  if (!_radioactivity_checksum_validate($pl['data'], $pl['checksum'])) {
    return false;
  }
  $payload = array(
    'checksum' => $pl['checksum'],
    'data' => $pl['data'],
  );
  return $payload;
}



/**
 * Prepare payload
 */
function _radioactivity_prepare_payload($data) {
  return array(
    'checksum' => _radioactivity_checksum_generate($data),
    'data' => $data,
  );
}


