<?php

/**
 * Caching plugin that provides cache control based on custom PHP code.
 *
 * @ingroup views_cache_plugins
 */
class views_php_plugin_cache extends views_plugin_cache {

  /**
   * Implements views_plugin_cache#summary_title().
   */
  function summary_title() {
    return t('PHP');
  }

  /**
   * Implements views_object#option_definition().
   */
  function option_definition() {
    $options = parent::option_definition();
    $options['php_cache_results'] = array('default' => '');
    $options['php_cache_output'] = array('default' => '');

    return $options;
  }

  /**
   * Implements views_plugin#options_form().
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form += views_php_form_element($this,
      FALSE,
      array('php_cache_results', t('Result cache code'), t('The code must return TRUE if the cache is still fresh, FALSE otherwise.'), FALSE),
      array('$view', '$plugin', '$cache')
    );
    $form += views_php_form_element($this,
      FALSE,
      array('php_cache_output', t('Output cache code'), t('The code must return TRUE if the cache is still fresh, FALSE otherwise.'), FALSE),
      array('$view', '$plugin', '$cache')
    );
  }

  /**
   * Implements views_plugin_cache#cache_get()
   */
  function cache_get($type) {
    //$cutoff = $this->cache_expire($type);
    switch ($type) {
      case 'query':
        // Not supported currently, but this is certainly where we'd put it.
        return FALSE;
      case 'results':
        $cache = cache_get($this->get_results_key(), $this->table);
        $fresh = !empty($cache);
        if ($fresh && !empty($this->options['php_cache_results'])) {
          $function = create_function('$view, $plugin, $cache', $this->options['php_cache_results'] . ';');
          ob_start();
          $fresh = $function($this->view, $this, $cache);
          ob_end_clean();
        }
        // Values to set: $view->result, $view->total_rows, $view->execute_time,
        // $view->current_page.
        if ($fresh) {
          //if (!$cutoff || $cache->created > $cutoff) {
            $this->view->result = $cache->data['result'];
            $this->view->total_rows = $cache->data['total_rows'];
            $this->view->set_current_page = $cache->data['current_page'];
            $this->view->execute_time = 0;
            return TRUE;
          //}
        }
        return FALSE;
      case 'output':
        $cache = cache_get($this->get_output_key(), $this->table);
        $fresh = !empty($cache);
        if ($fresh && !empty($this->options['php_cache_output'])) {
          $function = create_function('$view, $plugin, $cache', $this->options['php_cache_output'] . ';');
          ob_start();
          $fresh = $function($this->view, $this, $cache);
          ob_end_clean();
        }
        if ($fresh) {
          //if (!$cutoff || $cache->created > $cutoff) {
            $this->storage = $cache->data;
            $this->view->display_handler->output = $cache->data['output'];
            $this->restore_headers();
            return TRUE;
          //}
        }
        return FALSE;
    }
  }
}
