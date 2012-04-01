<?php

/**
 * Implements hook_heartbeat_stream_info().
 */
function heartbeat_heartbeat_stream_info() {

  $heartbeatstreams = array();

  $heartbeatstream = new HeartbeatStreamConfig;
  $heartbeatstream->disabled = FALSE; /* Edit this to true to make a default heartbeatstream disabled initially */
  $heartbeatstream->api_version = 1;
  $heartbeatstream->class = 'singleactivity';
  $heartbeatstream->name = 'Single activity';
  $heartbeatstream->module = 'heartbeat';
  $heartbeatstream->title = 'Activity for one single instance';
  $heartbeatstream->path = 'streams/singleactivity.inc';
  $heartbeatstream->settings = array(
    'messages_denied' => array(),
    'page_disabled' => 1,
  );
  $heartbeatstream->variables = array();
  $heartbeatstreams['singleactivity'] = $heartbeatstream;

  $heartbeatstream = new HeartbeatStreamConfig;
  $heartbeatstream->disabled = FALSE; /* Edit this to true to make a default heartbeatstream disabled initially */
  $heartbeatstream->api_version = 1;
  $heartbeatstream->class = 'viewsactivity';
  $heartbeatstream->name = 'Views activity';
  $heartbeatstream->module = 'heartbeat';
  $heartbeatstream->title = 'Views activity (automatically used for views)';
  $heartbeatstream->path = 'streams/viewsactivity.inc';
  $heartbeatstream->settings = array(
    'messages_denied' => array(),
    'page_disabled' => 1,
  );
  $heartbeatstream->variables = array();
  $heartbeatstreams['viewsactivity'] = $heartbeatstream;

  return $heartbeatstreams;

}

/**
 * Prepares a heartbeat stream.
 * @param $stream_name Name of the stream.
 */
function heartbeat_stream($stream_name, $page = 0, $account = NULL) {

  // Load a configuration object for the stream.
  $streamConfig = heartbeat_stream_config_load($stream_name);

  // Load the stream if access to it is granted.
  if (_heartbeat_stream_has_access($streamConfig)) {

    $heartbeatStream = HeartbeatStreamFactory::getStream($streamConfig, $page, $account);

    drupal_add_js(array('heartbeatPollNewerMessages' => array($streamConfig->name => $streamConfig->poll_messages)), 'setting');

    // Give other modules a chance to take action when the stream is loaded.
    module_invoke_all('heartbeat_stream_load', $heartbeatStream);

    if ($heartbeatStream instanceof HeartbeatStream && !$heartbeatStream->hasErrors()) {
      return $heartbeatStream;
    }
  }

  return FALSE;
}

/**
 * Builds a heartbeat stream in the StreamObject.
 */
function heartbeat_stream_build(HeartbeatStream & $heartbeatStream) {
  // Load the messages.
  $messages = $heartbeatStream->execute();
}

/**
 * Builds a heartbeat stream from views results.
 */
function heartbeat_stream_views_build(HeartbeatStream & $heartbeatStream, $values) {
  // Load the messages from views response.
  return $heartbeatStream->processViewsQuery($values);
}

/**
 * Views a heartbeat stream.
 */
function heartbeat_stream_view(HeartbeatStream & $heartbeatStream) {

  $build = array('#type' => 'markup', 'messages' => array());
  $weight = 0;

  // Get the messages.
  foreach (array_keys($heartbeatStream->messages) as $key) {
    $build['messages'][$key] = heartbeat_activity_view($heartbeatStream->messages[$key]);
    $build['messages'][$key]['#weight'] = $weight;
    $weight++;
  }
  $build['messages']['#sorted'] = TRUE;

  $build['pager'] = array(
    '#stream' => $heartbeatStream,
    '#theme' => 'activity_pager',
    '#weight' => 5,
  );

  if ($heartbeatStream->needsModal()) {
    // Add CTools modal requirements.
    heartbeat_ctools_modal_prepare();
  }

  // Add the javascript and css files.
  $build['#attached']['js'][] = drupal_get_path('module', 'heartbeat') . '/js/heartbeat.js';
  $build['#attached']['css'][] = drupal_get_path('module', 'heartbeat') . '/css/heartbeat.css';

  return $build;

}

/**
 * Load all the stream configuration objects.
 */
function heartbeat_stream_config_load_all() {

  $streams = &drupal_static('heartbeat_streams');

  if ($object = cache_get('heartbeat_streams')) {
    $streams = $object->data;
  }
  else {
    // Fetch the streams with ctools export.
    ctools_include('export');
    $streams = ctools_export_crud_load_all('heartbeat_streams');
    cache_set('heartbeat_streams', $streams);
  }

  return $streams;

}

/**
 * Implementation of ctools load all function.
 */
function _heartbeat_stream_config_load_all($reset) {
  $streams = ctools_export_load_object('heartbeat_streams', 'all');
  foreach (array_keys($streams) as $key) {
    _heartbeat_stream_config_unpack($streams[$key]);
  }
  return $streams;
}

/**
 * Menu loader for the configuration object of a stream.
 */
function heartbeat_stream_config_load($class) {

  $streams = &drupal_static('heartbeat_streams');

  if (!isset($streams[$class])) {
    ctools_include('export');
    $streams[$class] = ctools_export_crud_load('heartbeat_streams', $class);
    _heartbeat_stream_config_unpack($streams[$class]);
  }

  return $streams[$class];

}

/**
 * Implmentation of the Ctools load function.
 */
function _heartbeat_stream_config_load($name) {

  $result = ctools_export_load_object('heartbeat_streams', 'names', array($name));
  if (isset($result[$name])) {
    return $result[$name];
  }

}

/**
 * Helper function to unpack the settings into the root of the object.
 */
function _heartbeat_stream_config_unpack(HeartbeatStreamConfig & $streamConfig) {
  if (!empty($streamConfig->settings)) {
    foreach ($streamConfig->settings as $key => $value) {
      $streamConfig->$key = $value;
    }
  }
}

/**
 * Helper function to check access on activity stream
 */
function _heartbeat_stream_has_access($stream) {

  if (is_string($stream)) {
    ctools_include('export');
    $streamConfig = ctools_export_crud_load('heartbeat_streams', $stream);
  }
  else {
    $streamConfig = $stream;
  }

  return user_access('view heartbeat messages') && user_access('view ' . $streamConfig->name . ' stream');

}

/**
 * Factory to return a stream object.
 */
class HeartbeatStreamFactory {

  static public function getStream($streamConfig, $page = 0, $account = NULL) {

    try {
      heartbeat_include('HeartbeatParser');
      heartbeat_include('HeartbeatStream');
      heartbeat_include($streamConfig->name, $streamConfig->module, $streamConfig->path);

      switch ($streamConfig->class) {
        default:
          $className = !empty($streamConfig->real_class) ? $streamConfig->real_class : $streamConfig->class;
          if (class_exists($className)) {
            return new $className($streamConfig, $page, $account);
          }
          else {
            throw new HeartbeatInvalidStreamException(t('Stream class @class does not exist in @path or it\'s not auto-included in yourmodule.info.', array('@class' => $streamConfig->class, '@path' => $streamConfig->path)), 1000, 'stream does not exist');
          }
      }
    }
    catch (HeartbeatInvalidStreamException $h) {
      drupal_set_message($h->getMessage());
      return NULL;
    }

  }

}

class HeartbeatInvalidStreamException extends Exception {

  public $arguments;

  function __construct($message, $code = 0, $arguments = array()) {
    parent::__construct($message, $code);
    $this->arguments = $arguments;
  }

}

/**
 * eof().
 */
