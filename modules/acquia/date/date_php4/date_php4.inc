<?php
// $Id: date_php4.inc,v 1.27.4.39 2009/05/12 09:58:59 karens Exp $

/**
 * @file
 *
 * PHP 5 Date and Timezone function substitutions for PHP 4.
 * Replicates some PHP 5 timezone functions and procedures.
 * Not all PHP 5 functions have equivalents.
 *
 * Simpletest tests are provided to use with Simpletest module.
 *
 * Supported:
 *   date_create()
 *     with limitations, input date must be one of:
 *       - YYYY-MM-DD HH:MM:SS
 *       - YYYY-MM-DDTHH:MM:SS
 *       - 'now'
 *       - unix timestamp prefixed with '@', like '@99999999'
 *       - or something strtotime will understand (only for dates later than 1970).
 *     also must supply timezone as second argument to date_create().
 *   date_modify()
 *     with limitations, must supply modification in form like:
 *       '+1 day|week|month|year|Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday'
 *       '-3 day|week|month|year|Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday'
 *   date_format()
 *   date_date_set()
 *   date_timezone_set()
 *   date_offset_get()
 *   date_timezone_get()
 *   timezone_offset_get()
 *   timezone_name_get()
 *   timezone_open()
 *   timezone_abbreviations_list()
 *   timezone_identifiers_list()
 *
 * Not supported:
 *   timezone_ transitions_ get()
 *    - we just don't have all those historical transitions available in PHP 4
 */
if (!function_exists('date_create')) {
/**
 *  PHP 4 equivalent for date_create().
 *
 *  @param string $date_in
 *    - 'now' or empty
 *    - anything strtotime will support for a post-1970 date,
 *    - a timestamp (must be prefixed with a @ to be interpreted as a
 *        timestamp in PHP 5 -- see http://bugs.php.net/bug.php?id=40171)
 *    - otherwise, must be a DATETIME date (YYYY-MM-DD HH:MM:SS) or
 *        ISO date (YYYY-MM-DDTHH:MM:SS)
 *        PHP 5 accepts more alternative formats, but that's too complicated
 *        for us to handle here, so a standardized format is required.
 *
 *  @param string $timezone
 *    PHP 5 uses a timezone object, in PHP 4 use a timezone name.
 *  @return object
 *    Returns and object structured like:
 *      Object =>
 *        value => '2007-04-15 09:25'
 *        timezone => 'UTC'
 */
  function date_create($date_in = NULL, $timezone = NULL) {
    if (empty($date_in) || strtolower($date_in) == 'now') {
      // Create a current date for an empty value or 'now'.
      $date_out = date_date('Y-m-d H:i:s', (time()), $timezone);
    }
    elseif (substr($date_in, 0, 1) == '@' && is_numeric(substr($date_in, 1))) {
      // An number prefixed with '@' is interpreted as a timestamp.
      $date_out = date_date('Y-m-d H:i', substr($date_in, 1), $timezone);
    }
    elseif (preg_match(DATE_REGEX_LOOSE, $date_in, $regs)) {
      $year = date_pad(isset($regs[1]) ? $regs[1] : 0, 4);
      $month = date_pad(isset($regs[2]) ? $regs[2] : 0);
      $day = date_pad(isset($regs[3]) ? $regs[3] : 0);
      $hour = date_pad(isset($regs[5]) ? $regs[5] : 0);
      $minute = date_pad(isset($regs[6]) ? $regs[6] : 0);
      $second = date_pad(isset($regs[7]) ? $regs[7] : 0);
      $date_out = "$year-$month-$day $hour:$minute:$second";
    }
    else {
      // Try to use strtotime, will only work on post-1970 dates.
      // Only use it if the value is big enough that timezone conversions
      // won't make it into a negative value.
      $test = @strtotime($date_in .' '. $timezone);
      if ($test > 86400) {
        $date_out = date_date('Y-m-d H:i', $test, $timezone);
      }
    }
    if (empty($date_out)) {
      return NULL;
    }
    $date = new StdClass();
    $date->value = $date_out;
    $date->timestamp = date_datetime2timestamp($date->value, $timezone);
    $date->timezone = $timezone;
    return $date;
  }
}

if (!function_exists('date_format')) {
/**
 * PHP 4 equivalent for date_format().
 *
 * Some formats that work in PHP 5 won't work in PHP 4, many won't work for
 * dates earlier than 1970. Filter out those that need special treatment.
 * No translation done here because the PHP 5 date_format() function isn't
 * doing any translation and we need to return identical values in both.
 *
 * @param object $date
 * @param string $format
 * @return string
 *   return formatted date
 */
  function date_format($date, $format) {
    $php5_formats = array('N', 'o', 'e', 'P', 'c', '\\');
    $timestamp = $date->timestamp;
    $date_string = '';
    $max = strlen($format);
    for ($i = 0; $i < $max; $i++) {
      $c = $format[$i];
      if ($date->timezone == 'UTC' && abs($timestamp) <= 0x7FFFFFFF && !in_array($c, $php5_formats)) {
        $date_string .= gmdate($c, $timestamp);
      }
      else{
        require_once('./'. drupal_get_path('module', 'date_php4') .'/date_php4_calc.inc');
        switch ($c) {
          case 'U':
            $date_string .= $timestamp;
            break;
          case 'l':
          case 'w':
          case 'D':
            $dow = date_dow(date_part_extract($date->value, 'day'),
              date_part_extract($date->value, 'month'),
              date_part_extract($date->value, 'year'));
            if ($c == 'w') {
              $date_string .= $dow;
            }
            elseif ($c == 'l') {
              $days = date_week_days_untranslated();
              $date_string .= $days[$dow];
            }
            elseif ($c == 'D') {
              // There is no date_week_days_abbr_untranslated(). 
              // This rule works for English, untranslated day names.
              $days = date_week_days_abbr();
              $date_string .= substr($days[$dow], 0, 3);
            }
            break;
          case 'L':
            $date_string .= date_is_leap_year(date_part_extract($date->value, 'year'));
            break;
          case 'z':
            $date_string .= date_calc_julian_date(date_part_extract($date->value, 'day'),
            date_part_extract($date->value, 'month'),
            date_part_extract($date->value, 'year'));
            break;
          case 'N':
            $w = date_date('w', $timestamp, $date->timezone);
            $date_string .= $w != 0 ? $w : 7;
            break;
          case 'o':
            $iso_week = date_calc_gregorian_to_ISO(date_part_extract($date->value, 'day'),
              date_part_extract($date->value, 'month'),
              date_part_extract($date->value, 'year'));
            $iso = explode('-', $iso_week);
            $date_string .= ($c == 'o') ? $iso[0] : $iso[1];
            break;
          case 'O':
            $date_string .= sprintf('%s%02d%02d', (date_offset_get($date) < 0 ? '-' : '+'), abs(date_offset_get($date) / 3600), abs(date_offset_get($date) % 3600) / 60);
            break;
          case 'P':
            $date_string .= sprintf('%s%02d:%02d', (date_offset_get($date) < 0 ? '-' : '+'), abs(date_offset_get($date) / 3600), abs(date_offset_get($date) % 3600) / 60);
            break;
          case 'e':
            $date_string .= $date->timezone;
            break;
          case 'Z':
            $date_string .= date_offset_get($date);
            break;
          case '\\':
            $date_string .= $format[++$i];
            break;
          case 't':
            $date_string .= date_calc_days_in_month(date_part_extract($date->value, 'month'),
              date_part_extract($date->value, 'year'));
            break;
          default:
            if (strpos('AaeDFlMTBcdGgHhIijLmNnOoPrSstUuWwYyZz', $c) !== FALSE) {
              $date_string .= date_date($c, $timestamp, $date->timezone);
            }
            else {
              $date_string .= $c;
            }
        }
      }
    }
    return $date_string;
  }
}

/**
 *  PHP 4 equivalent for date_date_set().
 *
 *  @param $date
 *    a date object returned by date_create().
 *  @param $year
 *    a new year.
 *  @param $month
 *    a new month.
 *  @param $day
 *    a new day.
 *  @return $date
 *    the date objected updated for the new timezone.
 *
 */
if (!function_exists('date_date_set')) {
  function date_date_set(&$date, $year, $month, $day) {
    $new_date = substr_replace($date->value, date_pad($year, 4), 0, 4);
    $new_date = substr_replace($new_date, date_pad($month), 5, 2);
    $new_date = substr_replace($new_date, date_pad($day), 8, 2);
    $date = date_create($new_date, $date->timezone);
    return $date;
  }
}

/**
 *  PHP 4 equivalent for date_date_set().
 *
 *  @param $date
 *    a date object returned by date_create().
 *  @param $year
 *    a new year.
 *  @param $month
 *    a new month.
 *  @param $day
 *    a new day.
 *  @return $date
 *    the date objected updated for the new timezone.
 *
 */
if (!function_exists('date_time_set')) {
  function date_time_set(&$date, $hour, $minute, $second) {
    $new_date = substr_replace($date->value, date_pad($hour), 11, 2);
    $new_date = substr_replace($new_date, date_pad($minute), 14, 2);
    $new_date = substr_replace($new_date, date_pad($second), 16, 2);
    $date = date_create($new_date, $date->timezone);
    return $date;
  }
}

if (!function_exists('date_timezone_set')) {
/**
 *  PHP 4 equivalent for date_timezones_set().
 *
 *  @param $date
 *    a date object returned by date_create().
 *  @param $timezone
 *    a new timezone for the date object.
 *  @return $date
 *    the date objected updated for the new timezone.
 *
 */
  function date_timezone_set(&$date, $timezone) {
    $date->timezone = $timezone;
    $date->value = date_date(DATE_FORMAT_DATETIME, $date->timestamp, $timezone);
    return $date;
  }
}

if (!function_exists('timezone_open')) {
/**
 *  PHP 4 equivalent for timezone_open().
 *   Just track the timezone name.
 */
  function timezone_open($timezone) {
    return $timezone;
  }
}

if (!function_exists('timezone_name_get')) {
/**
 *  PHP 4 equivalent for timezone_name_get().
 */
  function timezone_name_get($timezone) {
    return $timezone;
  }
}

if (!function_exists('date_timezone_get')) {
/**
 *  PHP 4 equivalent for date_timezone_get().
 */
  function date_timezone_get($date) {
    return $date->timezone;
  }
}

if (!function_exists('timezone_offset_get')) {
/**
 *  PHP 4 equivalent for timezone_offset_get().
 *
 *  Cache results for expensive process of getting offsets for each timezone.
 *  Each cached timezone array will show whether the timezone has dst and the
 *  dst and non-dst offset for that timezone.
 *
 *  @param $timezone
 *    a timezone returned by timezone_open().
 *  @param $date
 *    a date object returned by date_create().
 *  @return $offset
 *    the offset in seconds for the supplied date object.
 */
  function timezone_offset_get($timezone, $date) {
    if (empty($timezone) || $timezone == 'UTC' || !date_timezone_is_valid($timezone)) {
      return 0;
    }
    static $zones = array(), $offsets = array();
    if (!in_array($timezone, $zones)) {
      $cached = cache_get('date_timezone_offsets:'. $timezone);
      $offsets[$timezone] = isset($cached->data) ? $cached->data : array();
      $zones[] = $timezone;
      if (empty($offsets[$timezone])) {
        $offsets[$timezone] = array('dst' => 0);
        $zones = timezone_abbreviations_list();
        foreach ($zones as $key => $abbr) {
          foreach ($abbr as $zone) {
            if ($zone['timezone_id'] == $timezone && $zone['dst'] == 1 && !isset($offsets[$timezone]['dst_offset'])) {
              $offsets[$timezone]['dst_offset'] = $zone['offset'];
              $offsets[$timezone]['dst'] = 1;
            }
            elseif ($zone['timezone_id'] == $timezone && $zone['dst'] != 1 && !isset($offsets[$timezone]['offset'])) {
              $offsets[$timezone]['offset'] = $zone['offset'];
            }
          }
        }
        cache_set('date_timezone_offsets:'. $timezone, $offsets[$timezone]);
      }
    }
    $timestamp = $date->timestamp;
    if ($offsets[$timezone]['dst'] == 1) {
      if (date_is_dst($timestamp, $timezone, $offsets[$timezone]['dst'])) {
        return $offsets[$timezone]['dst_offset'];
      }
    }
    return $offsets[$timezone]['offset'];
  }
}

if (!function_exists('date_offset_get')) {
/**
 *  PHP 4 equivalent for date_offset_get().
 *
 *  Cache results for expensive process of getting offsets for each timezone.
 *  Each cached timezone array will show whether the timezone has dst and the
 *  dst and non-dst offset for that timezone.
 *
 *  @param $date
 *    a date object returned by date_create().
 *  @return $offset
 *    the offset in seconds for the supplied date object.
 */
  function date_offset_get($date) {
    return timezone_offset_get(timezone_open($date->timezone), $date);
  }
}

if (!function_exists('date_modify')) {
/**
 * A limited set of options are provided here to modify dates.
 * Uses strtotime() on dates after 1970. Uses date_php4_calc.inc to do
 * calculations on older dates.
 *
 * Will work for things like date_modify($date, '+1 Sunday');
 *
 * @param $date
 *   a date object created by date_create() to use as a reference
 *   point for the calculation.
 * @param $change
 *   something like following phrases:
 *     '+1 day|week|month|year|Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday'
 *     '-3 day|week|month|year|Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday'
 *
 * @return
 *   the date that matches the phrase.
 *
 */
  function date_modify(&$date, $change) { 
    $cdate = $date->value;
    $time = substr($date->value, 11, 8);
   
    // We have to use strings like 'First Sunday' instead of
    // +1 Sunday to overcome a PHP5 bug (see #369020).
    // Swap those values out here and go back to '+1 Sunday',
    // the code that works in PHP4.
    $replace = array_flip(date_order());
    $change = strtr($change, $replace);
    
    $change = trim($change);   
    //Date is too old for strtotime(), or looking for custom +/- values, use date_calc instead.
    if (substr($change, 0, 1) != '+' && substr($change, 0, 1) != '-'
    && 1971 < date_part_extract($cdate, 'year')
    && date_part_extract($cdate, 'year') < 2038) {
      $cdate = strtotime($change, strtotime($cdate .' UTC'));
      // strtotime will sometimes end up with a bogus adjustment for daylight
      // savings time, so compute the date and force them time back to the original.
      $date->value = substr_replace(date_date(DATE_FORMAT_DATETIME, $cdate, $date->timezone), $time, 11);
    }
    else {
      require_once('./'. drupal_get_path('module', 'date_php4') .'/date_php4_calc.inc');
      $days = date_week_days_untranslated();
      $dows = array_flip($days);
      preg_match('/([+|-])\s?([0-9]{1,32})\s?([day(s)?|week(s)?|month(s)?|year(s)?|hour(s)?|minute(s)?|second(s)?|Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday]{1,10})/', $change, $results);
      $direction = $results[1];
      $count = $results[2];
      $item = $results[3];
      if (empty($results) || empty($item)) {
        return;
      }
      if (substr($item, -1) == 's') {
        $item = substr($item, 0, strlen($item) - 1);
      }
      // Process +/- Sunday|Monday|Tuesday|Wednesday|Thursday|Friday|Saturday
      if (in_array($item, $days)) {
        $dow = $dows[$item];
        if ($direction == '+') {
          $function = 'date_calc_next_day_of_week';
        }
        else {
          $function = 'date_calc_prev_day_of_week';
        }
        for ($i = 1; $i <= $count; $i++) {
          $cdate = $function($dow,
            date_part_extract($cdate, 'day'),
            date_part_extract($cdate, 'month'),
            date_part_extract($cdate, 'year'));
        }
        $date->value = $cdate .' '. $time;
      }
      elseif ($item == 'day') {
        if ($direction == '+') {
          $function = 'date_calc_next_day';
        }
        else {
          $function = 'date_calc_prev_day';
        }
        for ($i = 1; $i <= $count; $i++) {
          $cdate = $function(date_part_extract($cdate, 'day'),
            date_part_extract($cdate, 'month'),
            date_part_extract($cdate, 'year'));
        }
        $date->value = $cdate .' '. $time;
      }
      elseif ($item == 'month') {
        if ($direction == '+') {
          $function = 'date_calc_begin_of_month_by_span';
        }
        else {
          $function = 'date_calc_end_of_month_by_span';
        }
        // Find the next end or beginning of month that matches the search.
        $day = date_part_extract($cdate, 'day');
        $cdate = $function($direction . $count, date_part_extract($cdate, 'month'), date_part_extract($cdate, 'year'));
        // Construct a new date with the current day of the month and the new month and year.
        $mon = date_part_extract($cdate, 'month');
        $year = date_part_extract($cdate, 'year');
        $date->value = date_pad($year, 4) .'-'. date_pad($mon) .'-'. date_pad($day) .' '. $time;
      }
      elseif ($item == 'week') {
        $dow = date_day_of_week($date);
        if ($direction == '+') {
          $function = 'date_calc_begin_of_next_week';
        }
        else {
          $function = 'date_calc_begin_of_prev_week';
        }
        // Move to right week.
        for ($i = 1; $i <= $count; $i++) {
          $cdate = $function(date_part_extract($cdate, 'day'),
            date_part_extract($cdate, 'month'),
            date_part_extract($cdate, 'year'));
        }
        // Move to the right day of the week, if we're not looking for the first day.
        if ($dow != variable_get('date_first_day', 1)) {
          for ($i = 1; $i <= $dow; $i++) {
            $cdate = date_calc_next_day(date_part_extract($cdate, 'day'),
              date_part_extract($cdate, 'month'),
              date_part_extract($cdate, 'year'));
          }
        }
        $date->value = $cdate .' '. $time;
      }
      elseif ($item == 'year') {
        // Move to the new year.
        $year = date_part_extract($cdate, 'year');
        for ($i = 1; $i <= $count; $i++) {
          if ($direction == '+') {
            $year++;
          }
          else {
            $year--;
          }
        }
        // Construct a new date with the current day and month and the new year.
        $day = date_part_extract($cdate, 'day');
        $mon = date_part_extract($cdate, 'month');
        $date->value = date_pad($year, 4) .'-'. date_pad($mon) .'-'. date_pad($day) .' '. $time;
      }
      else {
        switch ($item) {
          case 'hour':
            $count = $count * 3600;
            break;
          case 'minute':
            $count = $count * 60;
            break;
        }
        switch ($direction) {
          case '-':
            $timestamp = $date->timestamp - $count;
            break;
          default:
            $timestamp = $date->timestamp + $count;
        }
        $date->value = date_date('Y-m-d H:i:s', $timestamp, $date->timezone);
      }
    }
    $date->timestamp = date_datetime2timestamp($date->value, $date->timezone);
    return $date;
  }
}

if (!function_exists('timezone_identifiers_list')) {
/**
 * PHP 4 equivalent for timezone_identifiers_list().
 *
 * Expanded array looks like:
 * array(
 *   0 => 'Africa/Abidjan',
 *   1 => 'Africa/Accra',
 *   2 => 'Africa/Addis_Ababa',
 *   ...
 */
  function timezone_identifiers_list() {
    static $timezone_identifiers_list = array();
    if (empty($timezone_identifiers_list)) {
      include(drupal_get_path('module', 'date_php4') .'/date_php4_tz_identifiers.inc');
    }
    return $timezone_identifiers_list;
  }
}

if (!function_exists('timezone_abbreviations_list')) {
/**
 * PHP 4 equivalent for timezone_abbreviations_list().
 *   Array keyed on timezone abbreviation.
 *
 * Expanded array looks like:
 * array(
 *   'cdt' => array (
 *     0 => array (
 *      'dst' => true,
 *      'offset' => -18000,
 *      'timezone_id' => 'America/Chicago',
 *     ),
 *     1 => array (
 *      'dst' => true,
 *      'offset' => -14400,
 *      'timezone_id' => 'America/Havana',
 *     )...
 *
 */
  function timezone_abbreviations_list() {
    static $timezone_abbreviations_list = array();
    if (empty($timezone_abbreviations_list)) {
      include(drupal_get_path('module', 'date_php4') .'/date_php4_tz_abbreviations_list.inc');
    }
    return $timezone_abbreviations_list;
  }
}

/**
 * Create a timestamp from a datetime value.
 *
 * Can't use date_convert() to turn datetime to unix within the
 * PHP 4 emulation  because it creates endless loops.
 */
function date_datetime2timestamp($datetime, $timezone) {
  preg_match(DATE_REGEX_LOOSE, $datetime, $regs);
  return date_mktime((isset($regs[5]) ? $regs[5] : 0), (isset($regs[6]) ? $regs[6] : 0), (isset($regs[7]) ? $regs[7] : 0), (isset($regs[2]) ? $regs[2] : 0), (isset($regs[3]) ? $regs[3] : 0), (isset($regs[1]) ? $regs[1] : 0), $timezone);
}

/**
 * Check if time is in Daylight Savings Time
 *   Uses dst region code, as stored in the $timezone_map array
 *   from date_php4_tz_map.inc
 *
 * @param $timezone name
 * @param $timestamp
 * @return
 *   0 or 1
 */
function date_is_dst($timestamp, $timezone = NULL) {
  if (abs($timestamp) > 0x7FFFFFFF) {
    return FALSE;
  }
  if (empty($timezone)) {
    $timezone = date_default_timezone_name();
  }
  
  static $timezone_map = array();
  if (empty($timezone_map)) {
    include(drupal_get_path('module', 'date_php4') .'/date_php4_tz_map.inc');
  }
  $region = array_key_exists($timezone, $timezone_map) ? $timezone_map[$timezone]['dst_region'] : 0;
           
  // This should really be done with date_date() to get the right timezone
  // adjustment for the year, but that leads to circular code that won't
  // work, so we're stuck with date(), which hopefully will create the right
  // year in most cases.
  $year = date('Y', $timestamp);

  // Information on Daylight Saving time was obtained from http://webexhibits.org/daylightsaving/g.html
  // Note that 'last sunday' is interpreted as 'the last sunday before...'.
  switch ($region) {
    case 0:
      return 0;
    case 1: // Egypt
      // start of DST (last Friday in April)
      $dststart = strtotime("last friday UTC", strtotime("1 may $year UTC"));
      // end of DST (last Thursday in September)
      $dstend = strtotime("last thursday UTC", strtotime("1 october $year UTC"));
      break;
    case 2: // Namibia
      // start of DST (first Sunday in September)
      $dststart = strtotime("first sunday UTC", strtotime("1 september $year UTC"));
      // end of DST (first Sunday April)
      $dstend = strtotime("first sunday UTC", strtotime("1 april $year UTC"));
      break;
    case 3: // Former USSR
      // start of DST (last Sunday in March)
      $dststart = strtotime("last sunday UTC", strtotime("1 april $year UTC"));
      // end of DST (last Sunday October)
      $dstend = strtotime("last sunday UTC", strtotime("1 november $year UTC"));
      break;
    case 4: // Iraq, Syria
      // start of DST (April 1st)
      $dststart = strtotime("1 april $year UTC");
      // end of DST (October 1st)
      $dstend = strtotime("1 october $year UTC");
      break;
    case 5: // Israel
      // start of DST (last Friday befor April 2nd, 2am)
      $dststart = strtotime("-1 week friday GMT", strtotime("2 april $year GMT")) + 7200;
      // end of DST (last sunday before 10th of tishray. see http://cgate.co.il/navigation/shaon_kaiz.htm.
      switch ($year) {
        case '2007':
          $dstend = strtotime("-1 sunday GMT", strtotime("16 september $year GMT"))+ 7200;
          break;
        case '2008':
          $dstend = strtotime("-1 sunday GMT",strtotime("9 october $year GMT"))+7200;
          break;
        case '2009':
          $dstend = strtotime("-1 sunday GMT", strtotime("28 september $year GMT"))+ 7200;
          break;
        case '2010':
          $dstend = strtotime("-1 sunday GMT", strtotime("18 september $year GMT"))+ 7200;
          break;
        case '2011':
          $dstend = strtotime("-1 sunday GMT", strtotime("8 october $year GMT"))+ 7200;
          break;
        case '2012':
          $dstend = strtotime("-1 sunday GMT", strtotime("26 september $year GMT"))+ 7200;
          break;
        case '2013':
          $dstend = strtotime("-1 sunday GMT", strtotime("14 september $year GMT"))+ 7200;
          break;
        case '2014':
          $dstend = strtotime("-1 sunday GMT", strtotime("4 october $year GMT"))+ 7200;
          break;
        case '2015':
          $dstend = strtotime("-1 sunday GMT", strtotime("23 september $year GMT"))+ 7200;
          break;
        case '2016':
          $dstend = strtotime("-1 sunday GMT", strtotime("12 october $year GMT"))+ 7200;
          break;
        default:
          return false;
      }
      break;
    case 6: // Lebanon, Kirgizstan
      // start of DST (Last Sunday in March)
      $dststart = strtotime("last sunday UTC", strtotime("1 april $year UTC"));
      // end of DST (Last Sunday in October)
      $dstend = strtotime("last sunday UTC", strtotime("1 november $year UTC"));
      break;
    case 7: // Palestine
      // start of DST (First Friday on or after April 15th)
      $dststart = strtotime("next friday UTC", strtotime("14 april $year UTC"));
      // end of DST (First Friday on or after October 15th)
      $dstend = strtotime("next friday UTC", strtotime("14 october $year UTC"));
      break;
    case 8: // Iran
      // start of DST (the first day of Farvardin (March 21))
      $dststart = strtotime("21 march $year UTC");
      // end of DST (the first day of Mehr (September 23))
      $dstend = strtotime("23 september $year UTC");
      break;
    case 9: // South Australia
      // start of DST  (last Sunday in October)
      $dststart = strtotime("last sunday UTC", strtotime("1 november $year UTC"));
      // end of DST (last Sunday in March)
      $dstend = strtotime("last sunday UTC", strtotime("1 april $year UTC"));
      break;
    case 10: // Australia, Tasmania
      // start of DST  (first Sunday in October)
      $dststart = strtotime("first sunday UTC", strtotime("1 october $year UTC"));
      // end of DST (last Sunday in March)
      $dstend = strtotime("last sunday UTC", strtotime("1 april $year UTC"));
      break;
    case 11: // New Zealand
      // start of DST  (first Sunday in October)
      $dststart = strtotime("first sunday UTC", strtotime("1 october $year UTC"));
      // end of DST (first Sunday in April)
      $dstend = strtotime("first sunday UTC", strtotime("1 april $year UTC"));
      break;
    case 12: // Tonga
      // start of DST (first Sunday in November)
      $dststart = strtotime("first sunday UTC",  strtotime("1 november $year UTC"));
      // end of DST (last Sunday in January)
      $dstend = strtotime("last sunday UTC", strtotime("1 february $year UTC"));
      break;
    case 13: // EU and other European countries
      // start of DST (last Sunday in March 1 am UTC)
      $dststart = strtotime("last sunday UTC", strtotime("1 april $year UTC"));
      // end of DST in Europe (last Sunday in October 1 am UTC)
      $dstend = strtotime("last sunday UTC", strtotime("1 november $year UTC"));
      break;
    case 14: // Russian Federation
      // start of DST (last Sunday in March 2 am local time)
      $dststart = strtotime("last sunday UTC", strtotime("1 april $year UTC"));
      // end of DST (last Sunday in October 2 am local time)
      $dstend = strtotime("last sunday UTC", strtotime("1 november $year UTC"));
      break;
    case 15: // Northern America (where applicable)
      // start of DST  (where applicable) (first Sunday in April before 2007,
      // after that second Sunday in March, 2 am local time)
      if ($year < 2007) {
        $dststart = strtotime("first sunday UTC", strtotime("1 april $year UTC"));
      }
      else {
        $dststart = strtotime("second sunday UTC", strtotime("1 march $year UTC"));
      }
      // end of DST (where applicable) (last Sunday in October 2 am local time)
      if ($year < 2007) {
        $dstend = strtotime("last sunday UTC", strtotime("1 november $year UTC"));
      }
      else {
        $dstend = strtotime("first sunday UTC", strtotime("1 november $year UTC"));
      }
      break;
    case 16: // Cuba
      // start of DST  (April 1st)
      $dststart = strtotime("1 april $year UTC");
      // end of DST (last Sunday in October)
      $dstend = strtotime("last sunday UTC", strtotime("1 november $year UTC"));
      break;
    case 17: // Brazil
      // start of DST  (first Sunday in November)
      $dststart = strtotime("first sunday UTC", strtotime("1 november $year UTC"));
      // end of DST (third Sunday in February)
      $dstend = strtotime("third sunday UTC", strtotime("1 february $year UTC"));
      break;
    case 18: // Chile
      // start of DST  (Second Saturday of October - at midnight)
      $dststart = strtotime("second saturday UTC", strtotime("1 october $year UTC"));
      // end of DST (Second Saturday of March - at midnight)
      $dstend = strtotime("second sunday UTC", strtotime("1 march $year UTC"));
      break;
    case 19: // Falklands
      // start of DST  (First Sunday on or after 8 September)
      $dststart = strtotime("next sunday UTC", strtotime("7 september $year UTC"));
      // end of DST (First Sunday on or after 6 April)
      $dstend = strtotime("next sunday UTC", strtotime("5 april $year UTC"));
      break;
    case 20: // Paraguay
      // start of DST  (first Sunday in September)
      $dststart = strtotime("first sunday UTC", strtotime("1 september $year UTC"));
      // end of DST (first Sunday in April)
      $dstend = strtotime("first sunday UTC", strtotime("1 april $year UTC"));
      break;
  }
  // Have to use reverse logic in southern hemisphere.
  $southern = array(9, 10, 11, 12, 17, 18, 19, 20);
  if (in_array($region, $southern)) {
    return !($timestamp <= $dststart && $dstend <= $timestamp);
  }
  else {
    return ($dststart <= $timestamp && $timestamp <= $dstend);
  }
}

/**
 * @ingroup adodb
 * @{
 */
/**
 * Functions to handle mktime(), date(), and getdate() for dates outside the
 * normal range. Uses native php date functions when possible, alterate
 * methods when native functions won't work.
 *
 * Without these functions date(), mktime(), and getdate() won't work on
 * dates prior to 1970 in PHP 4 or on dates prior to 1901 in PHP 5.
 *
 * Replace native php functions:
 *   getdate() with date_getdate()
 *   date() with date_date()
 *   gmdate() with date_gmdate()
 *   mktime() with date_mktime()
 *   gmmktime() with gmdate_mktime()
 *
 * Note: Dates earlier than 1582 need a Gregorian correction applied, which
 * works correctly if only applied once. If the same early date is run through
 * these functions more than once, the Gregorian correction may get duplicated
 * or undone and not work correctly.
 *
 * The solution in PHP 5 is to use date_create() and date_format() for very old
 * dates, which will correctly handle that adjustment. There is no foolproof
 * workaround for PHP 4. PHP 5 date_create() and date_format() functions
 * will also handle dates earlier than 100.
 *
 * The following functions were derived from code obtained from
 * http://phplens.com/phpeverywhere/adodb_date_library, licensed as follows:
 *
 * COPYRIGHT(c) 2003-2005 John Lim
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted under the terms of the BSD License.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
*/

/**
 * Returns an array with date info.
 *
 * @param $timestamp
 *   A unix timestamp.
 * @param $timezone name
 *   Use 'UTC' to avoid timezone conversion.
 */
function date_getdate($timestamp = false, $timezone = false) {
  require_once('./'. drupal_get_path('module', 'date_php4') .'/date_php4_lib.inc');
  return _date_getdate($timestamp, $timezone);
}

/**
 * Like date_getdate with no GMT conversion.
 */
function date_gmgetdate($timestamp = false) {
  if (!$timestamp) return array();
  $array    = date_getdate($timestamp, 'UTC');
  $array[0] = $timestamp;
  return $array;
}

/**
 * Return formatted date based on timestamp $timestamp.
 *
 * @param $format
 *   the format to be used for the returned timestamp
 * @param $timestamp
 *   a unix timestamp
 * @param $timezone name
 *   Use 'UTC' to avoid timezone conversion.
 */
function date_date($format, $timestamp = false, $timezone = false) {
  // date($format) will adjust to the server timezone, which may
  // not be right, but we don't use it in this module anywhere and 
  // there is no easy way to get it working right without creating 
  // circular code.
  if ($timestamp === false) return ($is_gmt)? @gmdate($format): @date($format);
  if ($timezone === FALSE) {
    $timezone = date_default_timezone_name();
  }
  // Anything with a timezone other than UTC needs to be forced to the
  // low level function to compute the timezone offset correctly.
  $is_gmt = $timezone == 'UTC' ? TRUE : FALSE;
  if (abs($timestamp) <= 0x7FFFFFFF) {
    // Below PHP 5.2 in windows, must be positive integer
    if ($timestamp >= 0 && $is_gmt) {
      return @gmdate($format, $timestamp);
    }
    elseif ($timestamp >= 0 && variable_get('date_use_server_zone', FALSE)) {
      return @date($format, $timestamp);
    }
  }
  require_once('./'. drupal_get_path('module', 'date_php4') .'/date_php4_lib.inc');
  return _date_date($format, $timestamp, $timezone);
}

/**
 * Like date_date with no GMT conversion.
 */
function date_gmdate($format, $timestamp = false, $test = false) {
  return date_date($format, $timestamp, 'UTC', $test);
}

/**
 * Return a timestamp given a local time.
 * 
 * @param $timezone name
 *   Use 'UTC' to avoid timezone conversion.
 *
 * Force anything that will require timezone conversion to use
 * lower level functions to make sure we adjust to the right timezone
 * (native 'date' functions use the server timezone).
 */
function date_mktime($hr, $min, $sec, $mon = false, $day = false, $year = false, $timezone = false) {
  if ($timezone === FALSE) {
    $timezone = date_default_timezone_name();
  }
  $is_gmt = $timezone == 'UTC' ? TRUE : FALSE;
  $hr = intval($hr);
  $min = intval($min);
  $sec = intval($sec);
  if ($mon === false && $is_gmt) {
    return @gmmktime($hr, $min, $sec);
  }
  $mon = intval($mon) > 0 ? intval($mon) : date('n');
  $day = intval($day) > 0 ? intval($day) : date('j');
  $year = intval($year) > 0 ? intval($year) : date('Y');
  
  // Don't use native handling for any values that could create negative
  // timestamp, Windows does not support them at all in PHP 4, and
  // the adodb date library notes that since RedHat 7.3, negative 
  // timestamps are not supported in glibc either.
  // Step in one year on either side of allowable 1970-2038 window
  // to be sure timezone adjustments and month and day additions
  // won't push our date outside this window.
  if (1971 < $year && $year < 2037 && $is_gmt) {
    return @gmmktime($hr, $min, $sec, $mon, $day, $year);
  }
  
  require_once('./'. drupal_get_path('module', 'date_php4') .'/date_php4_lib.inc');
  return _date_mktime($hr, $min, $sec, $mon, $day, $year, $timezone);
}

/**
 * Like date_mktime with no GMT conversion.
*/
function date_gmmktime($hr, $min, $sec, $mon = false, $day = false, $year = false) {
  return date_mktime($hr, $min, $sec, $mon, $day, $year, 'UTC');
}

/**
 * Get local time zone offset from GMT.
 * 
 * @return timezone offset in seconds
 */
function date_get_gmt_diff_ts($timestamp, $timezone = NULL) {
  if ($timezone == 'UTC') {
    return 0;
  }
  elseif (empty($timezone)) {
    $timezone = date_default_timezone_name();
  }
  $date = new StdClass();
  $date->timestamp = $timestamp;
  $date->timezone = $timezone;
  return date_offset_get($date);
}


/**
 * Fix 2-digit years. Works for any century. Assumes that if 2-digit is
 * more than 30 years in future, then previous century.
*/
function date_year_digit_check($y) {
  if ($y < 100) {
    $yr = (integer) date("Y");
    $century = (integer) ($yr /100);
    if ($yr%100 > 50) {
      $c1 = $century + 1;
      $c0 = $century;
    } else {
      $c1 = $century;
      $c0 = $century - 1;
    }
    $c1 *= 100;
    // if 2-digit year is less than 30 years in future, set it to this century
    // otherwise if more than 30 years in future, then we set 2-digit year to the prev century.
    if (($y + $c1) < $yr + 30) $y = $y + $c1;
    else $y = $y + $c0 * 100;
  }
  return $y;
}

/**
 * Returns day of week for given date (0 = Sunday), works on dates outside
 * normal date range.
 *
 * Adapted from a function in the ADODB date library by John Lim, more info in
 * date_php4_lib.inc.
 *
 * @param int  $day
 * @param int  $month
 * @param int  $year
 * @return
 *    the number of the day in the week
 */
function date_dow($day, $month, $year) {
  // Gregorian correction from ADODB
  if ($year <= 1582) {
    if ($year < 1582 ||
      ($year == 1582 && ($month < 10 || ($month == 10 && $day < 15)))) $greg_correction = 3;
     else
      $greg_correction = 0;
  } else
    $greg_correction = 0;
  if($month > 2)
      $month -= 2;
  else {
      $month += 10;
      $year--;
  }
  $day = (floor((13 * $month - 1) / 5) +
    $day + ($year % 100) +
    floor(($year % 100) / 4) +
    floor(($year / 100) / 4) - 2 *
    floor($year / 100) + 77 + $greg_correction);
  $weekday_number = $day - 7 * floor($day / 7);
  return $weekday_number;
}

/**
 * Returns true for a leap year, else false. Works outside normal date range.
 *
 * @param int  $year
 * @return boolean
 */
function date_is_leap_year($year) {
  if ($year < 1000) {
    return false;
  }
  if ($year < 1582) { // pre Gregorio XIII - 1582
    return ($year % 4 == 0);
  }
  else {  // post Gregorio XIII - 1582
    return (($year % 4 == 0) && ($year % 100 != 0)) || ($year % 400 == 0);
  }
}

/**
 * @} End of ingroup "datelib".
 */