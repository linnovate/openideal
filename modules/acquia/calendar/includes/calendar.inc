<?php
//$Id: calendar.inc,v 1.1.2.40 2009/05/11 22:24:27 karens Exp $
/**
 * Build calendar
 *
 * @param unknown_type $view
 * @param unknown_type $items
 * @return themed table
 */
function calendar_build_calendar($view, $items) {
  // Remove nodes outside the selected date range.
  $values = array();
  foreach ($items as $delta => $item) {
    if (empty($item->calendar_start_date) || empty($item->calendar_end_date)) {
      continue;
    }
    $item_start = date_format($item->calendar_start_date, DATE_FORMAT_DATE);
    $item_end = date_format($item->calendar_end_date, DATE_FORMAT_DATE);
    if (($item_start >= $view->date_info->min_date_date && $item_start <= $view->date_info->max_date_date)
     || ($item_end >= $view->date_info->min_date_date && $item_end <= $view->date_info->max_date_date)) {
       $values[$item_start][date_format($item->calendar_start_date, 'H:i:s')][] = $item;
    }
  }
  $items = $values;
  ksort($items);
  
  $rows = array();
  $curday = drupal_clone($view->date_info->min_date);
  
  switch ($view->date_info->granularity) {
    case 'year':
      $rows = array();
      $view->date_info->mini = TRUE;
      for ($i = 1; $i <= 12; $i++) {
        $rows[$i] = calendar_build_month($curday, $view, $items);
      }
      $view->date_info->mini = FALSE;
      break;

    case 'month':
      $rows = calendar_build_month($curday, $view, $items);
      break;

    case 'day':
      $rows = calendar_build_day($curday, $view, $items);
      break;

    case 'week':
      $rows = calendar_build_week($curday, $view, $items);

      // Merge the day names in as the first row.
      $rows = array_merge(array(calendar_week_header($view)), $rows);
      break;
  }
  return $rows;
  
}

/**
 * Build one month.
 */
function calendar_build_month(&$curday, $view, $items) {
  $month = date_format($curday, 'n');
  date_modify($curday, '-' . strval(date_format($curday, 'j')-1) . ' days');

  $rows = array();
  do {
    $rows = array_merge($rows, calendar_build_week($curday, $view, $items, TRUE));
    $curday_date = date_format($curday, DATE_FORMAT_DATE);
    $curday_month = date_format($curday, 'n');
  } while ($curday_month == $month && $curday_date <= $view->date_info->max_date_date);

  // Merge the day names in as the first row.
  $rows = array_merge(array(calendar_week_header($view)), $rows);
  return $rows;
}

/**
 * Build one week row.
 */
function calendar_build_week(&$curday, $view, $items, $check_month = FALSE) {
  $curday_date = date_format($curday, DATE_FORMAT_DATE);
  $weekdays = calendar_untranslated_days($items, $view);
  $today = date_format(date_now(date_default_timezone_name()), DATE_FORMAT_DATE);
  $month = date_format($curday, 'n');
  $week = date_week($curday_date);
  $first_day = variable_get('date_first_day', 0);

  // move backwards to the first day of the week
  $day_wday = date_format($curday, 'w');
  date_modify($curday, '-' . strval((7 + $day_wday - $first_day) % 7) . ' days');
  $curday_date = date_format($curday, DATE_FORMAT_DATE);
    
  // If we're displaying the week number, add it as the
  // first cell in the week.
  if (!empty($view->date_info->style_with_weekno) && !in_array($view->date_info->granularity, array('day', 'week'))) {
    $url = $view->get_path() .'/'. $view->date_info->year .'-W'. $week;
    if (!empty($view->date_info->display_types['week'])) {
      $weekno = l($week, $url, array('query' => !empty($view->date_info->append) ? $view->date_info->append : ''));
    }
    else { 
      // Do not link week numbers, if Week views are disabled.
      $weekno = $week;
    }
    $rows[$week][] = array(
      'data' => $weekno,
      'id' => $view->name . '-weekno-' . $curday_date,  
      'class' => 'week');
  }
  for ($i = 0; $i < 7; $i++) {
    $curday_date = date_format($curday, DATE_FORMAT_DATE); 
    $class = strtolower($weekdays[$i] .
    ($view->date_info->mini ? ' mini' : ''));
    if ($check_month && ($curday_date < $view->date_info->min_date_date || $curday_date > $view->date_info->max_date_date || date_format($curday, 'n') != $month)) {
      $class .= ' empty';
      $content = array(
        'date' => '', 
        'datebox' => '', 
        'empty' => theme('calendar_empty_day', $curday_date, $view), 
        'link' => '',
        'all_day' => array(), 
        'items' => array(),
        );
    }
    else {
      $content = calendar_build_day($curday, $view, $items);
      $class .= ($curday_date == $today ? ' today' : '') .
        ($curday_date < $today ? ' past' : '') .
        ($curday_date > $today ? ' future' : '') .
        (empty($items[$curday_date]) ? ' has-no-events' : ' has-events');
    }
    
    $rows[$week][] = array(
      'data' => $content,
      'class' => $class, 'id' => $view->name . '-' . $curday_date);
    date_modify($curday, '+1 day');
  }
  return $rows;
}

/**
 * Build the contents of a single day for the $rows results.
 */
function calendar_build_day($curday, $view, $items) {
  $curday_date = date_format($curday, DATE_FORMAT_DATE);
  $selected = FALSE;
  $max_events = !empty($view->date_info->style_max_items) ? $view->date_info->style_max_items : 0;
  $types = array();
  $inner = array();
  $all_day = array();
  $empty = '';
  $link = '';
  $count = 0;
  foreach ($items as $date => $day) {
    if ($date == $curday_date) {
      $count = 0;
      $selected = TRUE;
      ksort($day);
      foreach ($day as $time => $hour) {
        foreach ($hour as $key => $item) {
          $count++;
          $types[$item->type] = $item;
          if (!$view->date_info->mini && ($max_events == CALENDAR_SHOW_ALL || $count <= $max_events || ($count > 0 && $max_events == CALENDAR_HIDE_ALL))) {
            // Theme the item here unless this is a 'Day' or 'Week' view.
            // Day and week views need to do more processing before rendering
            // the item, so just past them the unrendered item.
            $theme = isset($item->calendar_node_theme) ? $item->calendar_node_theme : 'calendar_'. $view->date_info->granularity .'_node';
            if ($item->calendar_all_day) {
              $all_day[] = in_array($view->date_info->calendar_type, array('day', 'week')) ? $item : theme($theme, $item, $view);
            }
            else {
              $key = date_format($item->calendar_start_date, 'H:i:s');
              $inner[$key][] = in_array($view->date_info->calendar_type, array('day', 'week')) ? $item : theme($theme, $item, $view);
            }
          }
        }
      }
    }
  }
  ksort($inner);
  if (empty($inner) && empty($all_day)) {
    $empty = theme('calendar_empty_day', $curday_date, $view);
  }
  // We have hidden events on this day, use the theme('calendar_multiple_') to show a link.
  if ($max_events != CALENDAR_SHOW_ALL && $count > 0 && $count > $max_events && $view->date_info->calendar_type != 'day' && !$view->date_info->mini) {
    if ($view->date_info->style_max_items_behavior == 'hide' || $max_events == CALENDAR_HIDE_ALL) {
      $all_day = array();
      $inner = array();
    }
    $link = theme('calendar_'. $view->date_info->calendar_type .'_multiple_node', $curday_date, $count, $view, $types);
  }
    
  $content = array(
    'date' => $curday_date, 
    'datebox' => theme('calendar_datebox', $curday_date, $view, $items, $selected), 
    'empty' => $empty,
    'link' => $link,
    'all_day' => $all_day,
    'items' => $inner,
    );
  return $content;
}

/**
 * Formats the weekday information into table header format
 *
 * @ingroup event_support
 * @return array with weekday table header data
 */
function calendar_week_header($view) {
  $len = isset($view->date_info->style_name_size) ? $view->date_info->style_name_size : (!empty($view->date_info->mini) ? 1 : 3);
  $with_week = !empty($view->date_info->style_with_weekno);
  
  // create week header
  $untranslated_days = calendar_untranslated_days();
  if ($len == 99) {
    $translated_days = date_week_days_ordered(date_week_days(TRUE));
  }
  else {
    $translated_days = date_week_days_ordered(date_week_days_abbr(TRUE));
  }
  if ($with_week) {
    $row[] = array('header' => TRUE, 'class' => "days week", 'data' => '&nbsp;');
  }
  foreach ($untranslated_days as $delta => $day) {
    $label = $len < 3 ? drupal_substr($translated_days[$delta], 0 , $len) : $translated_days[$delta];
    $row[] = array('header' => TRUE, 'class' => "days ". $day, 'data' => $label);
  }
  return $row;
}

/**
 * Array of untranslated day name abbreviations, forced to lowercase
 * and ordered appropriately for the site setting for the first day of week.
 *
 * The untranslated day abbreviation is used in css classes.
 */
function calendar_untranslated_days() {
  $untranslated_days = date_week_days_ordered(date_week_days_untranslated());
  foreach ($untranslated_days as $delta => $day) {
    $untranslated_days[$delta] = strtolower(substr($day, 0, 3));
  }
  return $untranslated_days;
}

/**
 * Take the array of items and alter it to an array of
 * calendar nodes that the theme can handle.
 *
 * Iterate through each datefield in the view and each item
 * returned by the query, and create pseudo date nodes.
 *
 * If there is more than one date field in the node, this will create
 * multiple nodes, one each with the right calendar date for that
 * field's value. If a field value has a date range that covers more than
 * one day, separate nodes will be created for each day in the field's
 * day range, limited to the minimum and maximum dates for the view.
 *
 * When we finish, we will have a distinct node for each distinct day
 * and date field.
 */
function calendar_build_nodes(&$view, &$items) {
  if (empty($view->date_info->min_date) || empty($view->date_info->max_date)) {
    return $items;
  }
  // Midnights are determined based on the same timezone as the View uses
  $display_timezone = date_timezone_get($view->date_info->min_date);
  $display_timezone_name = timezone_name_get($display_timezone);
  
  // Translate the view min and max dates to UTC values
  // so we can compare UTC dates to the view range.
  $min_utc = drupal_clone($view->date_info->min_date);
  date_timezone_set($min_utc, timezone_open('UTC'));
  $max_utc = drupal_clone($view->date_info->max_date);
  date_timezone_set($max_utc, timezone_open('UTC'));
  $min_zone_string = array(); // Will cache $min_utc-strings in various timezones 
  $max_zone_string = array();

  $view->date_info->nodes_per_page = 0;
  $type_names = node_get_types('names');

  $datefields = array();
  $fields = date_api_fields($view->base_table);
  if (!empty($view->filter['date_filter'])) {
    $date_filter = $view->filter['date_filter'];
    foreach ($view->filter['date_filter']->options['date_fields'] as $name) {
      $datefields[] = $fields['name'][$name]['query_name'];
    }
  }
  if (!empty($view->argument['date_argument'])) {
    $date_filter = $view->argument['date_argument'];
    foreach ($view->argument['date_argument']->options['date_fields'] as $name) {
      $datefields[] = $fields['name'][$name]['query_name'];
    }
  }

  $view_fields = date_api_views_fetch_fields('node', 'field');
  $field_names = (array) array_keys($fields['name']);
  $nodes = array();
  $i = 0;
  foreach ($date_filter->options['date_fields'] as $name) {
    $field        = $fields['name'][$name];
    $field_type   = strstr($field['type'], 'string') ? 'string' : 'timestamp';
    $alias        = $field['query_name'];
    $field_name   = $field['field_name'];
    $fromto       = $field['fromto'];
    $tz_handling  = $field['tz_handling'];
    $label        = isset($view->field[$name]) ? $view->field[$name]['label'] : $field['field_name'];
    $tz_alias     = str_replace('.', '_', $field['timezone_field']);
    $db_tz        = date_get_timezone_db($field['tz_handling']);
    $local_tz     = date_get_timezone($field['tz_handling'], 'date');
    $field_name   = $field['field_name'];
    $rrule_field  = str_replace(array('_value2', '_value'), '_rrule', $alias);
            
    // Set a flag to tell us if individual multi-day dates need to be 
    // split into separate nodes.
    $split_dates = TRUE;
    if (strstr($view->current_display, '_ical')) {
      $split_dates = FALSE;
    }
    
    // If there is no field for this item, just default to the site format.
    if (!isset($view->field[$field_name])) {
      $format = variable_get('date_format_short', 'm/d/Y - H:i');
    }
    else {
      if (strstr($field['type'], 'cck')) {
        $format = $view->field[$field_name]->options['format'];
        $cck_field_name = str_replace(array('_value2', '_value'), '', $field_name);
        $format = date_formatter_format($format, $cck_field_name);
      }
      else {
        $format = $view->field[$field_name]->options['date_format'];
        switch ($format) {
          case 'long':
            $format = variable_get('date_format_long',  'l, F j, Y - H:i');
            break;
          case 'medium':
            $format = variable_get('date_format_medium',  'D, m/d/Y - H:i');
            break;
          case 'custom':
            $format = $view->field[$field_name]->options['custom_date_format'];
            break;
          case 'time ago':
            break;
          default:
            $format = variable_get('date_format_short', 'm/d/Y - H:i');
            break;
        }
      }
    }
     
    // If there are multiple date fields in this calendar we may get
    // duplicate items from the other date fields, so add a way to
    // make sure each individual date field only gets added to the
    // calendar one time.
    $processed = array();
    $rrule_processed = array();
    foreach ($items as $pos => $item) {
      $delta = !empty($field['delta_field']) && !empty($item->{$field['delta_field']}) ? $item->{$field['delta_field']} : 0;
      $real_field = $field_name;
      if (substr($field['type'], 0, 3) == 'cck') {
        $real_field = str_replace(array('_value2', '_value'), '', $field_name);
      }
      
      $id = 'calendar:'. $item->{$view->base_field} .':'. $real_field .':'. $delta;
      
      // When creating iCal feeds for repeating dates we don't want all
      // the multiple values, send only the first value.
      if (strstr($view->current_display, '_ical') && !empty($rrule_field) && !empty($item->$rrule_field)) {
        if (!in_array($rrule_field, $rrule_processed)) {
          $rrule_processed[] = $rrule_field;
        }
        else {
          continue;
        }
      }
      
      if (!in_array($id, $processed) && !empty($item->calendar_fields->$alias)) {
        
        // Create from and to date values for each item, adjusted to
        // the correct timezone.
        $values[0] = !empty($item->calendar_fields->$fromto[0]) ? $item->calendar_fields->$fromto[0] : $item->calendar_fields->$alias;
        $values[1] = !empty($item->calendar_fields->$fromto[1]) ? $item->calendar_fields->$fromto[1] : $item->calendar_fields->$alias;
               
        $db_tz   = date_get_timezone_db($tz_handling, isset($item->$tz_alias) ? $item->$tz_alias : $display_timezone_name);
        $to_zone = date_get_timezone($tz_handling, isset($item->$tz_alias) ? $item->$tz_alias : $display_timezone_name);
        
        // Now $display_timezone determines how $item is split into 
        // one entry per day, while $to_zone determines how date is displayed.
        // For now, use the date fields's timezone for the day splitting.
        $display_timezone_name = $to_zone;
        $values_display = array();
        
        // Start date
        $date = date_make_date($values[0], $db_tz, $field['sql_type']);
        if ($db_tz != $to_zone) {
          date_timezone_set($date, timezone_open($to_zone));
        }
        $values[0] = date_format($date, DATE_FORMAT_DATETIME);
            
        if ($display_timezone_name != $to_zone) {
          date_timezone_set($date, $display_timezone);
          $values_display[0] = date_format($date, DATE_FORMAT_DATETIME);
        } 
        else {
          $values_display[0] = $values[0];
        }
            
        // End date
        $date = date_make_date($values[1], $db_tz, $field['sql_type']);
        if ($db_tz != $to_zone) {
          date_timezone_set($date, timezone_open($to_zone));
        }
        $values[1] = date_format($date, DATE_FORMAT_DATETIME);
        if ($display_timezone_name != $to_zone) {
          date_timezone_set($date, $display_timezone);
          $values_display[1] = date_format($date, DATE_FORMAT_DATETIME);
        } 
        else {
          $values_display[1] = $values[1];
        }
               
        // Now $values contain start and end date of a node,
        // expressed as strings in the display (local) timezone.
        // $values_utc does the same in UTC timezone.

        // Get calendar min and max day (not time) as strings in the
        // $display_timezone. Cache in $min_zone_string and $max_zone_string,
        // since many items or fields typically use the samee timezone.
        if (!isset($min_zone_string[$display_timezone_name])) {
          $date = drupal_clone($view->date_info->min_date);
          date_timezone_set($date, $display_timezone);
          $min_zone_string[$display_timezone_name] = date_format($date, DATE_FORMAT_DATE);
          $date = drupal_clone($view->date_info->max_date);
          date_timezone_set($date, $display_timezone);
          $max_zone_string[$display_timezone_name] = date_format($date, DATE_FORMAT_DATE);
        }
        
        // Create a node for each date within the field's date range,
        // limited to the view's date range (regarding only day, not time).
        $now = max($min_zone_string[$display_timezone_name], substr($values_display[0], 0, 10));
        $to  = min($max_zone_string[$display_timezone_name], substr($values_display[1], 0, 10));
        $next = date_make_date($now, $display_timezone);
        
        if ($display_timezone_name != $to_zone) {
          // Make $start and $end (derived from $node) use the timezone $to_zone, just as $values[..] do
          date_timezone_set($next, timezone_open($to_zone));
        }
        if (empty($to)) {
          $to = $now;
        }
                  
        // $now and $next are midnight (in display timezone) on the first day where node will occur.
        // $to is midnight on the last day where node will occur.
        // All three were limited by the min-max date range of the view.
        while ($now <= $to) {
          $node = drupal_clone($item);
          
          // Make sure the pseudo node has the same properties a
          // regular node would have.
          if (isset($node->node_title) && !isset($node->title)) {
            $node->title = $node->node_title;
          }
          if (isset($node->node_type) && !isset($node->type)) {
            $node->type = $node->node_type;
          }

          $exceptions = array('format_interval', 'time ago');
          $node->label = $label;
          $node->format = $format;
          $node->format_time = !in_array($node->format, $exceptions) ? date_limit_format($format, array('hour', 'minute', 'second')) : '';
          $node->url = calendar_get_node_link($node);
                              
          //$node->$fromto[0] = $values[0];
          //$node->$fromto[1] = $values[1];
          
          // Flag which datefield this node is using, in case
          // there are multiple date fields in the view.
          $node->datefield = $alias;

          // If there are other datefields in the View, get rid
          // of them in this pseudo node. There should only be one
          // date in each calendar node.
          foreach ($node as $key => $val) {
            if ($key != $alias && in_array($key, $datefields)) {
              unset($node->$key);
              foreach ($fields['name'] as $other_fields) {
                // If the unused date has other fields, unset them, too.
                if ($other_fields['query_name'] == $key) {
                  foreach ($other_fields['related_fields'] as $related_field) {
                    $key2 = str_replace('.', '_', $related_field);
                    unset($node->$key2);
                  }
                }
              }
            }
          }

          // If we don't deconstruct dates into individual date parts, 
          // use date values as-is.
          if (!$split_dates) {
            $node->calendar_start = $values[0];
            $node->calendar_end = $values[1];
          }
          // Split dates get intersection of current day and the node 
          // value's duration (as strings in $to_zone timezone)
          else {
           // Get start and end of current day
            $start = date_format($next, DATE_FORMAT_DATETIME);
            date_modify($next, '+1 day');
            date_modify($next, '-1 second');
            $end = date_format($next, DATE_FORMAT_DATETIME);
            $node->calendar_start = $values[0] < $start ? $start : $values[0];
            $node->calendar_end = !empty($values[1]) ? ($values[1] > $end ? $end : $values[1]) : $node->calendar_start;
          }
          
          // Make date objects
          $node->calendar_start_date = date_create($node->calendar_start, timezone_open($to_zone));
          $node->calendar_end_date = date_create($node->calendar_end, timezone_open($to_zone));
          if (date_format($node->calendar_start_date, 'H:i:s') == '00:00:00' && 
            date_format($node->calendar_end_date, 'H:i:s') == '23:59:59') {
            $node->calendar_all_day = TRUE;
          }
          else {
            $node->calendar_all_day = FALSE;
          }
          
          // Flag all day values specifically set in date.
          $all_day_field = str_replace(array('_value2', '_value'), '_all_day', $node->datefield);
          if (!empty($all_day_field) && !empty($item->$all_day_field)) {
            $node->calendar_all_day = TRUE;
          }

          // Change string timezones into 
          // calendar_start and calendar_end are UTC dates as formatted strings
          $node->calendar_start = date_format($node->calendar_start_date, DATE_FORMAT_DATETIME);
          $node->calendar_end = date_format($node->calendar_end_date, DATE_FORMAT_DATETIME);
          
          unset($node->calendar_fields);
          if (isset($node) && (empty($node->calendar_start))) {
            // if no date for the node and no date in the item
            // there is no way to display it on the calendar
            unset($node);
          }
          else {
            calendar_node_stripe($view, $node, $alias, $alias);
            calendar_node_taxonomy_stripe($view, $node, $alias, $alias);
            $node->date_id = $id .':'. $pos;

            $nodes[] = $node;
            unset($node);
          }
          $processed[] = $id;
          if ($split_dates) {
            date_modify($next, '+1 second');
            $now = date_format($next, DATE_FORMAT_DATE);
          }
          else {
            break;
          }
        }
      }
    }
  }
  return $nodes;
}
