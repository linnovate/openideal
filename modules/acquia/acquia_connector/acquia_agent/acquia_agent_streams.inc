<?php
// $Id: acquia_agent_streams.inc,v 1.4 2009/06/16 20:47:10 pwolanin Exp $

/**
 * @file
 *   XML-RPC communication functions for Acquia communication.
 */

/**
 * Performs one or more XML-RPC request(s), using a PHP stream context
 * when creating the socket.  This function is copied and modified
 * from Drupal 6's common.inc and xmlrpc.inc.
 *
 * This function should never be called directly - use acquia_agent_call().
 *
 * @param $context
 *   A PHP stream context created with stream_create_context().  This
 *   context will be used when a socket connection to the XML-RPC
 *   endpoint is created.
 * @param ...
 *   The rest of the parameters and return values are the same as xmlrpc().
 */
function _acquia_agent_xmlrpc() {
  require_once './includes/xmlrpc.inc';

  $args = func_get_args();
  $context = array_shift($args);
  $url = array_shift($args);
  if (is_array($args[0])) {
    $method = 'system.multicall';
    $multicall_args = array();
    foreach ($args[0] as $call) {
      $multicall_args[] = array('methodName' => array_shift($call), 'params' => $call);
    }
    $args = array($multicall_args);
  }
  else {
    $method = array_shift($args);
  }
  $xmlrpc_request = xmlrpc_request($method, $args);
  $result = acquia_agent_http_request($context, $url, array("Content-Type" => "text/xml"), 'POST', $xmlrpc_request->xml);
  if ($result->code != 200) {
    xmlrpc_error($result->code, $result->error);
    return FALSE;
  }
  $message = xmlrpc_message($result->data);
  // Now parse what we've got back
  if (!xmlrpc_message_parse($message)) {
    // XML error
    xmlrpc_error(-32700, t('Parse error. Not well formed'));
    return FALSE;
  }
  // Is the message a fault?
  if ($message->messagetype == 'fault') {
    xmlrpc_error($message->fault_code, $message->fault_string);
    return FALSE;
  }
  // Message must be OK
  return $message->params[0];
}

/**
 * Builds a stream context based on a url and local .pem file if available.
 */
function acquia_agent_stream_context_create($url, $module = 'acquia_agent') {
  $opts = array();
  $uri = parse_url($url);
  if (isset($uri['scheme']) && ($uri['scheme'] == 'https') && variable_get('acquia_agent_verify_peer', 0)) {
    // Look for a local certificate to validate the server identity.
    $pem_file = drupal_get_path('module', $module) .'/'. $uri['host'] . '.pem';
    if (file_exists($pem_file)) {
      $opts['ssl'] = array(
        'verify_peer' => TRUE,
        'cafile' => $pem_file,
        'allow_self_signed' => FALSE, // doesn't mean anything in this case
        'CN_match' => $uri['host']);
    }
  }
  return stream_context_create($opts);
}

/**
 * Perform an HTTP request.  This function is copied and modified
 * from Drupal 6's common.inc.
 *
 * @param $context
 *   A PHP stream context created with stream_create_context().  This
 *   context will be used when a socket connection is created.
 * @param ...
 *   The rest of the parameters and return values are the same as xmlrpc().
 */
function acquia_agent_http_request($context, $url, $headers = array(), $method = 'GET', $data = NULL, $retry = 3) {
  $result = new stdClass();

  // Parse the URL and make sure we can handle the schema.
  $uri = parse_url($url);

  switch ($uri['scheme']) {
    case 'http':
      $port = isset($uri['port']) ? $uri['port'] : 80;
      $host = $uri['host'] . ($port != 80 ? ':'. $port : '');
      $fp = @fsockopen($uri['host'], $port, $errno, $errstr, 15);
      break;
    case 'https':
      // Note: Only works for PHP 4.3 compiled with OpenSSL.
      $port = isset($uri['port']) ? $uri['port'] : 443;
      $host = $uri['host'] . ($port != 443 ? ':'. $port : '');
      if (!isset($context)) {
        $fp = @fsockopen('ssl://'. $uri['host'], $port, $errno, $errstr, 20);
      }
      else {
        $fp = @stream_socket_client('ssl://'. $uri['host'] .':'. $port, $errno, $errstr, 20, STREAM_CLIENT_CONNECT, $context);
        if (!$fp && $errno == 0) {
          // An SSL error occurred.  I do not know of any way to get
          // an error code or message programmatically.  By not having
          // an @ before stream_socket_client(), the actual SSL error
          // will be logged via watchdog.
          $errno = 999;
          $errstr = t('SSL error creating socket');
        }
      }
      break;
    default:
      $result->error = 'invalid schema '. $uri['scheme'];
      return $result;
  }

  // Make sure the socket opened properly.
  if (!$fp) {
    // When a network error occurs, we use a negative number so it does not
    // clash with the HTTP status codes.
    $result->code = -$errno;
    $result->error = trim($errstr);
    return $result;
  }

  // Construct the path to act on.
  $path = isset($uri['path']) ? $uri['path'] : '/';
  if (isset($uri['query'])) {
    $path .= '?'. $uri['query'];
  }

  // Create HTTP request.
  $defaults = array(
    // RFC 2616: "non-standard ports MUST, default ports MAY be included".
    // We don't add the port to prevent from breaking rewrite rules checking the
    // host that do not take into account the port number.
    'Host' => "Host: $host",
    'User-Agent' => 'User-Agent: Drupal (+http://drupal.org/)',
  );

  // Only add Content-Length if we actually have any content or if it is a POST
  // or PUT request. Some non-standard servers get confused by Content-Length in
  // at least HEAD/GET requests, and Squid always requires Content-Length in
  // POST/PUT requests.
  if (!empty($data) || $method == 'POST' || $method == 'PUT') {
    $defaults['Content-Length'] = 'Content-Length: '. strlen($data);
  }

  // If the server url has a user then attempt to use basic authentication
  if (isset($uri['user'])) {
    $defaults['Authorization'] = 'Authorization: Basic '. base64_encode($uri['user'] . (!empty($uri['pass']) ? ":". $uri['pass'] : ''));
  }

  foreach ($headers as $header => $value) {
    $defaults[$header] = $header .': '. $value;
  }

  $request = $method .' '. $path ." HTTP/1.0\r\n";
  $request .= implode("\r\n", $defaults);
  $request .= "\r\n\r\n";
  if ($data) {
    $request .= $data ."\r\n";
  }
  $result->request = $request;

  fwrite($fp, $request);

  // Fetch response.
  $response = '';
  while (!feof($fp) && $chunk = fread($fp, 1024)) {
    $response .= $chunk;
  }
  fclose($fp);

  // Parse response.
  list($split, $result->data) = explode("\r\n\r\n", $response, 2);
  $split = preg_split("/\r\n|\n|\r/", $split);

  list($protocol, $code, $text) = explode(' ', trim(array_shift($split)), 3);
  $result->headers = array();

  // Parse headers.
  while ($line = trim(array_shift($split))) {
    list($header, $value) = explode(':', $line, 2);
    if (isset($result->headers[$header]) && $header == 'Set-Cookie') {
      // RFC 2109: the Set-Cookie response header comprises the token Set-
      // Cookie:, followed by a comma-separated list of one or more cookies.
      $result->headers[$header] .= ','. trim($value);
    }
    else {
      $result->headers[$header] = trim($value);
    }
  }

  $responses = array(
    100 => 'Continue', 101 => 'Switching Protocols',
    200 => 'OK', 201 => 'Created', 202 => 'Accepted', 203 => 'Non-Authoritative Information', 204 => 'No Content', 205 => 'Reset Content', 206 => 'Partial Content',
    300 => 'Multiple Choices', 301 => 'Moved Permanently', 302 => 'Found', 303 => 'See Other', 304 => 'Not Modified', 305 => 'Use Proxy', 307 => 'Temporary Redirect',
    400 => 'Bad Request', 401 => 'Unauthorized', 402 => 'Payment Required', 403 => 'Forbidden', 404 => 'Not Found', 405 => 'Method Not Allowed', 406 => 'Not Acceptable', 407 => 'Proxy Authentication Required', 408 => 'Request Time-out', 409 => 'Conflict', 410 => 'Gone', 411 => 'Length Required', 412 => 'Precondition Failed', 413 => 'Request Entity Too Large', 414 => 'Request-URI Too Large', 415 => 'Unsupported Media Type', 416 => 'Requested range not satisfiable', 417 => 'Expectation Failed',
    500 => 'Internal Server Error', 501 => 'Not Implemented', 502 => 'Bad Gateway', 503 => 'Service Unavailable', 504 => 'Gateway Time-out', 505 => 'HTTP Version not supported'
  );
  // RFC 2616 states that all unknown HTTP codes must be treated the same as the
  // base code in their class.
  if (!isset($responses[$code])) {
    $code = floor($code / 100) * 100;
  }

  switch ($code) {
    case 200: // OK
    case 304: // Not modified
      break;
    case 301: // Moved permanently
    case 302: // Moved temporarily
    case 307: // Moved temporarily
      $location = $result->headers['Location'];

      if ($retry) {
        $result = acquia_agent_http_request($context, $result->headers['Location'], $headers, $method, $data, --$retry);
        $result->redirect_code = $result->code;
      }
      $result->redirect_url = $location;

      break;
    default:
      $result->error = $text;
  }

  $result->code = $code;
  return $result;
}

/**
 * Determine if a response from the Acquia Network is valid.
 *
 * @param $data
 *   The data array returned by acquia_agent_call().
 * @return
 *   TRUE or FALSE.
 */
function acquia_agent_valid_response($data) {
  $authenticator = $data['authenticator'];
  $result = $data['result'];
  $result_auth = $result['authenticator'];
  $valid = ($authenticator['nonce'] == $result_auth['nonce']);
  $valid = $valid && ($authenticator['time'] < $result_auth['time']);
  $key = acquia_agent_settings('acquia_key');
  $hash = _acquia_agent_hmac($key, $result_auth['time'], $result_auth['nonce'], $result['body']);
  return $valid && ($hash == $result_auth['hash']);
}

/**
 * Send a XML-RPC request.
 *
 * This function should never be called directly - use acquia_agent_call().
 */
function _acquia_agent_request($url, $method, $data) {
  $ctx = acquia_agent_stream_context_create($url);
  if (!$ctx) {
    // TODO: what's a meaningful fault code?
    xmlrpc_error(-1, t('SSL is not supported or setup failed'));
    $result = FALSE;
  }
  else {
    $result = _acquia_agent_xmlrpc($ctx, $url, $method, $data);
  }
  if ($errno = xmlrpc_errno()) {
    watchdog('acquia agent', '@message (@errno): %server - %method - <pre>@data</pre>', array('@message' => xmlrpc_error_msg(), '@errno' => xmlrpc_errno(), '%server' => $url, '%method' => $method, '@data' => print_r($data, TRUE)), WATCHDOG_ERROR);
  }
  return $result;
}

/**
 * Creates an authenticator based on xmlrpc params and a HMAC-SHA1.
 */
function _acquia_agent_authenticator($params = array(), $identifier = NULL, $key = NULL) {
  if (empty($identifier)) {
    $identifier = acquia_agent_settings('acquia_identifier');
  }
  if (empty($key)) {
    $key = acquia_agent_settings('acquia_key');
  }
  $time = time();
  $nonce = md5(acquia_agent_random_bytes(55));
  $authenticator['identifier'] = $identifier;
  $authenticator['time'] = $time;
  $authenticator['hash'] = _acquia_agent_hmac($key, $time, $nonce, $params);
  $authenticator['nonce'] = $nonce;
  return $authenticator;
}

/**
 * Calculates a HMAC-SHA1 according to RFC2104 (http://www.ietf.org/rfc/rfc2104.txt).
 * With addition of xmlrpc params.
 */
function _acquia_agent_hmac($key, $time, $nonce, $params) {
  return base64_encode(
    pack("H*", sha1((str_pad($key, 64, chr(0x00)) ^ (str_repeat(chr(0x5c), 64))) .
    pack("H*", sha1((str_pad($key, 64, chr(0x00)) ^ (str_repeat(chr(0x36), 64))) .
    $time .':'. $nonce .':'. $key .':'. serialize($params)))))
  );
}

