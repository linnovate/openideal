<?php

/**
 * @file
 * A member of the Grammar Parser API classes. These classes provide an API for
 * parsing, editing and rebuilding a code snippet.
 *
 * Copyright 2009 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Grammar Parser base class.
 *
 * This class provides members and routines shared by its extended classes.
 *
 * This parent class holds references to arrays of:
 * - tokens,
 * - statements,
 * - interfaces, classes, and functions,
 * - function calls,
 * - constants, globals, and comments.
 *
 * By using the getter and setter methods, the API provides the flexibility to
 * allow the child classes to have access to and be able to manipulate the same
 * array or different arrays.
 */
class PGPParser {

  /**
   * Array of tokens corresponding to a code snippet.
   *
   * @var array
   */
  protected $tokens = array();

  /**
   * A PGPBody (or PGPExpression) instance of grammar statements corresponding to tokens.
   *
   * @var array
   */
  protected $statements = NULL;

  /**
   * Array of interfaces included in grammar statements.
   *
   * This should point to a PGPInterfaces class that also holds lists of
   * functions.
   *
   * @var array
   */
  protected $interfaces = array();

  /**
   * Array of classes included in grammar statements.
   *
   * This should point to a PGPClasses class that also holds lists of functions
   * and calls.
   *
   * @var array
   */
  protected $classes = array();

  /**
   * Array of functions included in grammar statements.
   *
   * @var array
   */
  protected $functions = array();

  /**
   * Array of function calls included in grammar statements.
   *
   * @var array
   */
  protected $functionCalls = array();

  /**
   * Array of defines (non-class constants) included in grammar statements.
   *
   * @var array
   */
  protected $defines = array();

  /**
   * Array of global variables included in grammar statements.
   *
   * @var array
   */
  protected $globals = array();

  /**
   * Array of document comments included in grammar statements.
   *
   * @var array
   */
  protected $comments = array();

  /**
   * Whether to print debug information.
   *
   * @var boolean
   */
  protected $debug = FALSE;

  public function __construct() {
    @set_error_handler(array($this, 'errorHandler'));
    @set_exception_handler(array($this, 'exception_handler'));

    $this->initValues();
  }

  /**
   * Creates additional language token constants used by the grammar parser.
   *
   * The comment defines are not needed since we require PHP5.
   */
  protected function initValues() {
    // Add custom tokens ('else if' should be in tokenizer).
    $this->defineConstant('T_ELSE_IF', 600);
    $this->defineConstant('T_FUNCTION_CALL', 601);
    $this->defineConstant('T_ASSIGNMENT', 602);
    $this->defineConstant('T_SPECIAL_STRING', 604);
    $this->defineConstant('T_DEFINE', 606);

    // Mask T_ML_COMMENT (PHP4) as T_COMMENT (PHP5).
    if (!defined('T_ML_COMMENT')) {
      define('T_ML_COMMENT', T_COMMENT);
    }
    // Mask T_DOC_COMMENT (PHP5) as T_ML_COMMENT (PHP4).
    elseif (!defined('T_DOC_COMMENT')) {
      define('T_DOC_COMMENT', T_ML_COMMENT);
    }
  }

  /**
   * Defines a constant if not already defined.
   *
   * @param string $name
   *   Constant name.
   * @param mixed $value
   *   Constant value.
   */
  protected function defineConstant($name, $value) {
    if (!defined($name)) {
      define($name, $value);
    }
  }

  /**
   * Returns array of tokens related to code snippet that was parsed.
   *
   * @return array
   *   An array of tokens.
   */
  public function getTokens() {
    return $this->tokens;
  }

  /**
   * Returns list of grammar statements.
   *
   * @return PGPBody (or PGPExpression)
   *   A list of statements.
   */
  public function &getStatements() {
    return $this->statements;
  }

  /**
   * Sets the list of grammar statements.
   *
   * @param PGPBody (or PGPExpression) $statements
   *   A list of grammar statements.
   */
  public function setStatements(&$statements = NULL) {
    $this->statements = &$statements;
  }

  /**
   * Returns array of interfaces included in grammar statements.
   *
   * @return array
   *   An array of statements.
   */
  public function &getInterfaces() {
    return $this->interfaces;
  }

  /**
   * Returns array of classes included in grammar statements.
   *
   * @return array
   *   An array of statements.
   */
  public function &getClasses() {
    return $this->classes;
  }

  /**
   * Returns array of functions included in grammar statements.
   *
   * @return array
   *   An array of statements.
   */
  public function &getFunctions() {
    return $this->functions;
  }

  /**
   * Returns array of function calls included in grammar statements.
   *
   * @return array
   *   An array of statements.
   */
  public function &getFunctionCalls() {
    return $this->functionCalls;
  }

  /**
   * Returns array of defines included in grammar statements.
   *
   * @return array
   *   An array of statements.
   */
  public function &getDefines() {
    return $this->defines;
  }

  /**
   * Returns array of global variables included in grammar statements.
   *
   * @return array
   *   An array of statements.
   */
  public function &getGlobals() {
    return $this->globals;
  }

  /**
   * Returns array of comments included in grammar statements.
   *
   * @return array
   *   An array of statements.
   */
  public function &getComments() {
    return $this->comments;
  }

  /**
   * Sets the debug flag used with debug printing.
   *
   * @param boolean $debug
   *   Indicates whether to print debug information.
   */
  public function setDebug($debug = FALSE) {
    $this->debug = $debug;
  }

  /**
   * Returns token name (including custom tokens).
   *
   * @param integer $token
   *   The token in question.
   * @return string
   *   The token name.
   */
  public function tokenName($token) {
    switch ($token) {
      case T_ELSE_IF:
        return 'T_ELSE_IF';

      case T_FUNCTION_CALL:
        return 'T_FUNCTION_CALL';

      case T_ASSIGNMENT:
        return 'T_ASSIGNMENT';

      case T_SPECIAL_STRING:
        return 'T_SPECIAL_STRING';

      case T_DEFINE:
        return 'T_DEFINE';

      default:
        return token_name($token);
    }
  }

  /**
   * Prints debug information if debug flag is on.
   *
   * @param mixed $text
   *   A string or array to print.
   */
  protected function debugPrint($text) {
    static $path = '';

    if (!$this->debug) {
      return;
    }
    if (!$path) {
      $path = $this->debugPath();
    }
    if ($text instanceof PGPList) {
      file_put_contents($path, $text->print_r(), FILE_APPEND);
    }
    elseif ($text instanceof PGPBase) {
      file_put_contents($path, $text->print_r(), FILE_APPEND);
    }
    elseif (is_object($text)) {
//      print_r($text);
    }
    elseif (is_array($text)) {
      file_put_contents($path, print_r($text, 1), FILE_APPEND);
    }
    else {
      file_put_contents($path, $text . "\n", FILE_APPEND);
    }
  }

  /**
   * Prints debug information regardless of debug flag.
   *
   * @param mixed $text
   *   A string or array to print.
   */
  protected function debugPrint2($text) {
    $debug = $this->debug;
    $this->debug = TRUE;
    $this->debugPrint($text);
    $this->debug = $debug;
  }

  /**
   * Returns path to debug file.
   *
   * @return string
   *   Path to file.
   */
  public function debugPath() {
    static $path = '';

    if (!$path) {
      $path = '.';
      if (function_exists('file_directory_path')) {
        $path = file_directory_path();
        if (defined('PARSER_DIR') && is_dir($path . '/' . variable_get('gpui_dir', PARSER_DIR))) {
          $path .= '/' . variable_get('gpui_dir', PARSER_DIR);
        }
      }
      $path .= '/debug.txt';
    }
    return $path;
  }

  /*
   * Error and exception handling routines copied from DrupalTestCase in
   * drupal_web_test_case.php. This code is similar to that in common.inc.
   * However, the latter lists more error constants.
   */

  /**
   * Fires an error assertion.
   *
   * @param $message
   *   The message to display along with the assertion.
   * @param $severity
   *   The error reporting level.
   * @param $caller
   *   The caller of the error.
   * @return
   *   FALSE.
   */
  protected function error($message = '', $severity = '', array $caller = NULL) {
//    return $this->assert('exception', $message, $group, $caller);
    echo "ERROR: " . print_r($message, 1) . " on line " . $caller['line'] . " in " . $caller['file'] . " ($severity)" . "\n";
  }

  /**
   * Handles errors.
   *
   * Because this is registered in set_error_handler(), it has to be public.
   * @see set_error_handler
   *
   */
  public function errorHandler($severity, $message, $file = NULL, $line = NULL) {
    if ($severity & error_reporting()) {
      $error_map = array(
        E_STRICT => 'Run-time notice',
        E_WARNING => 'Warning',
        E_NOTICE => 'Notice',
        E_CORE_ERROR => 'Core error',
        E_CORE_WARNING => 'Core warning',
        E_USER_ERROR => 'User error',
        E_USER_WARNING => 'User warning',
        E_USER_NOTICE => 'User notice',
        E_RECOVERABLE_ERROR => 'Recoverable error',
      );

      $backtrace = debug_backtrace();
      $this->error($message, $error_map[$severity], $this->getLastCaller($backtrace));
    }
    return TRUE;
  }

  /**
   * Handles exceptions.
   *
   * @see set_exception_handler
   */
  protected function exceptionHandler($exception) {
    $backtrace = $exception->getTrace();
    // Push on top of the backtrace the call that generated the exception.
    array_unshift($backtrace, array(
      'line' => $exception->getLine(),
      'file' => $exception->getFile(),
    ));
    $this->error($exception->getMessage(), 'Uncaught exception', $this->getLastCaller($backtrace));
  }

  /**
   * Returns the last caller from a backtrace.
   *
   * Copied from _drupal_get_last_caller. Add here so code can be run:
   * - from command line without invoking Drupal
   * - in Drupal 6 and 7.
   *
   * @param $backtrace
   *   A standard PHP backtrace.
   * @return
   *   An associative array with keys 'file', 'line' and 'function'.
   */
  protected function getLastCaller($backtrace) {
    // Errors that occur inside PHP internal functions
    // do not generate information about file and line.
    while ($backtrace && !isset($backtrace[0]['line'])) {
      array_shift($backtrace);
    }

    // The first trace is the call itself.
    // It gives us the line and the file of the last call.
    $call = $backtrace[0];

    // The second call give us the function where the call originated.
    if (isset($backtrace[1])) {
      if (isset($backtrace[1]['class'])) {
        $call['function'] = $backtrace[1]['class'] . $backtrace[1]['type'] . $backtrace[1]['function'] . '()';
      }
      else {
        $call['function'] = $backtrace[1]['function'] . '()';
      }
    }
    else {
      $call['function'] = 'main()';
    }
    return $call;
  }
}
