<?php

/**
 * @file
 * A member of the Grammar Parser API classes. These classes provide an API for
 * parsing, editing and rebuilding a code snippet.
 *
 * Copyright 2009 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/*
 * TODO
 *
 * Questions
 * - we need easy ways to traverse the array and find the element to be manipulated.
 * - in this sense, it might be useful to have helper classes with getter and setter methods.
 * - Examples:
 * -   class PGPFunction::getParameters, getBody($line_no)
 * -   class PGPConditional::getCondition($index)
 *
 * - PGPEditor
 *    ->find(T_FUNCTION, 'my_function', $statements)
 *    ->find(array(T_REQUIRE, T_REQUIRE_ONCE, T_INCLUDE, T_INCLUDE_ONCE), '')
 *    ->insertSource($source, $index = 0);
 *
 * - PGPEditor creates new PGPFunction and assigns &$statements
 * - PGPFunction->find would not have a $statements parameter (or would default it to previously assigned $statements)
 * - PGPFunction creates new PGPFunctionCall
 * - PGPFunctionCall->insertSource($source, $index = 0) tokenizes the string and inserts it before
 *     the specified index of the operands/operators.
 *
 * - to loop on statements, find and manipulate all occurrences, use a callback as a parameter.
 * - now we could use the function call list and to tag statements as T_FUNCTION_CALL.
 */

/*
 * WARNING!
 * The following functions have not been converted from a statement array to a
 * statement list (i.e. PGPBody object) and likely will not function properly:
 *
 * - find
 * - functionBodyDeleteAll
 */

/**
 * Grammar Parser statement editor class.
 *
 * This class provides an API for editing the parsed statements from a code
 * snippet. The statements are stored in a structured array based on the
 * grammar of the programming language. The code snippet may be a single line,
 * a function, or an entire file.
 */
class PGPEditor extends PGPParser {

  /*
   * Singleton instance of this class.
   */
  private static $editor;

  private static $reader;
  private static $writer;

  public function __construct($statements = array()) {
    parent::__construct();

    $this->setStatements($statements);
  }

  /**
   * Returns singleton instance of this class.
   *
   * @return PGPReader
   */
  public static function getInstance() {
    if (!self::$editor) {
      self::$editor = new PGPEditor();
    }
    return self::$editor;
  }

  public function getReader() {
    if (!self::$reader) {
      self::$reader = new PGPReader();
//      self::$reader->setDebug(TRUE);
    }
    return self::$reader;
  }

  protected function getWriter() {
    if (!self::$writer) {
      self::$writer = new PGPWriter();
    }
    return self::$writer;
  }

  /**
   * Traverse a list of grammar statements and apply a callback function.
   *
   * In the grammar statement array, the statement['type'] constant precisely
   * identifies the statement if it is an interface, class, or function. A
   * "function call" type is assigned to several statements for simpicity and
   * code reuse. In these cases, the statement['name']['type'] constant
   * precisely identifies the statement. The "function call" statement types
   * are:
   *
   * - T_DEFINE
   * - T_STRING
   * - T_VARIABLE
   * - T_REQUIRE:
   * - T_REQUIRE_ONCE:
   * - T_INCLUDE:
   * - T_INCLUDE_ONCE:
   * - T_EVAL:
   * - T_EMPTY:
   * - T_UNSET:
   * - T_PRINT:
   * - T_THROW
   * - T_CLONE
   * - T_STRING_VARNAME
   *
   * The values to pass in the parameters are illustrated by the following
   * examples.
   *
   * To traverse functions and true function calls (where the function name is
   * a string but not a variable expression), set
   * @code
   *   $items = statements (e.g. $reader->getStatements())
   *   $types = array(T_FUNCTION, T_STRING)
   * @endcode
   *
   * To traverse included files, set
   * @code
   *   $items = function calls (e.g. $reader->getFunctionCalls())
   *   $types = array(T_REQUIRE, T_REQUIRE_ONCE, T_INCLUDE, T_INCLUDE_ONCE)
   * @endcode
   *
   * @param array $items
   *   List of statements of one or more types. After reading and parsing a
   *   code snippet using the PGPReader class, use one of the following
   *   helper functions to provide the list of statements:
   *   @code
   *     getStatements
   *     getInterfaces (not available yet)
   *     getClasses (not available yet)
   *     getFunctions
   *     getFunctionCalls (see above notes on included statements)
   *     getDefines
   *     getGlobals
   *     getComments
   *   @endcode
   *
   * @param string $callback
   *   Name of the callback function. This function will be called with each
   *   statement passing the types filter.
   *
   * @param array $types
   *   List of statement type constants used to filter statements sent to the
   *   callback. Omit this parameter to include all $items.
   *
   * @param PGPReader $reader
   *   The reader object to pass to the callback routine.
   */
  public function traverseCallback($items = array(), $callback = '', $types = array(), &$reader = NULL) {
    if (!function_exists($callback)) {
      return;
    }

    $this->debugPrint(__METHOD__);

    foreach ($items as &$item) {
      if (!is_object($item)) {
        // The reference could have been changed in another routine so that it
        // no longer refers to an ojbect.
        continue;
      }
      // The function call list contains references to PGPFunctionCall objects.
      // The other lists contain references to PGPNode objects.
      $item2 = &$item;
      if ($item instanceof PGPNode) {
        $item2 = &$item->data;
      }
      if (!empty($types)) {
        if (!in_array($item2->type, $types)) {
          if ($item2->type == T_FUNCTION_CALL && !in_array($item2->name['type'], $types)) {
            continue;
          }
        }
      }

      switch ($item2->type) {
        case T_FUNCTION:
          // Invoke callback function.
          $callback($item, $reader);
          break;

        case T_FUNCTION_CALL:
//            if (isset($item2->name)) {
//              $name = $item2->name;
//              if (isset($name['type']) && $name['type'] == T_STRING /*$type*/) {
//                return isset($name['value']) ? $name['value'] : '';
//              }
//            }
            // Invoke callback function.
            $callback($item, $reader);
          break;
      }
    }
  }

  /**
   * Returns the function object with the specified name.
   *
   * @todo Add a parameter indicating what to return: node or data.
   *
   * @param array $functions
   *   Array of nodes containing function objects.
   * @param string $name
   *   Name of function to search for.
   * @param string $return
   *   String indicating what to return: node or data.
   * @return mixed
   *   Function object if found, or NULL.
   */
  function &findFunction($functions = array(), $name = '', $return = 'data') {
    foreach ($functions as &$function) {
      if (!($function->data instanceof PGPClass)) {
        continue;
      }
      if ($function->data->name == $name) {
        if ($return == 'node') {
          return $function;
        }
        else {
          $data = &$function->data;
          return $data;
        }
        break;
      }
    }
    $result = NULL;
    return $result;
  }

  /**
   * Search for a statement of a given type with a given value.
   *
   * @param integer $type
   *   The statement type to search for.
   * @param string $value
   *   The name to search for.
   * @param PGPList $statements
   *   A list of statements to search.
   * @return ?
   *
   * @todo Add a parameter indicating return_type = array or object
   */
  public function &find($type, $value, &$statements) {
    $class = NULL;
    return $class; // TEMP

//    $this->setStatements($statements);
    $this->debugPrint(__METHOD__);
    $this->debugPrint("$type");
    $this->debugPrint("$value");
    $this->debugPrint($statements);

    $this->debugPrint("entering loop");
    // Traverse statement array to find the requested item.
    foreach ($statements as $index => &$statement) {
      $this->debugPrint($statement);
      if (!isset($statement['type'])) {
        continue;
      }
//      $this->debugPrint($statement['type']);
      switch ($statement['type']) {
        case $type:
          // Handle statement types other than interface, class, or function.
          switch ($type) {
            case T_COMMENT:
              break;

            case T_DOC_COMMENT:
              break;

            case T_WHITESPACE:
              break;

            case T_CONST:
            case T_GLOBAL:
            case T_VAR:
              break;

            case T_DEFINE:
              break;

            case T_REQUIRE:
            case T_REQUIRE_ONCE:
            case T_INCLUDE:
            case T_INCLUDE_ONCE:
              $this->debugPrint("hit case include");
              $this->debugPrint($statement);
              $class = new PGPFunctionCall2();
              $class->setStatements($statement);
              if ($value == $class->getOperand(0, 1)) {
                $this->debugPrint("found statement");
                $this->debugPrint($statement);
                return $class;
              }
              continue;
          }
          // Omit break and process special statements below.

        case T_FUNCTION:
          $this->debugPrint("inside T_FUNCTION");
          switch ($type) {
            case T_INTERFACE:
            case T_CLASS:
              continue;

            case T_FUNCTION:
              if ($statement['name'] == $value) {
                $class = new PGPFunction();
                $class->setStatements($statement);
                $this->debugPrint("return class from inside T_FUNCTION");
                return $class; // Inconsistent type class vs. array???
              }
              continue;

//            default:
//              $class = new PGPFunction();
//              if ($found = $class->find($type, $value, $statement)) {
//                return $found;
//              }
//              continue;
          }
//          break; // This prevents entering next case block!!!

        case T_FUNCTION_CALL:
          $this->debugPrint("inside T_FUNCTION_CALL");
          switch ($type) {
            case T_FUNCTION_CALL:
              if ($statement['name']['value'] == $value) {
                return $statement;

//                $class = new PGPFunctionCall2();
//                $class->setStatements($statement);
//                return $class; // Inconsistent type class vs. array???
              }
              elseif ($found = $class->find($type, $value, $statement['parameters'])) {
                return $found;
              }
              continue;

            default:
              $class = new PGPFunction();
              if ($found = $class->find($type, $value, $statement['parameters'])) {
                return $found;
              }
              continue;
          }
//          break; // This prevents entering next case block!!!

        case T_INTERFACE:
        case T_CLASS:
          break;
      }
    }

    $var = array();
    return $var;
  }

  /**
   * TODO Move this to PGPFunction class.
   * Use with traverseCallback on type=T_FUNCTION; have the callback routine
   * call this function.
   *
   * @todo This is deprecated!!!
   *
   * @param unknown_type $statements
   */
  public function functionBodyDeleteAll(&$statements = array()) {
    // Traverse statement array and remove body statements.
    foreach ($statements as &$statement) {
      if ($statement['type'] == T_CLASS) {
        $this->functionBodyDeleteAll($statement['body']);
      }
      elseif ($statement['type'] == T_FUNCTION) {
        // TODO Does unset cause us to free memory?
        $this->debugPrint("functionBodyDeleteAll " . $statement['name']);
        // Remove comment from statement.
        $temp = $statement;
        unset($temp['comment']);
        $this->debugPrint(print_r($temp, 1));
        $statement['text'] = $this->getWriter()->toString(array($temp));
        // Remove body from statement.
        unset($statement['body']);
        $statement['body'] = array();
      }
    }
  }

  /**
   * Returns an interface, class or function signature.
   *
   * TODO This is now deprecated; remove after api module is patched.
   *
   * @param PGPClass $statement
   *   A grammar object of the statement block.
   * @return string
   *   A string of the function signature.
   */
  public function functionGetSignature($statement) {
    if (!in_array($statement->type, array(T_INTERFACE, T_CLASS, T_FUNCTION))) {
      return 'Expected statement type of: interface, class, or function';
    }

    return $statement->signature();
  }

  /**
   * Returns the text of a statement block (without comment).
   *
   * @todo Make comment inclusion a parameter.
   *
   * @param mixed $statements
   *   A node or a list of the statement block.
   * @return string
   *   A string of the statement block.
   */
  public function statementsToText($statements) {
    $this->debugPrint(__METHOD__);
    if (!is_object($statements)) {
      return 'NOT AN OBJECT';
    }
    if ($statements instanceof PGPNode) {
      $statement = $statements->data;
      if (isset($statement->comment)) {
        unset($statement->comment);
      }
      if (isset($statement->parent)) {
        unset($statement->parent);
      }
      $statements = new PGPBody();
      $statements->insertLast($statement);
    }
    elseif (!($statements instanceof PGPList)) {
      return 'Not a list or node';
    }
    return $statements->toString();
  }

  /**
   * Returns a grammar array for a comment.
   *
   * @param string $snippet
   *   A comment string.
   * @return array
   *   A grammar array of the comment.
   */
  public function commentToStatement($snippet = '') {
    if (!in_array(substr($snippet, 0, 2), array('//', '/*'))) {
      $snippet = '// ' . $snippet;
    }
    return array(
      'type' => T_COMMENT,
      'value' => $snippet,
    );
  }

  /**
   * Parses text and returns a grammar expression.
   *
   * The snippet should be a simple expression, but not a complete statement or
   * a statement block. The snippet should not include PHP open or close tags.
   *
   * @param string $snippet
   *   A string of text to parse.
   * @return PGPExpression
   *   A grammar object of the expression.
   */
  public function expressionToStatement($snippet = '') {
    $this->debugPrint(__METHOD__);
    $this->debugPrint($snippet);

    // Add a semi-colon as this should always be safe?
    $snippet = "<?php\n" . $snippet . ";";
    $this->getReader()->buildSnippet($snippet);
    $statements = $this->getReader()->getStatements();
//    $statements->printArray();
//    $this->debugPrint($statements);
    return $statements;
  }

  /**
   * Parses text and returns a statement list.
   *
   * The snippet should include one or more complete statements, or a statement
   * block. The snippet should not include PHP open or close tags.
   *
   * @param string $snippet
   *   A string of text to parse.
   * @return PGPList
   *   A list of grammar statements.
   */
  public function textToStatements($snippet = '') {
    $this->debugPrint(__METHOD__);
    $this->debugPrint($snippet);
    // Add a semi-colon as this should always be safe?
    $snippet = "<?php\n" . $snippet . ";";
    $this->getReader()->buildGrammar($snippet);
    $statements = $this->getReader()->getStatements();
    $statements->delete($statements->get(0));
//    cdp($statements->printList());
//    $this->debugPrint($statements);
    return $statements;
  }

  /**
   * Returns the primary operand of a statement.
   *
   * @param PGPBase $statement
   *   A grammar object of the statement block.
   * @return string
   *   A string of the statement block.
   */
  public function statementOperandToText($statement) {
    if (!$statement) {
      return '';
    }
    switch ($statement->type) {
      case T_INTERFACE:
      case T_CLASS:
      case T_FUNCTION:
        return $statement->name;
        break;

      case T_COMMENT:
        break;

      case T_DOC_COMMENT:
        break;

      case T_WHITESPACE:
        break;

      case T_CONST:
      case T_GLOBAL:
      case T_VAR:
      case T_ASSIGNMENT: // Handles a class property
        // TODO Write the expression here instead of function calls?
        return $this->globalToString($statement);
        break;

      case T_DEFINE:
        return $this->defineToString($statement);
        break;
    }
    return $statement->toString();
  }

  /**
   * Returns comment string stripped of comment delimiters.
   *
   * @param array $statement
   *   An array of the statement block.
   * @return string
   *   A string of the statement.
   */
  public function commentToString($statement = array()) {
    if (!isset($statement['value'])) {
      return '';
    }

    /*
     * This needs to be reworked into a robust Doxygen-style comment parser.
     *
     * api/parser.inc is not intended for interface or class definitions.
     * We need to added expressions for the comment delimiters.
     * This is intended to handle class and interface definitions where the
     * lines may be indented.
     * Remove: 1) delimiting '/**' and '*\/' characters, 2) '*' on other lines.
     * Work from bottom to top.
     * Do not remove all spaces or tabs at the front including indented lines.
     * We need to know the line indention and only remove the spaces (or tabs)
     * corresponding to the indention.
     *
     * For now (not documenting any interfaces or classes), as last step, only
     * delete one space on each new line.
     */

    $string = preg_replace(array("!\*/!", "!\n[\t ]+\*!", "!/\*\*[\t ]*\n!", "!/\*\*!", "!\n !"), array("", "\n", "", "", "\n"), $statement['value']);
    return $string;
  }

  /**
   * Returns the operand of a define statement.
   *
   * @param PGPFunctionCall $statement
   *   A grammar object of the statement.
   * @return string
   *   A string of the operand.
   */
  public function defineToString($statement) {
    if (is_object($statement) && get_class($statement) == 'PGPFunctionCall' && $statement->type == T_DEFINE) {
      $x_define = $statement->getParameter()->toString(); // Eclipse chokes on the variable named define???
      return str_replace(array('"', "'"), array('', ''), $x_define);
    }
    return '';
  }

  /**
   * Returns the operand of a global statement.
   *
   * @param PGPAssignment $statement
   *   A grammar object of the statement.
   * @return string
   *   A string of the operand.
   */
  public function globalToString($statement) {
    if (is_object($statement) && get_class($statement) == 'PGPAssignment' && in_array($statement->type, array(T_CONST, T_GLOBAL, T_VAR, T_ASSIGNMENT))) {
      $assign_variable = $statement->values->getElement()->getElement()->toString();
      $this->debugPrint("assign_variable = $assign_variable in " . __FUNCTION__);
      return str_replace(array('"', "'"), array('', ''), $assign_variable);
    }
    return '';
  }

  /**
   * Returns the statement type as a string.
   *
   * @param PGPBase $statement
   *   A grammar object of the statement.
   * @return string
   *   The statement type string.
   */
  public function statementTypeToString($statement) {
    if (isset($statement->type)) {
      return PGPWriter::statementTypeToString($statement->type);
    }
    return '';
  }

  /**
   * Replaces statement parameters with the grammar equivalent of snippets.
   *
   * @param PGPFunctionCall $statement
   *   A function call statement object.
   * @param string $snippets
   *   An array of text strings to parse.
   */
  public function setParameters(&$statement, $snippets = array()) {
    if (!in_array(get_class($statement), array('PGPClass', 'PGPFunctionCall'))) {
      return;
    }
    if (isset($statement->parameters)) {
      $statement->parameters->clear();
    }
    $index = 0;
    foreach ($snippets as $snippet) {
      $this->setParameter($statement, $index, $snippet, FALSE);
      $index++;
    }
  }

  /**
   * Sets a statement parameter to the grammar equivalent of snippet.
   *
   * @param PGPBase $statement
   *   The interface, class. function, or function call statement object.
   * @param integer $index
   *   The index in the parameter array.
   * @param string $snippet
   *   A string of text to parse.
   */
  public function setParameter(&$statement, $index = 0, $snippet = '') {
    $this->debugPrint(__METHOD__);

    if (in_array(get_class($statement), array('PGPClass', 'PGPFunctionCall'))) {
      $statement->setParameter($index, $this->expressionToStatement($snippet));
    }
  }

  /**
   * Inserts a snippet as the statement parameter at a specified index.
   *
   * @param PGPBase $statement
   *   The interface, class. function, or function call statement object.
   * @param integer $index
   *   The index in the parameter array.
   * @param string $snippet
   *   A string of text to parse.
   */
  public function insertParameter(&$statement, $index = 0, $snippet = '') {
    $this->debugPrint(__METHOD__);

    if (in_array(get_class($statement), array('PGPClass', 'PGPFunctionCall'))) {
      $statement->insertParameter($index, $this->expressionToStatement($snippet));
    }
  }

  /**
   * Array-itizes the parameters of an object, removing any default values.
   *
   * @param object $item
   *   A PGPClass or PGPFunctionCall object.
   * @param integer $first
   *   The index of the first parameter to include in the array.
   *   (This assumes all subsequent parameters are included.)
   * @param array $keys
   *   The associative keys of the new parameter array.
   * @param array $defaults
   *   The respective default values.
   * @return string
   *   A string of the parameter array.
   */
  public function arrayitize(&$item, $first, $keys, $defaults) {
    $count = $item->parameters->count();
    $string = 'array()';
    $strings = array();
    for ($index = $count - 1; $index >= $first; $index--) {
      if ($count > $index) {
        $value = $item->printParameter($index);
        $index2 = $index - $first;
        if ($value != "{$defaults[$index2]}") {
          $params[$keys[$index2]] = $value;
          $strings[] = "'$keys[$index2]' => $value";
        }
        $item->deleteParameter($index);
      }
      $string = 'array(' . implode(', ', array_reverse($strings)) . ')';
      if (isset($params)) {
        coder_upgrade_debug_print('params');
        coder_upgrade_debug_print(print_r($params, 1));
      }
      coder_upgrade_debug_print('string');
      coder_upgrade_debug_print($string);
    }
    return $string;
  }

  /**
   * Removes default parameters from right to left.
   *
   * @param object $item
   *   A PGPClass or PGPFunctionCall object.
   * @param integer $first
   *   The index of the first parameter to include in the array.
   *   (This assumes all subsequent parameters are included.)
   * @param array $defaults
   *   The respective default values.
   */
  public function removeDefaults(&$item, $first, $defaults) {
    $count = $item->parameters->count();
    for ($index = $count - 1; $index >= $first; $index--) {
      if ($count == $index + 1) {
        $value = $item->printParameter($index);
        $index2 = $index - $first;
        if ((is_array($defaults[$index2]) && in_array($value, $defaults[$index2])) || (!is_array($defaults[$index2]) && $value == $defaults[$index2])) {
          $item->deleteParameter($index);
          $count = $item->parameters->count();
        }
      }
    }
  }
}
