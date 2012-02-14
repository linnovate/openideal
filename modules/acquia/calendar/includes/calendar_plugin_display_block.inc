<?php
// $Id: calendar_plugin_display_block.inc,v 1.1.2.10 2008/12/06 14:56:15 karens Exp $
/**
 * The plugin that handles a calendar block.
 * 
 * The only style option that will be available is the calendar
 * style, which creates the navigation and links to other calendar
 * displays. All options for paging, row plugins, etc. are
 * deferred to the attachments.
 */
class calendar_plugin_display_block extends views_plugin_display_block {

  function init(&$view, &$display) {
    parent::init($view, $display);
  }
  
  /**
   * Display validation.
   */
  function validate() {
    $errors = parent::validate();
    
    $arguments = $this->display->handler->get_option('arguments');
    if (!in_array('date_argument', array_keys($arguments))) {
      if (empty($this->view->date_info->arg_missing)) {
        $errors[] = t("The Calendar period display '@display_title' will not work without a Date argument.", array('@display_title' => $this->definition['title']));      
      }
      $this->view->date_info->arg_missing = TRUE;
    }
    elseif ($arguments['date_argument']['default_action'] != 'default' || $arguments['date_argument']['default_argument_type'] != 'date') {
      if (empty($this->view->date_info->arg_missing_default)) {
        $errors[] = calendar_errors('missing_argument_default');      
      }
      $this->view->date_info->arg_missing_default = TRUE;
    }
    return $errors;
  }
  
  function get_style_type() { return 'calendar'; }
  
  function defaultable_sections($section = NULL) {
    if (in_array($section, array('style_plugin', 'row_options', 'row_plugin', 'items_per_page'))) {
      return FALSE;
    }
    return parent::defaultable_sections($section);
  }

  function options(&$display) {
    parent::options($display);
    $display->display_options['displays'] = array();
    $display->display_options['style_plugin'] = 'calendar_nav';
    $display->display_options['items_per_page'] = 0;
    $display->display_options['row_plugin'] = '';
    $display->display_options['defaults']['style_plugin'] = FALSE;
    $display->display_options['defaults']['style_options'] = FALSE;
    $display->display_options['defaults']['items_per_page'] = FALSE;
    $display->display_options['defaults']['row_plugin'] = FALSE;
    $display->display_options['defaults']['row_options'] = FALSE;
  } 
  
  function option_definition () {
    $options = parent::option_definition();

    $options['style_plugin'] = array('default' => 'calendar_nav');
    return $options;
  }

  /**
   * The display block handler returns the structure necessary for a block.
   * 
   * TODO This can be removed when the patch at http://drupal.org/node/290328
   * gets into an official release.
   */
  function execute() {
    // Prior to this being called, the $view should already be set to this
    // display, and arguments should be set on the view.
    $info['content'] = $this->view->render();
    $info['subject'] = filter_xss_admin($this->view->get_title());
    if (!empty($this->view->result) || $this->get_option('empty') || !empty($this->view->style_plugin->definition['even empty'])) {
      return $info;
    }
  }
}
