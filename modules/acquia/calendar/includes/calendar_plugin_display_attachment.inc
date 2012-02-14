<?php
// $Id: calendar_plugin_display_attachment.inc,v 1.1.2.17 2009/03/17 18:03:36 karens Exp $
/**
 * The plugin that handles calendar attachment displays.
 * 
 * Adding year/month/day/week pages as attachments makes it
 * possible to use any style type, so they could be tables,
 * lists, teasers, or nodes as well as traditional calendar
 * pages.
 * 
 * Force 'inherit_arguments' and 'inherit_filters' to TRUE, 
 * and 'attachment_position' to 'after', and don't display 
 * those options in the UI.
 * 
 * Allows paging (regular attachments do not), and adds an option 
 * to choose what calendar period this display represents.
 */
class calendar_plugin_display_attachment extends views_plugin_display_attachment {

  /**
   * Instead of going through the standard views_view.tpl.php, delegate this
   * to the style handler.
   */
  function render() {
    return $this->view->style_plugin->render($this->view->result);
  }
  
  /**
   * Create an array of possible display periods.
   */
  function display_types($type = 'month') {
    $types = calendar_display_types();
    return $types[$type];
  }

  /**
   * Identify the period of this display.
   */
  function calendar_type() {
    $types = calendar_display_types();
    $default = $this->get_option('calendar_type');
    if (!array_key_exists($default, $types)) $default = 'month';
    return $default;    
  }
  

  /**
   * Inspect argument and view information to see which calendar
   * period we should show. The argument tells us what to use
   * if there is no value, the view args tell us what to use
   * if there are values.
   */
  function display_granularity($display_id) {
    
    $arguments = $this->view->get_items('argument', $display_id);
    $wildcard = '';
    $argument = '';
    $default_granularity = '';
    $i = 0;
    foreach ($arguments as $argument) {
      if ($argument['id'] == 'date_argument') {
        $pos = $i;
        $default_granularity = $argument['granularity'];
        $wildcard = $argument['wildcard'];
        $argument = !empty($this->view->args) && !empty($this->view->args[$pos]) ? $this->view->args[$pos] : '';
        break;
      }
      $i++;
    }
    // TODO Anything else we need to do for 'all' arguments?
    if ($argument == $wildcard) {
      $view_granularity = $default_granularity;
    }
    elseif (!empty($argument)) {
      require_once('./'. drupal_get_path('module', 'date_api') .'/date_api_sql.inc');
      $date_handler = new date_sql_handler();
      $view_granularity = $date_handler->arg_granularity($argument);
    }
    else {
      $view_granularity = $default_granularity;
    }
    return $view_granularity;
  }
  
  /**
   * Display validation.
   */
  function validate() {
    $errors = parent::validate();
    
    $arguments = $this->display->handler->get_option('arguments');
    if (!in_array('date_argument', array_keys($arguments))) {
      if (empty($this->view->date_info->arg_missing)) {
        $errors[] = t("A Calendar period display will not work without a Date argument."); 
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
  
  /**
   * Attach only the appropriate displays for the current argument.
   */
  function attach_to($display_id) {
    $display_granularity = $this->calendar_type();
    $view_granularity = $this->display_granularity($display_id);
    
    // If this is not the right display to show, 
    // don't attach it, just exit.
    if ($view_granularity != $display_granularity) {
      unset($this->display);
      return;
    }
    
    $this->view->date_info->parent_id = $display_id;

    // See if we're attaching to a block rather than a page.
    if (substr($display_id, 0, 14) == 'calendar_block') {
      $this->view->date_info->mini = TRUE;
      $this->view->date_info->block = TRUE;
      $this->date_info->calendar_popup = FALSE;
      if (!isset($this->view->date_info->block_identifier)) {
        $this->view->date_info->block_identifier = 'mini';
      }
    }
    elseif (substr($display_id, 0, 9) == 'calendar_') {
      $this->view->date_info->calendar_colors = $this->view->display[$display_id]->handler->options['calendar_colors'];
      $this->view->date_info->calendar_colors_taxonomy = $this->view->display[$display_id]->handler->options['calendar_colors_taxonomy'];
      $this->view->date_info->calendar_popup = $this->view->display[$display_id]->handler->options['calendar_popup'];
      $this->view->date_info->calendar_date_link = $this->view->display[$display_id]->handler->options['calendar_date_link'];
    }  
    parent::attach_to($display_id);
  }

  function pre_execute() {
    // Make sure parent function is called so things like items per page get set.
    parent::pre_execute();
    $this->view->date_info->display_granularity = $this->calendar_type();
    $this->view->date_info->calendar_type = $this->calendar_type();
  }
  
  function query() {
    // If we are using legend colors based on taxonomy, make sure the 
    // node vid field is added to the query so the theme can use it.
    if (!empty($this->view->date_info->calendar_colors_taxonomy)) {
      if (empty($this->additional_fields)) $this->additional_fields = array(); 
      $this->view->query->add_field('node', 'vid');
    }
    parent::query();    
  }
  
  /**
   * Set default values for the display.
   */
  function options(&$display) {
    parent::options($display);
    $display->display_options['inherit_argments'] = TRUE;
    $display->display_options['inherit_filters'] = TRUE;
    $display->display_options['attachment_position'] = 'after';
    $display->display_options['calendar_type'] = $this->calendar_type();
  }  

  /**
   * Add custom option definitions.
   */
  function option_definition () {
    $options = parent::option_definition();
    $options['calendar_type'] = array('default' => $this->calendar_type());
    return $options;
  }
  
 function options_form(&$form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_form($form, $form_state);
       
    switch ($form_state['section']) {
      case 'calendar_type':
        $form['#title'] .= t('Calendar period');
        $form['calendar_type'] = array(
          '#type' => 'select',
          '#description' => t('Select the calendar time period for this display.'),
          '#default_value' => $this->calendar_type(),
          '#options' => calendar_display_types(),
          );
         break;
    }
  }

  /**
   * Perform any necessary changes to the form values prior to storage.
   * There is no need for this function to actually store the data.
   */
  function options_submit($form, &$form_state) {
    // It is very important to call the parent function here:
    parent::options_submit($form, $form_state);
    switch ($form_state['section']) {
      case 'calendar_type':
        $this->set_option($form_state['section'], $form_state['values'][$form_state['section']]);
        break;
    }
  }
    
  /**
   * Provide the summary for attachment options in the views UI.
   *
   * This output is returned as an array.
   */
  function options_summary(&$categories, &$options) {
    parent::options_summary($categories, $options);
    $types = calendar_display_types();
    $categories['calendar_settings'] = array(
      'title' => t('Calendar settings'),
    );
    
    $options['calendar_type'] = array(
      'category' => 'calendar_settings',
      'title' => t('Calendar period'),
      'value' => $types[$this->calendar_type()],
    );
  }

  /**
   * Take away the option to change these values.
   */
  function defaultable_sections($section = NULL) {
    if (in_array($section, array('inherit_argments', 'inherit_filters', 'attachment_position',))) {
      return FALSE;
    }
    return parent::defaultable_sections($section);
  }

}
