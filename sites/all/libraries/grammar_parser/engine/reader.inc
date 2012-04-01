<?php

/**
 * @file
 * A member of the Grammar Parser API classes. These classes provide an API for
 * parsing, editing and rebuilding a code snippet.
 *
 * Copyright 2009 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Grammar Parser token reader class.
 *
 * This class provides an API for parsing the statements from a code snippet
 * and returning a structured object based on the grammar of the programming
 * language. The code snippet may be a single line, a function, or an entire
 * file.
 *
 * @example Create a grammar object from a code file.
 * @code
 *   $source = file_get_contents($filename);
 *   $reader = new PGPReader($source);
 *   // Optionally, add token names for readability of the array elements.
 *   $reader->addTokenNames();
 *   $reader->buildGrammar();
 *   $grammar = $reader->getStatements();
 *   // Do something with $grammar.
 * @endcode
 *
 * @example Create a grammar object from a line of code.
 * @code
 *   // Important to enclose with a PHP open (and optionally a close) tag.
 *   $source = "<?php\nfile_get_contents($filename)\n?>";
 *   $reader = new PGPReader();
 *   $reader->buildGrammar($source);
 *   $grammar = $reader->getStatements();
 *   // Do something with $grammar.
 * @endcode
 *
 * This reader will not handle a code snippet that does not start with a php
 * open tag. Also, template files with inline HTML and multiple php open and
 * close tags are not supported.
 *
 * Tokens not handled:
 * - [311] => T_INLINE_HTML (this one messes up the flow)
 * - [312] => T_CHARACTER
 * - [313] => T_BAD_CHARACTER (anything below ASCII 32 except \t (0x09), \n (0x0a) and \r (0x0d))
 * - [339] => T_USE (namespace-related)
 * - [351] => T_HALT_COMPILER (__halt_compiler())
 *
 * - [xxx] => T_NS_C (Also defined as T_NAMESPACE (available since PHP 5.3.0))
 *
 * @link http://php.net/manual/en/tokens.php
 *
 * Grammar elements not handled:
 * - nop (no operation, just a semi-colon)
 * - a '`' character (execute code)
 *
 * [Non-standard] Formatting that is modified:
 * - array items inline as opposed to multi-line
 *   - not Drupal standard or standard is unclear
 *   - inline array code is retained if $preserveArrayFormat == TRUE
 * - array done inline in code (not a function or function call)
 *   - not Drupal standard or standard is unclear
 * - no comma after last array element (not Drupal standard) (adds a comma)
 * - whitespace before a ';' (whitespace is removed)
 * - a statement enclosed in parentheses, e.g. "($var = $xx - $yy);"
 *   - parentheses are retained (DELETE)
 * - a one-line block not enclosed in braces, e.g. "if (!$data) continue;"
 *   - braces and new lines are added
 * - a nop block (i.e. ';'), e.g. "if (!$data);"
 *   - braces and new lines are added
 * - no whitespace around operators (a space is added)
 * - braces '{}' used to indicate index of string variable (e.g. $string{0})
 *   - braces are replaced by brackets '[]'
 * - in a statement block outside of its parenthesized element or body
 *   - additional whitespace is removed
 *   - inline comments are dropped
 *   - function /*goodbye*\/ foo() {}
 *   - if /*hello*\/(TRUE) {}
 */
class PGPReader extends PGPParser {

  /*
   * Singleton instance of this class.
   */
  private static $reader;

  /**
   * Count of expression depth during recursive calls to buildExpression.
   *
   * Helpful when debugging.
   *
   * @var integer
   */
  private $expressions = 0;

  /**
   * Whether to build an array inline or on multiple lines.
   *
   * @var boolean
   */
  private $isMultiline = TRUE; // Deprecated.

  /**
   * Whether the current item is an interface definition.
   *
   * @var boolean
   */
  private $isInterface = FALSE;

  /**
   * Whether the current item is an inline if expression.
   *
   * @var boolean
   */
  private $isInlineIf = FALSE; // Used only once.

  /**
   * Whether the current item is part of a function (call) parameter list.
   *
   * @var boolean
   */
  private $inFunctionOrCall = FALSE;

  /**
   * Whether to save whitespace information with the current item.
   *
   * @var boolean
   */
  private $addWhitespace = FALSE;

  /**
   * Whether to shift newline to whitespace token following a comment.
   *
   * @var boolean
   */
  private $addNewLineToWhitespace = FALSE;

  /**
   * Whether to duplicate the input array format.
   *
   * @var boolean
   */
  private $preserveArrayFormat = FALSE;

  /**
   * The current unary operator.
   *
   * @var string
   */
  private $unary = '';

  /**
   * The current reference operator.
   *
   * @var string
   */
  private $reference = '';

  /**
   * The current warning suppression operator.
   *
   * @var string
   */
  private $suppress = '';

  /**
   * The current indirect reference operator.
   *
   * @var string
   */
  private $indirect = '';

  /**
   * Whether the previous token includes a newline character.
   *
   * @var boolean
   */
  private $previousTokenIncludesNewLine = FALSE;

  /**
   * Comment string.
   *
   * @var string
   */
  private $comment = array();

  /**
   * A stack of parent (i.e. PGPNode object) statement references.
   *
   * @var array
   */
  private $parents = array();

  /**
   * A stack of parent expression (i.e. PGPExpression object) references.
   *
   * @var array
   */
  private $parentExpressions = array();

  /**
   * Whether to reclassify the current expression to a function call.
   *
   * @var unknown_type
   */
  private $buildFunctionCall = FALSE; // TODO This is the same name as a function - change this.

  /**
   * Class constructor.
   *
   * @param string $snippet
   *   A code snippet string.
   */
  public function __construct($snippet = NULL) {
    parent::__construct();

    $this->setSnippet($snippet);
  }

  /**
   * Perform any necessary initialization.
   */
  protected function initValues() {
    parent::initValues();
  }

  /**
   * Returns singleton instance of this class.
   *
   * @return PGPReader
   */
  public static function getInstance() {
    if (!self::$reader) {
      self::$reader = new PGPReader();
    }
    return self::$reader;
  }

  /**
   * @defgroup pgp_builder Grammar parser builder functions
   * @{
   */

  /**
   * Builds grammar statements from code snippet tokens.
   *
   * The tokenizer expects a snippet starting with a PHP open tag.
   * The open tag is included in the generated grammar statements.
   *
   * @param string $snippet
   *   A code snippet string.
   */
  public function buildGrammar(&$snippet = NULL) {
    if ($snippet) {
      $this->setSnippet($snippet);
    }
    if (empty($this->tokens)) {
      $this->statements = new PGPBody();
      return;
    }

    // Global items.
    $this->resetOthers();

    /*
     * TODO
     * Can we eliminate this switch statement by using a while(current()) { ... next() } format?
     * There is not a way to always do while (next($this->tokens)) and get the first token.
     * Changing to current will require revisions to several methods that now do while (next($this->tokens)).
     *
     * In a template file, the first item may not be php code. Don't use the parser on those files!!!
     */

    // Process first token.
    reset($this->tokens);

//    $this->statements = $this->buildBody();
    switch ($this->tokenType()) {
      case T_OPEN_TAG:
      case T_OPEN_TAG_WITH_ECHO:
        // The open tags include a new line character.
        // For now, we do not handle multiple open and close tags. So we can
        // eliminate the new line character to make our printing simpler.
        $token = $this->cleanToken();
        $token['value'] = str_replace("\n", '', $token['value']);
        // If followed by T_WHITESPACE, then add the number of new line characters.
        $first_statement = $token;
        $this->previousTokenIncludesNewLine = ($this->nextTokenType() == T_WHITESPACE);

        // Set the parent object reference for recursive calls.
        $parent = NULL;
        $this->parents[] = &$parent; // JB 2010-06-03
//        $this->parentExpressions[] = &$parent;

        // Build grammar statements.
        $this->statements = $this->buildBody();
        $this->statements->insertFirst($first_statement);
        break;
/*
      case T_INLINE_HTML: // Only intended for info files.
        $token = $this->cleanToken();
        $first_statement = $token;

        // Set the parent object reference for recursive calls.
        $parent = NULL;
        $this->parents[] = &$parent;
//        $this->parentExpressions[] = &$parent;

        // Build grammar statements.
        $this->statements = $this->buildBody();
        $this->statements->insertFirst($first_statement);
        break;
*/
      default:
        // The file is not valid.
        // If first token is not T_OPEN_TAG, then return error?
        break;
    }
  }

  /**
   * Builds grammar statements from code snippet tokens.
   *
   * This function enables the editor to parse expressions on the fly.
   * The snippet needs to have a PHP open tag for the tokenizer to work.
   * The open tag is not included in the generated grammar statements.
   * Thus, the open tag is swallowed (i.e. not returned) by this function.
   *
   * @param string $snippet
   *   A code snippet string.
   */
  public function buildSnippet(&$snippet = NULL) {
    if ($snippet) {
      $this->setSnippet($snippet);
      $this->debugPrint($this->tokens);
    }
    if (empty($this->tokens)) {
      return;
    }

    // Global items.
    $this->resetOthers();

    // Process tokens.
    reset($this->tokens);
    $this->statements = $this->buildExpression(array(';'));
  }

  /**
   * Builds grammar statements from code snippet tokens.
   *
   * This function only parses statements of a specific type (currently only
   * functions) whose name is in the list of values.
   *
   * The snippet needs to have a PHP open tag for the tokenizer to work.
   * The open tag is not included in the generated grammar statements.
   * Thus, the open tag is swallowed (i.e. not returned) by this function.
   *
   * @param string $snippet
   *   A code snippet string.
   */
  public function buildSpecific($type, $values, &$snippet = NULL) {
    if (!$type) {
      return;
    }
    if ($snippet) {
      $this->setSnippet($snippet);
      $this->debugPrint($this->tokens);
    }
    if (empty($this->tokens)) {
      return;
    }

    // Global items.
    $this->resetOthers();

    // Process tokens.
    reset($this->tokens);

    // Local items.
    $body = new PGPBody();
    $modifiers = array();

    // Loop on tokens.
    while (next($this->tokens) !== FALSE) { // while (current($this->tokens) !== FALSE) {
      if ($this->tokenType() != $type) {
        continue;
      }
      // Build expression based on token type.
      switch ($this->tokenType()) {
        case T_FUNCTION:
          $next_token = $this->nextRealToken('value');
          if (in_array($next_token, $values)) {
            // Create an empty node as parent object reference for recursive calls.
            $dummy = NULL;
            $node = &$body->insertLast($dummy);
            $node->line = $this->tokenLine();

            $node->data = $this->buildFunction($modifiers);
            $this->functions[] = &$node; // Store node not data so we can insert statements around the function.
            $modifiers = array();
          }
          break;
      }
    }

    $this->statements = $body;
  }

  /**
   * Builds a list of body statements.
   *
   * This routine is called recursively from the other builder routines:
   * class, function, conditional, for, foreach, case, etc.
   *
   * @param array $return_tokens
   *   An array of tokens marking the end of the expression.
   * @return PGPList
   *   A list of grammar statements.
   */
  private function buildBody($return_tokens = array(), $statements_to_return = 0) {
    $this->debugPrint(__FUNCTION__);
    $this->debugPrint($return_tokens);

    // Local items.
    $body = new PGPBody();
    $previousStatementIsComment = FALSE;
    $modifiers = array();
    $statements = 0;

    // Set the list parent to the current parent node.
    $body->parent = &$this->currentParent();

    // Loop on tokens.
    while (next($this->tokens) !== FALSE) { // while (current($this->tokens) !== FALSE) {
      $this->debugPrint("statements_to_return = $statements_to_return in buildBody");
      $this->debugPrint("while buildBody");
      $this->printToken();

      // Return if a return token is encountered.
      if (in_array($this->tokenType(), $return_tokens) || ($statements_to_return && $statements == $statements_to_return)) {
        $this->debugPrint("checking for appended comment before leaving buildBody");
        $this->debugPrint($this->nextRealToken('type', TRUE));
        $this->debugPrint($this->nextTokenType());
        $this->debugPrint($this->nextTokenValue());
        if (in_array($this->tokenType(), array('}', ';')) && $this->nextRealToken('type', TRUE) == T_COMMENT && $this->nextTokenType() == T_WHITESPACE && strpos($this->nextTokenValue(), "\n") === FALSE) {
          // Include an appended comment.
//           substr_count($this->previousTokenValue(), "\n");
          $statements--;
        }
        else {
          $this->debugPrint("leaving buildBody");
          $this->printToken();
          prev($this->tokens);
          array_pop($this->parents);
          return $body;
        }
      }
      // Add this for stacked regular comments.
      if ($this->tokenType() != T_COMMENT) {
        $previousStatementIsComment = FALSE;
      }

      // Create an empty node as parent object reference for recursive calls.
      $dummy = NULL;
      $node = &$body->insertLast($dummy);
      $node->line = $this->tokenLine();
      $this->parents[] = &$node; // JB 2010-06-03

      // Build expression based on token type.
      switch ($this->tokenType()) {
        case T_WHITESPACE:
          // We will automatically add a blank line in most situations.
          // Only save a whitespace token if more than one blank is present.
          // But subtract one (unless a blank is included in previous token).
          $blankLines = substr_count($this->tokenValue(), "\n");
          if ($blankLines > 0 && !$this->previousTokenIncludesNewLine) {
            $blankLines--;
          }
          if ($blankLines > 0) {
            $node->data = array(
              'type' => $this->tokenType(),
              'value' => $blankLines,
            );
          }
          $this->previousTokenIncludesNewLine = FALSE;
          // Add this for stacked regular comments.
          if ($this->previousTokenType() == T_COMMENT) {
            $previousStatementIsComment = TRUE;
          }
          break;

        case T_STRING:
//          echo "HITTING T_STRING!!!\n";
          // TODO This would incorrectly classify a class method named define.
          // To catch this, check for existence of modifiers and in a function.
          $is_define = $this->tokenValue() == 'define';
          if ($is_define) {
            $node->data = $this->buildFunctionCall();
            $node->data->type = T_DEFINE;
            $this->defines[] = &$node->data;
          }
          else {
            // Class references like parent are good examples of places where
            // setting the statement type to 'function call' would be helpful.
            $next_token = $this->nextRealToken();
            if ($next_token == '(') {
              prev($this->tokens);
              $node->data = $this->buildAssignment($modifiers);
              $this->functionCalls[] = &$node/*->data*/;
            }
            else { // TODO Added 2010-01-27
              prev($this->tokens);
              $node->data = $this->buildAssignment($modifiers);
            }
          }
          break;

        case T_VARIABLE:
          // This could be an array
          $next_token = $this->nextRealToken();
          if ($next_token == '(') {
            $node->data = $this->buildFunctionCall();
            $this->functionCalls[] = &$node/*->data*/;
          }
          /*
           * As of PHP 5.3.0, it's possible to reference the class using a variable.
           * The variable's value can not be a keyword (e.g. self, parent or static).
           */
          else {
            prev($this->tokens);
            $node->data = $this->buildAssignment($modifiers);
            $modifiers = array();
            $this->debugPrint("LEAVING case T_VARIABLE after buildAssignment call from buildBody");
          }
          break;

        case T_DO:
        case T_IF:
        case T_ELSEIF:
        case T_ELSE:
        case T_WHILE:
        case T_SWITCH:
          $node->data = $this->buildConditional();
          break;

        case T_CASE:
        case T_DEFAULT:
          $node->data = $this->buildCase();
          break;

        case T_FOR:
          $node->data = $this->buildFor();
          break;

        case T_FOREACH:
          $node->data = $this->buildForEach();
          break;

        case T_COMMENT:
          // If previous token is ';' or whitespace without a return,
          // then the comment is appended to the line.
          // TODO Could also be a ',' in an array expression.
          $append = 0;
          if ($this->previousTokenType() == ';') {
            $append = 1;
          }
          elseif ($previousStatementIsComment) {
            ;
          }
          elseif ($this->previousTokenType() == T_WHITESPACE) {
            $blankLines = substr_count($this->previousTokenValue(), "\n");
            if ($blankLines == 0) {
              $append = 1;
            }
          }
          // A comment only includes a newline if its style is '//' or '#'.
          $node->data = array(
            'type' => $this->tokenType(),
            'value' => $this->cleanComment($this->tokenValue()),
            'append' => $append,
          );
          if ($this->includesNewline($this->tokenValue()) && $this->nextTokenType() == T_WHITESPACE) {
            $this->previousTokenIncludesNewLine = TRUE;
          }
          break;

        case T_ML_COMMENT:
        case T_DOC_COMMENT:
          // NOTE: A regular comment (with a double slash) includes a new line character.
          // If followed by T_WHITESPACE, then add the number of new line characters.
          // T_WHITESPACE includes the spaces at the beginning of the next line
          // (may only be relevant for code comments that are in first column).
          if (($temp = $this->buildComment()) != array()) {
            $node->data = $temp;
            $this->comments[] = &$node->data;
          }
          break;

        case T_BREAK:
        case T_CONTINUE:
        case T_ECHO:
        case T_RETURN:
        case T_EXIT:
          // TODO Could call buildAssignment like next group of tags.
          // This would replace the 'value' key with 'values'.
          /// We print all 7 tokens using PGPAssignment::toString().
          // TODO All but continue can be used with parentheses.
          // With exit (exit or die), parentheses may not be optional.
          // PHP lists echo and exit under functions in the manual.
          // Echo may have multiple parameters only if surrounded by parentheses.
          // Use function call builder.
          $type = $this->tokenType();
//          $temp = new PGPAssignment();
//          $temp->type = $type;
//          $temp->value = $this->buildExpression(array(';'));
//          $node->data = $temp;
          $node->data = $this->buildFunctionCall(TRUE);
          $node->data->type = $type;
          $this->functionCalls[] = &$node/*->data*/;
          break;

        case T_CONST:
        case T_GLOBAL:
        case T_VAR:
          $type = $this->tokenType();
          $temp = $this->buildAssignment($modifiers);
          $temp->type = $type;
          $node->data = $temp;
          $modifiers = array();
          if ($type == T_GLOBAL) {
            $this->globals[] = &$node->data;
          }
          break;
/*
        case T_BREAK:
          $node->data = $this->cleanToken();
          break;
*/
        case T_INC:
        case T_DEC:
//          $node->setData('operator2', $this->tokenValue());
          $this->unary = $this->tokenValue();
          break;

        case '@': // 2010-01-30
          // Warning suppression operator, e.g. @$$var = function().
          // This is similar to a unary operator as is '&'.
//          if ($this->nextTokenType() == T_VARIABLE) {
            $this->suppress = $this->tokenValue();
//          }
          break;

        case '$':
          // Indirect references, e.g. $$var.
          // This is similar to a unary operator as is '&'.
          // Use nextRealToken() as /**/ could be embedded
          if ($this->nextRealToken() == T_VARIABLE) {
            $this->indirect = $this->tokenValue();
          }
          elseif ($this->nextRealToken() == '{') {
            // Marks the begin and end of an expression.
            prev($this->tokens);
            $node->data = $this->buildAssignment($modifiers);
          }
          break;

        case T_PUBLIC:
        case T_PRIVATE:
        case T_PROTECTED:
        case T_STATIC:
        case T_ABSTRACT:
        case T_FINAL: // Properties cannot be declared final, only classes and methods may be declared as final.
          $modifiers[] = $this->tokenValue();
          break;

        case T_FUNCTION:
          $node->data = $this->buildFunction($modifiers);
          $this->functions[] = &$node; // Store node not data so we can insert statements around the function.
          $modifiers = array();
          break;

        case T_INTERFACE:
          $this->isInterface = TRUE;
        case T_CLASS:
          $node->data = $this->buildClass($modifiers);
          $this->classes[] = &$node;
          $modifiers = array();
          $this->isInterface = FALSE;
          break;

        case T_ENDIF:
        case T_ENDWHILE:
        case T_ENDSWITCH:
        case T_ENDFOR:
        case T_ENDFOREACH:
        case T_ENDDECLARE:
        case '}':
          prev($this->tokens);
          array_pop($this->parents);
          // Remove an empty node created before reading the token.
          if ($node->data == NULL) {
            $body->delete($node);
          }
          return $body;
          break;

        case T_TRY:
        case T_CATCH:
          $node->data = $this->buildTryCatch();
          break;

        case T_REQUIRE:
        case T_REQUIRE_ONCE:
        case T_INCLUDE:
        case T_INCLUDE_ONCE:
          $node->data = $this->buildFunctionCall(TRUE);
          $this->functionCalls[] = &$node/*->data*/;
          break;

        case T_EVAL:
        case T_EMPTY:
        case T_UNSET:
          $node->data = $this->buildFunctionCall();
          $this->functionCalls[] = &$node/*->data*/;
          break;

        case T_PRINT:
        case T_THROW:
          $node->data = $this->buildFunctionCall(TRUE);
          $this->functionCalls[] = &$node/*->data*/;
          break;

        case T_DECLARE:
          $node->data = $this->buildDeclare();
          $modifiers = array();
          break;

        case T_LIST:
          $node->data = $this->buildList();
          break;

        case T_ISSET: // 2010-02-23 Added.
          prev($this->tokens);
          $node->data = $this->buildAssignment($modifiers);
          break;

        case T_NEW: // 2010-03-02 Added. Example: new Exception(..);
          prev($this->tokens);
          $node->data = $this->buildAssignment($modifiers);
          break;

        case T_INLINE_HTML:
          // 2009-06-13 Inline html
//          echo "INLINE HTML in " . __FUNCTION__ . "\n";
//          echo $this->tokenValue() . "\n";
          $node->data = $this->cleanToken();
          break;

        case T_OPEN_TAG:
        case T_OPEN_TAG_WITH_ECHO:
        case T_CLOSE_TAG:
          // The open tags include a new line character.
          // For now, we do not handle multiple open and close tags. So we can
          // eliminate the new line character to make our printing simpler.
          $token = $this->cleanToken();
          $token['value'] = str_replace("\n", '', $token['value']);
          // If followed by T_WHITESPACE, then add the number of new line characters.
          $node->data = $this->cleanToken();
          $this->previousTokenIncludesNewLine = ($this->nextTokenType() == T_WHITESPACE);
          break;

        case '(':
          /*
           * TODO Need a way to handle this. Ex: ($some_var) ? print 'true' : print 'false';
           * Callling buildExpression leaves the toString method hanging as an expression
           * is not expected. Calling buildAssignment leaves that routine up in arms as the
           * left paren signals it to build a function call.
           */
          $this->debugPrint("Hitting left paren in " . __METHOD__);
          prev($this->tokens);
          $node->data = $this->buildAssignment($modifiers); // $node->data = $this->buildExpression(array(';'));
          break;

        // Are there other tokens we should simply swallow!!!
        case '{': // When does this tag hit here???
        case ';':
          $this->debugPrint("Hitting semi-colon in " . __METHOD__);
          break;

        default:
//          $body['warning'] = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
//          return $body;
          break;
      } // end switch

      // Remove an empty node created before reading the token.
      if ($node->data == NULL) {
        $body->delete($node);
      }
      elseif ($statements_to_return) {
        $statements++;
      }

//      next($this->tokens);
    } // end while

    return $body;
  }

  /**
   * Builds a class or interface statement block.
   *
   * @param array $modifiers
   *   An array of modifiers (e.g. public, private) for the statement block.
   * @return PGPClass
   *   An object of the statement block.
   */
  private function buildClass($modifiers) {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $class = new PGPClass();
    $class->comment = $this->comment;
    $class->modifiers = $modifiers;
    $class->type = $this->tokenType();

    // Global items.
    $this->comment = array();

    // Local items.
    $extends = 0;
    $implements = 0;

    while (next($this->tokens) !== FALSE) {
      switch ($this->tokenType()) {
        case T_STRING:
          if ($extends) {
            $class->extends[] = $this->tokenValue();
          }
          elseif ($implements) {
            $class->implements[] = $this->tokenValue();
          }
          else {
            $class->name = $this->tokenValue();
          }
          break;

        case '{':
          $class->bodyComment = $this->buildBodyComment();
          $class->body = $this->buildBody();
          break;

        case '}':
          return $class;
          break;

        case T_EXTENDS:
          // Interfaces may extend multiple interfaces, classes only one class.
          $extends++;
          $implements = 0;
          break;

        case T_IMPLEMENTS:
          $implements++;
          $extends = 0;
          break;

        case ',':
          // Delimits lists of interfaces implemented or classes extended.
          if ($implements) {
            $implements++;
          }
          elseif ($extends) {
            $extends++;
          }
          // else error
          break;

        case T_COMMENT: // TODO Until we can figure a better way to handle embedded comments
        case T_WHITESPACE:
          break;

        default:
          $class->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
          return $class;
          break;
      }
    }
    $class->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $class->extra = $this->cleanToken();
    return $class;
  }

  /**
   * Builds a function statement block.
   *
   * @param array $modifiers
   *   An array of modifiers (e.g. public, private) for the statement block.
   * @return PGPClass
   *   An object of the statement block.
   */
  private function buildFunction($modifiers) {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $function = new PGPClass();
    $function->comment = $this->comment;
    $function->modifiers = $modifiers;
    $function->type = $this->tokenType();
    $function->parameters = new PGPList();

    // Global items.
    $this->comment = array();

    while (next($this->tokens) !== FALSE) {
      switch ($this->tokenType()) {
        case T_STRING:
          $function->name = $this->tokenValue();
          break;

        case '(':
        case ',':
          $this->inFunctionOrCall = TRUE;
          $parameter = $this->buildExpression(array(',', ')'));
          if (!$parameter->isEmpty()) {
            $function->parameters->insertLast($parameter);
          }
          $this->inFunctionOrCall = FALSE;
          break;

        case ')':
          $this->debugPrint("Hitting ) in while buildFunction");
          break;

        case '{':
          $function->bodyComment = $this->buildBodyComment();
          $function->body = $this->buildBody();
          break;

        case '}':
          return $function;
          break;

        case '&':
          $function->reference = $this->tokenValue();
          break;

        case ';':
          if ($this->isInterface || in_array('abstract', $modifiers)) {
            return $function;
          }
          $function->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
          break;

        case T_COMMENT: // TODO Until we can figure a better way to handle embedded comments
        case T_WHITESPACE:
          break;

        default:
          $function->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
          return $function;
          break;
      }
    }
    $function->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $function->extra = $this->cleanToken();
    return $function;
  }

  /**
   * Builds a function call (or other) expression.
   *
   * The other expressions built by this routine are: clone, [define,] empty,
   * eval, include, print, require, throw, or unset. Parentheses are not
   * required by an include, print, require, or throw expression. Define
   * expressions may have a T_DOC_COMMENT.
   *
   * A function call may occur in several places, such as:
   * - a separate statement
   * - a parameter in another function call
   * - in an assignment statement
   * - in a return statement.
   *
   * @todo Should next parameter be $parens_are_optional described as:
   *   Indicates whether parentheses are optional around parameters.
   *   We only set the parameter to TRUE on include, print, require, or
   *   throw expressions.
   * @param boolean $require
   *   Indicates whether parentheses are required around parameters.
   * @param array $expression
   *   A partial array of the expression from another builder routine.
   * @return array
   *   An array of the expression.
   */
  private function buildFunctionCall($require = FALSE, $expression = array()) {
    $this->debugPrint(__FUNCTION__);

    if (!$expression) {
      // Set the item order so we can use case block when printing.
      $parent = &$this->currentParent();

      // Create object.
      $expression = new PGPFunctionCall();
      $expression->parent = &$parent;
      $expression->comment = $this->comment;
      $expression->type = T_FUNCTION_CALL;
      $expression->name = $this->cleanToken();
      $expression->noparens = 0;
      $expression->parameters = new PGPList();
//      $expression->expression = array(); // Deprecated.

      // TODO This code is now in 3 places - refactor.
      if ($this->suppress != '' || $this->reference != '') {
        // Create object.
        $name = new PGPOperand();
        if ($this->suppress != '') {
          // Ex: @parent::function();
          $name->insertLast($this->suppress, 'suppress');
          $this->suppress = '';
        }
        if ($this->reference != '') {
          // Ex: &parent::$tokens
          $name->insertLast($this->reference, 'reference');
          $this->reference = '';
        }
        $name->insertLast($this->tokenType(), 'type');
        $name->insertLast($this->tokenValue(), 'value');
        $expression->name = $name;
      }
    }
    $expression->noparens = 1;

    // Global items.
    $this->comment = array();

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildFunctionCall");
      $this->printToken();
      switch ($this->tokenType()) {
        case T_WHITESPACE:
          // Intentionally bleed into next case for a require statement.
          // TODO There could be whitespace even with parentheses.
          // Check for next non-comment and non-whitespace token.
          if (!$require || !$expression->parameters->isEmpty()) {
            break;
          }
//          $expression->noparens = 1;

        case '(':
          if ($this->tokenType() == '(') {
            $expression->noparens = 0; // 2010-02-08
          }
        case ',':
          $this->inFunctionOrCall = TRUE;
          $return_tokens = array(',', ';', T_CLOSE_TAG);
          if ($this->isInlineIf && $expression->noparens) {
            // Example: $question ? print "foo" : bar();
            $return_tokens += array(3 => ':'); // @todo Use array_merge
          }
          $parameter = $this->buildExpression($return_tokens); // $parameter = $this->buildExpression(array(',', ';', T_CLOSE_TAG)); // 2009-06-13 Inline html
          if (!$parameter->isEmpty()) {
            $expression->parameters->insertLast($parameter);
          }
          $this->inFunctionOrCall = FALSE;
          break;

        case ')':
          return $expression;
          break;

        case ':':
          // This applies when an inline if expession has a "function" call without parentheses, e.g. print "foo"
        case ';': // For a require.
          $this->debugPrint("Hitting ; in while buildFunctionCall");
          $this->debugPrint($expression);
          prev($this->tokens); // Try to catch semi-colons in buildBody!!!
          return $expression;
          break;

        case T_COMMENT:
          // An inline comment outside of the parenthesized element or body.
          // Drop the comment.
          break;

        case T_CLOSE_TAG:
          // 2009-06-13 Inline html
          $expression->inline = 1; // TODO This property is not defined in the class.
          prev($this->tokens);
          return $expression;
          break;

        default:
          $expression->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
          $expression->extra = $this->cleanToken();
          return $expression;
          break;
      }
    }
    $expression->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $expression->extra = $this->cleanToken();
    return $expression;
  }

  /*
   * 2009-09-16
   * Create a new expression for the function call so hopefully if it has operator items
   * these are just part of the expression. The function call is the first item of the
   * expression. This differs from current approach where the operator expression is
   * added to the function call as its "expression."
   *
   * Basically, each operand = array() should become a PGPExpression. This way, a
   * complicated expression can be encapsulated in an expression object. The operand
   * expression would include the indices in an array and the object operators.
   * The "pieces" of the big expression are delimited by operators. In this sense,
   * the double colon and object operator are not operators.
   *
   * How to control the nesting of expressions?
   *
   * Example: $condition->condition('table_schema', $schema);
   * This is one function call. We would initially identify this as an assignment:
   * - operand = $condition
   * - operator2 = ->
   * - operand = condition
   * Then determine a function call where the function name is the complex expression
   * already realized.
   * So statement changes from PGPAssignment to PGPFunctionCall and
   * - type = 601
   * - name = previous PGPAssignment expression
   * - parameters = PGPList as usual.
   *
   * Example: $options += $this->defaultOptions();
   * This is an assignment and is initially identified as such.
   * - operand = PGPExpression
   *   - operand = $options
   *   - index
   *
   * Example: $schema = $info['default']['database'];
   * This is an assignment and is initially identified as such.
   * - operand = PGPExpression
   *   - operand = Array
   *     - type = T_VARIABLE
   *     - value = $schema
   * - assign = =
   * - operand = PGPExpression
   *   - operand = Array
   *     - type = T_VARIABLE
   *     - value = $info
   *     - index = PGPExpression
   *       - operand = Array
   *         - type = T_CONSTANT_ENCAPSED_STRING
   *         - value = 'default'
   *     - index = PGPExpression
   *       - operand = Array
   *         - type = T_CONSTANT_ENCAPSED_STRING
   *         - value = 'database'
   *
   * Elements beneath PGPExpression need to be consistently tagged as
   * operand and operator(2). Thus, the above need to have more levels.
   *
   * Example: $schema = $info['default' . $x][$database + $schema] + $var ...;
   * This is an assignment and is initially identified as such.
   * - operand = PGPExpression
   *   - operand = Array
   *     - type = T_VARIABLE
   *     - value = $schema
   * - assign = =
   * - operand = PGPExpression (call this PGPOperand and don't require operand type beneath it)
   *   - operand = Array (DELETE THIS LEVEL)
   *     - type = T_VARIABLE
   *     - value = $info
   *     - index = PGPExpression
   *       - operand = Array
   *         - type = T_CONSTANT_ENCAPSED_STRING
   *         - value = 'default'
   *       - operator = .
   *       - operand = Array
   *         - type = T_VARIABLE
   *         - value = $x
   *     - index = PGPExpression
   *       - operand = Array
   *         - type = T_CONSTANT_ENCAPSED_STRING
   *         - value = 'database'
   *       - operator = +
   *       - operand = Array
   *         - type = T_VARIABLE
   *         - value = $schema
   * - operator = +
   * - operand = PGPExpression
   *   - operand = Array
   *     - type = T_VARIABLE
   *     - value = $info
   *
   * The above expression would be in values[0] of the PGPList for the assignment.
   * - statement = PGPAssignment
   * - type = 602
   * - values = PGPList of PGPExpression items
   *
   * This arrangement feels right and would line up with a BNF grammar for PHP.
   */

  /**
   * Builds an expression.
   *
   * This routine is called recursively from most routines. It is the primary
   * workhorse for building expressions from the tokens.
   *
   * Setting the array of return tokens allows us to fine tune its operation.
   * The return tokens may either mark the end of the pattern or the start of
   * a new pattern.
   *
   * Examples: with expressions, when trying to collect an operand, we look for
   * operators which start a new pattern. With functions and arrays, we look
   * for a right parenthesis which marks the end of the pattern. This routine
   * needs to know when to back up the token array pointer (because we found
   * the start of a new pattern) or to swallow the token (at end of pattern).
   *
   * This routine automatically returns when it encounters an unbalanced right
   * parenthesis, bracket, or brace.
   *
   * @param array $return_tokens
   *   An array of tokens marking the end of the expression.
   * @param PGPExpression (or descendant most likely PGPOperand) $expression
   *   An expression to append the new items to.
   *   A partial expression from another builder routine.
   * @return PGPExpression
   *   A grammar object of the expression.
   */
  private function buildExpression($return_tokens, $expression = NULL) {
    // Use local variable set to incremented value of global variable.
    $expressions = ++$this->expressions;

    $this->debugPrint(__FUNCTION__ . " $expressions");
//    $this->debugPrint($return_tokens);

    // Create object.
    if (is_null($expression)) {
      $expression = new PGPExpression();
      // Set the list parent to the current parent node.
//      $expression->parentExpression = &$this->currentParentExpression();
      // Add this expression to the list.
//      $this->parentExpressions[] = &$expression;
    }

    // Local items.
    // Whether a left parentheses indicates a function call expression.
    $isLeftParenPartOfExpr = TRUE;

    // Initialize counters related to the current expression.
    $lparens = 0;
    $rparens = 0;
    $lbrackets = 0;
    $rbrackets = 0;
    $lbraces = 0; // NOT set
    $rbraces = 0; // NOT set

    // Loop on tokens.
    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildExpression $expressions");
      $this->printToken();

      // Return if a return token is encountered.
      if (in_array($this->tokenType(), $return_tokens)) {
        $this->debugPrint("testing leave from buildExpression $expressions");
        $this->debugPrint("lparens = $lparens <=> rparens = $rparens <=> lbrackets = $lbrackets <=> rbrackets = $rbrackets <=> lbraces = $lbraces <=> rbraces = $rbraces");
        // If ')' then process it (so added to array) and test after switch.
        // Otherwise test for unbalanced parens.
        if (!in_array($this->tokenType(), array(']', ')', '}')) && ($rbrackets == $lbrackets) && ($rparens == $lparens) && ($rbraces == $lbraces)) {
          $this->debugPrint("leaving buildExpression $expressions");
          $this->printToken();
          $this->debugPrint($expression);
          prev($this->tokens);
          $this->debugPrint("PREVIOUS in buildExpression $expressions");
//          array_pop($this->parentExpressions); // TODO The right spot???
          return $expression;
        }
      }

      // Create an empty node as parent object reference for recursive calls.
      $dummy = NULL;
      $node = &$expression->insertLast($dummy);
      $node->line = $this->tokenLine();
//      $this->parents[] = &$node;

      // Build expression based on token type.
      switch ($this->tokenType()) {
        case T_COMMENT:
          // TODO If code is poorly formatted, then there might not be whitespace around any tokens.
          // It is fairly pointless to check only around comments!
          // TODO Could insert whitespace using $expression->insertBefore($current, $data, $type)
          if ($this->previousTokenType() != T_WHITESPACE) {
            $node->setData('whitespace', '');
            // Create an empty node as parent object reference for recursive calls.
            $dummy = NULL;
            $node = &$expression->insertLast($dummy);
//            $this->parents[] = &$node;
          }
          /*
           * Remove newline from EOL comment and add it to the whitespace
           * token that follows, or add one if it doesn't.
           */
          // A comment only includes a newline if its style is '//' or '#'.
          $comment = array(
            'type' => $this->tokenType(),
            'value' => $this->cleanComment($this->tokenValue()),
//            'append' => $append
          );
          $node->setData('comment', $comment);
          if ($this->includesNewline($this->tokenValue())) {
            if ($this->nextTokenType() == T_WHITESPACE) {
              $this->addNewLineToWhitespace = TRUE;
            }
            else {
              $data = "\n";
              $expression->insertLast($data, 'whitespace');
            }
          }
          break;

        case T_WHITESPACE:
          if ($this->addNewLineToWhitespace) {
            // Transfer newline from end-of-line comment to whitespace.
            $this->addNewLineToWhitespace = FALSE;
            $token = &$this->tokens[key($this->tokens)];
            $token[1] = "\n" . $token[1];
          }
          // 2009-06-16 add this block
          // We need to return once and then add the non-standard whitespace to the expression.
          // The issue is probably due to two ways of adding following tags:
          // With array keys, we do not store an element for the => character, automatically adding it after the key.
          // With other characters (assign and operator), we have an element for the character.
          // If we added one for the =>, then this issue might be handled consistently.
          // This makes whitespace a separate element.
          if ($this->tokenValue() != ' ' && !$this->addWhitespace) {
            // Can we return only once and the next time add the whitespace???
            $test = in_array($this->nextRealToken(), array_merge($this->assignReturnTokens(), $this->operatorReturnTokens(), array(')')));
            if ($test && get_class($expression) == 'PGPOperand') {
              $this->addWhitespace = TRUE;
              $this->debugPrint("setting add_whitespace");
//              print_r($expression);
              $expression->delete($node);
              prev($this->tokens);
//              array_pop($this->parentExpressions); // TODO The right spot???
              return $expression;
            }
          }
          $this->addWhitespace = FALSE;
          // TODO If previous token is a regular comment, then we need to get the line spacing from it
          // TODO Not consistent on storing whitespace, only do on assignments???
          if ($this->tokenValue() != ' ' ||
             ($this->previousTokenType() == '(' || $this->nextTokenType() == ')')) {
            $node->setData('whitespace', $this->tokenValue());
//            echo "added whitespace\n";
          }
          break;

        case T_STRING:
          // T_STRING can be function call or name after an object operator.
          // After an object operator, it could have indices inside '[]'.
          // A function name could be followed by white space or comment ('/* */').
          if ($this->nextRealToken() == '(') {
            $this->debugPrint("ABOUT TO FORK TO FUNCTION CALL");
            $this->debugPrint($expression);
            // TODO Remove this error catching if!!
            if (is_object($expression->first()->data)) { // if (is_a($expression->first()->data, 'PGPOperand')) {
//              $this->debugPrint("ABOUT TO FORK TO FUNCTION CALL");
//              $this->debugPrint($expression);
            }
            elseif ($expression->first()->data == T_VARIABLE) {
              // This finds $this in $options += $this->defaultOptions(); even though we
              // discover the parentheses at the end. Need to check how many items in the expression.
              // Then what?
              // This expression was identified as an assignment and needs to be reclassified.
              $node->setData('operand', $this->cleanToken());
              $this->buildFunctionCall = TRUE;
//              array_pop($this->parentExpressions); // TODO The right spot???
              return $expression;
            }
            // This is a function call.
            $node->setData('operand', $this->buildFunctionCall());
            $node->data->parentExpression = &$expression; // THIS IS THE SPOT!!! // JB 2010-05-03
            $this->functionCalls[] = &$node/*->data*/;
            $this->debugPrint("LEAVING case T_STRING $expressions");
          }
          else {
            // Create object.
            $component = new PGPOperand();
            if ($this->unary != '') {
              // Ex: ++parent::$tokens;
              $component->insertLast($this->unary, 'unary');
              $this->unary = '';
            }
            if ($this->suppress != '') {
              // Ex: @parent::function();
              $component->insertLast($this->suppress, 'suppress');
              $this->suppress = '';
            }
            if ($this->reference != '') {
              // Ex: &parent::$tokens
              $component->insertLast($this->reference, 'reference');
              $this->reference = '';
            }
            $component->insertLast($this->tokenType(), 'type');
            $component->insertLast($this->tokenValue(), 'value');
            $node->setData('operand', $component);
          }
          break;

        // These are equivalent to T_STRING when it is followed by parenthesis.
        case T_EVAL:
        case T_EMPTY:
        case T_UNSET:
        case T_ISSET:
          $node->setData('operand', $this->buildFunctionCall());
          $this->functionCalls[] = &$node/*->data*/;
          break;

        case T_PRINT:
          $node->setData('operand', $this->buildFunctionCall(TRUE));
          $this->functionCalls[] = &$node/*->data*/;
          break;

        case T_CLONE:
          if ($this->nextRealToken() == '(') {
            $node->setData('operand', $this->buildFunctionCall(TRUE));
            $this->functionCalls[] = &$node/*->data*/;
          }
          else {
            $node->setData('operator', $this->tokenValue());
          }
          break;

        case T_VARIABLE:
        case T_STRING_VARNAME:
          $next_token = $this->nextRealToken();
          if ($next_token == '(') {
            $node->setData('operand', $this->buildFunctionCall());
            $this->functionCalls[] = &$node/*->data*/;
          }
          else {
            // Create object.
            $component = new PGPOperand();
            // Unary and indirect are handled differently than reference only
            // because they can be encountered in buildBody or here. It would
            // seem odd for an '&' to begin a statement. If it did, then we
            // could treat all three the same way.
            if ($this->unary != '') {
              $component->insertLast($this->unary, 'unary');
              $this->unary = '';
            }
            // Order is important as the reference would go before the indirect operator.
            if ($this->reference != '') {
              $component->insertLast($this->reference, 'reference');
              $this->reference = '';
            }
            if ($this->suppress != '') {
              // Ex: @parent::function();
              $component->insertLast($this->suppress, 'suppress');
              $this->suppress = '';
            }
            if ($this->indirect != '') {
              $component->insertLast($this->indirect, 'indirect');
              $this->indirect = '';
            }
            $component->insertLast($this->tokenType(), 'type');
            $component->insertLast($this->tokenValue(), 'value');
            $returnTokens2 = array_merge($return_tokens, $this->assignReturnTokens(), $this->operatorReturnTokens());
            // Need to merge the two expressions.
            $component = $this->buildExpression($returnTokens2, $component);
            $this->debugPrint("component");
            $this->debugPrint($component);
            if ($this->buildFunctionCall) {
              $this->buildFunctionCall = FALSE;
              $this->debugPrint("YES this->buildFunctionCall");
              // TODO Make this into a routine of its own.
              $name = $component;
              $this->debugPrint("name");
              $this->debugPrint($name);
              // Create object.
              $expression2 = new PGPFunctionCall();
              $expression2->parent = &$this->currentParent();
//              $expression2->comment = $expression->comment;
              $expression2->type = T_FUNCTION_CALL;
              $expression2->name = $name;
              $expression2->noparens = 0;
              $expression2->parameters = new PGPList();
              unset($component);
//              prev($this->tokens); // TODO ???
              $component = $this->buildFunctionCall(FALSE, $expression2); // TODO Add this to functionCalls?
              $this->functionCalls[] = &$node/*&$component*/; // 2010-02-19
            }

            // TODO This condition should no longer be encountered. 2010-03-31.
            if ($this->unary != '') {
//              $node->data['unary'] = $this->unary; // TODO How to handle unary following variable expression???
              $component->insertLast($this->unary, 'unary');
              $this->unary = '';
            }

            $node->setData('operand', $component);
            $this->debugPrint("component");
            $this->debugPrint($component);

//            $node->setData('operand', $temp);
            $this->debugPrint("LEAVING case T_VARIABLE $expressions");
            $this->printToken();
            $isLeftParenPartOfExpr = FALSE;
          }
          break;

        case T_CONSTANT_ENCAPSED_STRING:
          $node->setData('operand', $this->cleanToken());
          break;

        case T_OBJECT_OPERATOR:
          $node->setData('operator2', $this->tokenValue());
          break;
/*
          // TODO This could be followed by just about anything, eg. T_INC, operator or assign tokens
          $specialReturns = array('[', ']', '(', ')', ' ', '"', T_INC, T_DEC, /*T_VARIABLE,*\/ T_ENCAPSED_AND_WHITESPACE, T_CLOSE_TAG); // 2009-06-13 Inline html // 2009-06-13 added T_CLOSE_TAG -- need a better way to handle these!!!
          $returnTokens2 = array_merge($specialReturns, array(T_OBJECT_OPERATOR), $this->assignReturnTokens(), $this->operatorReturnTokens());
          $temp = $this->buildExpression($returnTokens2);
          $node->setData('object', $temp);
          $this->debugPrint("LEAVING case T_OBJECT_OPERATOR $expressions");
          break;
*/
        case T_ARRAY:
          if ($this->nextRealToken() == '(') {
            $this->debugPrint("isMultiline = $this->isMultiline");
            $node->setData('operand', $this->buildArray(array(')', ';')));
          }
          else {
            $node->setData('operand', $this->cleanToken());
          }
          break;
// 2010-02-25
        case '{':
          $isLeftParenPartOfExpr = TRUE;
          $this->debugPrint("LEFT  lbraces = $lbraces <=> rbraces = $rbraces in $expressions next token value = " . $this->nextTokenValue());
          // '{}' braces may delimit character positions in strings (deprecated
          // as of PHP 5.3, although the syntax still passes lint). They may
          // also delimit an expression.
          // The return tokens on a variable include the right bracket. So we
          // need to test the next character here for '->' or '[' and continue.
          // TODO Convert '{}' used to access a character of a string to '[]'.
          $lbraces++; // TODO Use $lbraces for '{'
          $node->setData('curly', $this->buildExpression(array('}')));
          break;

        case '}':
          $isLeftParenPartOfExpr = FALSE;
          $this->debugPrint("RIGHT lbraces = $lbraces <=> rbraces = $rbraces in $expressions next token value = " . $this->nextTokenValue());
          // Right side count = left side count = 0 when this is a nested call.
          // The left side count was incremented in the parent call.
          if ($rbraces < $lbraces) {
            $rbraces++; // TODO Use $rbraces for '}'
          }
          // Unbalanced braces.
          elseif ($rbraces == $lbraces) {
            $rbraces++;
          }
          break;

        case '[':
//        case '{':
          $isLeftParenPartOfExpr = TRUE;
          $this->debugPrint("LEFT  lbrackets = $lbrackets <=> rbrackets = $rbrackets in $expressions next token value = " . $this->nextTokenValue());
          // '{}' braces may delimit character positions in strings (deprecated
          // in PHP 5.3 or 6.0). They may also delimit an expression.
          // The return tokens on a variable include the right bracket. So we
          // need to test the next character here for '->' or '[' and continue.
          $lbrackets++; // TODO Use $lbraces for '{'
          $node->setData('index', $this->buildExpression(array(']'/*, '}'*/)));
          break;

        case ']':
//        case '}':
          $isLeftParenPartOfExpr = FALSE;
          $this->debugPrint("RIGHT lbrackets = $lbrackets <=> rbrackets = $rbrackets in $expressions next token value = " . $this->nextTokenValue());
          // Right side count = left side count = 0 when this is a nested call.
          // The left side count was incremented in the parent call.
          if ($rbrackets < $lbrackets) {
            $rbrackets++; // TODO Use $rbraces for '}'
          }
          // Unbalanced brackets.
          elseif ($rbrackets == $lbrackets) {
            $rbrackets++;
          }
          break;

        case '(':
          if (!$isLeftParenPartOfExpr) {
            // This left parenthesis indicates a function call if the previous token is not an operator or logical
            $this->debugPrint("hitting left p in buildExpression");
            $expression->delete($node);
            prev($this->tokens);
            $this->buildFunctionCall = TRUE;
//            array_pop($this->parentExpressions); // TODO The right spot???
            return $expression;
          }
          $lparens++;
          $node->setData('lparens', $this->tokenType());
          $isLeftParenPartOfExpr = TRUE;
          break;

        case ')':
          $isLeftParenPartOfExpr = TRUE;
          $this->debugPrint("HITTING RPARENS $expressions");
          if ($rparens < $lparens) {
            $rparens++;
            $node->setData('rparens', $this->tokenType());
          }
          // Unbalanced parentheses.
          elseif ($rparens == $lparens) {
            $rparens++;
          }
          break;

        case T_DNUMBER:
        case T_LNUMBER:
          $node->setData('operand', $this->tokenValue());
          break;

        case T_INC:
        case T_DEC:
          $this->debugPrint("====== INC $expressions");
          $this->debugPrint($node->data);
          if ($expression->findNode('type', 'backward')) {
            // These operators need to be part of an operand expression. If the
            // operand has been parsed, then the expression has a 'type' node.
            $node->setData('unary', $this->tokenValue());
          }
          else {
            // This handles the cases where the operator precedes the operand.
            $this->unary = $this->tokenValue();
//            $expression->delete($node);
          }
          break;

        case T_AND_EQUAL:
        case T_CONCAT_EQUAL:
        case T_DIV_EQUAL:
        case T_MINUS_EQUAL:
        case T_MOD_EQUAL:
        case T_MUL_EQUAL:
        case T_OR_EQUAL:
        case T_PLUS_EQUAL:
        case T_SL_EQUAL: // '<<='
        case T_SR_EQUAL: // '>>='
        case T_XOR_EQUAL:
        case '=':
          $node->setData('assign', $this->tokenValue());
          $isLeftParenPartOfExpr = TRUE;
          break;

        // TODO Split these into two groups: logical and comparison.
        // The latter include <, >, <=, >=, ==, !=, ===, !==
        case T_BOOLEAN_AND:
        case T_BOOLEAN_OR:
        case T_IS_EQUAL:
        case T_IS_NOT_EQUAL:
        case T_IS_IDENTICAL:
        case T_IS_NOT_IDENTICAL:
        case T_IS_GREATER_OR_EQUAL:
        case T_IS_SMALLER_OR_EQUAL:
        case T_LOGICAL_AND:
        case T_LOGICAL_OR:
        case T_LOGICAL_XOR:
        case T_SL: // '<<'
        case T_SR: // '>>'
        case '.':
        case '+':
//        case '-':
        case '*':
        case '/':
        case '%':
        case '^':
//        case '&': // This is the reference operator and bitwise and!!!
        case '|': // Bitwise or.
        case '>':
        case '<':
        case T_INSTANCEOF: // 2010-02-23
          $isLeftParenPartOfExpr = TRUE;
          $node->setData('operator', $this->tokenValue());
//          $this->debugPrint("== OPERATOR =='" .  $expression['operator'] . "'");
          break;

        case '-':
          $isLeftParenPartOfExpr = TRUE; // TODO Will this mess up anthing here?
        case '~':
        case '!':
          $node->setData('operator', $this->tokenValue());
          // Handle numeric and logical negation operators.
          if ($this->nextTokenType() != T_WHITESPACE) {
            // Create an empty node as parent object reference for recursive calls.
            $dummy = NULL;
            $node = &$expression->insertLast($dummy);
            $node->setData('whitespace', ''); // This will remove the space otherwise added.
          }
          break;

        case '&':
          // References or logical and expressions.
          if ($this->previousTokenValue() == '=') {
            // Example: $foo =& $bar().
            $this->reference = $this->tokenValue();
          }
          elseif ($this->nextTokenType() == T_WHITESPACE) { // TODO Should this be nextRealToken != T_VARIABLE?
            $node->setData('operator', $this->tokenValue());
            $isLeftParenPartOfExpr = TRUE; // 2011-01-31
          }
          else {
//            $node->setData('reference', $this->tokenValue());
            $this->reference = $this->tokenValue();
          }
          break;

        case '@': // 2010-01-30
          // Warning suppression operator, e.g. $var = @function().
          // This is similar to a unary operator as is '&'.
//          if ($this->nextTokenType() == T_VARIABLE) {
            $node->setData('operator3', $this->tokenValue());
//          }
          break;

        case '$':
          // Indirect references, e.g. $$var.
          // This is similar to a unary operator as is '&'.
          // Use nextRealToken() as /**/ could be embedded
          if ($this->nextRealToken() == T_VARIABLE) {
            $this->indirect = $this->tokenValue();
          }
          elseif ($this->nextRealToken() == '{') {
            // Marks the begin and end of an expression.
            $node->setData('operator2', $this->tokenValue());
          }
          break;
/*
        case '?':
          $this->isInlineIf = TRUE;
          $node->setData('inlineif', $this->buildExpression(array(':'))); // $node->setData('operator', $this->tokenValue());
          $isLeftParenPartOfExpr = TRUE;
          break;

        case ':':
          if ($this->isInlineIf) {
            // Add '=>' for an expression like array($is_https ? 'ssid' : 'sid' => session_id())
            // Add 'as' for an expression like foreach ($condition ? $true : $false as $item) { }
            $node->setData('inlineelse', $this->buildExpression(array(T_AS, T_DOUBLE_ARROW, ',', ';'))); // $node->setData('operator', $this->tokenValue());
            $this->isInlineIf = FALSE;
          }
          $isLeftParenPartOfExpr = TRUE;
          break;
*/
        case '?':
        case ':':
          $this->isInlineIf = $this->tokenValue() == '?';
          $isLeftParenPartOfExpr = TRUE;
          // Intentionally bleeding into next case.

        case T_UNSET_CAST:
        case T_BOOL_CAST:
        case T_OBJECT_CAST:
        case T_ARRAY_CAST:
        case T_STRING_CAST:
        case T_DOUBLE_CAST:
        case T_INT_CAST:
          $node->setData('operator', $this->tokenValue());
          break;

        case T_DOUBLE_COLON:
          $node->setData('operator2', $this->tokenValue());
          break;

        case T_NEW:
          $node->setData('operator', $this->tokenValue());
          break;

        case '"':
          // Marks the begin and end of a string with variables inside.
          $node->setData('special', $this->buildString());
          break;

        case T_NUM_STRING:
          $node->setData('operand', $this->cleanToken());
          break;

        case T_LIST: // Added 2010-02-23.
          $node->setData('operand', $this->buildList());
          break;

        case T_CLASS_C:
//        case T_DIR: (Added in PHP 5.3.0.)
        case T_FILE:
        case T_FUNC_C:
        case T_LINE:
        case T_METHOD_C:
          $node->setData('operand', $this->tokenValue());
          break;

        case T_START_HEREDOC:
          prev($this->tokens);
          $node->setData('operand', $this->buildString());
          break;

//        case T_END_HEREDOC:
//          $node->setData('operand', $this->tokenValue());
//          break;

        default:
//          prev($this->tokens); // This would be the ';' character???
//          return $expression;
          break;
      } // end switch

      // Clean up after any tokens that are not added to the expression.
      if ($node->data === NULL) {
        $expression->delete($node);
      }

      // Return if an unbalanced delimiter (')', ']', '}') is encountered.
      if (in_array($this->tokenType(), array(']', ')', '}'))) {
        $this->debugPrint("NEW ==== testing leave from buildExpression AT END $expressions");
        $this->debugPrint("lparens = $lparens <=> rparens = $rparens <=> lbrackets = $lbrackets <=> rbrackets = $rbrackets <=> lbraces = $lbraces <=> rbraces = $rbraces");
        // Test for unbalanced delimiter.
        if (($rparens > $lparens) || ($rbrackets > $lbrackets) || ($rbraces > $lbraces)) {
          $this->debugPrint("NEW ==== leaving buildExpression AT END $expressions");
          $this->printToken();
//          $this->debugPrint($expression);
          prev($this->tokens);
          $this->debugPrint("NEW ==== bye bye from buildExpression AT END $expressions");
//          array_pop($this->parentExpressions); // TODO The right spot???
          return $expression;
        }
      }

    } // end while
    $expression->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $expression->extra = $this->cleanToken();
    return $expression;
  }

  /**
   * Builds an assignment expression or statement (or similar statements).
   *
   * The other statements built by this routine are:
   * - a constant definition statement
   * - a global definition statement
   * - a variable definition (older PHP style for backwards compatability)
   *
   * An assignment may be a stand-alone statement or an expression embedded
   * in other statements. Many of the statement types call this routine to
   * build a portion of the statement (even if there is not an actual assignment
   * signified by an assignment operator).
   *
   * @param array $modifiers
   *   An array of modifiers (e.g. public, private, static) for the expression.
   * @return array
   *   An array of the expression.
   */
  private function buildAssignment($modifiers = array()) {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $expression = new PGPAssignment();
    $expression->comment = $this->comment;
    $expression->modifiers = $modifiers;
    $expression->type = T_ASSIGNMENT;
    $expression->values = new PGPList();

    // Global items.
    $this->comment = array();
    $type = $this->tokenType(); // Save this for later test.
    $this->debugPrint("XXXXXX type = $type");

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildAssignment");
      $this->printToken();
      switch ($this->tokenType()) {
        case ',':
          $expression->values->insertLast($this->buildExpression(array(',', ';')));
          break;

        case '(':
          if (!$expression->values->count()) {
            // Example: ($directory = '.');
            // Example: ($directory == '.') ? $directory = '' : '';
            prev($this->tokens);
            $assignment = $this->buildExpression(array(',', ';'));
            $expression->values->insertLast($assignment);
            break;
          }
          // TODO Will this code ever be hit now that we catch the function call in buildExpression???
          // This is a function call not an assignment (or similar).
          $this->debugPrint("hitting left p in buildAssign");
          $this->debugPrint($expression);
          $name = $expression->values->first()->data->first()->data;
          // Create object.
          $expression2 = new PGPFunctionCall();
          $expression2->parent = &$this->currentParent();
          $expression2->comment = $expression->comment;
          $expression2->type = T_FUNCTION_CALL;
          $expression2->name = $name;
          $expression2->noparens = 0;
          $expression2->parameters = new PGPList();
          unset($expression);
          prev($this->tokens);
          return $this->buildFunctionCall(FALSE, $expression2); // TODO Add this to functionCalls?
          break;

        case ')':
          // To catch an assignment statement beginning with parentheses.
          // The parentheses are not stored.
          // Example: ($var++);
          break;

        case ';':
          if ($expression->values->count() == 1) {
            $this->debugPrint("ONE expression in buildAssign");
            // TODO This is a hack!
            if (!in_array($type, array(T_CONTINUE, T_ECHO, T_RETURN, T_EXIT, T_CONST, T_GLOBAL, T_VAR))) {
              $operand = $expression->values->first()->data->first()->data; // TODO Do we need '&'?
              if ($operand instanceof PGPFunctionCall) {
                // This is a function call not an assignment (or similar).
//                return $operand;
              }
            }
            $this->debugPrint("hitting funky type check in buildAssign");
          }
          // The prev() is needed for a conditional where the body is a single
          // statement not surrounded by brackets.
          prev($this->tokens); // Try to catch semi-colons in buildBody!!! 2009-09-29
          return $expression;
          break;

        case T_WHITESPACE:
          break;

        default:
          prev($this->tokens);
          $expression->values->insertLast($this->buildExpression(array(',', ';')));
//          $expression->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
//          $expression->extra = $this->cleanToken();
//          return $expression;
          break;
      }
    }
    $expression->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $expression->extra = $this->cleanToken();
    return $expression;
  }

  /**
   * Builds an array expression.
   *
   * @param array $return_tokens
   *   An array of tokens marking the end of the expression.
   * @return array
   *   An array of the expression.
   */
  private function buildArray($return_tokens = array(')')) {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $expression = new PGPArray();
    $expression->type = $this->tokenType();
    $expression->multiline = FALSE;
    $expression->original = FALSE;
    $expression->count = 0;
    $expression->values = new PGPList();

    // Local items.
    $lparens = 0;
    $rparens = 0;
    $assigns = 0;
    $values = 0;
    $commas = 0;
    $in_function_or_call = $this->inFunctionOrCall;
    $original = FALSE;

    /*
     * Multiline (true) vs inline (false)
     * Precedence order
     * - if original has newlines, then true (Move this down to after 'keys'?)
     * - if original is in a function parameter list, then false
     * - if original is in a function call parameter list, then false
     * - if original has only one 'value', then false
     * - if original has multiple 'keys', then true
     * - if original has multiple 'values', then maybe???
     *
     * Other (NOT DONE)
     * - if return occurrences (a single whitespace with multiple returns counts as one)
     *   are not equal to values, then assume non-standard newline, set
     *   newline = FALSE, and follow the whitespace characters.
     * - if return occurrences = values, then ignore whitespace characters.
     *
     * Handle T_COMMENT tokens in the array expression.
     */

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildArray");
      $this->printToken();

      switch ($this->tokenType()) {
//        case '(':
//          $lparens++;
//          $expression->values->insertLast($this->tokenType(), 'lparens');
//          break;

        case ')':
          $this->debugPrint("while buildArray lparens = $lparens rparens = $rparens");
          $rparens++;
          // TODO Why not just add the rparens??? We handle it in PGPWriter.
          // TODO Is this condition check necessary? Should it be TRUE always?
          if ($rparens < $lparens) {
            $expression->values->insertLast($this->tokenType(), 'rparens');
          }
          if ($rparens == $lparens) {
            $this->debugPrint("Hitting ) in while buildArray");
            $expression->multiline = $this->setMultiline($original, $assigns, $values, $in_function_or_call);
            $expression->original = $original;
            $expression->preserve = $this->preserveArrayFormat;
            $expression->count = $values;
            $expression->commaCount = $commas;
            return $expression;
          }
          break;

        case ',':
          $commas++;
          break;

        case T_DOUBLE_ARROW:
          $assigns++;
          $expression->values->insertLast($this->tokenValue(), 'assign');
          break;

        case T_WHITESPACE:
          if ($this->addNewLineToWhitespace) {
            // Transfer newline character from an end-of-line comment.
            $this->addNewLineToWhitespace = FALSE;
            $token = &$this->tokens[key($this->tokens)];
            $token[1] = "\n" . $token[1];
          }
          $blankLines = substr_count($this->tokenValue(), "\n");
          if ($blankLines > 0) {
            $original = TRUE;
          }
          // 2010-03-24
          if (!$this->preserveArrayFormat) {
            break;
          }
          if ($this->tokenValue() != ' ') {
            // Add whitespace.
            $expression->values->insertLast($this->tokenValue(), 'whitespace');
          }
          break;

          /*
           * It would be preferable to have the whitespace token include the
           * newline that Zend includes on an end-of-line comment. This would
           * seem to be consistent. The EOL comment and the open tag are the
           * only two tags that include the newline. Why?
           *
           * If we stored the newline with the whitespace and stripped off the
           * indent spaces, then we could indent ourselves. This would ensure
           * the rewritten file is formatted nicely. With upgrade routines, if
           * we needed to write the array at a different level of indention,
           * then we could do so. Storing the indent eliminates this flexibility.
           * This applies to expressions other than arrays.
           *
           * The question is identifying when we want to store the whitespace
           * information at all. If an array is inline and drupal standard is
           * multiline, then we don't care about storing the whitespace. If
           * there are no EOL comments embedded in the array, then we also
           * don't care.
           */

        case T_COMMENT:
          /*
           * Reasoning: if previous token is whitespace, then it determines
           * whether the comment is appended. If previous token is not
           * whitespace, then the comment must be appended.
           */
          $append = 1;
          if ($this->previousTokenType() == T_WHITESPACE) {
            $blankLines = substr_count($this->previousTokenValue(), "\n");
            if ($blankLines > 0) {
              $append = 0;
            }
          }
          // Remove newline character from an end-of-line comment.
          $comment = array(
            'type' => $this->tokenType(),
            'value' => $this->cleanComment($this->tokenValue()),
            'append' => $append,
          );
          // Add the comment.
          $expression->values->insertLast($comment, 'comment');
          // Transfer newline (if one) to next whitespace token.
          if ($this->includesNewline($this->tokenValue())) {
            if ($this->nextTokenType() == T_WHITESPACE) {
              $this->addNewLineToWhitespace = TRUE;
            }
            elseif ($this->preserveArrayFormat) { // else { // 2010-03-24
              $whitespace = "\n";
              $expression->values->insertLast($whitespace, 'whitespace');
            }
          }
          break;

        case '(':
          if ($lparens == 0) {
            // This is the left parenthesis following 'array'.
            $lparens++;
            $expression->values->insertLast($this->tokenType(), 'lparens');
            break;
          }
          // All other parentheses mark the start of an expression.
          // Bleed into next case so buildExpression handles the parentheses.

        default:
          prev($this->tokens);
          /*
           * This would be cleaner if the tokens distinguished between comment
           * styles, embedded ('/**\/') vs. end of line ('//').
           * Set flag, do prev(), and save the $temp for reuse on next loop.
           */
          $temp = $this->buildExpression(array(T_DOUBLE_ARROW, ',', ')'));
          $type = $this->nextRealToken('type', TRUE);

          /*
           * Comma use cases
           * If next token = T_DOUBLE_ARROW, then insert the key expression, $temp = NULL.
           * If next token = T_COMMA, then insert the value expression and set $has_comma, $temp = NULL.
           *
           * No comma use cases
           * If current token = T_WHITESPACE with a newline, then insert the expression, $temp = NULL.
           * If current token = T_WHITESPACE, then save the expression ($temp) and cycle.
           *
           * If next token = end-of-line comment (add a helper function), then insert the expression and the comment with append, $temp = NULL.
           * If next token = inline comment (add a helper function), then insert the expression and the comment with append, $temp = NULL. (RETHINK)
           */

          $this->debugPrint("inside default case return from buildExpression");

          $comment = NULL;
          if ($type == T_DOUBLE_ARROW) {
            $whitespace = '';
            $key = $temp->last()->type;
            if ($key == 'whitespace') {
              // Separate the whitespace item so the subsequent double arrow is
              // printed to match the input.
              $whitespace = $temp->last()->data;
              $temp->deleteLast();
              $this->debugPrint("ELSEIF deleteLast on T_DOUBLE_ARROW");
            }
            $expression->values->insertLast($temp, 'key');
            $temp = NULL;
            if ($whitespace && $this->preserveArrayFormat) { // if ($whitespace) { // 2010-03-24
              $expression->values->insertLast($whitespace, 'whitespace');
              $whitespace = NULL;
            }
          }
          elseif ($type == ',') {
            $values++;
            $expression->values->insertLast($temp, 'value');
            $temp = NULL;
          }
          elseif ($type == ')') {
            /*
             * The last item in the value expression can be an expression
             * (e.g. inlineelse) in which case we need to look inside the
             * expression.
             */
            // Find the last expression in $temp.
            $last = $temp;
            while (TRUE) {
              if ($last instanceof PGPExpression && $last->last()->data instanceof PGPExpression) {
                $last = $last->last()->data;
              }
              else {
                break;
              }
            }
            // Check for whitespace.
            $whitespace = '';
            $key = $last->last()->type;
            if ($key == 'whitespace') {
              $whitespace = $last->last()->data;
              // Remove the whitespace item so we close the array nicely.
              $last->deleteLast();
              $this->debugPrint("ELSEIF deleteLast on ')'");
            }

            /*
             * Check last expression item == T_COMMENT
             * If so, create a separate element for the comment and set append = 1
             * Use if here not elseif as expression could end in comment followed
             * by whitespace.
             */
            $key = $last->last()->type;
            if ($key == 'comment') {
              // Save comment.
              if ($this->includesNewline($last->last()->data['value'])) {
                // Only comments starting with '//' need special treatment.
                // Comment has already had newline removed in buildExpression.
                $comment = $last->last()->data;
//                $comment['value'] = $this->cleanComment($comment['value']);
                $comment['append'] = 1; // TODO Is this correct if preceded by whitespace with newline?
                // Remove comment from expression.
                $last->deleteLast();
                $this->debugPrint("ELSEIF deleteLast with key = comment");
              }
            }
            $values++;
            $expression->values->insertLast($temp, 'value');
            $temp = NULL;
            // Add comment.
            if ($comment) {
              $expression->values->insertLast($comment, 'comment');
            }
            if ($whitespace && $this->preserveArrayFormat) { // if ($whitespace) { // 2010-03-24
              $expression->values->insertLast($whitespace, 'whitespace');
            }
          }
            /*
             * No comma use cases
             * Implies these are the last entries in the array
             *
             * end-of-line comment use cases
             * If preceded by whitespace with a newline, then insert the comment with append = 0.
             * If preceded by whitespace, then insert the comment with append = 1.
             *
             * inline comment use cases
             * If preceded by whitespace with a newline, then insert the comment with append = 0.
             * If preceded by whitespace, then insert the comment with append = 1.
             *
             * Could have EOL comments after each item
             * key
             * // comment
             * =>
             * // comment
             * value[,\w]
             * In this case we need to hold key until we see the double arrow or another expression.
             * What if the expression has operators separated by EOL comments?
             */

            /*
             * This code has to do with the buildExpression including the
             * whitespace with the value operand in an element like
             * 'pass' => $user->pass_raw which does not end in a comma.
             * Also happens on a string value that does not have a comma.
             * However, in this case, the whitespace is its own element in the
             * expression object. This is probably NOT the place to handle
             * the whitespace. Should do in buildExpression.
             *
             * Does this next if make sense now or did we eliminate the case
             * by changing the T_STRING from array to PGPOperand?
             */
          else {
            $this->debugPrint("Array failure");
            $this->debugPrint($type);
            $this->debugPrint($temp);
          }
          break;

//          $expression['warning'] = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
//          $expression['extra'] = $this->cleanToken();
//          return $expression;
          break;
      }
    }
    $expression->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $expression->extra = $this->cleanToken();
    return $expression;
  }

  private function setMultiline($original, $assigns, $values, $in_function_or_call) {
    /*
     * Multiline (true) vs inline (false)
     * Precedence order
     * - if original has newlines, then false // - if original has newlines, then true
     * - if original is in a function parameter list, then false
     * - if original is in a function call parameter list, then false
     * - if original has only one 'value', then false
     * - if original has multiple 'keys', then true
     * - if original has multiple 'values', then maybe???
     *
     * Reasoning: if original has newlines, then we are recording whitespace
     * and need to have it dictate. This is an issue if the spacing is not
     * nice. Need to remove extraneous whitespace (indent spaces and extra
     * newlines) and indent on our own.
     *
     * We now ignore whitespace when preserveArrayFormat == FALSE. So return
     * TRUE here if original has newlines (when we might otherwise not). But we
     * control the spacing.
     */
    $this->debugPrint("setMultiline: '$original', $assigns, $values, '$in_function_or_call'");
    if ($this->preserveArrayFormat) return FALSE;
    if ($original) return TRUE; // if ($original) return FALSE;
    if ($in_function_or_call) return FALSE;
    if ($values < 2) return FALSE;
    if ($assigns > 1) return TRUE;
//    if ($values) return TRUE;
  }

  /**
   * Builds a conditional statement block: do, if, elseif, else, while, switch.
   *
   * @return PGPConditional
   *   An object of the statement block.
   */
  private function buildConditional() {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $block = new PGPConditional();
    $block->type = $this->tokenType();
    $block->conditions = new PGPList();
    $block->nobraces = 1;

    // Local items.
    $conditions = 0;
    $body_captured = FALSE;

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildConditional");
      $this->printToken();
      if ($block->nobraces == 1 && $body_captured) {
        // Check here in case a one-line body has an appended comment as the
        // next token could be anything. Without an appended comment, the body
        // should end with a semi-colon.
        $this->tokenType() == ';' ? '' : prev($this->tokens);
        return $block;
      }
      switch ($this->tokenType()) {
        case '(':
          $block->conditions->insertLast($this->buildExpression($this->logicalReturnTokens()), 'condition');
          break;

        case T_BOOLEAN_AND:
        case T_BOOLEAN_OR:
        case T_LOGICAL_AND:
        case T_LOGICAL_OR:
        case T_LOGICAL_XOR:
        case '^':
        case '&':
        case '|':
          $block->conditions->insertLast($this->tokenValue(), 'operator');
          $block->conditions->insertLast($this->buildExpression($this->logicalReturnTokens()), 'condition');
          break;

        case T_IS_EQUAL:
        case T_IS_NOT_EQUAL:
        case T_IS_IDENTICAL:
        case T_IS_NOT_IDENTICAL:
        case T_IS_GREATER_OR_EQUAL:
        case T_IS_SMALLER_OR_EQUAL:
        case '>':
        case '<':
        case T_INSTANCEOF:
          $block->conditions->insertLast($this->tokenValue(), 'operator');
          $block->conditions->insertLast($this->buildExpression($this->logicalReturnTokens()), 'condition');
          break;

        case ')':
          if ($block->type == T_DO) {
            return $block;
          }
          break;

        case ':':
          $block->colon = 1;
        case '{':
          $block->nobraces = 0;
          $block->comment = $this->buildBodyComment();
          $block->body = $this->buildBody(array(T_CLOSE_TAG)); // 2009-06-13 Inline html
          break;

        case T_ENDIF:
        case T_ENDWHILE:
        case T_ENDSWITCH:
          $block->end = $this->tokenValue();
          break;

        case ';':
          // TODO if (!isset($block->end) { error }
          // TODO if (!$block->body) { $block->nop = 1 } // This should result in printing a ';' without braces.
        case '}':
          if ($block->type != T_DO) {
            return $block;
          }
          break;

        case T_WHILE:
          if ($block->type != T_DO) {
            // This should be a while block that is the only statement of a parent if without braces.
            prev($this->tokens);
            $block->body = $this->buildBody(array('}', ';'), 1);
            return $block;
          }
          break;

        case T_COMMENT: // TODO Until we can figure a better way to handle embedded comments
          if ($block->nobraces == 1 && !$body_captured) {
            prev($this->tokens);
            $block->comment = $this->buildBodyComment();
            $this->debugPrint("after buildBodyComment");
            $this->printToken();
            break;
          }
        case T_WHITESPACE:
          if ($block->nobraces == 1 && $body_captured) {
            prev($this->tokens);
            return $block;
          }
          break;

        case T_IF:
          if ($block->type == T_ELSE) {
            // Handle else if statements.
            $block->type = T_ELSE_IF;
          }
          else {
            // This should be an if block that is the only statement of a parent if without braces.
            prev($this->tokens);
            $block->body = $this->buildBody(array('}', ';'), 1);
            return $block;
          }
          break;

        case T_CLOSE_TAG:
          // 2009-06-13 Inline html
          // For inline html pages
          if (!$block->body) {
//            unset($block->body);
          }
          $block->inline = 1;
          prev($this->tokens);
          return $block;
          break;

        default:
          if ($block->nobraces == 1 && !$body_captured) {
            $statements_to_return = $this->tokenType() == T_TRY ? 2 : 1;
            prev($this->tokens);
            $block->body = $this->buildBody(array(';'), $statements_to_return);
            $body_captured = TRUE;
          }
          elseif ($block->nobraces == 1 && $body_captured) {
            return $block;
          }
          else {
            $block->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
            $block->extra = $this->cleanToken();
            return $block;
          }
          break;
      }
    }
    $block->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $block->extra = $this->cleanToken();
    return $block;
  }

  /**
   * Builds a for statement block.
   *
   * @return PGPFor
   *   An object of the statement block.
   */
  private function buildFor() {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $block = new PGPFor();
    $block->type = $this->tokenType();
    $block->initial = new PGPList();
    $block->condition = new PGPList();
    $block->increment = new PGPList();
    $block->nobraces = 1;

    // Local items.
    $body_captured = FALSE;
    $condition_to_set = 0;
    $conditions = array('initial', 'condition', 'increment');
    $condition = $conditions[$condition_to_set];

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildFor");
      $this->printToken();
      switch ($this->tokenType()) {
        case ';':
          if ($body_captured || isset($block->end)) {
            return $block;
          }
          elseif ($condition_to_set == 2) {
//            $block->nop = 1; // TODO this
            return $block;
          }
          $condition_to_set++;
          $condition = $conditions[$condition_to_set];
        case '(':
        case ',':
          $expression = $this->buildExpression(array(',', ';')); // TODO This handles multiple conditions differently than buildConditional???
          if (!$expression->isEmpty()) {
            $block->$condition->insertLast($expression);
          }
          break;

        case ')':
          break;

        case ':':
          $block->colon = 1;
        case '{':
          $block->nobraces = 0;
          $block->comment = $this->buildBodyComment();
          $block->body = $this->buildBody();
          break;

        case T_ENDFOR:
          $block->end = $this->tokenValue();
          break;

        case '}':
          return $block;
          break;

        case T_COMMENT: // TODO Until we can figure a better way to handle embedded comments
        case T_WHITESPACE:
          if ($block->nobraces == 1 && $body_captured) {
            prev($this->tokens);
            return $block;
          }
          break;

        default:
          if ($block->nobraces == 1 && !$body_captured) {
            $statements_to_return = $this->tokenType() == T_TRY ? 2 : 1;
            prev($this->tokens);
            $block->body = $this->buildBody(array(';'), $statements_to_return);
            $body_captured = TRUE;
          }
          elseif ($block->nobraces == 1 && $body_captured) {
            return $block;
          }
          else {
            $block->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
            $block->extra = $this->cleanToken();
            return $block;
          }
          break;
      }
    }
    $block->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $block->extra = $this->cleanToken();
    return $block;
  }

  /**
   * Builds a foreach statement block.
   *
   * @return PGPForeach
   *   An object of the statement block.
   */
  private function buildForEach() {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $block = new PGPForeach();
    $block->type = $this->tokenType();
    $block->nobraces = 1;

    // Local items.
    $body_captured = FALSE;

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint($this->tokenType());
      switch ($this->tokenType()) {
        case '(':
          $block->initial = $this->buildExpression(array(T_AS));
          break;

        case T_AS:
        case T_DOUBLE_ARROW:
          $temp = $this->buildExpression(array(T_DOUBLE_ARROW, ')'));
          if ($this->nextTokenType() == T_DOUBLE_ARROW) {
            $block->key = $temp;
          }
          else {
            $block->value = $temp;
          }
          break;

        case ')':
          break;

        case ':':
          $block->colon = 1;
        case '{':
          $block->nobraces = 0;
          $block->comment = $this->buildBodyComment();
          $block->body = $this->buildBody();
          break;

        case T_ENDFOREACH:
          $block->end = $this->tokenValue();
          break;

        case ';':
          // TODO if (!isset($block->end) { error }
        case '}':
          return $block;
          break;

        case T_COMMENT: // TODO Until we can figure a better way to handle embedded comments
        case T_WHITESPACE:
          if ($block->nobraces == 1 && $body_captured) {
            prev($this->tokens);
            return $block;
          }
          break;

        default:
          if ($block->nobraces == 1 && !$body_captured) {
            $statements_to_return = $this->tokenType() == T_TRY ? 2 : 1;
            prev($this->tokens);
            $block->body = $this->buildBody(array(';'), $statements_to_return);
            $body_captured = TRUE;
          }
          elseif ($block->nobraces == 1 && $body_captured) {
            return $block;
          }
          else {
            $block->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
            $block->extra = $this->cleanToken();
            return $block;
          }
          break;
      }
    }
    $block->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $block->extra = $this->cleanToken();
    return $block;
  }

  /**
   * Builds a case statement block.
   *
   * @return PGPCase
   *   An object of the statement block.
   */
  private function buildCase() {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $block = new PGPCase();
    $block->type = $this->tokenType();

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildCase");
      $this->printToken();
      switch ($this->tokenType()) {
        case ':':
        case ';': // Allowed delimiter, but not typical (usually a typo)
          $block->comment = $this->buildBodyComment();
          $block->body = $this->buildBody(array(T_CASE, T_DEFAULT, T_ENDSWITCH, '}'));
          break;

        case T_CASE:
        case T_DEFAULT:
        case T_ENDSWITCH:
        case '}':
          prev($this->tokens);
          return $block;
          break;

        default:
          prev($this->tokens);
          // A semi-colon is an allowed case delimiter, but usually a typo
          $block->case = $this->buildExpression(array(':', ';'));
          break;
      }
    }
    $block->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $block->extra = $this->cleanToken();
    return $block;
  }

  /**
   * Builds a declare statement.
   *
   * @return PGPDeclare
   *   An object of the statement.
   */
  private function buildDeclare() {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $block = new PGPDeclare();
    $block->comment = $this->comment; // TODO Is this a valid item for this statement? Or a leftover item from copy and paste?
    $block->type = $this->tokenType();
    $block->parameters = new PGPList();

    // Global items.
    $this->comment = array();

    while (next($this->tokens) !== FALSE) {
      switch ($this->tokenType()) {
        case T_STRING:
          $block->name = $this->tokenValue(); // TODO Is this a valid item for this statement? Or a leftover item from copy and paste?
          break;

        case '(':
//        case ',':
          $this->isMultiline = FALSE;
          $block->parameters->insertLast($this->buildExpression(array(',', ')')));
          $this->isMultiline = TRUE;
          break;

        case ')':
          $this->debugPrint("Hitting ) in while buildDeclare");
          break;

        case ':':
          $block->colon = 1;
        case '{':
          $block->body = $this->buildBody();
          break;

        case T_ENDDECLARE:
          $block->end = $this->tokenValue();
          break;

        case ';':
          // TODO if (!isset($block->end) { error }
        case '}':
          return $block;
          break;

        case T_WHITESPACE:
          break;

        default:
          $block->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
          return $block;
          break;
      }
    }
    $block->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $block->extra = $this->cleanToken();
    return $block;
  }

  /**
   * Builds a try-catch statement block.
   *
   * @return PGPTryCatch
   *   An object of the statement block.
   */
  private function buildTryCatch() {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $block = new PGPTryCatch();
    $block->type = $this->tokenType();

    while (next($this->tokens) !== FALSE) {
      switch ($this->tokenType()) {
        case '(':
          $block->exception = $this->buildExpression(array(')'));
          break;

        case ')':
          break;

        case '{':
          $block->comment = $this->buildBodyComment();
          $block->body = $this->buildBody();
          break;

        case '}':
          return $block;
          break;

        case T_COMMENT: // TODO Until we can figure a better way to handle embedded comments
        case T_WHITESPACE:
          break;

        default:
          $block->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
          $block->extra = $this->cleanToken();
          return $block;
          break;
      }
    }
    $block->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $block->extra = $this->cleanToken();
    return $block;
  }

  /**
   * Returns a document comment string or store it if related to the next token.
   *
   * This may be added to: statement list, function or function body.
   * If atop a function, then add to function.
   * If inside a function, then add to function body.
   * Else add to statements.
   *
   * @return string
   *   The document comment (or blank if related to the next token).
   */
  private function buildComment() {
    $this->debugPrint(__FUNCTION__);

    $comment = $this->cleanToken();
    $this->comment = array();

    switch ($this->tokenType()) {
      case T_ML_COMMENT:
      case T_DOC_COMMENT:
        if ($this->nextTokenType() == T_WHITESPACE && substr_count($this->nextTokenValue(), "\n") > 1) {
          return $comment;
        }
        $obvious = array(
          // Class property without a modifier.
          T_VARIABLE,
          // Obvious ones.
          T_CONST,
          T_GLOBAL,
          T_VAR,
          // Modifiers.
          T_PUBLIC,
          T_PRIVATE,
          T_PROTECTED,
          T_STATIC,
          T_ABSTRACT,
          T_FINAL,
          // Obvious ones.
          T_FUNCTION,
          T_INTERFACE,
          T_CLASS,
        );
        $key = $this->nextRealToken('key', TRUE);
        if (in_array($key, $obvious) || ($key == T_STRING && $this->nextRealToken('value', TRUE) == 'define')) {
          $this->comment = $comment;
        }
        else {
          return $comment;
        }
        break;
    }

    return array();
  }

  /**
   * Checks for and builds a body comment.
   *
   * @return string
   *   A string of the comment.
   */
  private function buildBodyComment() {
    $this->debugPrint(__FUNCTION__);
    if ($this->nextRealToken('type', TRUE) == T_COMMENT) {
      // If previous token is ';', ')' (this can occur with a one-line body), or
      // whitespace without a return, then the comment is appended to the line.
      // TODO Could also be a ',' in an array expression.
      // TODO Need not have any whitespace before the comment.
      $append = 0;
      if ($this->nextTokenType() == T_WHITESPACE) {
        $blankLines = substr_count($this->nextTokenValue(), "\n");
        if ($blankLines > 0) {
          // The comment is not appended.
          return;
        }
        // Advance the tokens.
        next($this->tokens);
      }
      $append = 1;
      // A comment only includes a newline if its style is '//' or '#'.
      $data = array(
        'type' => $this->tokenType(), // TODO Why set this value as type?
        'value' => $this->cleanComment($this->nextRealToken('value', TRUE)),
        'append' => $append,
      );
      // Advance the tokens.
      next($this->tokens);
      // Return the comment.
      return $data['value'];
    }
    return '';
  }

  /**
   * Builds a special string expression with variable references.
   *
   * @return array
   *   An array of the expression.
   */
  private function buildString() {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $expression = new PGPString();
    $expression->type = T_SPECIAL_STRING;

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildString");
      $this->printToken();

      switch ($this->tokenType()) {
        case T_STRING: // TODO Is this token possible in this context?
          $this->debugPrint('inside T_STRING of ' . __FUNCTION__);
          $expression->insertLast($this->buildFunctionCall(), 'call'); // TODO The toString method does not have a case for this key.
//          $this->functionCalls[] = &$expression; // TODO Include this? // 2010-07-08 Hmm???
          break;

        case T_VARIABLE:
          // This could be an array
          $return_tokens = array('"', T_VARIABLE, T_ENCAPSED_AND_WHITESPACE, T_CONSTANT_ENCAPSED_STRING); // Not sure if last one applies

          // Create object.
          $component = new PGPOperand();
          $component->insertLast($this->tokenType(), 'type');
          $component->insertLast($this->tokenValue(), 'value');
          $component = $this->buildExpression($return_tokens, $component);
          $expression->insertLast($component, 'operand');
          break;

//        case T_CONSTANT_ENCAPSED_STRING:
        case T_ENCAPSED_AND_WHITESPACE:
          $expression->insertLast($this->cleanToken(), 'string');
          break;
/*
        case '$': // 2010-02-23 Added for ${DB_ERROR} = TRUE;
          $expression->insertLast($this->tokenType(), 'dollar'); // Was 'begin'
          $has_begin = TRUE;
          break;
*/
//        case '{': // 2010-02-23 Added for ${DB_ERROR} = TRUE;
        case T_CURLY_OPEN:
        case T_DOLLAR_OPEN_CURLY_BRACES:
          $return_tokens = array('}');
          $expression->insertLast($this->cleanToken(), 'open');
          $expression->insertLast($this->buildExpression($return_tokens), 'operand');
          $dummy = '}';
          $node = &$expression->insertLast($dummy, 'close');
          break;

        case '"':
          // Marks the begin and end of a string with variables inside.
          return $expression;
          break;

        case '}':
          break;

        case T_WHITESPACE:
          // Example: catch whitespace at end of array value
          if ($this->tokenValue() != ' ') {
            $expression->insertLast($this->tokenValue(), 'whitespace');
          }
          break;

        case T_START_HEREDOC:
          $expression->insertLast($this->tokenValue(), 'begin');
          break;

        case T_END_HEREDOC:
          $temp = $this->tokenValue();
          // Only a ';' may follow the closing identifier.
          if ($this->nextRealToken() != ';') {
            $temp .= "\n";
          }
          $expression->insertLast($temp, 'end');
          break;

        default:
          prev($this->tokens);
          $expression->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
          $expression->extra = $this->cleanToken();
          return $expression;
          break;
      }
    }
    $expression->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $expression->extra = $this->cleanToken();
    return $expression;
  }

  /**
   * Builds a list statement.
   *
   * TODO This is a "function call" with an attached "assignment" expression.
   *
   * @return array
   *   An array of the statement.
   */
  private function buildList() {
    $this->debugPrint(__FUNCTION__);

    // Create object.
    $expression = new PGPListStatement();
    $expression->type = $this->tokenType();
    $expression->name = $this->tokenValue();
    $expression->noparens = 0;
    $expression->parameters = new PGPList();
//    $expression->assignment = new PGPExpression();

    // Local variables.
    $has_assignment = FALSE;

    while (next($this->tokens) !== FALSE) {
      $this->debugPrint("while buildList");
      $this->printToken();
      switch ($this->tokenType()) {
        case T_WHITESPACE:
          break; // 2010-03-21 Following code is copied from buildFunctionCall. Why?
          // Intentionally bleed into next case for a require statement.
          // TODO There could be whitespace even with parentheses.
          // Check for next non-comment and non-whitespace token.
//          if (/*!$require ||*/ !$expression->parameters->isEmpty()) {
//            break;
//          }
//          $expression->noparens = 1;

        case '(':
        case ',':
          $this->isMultiline = FALSE; // TODO Is this necessary?
          $expression->parameters->insertLast($this->buildExpression(array(',', ';', T_CLOSE_TAG))); // 2009-06-13 Inline html
          $this->isMultiline = TRUE;
          break;

        case ')':
          if (!$has_assignment) {
            $expression->assignment = $this->buildExpression(array(',', ';', T_CLOSE_TAG)); // array(',', ')')); // 2009-06-13 Inline html
            $has_assignment = TRUE;
          }
          else {
            // Added 2010-02-23 for if ((list(..) = function_call(..)) && ..)
            prev($this->tokens);
            return $expression;
          }
          break;

        case ';':
          return $expression;
          break;

        case T_CLOSE_TAG:
          // 2009-06-13 Inline html
          $expression->inline = 1; // TODO This property is not defined in the class.
          prev($this->tokens);
          return $expression;
          break;

        default:
          if ($has_assignment) {
            // Added 2010-02-23 for things like: if (list(..) = <array expression> <array operator> <array expression> ..)
            prev($this->tokens);
            return $expression;
          }
          $expression->warning = 'unexected syntax in ' . __METHOD__ . '(' . __LINE__ . ')';
          $expression->extra = $this->cleanToken();
          return $expression;
          break;
      }
    }
    $expression->warning = 'unexpected end of file in ' . __METHOD__ . '(' . __LINE__ . ')';
    $expression->extra = $this->cleanToken();
    return $expression;
  }

  /**
   * @} End of "defgroup pgp_builder".
   */

  /**
   * @defgroup pgp_helper Grammar parser helper functions
   * @{
   */

  /**
   * Sets the preserve array flag used with array parsing.
   *
   * @param boolean $preserveArrayFormat
   *   Indicates whether to preserve array formatting.
   */
  public function setPreserveArrayFormat($preserveArrayFormat = FALSE) {
    $this->preserveArrayFormat = $preserveArrayFormat;
  }

  /**
   * Adds token names to array of tokens.
   *
   * Note: Not called by this code but from script.
   */
  public function addTokenNames() {
    foreach ($this->tokens as $key => &$token) {
      if (is_array($token)) {
        if ($token[0] == T_WHITESPACE) {
//          $this->tokens[$key][1] = '[' . $token[1] . ']';
        }
        $this->tokens[$key][0] = $token[0] . ' (' . $this->tokenName($token[0]) . ')';
      }
    }
  }

  /**
   * Sets the code snippet to be parsed.
   *
   * TODO Why would tokens need to be shared among classes?
   *
   * @param string $snippet
   *   A code snippet string.
   */
  public function setSnippet(&$snippet = NULL) {
    // Clear tokens.
    $this->clearArray($this->tokens);
    $this->resetTokens();
    if ($snippet) {
      $this->tokens = token_get_all($snippet);
    }
  }

  /**
   * Resets everything (to free up memory).
   */
  public function reset() {
    // Clear tokens.
    $this->clearArray($this->tokens);

    // Clear arrays with node references.
    $this->clearArray($this->interfaces);
    $this->clearArray($this->classes);
    $this->clearArray($this->functions);
    $this->clearArray($this->functionCalls);
    $this->clearArray($this->defines);
    $this->clearArray($this->globals);
    $this->clearArray($this->comments);

    $this->clearArray($this->parents);
    $this->clearArray($this->parentExpressions);

    // Recursively clear the grammar statement objects.
    if ($this->statements instanceof PGPList) {
      $this->statements->clear_r();
    }
    unset($this->statements);

    // Clear the rest.
    $this->resetTokens();
    $this->resetOthers();
  }

  private function clearArray(&$items) {
    foreach ($items as $key => &$item) {
      unset($items[$key]);
    }
    unset($items);
  }

  /**
   * Resets the tokens array.
   */
  private function resetTokens() {
    $this->tokens = array();
  }

  /**
   * Resets the other arrays and class variables.
   */
  private function resetOthers() {
    // Items declared in parent.
    $this->statements = NULL;
    $this->interfaces = array();
    $this->classes = array();
    $this->functions = array();
    $this->functionCalls = array();
    $this->defines = array();
    $this->globals = array();
    $this->comments = array();

    // Items declared in this class.
    $this->expressions = 0;
    $this->isMultiline = TRUE;
    $this->isInterface = FALSE;
    $this->isInlineIf = FALSE;
    $this->inFunctionOrCall = FALSE;
    $this->addWhitespace = FALSE;
    $this->addNewLineToWhitespace = FALSE;
    $this->unary = '';
    $this->reference = '';
    $this->suppress = '';
    $this->indirect = '';
    $this->previousTokenIncludesNewLine = FALSE;
    $this->comment = array();
    $this->parents = array();
    $this->parentExpressions = array();
    $this->buildFunctionCall = FALSE;
  }

  /**
   * Removes the ending newline character from an end-of-line comment.
   *
   * @param string $comment
   *   A string containing a single-line comment.
   * @return string
   *   The modified string.
   */
  private function cleanComment($comment) {
    if ($this->includesNewline($comment)) {
      return str_replace("\n", '', $comment);
    }
    return $comment;
  }

  /**
   * Returns current token after removing the line number element.
   *
   * @return array
   *   An array representing the token.
   */
  private function cleanToken() {
    $token = current($this->tokens);
    if (is_array($token)) {
      return array(
        'type' => $token[0],
        'value' => $token[1], // check_plain($token[1]),
      );
    }
    return $token;
  }

  /**
   * Returns current token after cleaning text and removing the line number.
   *
   * Note: Not called by this code but from script.
   *
   * @return array
   *   An array representing the token.
   */
  private function cleanTokenToPrint($token) {
    switch ($token['type']) {
      case T_OPEN_TAG:
      case T_OPEN_TAG_WITH_ECHO:
      case T_CLOSE_TAG:
        $token['value'] = check_plain($token['value']);
        break;

      case T_COMMENT:
      case T_ML_COMMENT:
      case T_DOC_COMMENT:
        $token['value'] = '[' . $token['value'] . ']';
        break;
    }
    return $token;
  }

  /**
   * Prints current token (if an array), otherwise the type.
   *
   * Note: $this->debug must equal TRUE.
   */
  private function printToken() {
    if (!$this->debug) {
      return;
    }
    if (is_array(current($this->tokens))) {
      $this->debugPrint(current($this->tokens));
    }
    else {
      $this->debugPrint($this->tokenType());
    }
  }

  /**
   * Prints current token (if an array), otherwise the type.
   *
   * Note: $this->debug need not equal TRUE.
   */
  private function printToken2() {
//    return;
    $debug = $this->debug;
    $this->debug = TRUE;
    $this->printToken();
    $this->debug = $debug;
  }

  /**
   * Returns token type.
   *
   * @return string
   */
  private function tokenType() {
    $token = current($this->tokens);
    if (is_array($token)) {
      return $token[0];
    }
    return $token;
  }

  /**
   * Returns token value.
   *
   * @return string
   */
  private function tokenValue() {
    $token = current($this->tokens);
    if (is_array($token)) {
      return $token[1];
    }
    return $token;
  }

  /**
   * Returns token line number.
   *
   * @return string
   */
  private function tokenLine() {
    $token = current($this->tokens);
    if (is_array($token)) {
      return $token[2];
    }
    return 0;
  }

  /**
   * Returns previous token type.
   *
   * @return string
   */
  private function previousTokenType() {
    prev($this->tokens);
    $type = $this->tokenType();
    next($this->tokens);
    return $type;
  }

  /**
   * Returns previous token value.
   *
   * @return string
   */
  private function previousTokenValue() {
    prev($this->tokens);
    $value = $this->tokenValue();
    next($this->tokens);
    return $value;
  }

  /**
   * Returns next token type.
   *
   * @return string
   */
  private function nextTokenType() {
    // Need to test for end of file.
    next($this->tokens);
    $type = $this->tokenType();
    prev($this->tokens);
    return $type;
  }

  /**
   * Returns next token value.
   *
   * @return string
   */
  private function nextTokenValue() {
    // Need to test for end of file.
    next($this->tokens);
    $value = $this->tokenValue();
    prev($this->tokens);
    return $value;
  }

  /**
   * Returns next non-whitespace token type or value.
   *
   * @param string $key
   *   A string equal to 'type' or 'value'.
   * @param boolean $include_comment
   *   Indicates whether a comment counts as a real token.
   * @return string (or constant)
   *   A string with the desired token value.
   */
  private function nextRealToken($key = '', $include_comment = FALSE) {
    $count = 0;
    $values = array(T_WHITESPACE);
    if (!$include_comment) {
      $values[] = T_COMMENT;
    }
    // Need to test for end of file.
    while (next($this->tokens) !== FALSE) {
      $count++;
      if (!in_array($this->tokenType(), $values)) {
        break;
      }
    }
    // 2006-06-20
    if (current($this->tokens) === FALSE) {
      return 'EOA';
    }
    // Save desired value.
    if ($key == 'value') {
      $item = $this->tokenValue();
    }
    else {
      $item = $this->tokenType();
    }
    // Reset position in array.
    while ($count > 0) {
      prev($this->tokens);
      $count--;
    }
    return $item;
  }

  /**
   * Returns the assignment tokens.
   *
   * @return array
   *   An array of tokens.
   */
  private function assignReturnTokens() {
    return array(
      T_AND_EQUAL,
      T_AS,
//      T_BOOLEAN_AND,
//      T_BOOLEAN_OR,
      T_CONCAT_EQUAL,
      T_DIV_EQUAL,
      T_DOUBLE_ARROW,
//      T_IS_EQUAL,
//      T_IS_NOT_EQUAL,
//      T_IS_IDENTICAL,
//      T_IS_NOT_IDENTICAL,
//      T_IS_GREATER_OR_EQUAL,
//      T_IS_SMALLER_OR_EQUAL,
//      T_LOGICAL_AND,
//      T_LOGICAL_OR,
//      T_LOGICAL_XOR,
      T_MINUS_EQUAL,
      T_MOD_EQUAL,
      T_MUL_EQUAL,
      T_OR_EQUAL,
      T_PLUS_EQUAL,
//      T_SL,
      T_SL_EQUAL,
//      T_SR,
      T_SR_EQUAL,
      T_XOR_EQUAL,
      '=',
      ',',
      ';' // Catchall if we see end of statement.
    );
  }

  /**
   * Returns the operator tokens.
   *
   * @return array
   *   An array of tokens.
   */
  private function operatorReturnTokens() {
    return array(
      T_BOOLEAN_AND,
      T_BOOLEAN_OR,
      T_IS_EQUAL,
      T_IS_NOT_EQUAL,
      T_IS_IDENTICAL,
      T_IS_NOT_IDENTICAL,
      T_IS_GREATER_OR_EQUAL,
      T_IS_SMALLER_OR_EQUAL,
      T_LOGICAL_AND,
      T_LOGICAL_OR,
      T_LOGICAL_XOR,
      T_SL,
      T_SR,
      '.',
      '+',
      '-',
      '*',
      '/',
      '%',
      '^',
      '&',
      '|',
      '>',
      '<',
      '?', // Add these two special delimiters.
      ':',
      ';', // Catchall if we see end of statement.
    );
  }

  /**
   * Returns the logical operator tokens.
   *
   * @return array
   *   An array of tokens.
   */
  private function logicalReturnTokens() {
    return array(
      T_BOOLEAN_AND,
      T_BOOLEAN_OR,
      T_LOGICAL_AND,
      T_LOGICAL_OR,
      T_LOGICAL_XOR,
      T_INSTANCEOF,
      '^',
      '&',
      '|',
      ')', // Catchall if we see end of statement.
    );
  }

  /**
   * Returns the current parent node object.
   *
   * The list container is automatically set in PGPNode when created from
   * PGPList.
   *
   * @return PGPNode
   */
  private function &currentParent() {
    if (empty($this->parents)) {
      $parent = NULL;
      return $parent;
    }
    return $this->parents[count($this->parents) - 1];
  }

  /**
   * Returns the current parent expression object.
   *
   * @todo Not used.
   * @return PGPExpression
   */
  private function &currentParentExpression() {
    if (empty($this->parentExpressions)) {
      $parent = NULL;
      return $parent;
    }
    return $this->parentExpressions[count($this->parentExpressions) - 1];
  }

  /**
   * Returns TRUE if the comment includes a newline.
   *
   * @param string $comment
   * @return boolean
   */
  private function includesNewline($comment) {
    return (strpos($comment, '//') === 0 || strpos($comment, '#') === 0);
  }

  /**
  * Returns the function object whose body includes the specified line number.
  *
  * @param integer $line
  *   The line number included in the function body.
  *
  * @return PGPClass
  *   The function object, or FALSE.
  */
  public function findFunction($line) {
    $found = FALSE;
    // @todo Is array_reverse() slow? Would it be better to use a reverse for loop?
    $nodes = array_reverse($this->getFunctions());
    foreach ($nodes as &$node) {
      if ($node->line < $line) {
        $item = &$node->data;
        if (!isset($item) || !is_object($item) || !($item instanceof PGPClass) || $item->type != T_FUNCTION) {
          // The reference could have been changed in another routine so that it
          // no longer refers to an object.
          return FALSE;
        }
        $body = $item->body;
        if (empty($body)) {
          return FALSE;
        }
        elseif ($body->first()->line <= $line && $body->last()->line >= $line) {
          $found = TRUE;
          return $item;
        }
      }
    }
    if (!$found) {
      return FALSE;
    }
  }

  /**
   * @} End of "defgroup pgp_helper".
   */
}
