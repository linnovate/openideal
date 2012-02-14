<?php

/**
 * @file
 * A member of the Grammar Parser API classes. These classes provide an API for
 * parsing, editing and rebuilding a code snippet.
 *
 * Copyright 2009 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Grammar Parser statement writer class.
 *
 * This class provides an API for rebuilding a code snippet from a structured
 * array based on the grammar of the programming language. The code snippet may
 * be a single line, a function, or an entire file.
 *
 * @example Write a code file from a grammar array.
 * @code
 *   // Assume $reader is of class PGPReader.
 *   $writer = new PGPWriter();
 *   $string = $writer->toString($reader->getStatements());
 *   file_put_contents($filename, $string);
 * @endcode
 */
class PGPWriter extends PGPParser {

  /*
   * Singleton instance of this class.
   */
  private static $writer;

  /**
   * Indent depth during recursive calls to the print routines.
   *
   * @var integer
   */
  public static $indent = 0;

  /**
   * Class constructor.
   *
   * @param PGPBody $statements
   *   A list of grammar statements to convert back to a code snippet.
   */
  public function __construct($statements = NULL) {
    parent::__construct();

    $this->setStatements($statements);
  }

  /**
   * Performs any necessary initialization.
   */
  protected function initValues() {
    parent::initValues();
  }

  /**
   * Returns singleton instance of this class.
   *
   * @return PGPWriter
   */
  public static function getInstance() {
    if (!self::$writer) {
      self::$writer = new PGPWriter();
    }
    return self::$writer;
  }

  /**
   * Converts statements to string of PHP code.
   *
   * @param PGPBody $statements
   *   A list of grammar statements to convert back to a code snippet.
   * @return string
   *   The code snippet string.
   */
  public function toString($statements) {
    if ($statements instanceof PGPList /*&& !$statements->isEmpty()*/) {
      $this->statements = $statements;
    }
    if (!$this->statements || $this->statements->isEmpty()) {
      return 'NOTHING TO PRINT';
    }

    $this->debugPrint("inside " . __FUNCTION__);
    $this->debugPrint('COUNT=' . $this->statements->count());

    // Global items.
    PGPWriter::$indent = 0;

    // Local items.
    $strings = array();

    // Rebuild grammar statements into a string of code.
    PGPWriter::$indent--;
    $strings[] = $this->statements->toString();
    PGPWriter::$indent++;

    // Remove spaces at ends of lines.
    return preg_replace('/[ ]+$/m', '', implode("\n", $strings) . "\n");
  }

  /**
   * Prints comment string.
   *
   * TODO Deprecate this.
   * - several calls remain
   *
   * @param array $comment
   *   An array constituting the comment.
   * @return string
   *   A string of the expression.
   */
  public static function printComment($comment, $indent = '') {
    if (!isset($comment) || empty($comment)) {
      return '';
    }

    if ($comment['type'] == T_DOC_COMMENT ||
        (strpos($comment['value'], '/*') === 0 && substr_count($comment['value'], "\n"))) {
      // If comments are well-formatted, then enable the following line.
//       return $indent . $comment['value'];

      // Indent multi-line comments based on the current indention amount.
      $strings = explode("\n", $comment['value']);
      foreach ($strings as &$string) {
        $string = $indent . trim($string);
      }
      // TODO Reformat to 80-character lines (including the indent and comment characters).
      return implode("\n ", $strings);
    }

    // Code commented out usually has multiple spaces following the '//'.
    if (strpos($comment['value'], '//  ') === 0) {
      return $comment['value'];
    }

    return $indent . $comment['value'];
  }

  /**
   * Returns the statement type as a string.
   *
   * @param constant $type
   *   The statement type constant.
   * @return string
   *   The statement type string.
   */
  public static function statementTypeToString($type) { // TODO Remove static if not needed by PGPClass and PGPAssignment!!!
    switch ($type) {
      case T_FUNCTION:
        return 'function';

      case T_CLASS:
        return 'class';

      case T_INTERFACE:
        return 'interface';

      case T_CONTINUE:
        return 'continue';

      case T_ECHO:
        return 'echo';

      case T_RETURN:
        return 'return';

      case T_EXIT:
        return 'exit';

      case T_CONST:
        return 'const';

      case T_GLOBAL:
        return 'global';

      case T_VAR:
        return 'var';

      case T_STATIC:
        return 'static';

      case T_DEFINE:
        return 'constant'; // For API documentation purposes, at least.

      default:
        return 'UNKNOWN-TYPE';
    }
  }
}
