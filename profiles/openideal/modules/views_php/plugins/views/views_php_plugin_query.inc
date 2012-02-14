<?php

/**
 * A (fake) pager plugin that wraps around the actual query.
 *
 * @ingroup views_query_plugins
 */
class views_php_plugin_query extends views_php_plugin_wrapper {

  /**
   * Implements views_plugin_query#execute().
   */
  function execute(&$view) {
    $pager = new views_php_plugin_pager();
    $pager->php_wrap($this->wrapped->pager);

    $this->wrapped->execute($view);

    $pager->php_unwrap();
  }
}
