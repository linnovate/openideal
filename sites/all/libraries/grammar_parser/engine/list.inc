<?php

/**
 * @file
 * A member of the Grammar Parser API classes. These classes provide an API for
 * parsing, editing and rebuilding a code snippet.
 *
 * Copyright 2009 by Jim Berry ("solotandem", http://drupal.org/user/240748)
 */

/**
 * Grammar Parser node class.
 *
 * This class provides a container for storing a grammar statement along with
 * pointers to the statements surrounding it and its containing list object.
 */
class PGPNode {

  /**
   * The PGPList object containing the node.
   *
   * @var PGPList
   */
  public $container; // Or $list

  /**
   * The node statement type.
   *
   * @var mixed (integer or string)
   */
  public $type;

  /**
   * The data contained by the node.
   *
   * @var mixed
   */
  public $data;

  /**
   * Reference to the next node in the containing list.
   *
   * @var PGPNode
   */
  public $next;

  /**
   * Reference to the previous node in the containing list.
   *
   * @var PGPNode
   */
  public $previous;

  /**
   * The line number containing the node.
   *
   * @var integer
   */
  public $line;

  public function __construct(/*&*/$data, /*&*/$container) {
    $this->data = /*&*/$data;
    $this->container = /*&*/$container;
  }

  /**
   * Sets type and data properties of the node.
   *
   * @param integer $type
   * @param mixed $data
   */
  public function setData($type, $data) {
    $this->type = $type;
    $this->data = $data;
//    $this->debugPrint("this->type = " . $this->type . "");
  }

  /**
   * Returns statement type.
   *
   * @todo This and next function are not called.
   * @return string
   */
  public function statementType() {
    if (is_object($this->data)) {
      return $this->data->type;
    }
    elseif (is_array($this->data)) {
      return $this->data['type'];
    }
    return $this->data;
  }

  /**
   * Returns statement value.
   *
   * @param string $key
   *   The key for the value to return.
   * @return string
   */
  public function statementValue($key) {
    if (is_object($this->data)) {
      return $this->data->type;
    }
    elseif (is_array($this->data) && isset($this->data[$key])) {
      return $this->data[$key];
    }
    return $this->data;
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
   * Inserts a statement before the statement containing the expression.
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

    if (!isset($this->container)) {
      $this->debugPrint(__METHOD__);
      $this->debugPrint("No container to node");
    }

    $node = &$this->container->insertBefore($this, $statement);

    return $node;
  }

  /**
   * Inserts a statement after the statement containing the expression.
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

    if (!isset($this->container)) {
      $this->debugPrint(__METHOD__);
      $this->debugPrint("No container to node");
    }

    $node = &$this->container->insertAfter($this, $statement);

    return $node;
  }
}

/**
 * Grammar Parser list class.
 *
 * This class provides a custom doubly linked list data structure.
 *
 * If we create sentinels for the first and last nodes, then we can avoid all
 * the checking surrounding these.
 */
class PGPList {

  /**
   * The PGPNode object containing the list.
   *
   * @var PGPNode
   */
  public $parent;

  /**
   * The first node in the list.
   *
   * @var PGPNode
   */
  private $first;

  /**
   * The last node in the list.
   *
   * @var PGPNode
   */
  private $last;

  /**
   * The number of nodes in the list.
   *
   * @var integer
   */
  private $count;

  /**
   * Whether to print debug information.
   *
   * @var boolean
   */
  protected $debug = FALSE; // TRUE; //

  public function __construct() {
    $data = NULL;
    $this->first = new PGPNode($data, $this);
    $this->last = new PGPNode($data, $this);

    $this->first->next = &$this->last;
    $this->last->previous = &$this->first;

    $this->count = 0;
  }

  public function isEmpty() {
    return ($this->count == 0);
  }

  public function &first() {
//    return $this->isEmpty() ? NULL : $this->first->next;
    return $this->first->next; // TODO Not defined if empty!!!
  }

  public function &last() {
//    return $this->isEmpty() ? NULL : $this->last->previous;
    return $this->last->previous; // TODO Not defined if empty!!!
  }

  /**
   * Inserts a node with the specified data at the beginnig of the list.
   *
   * @param mixed $data
   *   The data to store on the node.
   * @return PGPNode
   *   The inserted node.
   */
  public function &insertFirst(/*&*/$data, $type = NULL) {
    return $this->insertAfter($this->first, $data, $type);
  }

  /**
   * Inserts a node with the specified data at the end of the list.
   *
   * This is the most common method when building the list.
   *
   * @param mixed $data
   *   The data to store on the node.
   * @return PGPNode
   *   The inserted node.
   */
  public function &insertLast(/*&*/$data, $type = NULL) {
    return $this->insertBefore($this->last, $data, $type);
  }

  /**
   * Inserts a node with the specified data before the specified node.
   *
   * This is the most common method when editing the list.
   *
   * @param PGPNode $current
   *   The specified node.
   * @param mixed $data
   *   The data to store on the node.
   * @return PGPNode
   *   The inserted node.
   */
  public function &insertBefore(&$current, /*&*/$data, $type = NULL) { // Comment '&' for Coder Upgrade
//    $this->debugPrint(__METHOD__);
    if ($current == NULL) {
      $node = NULL;
      return $node;
    }

    // This comparison was intended for integers.
    if ($current == $this->first) {
//      $this->debugPrint("hitting first comparison");
      return $this->insertAfter($current, $data, $type);
    }

    $node = new PGPNode($data, $this);
    $node->type = $type;

    $one = 1;
    if ($one == 1) {
      $node->previous = $current->previous;
      $current->previous->next = &$node;
      $node->next = $current;
      $current->previous = &$node;
    }
    else {
      $node->next = $current;
      $node->previous = $current->previous;
      $previous = $current->previous;
      $current->previous = $node;
      $previous->next = $node;
    }

    $this->count++;

    return $node;
  }

  /**
   * Inserts a node with the specified data after the specified node.
   *
   * @todo Should we return success or the node?
   *
   * @param PGPNode $current
   *   The specified node.
   * @param mixed $data
   *   The data to store on the node.
   * @return PGPNode
   *   The inserted node.
   */
  public function &insertAfter(&$current, /*&*/$data, $type = NULL) {
    if ($current == NULL) {
      $node = NULL;
      return $node;
    }

    // This comparison was intended for integers.
    if ($current == $this->last) {
//      $this->debugPrint("hitting last comparison");
      return $this->insertBefore($current, $data, $type);
    }

    $node = new PGPNode($data, $this);
    $node->type = $type;

    $node->next = &$current->next;
    $node->previous = &$current;

    $next = &$current->next;
    $current->next = &$node;
    $next->previous = &$node;

    $this->count++;

    return $node;
  }

  /**
   * Inserts a list of nodes before the specified node.
   *
   * @param PGPNode $current
   *   The specified node.
   * @param PGPList $list
   *   The list of nodes to store.
   * @return boolean
   *   Indicates whether the list of nodes was successfully inserted.
   */
  public function insertListBefore(&$current, $list, $type = NULL) {
//    $this->debugPrint(__METHOD__);
    if ($current == NULL) {
      return FALSE;
    }
    if (!($list instanceof PGPList)) {
      return FALSE;
    }

    // This comparison was intended for integers.
    if ($current == $this->first) {
//      $this->debugPrint("hitting first comparison");
      return $this->insertListAfter($current, $list, $type);
    }

    $node = $list->first();
    while ($node->next != NULL) {
      $type2 = is_null($type) ? $node->type : $type; // TODO Would seem to be preferable to always use type from the node being inserted.
      $data = $node->data;
      $this->insertBefore($current, $data, $type2);
      $node = &$node->next;
    }

    return TRUE;
  }

  /**
   * Inserts a list of nodes after the specified node.
   *
   * @param PGPNode $current
   *   The specified node.
   * @param PGPList $list
   *   The list of nodes to store.
   * @return boolean
   *   Indicates whether the list of nodes was successfully inserted.
   */
  public function insertListAfter(&$current, $list, $type = NULL) {
//    $this->debugPrint(__METHOD__);
    if ($current == NULL) {
      return FALSE;
    }
    if (!($list instanceof PGPList)) {
      return FALSE;
    }

    // This comparison was intended for integers.
    if ($current == $this->first) {
//      $this->debugPrint("hitting first comparison");
      return $this->insertListBefore($current, $list, $type);
    }

    $node = $list->last();
    while ($node->previous != NULL) {
      $type2 = is_null($type) ? $node->type : $type; // TODO Would seem to be preferable to always use type from the node being inserted.
      $data = $node->data;
      $this->insertAfter($current, $data, $type2);
      $node = &$node->previous;
    }

    return TRUE;
  }

  /**
   * Updates the specified node.
   *
   * @param PGPNode $current
   *   The specified node.
   * @param mixed $data
   *   The data to store on the node.
   * @return PGPNode
   *   The node that was updated or NULL if no node exists at $index.
   */
  public function &update(&$current, &$data) {
//    $this->debugPrint(__METHOD__);
//    echo print_r($data, 1);
    if ($current == NULL) {
      return NULL;
    }

    $current->data = $data;
    return $current;
  }

  /**
   * Updates the node at the specified index (or insert a node if none exists).
   *
   * @param integer $index
   *   The position of the node in the list.
   * @param mixed $data
   *   The data to store on the node.
   * @return PGPNode
   *   The node that was updated or NULL if no node exists at $index.
   */
  public function &updateKey($index, &$data) {
//    $this->debugPrint(__METHOD__);
//    echo $this->print_r(4, $data); // echo print_r($data, 1); // The 4 is for testing only
    if ($this->count() < $index + 1) {
      return $this->insertLast($data);
    }

    $counter = 0;
    $current = $this->first();
    while ($current->next != NULL && $counter < $index) {
      $current = &$current->next;
      $counter++;
    }
    if ($counter == $index && $current->next != NULL) {
//      $this->debugPrint("counter == index");
      $current->data = $data;
      return $current;
    }
    else {
//      $this->debugPrint("insertLast");
      return $this->insertLast($data);
    }

    return NULL;
  }

  /**
   * Removes all items from the list.
   *
   * TODO Do we need to recurse into an item that contains a list?.
   *
   * @return integer
   *   The number of nodes deleted.
   */
  public function clear() {
//    $this->debugPrint(__METHOD__);
    if ($this->isEmpty()) {
      return 0;
    }

    $counter = 0;
    $current = $this->first();
    while ($current->next != NULL) {
      $next = &$current->next;
      $this->delete($current);
      $current = &$next;
      $counter++;
    }

    return $counter;
  }

  /**
   * This returns [a pointer to] a node.
   *
   * @return unknown
   */
  public function deleteFirst() {
    return $this->delete($this->first->next);
  }

  public function deleteLast() {
    return $this->delete($this->last->previous);
  }

  public function delete(&$current) {
    if ($current == NULL) {
      return FALSE;
    }

    if (!$current->previous) { // if ($current == $this->first) {
      $current = &$current->next;
    }

    if (!$current->next) { // if ($current == $this->last) {
      $current = &$current->previous;
    }

    $current->previous->next = &$current->next;
    $current->next->previous = &$current->previous;

//     unset($current->next); // JB No effect // JB 2010-06-03
//     unset($current->previous);
    unset($current);

    $this->count--;

    return TRUE;
  }

  /**
   * Returns the node at the specified index.
   *
   * @param integer $index
   *   The position of the node in the list.
   * @param boolean $return_node
   *   Indicates whether to return the node object (the default) or its data.
   * @return PGPNode
   *   The node or NULL if no node exists at $index.
   */
  public function &get($index, $return_node = TRUE) {
    if ($this->count() > $index) {
      $counter = 0;
      $current = $this->first();
      while ($current->next != NULL && $counter < $index) {
        $current = &$current->next;
        $counter++;
      }
      if ($counter == $index && $current->next != NULL) {
        if ($return_node) {
          return $current;
        }
        else {
          return $current->data;
        }
      }
    }

    $var = NULL;
    return $var;
  }

  /**
   * Invokes callback on each item in selected list.
   *
   * On forward, start with $first->next.
   * On reverse, start with $last->previous.
   *
   * @todo Add a parameter indicating what to return: node or data.
   *
   * @param string $callback
   *   Name of a callback function.
   * @param string $direction
   *   Direction in which to traverse the list.
   */
  public function traverse($callback, $direction = 'forward') {
    $current = $this->first();
    while ($current->next != NULL) {
      $callback($current);
      $current = &$current->next;
    }
  }

  /**
   * Returns a statement of the given type [with a given value].
   *
   * On forward, start with $first->next.
   * On reverse [or backward], start with $last->previous.
   *
   * @param integer $type
   *   The statement type to search for.
   * @param string $direction
   *   The direction in which to search the list for the statement.
   * @param boolean $return_node
   *   Indicates whether to return the node object or its data (the default).
   * @return mixed
   *   The statement found or FALSE if not found.
   */
  public function &find($type, $direction = 'forward', $return_node = FALSE) {
    if ($direction == 'forward') {
      $current = $this->first();
      $next = 'next';
    }
    else {
      $current = $this->last();
      $next = 'previous';
    }

    while ($current->$next != NULL) {
      $statement = $current->data;
      $this_type = is_object($statement) ? $statement->type : $statement['type'];
      if ($this_type == $type) {
        if ($return_node) {
          return $current;
        }
        else {
          return $current->data;
        }
      }
      $current = &$current->$next;
    }
    $var = FALSE;
    return $var;
  }

  /**
   * Returns a statement attached to a node of the given type.
   *
   * On forward, start with $first->next.
   * On backward, start with $last->previous.
   *
   * @todo Add a parameter indicating what to return: node or data.
   *
   * @param string $callback
   *   Name of a callback function.
   * @param string $direction
   *   Direction in which to traverse the list.
   * @param boolean $return_node
   *   Indicates whether to return the node object or its data (the default).
   * @return mixed
   *   A statement object or FALSE if not found.
   */
  public function &findNode($type, $direction = 'forward', $return_node = FALSE) {
    if ($direction == 'forward') {
      $current = $this->first();
      $next = 'next';
    }
    else {
      $current = $this->last();
      $next = 'previous';
    }

    while ($current->$next != NULL) {
      if ($current->type == $type) {
        if ($return_node) {
          return $current;
        }
        else {
          return $current->data;
        }
      }
      $current = &$current->$next;
    }
    $var = FALSE;
    return $var;
  }

  /**
   * Returns a string representation of a PGPList object
   * (identical to print_r()).
   *
   * Unlike print_r, this omits the printing of embedded object references
   * that would result in infinite recursion.
   *
   * TODO This is deprecated!!!
   *
   * @return string
   *   The string representation.
   */
  public function printList() {
//    $this->debugPrint(__METHOD__);

    return $this->print_r();

    // Local items.
    $strings = array();
    $strings[] = get_class($this);
    $strings[] = "(";
    $strings[] = $this->print_r(1, $this);
    $strings[] = ")";

    return implode("\n", $strings) . "\n";
  }

  /**
   * Returns a string representation of a PGPNode object
   * (identical to print_r()).
   *
   * Unlike print_r, this omits the printing of embedded object references
   * that would result in infinite recursion.
   *
   * @param PGPNode $node
   *   The node object to print.
   * @return string
   *   The string representation.
   */
  public function printNode($node) {
//    $this->debugPrint(__METHOD__);

    if (!($node instanceof PGPNode)) {
      return "Not a PGPNode object\n";
    }

    // Local items.
    $strings = array();
    $strings[] = get_class($node);
    $strings[] = "(";
    $strings[] = $this->printRecursiveArray($node->data, 1);
    $strings[] = ")";

    return implode("\n", $strings) . "\n";
  }

  /**
   * Returns a string representation of an array containing objects
   * (identical to print_r()).
   *
   * Unlike print_r, this omits the printing of embedded object references
   * that would result in infinite recursion.
   *
   * @param array $array
   *   The array to print.
   * @return string
   *   The string representation.
   */
  public function printArray($array) {
//    $this->debugPrint(__METHOD__);

    if (!is_array($array)) {
      return '';
    }

    // Local items.
    $strings = array();
    $strings[] = "Array";
    $strings[] = "(";
    $strings[] = $this->printRecursiveArray($array, 1);
    $strings[] = ")";

    return implode("\n", $strings) . "\n";
  }

  /**
   * Returns a string representation of a PGPList object
   * (identical to print_r()).
   *
   * Unlike print_r, this omits the printing of embedded object references
   * that would result in infinite recursion.
   *
   * @param integer $indent
   *   The indent level.
   * @param PGPList $list
   *   A PGPList object to represent.
   * @param boolean $recurseNode
   *   Indicates whether to recurse on PGPNode objects.
   * @return string
   *   A string of the $list formatted like print_r() output.
   */
  public function print_r($indent = 0, $list = NULL, $recurseNode = TRUE) {
//    echo __METHOD__ . " $this->indent\n";

    $list = is_null($list) ? $this : $list;

    // The first call formats without indenting the parentheses.
    if (!$indent) {
      $strings = array();
      $strings[] = get_class($list);
      $strings[] = "(";
      $strings[] = $this->print_r(1, $list);
      $strings[] = ")";

      return implode("\n", $strings) . "\n";
    }

    $indent0 = str_repeat('    ', $indent);
    $indent1 = str_repeat('    ', $indent + 1);
    $strings = array();
    $counter = 0;

    $current = $list->first();

    while ($current->next != NULL) {
      $data = $current->data;

      $index = ($current->type != NULL) ? $current->type : $counter;

      if (is_array($data)) {
//        $strings[] = "<========= data is a Array ========>"; // This is encountered!!!
        $strings[] = $indent0 . "[$index] => Array";
        $strings[] = $indent1 . "(";
        $temp = $this->printRecursiveArray($data, $indent + 2);
        if ($temp) {
          $strings[] = $temp;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      elseif ($data instanceof PGPBase) {
//        $strings[] = "<========= data is a subclass of PGPBase ========>"; // This is encountered!!!
        $strings[] = $indent0 . "[$index] => " . get_class($data);
        $strings[] = $indent1 . "(";
        $temp = $data->print_r($indent + 2, $data);
        if ($temp) {
          $strings[] = $temp;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      elseif ($data instanceof PGPList) { // elseif (is_a($data, 'PGPExpression')) {
//        $strings[] = "<========= data is a PGPExpression ========>"; // This is encountered!!!
        $strings[] = $indent0 . "[$index] => " . get_class($data);
        $strings[] = $indent1 . "(";
        $temp = $this->print_r($indent + 2, $data);
        if ($temp) {
          $strings[] = $temp;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      else {
//        $strings[] = "<========= data is a non-array string ========>"; // This is encountered!!!
        $strings[] = $indent0 . "[$index] => $data";
      }

      $current = &$current->next;
      $counter++;
    }
    return implode("\n", $strings);
  }

  /**
   * Returns a string representation of an array
   * (identical to print_r()).
   *
   * @param array $items
   *   An array of items.
   * @param integer $indent
   *   The indent level.
   * @param boolean $recurseNode
   *   Indicates whether to recurse on PGPNode objects.
   * @return string
   *   A string of the $items formatted like print_r() output.
   */
  private function printRecursiveArray($items, $indent, $recurseNode = TRUE) {
//    echo __METHOD__ . " $this->indent\n";
    $indent0 = str_repeat('    ', $indent);
    $indent1 = str_repeat('    ', $indent + 1);
    $strings = array();

    foreach ($items as $key => $item) {
      if (is_array($item)) {
        $strings[] = "@+++++++++ item is a Array ++++++++@";
        $strings[] = $indent0 . "[$key] => Array";
        $strings[] = $indent1 . "(";

        $temp = $this->printRecursiveArray($item, $indent + 2);
        if ($temp) {
          $strings[] = $temp;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      elseif ($item instanceof PGPBase) {
        $strings[] = "@+++++++++ item is a " . get_class($item) . " ++++++++@";
        $strings[] = $indent0 . "[$key] => " . get_class($item);
        $strings[] = $indent1 . "(";
        $temp = $item->print_r($indent + 2, $item);
        if ($temp) {
          $strings[] = $temp;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      elseif ($item instanceof PGPList) { // elseif (is_a($item, 'PGPExpression')) {
//        $strings[] = "@+++++++++ item is a PGPList ++++++++@"; // This is encountered!!!
        $strings[] = $indent0 . "[$key] => " . get_class($item);
        $strings[] = $indent1 . "(";
        $temp = $this->print_r($indent + 2, $item);
        if ($temp) {
          $strings[] = $temp;
        }
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      elseif ($item instanceof PGPNode) {
        $strings[] = "@+++++++++ item is a PGPNode ++++++++@";
        $strings[] = $indent0 . "[$key] => PGPNode";
        $strings[] = $indent1 . "(";
        $strings[] = $indent1 . "    [type] => " . $item->data['type'];
        $strings[] = $indent1 . ")";
        $strings[] = '';
      }
      else {
//        $strings[] = "@+++++++++ item is a non-array string ++++++++@"; // This is encountered!!!
        $strings[] = $indent0 . "[$key] => $item";
      }
    } // End foreach
    return implode("\n", $strings);
  }

  /**
   * Applies a callback function to all elements of a PGPList object meeting the
   * search criteria.
   *
   * Based on the optional parameters, the search may begin:
   * - with the first element of the list (the default),
   * - with the last element of the list, or
   * - relative to a specified element.
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
   * @param PGPNode $current
   *   The specified node to begin the search from.
   */
  public function searchCallback($callback, $class, $member, $key, $value, $direction = 'forward', &$current = NULL) {
    // Check pre-conditions.
    if (!function_exists($callback)) {
//      cdp('!function_exists($callback)');
      return;
    }

    // Initialize.
    if ($direction == 'forward') {
      if (is_null($current)) {
        $current = $this->first();
      }
      $next = 'next';
    }
    else {
      if (is_null($current)) {
        $current = $this->last();
      }
      $next = 'previous';
    }

    while ($current->$next != NULL) {
      $data = &$current->data;
      if (is_array($data)) {
      }
      elseif ($data instanceof PGPBase) {
        if (get_class($data) == $class) {
          if (method_exists($data, 'matches')) {
            if ($data->matches($member, $key, $value)) {
              // Invoke callback function.
              $callback($data);
            }
          }
        }
        // Confirm the object as it may have been altered in the callback.
        if ($data instanceof PGPBase) {
          // Recurse on this element's members.
          $data->searchCallback($callback, $class, $member, $key, $value, $direction);
        }
      }
      elseif ($data instanceof PGPList) {
        // Recurse on this element's members.
        $data->searchCallback($callback, $class, $member, $key, $value, $direction);
      }
      elseif ($data instanceof PGPNode) {
      }
      else {
      }
      $current = /*&*/$current->$next; // With '&' the $current is not updated in caller.
//      (is_object($current->data)) ? cdp($current->data->toString(), __FUNCTION__ . ' $current') : cdp($current->data, __FUNCTION__ . ' $current'); // cdp($current->data->toString(), __FUNCTION__ . ' $current');
    }
  }

  /*
   * TODO
   * Add search with direction and starting item.
   * Add traverse with similar parameters.
   * Replace the $list parameter with the $current starting item.
   * Make a big function that can handle all of the parameters.
   * The specific functions call the big dog.
   *
   * Difficulty is being able to specify what we are searching for
   * since it could be anywhere in an expression.
   * Common use is an assignment to a variable. We need to look at
   * the first element in the first expression of the values list
   * of the assignment statement.
   *
   * Bodies need a reference to the statement they are a part of.
   * Statements in a function body would have reference to the function.
   * From there they could get a reference to the file or class the
   * function is part of, as well as the list of statements the
   * function is part of. This would improve our ability to traverse.
   * Store the reference on the first statement in the list, or each
   * statement. The latter would safeguard against an edit that deletes
   * the first stateement.
   *
   * NOTE: We have a $parent member on PGPList but it is not being set.
   */

  /**
   * These two functions are convenience wrappers to search().
   */
//  public function &searchForward($class, $member, $key, $value, $return_node = FALSE, &$current = NULL) {
  public function &searchForward($class, $member, $key, $value, &$current = NULL, $return_node = FALSE) { // Use this parameter order until we transition
    return $this->search($class, $member, $key, $value, $return_node, 'forward', $current);
  }

//  public function &searchBackward($class, $member, $key, $value, $return_node = FALSE, &$current = NULL) {
  public function &searchBackward($class, $member, $key, $value, &$current = NULL, $return_node = FALSE) { // Use this parameter order until we transition
    return $this->search($class, $member, $key, $value, $return_node, 'backward', $current);
  }

  /**
   * Returns the first element of a PGPList object meeting the search criteria.
   *
   * Based on the optional parameters, the search may begin:
   * - with the first element of the list (the default),
   * - with the last element of the list, or
   * - relative to a specified element
   * and the ojbect returned may be:
   * - a PGPBase object (the default), or
   * - a PGPNode object.
   *
   * Usage
   * - search for assignment to variable $mask
   * - searchForward('PGPAssignment', 'values', 0, '$mask')
   * - we need to pass this to PGPAssignment->assignsTo()
   * - here the zero tells it to look at the first operand (which would be the assignee)
   * - if we use toString, then we need to strip whitespace and comment from operand
   * - otherwise assignsTo() will know to look at the value and the operand has to be T_VARIABLE
   *
   * - search for function call named t()
   * - searchForward('PGPFunctionCall', 'name', 'value', 't');
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
   * @param PGPNode $current
   *   The specified node to begin the search from.
   * @return mixed
   *   The object meeting the search criteria, its node, or FALSE.
   */
  public function &search($class, $member, $key, $value, $return_node = FALSE, $direction = 'forward', &$current = NULL) {
    // Initialize.
    if ($direction == 'forward') {
      if (is_null($current)) {
        $current = $this->first();
      }
      $next = 'next';
    }
    else {
      if (is_null($current)) {
        $current = $this->last();
      }
      $next = 'previous';
    }

    while ($current->$next != NULL) {
      $data = &$current->data;
      if (is_array($data)) {
      }
      elseif ($data instanceof PGPBase) {
        if (get_class($data) == $class) {
          if (method_exists($data, 'matches')) {
            if ($data->matches($member, $key, $value)) {
              if ($return_node) {
                return $current;
              }
              else {
                return $data;
              }
            }
          }
        }
        // Confirm the object as it may have been altered in the callback.
        if ($data instanceof PGPBase) {
          // Recurse on this element's members.
          $result = $data->search($class, $member, $key, $value, $return_node, $direction);
          if ($result !== FALSE) {
            return $result;
          }
        }
      }
      elseif ($data instanceof PGPList) {
        // Recurse on this element's members.
        $result = $data->search($class, $member, $key, $value, $return_node, $direction);
        if ($result !== FALSE) {
          return $result;
        }
      }
      elseif ($data instanceof PGPNode) {
      }
      else {
      }
      $current = /*&*/$current->$next; // With '&' the $current is not updated in caller.
//      (is_object($current->data)) ? cdp($current->data->toString(), __FUNCTION__ . ' $current') : cdp($current->data, __FUNCTION__ . ' $current'); // cdp($current->data->toString(), __FUNCTION__ . ' $current');
    }

    $var = FALSE;
    return $var;
  }

  /**
   * Returns all elements of a PGPList object meeting the search criteria.
   *
   * Based on the optional parameters, the search may begin:
   * - with the first element of the list (the default),
   * - with the last element of the list, or
   * - relative to a specified element.
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
   * @param PGPNode $current
   *   The specified node to begin the search from.
   * @return array
   *   The list of elements meeting the search criteria.
   */
  public function searchAll($class, $member, $key, $value, $return_node = FALSE, $direction = 'forward', &$current = NULL) {
    // Initialize.
    if ($direction == 'forward') {
      if (is_null($current)) {
        $current = $this->first();
      }
      $next = 'next';
    }
    else {
      if (is_null($current)) {
        $current = $this->last();
      }
      $next = 'previous';
    }
    $items = array();

    while ($current->$next != NULL) {
      $data = &$current->data;
      if (is_array($data)) {
      }
      elseif ($data instanceof PGPBase) {
        if (get_class($data) == $class) {
          if (method_exists($data, 'matches')) {
            if ($data->matches($member, $key, $value)) {
              // Add item to list.
              $items[] = $return_node ? $current : $data;
            }
          }
        }
        // Confirm the object as it may have been altered in the callback.
        if ($data instanceof PGPBase) {
          // Recurse on this element's members.
          $temp = $data->searchAll($class, $member, $key, $value, $return_node, $direction);
          $items = array_merge($items, $temp);
        }
      }
      elseif ($data instanceof PGPList) {
        // Recurse on this element's members.
        $temp = $data->searchAll($class, $member, $key, $value, $return_node, $direction);
        $items = array_merge($items, $temp);
      }
      elseif ($data instanceof PGPNode) {
      }
      else {
      }
      $current = /*&*/$current->$next; // With '&' the $current is not updated in caller.
//      (is_object($current->data)) ? cdp($current->data->toString(), __FUNCTION__ . ' $current') : cdp($current->data, __FUNCTION__ . ' $current'); // cdp($current->data->toString(), __FUNCTION__ . ' $current');
    }

    return $items;
  }

/*
  public function displayForward() {
//    $this->debugPrint(__METHOD__);

    $string = '';
    $current = $this->first;

    while ($current != NULL) {
      $text = $current->data;
      if (is_array($text)) {
        $string .= print_r($text, 1); // Wrap with drupal_set_message and add ', 1') to print_r parameters?
      }
      else {
        $string .= $text . "\n";
      }

      $current = &$current->next;
    }
    return $string;
  }

  public function displayBackward() {
    $current = $this->last;

    while ($current != NULL) {
//      echo $current->data . " ";
      $current = &$current->previous;
    }
  }
*/
  public function count() {
    return $this->count;
  }

  /**
   * Do not use the following.
   */

  public function insertAfterKey($key, $data) {
    $current = $this->first; // TODO Should this be first()?

    while ($current->data != $key) {
      $current = &$current->next;

      if ($current == NULL)
        return FALSE;
    }

    $node = new PGPNode($data, $this);

    if ($current == $this->last) {
      $node->next = NULL;
      $this->last = &$node;
    }
    else {
      $node->next = &$current->next;
      $current->next->previous = &$node;
    }

    $node->previous = &$current;
    $current->next = &$node;
    $this->count++;

    return TRUE;
  }

  public function deleteKey($key) {
    $current = $this->first; // TODO Should this be first()?

    while ($current->data != $key) {
      $current = &$current->next;
      if ($current == NULL)
        return NULL;
    }

    if ($current == $this->first) {
      $this->first = &$current->next;
    }
    else {
      $current->previous->next = &$current->next;
    }

    if ($current == $this->last) {
      $this->last = &$current->previous;
    }
    else {
      $current->next->previous = &$current->previous;
    }

    $this->count--;
    return $current;
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

  /**
   * Returns a string representation of a list.
   *
   * Case 1: comma-separated parameters to a function (or function call)
   * Case 2: conditions in a conditonal statement
   *
   * @param string $indent
   * @return string
   *   The string representation.
   */
  public function toString($indent = '') {
    $this->debugPrint(__METHOD__);

    $glue = ', ';
    $strings = array();
    $current = $this->first();
    while ($current->next != NULL) {
      $item = $current->data;

      if ($item instanceof PGPExpression) {
        $this->debugPrint("is a PGPExpression");
        $strings[] = $item->toString($indent);
      }
      elseif (is_object($item)) {
        $this->debugPrint("is an object");
        $strings[] = $item->toString($indent);
      }
      elseif ($current->type == 'operator') {
        // This only applies to conditions in a conditional.
        $strings[] = $item;
      }
      else {
        $this->debugPrint($item);
      }

      if ($current->type == 'condition') {
        $glue = ' ';
      }

      $current = $current->next;
    }
    // TODO This adds an extra space when there is non-standard white space between parameters.
    return implode($glue, $strings);
  }

  /**
   * Returns the list element at the specified index.
   *
   * @param integer $index
   *   The index in the list.
   * @return mixed
   *   The list element.
   */
  public function &getElement($index = 0) {
    $this->debugPrint(__METHOD__);

//    if (is_a($this, 'PGPList')) {
      if ($this->count() > $index) {
        return $this->get($index, FALSE);
      }
//    }

    $result = FALSE;
    return $result;
  }

  /**
   * Sets the list element at the specified index.
   *
   * @param integer $index
   *   The index in the list.
   * @param mixed $element
   *   The list element.
   */
  public function setElement($index, $element) {
    $this->debugPrint(__METHOD__);

//    if (is_a($this, 'PGPList')) {
      if ($this->count() > $index) {
        $this->updateKey($index, $element);
      }
      else {
        $this->insertLast($element);
      }
//    }
  }

  /**
   * Inserts the indicated list element at the specified index.
   *
   * @param integer $index
   *   The index in the list.
   * @param mixed $element
   *   The list element.
   */
  public function insertElement($index, $element) {
    $this->debugPrint(__METHOD__);

//    if (is_a($this, 'PGPList')) {
      if ($this->count() > $index) {
        $this->insertBefore($this->get($index), $element);
      }
      else {
        $this->insertLast($element);
      }
//    }
  }

  /**
   * Deletes the list element at the specified index.
   *
   * @param integer $index
   *   The index in the list.
   */
  public function deleteElement($index = 0) {
    $this->debugPrint(__METHOD__);

//    if (is_a($this, 'PGPList')) {
      if ($this->count() > $index) {
        $this->delete($this->get($index));
      }
//    }
  }

  /**
   * Clears the members of a PGPList object.
   *
   * @param mixed $list
   *   The PGPList object to clear.
   * @param boolean $recurseNode
   *   Indicates whether to recurse on PGPNode objects.
   */
  public function clear_r($list = NULL, $recurseNode = TRUE) {
//    $this->debugPrint(__METHOD__);

    $list = is_null($list) ? $this : $list;
    $current = $list->first();

    if (!is_object($current)) {
//      cdp('current is NOT an object');
//      cdp($list->parent->data, '$list->parent->data');
      // JB 2010-06-03 -- these are helpful based on refcount.php
      // These are not set when in this block?
//      unset($list->parent); // unset($this->parent);
//      unset($list->first); // unset($this->first);
//      unset($list->last); // unset($this->last);
      return;
    }

    while ($current->next != NULL) {
      if (!isset($current->data)) {
//        cdp($current->type, '$current->type');
        continue;
      }
      $data = &$current->data;

      if (is_array($data)) {
//        $this->debugPrint("data is array");
        $this->clear_array($data);
      }
      elseif ($data instanceof PGPBase) {
//        $this->debugPrint('class of data is ' . get_class($data));
        $data->clear_r();
      }
      elseif ($data instanceof PGPList) {
//        $this->debugPrint('class of data is ' . get_class($data));
        $data->clear_r();
      }
      else {
//        $this->debugPrint("data is string '$data'");
      }
      unset($data);
      unset($current->container);

      $next = &$current->next;
      // JB 2010-06-03
//       $this->delete($current);
      unset($current->data->parent);
      unset($current->next);
      unset($current->previous);
      unset($current);
      $current = &$next;
    }
    // JB 2010-06-03 -- these are helpful based on refcount.php
    unset($list->parent);
//    unset($list->first->next); -- no effect based on refcount.php
//    unset($list->last->previous);
    unset($list->first);
    unset($list->last);
  }

  /**
   * Clears the members of an array.
   *
   * @param mixed $items
   *   The array to clear.
   * @param boolean $recurseNode
   *   Indicates whether to recurse on array items.
   */
  private function clear_array($items, $recurseNode = TRUE) {
//    $this->debugPrint(__METHOD__);

    foreach ($items as $key => &$item) {
      if (is_array($item)) {
        $this->clear_array($item);
      }
      elseif ($item instanceof PGPBase) {
//        $this->debugPrint('class of item is ' . get_class($item)); // Not hit.
        $item->clear_r();
      }
      elseif ($item instanceof PGPList) {
//        $this->debugPrint('class of item is ' . get_class($item)); // Not hit.
        $item->clear_r();
      }
      elseif ($item instanceof PGPNode) {
//        $this->debugPrint('class of item is ' . get_class($item)); // Not hit.
        $data = &$item->data;
        $data->clear_r();
        unset($data);
        // JB 2010-06-03
        unset($item->next);
        unset($item->previous);
      }
      else {
//        $this->debugPrint("item is string '$item'");
      }
      unset($item);
    }
  }
}

/**
 * Grammar Parser body class.
 *
 * The body statements for a class, function, if, for, while, etc.
 * The body needs to know about indention and semi-colons to end statements.
 */
class PGPBody extends PGPList {
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
   * Indent depth during recursive calls to the print routines.
   *
   * @var integer
   */
  private $indent = 0;

  public function __construct() {
    parent::__construct();
    $this->debug = FALSE; // TRUE; //
  }

  /**
   * Returns a string representation of the body statements.
   *
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
    /*
     * TODO The cases can be consolidated into a couple of types now that
     * everything is an object with a toString method.
     */
    $this->debugPrint(__METHOD__);
    $this->indent = ++PGPWriter::$indent;
    $indent = str_repeat('  ', $this->indent);
    $this->debugPrint("indent = " . $this->indent);

    $strings = array();
    $current = $this->first();
    while ($current->next != NULL) {
      $statement = $current->data;
//      $this->debugPrint("while printBody");
//      $this->debugPrint($statement);
      $type = is_object($statement) ? $statement->type : $statement['type'];
      switch ($type) {
        case T_STRING:
        case T_VARIABLE:
        case T_FUNCTION_CALL:
        case T_DEFINE:
          $strings[] = $statement->toString($indent) . ";";
          break;

        case T_ASSIGNMENT:
          $strings[] = $statement->toString($indent) . ";";
          break;

        case T_DO:
        case T_IF:
        case T_ELSEIF:
        case T_ELSE_IF:
        case T_ELSE:
        case T_WHILE:
        case T_SWITCH:
          $strings[] = $statement->toString($indent);
          break;

        case T_CASE:
        case T_DEFAULT:
          $strings[] = $statement->toString($indent);
          break;

        case T_FOR:
          $strings[] = $statement->toString($indent);
          break;

        case T_FOREACH:
          $strings[] = $statement->toString($indent);
          break;

        case T_COMMENT:
          // Handle a regular comment appended to a line.
          if (isset($statement['append']) && $statement['append']) {
            if (!count($strings)) {
              $strings[] .= ' ' . PGPWriter::printComment($statement);
            }
            else {
              // TODO Whitespace may be other than a single blank.
              $strings[count($strings) - 1] .= ' ' . PGPWriter::printComment($statement);
            }
          }
          else {
            $strings[] = PGPWriter::printComment($statement, $indent);
          }
          break;

        case T_ML_COMMENT:
        case T_DOC_COMMENT:
          // A regular comment (with only a double slash) includes a new line character.
          $strings[] = PGPWriter::printComment($statement, $indent);
          break;

        case T_BREAK:
        case T_CONTINUE:
        case T_ECHO:
        case T_RETURN:
        case T_EXIT:
        case T_CONST:
        case T_GLOBAL:
        case T_VAR:
          // The value is optional with continue, return, and exit.
          $this->debugPrint(__METHOD__ . " continue,echo,etc........");
          $strings[] = $statement->toString($indent) . ";";
          break;
/*
        case T_BREAK:
          $strings[] = $indent . $statement['value'] . ';';
          break;
*/
        case T_INTERFACE:
        case T_CLASS:
        case T_FUNCTION:
          $strings[] = $statement->toString($indent);
          break;

        case T_WHITESPACE:
          $strings[] = str_repeat("\n", $statement['value'] - 1);
          break;

        case T_TRY:
        case T_CATCH:
          $strings[] = $statement->toString($indent);
          break;

//        case T_REQUIRE:
//        case T_REQUIRE_ONCE:
//        case T_INCLUDE:
//        case T_INCLUDE_ONCE:
//          // TODO These cases are never encountered as these come through with type = T_FUNCTION_CALL.
//          // Parentheses are optional with these statements.
//          $this->debugPrint("require,include....");
//          $strings[] = $statement->toString($indent) . ";";
//          break;

//        case T_EVAL:
//        case T_EMPTY:
//        case T_UNSET:
//          // TODO These cases are never encountered as these come through with type = T_FUNCTION_CALL.
//          $this->debugPrint("eval,empty,unset....");
//          $strings[] = $statement->toString($indent) . ";";
//          break;

//        case T_PRINT:
//        case T_THROW:
//          // TODO These cases are never encountered as these come through with type = T_FUNCTION_CALL.
//          $this->debugPrint("print,throw....");
//          $strings[] = $statement->toString($indent) . ";";
//          break;

        case T_DECLARE:
          $strings[] = $statement->toString($indent);
          break;

        case T_LIST:
          $strings[] = $indent . $statement->toString() . ";";
          break;

        case T_INLINE_HTML:
          // 2009-06-13 Inline html
          $strings[] = $current->statementValue('value');
          break;

        case T_OPEN_TAG:
        case T_OPEN_TAG_WITH_ECHO:
        case T_CLOSE_TAG:
          // 2009-06-13 Inline html
          // NOTE: The open tags include a new line character.
          $strings[] = $current->statementValue('value');
          break;
      }
      $current = $current->next;
    }

    $this->indent--; PGPWriter::$indent--;
    $this->debugPrint("at end of " . __FUNCTION__);
    $this->debugPrint($strings);
    return implode("\n", $strings);
  }
}

/**
 * Grammar Parser expression class.
 *
 * An expression will have nodes of type: operand, operator.
 */
class PGPExpression extends PGPList {
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

  public function __construct() {
    parent::__construct();
    $this->debug = FALSE; // TRUE; //
  }

  /**
   * Returns a string representation of the expression.
   *
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
//    $this->debugPrint(__METHOD__);

    $add_space = 0;
    $space = '';
    $string = '';

    // Items in this list are what used to be array elements.
    $current = $this->first();
    while ($current->next != NULL) {
      $type = $current->type;
      $data = $current->data;

      $space = $add_space ? ' ' : '';
      switch ($type) {
        case 'operand': // Hits, PGPOperand
          if (is_object($data)) {
            if (method_exists($data, 'toString')) {
              $string .= $space . $data->toString($indent);
            }
            else {
              $string .= ' MISSING FC';
            }
          }
          elseif (is_array($data) && isset($data['value'])) {
            $string .= $space . $data['value']; // Should there be any other index?
          }
          else {
            // This hits on numbers T_DNUMBER and T_LNUMBER
            $string .= $space . print_r($data, 1); // TODO Use print_r for now so we can see the unhandled items
          }
          break;

        case 'operator': // Hits
          $string .= $space . $data;
          break;

        // Things like T_DOUBLE_COLON that should not have spaces surrounding them.
        case 'operator2': // Hits, PGPOperand
          $string .= $data;
          $add_space = -1;
          break;

        // Things like suppression operator (@) that should not have a following space.
        case 'operator3': // Hits, PGPOperand
          $string .= $space . $data;
          $add_space = -1;
          break;

        case 'assign': // Hits
          $string .= $space . $data;
          break;

        case 'object': // This key is now part of operator2
          $string .= '->' . $data->toString($indent);
          break;

        case 'unary': // PGPOperand
          $string .= $data;
          break;

        case 'curly': // PGPOperand
          $string .= '{' . $data->toString($indent) . '}';
          break;

        case 'index': // PGPOperand
          $string .= '[' . $data->toString($indent) . ']';
          break;

        case 'lparens': // Hits
          $add_space = -1; // What if there is white space following?
          $string .= $space . $data;

          $this->debugPrint("printing lparens in printConditionVariable");
          $this->debugPrint($string);
          $this->debugPrint("");

          break;

        case 'rparens': // Hits
          $string .= $data;
          break;

        case 'special': // PGPString
          $string .= $space . '"' . $data->toString($indent) . '"';
          break;

        case 'comment': // Hits
          // TODO What kind of value would this have? Inline comment?
          $string .= isset($data['value']) ? $space . $data['value'] : '';
          break;

        case 'whitespace': // Hits
          $string .= $data;
          // Set space to zero for next item.
          $add_space = -1;
          break;

        // For a variable expression.

        case 'reference': // PGPOperand
        case 'suppress': // PGPOperand
        case 'indirect': // PGPOperand
          $string .= $data; // == 1 ? '&' : '';
          break;

        case 'value': // PGPOperand
//        case 'unary':
          $string .= $data;
          break;

        case 'inlineif': // PGPExpression 2010-02-09; No longer used 2010-11-05
          // TODO If $data includes whitespace at beginning, then this adds an extra space.
          // @todo Data can be empty as of PHP 5.3; omit trailing space.
          $string .= ' ? ' . $data->toString($indent);
          break;

        case 'inlineelse': // PGPExpression 2010-02-09; No longer used 2010-11-05
          $string .= ' : ' . $data->toString($indent);
          break;

        default:
          $this->debugPrint("UNACCOUNTED type = $type");
          break;
      }
//      $this->debugPrint(__METHOD__ . " string = $string");
      if ($add_space <= 0) {
        $add_space++;
      }
//      $this->debugPrint(__METHOD__ . " end of SWITCH on type = $type");
      $current = $current->next;
    }
    return $string;
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
    if (!$type || !$this->count()) {
      return FALSE;
    }

    $operand = $this->findNode('operand');
    $this->debugPrint($operand);

    if ($operand instanceof PGPExpression) { // TODO What base class to use here?
      $this->debugPrint("checking case PGPExpression");
      $data = $operand->findNode('type');
      return $data !== FALSE && $data == $type;
    }
    elseif ($operand instanceof PGPBase) {
      $this->debugPrint("checking case PGPBase");
      return isset($operand->type) && $operand->type == $type;
    }
    elseif (is_array($operand)) {
      $this->debugPrint("checking case array");
      return isset($operand['type']) && $operand['type'] == $type;
    }
    else {
      $this->debugPrint("checking case ELSE");
      return FALSE;
    }
  }

  /**
   * Returns an operand with comment and whitespace items removed.
   *
   * @param integer $index
   *   The index of the operand to return.
   * @return PGPOperand
   *   The operand object with comment and whitespace items removed.
   */
  public function stripComments_OLD($index = 0) { // TODO Review upgrade routines that call this method.
    $operand = $this->getElement($index);
    if (!$operand) {
      return NULL;
    }

    if (is_array($operand)) {
      return $operand;
    }
    elseif (!is_object($operand)) {
      return NULL;
    }

    return $operand->stripComments();
  }

  /**
   * Returns an expression with comment and whitespace items removed.
   *
   * @return PGPExpression
   *   The expression object with comment and whitespace items removed.
   */
  public function stripComments() {
    if ($this->isEmpty()) {
      return new PGPExpression();
    }

    $stripped = new PGPExpression();

    $current = $this->first();
    while ($current->next != NULL) {
      $type = $current->type;
      $data = $current->data; // Copy this?
      $this->debugPrint(__METHOD__ . " type = $type");

      if (!in_array($current->type, array('comment', 'whitespace'))) {
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

  /**
   * Returns a trimmed expression with comment and whitespace items removed.
   *
   * @todo In most invocations, the $charlist = "'\"" (omits the space). Is this
   * an issue?
   *
   * @param string $charlist
   *   (optional) List of characters to strip before returning the string.
   * @param boolean $clean
   *   (optional) Whether comment and whitespace items are to be removed before
   *   rendering the expression as a string.
   *
   * @return string
   *   The trimmed string value of the expression object, optionally, without
   *   comments and whitespace removed.
   */
  public function trim($charlist = "'\" ", $clean = TRUE) {
    $expression = $clean ? $this->stripComments() : $this;
    return trim($expression->toString(), $charlist);
  }

  /**
   * Inserts a statement before the statement containing the expression.
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
   * Inserts a statement after the statement containing the expression.
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
   * Returns the count of nodes of a given type in the expression.
   *
   * @param string $type
   *   The string of the node type to count.
   * @return integer
   *   The number of nodes of the indicated type.
   */
  public function countType($type) {
//    $this->debugPrint(__METHOD__);

    $count = 0;

    $current = $this->first();
    while ($current->next != NULL) {
      $type2 = $current->type;
      $this->debugPrint(__METHOD__ . " type = $type2");

      if ($current->type == $type) {
        $count++;
      }
      $current = $current->next;
    }
    return $count;
  }

  /**
   * Returns the n-th occurrence of a node of a given type in the expression.
   *
   * @todo Does this belong in PGPList?
   * This is like findNode with an $occurrence parameter.
   *
   * @param string $type
   *   The string of the node type to return.
   * @param integer $count
   *   The occurrence to return.
   * @return mixed
   *   The statement object on the n-th occurrence of the node of the indicated
   *   type, or FALSE if not found.
   */
  public function getType($type, $occurrence = 1) {
//    $this->debugPrint(__METHOD__);

    $count = 0;

    $current = $this->first();
    while ($current->next != NULL) {
      $type2 = $current->type;
      $this->debugPrint(__METHOD__ . " type = $type2");

      if ($current->type == $type) {
        $count++;
        if ($count == $occurrence) {
          return $current->data; // TODO Node or data?
        }
      }
      $current = $current->next;
    }
    return FALSE;
  }
}

/**
 * Grammar Parser operand class.
 *
 * An operand will have nodes of type: type, value, index, object.
 */
class PGPOperand extends PGPExpression {

  public function __construct() {
    parent::__construct();
    $this->debug = FALSE; // TRUE; //
  }

  /**
   * Returns an operand with comment and whitespace items removed.
   *
   * Example: drupal_set_title(/* smoke screen *\/ 'check_plain' . ($node->title/* smoke screen *\/));
   * The whitespace (or lack therof) and comment at the end are part of an operand.
   * The same items at the beginning are part of an expression.
   *
   * @param integer $index
   *   The index of the operand to return. [Not used here.]
   * @return PGPOperand
   *   The operand object with comment and whitespace items removed.
   */
  public function stripComments($index = 0) {
//    $this->debugPrint(__METHOD__);

    $stripped = new PGPOperand();

    $current = $this->first();
    while ($current->next != NULL) {
      $type = $current->type;
      $data = $current->data; // Copy this?
      $this->debugPrint(__METHOD__ . " type = $type");

      if (!in_array($current->type, array('comment', 'whitespace'))) {
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
 * Grammar Parser special string expression with variable references class.
 *
 * A special string will have nodes of type:
 * - [call], string, operand, begin, end, open, close.
 *
 * The order of the members is important so as to be able to use a foreach
 * statement to print the object.
 */
class PGPString extends PGPExpression {
  /**
   * Statement type.
   *
   * @var integer
   */
  public $type;

  public function __construct() {
    parent::__construct();
    $this->debug = FALSE; // TRUE;
  }

  /**
   * Returns a string representation of the special string expression.
   *
   * @return string
   *   A string of the expression.
   */
  public function toString($indent = '') {
//    $this->debugPrint(__METHOD__);

    $string  = '';

    $current = $this->first();
    while ($current->next != NULL) {
      $type = $current->type;
      $data = $current->data;
      $this->debugPrint(__METHOD__ . " type = $type");

      switch ($type) {
        case 'begin':
        case 'end':
          $string .= $data;
          break;

        case 'open':
        case 'close':
          $string .= $data['value'];
          break;

        case 'operand':
          $string .= $data->toString($indent);
          break;

        case 'variable_xxx': // TODO Deprecated.
          $string .= $data->toString($indent);
          if ($data->first()->type == type && in_array($data->first()->data, array(T_DOLLAR_OPEN_CURLY_BRACES, T_CURLY_OPEN))) {
            $string .= '}';
          }
          break;

        case 'string':
          $string .= $data['value'];
          break;
      }

      $current = $current->next;
    }
    return $string;
  }
}
