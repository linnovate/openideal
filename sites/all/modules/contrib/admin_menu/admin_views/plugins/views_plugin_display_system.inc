<?php
// $Id: views_plugin_display_system.inc,v 1.3 2010/03/11 02:20:14 sun Exp $

/**
 * @file
 * System display plugin.
 */

/**
 * Plugin to handle replacement of existing system paths.
 *
 * @ingroup views_display_plugins
 */
class views_plugin_display_system extends views_plugin_display {
  /**
   * The system display requires a path.
   */
  function has_path() {
    return TRUE;
  }

  // @todo Needed?
  function uses_breadcrumb() {
    return TRUE;
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['path'] = array(
      'default' => '',
    );

    return $options;
  }

  /**
   * Add this display's path information to Drupal's menu system.
   */
  function execute_hook_menu($callbacks) {
    $items = array();

    // Replace % with the link to our standard views argument loader
    // views_arg_load().
    $bits = explode('/', $this->get_option('path'));
    $page_arguments = array($this->view->name, $this->display->id);

    // Replace % with %views_arg for menu autoloading and add to the
    // page arguments so the argument actually comes through.
    foreach ($bits as $pos => $bit) {
      if ($bit == '%') {
        $bits[$pos] = '%views_arg';
        $page_arguments[] = $pos;
      }
    }

    $path = implode('/', $bits);

    $access_plugin = $this->get_plugin('access');
    if (!isset($access_plugin)) {
      $access_plugin = views_get_plugin('access', 'none');
    }
    if (!$path) {
      return $items;
    }

    $items[$path] = array(
      // default views page entry
      'page callback' => 'views_page',
      'page arguments' => $page_arguments,
      // Default access check (per display)
      'access callback' => 'views_access',
      'access arguments' => array($access_plugin->get_access_callback()),
      // Identify URL embedded arguments and correlate them to a handler
      'load arguments'  => array($this->view->name, $this->display->id, '%index'),
    );

    // Grep all callbacks for router items below the target path.
    $children = preg_grep('@' . preg_quote($path, '@') . '/.+@', array_keys($callbacks));
    foreach ($children as $child_path) {
      // Only inherit properties from parent, if the child is not a default
      // local task.
      if (!isset($callbacks[$child_path]['type']) || $callbacks[$child_path]['type'] != MENU_DEFAULT_LOCAL_TASK) {
        // Add child to our stack of items to replace.
        $items[$child_path] = $callbacks[$child_path];
        // Ensure that the parent path really exists and inherit its
        // properties to the child (not replacing existing properties).
        if (isset($callbacks[$path])) {
          $items[$child_path] += $callbacks[$path];
        }
      }
    }

    // If the path really exists, copy over its remaining properties.
    if (isset($callbacks[$path])) {
      $items[$path] += $callbacks[$path];
    }

    return $items;
  }

  /**
   * Build and render the page view.
   *
   * Since we replace an existing page, we need to invoke views_set_page_view().
   * Also set the page title, because original page callbacks might do this.
   */
  function execute() {
    views_set_page_view($this);

    // Prior to this being called, the $view should already be set to this
    // display, and arguments should be set on the view.
    $this->view->build();
    if (!empty($this->view->build_info['fail'])) {
      return drupal_not_found();
    }
    // @todo Needed?
    $this->view->get_breadcrumb(TRUE);

    drupal_set_title(filter_xss_admin($this->view->get_title()), PASS_THROUGH);

    // @todo This is super-likely the totally wrong place to add our CSS; better
    //   suggestions and patches welcome.
    drupal_add_css(drupal_get_path('module', 'admin_views') . '/admin_views.css');

    return $this->view->render();
  }

  /**
   * Provide the summary for page options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);

    $categories['system'] = array(
      'title' => t('System path settings'),
    );

    $path = $this->get_option('path');
    if (empty($path)) {
      $path = t('None');
    }
    else {
      $path = truncate_utf8($path, 17, FALSE, TRUE);
    }

    $options['path'] = array(
      'category' => 'system',
      'title' => t('Path'),
      'value' => check_plain($path),
    );
  }

  /**
   * Provide the default form for setting options.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    switch ($form_state['section']) {
      case 'path':
        $form['#title'] .= t('An existing menu path this view replaces');
        $form['path'] = array(
          '#type' => 'textfield',
          '#description' => t('This view replaces this path on your site. You may use "%" for dynamic arguments. For example: "node/%/feed".'),
          '#default_value' => $this->get_option('path'),
      	  '#field_prefix' => '<span dir="ltr">' . url(NULL, array('absolute' => TRUE)) . (variable_get('clean_url', 0) ? '' : '?q='),
      	  '#field_suffix' => '</span>&lrm;',
      	  '#attributes' => array('dir'=>'ltr'),
        );
        break;
    }
  }

  function options_validate(&$form, &$form_state) {
    parent::options_validate($form, $form_state);

    switch ($form_state['section']) {
      case 'path':
        if (strpos($form_state['values']['path'], '%') === 0) {
          form_error($form['path'], t('"%" may not be used for the first segment of a path.'));
        }
        // Automatically remove '/' from path.
        $form_state['values']['path'] = trim($form_state['values']['path'], '/');
        break;
    }
  }

  function options_submit(&$form, &$form_state) {
    parent::options_submit($form, $form_state);

    switch ($form_state['section']) {
      case 'path':
        $this->set_option('path', $form_state['values']['path']);
        break;
    }
  }
}
