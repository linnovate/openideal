<?php

/**
 * @file
 * A member of the Grammar Parser API classes. These classes provide an API for
 * parsing, editing and rebuilding a code snippet.
 *
 * Copyright 2009 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Grammar Parser base statement class.
 *
 * This class provides a container for storing a grammar statement.
 *
 * @todo Add a parent reference to the containing object.
 */
class PGPBase {

  /**
   * Parent expression object. (This may not be applicable to all classes).
   *
   * @var PGPExpression
   */
  public $parentExpression;

  /**
   * Parser warning message.
   *
   * @var string
   */
  public $warning;

  /**
   * The current parse token when a warning is encountered.
   *
   * @var array
   */
  public $extra;

  /**
   * Whether to print debug information.
   *
   * @var boolean
   */
  protected $debug = FALSE; // TRUE; //

  public function __construct() {

  }

  /**
   * Returns a string representation of a PGPBase object
   * (identical to print_r()).
   *
   * Unlike print_r, this omits the printing of embedded object references
   * that would result in infinite recursion.
   *
   * @param integer $indent
   *   The indent level.
   * @param mixed $elements
   *   The PGPBase object to represent.
   * @return string
   *   A string of the object (or $elements) formatted like print_r() output.
   */
  public function print_r($indent = 0, $elements = NULL) {
    $elements = is_null($elements) ? $this : $elements;

    // The first call formats without indenting the parentheses.
    if (!$indent) {
      $strings = array();
      $strings[] = get_class($elements);
      $strings[] = "(";
      $strings[] = $this->print_r(1, $elements);
      $strings[] = ")";

      return implode("\n", $strings) . "\n";
    }

    $indent0 = str_repeat('    ', $indent);
    $indent1 = str_repeat('    ', $indent + 1);
    $strings = array();

    foreach ($elements as $key => $element) {
      if (!isset($element)) {
        continue;
      }
      if ($key === 'parentExpression') {
        $strings[] = $indent0 . "[$key] => " . get_class($element);
        $strings[] = $indent1 . "(";
        $strings[] = $indent1 . "    [type-1] => "; // . $element->data->type;
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      elseif (is_array($element)) {
//        $strings[] = "<_________ data is a Array ________>"; // This is encountered!!!
        if (!empty($element)) {
          $strings[] = $indent0 . "[$key] => Array";
          $strings[] = $indent1 . "(";

          $temp = $this->print_r($indent + 2, $element);
          if ($temp) {
            $strings[] = $temp;
          }
          $strings[] = $indent1 . ")";
          $strings[] = '';
        }
      }
      elseif ($element instanceof PGPBase) {
        $strings[] = $indent0 . "[$key] => " . get_class($element);
        $strings[] = $indent1 . "(";
        $temp = $element->print_r($indent + 2, $element);
        if ($temp) {
          $strings[] = $temp;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      elseif ($element instanceof PGPList) {
        $strings[] = $indent0 . "[$key] => " . get_class($element);
        $strings[] = $indent1 . "(";
        $temp = $element->print_r($indent + 2, $element);
        if ($temp) {
          $strings[] = $temp;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      elseif ($element instanceof PGPNode) { // TODO This handles the parent node, but we need to print expression nodes.
        $strings[] = $indent0 . "[$key] => PGPNode";
        $strings[] = $indent1 . "(";
        if (is_object($element->data)) {
          if (isset($element->data->type)) {
            $strings[] = $indent1 . "    [type-1] => " . $element->data->type;
          }
        }
        elseif (is_array($element->data)) {
          /*
           * This handles T_CONTINUE, T_ECHO, T_RETURN, and T_EXIT if their
           * $node->data = array(); If they are PGPAssignment objects, then
           * they would hit a different condition. The 4 statements end up
           * being the "parent" node when there is a function call as the
           * value on the T_ECHO or T_RETURN.
           */
          if (isset($element->data['type'])) {
            $strings[] = $indent1 . "    [type-2] => " . $element->data['type'];
          }
        }
        else {
          $strings[] = $indent1 . "    [type-3] => " . $element->type;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      else {
        $strings[] = $indent0 . "[$key] => $element";
      }
    }
    return implode("\n", $strings);
  }

  /**
   * Clears the members of a PGPBase object.
   *
   * @param mixed $elements
   *   The PGPBase object to clear.
   */
  public function clear_r(&$elements = NULL) {
//    $this->debugPrint(__METHOD__);
    $elements = is_null($elements) ? $this : $elements;

    foreach ($elements as $key => &$element) {
      if (!isset($element)) {
        continue;
      }
      if (in_array($key, array('parent', 'parentExpression'))) {
      }
      elseif (is_array($element)) {
        if (!empty($element)) {
          $this->clear_r($element);
        }
      }
      elseif ($element instanceof PGPBase) {
//        $this->debugPrint("element is PGPBase {$element->type}");
        $element->clear_r();
      }
      elseif ($element instanceof PGPList) {
//        $this->debugPrint("element is PGPList");
        $element->clear_r();
      }
      elseif ($element instanceof PGPNode) { // TODO This handles the parent node, but we need to print expression nodes.
//        $this->debugPrint("element is PGPNode {$element->type}");
        $data = &$element->data;

        if (is_object($data)) {
//          $data->clear_r();
          if (!in_array($key, array('parent', 'parentExpression')) && method_exists($data, 'clear_r')) {
//            $this->debugPrint("clear_r routine exists on PGPNode type of {$element->type}");
            $data->clear_r();
          }
          unset($data);
        }
        elseif (is_array($data)) {
          // Is this the case that hits references to parents and causes an infinite loop???
          if (!empty($data)) {
//            $this->clear_r($data);
          }
          unset($data);
        }
        else {
          unset($data);
        }
      }
      else {
//        $this->debugPrint("element is string '$element'");
      }
      unset($element);
      if (is_array($elements)) {
        unset($elements[$key]);
      }
    }
  }

  /**
   * Applies a callback function to all elements of a PGPBase object meeting the
   * search criteria.
   *
   * @param string $callback
   *   Name of the callback function to be invoked on each statement meeting the
   *   search criteria.
   * @param string $class
   *   Name of the class to find.
   * @param string $member
   *   Name of the member to evaluate.
   * @param string $key
   *   Name of the key of member to evaluate.
   * @param string $value
   *   Element value to be matched.
   * @param string $direction
   *   Direction in which to traverse the list.
   */
  public function searchCallback($callback, $class, $member, $key, $value, $direction = 'forward') {
//    $this->debugPrint(__METHOD__);
    foreach ($this as $key2 => $element) {
      if (!isset($element)) {
        continue;
      }
      if ($key2 === 'parentExpression') {

      }
      elseif (is_array($element)) {
//        $this->debugPrint("<_________ element is a Array ________>");
      }
      elseif ($element instanceof PGPBase) {
//        $this->debugPrint("<_________ element is a PGPBase ________>");
        if (get_class($element) == $class) {
          if (method_exists($data, 'matches')) {
            if ($element->matches($member, $key, $value)) {
              // Invoke callback function.
              $callback($element);
            }
          }
        }
        // Confirm the object as it may have been altered in the callback.
        if ($element instanceof PGPBase) {
          // Recurse on this element's members.
          $element->searchCallback($callback, $class, $member, $key, $value, $direction);
        }
      }
      elseif ($element instanceof PGPList) {
//        $this->debugPrint("<_________ element is a PGPList ________>");
        // Recurse on this element's members.
        $element->searchCallback($callback, $class, $member, $key, $value, $direction);
      }
      elseif ($element instanceof PGPNode) {
      }
      else {
      }
    }
  }

  /**
   * These two functions are convenience wrappers to search().
   */
  public function &searchForward($class, $member, $key, $value, $return_node = FALSE) {
    return $this->search($class, $member, $key, $value, $return_node, 'forward');
  }

  public function &searchBackward($class, $member, $key, $value, $return_node = FALSE) {
    return $this->search($class, $member, $key, $value, $return_node, 'backward');
  }

  /**
   * Returns the first element of a PGPBase object meeting the search criteria.
   *
   * @param string $class
   *   Name of the class to find.
   * @param string $member
   *   Name of the member to evaluate.
   * @param string $key
   *   Name of the key of member to evaluate.
   * @param string $value
   *   Element value to be matched.
   * @param boolean $return_node
   *   Indicates whether to return the node object or its data (the default).
   * @param string $direction
   *   Direction in which to traverse the list.
   * @return mixed
   *   The object meeting the search criteria, its node, or FALSE.
   */
  public function &search($class, $member, $key, $value, $return_node = FALSE, $direction = 'forward') {
//    $this->debugPrint(__METHOD__);
    foreach ($this as $key2 => $element) {
//      $this->debugPrint("key = $key");
      if (!isset($element)) {
        continue;
      }
      if ($key2 === 'parentExpression') {

      }
      elseif (is_array($element)) {
//        $this->debugPrint("<_________ element is a Array ________>");
      }
      elseif ($element instanceof PGPBase) {
//        $this->debugPrint("<_________ element is a PGPBase ________>");
        if (get_class($element) == $class) {
          if (method_exists($data, 'matches')) {
            if ($element->matches($member, $key, $value)) {
              // Return element.
              return $element;
            }
          }
        }
        // Confirm the object as it may have been altered in the callback.
        if ($element instanceof PGPBase) {
          // Recurse on this element's members.
          $result = $element->search($class, $member, $key, $value, $return_node, $direction);
          if ($result !== FALSE) {
            return $result;
          }
        }
      }
      elseif ($element instanceof PGPList) {
//        $this->debugPrint("<_________ element is a PGPList ________>");
        // Recurse on this element's members.
        $result = $element->search($class, $member, $key, $value, $return_node, $direction);
        if ($result !== FALSE) {
          return $result;
        }
      }
      elseif ($element instanceof PGPNode) {
      }
      else {
      }
    }

    $var = FALSE;
    return $var;
  }

  /**
   * Returns all elements of a PGPBase object meeting the search criteria.
   *
   * @param string $class
   *   Name of the class to find.
   * @param string $member
   *   Name of the member to evaluate.
   * @param string $key
   *   Name of the key of member to evaluate.
   * @param string $value
   *   Element value to be matched.
   * @param boolean $return_node
   *   Indicates whether to return the node object or its data (the default).
   * @param string $direction
   *   Direction in which to traverse the list.
   * @return mixed
   *   The object meeting the search criteria, its node, or FALSE.
   */
  public function searchAll($class, $member, $key, $value, $return_node = FALSE, $direction = 'forward') {
//    $this->debugPrint(__METHOD__);
    // Initialize.
    $items = array();

    foreach ($this as $key2 => $element) {
//      $this->debugPrint("key = $key");
      if (!isset($element)) {
        continue;
      }
      if ($key2 === 'parentExpression') {

      }
      elseif (is_array($element)) {
//        $this->debugPrint("<_________ element is a Array ________>");
      }
      elseif ($element instanceof PGPBase) {
//        $this->debugPrint("<_________ element is a PGPBase ________>");
        if (get_class($element) == $class) {
          if (method_exists($data, 'matches')) {
            if ($element->matches($member, $key, $value)) {
              // Add item to list.
              $items[] = /*$return_node ? $element :*/ $element;
            }
          }
        }
        // Confirm the object as it may have been altered in the callback.
        if ($element instanceof PGPBase) {
          // Recurse on this element's members.
          $temp = $element->searchAll($class, $member, $key, $value, $return_node, $direction);
          $items = array_merge($items, $temp);
        }
      }
      elseif ($element instanceof PGPList) {
//        $this->debugPrint("<_________ element is a PGPList ________>");
        // Recurse on this element's members.
        $temp = $element->searchAll($class, $member, $key, $value, $return_node, $direction);
        $items = array_merge($items, $temp);
      }
      elseif ($element instanceof PGPNode) {
      }
      else {
      }
    }

    return $items;
  }

  /**
   * Invokes callback on each item in selected list.
   *
   * On forward, start with $first->next.
   * On reverse, start with $last->previous.
   *
   * @todo Add a parameter indicating what to return: node or data.
   *
   * @param mixed $elements
   *   Class members.
   * @param string $callback
   *   Name of a callback function.
   * @param string $direction
   *   Direction in which to traverse the list.
   */
  public function traverse($elements, $callback, $direction = 'forward') {
    if (!isset($elements) || !($elements instanceof PGPList)) {
      return;
    }

    $current = $elements->first();
    while ($current->next != NULL) {
      $callback($this, $current);
      $current = &$current->next;
    }
  }

  /**
   * Returns TRUE if the type of first element of the expression matches search value.
   *
   * @param string $type
   *   A string of the type to match.
   * @return boolean
   *   TRUE if type matches.
   */
  public function isType($type) {
//    $this->debugPrint(__METHOD__);
    if (!$type) {
      return FALSE;
    }

    return isset($this->type) && $this->type == $type;
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
}

/**
 * Grammar Parser interface, class or function statement block class.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPClass extends PGPBase {
  /**
   * Body comment.
   *
   * @var string
   */
  public $bodyComment;

  /**
   * Documentation block comment.
   *
   * @var string
   */
  public $comment;

  /**
   * Scope modifiers (e.g. public, private) for the statement block.
   *
   * @var array
   */
  public $modifiers;

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * Indicates the function returns a value by reference.
   *
   * @var string
   */
  public $reference;

  /**
   * Class name.
   *
   * @var mixed (PGPExpression or array)
   */
  public $name;

  /**
   * Name of class (or names of interfaces) extended by this class (or interface).
   *
   * @var array
   */
  public $extends;

  /**
   * Names of interfaces implemented by this class.
   *
   * @var array (or PGPList???)
   */
  public $implements;

  /**
   * Function parameters.
   *
   * @var PGPList
   */
  public $parameters;

  /**
   * List of body statements.
   *
   * @var PGPBody
   */
  public $body;

  public function __construct($name = '') {
    parent::__construct();

//    $this->comment = '';
    $this->modifiers = array();
    $this->type = 0;
    $this->name = $name;
    $this->extends = array();
    $this->implements = array();
  }

  /**
   * Returns the text of an interface, class or function statement block.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the statement block.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $block = $this; // TODO Why use this variable name now?

    $strings = array();
    $header = $indent;

    // Assemble comment and block header.
    $add_space = 1; // Set this to one in this routine.
    foreach ($block as $key => $element) {
      $this->debugPrint("inside $key");
      if ($key == 'body') {
        break;
      }
      $space = $add_space ? ' ' : '';
      switch ($key) {
        case 'comment':
          if (!empty($element)) {
            $strings[] = PGPWriter::printComment($element, $indent);
          }
          break;

        case 'modifiers':
          // Add class and function items.
          if ($element) {
            $header .= implode(' ', $element);
          }
          break;

        case 'type':
          $type = PGPWriter::statementTypeToString($element);
          $space = $block->modifiers ? ' ' : '';
          $header .= $space . $type;
          $add_space = 1;
          break;

        case 'reference': // Added back 2010-02-23 (how and when was this lost?)
          $header .= $space . $element;
          $add_space = -1;
          break;

        case 'name':
          $header .= $space . $element; // $header .= ' ' . $block->name;
          break;

        case 'extends':
          // Add class and interface items.
          if ($element) {
            $header .= $space . 'extends ' . implode(', ', $element);
          }
          break;

        case 'implements':
          // Add class items.
          if ($element) {
            $header .= $space . 'implements ' . implode(', ', $element);
          }
          break;

        case 'parameters':
          // Add function parameters.
          if (is_object($element)) {
            $header .= '(' . $element->toString() . ')';
          }
          break;

        case 'whitespace':
          // We might encounter this tag even if not a class or interface
          $header .= $element;
          // Set space to zero for next item.
          $add_space = -1;
          break;
      }
      if ($add_space <= 0) {
        $add_space++;
      }
    }

    // Add block body.
    if (!isset($block->body)) {
      // Body may be missing on an abstract function.
      $header .= ";";
      $strings[] = $header;
    }
    elseif ($block->body->isEmpty()) {
      // Body may be empty on a class extends definition.
      $header .= $block->bodyComment ? ' { ' . $block->bodyComment . "\n" . $indent . "}" : ' { }';
      $strings[] = $header;
    }
    else {
//      $header .= " {";
      // Add body comment (i.e. a comment following the block beginning token).
      $header .= $block->bodyComment ? ' { ' . $block->bodyComment : ' {';
      $strings[] = $header;
      $strings[] = $block->body->toString();
      $strings[] = $indent . "}";
    }

    return implode("\n", $strings);
  }

  /**
   * Returns the signature of an interface, class or function statement block.
   *
   * @todo Consider refactoring toString() into header() and remaining. Then
   * call header() from here. Would eliminate the save and restore code below.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the statement less the block comment and statement type.
   */
  public function signature($indent = '') {
    $this->debugPrint(__METHOD__);
    // Save comment and body; restore after printing.
    $comment = $this->comment;
    $body = $this->body;
    $this->comment = '';
    $this->body = NULL;
    $text = $this->toString($indent);
    $needle = PGPWriter::statementTypeToString($this->type);
    $beg = strpos($text, $needle);
    $text = substr_replace($text, '', $beg, strlen($needle) + 1);
    // Restore comment and body.
    $this->comment = $comment;
    $this->body = $body;

    return substr($text, 0, -1); // Remove the ';' character.
  }

  /**
   * Returns the parameter count.
   *
   * @return integer
   *   The parameter count.
   */
  public function parameterCount() {
    if ($this->parameters instanceof PGPList) {
      return $this->parameters->count();
    }
    return 0;
  }

  /**
   * Returns the parameter at the specified index.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @return PGPExpression
   *   The parameter object.
   */
  public function &getParameter($index = 0) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        return $this->parameters->get($index)->data;
      }
    }

    $result = FALSE;
    return $result;
  }

  /**
   * Sets the parameter at the specified index to the indicated expression.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @param PGPExpression $expression
   *   The expression object.
   */
  public function setParameter($index, $expression) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        $this->parameters->updateKey($index, $expression);
      }
      else {
        $this->parameters->insertLast($expression);
      }
    }
  }

  /**
   * Inserts the indicated expression as the parameter at the specified index.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @param PGPExpression $expression
   *   The expression object.
   */
  public function insertParameter($index, $expression) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        $this->parameters->insertBefore($this->parameters->get($index), $expression);
      }
      else {
        $this->parameters->insertLast($expression);
      }
    }
  }

  /**
   * Deletes the parameter at the specified index.
   *
   * @param integer $index
   *   The index in the parameter list.
   */
  public function deleteParameter($index = 0) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        $this->parameters->delete($this->parameters->get($index));
      }
    }
  }

  /**
   * Returns the text of the parameter at the specified index.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @return string
   *   The text of the parameter.
   */
  public function printParameter($index = 0) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        return $this->parameters->get($index)->data->toString();
      }
    }
    return '';
  }

  /**
   * Returns the (comma-separated) text of the function call parameters.
   *
   * @return string
   *   The comma-separated text of the parameters.
   */
  public function printParameters() {
    return $this->parameters->toString();
  }

  /**
   * Clears the parameters.
   */
  public function clearParameters() {
    $this->parameters->clear();
  }

  /**
   * Returns the parameter variable at the specified index.
   *
   * The parameter variable is the first operand at the specified index,
   * stripped of inline comments included with the operand. Selecting only the
   * first operand also removes any default value assignment or inline comments
   * preceding the operand. Because this is not the entire parameter expression,
   * the object is not returned as a reference.
   *
   * @param integer $index
   *   The index in the parameter list.
   *
   * @return PGPExpression
   *   The parameter variable object.
   */
  public function getParameterVariable($index = 0) {
//    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        if ($parameter = /*&*/$this->parameters->get($index, FALSE)) { // TODO Are these '&' necessary?
//          cdp($parameter, '$parameter');
          if ($parameter->isType(T_VARIABLE)) {
            if ($variable = /*&*/$parameter->findNode('operand')) {
//              cdp($variable, '$variable');
              // Strip any embedded comments or whitespace included with the operand.
              return $variable->stripComments();
            }
          }
        }
      }
    }

    $result = FALSE;
    return $result;
  }
}

/**
 * Grammar Parser conditional statement block class.
 *
 * Applies to: do, if, elseif, else, while, switch.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPConditional extends PGPBase {
  /**
   * Body comment.
   *
   * @var string
   */
  public $comment;

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * List of conditions to be satisfied before executing body statements.
   *
   * @var PGPList
   */
  public $conditions;

  /**
   * Indicates whether statement body is enclosed in braces.
   *
   * @var integer (or boolean???)
   */
  public $nobraces;

  /**
   * Indicates whether statement block uses alternative syntax.
   *
   * @var integer (or boolean???)
   */
  public $colon;

  /**
   * List of body statements.
   *
   * @var PGPBody
   */
  public $body;

  /**
   * End block token type when statement block uses alternative syntax.
   *
   * @var integer
   */
  public $end;

  /**
   * Indicates whether statement block is part of inline html syntax.
   *
   * @var integer
   */
  public $inline; // TODO If inline html is implemented, this should go in PGPBase.

  public function __construct() {
    parent::__construct();

    $this->type = 0;
//    $this->nobraces = 0; // Does this mean the variable isset()?
//    $this->colon = 0;
  }

  /**
   * Returns the text of a conditional statement block: do, if, elseif, else, while, switch.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $open_brace = isset($expression->colon) ? ':' : '{';
    $close_brace = isset($expression->end) ? $expression->end . ";" : '}';
    $close = isset($expression->inline) ? '' : "\n" . $indent . $close_brace; // 2009-06-13 Inline html
    $close2 = '';
    switch ($expression->type) {
      case T_DO:
        $start = 'do {';
        $open = '';
        $close = "\n" . $indent . '} while ('; // TODO Check for inline?
        $close2 = ');';
        break;

      case T_IF:
        $start = 'if (';
        $open = ') ' . $open_brace;
        break;

      case T_ELSEIF:
        $start = 'elseif (';
        $open = ') ' . $open_brace;
        break;

      case T_ELSE_IF:
        $start = 'else if (';
        $open = ') ' . $open_brace;
        break;

      case T_ELSE:
        $start = 'else';
        $open = ' ' . $open_brace;
        break;

      case T_WHILE:
        $start = 'while (';
        $open = ') ' . $open_brace;
        break;

      case T_SWITCH:
        $start = 'switch (';
        $open = ') ' . $open_brace;
        break;
    }

    // Add body comment (i.e. a comment following the block beginning token).
    $open .= isset($expression->comment) && $expression->comment ? ' ' . $expression->comment : '';

    $add_space = 0;
    $space = '';
    $string = $indent . $start;
    foreach ($expression as $key => $element) {
//      if (!isset($element)) {
//        continue; // This prevents us from putting a ';' for a nop.
//      }
      $space = $add_space ? ' ' : '';
      switch ($key) {
        case 'type':
          $add_space = -1;
          break;

        case 'conditions':
          // Add conditions.
          if (is_object($expression->conditions)) {
            if ($expression->type == T_DO) {
              $condition = $element->toString();
            }
            else {
              $string .= $element->toString();
            }
          }
          break;

        case 'body':
          $string .= $open;
          if (isset($element) && !$element->isEmpty()) {
            $string .= "\n";
            $string .= $element->toString();
          }
          $string .= $close;
          $add_space = ($expression->type == T_DO) ? -1 : $add_space;
          break;

        case 'whitespace':
          $string .= $element;
          // Set space to zero for next item.
          $add_space = -1;
          break;
      }
      if ($add_space <= 0) {
        $add_space++;
      }
    }
    $string .= ($expression->type == T_DO) ? $condition . $close2 : '';
    return $string;
  }

  /**
   * Returns the text of the conditions in an array.
   *
   * @return array
   *   An array of the conditions.
   */
  public function conditionsToArray() {
    $this->debugPrint(__METHOD__);

    $keys = array();
    if ($this->conditions instanceof PGPList) {
      $current = $this->conditions->first();
      while ($current->next != NULL) {
        $type = $current->type;
        if ($type == 'condition') {
          $keys[$type] = $current->data->toString();
        }
        elseif ($type == 'operator') {
          $keys[$type] = $current->data;
        }
        $current = &$current->next;
      }
    }

    return $keys;
  }
}

/**
 * Grammar Parser for statement block class.
 *
 * Each of the condition expressions can be empty or contain multiple
 * expressions separated by commas.
 * See http://www.php.net/manual/en/control-structures.for.php.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPFor extends PGPBase {
  /**
   * Body comment.
   *
   * @var string
   */
  public $comment;

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * Inital value expression(s) for condition.
   *
   * @var PGPList
   */
  public $initial;

  /**
   * Condition expression(s) to be satisfied before executing body statements.
   *
   * @var PGPList
   */
  public $condition;

  /**
   * Final value expression(s) for condition.
   *
   * @var PGPList
   */
  public $increment;

  /**
   * Indicates whether statement body is enclosed in braces.
   *
   * @var integer (or boolean???)
   */
  public $nobraces;

  /**
   * Indicates whether statement block uses alternative syntax.
   *
   * @var integer (or boolean???)
   */
  public $colon;

  /**
   * List of body statements.
   *
   * @var PGPBody
   */
  public $body;

  /**
   * End block token type when statement block uses alternative syntax.
   *
   * @var integer
   */
  public $end;

  public function __construct() {
    parent::__construct();

    $this->type = 0;
//    $this->nobraces = 0; // Does this mean the variable isset()?
//    $this->colon = 0;
  }

  /**
   * Returns the text of a for statement block.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $open_brace = isset($expression->colon) ? ':' : '{';
    $close_brace = isset($expression->end) ? $expression->end . ";" : '}';

    $start = 'for (';
    $open = ') ' . $open_brace;
    $close = "\n" . $indent . $close_brace;

    // Add body comment (i.e. a comment following the block beginning token).
    $open .= $expression->comment ? ' ' . $expression->comment : '';

    $add_space = 0;
    $space = '';
    $string = $indent . $start;
    foreach ($expression as $key => $element) {
//      if (!isset($element)) {
//        continue; // This prevents us from putting a ';' for a nop.
//      }
      $space = $add_space ? ' ' : '';
      switch ($key) {
        case 'type':
          $add_space = -1;
          break;

        case 'initial':
        case 'condition':
          $string .= $element->toString() . '; ';
          break;

        case 'increment':
          $string .= $element->toString();
          break;

        case 'body':
          $string .= $open;
          if (isset($element) && !$element->isEmpty()) {
            $string .= "\n";
            $string .= $element->toString();
          }
          $string .= $close;
          break;

        case 'whitespace':
          $string .= $element;
          // Set space to zero for next item.
          $add_space = -1;
          break;
      }
      if ($add_space <= 0) {
        $add_space++;
      }
    }
    return $string;
  }
}

/**
 * Grammar Parser foreach statement block class.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPForeach extends PGPBase {
  /**
   * Body comment.
   *
   * @var string
   */
  public $comment;

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * Expression of items to loop on.
   *
   * @var PGPExpression
   */
  public $initial;

  /**
   * Name of key to reference each loop item.
   *
   * @var PGPExpression
   */
  public $key;

  /**
   * Name of value to reference each loop item.
   *
   * @var PGPExpression
   */
  public $value;

  /**
   * Indicates whether statement body is enclosed in braces.
   *
   * @var integer (or boolean???)
   */
  public $nobraces;

  /**
   * Indicates whether statement block uses alternative syntax.
   *
   * @var integer (or boolean???)
   */
  public $colon;

  /**
   * List of body statements.
   *
   * @var PGPBody
   */
  public $body;

  /**
   * End block token type when statement block uses alternative syntax.
   *
   * @var integer
   */
  public $end;

  public function __construct() {
    parent::__construct();

    $this->type = 0;
//    $this->nobraces = 0; // Does this mean the variable isset()?
//    $this->colon = 0;
  }

  /**
   * Returns the text of a foreach statement block.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $open_brace = isset($expression->colon) ? ':' : '{';
    $close_brace = isset($expression->end) ? $expression->end . ";" : '}';

    $start = 'foreach (';
    $open = ') ' . $open_brace;
    $close = "\n" . $indent . $close_brace;

    // Add body comment (i.e. a comment following the block beginning token).
    $open .= $expression->comment ? ' ' . $expression->comment : '';

    $add_space = 0;
    $space = '';
    $string = $indent . $start;
    foreach ($expression as $key => $element) {
//      if (!isset($element)) {
//        continue;// This prevents us from putting a ';' for a nop.
//      }
      $space = $add_space ? ' ' : '';
      switch ($key) {
        case 'type':
          $add_space = -1;
          break;

        case 'initial':
          $string .= $space . $element->toString() . ' as';
          break;

        case 'key':
          if (isset($element)) { // TODO Avoid checking here by adding the nop variable
            $string .= $space . $element->toString() . ' =>';
          }
          break;

        case 'value':
          $string .= $space . $element->toString();
          break;

        case 'body':
          $string .= $open;
          if (isset($element) && !$element->isEmpty()) {
            $string .= "\n";
            $string .= $element->toString();
          }
          $string .= $close;
          break;

//        case T_COMMENT:
        case 'whitespace':
          $string .= $element;
          // Set space to zero for next item.
          $add_space = -1;
          break;
      }
      if ($add_space <= 0) {
        $add_space++;
      }
    }
    return $string;
  }
}

/**
 * Grammar Parser case statement block class.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPCase extends PGPBase {
  /**
   * Body comment.
   *
   * @var string
   */
  public $comment;

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * Expression representing case value.
   *
   * @var PGPExpression
   */
  public $case;

  /**
   * List of body statements.
   *
   * @var PGPBody
   */
  public $body;

  public function __construct() {
    parent::__construct();

    $this->type = 0;
  }

  /**
   * Returns the text of a case statement block.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $start = 'case ';
    switch ($expression->type) {
      case T_CASE:
        $start = 'case ';
        break;

      case T_DEFAULT:
        $start = 'default';
        break;
    }
    $open = ":";
    $close = ""; // New line is added when we add the closing brace.

    // Add body comment (i.e. a comment following the block beginning token).
    $open .= $expression->comment ? ' ' . $expression->comment : '';

    $add_space = 0;
    $space = '';
    $string = $indent . $start;
    foreach ($expression as $key => $element) {
      if (!isset($element)) { // TODO It would seem this should not occur!!!
        continue;
      }
      $space = $add_space ? ' ' : '';
      switch ($key) {
        case 'case':
          $string .= $element->toString();
          break;

        case 'body':
          $string .= $open;
          if (!$element->isEmpty()) {
            $string .= "\n";
            $string .= $element->toString();
            $string .= $close;
          }
          break;

        case 'whitespace':
          $string .= $element;
          // Set space to zero for next item.
          $add_space = -1;
          break;
      }
      if ($add_space <= 0) {
        $add_space++;
      }
    }
    return $string;
  }
}

/**
 * Grammar Parser declare statement block class.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPDeclare extends PGPBase {
  /**
   * Documentation block comment.
   *
   * @var string
   */
  public $comment; // TODO Is this a valid item for this statement? Or a leftover item from copy and paste?

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * Class name.
   *
   * @var string
   */
  public $name; // TODO Is this a valid item for this statement? Or a leftover item from copy and paste?

  /**
   * Statement parameters.
   *
   * @var PGPList
   */
  public $parameters;

  /**
   * Indicates whether statement block uses alternative syntax.
   *
   * @var integer (or boolean???)
   */
  public $colon;

  /**
   * List of body statements.
   *
   * @var PGPBody
   */
  public $body;

  /**
   * End block token type when statement block uses alternative syntax.
   *
   * @var integer
   */
  public $end;

  public function __construct() {
    parent::__construct();

    $this->type = 0;
//    $this->colon = 0;
  }

  /**
   * Returns the text of a declare statement.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $open_brace = isset($expression->colon) ? ':' : '{';
    $close_brace = isset($expression->end) ? $expression->end . ";" : '}';

    $strings = array();
    // Test isset here or in the respective declares?
    if (isset($expression->comment) && !empty($expression->comment)) {
      $strings[] = PGPWriter::printComment($expression->comment, $indent);
    }
    $header = $indent . 'declare ';
    $header .= '(' . $expression->parameters->toString() . ')';
    if (!isset($expression->body)) {
      $header .= ";";
      $strings[] = $header;
    }
    else {
      $header .= " " . $open_brace;
      $strings[] = $header;
      $strings[] = $expression->body->toString();
      $strings[] = $indent . $close_brace;
    }
    return implode("\n", $strings);
  }
}

/**
 * Grammar Parser try-catch statement block class.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPTryCatch extends PGPBase {
  /**
   * Body comment.
   *
   * @var string
   */
  public $comment;

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * Catch exception expression.
   *
   * @var PGPExpression
   */
  public $exception;

  /**
   * List of body statements.
   *
   * @var PGPBody
   */
  public $body;

  public function __construct() {
    parent::__construct();

    $this->type = 0;
  }

  /**
   * Returns the text of a try-catch statement block.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    switch ($expression->type) {
      case T_TRY:
        $start = 'try';
        $open = ' {';
        break;

      case T_CATCH:
        $start = 'catch (';
        $open = ') {';
        break;
    }
    $close = "\n" . $indent . '}';

    // Add body comment (i.e. a comment following the block beginning token).
    $open .= $expression->comment ? ' ' . $expression->comment : '';

    $add_space = 0;
    $space = '';
    $string = $indent . $start;
    foreach ($expression as $key => $element) {
      if (!isset($element)) {
        continue;
      }
      $space = $add_space ? ' ' : '';
      switch ($key) {
        case 'type':
          $add_space = -1;
          break;

        case 'exception':
          $string .= $element->toString();
          break;

        case 'body':
          $string .= $open;
          if (!$element->isEmpty()) {
            $string .= "\n";
            $string .= $element->toString();
          }
          $string .= $close;
          break;

        case 'whitespace':
          $string .= $element;
          // Set space to zero for next item.
          $add_space = -1;
          break;
      }
      if ($add_space <= 0) {
        $add_space++;
      }
    }
    return $string;
  }
}

/**
 * Grammar Parser function call (or other) expression class.
 *
 * The other expressions built by this routine are: clone, [define,] empty,
 * eval, include, print, require, throw, or unset. Parentheses are not
 * required by an include, print, require, or throw expression. Define
 * expressions may have a T_DOC_COMMENT.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPFunctionCall extends PGPBase {
  /**
   * Parent node.
   *
   * @var PGPNode
   */
  public $parent;

  /**
   * Documentation block comment.
   *
   * @var string
   */
  public $comment;

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * Function call name (or list).
   *
   * @var string (or expression)
   */
  public $name;

  /**
   * Indicates whether function call parameters are enclosed in parentheses.
   *
   * @var integer (or boolean???)
   */
  public $noparens;

  /**
   * Statement parameters.
   *
   * @var PGPList
   */
  public $parameters;

  public function __construct() {
    parent::__construct();
    $this->debug = FALSE; // TRUE;

    $this->type = 0;
//    $this->noparens = 0;
  }

  /**
   * Returns the text of a function call (or other) expression (or statement).
   *
   * The other expressions printed by this routine are: clone, [define,] empty,
   * eval, include, print, require, throw, or unset. Parentheses are not
   * required by an include, print, require, or throw expression.
   *
   * From main loop add this to statements.
   * From a parameter, add this to parameters.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $strings = array();
    $noparens = 0;
    $string = '';

    $add_space = 0; // Set this to one in this routine. ???
    foreach ($expression as $key => $element) {
      $this->debugPrint(__METHOD__ . " key = $key");
      $space = $add_space ? ' ' : '';
      switch ($key) {
        case 'comment':
          // Define expressions may have a T_DOC_COMMENT.
          if (!empty($element)) {
            $strings[] = PGPWriter::printComment($element, $indent);
          }
          break;

        case 'name':
          // This key is used when the function name is a variable expression.
          if (is_object($element)) {
            if (method_exists($element, 'toString')) {
              $string .= $indent . $element->toString();
            }
            else {
              $string .= ' MISSING toString method ';
            }
          }
          elseif (is_array($element)) {
            $string .= $indent . $element['value']; // Should there be any other index?
          }
          else {
            $string .= $element;
          }
          break;

        case 'noparens':
          $noparens = $element;
          break;

        case 'parameters':
          // Add function call parameters.
          $params = $element->toString();
          if ($noparens) {
            $string .= $params !== '' ? ' ' . $params : '';
          }
          else {
            $string .= '(' . $params . ')';
          }
          break;
      }
      $this->debugPrint(__METHOD__ . " string = $string");
      if ($add_space <= 0) {
        $add_space++;
      }
    }
    // Add function call text.
    $strings[] = $string;

    return implode("\n", $strings);
  }

  /**
   * Returns the parameter count.
   *
   * @return integer
   *   The parameter count.
   */
  public function parameterCount() {
    if ($this->parameters instanceof PGPList) {
      return $this->parameters->count();
    }
    return 0;
  }

  /**
   * Returns the parameter at the specified index.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @return PGPExpression
   *   The parameter object.
   */
  public function &getParameter($index = 0) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        return $this->parameters->get($index)->data;
      }
    }

    $result = FALSE;
    return $result;
  }

  /**
   * Sets the parameter at the specified index to the indicated expression.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @param PGPExpression $expression
   *   The expression object.
   */
  public function setParameter($index, $expression) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        $this->parameters->updateKey($index, $expression);
      }
      else {
        $this->parameters->insertLast($expression);
      }
    }
  }

  /**
   * Inserts the indicated expression as the parameter at the specified index.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @param PGPExpression $expression
   *   The expression object.
   */
  public function insertParameter($index, $expression) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        $this->parameters->insertBefore($this->parameters->get($index), $expression);
      }
      else {
        $this->parameters->insertLast($expression);
      }
    }
  }

  /**
   * Deletes the parameter at the specified index.
   *
   * @param integer $index
   *   The index in the parameter list.
   */
  public function deleteParameter($index = 0) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        $this->parameters->delete($this->parameters->get($index));
      }
    }
  }

  /**
   * Returns the text of the parameter at the specified index.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @return string
   *   The text of the parameter.
   */
  public function printParameter($index = 0) {
    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        return $this->parameters->get($index)->data->toString();
      }
    }
    return '';
  }

  /**
   * Returns the (comma-separated) text of the function call parameters.
   *
   * @return string
   *   The comma-separated text of the parameters.
   */
  public function printParameters() {
    return $this->parameters->toString();
  }

  /**
   * Clears the parameters.
   */
  public function clearParameters() {
    $this->parameters->clear();
  }

  /**
   * Returns the parameter variable at the specified index.
   *
   * The parameter variable is the first operand at the specified index,
   * stripped of inline comments included with the operand. Selecting only the
   * first operand also removes any default value assignment or inline comments
   * preceding the operand. Because this is not the entire parameter expression,
   * the object is not returned as a reference.
   *
   * @param integer $index
   *   The index in the parameter list.
   *
   * @return PGPExpression
   *   The parameter variable object.
   */
  public function getParameterVariable($index = 0) {
//    $this->debugPrint(__METHOD__);

    if ($this->parameters instanceof PGPList) {
      if ($this->parameters->count() > $index) {
        if ($parameter = /*&*/$this->parameters->get($index, FALSE)) { // TODO Are these '&' necessary?
//          cdp($parameter, '$parameter');
          if ($parameter->isType(T_VARIABLE)) {
            if ($variable = /*&*/$parameter->findNode('operand')) {
//              cdp($variable, '$variable');
              // Strip any embedded comments or whitespace included with the operand.
              return $variable->stripComments();
            }
          }
        }
      }
    }

    $result = FALSE;
    return $result;
  }

  /**
   * Returns TRUE if the member key matches the search value.
   *
   * Usage
   * - search for function call named foo()
   * - search('PGPFunctionCall', 'name', 'value', 'foo');
   * - search for a parameter of $foo
   * - search('PGPFunctionCall', 'parameters', 0, '$foo');
   * - search for a parameter of 'foo'
   * - search('PGPFunctionCall', 'parameters', 0, "'foo'");
   *
   * @param string $member
   *   Name of the member to evaluate.
   * @param string $key
   *   Name of the key of member to evaluate.
   * @param string $value
   *   Element value to be matched.
   * @return boolean
   */
  public function matches($member, $key, $value) {
    $this->debugPrint(__METHOD__);

    if (!isset($this->$member)) return FALSE;

    switch ($member) {
      case 'name':
        return (is_array($this->name) && isset($this->name['value']) && $this->name['value'] == $value);
        break;

      case 'parameters':
        $parameter = $this->getParameter($key);
        // TODO Clean whitespace and comments, then evaluate; or loop and count operands
        if ($parameter->count() != 1) return FALSE;
        $operand = $parameter->getElement();
        $type = $operand->getElement(0);
        $value2 = $operand->getElement(1);
        return ($type == T_VARIABLE && $value2 == $value);
        break;
    }
    return FALSE;
  }

  /**
   * Inserts a statement before the statement containing the function call.
   *
   * @param mixed $statement
   *   The statement object (or array) to store in the parent container.
   * @return mixed
   *   The inserted statement object (or array).
   */
  public function &insertStatementBefore($statement) {
//    $this->debugPrint(__METHOD__);
    // Check type to be PGPBase or array.
    if (!($statement instanceof PGPBase) && !is_array($statement)) {
      $node = NULL;
      return $node;
    }

    if (!isset($this->parent)) {
      $this->debugPrint(__METHOD__);
      $this->debugPrint("No parent to expression");
    }

    $node = &$this->parent->insertStatementBefore($statement);

    return $node;
  }

  /**
   * Inserts a statement after the statement containing the function call.
   *
   * @param mixed $statement
   *   The statement object (or array) to store in the parent container.
   * @return mixed
   *   The inserted statement object (or array).
   */
  public function &insertStatementAfter($statement) {
//    $this->debugPrint(__METHOD__);
    // Check type to be PGPBase or array.
    if (!($statement instanceof PGPBase) && !is_array($statement)) {
      $node = NULL;
      return $node;
    }

    if (!isset($this->parent)) {
      $this->debugPrint(__METHOD__);
      $this->debugPrint("No parent to expression");
    }

    $node = &$this->parent->insertStatementAfter($statement);

    return $node;
  }

  /**
   * Returns a function call object with comment and whitespace items removed from the parameter expressions.
   *
   * @return PGPFunctionCall
   *   The function call object with comment and whitespace items removed.
   */
  public function stripComments() {
    if (!$this->parameterCount()) {
      return clone $this; // new PGPExpression(); // @todo Seems the references still refer to same value.
    }

    $stripped = new PGPFunctionCall(); // $stripped = clone $this; // Does this have issues with the parent reference? // new PGPExpression();
    $stripped->comment = $this->comment;
    $stripped->type = $this->type;
    $stripped->name = $this->name;
    $stripped->noparens = $this->noparens;
    $stripped->parameters = new PGPList();

    if ($this->parameters instanceof PGPList) {
      for ($index = 0; $index < $this->parameterCount(); $index++) {
        $stripped->setParameter($index, $this->getParameter($index)->stripComments());
      }
    }
    return $stripped;
  }

  /**
   * These two functions are convenience wrappers to PGPList::search().
   */
  public function &searchForward($class, $member, $key, $value, $return_node = FALSE) {
    if ($this->parent instanceof PGPNode && $this->parent->container instanceof PGPList) {
      // Set this variable as otherwise search will modify the parent property.
      $parent = $this->parent;
      return $this->parent->container->search($class, $member, $key, $value, $return_node, 'forward', $parent);
    }
    $var = FALSE;
    return $var;
  }

  public function &searchBackward($class, $member, $key, $value, $return_node = FALSE) {
    if ($this->parent instanceof PGPNode && $this->parent->container instanceof PGPList) {
      // Set this variable as otherwise search will modify the parent property.
      $parent = $this->parent;
      return $this->parent->container->search($class, $member, $key, $value, $return_node, 'backward', $parent);
    }
    $var = FALSE;
    return $var;
  }
}

/**
 * Grammar Parser assignment expression or statement (or similar statements) class.
 *
 * The other statements built by this routine are:
 * - a constant definition statement
 * - a global definition statement
 * - a variable definition (older PHP style for backwards compatability)
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPAssignment extends PGPBase {
  /**
   * Documentation block comment.
   *
   * @var string
   */
  public $comment;

  /**
   * Scope modifiers (e.g. public, private) for the statement block.
   *
   * @var array
   */
  public $modifiers;

  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * List of assignment expressions.
   *
   * @var PGPList
   */
  public $values;

  /**
   * Parameter on a T_CONTINUE, T_ECHO, T_RETURN, or T_EXIT.
   *
   * @var PGPExpression
   */
  public $value;

  public function __construct() {
    parent::__construct();

    $this->modifiers = array();
    $this->type = 0;
  }

  /**
   * Returns the text of an assignment expression (or statement).
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $add_space = 0;
    $space = '';
    $string = '';
    // Variable definitions and constants may have a T_DOC_COMMENT.
    if (isset($expression->comment) && !empty($expression->comment)) {
      $string .= PGPWriter::printComment($expression->comment, $indent) . "\n";
    }

    $space = ' ';
    $string .= $indent;
    if (isset($expression->modifiers) && !empty($expression->modifiers)) {
      $space = ' ';
      $string .= implode(' ', $expression->modifiers) . $space;
    }
    if (isset($expression->type)) {
      $type = PGPWriter::statementTypeToString($expression->type);
      if ($type != 'UNKNOWN-TYPE') {
        // As a general rule add the space before the element so we can control whitespace
        // Here for example, if this is return or exit without a parameter, then no space is needed.
        $string .= $type;
        $add_space = 1;
      }
    }

    // Applies to assignment expressions (or statements).
    $space = $add_space ? ' ' : '';
    if (isset($expression->values)) {
      if (!$expression->values->isEmpty()) {
        // Handle empty parameters that are in a PGPList.
        $temp = $expression->values->toString();
        $string .= $temp ? $space . $temp : '';
      }
      return $string;
    }

    // Applies to T_CONTINUE, T_ECHO, T_RETURN, T_EXIT.
    if (isset($expression->value) /*&& is_object($expression->value)*/) {
      if (($temp = $expression->value->toString()) !== '') {
        $string .= $space . $temp;
      }
//      $string .= $temp ? $space . $temp : ''; // This works too.
      return $string;
    }
  }

  /**
   * Returns the text of an assignment expression (or statement).
   *
   * @todo This works except for the $indent (need to check type and modifiers
   *   before value or values).
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   *//*
  public function toString2($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $add_space = 0;
    $space = '';
    $string = '';

    foreach ($expression as $key => $element) {
      $this->debugPrint(__METHOD__ . " key = $key");
      $space = $add_space ? ' ' : '';
      switch ($key) {
        case 'comment':
          // Variable definitions and constants may have a T_DOC_COMMENT.
          if (!empty($element)) {
            $string .= PGPWriter::printComment($element, $indent) . "\n";
          }
          break;

        case 'modifiers':
          if ($element) {
            $string .= $indent . implode(' ', $element);
          }
          break;

        case 'type':
          $type = PGPWriter::statementTypeToString($element);
          if ($type != 'UNKNOWN-TYPE') {
            // As a general rule add the space before the element so we can control whitespace
            // Here for example, if this is return or exit without a parameter, then no space is needed.
            $space = $expression->modifiers ? ' ' : $indent;
            $string .= $space . $type;
            $add_space = 1;
          }
          break;

        case 'values':
          // Applies to assignment expressions (or statements).
          if (is_a($element, 'PGPList')) { // if (!$expression->values->isEmpty()) {
            // Handle empty parameters that are in a PGPList.
            $temp = $element->toString();
            $string .= $temp ? $space . $temp : '';
          }
          break;

        case 'value':
          // Applies to T_CONTINUE, T_ECHO, T_RETURN, T_EXIT.
          if (is_a($element, 'PGPExpression')) {
            if (($temp = $element->toString()) !== '') {
              $string .= $space . $temp;
            }
//            $string .= $temp ? $space . $temp : ''; // This works too.
          }
          break;
      }
      if ($add_space <= 0) {
        $add_space++;
      }
    }
    return $string;
  }*/

  /**
   * Returns TRUE if the member key matches the search value.
   *
   * Usage
   * - search for function call named foo()
   * - search('PGPFunctionCall', 'name', 'value', 'foo');
   * - search for a parameter of $foo
   * - search('PGPFunctionCall', 'parameters', 0, '$foo');
   * - search for a parameter of 'foo'
   * - search('PGPFunctionCall', 'parameters', 0, "'foo'");
   *
   * - search for assignment to variable $mask
   * - searchForward('PGPAssignment', 'values', 0, '$mask')
   * - we need to pass this to PGPAssignment->assignsTo()
   * - here the zero tells it to look at the first operand (which would be the assignee)
   * - if we use toString, then we need to strip whitespace and comment from operand
   * - otherwise assignsTo() will know to look at the value and the operand has to be T_VARIABLE
   *
   * @param string $member
   *   Name of the member to evaluate.
   * @param string $key
   *   Name of the key of member to evaluate.
   * @param string $value
   *   Element value to be matched.
   * @return boolean
   */
  public function matches($member, $key, $value) {
    $this->debugPrint(__METHOD__);
//    $this->debug = TRUE;

    if (!isset($this->$member)) return FALSE;

    switch ($member) {
//      case 'name':
//        return (is_array($this->name) && isset($this->name['value']) && $this->name['value'] == $value);
//        break;

      case 'values':
        $expression = $this->values->getElement($key);
        // TODO Clean whitespace and comments, then evaluate; or loop and count operands
//        if ($expression->count() != 1) return FALSE;
        $operand = $expression->getElement();
        if ($operand instanceof PGPOperand) {
          $this->debugPrint("expression");
          $this->debugPrint($expression);
          $type = $operand->getElement(0);
          $value2 = $operand->getElement(1);
          $this->debug = FALSE;
          return ($type == T_VARIABLE && $value2 == $value);
        }
        break;
    }
    return FALSE;
  }

  /**
   * Returns the assignment value expression.
   *
   * @param boolean $strip
   *   Indicates whether to strip the expression of inline comments and whitespace.
   * @return PGPExpression
   */
  function getValue($strip = TRUE) {
    $this->debugPrint(__METHOD__); // cdp(__METHOD__);

    $stripped = new PGPExpression();
    $found = FALSE;

    // Remove the assignee and the assignment operator.
    $current = $this->values->getElement()->first();
    while ($current->next != NULL) {
      $type = $current->type;
      $data = $current->data; // Copy this?
      $this->debugPrint(__METHOD__ . " type = $type");

      if ($current->type == 'assign') {
        $found = TRUE;
      }
      elseif ($found && !in_array($current->type, array('comment', 'whitespace'))) {
        if (is_object($data) && method_exists($data, 'stripComments')) {
          $stripped->insertLast($data->stripComments(), $current->type);
        }
        else {
          $stripped->insertLast($data, $current->type);
        }
      }
      $current = $current->next;
    }
    return $stripped;
  }

}

/**
 * Grammar Parser list statement class.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPListStatement extends PGPBase {
  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * List name.
   *
   * @var string
   */
  public $name;

  /**
   * Indicates whether the list parameters are enclosed in parentheses.
   *
   * @var integer (or boolean???)
   */
  public $noparens;

  /**
   * Statement parameters.
   *
   * @var PGPList
   */
  public $parameters;

  /**
   * Assignment expression.
   *
   * @var PGPExpression
   */
  public $assignment;

  public function __construct() {
    parent::__construct();

    $this->type = 0;
//    $this->noparens = 0;
  }

  /**
   * Returns the text of a list statement.
   *
   * @param string $indent
   *   Line indent string.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $expression = $this; // TODO Why use this variable name now?

    $noparens = 0;
    $string = '';
    $space = ' ';

    foreach ($expression as $key => $element) {
      $this->debugPrint(__METHOD__ . " key = $key");
      switch ($key) {
        case 'name':
          $string .= $element;
          break;

        case 'parameters':
          // A PGPList object.
          $string .= '(' . $element->toString() . ')';
          break;

        case 'assignment':
          // A PGPExpression object.
          if ($element instanceof PGPExpression) {
            $string .= $space . $element->toString();
          }
          else {
            // TODO This should not occur!!!
            $string .= $space . $element->toString();
          }
          break;

        case 'noparens':
          // This should never be true for a list statement.
          $noparens = $element;
          break;
      }
    }
    return $string;
  }
}

/**
 * Grammar Parser array statement class.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPArray extends PGPBase {
  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  /**
   * Whether the array is to be printed on multiple lines.
   *
   * @var boolean
   */
  public $multiline;

  /**
   * Whether the original array is on multiple lines.
   *
   * @var boolean
   */
  public $original;

  /**
   * Whether to preserve the original formatting.
   *
   * @var boolean
   */
  public $preserve;

  /**
   * Number of values.
   *
   * @var integer
   */
  public $count;

  /**
   * Number of commas following value expressions.
   *
   * @var integer
   */
  public $commaCount;

  /**
   * Array elements (keys, values, and comments).
   *
   * @var PGPList
   */
  public $values;

  public function __construct() {
    parent::__construct();

    $this->type = 0;
  }

  /**
   * Returns the text of an array expression.
   *
   * TODO This code will not handle a mixture of keyed and non-keyed elements.
   *
   * @param string $indent
   *   Line indent string.
   * @param boolean $multiline
   *   A flag indicating whether to print the array inline or multiline.
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '', $multiline = FALSE) {
    $this->debugPrint(__METHOD__);
    $this->debugPrint("indent = '$indent'");

    $expression = $this; // TODO Why use this variable name now?
    $multiline = $this->multiline;
    $original = $this->original;

    PGPWriter::$indent++;
    $indent = str_repeat('  ', PGPWriter::$indent);
    $this->debugPrint("indent = '$indent'");
    $newline = $multiline ? "\n" . $indent : '';

    $lparens = 0;
    $rparens = 0;
    $operands = 0;
    $keys = 0;
    $commas = 0;
//    $add_comma = 0;

    $add_space = 0;
    $space = ''; // Never used; it is calculated on each while iteration
    $string = 'array';

    $current = $expression->values->first();
    while ($current->next != NULL) {
      $key = $current->type;
      $element = $current->data;

      $this->debugPrint(__METHOD__ . " key = $key");
      $this->debugPrint($element);

      $space = $add_space ? ' ' : '';
      switch ($key) {
//        case 'variable':
//          $string .= $this->printExpression($element);
//          break;

        case 'key':
          $this->debugPrint("printing key in printArray");
          $this->debugPrint($element);
          $keys++;
          $separator = '';
          if ($original) {
            // Add space after comma and before key if otherwise none.
            $separator = (($keys > 1 && $current->previous->type != 'whitespace') ? ' ' : '');
          }
          else { // if (!$original) {
            // Add space after comma and before key if inline.
            $separator = (($keys > 1) ? ($multiline ? '' : ' ') : '');
          }
          // Change previous line to check for whitespace in $element.
          $space = '';
          $string .= $separator . $newline . $element->toString();
          $space = ' '; // Set this for the assign case.
//          $add_comma = 1;
          break;

        case 'value':
          $commas++;
          $string .= (($keys == 0 && $multiline) ? $newline : $space) . $element->toString();
//          $string .= ($commas < $this->count || (($multiline | $original) && $commas == $this->count)) ? ',': '';
          $string .= ($commas < $this->count) ? ',' : '';
//          $string .= ($commas == $this->count && $this->commaCount == $this->count) ? ',': '';
          if ($commas == $this->count) {
            if ($this->preserve) {
              $string .= ($this->commaCount == $this->count) ? ',' : '';
            }
            else {
              $string .= ($this->multiline) ? ',' : '';
            }
          }
          $this->debugPrint("printing value in printArray");
          $this->debugPrint($string);
          $this->debugPrint("");
          break;

        case 'assign':
          $string .= $space . $element;
          break;

        case 'operand': // TODO Is this key ever encountered?
          $operands++;
          $string .= (($operands > 1) ? ', ' : '') . $element->toString();
          break;

        case 'lparens':
          $lparens++;
          // Add space for array('@rid' => (isset($role->rid) ? $role->rid : t('-n/a-')))
          $string .= $space . $element;
          $add_space = -1;
          break;

        case 'rparens':
          $rparens++;
          $string .= $element;
          break;

        case 'whitespace':
          if ($this->preserve) { // 2010-03-09
            $string .= $element;
            // Set space to zero for next item.
            $add_space = -1;
          }
          break;

        case 'comment':
          if (isset($element['append']) && $element['append']) {
            // Handle a regular comment appended to a line.
            $string .= $space . PGPWriter::printComment($element);
          }
          else {
            $string .= /*$indent*/$newline . PGPWriter::printComment($element);
          }
          break;
      }
      if ($add_space <= 0) {
        $add_space++;
      }

      $current = $current->next;
    }
    $this->debugPrint("$string");
    PGPWriter::$indent--;
    if ($multiline) {
      $indent = str_repeat('  ', PGPWriter::$indent);
      $newline = $multiline ? "\n" . $indent : '';
      $string .= $newline;
    }
    $this->debugPrint("$string");
    if ($lparens > $rparens) {
      $string .= ')';
    }
    $this->debugPrint("$string");
    return $string;
  }

  /**
   * Returns the array key object with the specified value.
   *
   * @param string $value
   *   The key value to search for.
   * @return PGPExpression
   *   The key object.
   */
  public function &findKey($value) {
    $this->debugPrint(__METHOD__);

    if ($this->values instanceof PGPList) {
      $current = $this->values->first();
      while ($current->next != NULL) {
        $type = $current->type;
        if ($type == 'key' && $current->data->trim() == $value) {
          return $current->data;
        }
        $current = &$current->next;
      }
    }

    $result = FALSE;
    return $result;
  }

  /**
   * Changes the value of the key object with the specified value.
   *
   * @param string $old_value
   *   The key value to search for.
   * @param string $new_value
   *   The new key value.
   * @return boolean
   *   TRUE if the key was found and the change was made.
   */
  public function changeKey($old_value, $new_value) {
    $this->debugPrint(__METHOD__);

    if ($this->values instanceof PGPList) {
      $current = $this->values->first();
      while ($current->next != NULL) {
        $type = $current->type;
        if ($type == 'key' && $current->data->trim() == $old_value) {
          if (!$current->data->isType(T_CONSTANT_ENCAPSED_STRING)) {
            return FALSE;
          }
          $element = &$current->data->getElement();
          $element['value'] = $new_value;
          return TRUE;
        }
        $current = &$current->next;
      }
    }

    return FALSE;
  }

  /**
   * Returns the array key object at the specified index.
   *
   * @param integer $value
   *   The index of the array key.
   * @return PGPExpression
   *   The key object.
   */
  public function &getKey($index = 0) {
    $this->debugPrint(__METHOD__);

    if ($this->values instanceof PGPList) {
      $count = -1;
      $current = $this->values->first();
      while ($current->next != NULL) {
        $type = $current->type;
        if ($type == 'key') {
          if (++$count == $index) {
            return $current->data;
          }
        }
        $current = &$current->next;
      }
    }

    $result = FALSE;
    return $result;
  }

  /**
   * Returns the array value object at the specified index.
   *
   * @param integer $index
   *   The index of the array value.
   * @return PGPExpression
   *   The value object.
   */
  public function &getValue($index = 0) {
    $this->debugPrint(__METHOD__);

    if ($this->values instanceof PGPList) {
      $count = -1;
      $current = $this->values->first();
      while ($current->next != NULL) {
        $type = $current->type;
        if ($type == 'value') {
          if (++$count == $index) {
            return $current->data;
          }
        }
        $current = &$current->next;
      }
    }

    $result = FALSE;
    return $result;
  }

  /**
   * Sets the parameter at the specified index to the indicated expression.
   *
   * @param integer $index
   *   The index in the parameter list.
   * @param PGPExpression $expression
   *   The expression object.
   *//*
  public function setKey($index, $expression) {
    $this->debugPrint(__METHOD__);

    if (is_a($this->values, 'PGPList')) {
      if ($this->values->count() > $index) {
        $this->values->updateKey($index, $expression);
      }
      else {
        $this->values->insertLast($expression, 'key');
      }
    }
  }*/

  /**
   * Returns the value object for the specified key.
   *
   * @param string $key
   *   The key value to search for.
   * @return PGPExpression
   *   The value object.
   */
  public function &findValue($key = NULL) {
    $this->debugPrint(__METHOD__);

    if (is_null($key)) {
      $result = FALSE;
      return $result;
    }

    if ($this->values instanceof PGPList) {
      $current = $this->values->first();

      // This could be separated into two while loops.
      $find = 'key';
      while ($current->next != NULL) {
        $type = $current->type;
        if ($find == 'key' && $type == 'key' && $current->data->trim() == $key) {
          $find = 'value';
        }
        if ($find == 'value' && $type == 'value') {
          return $current->data;
        }
        $current = &$current->next;
      }
    }

    $result = FALSE;
    return $result;
  }

  /**
   * Advances the list pointer to the next value object.
   *
   * @param string $current
   *   The key object to search from.
   * @return boolean
   *   TRUE if a value object was found.
   */
  public function findNextValue(&$current = NULL) {
    $this->debugPrint(__METHOD__);

    if ($this->values instanceof PGPList) {
      if (is_null($current)) {
        $current = $this->values->first();
      }

      $current = $current->next;
      while ($current->next != NULL) {
        if ($current->type == 'value') {
          return TRUE;
        }
        $current = $current->next;
      }
      if ($current->next == NULL) {
        $this->debugPrint("ERROR: did not find a value expression");
        return FALSE;
      }
    }

    return FALSE;
  }

  /**
   * Deletes the entry (key-value pair) with the specified key.
   *
   * @param string $key
   *   The key value to search for.
   * @return boolean
   *   TRUE if the entry was found and deleted.
   */
  public function deleteKey($key = NULL) {
    $this->debugPrint(__METHOD__);

    if (is_null($key)) {
      return FALSE;
    }

    // This is findKey but returning the node instead of the data.
    if ($this->values instanceof PGPList) {
      $current = $this->values->first();
      while ($current->next != NULL) {
        if ($current->type == 'key' && $current->data->trim() == $key) {
          break;
        }
        $current = &$current->next;
      }
      if ($current->next == NULL) {
        $this->debugPrint("ERROR: did not find a key expression");
        return FALSE;
      }

      // Delete the current and subsequent items until the next key or the second value.
      // This allows for embedded and end-of-line comments.
      $current = &$current->next;
      $this->values->delete($current->previous);
      $this->commaCount--;
      $this->count--;
      $keys = $values = 0;
      while ($current->next != NULL) {
        $keys += $current->type == 'key' ? 1 : 0;
        $values += $current->type == 'value' ? 1 : 0;

        if ($keys || $values > 1) {
          $this->debugPrint('$this->values');
          $this->debugPrint($this->values);
          return TRUE;
        }
        $current = &$current->next;
        $this->values->delete($current->previous);
      }
      // The element being deleted is the last element in the array.
      return TRUE;
    }

    return FALSE;
  }

  /**
   * Deletes the entry (key-value pair) with the specified value.
   *
   * @param string $value
   *   The element value to search for.
   * @return boolean
   *   TRUE if the entry was found and deleted.
   *//*
  public function deleteValue($value = NULL) {
    $this->debugPrint(__METHOD__);

    if (is_null($value)) {
      return FALSE;
    }

    if (is_a($this->values, 'PGPList')) {
      $current = $this->values->first();
    }

    return FALSE;
  }*/

  /**
   * Returns the value object for the specified key.
   *
   * @param integer $index
   *   The index of the array value (use when no keys, only values). (NOT DONE)
   * @param string $key
   *   The key value to search for.
   * @return PGPExpression
   *   The value object.
   *//*
  public function &getValue_OLD($index = -1, $key = NULL) {
    $this->debugPrint(__METHOD__);
//    $this->debugPrint("key = $key");

    if ($index == -1 && is_null($key)) {
      $result = FALSE;
      return $result;
    }

    if (is_a($this->values, 'PGPList')) {
      $current = $this->values->first();

      $count = -1;
      $find = 'key';
      while ($current->next != NULL) {
//        echo $current->type . "\n";
        $type = $current->type;
        if ($find == 'key' && $type == 'key' && $key == $current->data->toString()) {
          $find = 'value';
//          if (++$count == $index) {
//            return $current->data;
//          }
        }
        if ($find == 'value' && $type == 'value') {
          return $current->data;
        }
        $current = &$current->next;
      }
    }

    $result = FALSE;
    return $result;
  }*/

  /**
   * Returns the text of the array keys in an array.
   *
   * @return array
   *   An array of the keys.
   */
  public function keysToArray() {
    $this->debugPrint(__METHOD__);

    $keys = array();
    if ($this->values instanceof PGPList) {
      $current = $this->values->first();
      while ($current->next != NULL) {
        $type = $current->type;
        if ($type == 'key') {
          $keys[] = $current->data->toString();
        }
        $current = &$current->next;
      }
    }

    return $keys;
  }

  /**
   * Returns the elements of the array consisting solely of plain text.
   *
   * @return array
   *   An array of the text keys and values.
   */
  public function toArray() {
    $this->debugPrint(__METHOD__);

    $return = array();
    if ($this->values instanceof PGPList) {
      $key = NULL;
      $current = $this->values->first();
      while ($current->next != NULL) {
        $type = $current->type;
        if ($type == 'key') {
          $key_is_string = FALSE;
//           $key = NULL;
          $key_expression = $current->data->stripComments();
          if ($key_expression->countType('operand') == 1) {
            $operand = $key_expression->getElement();
            if (is_array($operand) && $operand['type'] == T_CONSTANT_ENCAPSED_STRING) {
              // The key is a simple string.
              $key = $key_expression->trim();
              $key_is_string = TRUE;
            }
          }
        }
        elseif ($type == 'value') {
          if (isset($key) && !$key_is_string) {
            continue;
          }
          $value = NULL;
          $value_isset = FALSE;
          $value_expression = $current->data->stripComments();
          if ($value_expression->countType('operand') == 1) {
            $operand = $value_expression->getElement();
            if (is_array($operand) && $operand['type'] == T_CONSTANT_ENCAPSED_STRING) { // if (is_array($operand) && in_array($operand['type'], array(T_STRING, T_CONSTANT_ENCAPSED_STRING))) {
              // The key is a simple string.
              $value = $value_expression->trim();
              $value_isset = TRUE;
            }
            elseif ($operand instanceof PGPOperand) {
              // T_STRING values like NULL.
              $value = $operand->toString();
              $value_isset = TRUE;
            }
            elseif ($operand instanceof PGPArray) {
              $value = $operand->toArray();
              $value_isset = TRUE;
            }
            elseif (is_numeric($operand)) {
              $value = $operand;
              $value_isset = TRUE;
            }
          }
          if (isset($value)) { // if ($value_isset) { // if (isset($value)) {
            if (isset($key)) {
              $return[$key] = $value;
            }
            else {
              $return[] = $value;
            }
          }
        }
        $current = &$current->next;
      }
    }

    return $return;
  }

  /**
   * Invokes callback on array elements.
   *
   * @param PGPNode $node
   *   The node of the statement containing this array.
   *   This is useful as a reference for inserting statements, etc.
   * @param string $hook
   *   The hook name.
   * @param string $callback
   *   Name of a callback function.
   * @param integer $start_depth
   *   The starting depth of nested arrays to traverse.
   * @param integer $remaining_depth
   *   The remaining depth of nested arrays to traverse. When equal to zero, stop.
   * @param integer $depth
   *   The actual depth of current nested array. (For internal use only.)
   *
   * @return mixed
   *   NULL or a list of operands as indicated by the callback.
   */
  function traverse2($node, $hook, $callback, $start_depth = 0, $remaining_depth = -1, $depth = 0) {
    $this->debugPrint(__METHOD__);
//    cdp(__METHOD__);
//    $callback = $callback == '' ? "coder_upgrade_callback_$hook" : $callback;

    if (!($this->values instanceof PGPList)) {
//      cdp('$this->values != PGPList');
      return;
    }

    if (!function_exists($callback)) {
//      cdp('!function_exists($callback)');
      return;
    }

    $items = array();
    $key = '';
    $current = $this->values->first();
    while ($current->next != NULL) {
      if ($current->type == 'key' /*&& $depth == $start_depth*/) {
        if ($depth >= $start_depth) { // if ($depth == $start_depth) { // @todo Why was this on equality only???
          $key = $current->data->trim(); // TODO May not want to do trim and toString here?
//           cdp(__METHOD__); cdp($key, '$key');
          $followup = $callback($node, $this, $current, $hook, $current->type, $key, NULL);
          if ($followup) {
            $items[] = $current->data;
          }
        }
      }
      elseif ($current->type == 'value') {
        if ($depth >= $start_depth) {
          $value = $current->data->trim();
//          cdp(__METHOD__); cdp($value, '$value');
//          cdp($node->data->toString(), '$node');
          $followup = $callback($node, $this, $current, $hook, $current->type, $key, $value);
          $key = ''; // Clear key in case not all elements have keys.
          if ($followup) {
            $items[] = $current->data;
          }
        }

        if (!is_object($current->data)) {
//           cdp('!is_object($current->data)');
//           cdp($current->data, '$current->data');
        }
        elseif ($current->data->isType(T_ARRAY) && (($depth < $start_depth) || ($remaining_depth !== 0))) { // ($remaining_depth == -1 || $remaining_depth > 0)) {
//           cdp('Recurse another level');
          // Recurse another level on array of values.
          $array = $current->data->getElement(); // TODO Is this comment-safe?
//           cdp($array->toString(), '$array');
          $items = array_merge($items, $array->traverse2($node, $hook, $callback, $start_depth /*- 1*/, $remaining_depth - 1, $depth + 1));
        }
      }
      $current = $current->next;
    }

    return $items;
  }
}
