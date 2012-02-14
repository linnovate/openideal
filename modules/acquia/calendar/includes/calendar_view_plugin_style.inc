<?php
// $Id: calendar_view_plugin_style.inc,v 1.1.2.19 2009/02/17 18:09:35 karens Exp $

/**
 * Style plugin to render the year, month, week, or day calendar view.
 */
class calendar_view_plugin_style extends calendar_plugin_style {

  /**
   * Init will be called after construct, when the plugin is attached to a
   * view and a display. 
   */
  function init(&$view, &$display) {
    parent::init($view, $display);
    $calendar_type = $this->display->handler->get_option('calendar_type');
    $view->date_info->style_name_size = $this->options['name_size'];
    $view->date_info->style_with_weekno = $this->options['with_weekno'];
    $view->date_info->style_max_items = $this->options['max_items'];
    $view->date_info->style_max_items_behavior = $this->options['max_items_behavior'];
    if (!empty($this->options['groupby_times_custom'])) {
      $view->date_info->style_groupby_times = explode(',', $this->options['groupby_times_custom']);
    }
    else {
      $view->date_info->style_groupby_times = calendar_groupby_times($this->options['groupby_times']);
    }
    $view->date_info->style_groupby_field = $this->options['groupby_field'];
    
    // TODO make this an option setting.
    $view->date_info->style_show_empty_times = !empty($this->options['groupby_times_custom']) ? TRUE : FALSE;
    
    // Make sure views does't try to limit the number of items in this view.
    $this->view->pager['items_per_page'] = 0;
    
  }
  
  /**
   * Set default options
   */
  function options(&$options) {
    $options['name_size'] = 3;
    $options['with_weekno'] = 0;
    $options['max_items'] = 0;
    $options['max_items_behavior'] = 'more';
    $options['groupby_times'] = 'hour';
    $options['groupby_times_custom'] = '';
    $options['groupby_field'] = '';
  }

  /**
   * Style options.
   */
  function options_form(&$form, &$form_state) {
    $calendar_type = $this->display->handler->get_option('calendar_type');
    $form['name_size'] = array(
      '#title' => t('Calendar day of week names'),
      '#default_value' => $this->options['name_size'],
      '#type' => in_array($calendar_type, array('year', 'month', 'week')) ? 'radios' : 'value',
      '#options' => array(1 => t('First letter of name'), 2 => t('First two letters of name'), 3 => t('Abbreviated name'), 99 => t('Full name')),
      '#description' => t('The way day of week names should be displayed in a calendar.'),
      );
    $form['with_weekno'] = array(
      '#title' => t('Show week numbers'),
      '#default_value' => $this->options['with_weekno'],
      '#type' => in_array($calendar_type, array('month')) ? 'radios' : 'value',
      '#options' => array(0 => t('No'), 1 => t('Yes')),
      '#description' => t('Whether or not to show week numbers in the left column of calendar weeks and months.'),
      );
    $form['max_items'] = array(
      '#title' => t('Maximum items'),
      '#type' => in_array($calendar_type, array('month')) ? 'select' : 'value',
      '#options' => array(CALENDAR_SHOW_ALL => t('Unlimited'), CALENDAR_HIDE_ALL => t('No items'), 3 => t('3 items'), 5 => t('5 items'), 10 => t('10 items')),
      '#default_value' => $calendar_type != 'day' ? $this->options['max_items'] : 0,
      '#description' => t('Maximum number of items to show in calendar cells, used to keep the calendar from expanding to a huge size when there are lots of items in one day. '),
      );
    $form['max_items_behavior'] = array(
      '#title' => t('Too many items'),
      '#type' => in_array($calendar_type, array('month')) ? 'select' : 'value',
      '#options' => array('more' => t("Show maximum, add 'more' link"), 'hide' => t('Hide all, add link to day')),
      '#default_value' => $calendar_type != 'day' ? $this->options['max_items_behavior'] : 'more',
      '#description' => t('Behavior when there are more than the above number of items in a single day. When there more items than this limit, a link to the day view will be displayed.'),
      );
    $form['groupby_times'] = array(
      '#title' => t('Time grouping'),
      '#type' => in_array($calendar_type, array('day', 'week')) ? 'select' : 'value',
      '#default_value' => $this->options['groupby_times'],
      '#description' => t("Group items together into time periods based on their start time."),
      '#options' => array('' => t('None'), 'hour' => t('Hour'), 'half' => t('Half hour'), 'custom' => t('Custom')),
      );
    $form['groupby_times_custom'] = array(
      '#title' => t('Custom time grouping'),
      '#type' => in_array($calendar_type, array('day', 'week')) ? 'textarea' : 'value',
      '#default_value' => $this->options['groupby_times_custom'],
      '#description' => t("When choosing the 'custom' Time grouping option above, create custom time period groupings as a comma-separated list of 24-hour times in the format HH:MM:SS, like '00:00:00,08:00:00,18:00:00'. Be sure to start with '00:00:00'. All items after the last time will go in the final group."),
      );
    // Create a list of fields that are available for grouping and truncation,
    // excluding the date fields in the view from the grouping options.  
    $field_options = array();
    $date_field_options = array();
    $fields = $this->display->handler->get_option('fields');
    $date_fields = array_keys($this->date_fields());
    foreach ($fields as $field_name => $field) {
      $handler = views_get_handler($field['table'], $field['field'], 'field');
      if (!in_array($field['table'] .'.'. $field['field'], $date_fields)) {
        $field_options[$field['table'] .'_'. $field['field']] = $handler->ui_name();
      }
      else {
        $date_field_options[$field['table'] .'_'. $field['field']] = $handler->ui_name();
      }
    }
    $form['groupby_field'] = array(
      '#title' => t('Field grouping'),
      '#type' => in_array($calendar_type, array('day')) ? 'select' : 'value',
      '#default_value' => $this->options['groupby_field'],
      '#description' => t("Optionally group items into columns by a field value, for instance select the content type to show items for each content type in their own column, or use a location field to organize items into columns by location."),
      '#options' => array('' => '') + $field_options,
      );
    foreach ($form as $key => $value) {
      if ($value['#type'] == 'value') {
        $form[$key]['#value'] = $value['#default_value'];
      }
    }
  }

  /**
   * Render the calendar attachment style.
   */
  function render() {
    // Adjust the theme to match the currently selected default.
    // Only the month view needs the special 'mini' class,
    // which is used to retrieve a different, more compact, theme.
    if (!empty($this->view->date_info->mini) && $this->view->date_info->granularity == 'month') {
      $this->definition['theme'] = 'calendar_mini';
    }
    else {
      $this->view->date_info->mini = FALSE;
      $this->definition['theme'] = 'calendar_'. $this->view->date_info->granularity;
    }
    $this->view->date_info->hide_admin_links = TRUE;
    return theme($this->theme_functions(), $this->view, $this->options, array());
  }
}