<?php
//$Id: nodequeue_handler_field_links.inc,v 1.3 2010/03/13 21:51:06 ezrag Exp $
/**
 * Views field handler for the nodequeue queue links
 */
class nodequeue_handler_field_links extends views_handler_field_node_link {
  function render($values) {
    if (user_access('manipulate queues') || user_access('manipulate all queues')) {
      $nid = $values->{$this->aliases['nid']};
      return theme('links', nodequeue_link('node', node_load($nid)));
    }
  }
}