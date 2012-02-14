<?php
// $Id: service_links_handler_field_service_links.inc,v 1.1.2.2 2010/09/17 12:06:18 thecrow Exp $

/**
 * @file
 * A handler to provide a service links field for a particular node.
 *
 * @ingroup views_field_handlers
 */
class service_links_handler_field_service_links extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['nid'] = 'nid';
    $this->additional_fields['title'] = 'title';
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function option_definition() {
    $options = parent::option_definition();

    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    // Remove the alteration stuff
    unset($form['alter']);
  }

  function render($values) {
    $node = new stdClass();
    $node->nid = $values->{$this->aliases['nid']};
    $node->title = $values->{$this->aliases['title']};
    // Nothing to render.
    return theme('links', service_links_render($node, TRUE));
  }
}
