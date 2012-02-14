<?php
// $Id: calendar_plugin_style.inc,v 1.1.2.15 2009/05/11 23:52:15 karens Exp $
/**
 * Style plugin to create the calendar navigation and links.
 * 
 * Used by the main calendar page and calendar block displays.
 */
class calendar_plugin_style extends views_plugin_style {
  /**
   * Init will be called after construct, when the plugin is attached to a
   * view and a display. 
   */
  function init(&$view, &$display) {
    parent::init($view, $display);
    $view->date_info->display_types = $this->display_types();
  }
    
  function display_types($granularity = NULL, $option_type = 'names') {
    $ids = array();
    $names = array();
    foreach (calendar_display_types() as $name => $type) {
      foreach ($this->view->display as $id => $display) {
        if ($display->display_plugin == 'calendar_period') {
          if (!empty($display->display_options['calendar_type']) && $display->display_options['calendar_type'] == $name) {
            $attachments = array_filter($display->display_options['displays']);
            if (isset($attachments['calendar_1'])) {
              $ids[$name] = $id;
              $names[$name] = $display->display_title;
            }
          }
        }
      }
    }
    if ($granularity) {
      return $$option_type[$granularity];
    }
    return $$option_type;
  }
  
  /**
   * Calendar argument date fields used in this view.
   */
  function date_fields() {
    $date_fields = array();
    $calendar_fields = date_api_fields($this->view->base_table);
    $arguments = $this->display->handler->get_option('arguments');
    foreach ($arguments as $name => $argument) {
      if (isset($argument['date_fields'])) {
        foreach ($argument['date_fields'] as $date_field) {
          $field = $calendar_fields['name'][$date_field];
          $handler = views_get_handler($field['table_name'], $field['field_name'], 'field');
          if ($handler) {
            $date_fields[$date_field] = $field;
            $date_fields[$date_field]['name'] = $handler->ui_name();
          }
        }
      }
    }
    return ($date_fields);
  }
  
  /**
   * Style validation.
   */
  function validate() {
    $errors = parent::validate();
    if (empty($this->display->display_options['style_plugin'])) {
      return $errors;
    }
    $style = $this->display->display_options['style_plugin'];
    
    $arguments = $this->display->handler->get_option('arguments');
    if (!in_array('date_argument', array_keys($arguments))) {
      if (empty($this->view->date_info->arg_missing)) {
        $errors[$style] = t("The @style style requires a Date argument.", array('@style' => $style));      
      }
      $this->view->date_info->arg_missing = TRUE;
      $this->date_info->arg_fields = array();
    }
    else {
      $this->date_info->arg_fields = $arguments['date_argument']['date_fields'];
      if ($arguments['date_argument']['default_action'] != 'default' || $arguments['date_argument']['default_argument_type'] != 'date') {
        if (empty($this->view->date_info->arg_missing_default)) {
          $errors[] = calendar_errors('missing_argument_default');      
        }
        $this->view->date_info->arg_missing_default = TRUE;
      }
    }
    
    // Make sure date fields are not set up to 'Group multiple values' 
    // in the calendar style.
    if ($style == 'calendar_style') {
      $view_fields = date_api_fields($this->view->base_table);
      $view_fields = $view_fields['name'];
      $fields = $this->display->handler->get_option('fields');
      $has_fields = FALSE;
      foreach ($fields as $column => $field) {
        $field_name = $field['table'] .".". $field['field'];
        if (in_array($field_name, $this->date_info->arg_fields)) {
          $has_fields = TRUE;
        }
        if (!empty($field['multiple']) && array_key_exists($field_name, $view_fields)) {
          $cck_fields = content_fields();
          $real_name = $view_fields[$field_name]['real_field_name'];
          if ($cck_fields[$real_name]['multiple'] && !empty($field['multiple']['group'])) {
            $errors[] = t("The date field '@field' used by the display '@display_title' cannot be set to 'Group multiple values'.", array('@field' => $view_fields[$field_name]['label'], '@display_title' => $this->display->display_title));
          }
        }
      }
      // The calendar needs the values from the date fields to split
      // the nodes into calendar cells, so make sure the field gets
      // added into the query.
      if (!$has_fields) {
        $errors[] = t('The date argument date fields must be added to this query. You can exclude them if you do not want them displayed in the calendar.');
      }
    }
    return $errors;
  }

  function query() {
    
    require_once('./'. drupal_get_path('module', 'date_api') .'/date_api_sql.inc');
        
    $style_options = $this->view->style_plugin->options;
    
    // Evaluate our argument values and figure out which 
    // calendar display we need to create.
    $i = 0;   
    foreach ($this->view->argument as $id => $argument) {
      if ($argument->field == 'date_argument') {
        // TODO Decide if we want to provide a date here or not.
        // Adding this now is to prevent fatal errors later if the
        // view is used in unexpected ways without a date being set.
        if (empty($argument->min_date)) {
          $value = $argument->get_default_argument();
          $range = $argument->date_handler->arg_range($value);
          $argument->min_date = $range[0];
          $argument->max_date = $range[1];
        }
        $this->view->date_info->granularity = !empty($argument->granularity) ? $argument->granularity : $argument->options['granularity'];
        $this->view->date_info->date_arg = !empty($this->view->args) && count($this->view->args) > $argument->position ? $this->view->args[$argument->position] : '';
        $this->view->date_info->date_arg_pos = $i;
        $this->view->date_info->year = isset($argument->year) ? $argument->year : NULL;
        $this->view->date_info->month = isset($argument->month) ? $argument->month: NULL;
        $this->view->date_info->day = isset($argument->day) ? $argument->day : NULL;
        $this->view->date_info->week = isset($argument->week) ? $argument->week : NULL;
        $this->view->date_info->min_date = $argument->min_date;
        $this->view->date_info->max_date = $argument->max_date;
        $this->view->date_info->min_date_date = date_format($this->view->date_info->min_date, DATE_FORMAT_DATE);
        $this->view->date_info->max_date_date = date_format($this->view->date_info->max_date, DATE_FORMAT_DATE);
        
        // Stop after the first date argument, if there is more than one.
        break;
      }
      $i++;
    }
    $this->view->date_info->display_types = $this->display_types();
    $keys = drupal_map_assoc(array_keys(calendar_display_types()));
    $this->view->date_info->calendar_type = $keys[$this->view->date_info->granularity];
    
    // bring the node type into the query so we can use it in the theme
    if ($this->view->base_table == 'node') {
      $this->view->query->add_field('node', 'type');
    }
      
    parent::query();
  }
  
  /**
   * Render the calendar navigation style.
   */
  function render() {
    return theme($this->theme_functions(), $this->view, $this->options, array());
  }
}

