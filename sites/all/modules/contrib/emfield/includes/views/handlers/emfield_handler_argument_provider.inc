<?php
// $Id: emfield_handler_argument_provider.inc,v 1.1.2.1 2009/09/30 15:11:01 aaron Exp $

/**
 * @file
 * The subclass simply adds properties,
 * for field-specific subclasses to use if they need to.
 */
class emfield_handler_argument_provider extends content_handler_argument_string {
  /**
   * Override the behavior of title().
   */
  function title() {
    $provider = $this->argument;
    $module = $this->content_field['module'];
    $info = emfield_include_invoke($module, $provider, 'info');
    return check_plain($info['name']);
  }
}
