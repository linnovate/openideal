<?php
// $Id: calendar_plugin_style_ical.inc,v 1.1.2.17 2009/04/28 22:47:29 karens Exp $
/**
 * Default style plugin to render an iCal feed.
 */
class calendar_plugin_style_ical extends views_plugin_style_rss {
  function init(&$view, &$display) {
    parent::init($view, $display);
    $fields = $display->handler->default_display->options['fields'];
    $this->options['fields'] = $fields;
  }
  
  function query() {
    // We need these values for the iCal feed.
    $this->view->query->add_field('node', 'title');
    $this->view->query->add_field('node', 'type');
    parent::query();
  }
  
  function attach_to($display_id, $path, $title) {
    $display = $this->view->display[$display_id]->handler;
    $url_options = array();
    $input = $this->view->get_exposed_input();
    if ($input) {
      $url_options['query'] = $input;
    }
    
    // TODO adjust this to pick up default values when no arg is set?
    $url = url($this->view->get_url(NULL, $path), $url_options);
    if (empty($this->view->feed_icon)) {
      $this->view->feed_icon = '';
    }
    $this->view->feed_icon .= theme('calendar_ical_icon', $url);

    drupal_add_link(array(
      'rel' => 'alternate',
      'type' => 'application/calendar',
      'title' => $title,
      'href' => $url
    ));
  }

  /**
   * Set default options
   */
  function options(&$options) {
    parent::options($options);
    $options['summary_field'] = 'title';
    $options['description_field'] = '';
    $options['location_field'] = '';
    $options['fields'] = array();
  }
  
  function option_definition() {
    $options = parent::option_definition();

    $options['summary_field'] = array('default' => '', 'translatable' => TRUE);
    $options['description_field'] = array('default' => '', 'translatable' => TRUE);
    $options['location_field'] = array('default' => '', 'translatable' => TRUE);
    
    return $options;
  }

  function options_form(&$form, &$form_state) {
    $options = array('' => '');
    foreach ($this->options['fields'] as $field) {
      $handler = views_get_handler($field['table'], $field['field'], 'field');
      $options[$field['field']] = $handler->ui_name();
    }
    $form['#prefix'] = '<div class="form-item">'. t("Map the View fields to the values they should represent in the iCal feed. Only fields that have been added to the view are available to use in this way. You can add additional fields to the view and mark them 'Exclude from display' if you only want them in the iCal feed.") .'</div>';
    
    $form['summary_field'] = array(
      '#type' => 'select',
      '#title' => t('Title'),
      '#default_value' => !empty($this->options['summary_field']) ? $this->options['summary_field'] : 'title',
      '#options' => $options,
      '#required' => TRUE,
      );
    $form['description_field'] = array(
      '#type' => 'select',
      '#title' => t('Description'),
      '#default_value' => $this->options['description_field'],
      '#options' => $options,
      );
    $form['location_field'] = array(
      '#type' => 'select',
      '#title' => t('Location'),
      '#default_value' => $this->options['location_field'],
      '#options' => $options,
      );
      
  }

   /**
   * Style validation.
   */
  function validate() {
    $errors = parent::validate();
    
    $style = $this->display->display_options['style_plugin'];
    
    $arguments = $this->display->handler->get_option('arguments');
    $filters = $this->display->handler->get_option('filters');
    
    if (!array_key_exists('date_argument', $arguments) && !array_key_exists('date_filter', $filters)) {
      if (empty($this->view->date_info->arg_missing)) {
        $errors[$style] = t("The @style style requires a Date argument or a Date filter.", array('@style' => $style));      
      }
      $this->view->date_info->arg_missing = TRUE;
    }
    if (array_key_exists('date_argument', $arguments) && 
    ($arguments['date_argument']['default_action'] != 'default' || $arguments['date_argument']['default_argument_type'] != 'date')) {
      if (empty($this->view->date_info->arg_missing_default)) {
        $errors[] = calendar_errors('missing_argument_default');      
      }
      $this->view->date_info->arg_missing_default = TRUE;
    }
     
    if (empty($this->options['summary_field'])) {
      $errors[] = $errors[$style] = t("The @style style requires a Title field for the iCal export.", array('@style' => $style));
    }
    // Make sure date fields are not set up to 'Group multiple values' 
    // in the calendar style.
    if ($style == 'calendar_style') {
      $view_fields = date_api_fields($this->view->base_table);
      $view_fields = $view_fields['name'];
      $fields = $this->display->handler->get_option('fields');
      foreach ($fields as $column => $field) {
        $field_name = $field['table'] .".". $field['field'];
        if (!empty($field['multiple'])) {
          $cck_fields = content_fields();
          $real_name = $view_fields[$field_name]['real_field_name'];
          if ($cck_fields[$real_name]['multiple'] && !empty($field['multiple']['group'])) {
            $errors[] = t("The date field '@field' used by the display '@display_title' cannot be set to 'Group multiple values'.", array('@field' => $view_fields[$field_name]['label'], '@display_title' => $this->display->display_title));
          }
        }
      }
    }
    return $errors;
  }
 
  function render() {
    require_once('./'. drupal_get_path('module', 'calendar') .'/includes/calendar.inc');
    
    // Transfer the style options to the view object so they
    // can be easily accessed in the theme.
    $style_options = $this->options;
    $this->view->date_info->summary_field = $style_options['summary_field'];
    $this->view->date_info->description_field = $style_options['description_field'];
    $this->view->date_info->location_field = $style_options['location_field'];
    
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
        $this->view->date_info->date_arg = !empty($this->view->args) ? $this->view->args[$argument->position] : '';
        $this->view->date_info->date_arg_pos = $i;
        $this->view->date_info->year = isset($argument->year) ? $argument->year : NULL;
        $this->view->date_info->month = isset($argument->month) ? $argument->month: NULL;
        $this->view->date_info->day = isset($argument->day) ? $argument->day : NULL;
        $this->view->date_info->week = isset($argument->week) ? $argument->week : NULL;
        $this->view->date_info->min_date = $argument->min_date;
        $this->view->date_info->max_date = $argument->max_date;
                
        // Stop after the first date argument, if there is more than one.
        break;
      }
      $i++;
    }
    
    // The ical display might have date filters instead of arguments.
    // If we missed getting a min date from date arguments, try date filters.
    if (empty($this->view->date_info->min_date)) {
      foreach ($this->view->filter as $id => $filter) {
        if ($filter->field == 'date_filter') {
          // TODO Decide if we want to provide a date here or not.
          // Adding this now is to prevent fatal errors later if the
          // view is used in unexpected ways without a date being set.
          if (empty($filter->min_date)) {
            $value = $filter->default_value('value');
            $range = $filter->date_handler->arg_range($value);
            $filter->min_date = $range[0];
            $filter->max_date = $range[1];
          }
          $this->view->date_info->granularity = !empty($filter->granularity) ? $filter->granularity : $filter->options['granularity'];
          $this->view->date_info->year = isset($filter->year) ? $filter->year : NULL;
          $this->view->date_info->month = isset($filter->month) ? $filter->month: NULL;
          $this->view->date_info->day = isset($filter->day) ? $filter->day : NULL;
          $this->view->date_info->week = isset($filter->week) ? $filter->week : NULL;
          $this->view->date_info->min_date = $filter->min_date;
          $this->view->date_info->max_date = $filter->max_date;
          if (empty($this->view->date_info->date_fields)) {
            $this->view->date_info->date_fields = array();
          }
          $this->view->date_info->date_fields = array_merge($this->view->date_info->date_fields, array_keys($filter->options['date_fields']));
          
          // Stop after the first date filter, if there is more than one.
          break;
        }
      }
      $i++;
    }
       
    // Render each field into an output array.
    $items = array();
    $calendar_fields = date_api_fields($this->view->base_table);
    $calendar_fields = array_keys($calendar_fields['alias']);
    
    foreach ($this->view->result as $num => $row) {
      $items[$num] = $row;
      // Store the raw date values before formatting the results.
      foreach ($row as $key => $value) {
        if (in_array($key, $calendar_fields)) {
          $items[$num]->calendar_fields->$key = $value;
        }
      }
      foreach ($this->view->field as $name => $field) {
        // Some fields, like the node edit and delete links, have no alias.
        $field_alias = $field->field_alias != 'unknown' ? $field->field_alias : $name; 
        if (!empty($field) && is_object($field)) {
          $field_output = $field->theme($row);
          $items[$num]->{$field_alias} = $field_output;
        }
      }
    }
    
    // Massage the resulting items into formatted calendar items.
    $items = calendar_build_nodes($this->view, $items);
    
    // Merge in items from other sources.
    foreach (module_implements('calendar_add_items') as $module) {
      $function = $module .'_calendar_add_items';
      if (function_exists($function)) {
        if ($feeds = $function($this->view)) {
          foreach ($feeds as $feed) {
            $items = $feed;
          }
        }
      }
    }
    return theme($this->theme_functions(), $this->view, $this->options, $items);
  }
}
