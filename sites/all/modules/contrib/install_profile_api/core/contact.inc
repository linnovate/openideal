<?php
// $Id: contact.inc,v 1.1.2.3 2008/12/14 04:32:47 dww Exp $

/**
 * Add a new contact category, including recipients and so on.
 */
function install_contact_add_category($category, $recipients, $reply = '', $weight = 0, $selected = 0) {
  $values = array();
  $values['category'] = $category;
  $values['recipients'] = $recipients;
  $values['reply'] = $reply;
  $values['weight'] = $weight;
  $values['selected'] = $selected;
  drupal_write_record('contact', $values);
}

