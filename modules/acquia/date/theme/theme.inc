<?php
// $Id: theme.inc,v 1.1.4.36 2009/06/08 23:10:58 karens Exp $
/**
 *  Preprocessor to construct back and next navigation from the date argument.
 */
function template_preprocess_date_navigation(&$vars) {
  $view = $vars['view'];
  $next_args = $view->args;
  $prev_args = $view->args;
  $pos = $view->date_info->date_arg_pos;
  $min_date = is_object($view->date_info->min_date) ? $view->date_info->min_date : date_now();
  $max_date = is_object($view->date_info->max_date) ? $view->date_info->max_date : date_now();
  
  if (empty($view->date_info->hide_nav)) {
    $prev_date = drupal_clone($min_date);
    date_modify($prev_date, '-1 '. $view->date_info->granularity);
    $next_date = drupal_clone($min_date);
    date_modify($next_date, '+1 '. $view->date_info->granularity);
    $format = array('year' => 'Y', 'month' => 'Y-m', 'day' => 'Y-m-d');
    switch ($view->date_info->granularity) {
      case 'week':
        $next_week = date_week(date_format($next_date, 'Y-m-d'));
        $prev_week = date_week(date_format($prev_date, 'Y-m-d'));
        $next_arg = date_format($next_date, 'Y-\W') . $next_week;
        $prev_arg = date_format($prev_date, 'Y-\W') . $prev_week;
        break;
      default:
        $next_arg = date_format($next_date, $format[$view->date_info->granularity]);
        $prev_arg = date_format($prev_date, $format[$view->date_info->granularity]);
    }
    $next_path = str_replace($view->date_info->date_arg, $next_arg, $view->date_info->url);
    $prev_path = str_replace($view->date_info->date_arg, $prev_arg, $view->date_info->url);
    $next_args[$pos] = $next_arg;
    $prev_args[$pos] = $prev_arg;
    $vars['next_url'] = date_real_url($view, NULL, $next_arg);
    $vars['prev_url'] = date_real_url($view, NULL, $prev_arg);
    $vars['next_options'] = $vars['prev_options'] = array();
  }
  else {
    $next_path = '';
    $prev_path = '';
    $vars['next_url'] = '';
    $vars['prev_url'] = '';
    $vars['next_options'] = $vars['prev_options'] = array();
  }
    
  // Check whether navigation links would point to 
  // a date outside the allowed range.
  if (!empty($next_date) && !empty($vars['next_url']) && date_format($next_date, 'Y') > $view->date_info->max_allowed_year) {
    $vars['next_url'] = '';    
  }
  if (!empty($prev_date) && !empty($vars['prev_url']) && date_format($prev_date, 'Y') < $view->date_info->min_allowed_year) {
    $vars['prev_url'] = '';    
  }

  $vars['prev_options'] += array('attributes' => array());
  $vars['next_options'] += array('attributes' => array());
  $prev_title = '';
  $next_title = '';

  // Build next/prev link titles.
  switch($view->date_info->granularity) {
    case 'year':
      $prev_title = t('Navigate to previous year');
      $next_title = t('Navigate to next year');
      break;
    case 'month':
      $prev_title = t('Navigate to previous month');
      $next_title = t('Navigate to next month');
      break;
    case 'week':
      $prev_title = t('Navigate to previous week');
      $next_title = t('Navigate to next week');
      break;      
    case 'day':
      $prev_title = t('Navigate to previous day');
      $next_title = t('Navigate to next day');
      break;
  }
  $vars['prev_options']['attributes'] += array('title' => $prev_title);
  $vars['next_options']['attributes'] += array('title' => $next_title);
  
  // Add nofollow for next/prev links.
  $vars['prev_options']['attributes'] += array('rel' => 'nofollow');
  $vars['next_options']['attributes'] += array('rel' => 'nofollow');
  
  $link = FALSE;
  // Month navigation titles are used as links in the block view.
	if (!empty($view->date_info->block) && $view->date_info->granularity == 'month') {
	  $link = TRUE;
  }
  
  $nav_title = theme('date_nav_title', $view->date_info->granularity, $view, $link);
  $vars['nav_title'] = $nav_title;
  $vars['block'] = !empty($view->date_info->block);
}
/**
 * Theme the calendar title
 */
function theme_date_nav_title($granularity, $view, $link = FALSE, $format = NULL) {
  switch ($granularity) {
    case 'year':
      $title = $view->date_info->year;
      $url = $view->date_info->url .'/'. $view->date_info->year;
      break;
    case 'month':
      $format = !empty($format) ? $format : (empty($view->date_info->mini) ? 'F Y' : 'F');
      $title = date_format_date($view->date_info->min_date, 'custom', $format);
      $url = $view->date_info->url .'/'. $view->date_info->year .'-'. date_pad($view->date_info->month);
      break;
    case 'day':
      $format = !empty($format) ? $format : (empty($view->date_info->mini) ? 'l, F j Y' : 'l, F j');
      $title = date_format_date($view->date_info->min_date, 'custom', $format);
      $url = $view->date_info->url .'/'. $view->date_info->year .'-'. date_pad($view->date_info->month) .'-'. date_pad($view->date_info->day);
      break;
    case 'week':
    	$format = !empty($format) ? $format : (empty($view->date_info->mini) ? 'F j Y' : 'F j');
      $title = t('Week of @date', array('@date' => date_format_date($view->date_info->min_date, 'custom', $format)));
    	$url = $view->date_info->url .'/'. $view->date_info->year .'-W'. date_pad($view->date_info->week);
    	break;
  }
  // TODO Update this.
  if (!empty($view->date_info->mini) || $link) {
  	// Month navigation titles are used as links in the mini view.
   $attributes = array('title' => t('View full page month'));
  	return l($title, $view->date_info->url, array('attributes' => $attributes));
  }
  else {
    return $title;
  }  
}

/**
 *  Preprocessor to construct an ical vcalendar
 * 
 * @param $events
 *   An array of events where each event is an array keyed on the uid:
 *    'start'
 *      Start date object,
 *    'end'
 *      End date object, optional, omit for all day event.
 *    'summary'
 *      Title of event (Text)
 *    'description'
 *      Description of event (Text)
 *    'location'
 *      Location of event (Text or vvenue id)
 *    'uid'
 *      ID of the event for use by calendaring program, usually the url of the node
 *    'url'
 *      URL of event information
 * 
 *    'alarm'
 *      sub-array of alarm information for the event, including:
 *      - 'action' - the action to take, either 'DISPLAY' or 'EMAIL'
 *      - 'trigger' - the time period for the trigger, like -P2D.
 *      - 'repeat' - the number of times to repeat the alarm.
 *      - 'duration' - the time period between repeated alarms, like P1D.
 *      - 'description' - the description of the alarm.
 *      An email alarm should have two additional parts:
 *      - 'email' - a comma-separated list of email recipients.
 *      - 'summary' - the subject of the alarm email.
 *
 * @param $calname
 *   Name of the calendar.  Use site name if none is specified.
 * 
 */
function template_preprocess_date_vcalendar(&$vars) {
    
  $vars['current_date'] = date_format(date_now(), DATE_FORMAT_ICAL);
  $vars['site_timezone'] = date_default_timezone_name();
  $vars['calname'] = date_ical_escape_text(!empty($vars['calname']) ? $vars['calname'] : variable_get('site_name', ''));
  
  // Format the event results as iCal expects.
  $events_in = $vars['events'];
  $events = array();
  $rows = $vars['rows'];
  foreach ($events_in as $uid => $event) {
    $row = array_shift($rows);
    // Omit any items with empty dates.
    if (!empty($event['start'])) {
      $events[$uid] = $event;
      $timezone = timezone_name_get(date_timezone_get($event['start']));
      if (!empty($timezone)) {
        $events[$uid]['timezone'] = "TZID=$timezone;";
      }
      else {
        $events[$uid]['timezone'] = '';
      }
      $date_format = ($row->calendar_all_day == TRUE) ? DATE_FORMAT_ICAL_DATE : DATE_FORMAT_ICAL;
      $events[$uid]['start'] = date_format($event['start'], $date_format);
      if ($event['start'] && $event['end']) {
        $events[$uid]['end'] = date_format($event['end'], $date_format);
      }
      else {
        $events[$uid]['end'] = $events[$uid]['start'];
      }
      foreach ($event as $key => $value) {
        if (is_string($value)) {
          $event[trim($key)] = trim($value);
        }
      }
      
      // Escape text values.
      foreach ($event as $key => $value) {
        if ($key == 'alarm') {
          foreach ($value as $alarm_key => $alarm_value) {
            if (in_array($alarm_key, array('summary', 'description'))) {
              $events[$uid]['alarm'][$alarm_key] = date_ical_escape_text($alarm_value);
            }
          }
        }
        elseif (in_array($key, array('summary', 'description', 'location'))) {
          $events[$uid][$key] = date_ical_escape_text($value);
        }
      }
    }
  }
  
  $vars['events'] = $events;  
}

/**
 * Preprocessor for Date Views filter form.
 */
function template_preprocess_date_views_filter_form(&$vars) {
  $form = $vars['form'];
  $vars['date'] = drupal_render($form['valuedate']);
  $vars['mindate'] = drupal_render($form['mindate']);
  $vars['maxdate'] = drupal_render($form['maxdate']);
  $vars['adjustment'] = drupal_render($form['valueadjustment']);
  $vars['minadjustment'] = drupal_render($form['minadjustment']);
  $vars['maxadjustment'] = drupal_render($form['maxadjustment']);
  $vars['description'] = drupal_render($form['description']) . drupal_render($form);
}

/**
 * Format a date timezone element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_timezone($element) {
  return '<div class="date-clear">'. theme('form_element', $element, $element['#children']) .'</div>';
}

/**
 * Format a date selection element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_select($element) {
  $output = '';
  $class = 'container-inline-date';
  // Add #date_float to allow date parts to float together on the same line. 
  if (empty($element['#date_float'])) {
    $class .= ' date-clear-block';
  }
  if (isset($element['#children'])) {
    $output = $element['#children'];
  }
  return '<div class="'. $class .'">'. theme('form_element', $element, $output) .'</div>';
}

/**
 * Format a date text element.
 *
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 * @return
 *   A themed HTML string representing the date selection boxes.
 */
function theme_date_text($element) {
  $output = '';
  $class = 'container-inline-date';
  // Add #date_float to allow date parts to float together on the same line. 
  if (empty($element['#date_float'])) {
    $class .= ' date-clear-block';
  }
  if (isset($element['#children'])) {
    $output = $element['#children'];
  }
  return '<div class="'. $class .'">'. theme('form_element', $element, $output) .'</div>';
}

/**
 *  Themes for date input form elements
 */
function theme_date_select_element($element) {
  $part = array_pop($element['#parents']);
  return '<div class="date-'. $part .'">'. theme('select', $element) .'</div>';
}

function theme_date_textfield_element($element) {
  $part = array_pop($element['#parents']);
  return '<div class="date-'. $part .'">'. theme('textfield', $element) .'</div>';
}

/**
 * Functions to separate date parts in form.
 *
 * Separators float up to the title level for elements with titles,
 * so won't work if this element has titles above the element date parts.
 */
function theme_date_part_hour_prefix($element) {
  if ($element['#date_label_position'] != 'above') {
    return '<span class="form-item date-spacer">&nbsp;-&nbsp;</span>';
  }
}

function theme_date_part_minsec_prefix($element) {
  if ($element['#date_label_position'] != 'above') {
    return '<span class="form-item date-spacer">:</span>';
  }
}

/**
 * Format labels for each date part in a date_select.
 *
 * @param $part_type
 *   the type of field used for this part, 'textfield' or 'select'
 * @param $element
 *   An associative array containing the properties of the element.
 *   Properties used: title, value, options, description, required and attributes.
 */
function theme_date_part_label_year($part_type, $element) {
  return date_t('Year', 'datetime');
}
function theme_date_part_label_month($part_type, $element) {
  return date_t('Month', 'datetime');
}
function theme_date_part_label_day($part_type, $element) {
  return date_t('Day', 'datetime');
}
function theme_date_part_label_hour($part_type, $element) {
  return date_t('Hour', 'datetime');
}
function theme_date_part_label_minute($part_type, $element) {
  return date_t('Minute', 'datetime');
}
function theme_date_part_label_second($part_type, $element) {
  return date_t('Second', 'datetime');
}
function theme_date_part_label_ampm($part_type, $element) {
  return ' ';
}
function theme_date_part_label_timezone($part_type, $element) {
  return t('Timezone');
}

/**
 * Theme for a date block that looks like a mini calendar day.
 * Pass in a date object already set to the right timezone, 
 * format as a calendar page date. The calendar styling is created in css.
 */
function theme_date_calendar_day($date) {
  if (empty($date)) {
    return NULL;
  }
  return '<div class="date-calendar-day">' .
    '<span class="month">' . date_format_date($date, 'custom', 'M') . '</span>' .
    '<span class="day">' . date_format_date($date, 'custom', 'j') . '</span>' .
    '<span class="year">' . date_format_date($date, 'custom', 'Y') . '</span>' .
  '</div>';
}

function theme_date_time_ago($start_date, $end_date, $interval = 2) {
  // If no date is sent, then return nothing
  if (empty($start_date) || empty($end_date)){
    return NULL;
  }

  // Time to compare dates to
  $now = date_format(date_now(), DATE_FORMAT_DATETIME);
  $start = date_format($start_date, DATE_FORMAT_DATETIME);
  $end = date_format($end_date, DATE_FORMAT_DATETIME);
  
  // 1) The date is entirely in the future
  if ($now < $start) {
    return t('!time from now', array('!time' => date_format_interval($start_date, $interval)));
  }
  // 2) Ongoing date
  elseif ($now > $start && $now <= $end) {
    //return t('Started !time ago', array('!time' => $dates['value']['interval']));
    return t('ongoing');
  }
  // 3) Date is in the past (format_interval added 'ago' to the value).
  else {
    return date_format_interval($start_date, $interval);
  }
}