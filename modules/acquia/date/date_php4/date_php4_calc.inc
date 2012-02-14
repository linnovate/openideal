<?php
// $Id: date_php4_calc.inc,v 1.4.4.2 2008/08/30 13:57:39 karens Exp $
/**
 * Calculates and manipulates dates with no reliance on 32-bit system
 * timestamps, to work on dates before 1970 and after 2038.
 *
 * This file must be included whenever these functions are used, it is
 * not automatically available.
 *
 * The functions below were derived from code obtained from
 * http://pear.php.net/package/Date/Calc.php, licensed as follows:
 *
 * Copyright (c) 1999-2006 Monte Ohrt, Pierre-Alain Joye, Daniel Convissor
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
define('DATE_CALC_FORMAT', '%Y-%m-%d');

// Some functions from the low level library are needed here, make sure
// they are available.
require_once('./'. drupal_get_path('module', 'date_php4') .'/date_php4_lib.inc');

/**
 * The default value for each method's $format parameter
 *
 * The default is '%Y%m%d'.  To override this default, define
 * this constant before including Calc.php.
 *
 * @since Constant available since Release 1.4.4
 */
/**
 * Formats the date in the given format, much like strfmt()
 *
 * This function is used to alleviate the problem with 32-bit numbers for
 * dates pre 1970 or post 2038, as strfmt() has on most systems.
 * Most of the formatting options are compatible.
 *
 * Formatting options:
 * <pre>
 * %a   abbreviated weekday name (Sun, Mon, Tue)
 * %A   full weekday name (Sunday, Monday, Tuesday)
 * %b   abbreviated month name (Jan, Feb, Mar)
 * %B   full month name (January, February, March)
 * %d   day of month (range 00 to 31)
 * %e   day of month, single digit (range 0 to 31)
 * %E   number of days since unspecified epoch (integer)
 *    (%E is useful for passing a date in a URL as
 *    an integer value. Then simply use
 *    date_calc_days_to_date() to convert back to a date.)
 * %j   day of year (range 001 to 366)
 * %m   month as decimal number (range 1 to 12)
 * %n   newline character (\n)
 * %t   tab character (\t)
 * %w   weekday as decimal (0 = Sunday)
 * %U   week number of current year, first sunday as first week
 * %y   year as decimal (range 00 to 99)
 * %Y   year as decimal including century (range 0000 to 9999)
 * %%   literal '%'
 * </pre>
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The format string.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_format($day, $month, $year, $format = DATE_CALC_FORMAT) {
  if (!date_calc_is_valid($day, $month, $year)) {
    $year  = date_calc_date_now('%Y');
    $month = date_calc_date_now('%m');
    $day   = date_calc_date_now('%d');
  }
  $output = '';
  for ($strpos = 0; $strpos < strlen($format); $strpos++) {
    $char = substr($format, $strpos, 1);
    if ($char == '%') {
      $nextchar = substr($format, $strpos + 1, 1);
      switch ($nextchar) {
        case 'a':
          $output .= date_calc_get_weekday_abbrname($day, $month, $year);
          break;
        case 'A':
          $output .= date_calc_get_weekday_fullname($day, $month, $year);
          break;
        case 'b':
          $output .= date_calc_get_month_abbrname($month);
          break;
        case 'B':
          $output .= date_calc_get_month_fullname($month);
          break;
        case 'd':
          $output .= date_pad($day);
          break;
        case 'e':
          $output .= $day;
          break;
        case 'E':
          $output .= date_calc_date_to_days($day, $month, $year);
          break;
        case 'j':
          $output .= date_calc_julian_date($day, $month, $year);
          break;
        case 'm':
          $output .= date_pad($month);
          break;
        case 'n':
          $output .= "\n";
          break;
        case 't':
          $output .= "\t";
          break;
        case 'w':
          $output .= date_dow($day, $month, $year);
          break;
        case 'U':
          $output .= date_calc_week_of_year($day, $month, $year);
          break;
        case 'y':
          $output .= substr(date_pad($year, 4), 2, 2);
          break;
        case 'Y':
          $output .= date_pad($year, 4);
          break;
        case '%':
          $output .= '%';
          break;
        default:
          $output .= $char . $nextchar;
      }
      $strpos++;
    }
    else {
      $output .= $char;
    }
  }
  return $output;
}

/**
 * Returns true for valid date, false for invalid date.
 * Renamed this function because we already have a similar function that
 * expects a full date as a parameter.
 *
 * @param int  $day
 *   the day of the month
 * @param int  $month
 *   the month
 * @param int  $year
 *   the year, using 2005, not 05. Do not add leading 0's for years prior to 1000.
 *
 * @return boolean
 */
function date_calc_is_valid($day, $month, $year) {
  if ($year > variable_get('date_max_year', 4000) || $year < variable_get('date_min_year', 1)) {
    return false;
  }
  if (!checkdate($month, $day, $year)) {
    // Checkdate won't work on very old dates in PHP 4, need more testing.
    if (!date_gmmktime(0, 0, 0, $month, $day, $year)) {
      return false;
    }
  }
  return true;
}

/**
 * Converts a date to number of days since a distant unspecified epoch
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return integer
 *   The number of days since the date_calc epoch.
 */
function date_calc_date_to_days($day, $month, $year) {
  $century = (int)substr($year, 0, 2);
  $year = (int)substr($year, 2, 2);
  if ($month > 2) {
    $month -= 3;
  }
  else {
    $month += 9;
    if ($year) {
      $year--;
    }
    else {
      $year = 99;
      $century --;
    }
  }
  return (floor((146097 * $century) / 4 ) +
    floor((1461 * $year) / 4 ) +
    floor((153 * $month + 2) / 5 ) +
    $day + 1721119);
}

/**
 * Converts number of days to a distant unspecified epoch
 *
 * @param int  $days
 *   The number of days since the date_calc epoch.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_days_to_date($days, $format = DATE_CALC_FORMAT) {
  $days   -= 1721119;
  $century = floor((4 * $days - 1) / 146097);
  $days  = floor(4 * $days - 1 - 146097 * $century);
  $day   = floor($days / 4);

  $year  = floor((4 * $day +  3) / 1461);
  $day   = floor(4 * $day +  3 - 1461 * $year);
  $day   = floor(($day +  4) / 4);

  $month   = floor((5 * $day - 3) / 153);
  $day   = floor(5 * $day - 3 - 153 * $month);
  $day   = floor(($day +  5) /  5);

  if ($month < 10) {
    $month +=3;
  }
  else {
    $month -=9;
    if ($year++ == 99) {
      $year = 0;
      $century++;
    }
  }
  $century = date_pad($century);
  $year  = date_pad($year);
  return date_calc_format($day, $month, $century . $year, $format);
}

/**
 * Converts from Gregorian Year-Month-Day to ISO Year-WeekNumber-WeekDay
 *
 * Uses ISO 8601 definitions.  Algorithm by Rick McCarty, 1999 at
 * http://personal.ecu.edu/mccartyr/ISOwdALG.txt .
 * Transcribed to PHP by Jesus M. Castagnetto.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return string
 *   The date in ISO Year-WeekNumber-WeekDay format.
 */
function date_calc_gregorian_to_ISO($day, $month, $year) {
  $mnth = array(0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334);
  $y_isleap = date_is_leap_year($year);
  $y_1_isleap = date_is_leap_year($year - 1);
  $day_of_year_number = $day + $mnth[$month - 1];
  if ($y_isleap && $month > 2) {
    $day_of_year_number++;
  }
  // find Jan 1 weekday (monday = 1, sunday = 7)
  $yy = ($year - 1) % 100;
  $c = ($year - 1) - $yy;
  $g = $yy + intval($yy / 4);
  $jan1_weekday = 1 + intval((((($c / 100) % 4) * 5) + $g) % 7);
  // weekday for year-month-day
  $h = $day_of_year_number + ($jan1_weekday - 1);
  $weekday = 1 + intval(($h - 1) % 7);
  // find if Y M D falls in YearNumber Y-1, WeekNumber 52 or
  if ($day_of_year_number <= (8 - $jan1_weekday) && $jan1_weekday > 4) {
    $yearnumber = $year - 1;
    if ($jan1_weekday == 5 || ($jan1_weekday == 6 && $y_1_isleap)) {
      $weeknumber = 53;
    }
    else {
      $weeknumber = 52;
    }
  }
  else {
    $yearnumber = $year;
  }
  // find if Y M D falls in YearNumber Y+1, WeekNumber 1
  if ($yearnumber == $year) {
    if ($y_isleap) {
      $i = 366;
    }
    else {
      $i = 365;
    }
    if (($i - $day_of_year_number) < (4 - $weekday)) {
      $yearnumber++;
      $weeknumber = 1;
    }
  }
  // find if Y M D falls in YearNumber Y, WeekNumber 1 through 53
  if ($yearnumber == $year) {
    $j = $day_of_year_number + (7 - $weekday) + ($jan1_weekday - 1);
    $weeknumber = intval($j / 7);
    if ($jan1_weekday > 4) {
      $weeknumber--;
    }
  }
  // put it all together
  if ($weeknumber < 10) {
    $weeknumber = '0'. $weeknumber;
  }
  return $yearnumber .'-'. $weeknumber .'-'. $weekday;
}

/**
 * Determines julian date of the given season
 *
 * Adapted from previous work in Java by James Mark Hamilton.
 *
 * @param string $season
 *   The season to get the date for:
 *   VERNALEQUINOX, SUMMERSOLSTICE, AUTUMNALEQUINOX, or WINTERSOLSTICE.
 * @param string $year
 *   The year in four digit format.  Must be between -1000BC and 3000AD.
 *
 * @return float
 *   The julian date the season starts on.
 *
 * @author James Mark Hamilton <mhamilton@qwest.net>
 * @author Robert Butler <rob@maxwellcreek.org>
 */
function date_calc_date_season($season, $year = 0) {
  if ($year == '') {
    $year = date_calc_get_year();
  }
  if (($year >= -1000) && ($year <= 1000)) {
    $y = $year / 1000.0;
    switch ($season) {
      case 'VERNALEQUINOX':
        $date_calc_julian_date = (((((((-0.00071 * $y) - 0.00111) * $y) + 0.06134) * $y) + 365242.1374) * $y) + 1721139.29189;
        break;
      case 'SUMMERSOLSTICE':
        $date_calc_julian_date = (((((((0.00025 * $y) + 0.00907) * $y) - 0.05323) * $y) + 365241.72562) * $y) + 1721233.25401;
        break;
      case 'AUTUMNALEQUINOX':
        $date_calc_julian_date = (((((((0.00074 * $y) - 0.00297) * $y) - 0.11677) * $y) + 365242.49558) * $y) + 1721325.70455;
        break;
      case 'WINTERSOLSTICE':
      default:
        $date_calc_julian_date = (((((((-0.00006 * $y) - 0.00933) * $y) - 0.00769) * $y) + 365242.88257) * $y) + 1721414.39987;
    }
  }
  elseif (($year > 1000) && ($year <= 3000)) {
    $y = ($year - 2000) / 1000;
    switch ($season) {
      case 'VERNALEQUINOX':
        $date_calc_julian_date = (((((((-0.00057 * $y) - 0.00411) * $y) + 0.05169) * $y) + 365242.37404) * $y) + 2451623.80984;
        break;
      case 'SUMMERSOLSTICE':
        $date_calc_julian_date = (((((((-0.0003 * $y) + 0.00888) * $y) + 0.00325) * $y) + 365241.62603) * $y) + 2451716.56767;
        break;
      case 'AUTUMNALEQUINOX':
        $date_calc_julian_date = (((((((0.00078 * $y) + 0.00337) * $y) - 0.11575) * $y) + 365242.01767) * $y) + 2451810.21715;
        break;
      case 'WINTERSOLSTICE':
      default:
        $date_calc_julian_date = (((((((0.00032 * $y) - 0.00823) * $y) - 0.06223) * $y) + 365242.74049) * $y) + 2451900.05952;
    }
  }
  return $date_calc_julian_date;
}

/**
 * Returns the current local date.
 *
 * NOTE: This function retrieves the local date using strftime(),
 * which may or may not be 32-bit safe on your system.
 *
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The current date in the specified format.
 */
function date_calc_date_now($format = DATE_CALC_FORMAT) {
  return strftime($format, time());
}

/**
 * Returns the current local year in format CCYY.
 *
 * @param  int $days
 *   An integer calculated by date_calc_date_to_days().
 *
 * @return string
 *   The current year in four digit format.
 */
function date_calc_get_year() {
  return date_calc_date_now('%Y');
}

/**
 * Returns the current local month in format MM
 *
 * @param  int $days
 *   An integer calculated by date_calc_date_to_days().
 *
 * @return string
 *   The current month in two digit format.
 */
function date_calc_get_month() {
  return date_calc_date_now('%m');
}

/**
 * Returns the current local day in format DD
 *
 * @param  int $days
 *   An integer calculated by date_calc_date_to_days().
 *
 * @return string
 *   The current day of the month in two digit format.
 */
function date_calc_get_day() {
  return date_calc_date_now('%d');
}

/**
 * Returns number of days since 31 December of year before given date.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return int
 *   The julian date for the date.
 */
function date_calc_julian_date($day = 0, $month = 0, $year = 0) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $days = array(0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334);
  $julian = ($days[$month - 1] + $day);
  if ($month > 2 && date_is_leap_year($year)) {
    $julian++;
  }
  return $julian;
}

/**
 * Returns the full weekday name for the given date
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return string
 *   The full name of the day of the week/
 */
function date_calc_get_weekday_fullname($day = 0, $month = 0, $year = 0) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $weekday_names = date_week_days();
  $weekday = date_dow($day, $month, $year);
  return $weekday_names[$weekday];
}

/**
 * Returns the abbreviated weekday name for the given date.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The year.  Use the complete year instead of the abbreviated version.
 *   E.g. use 2005, not 05. Do not add leading 0's for years prior to 1000.
 * @param int  $length
 *   The length of abbreviation.
 *
 * @return string
 *   The abbreviated name of the day of the week.
 */
function date_calc_get_weekday_abbrname($day = 0, $month = 0, $year = 0) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $weekday_names = date_week_days_abbr();
  $weekday = date_dow($day, $month, $year);
  return $weekday_names[$weekday];
}

/**
 * Returns the full month name for the given month.
 *
 * @param int  $month
 *   The month.
 *
 * @return string
 *   The full name of the month.
 */
function date_calc_get_month_fullname($month) {
  $month = (int)$month;
  if (empty($month)) {
    $month = (int)date_calc_get_month();
  }
  $month_names = date_month_names();
  return $month_names[$month];
}

/**
 * Returns the abbreviated month name for the given month.
 *
 * @param int  $month
 *   The month.
 * @param int  $length
 *   The length of abbreviation.
 *
 * @return string
 *   The abbreviated name of the month.
 */
function date_calc_get_month_abbrname($month, $length = 3) {
  $month = (int)$month;
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  return drupal_substr(date_calc_get_month_fullname($month), 0, $length);
}

/**
 * Returns the numeric month from the month name or an abreviation
 * Both August and Aug would return 8.
 *
 * @param string $month
 *   The name of the month to examine. Case insensitive.
 *
 * @return integer
 *   The month's number.
 */
function date_calc_get_month_from_fullname($month) {
  $month = strtolower($month);
  $months = date_month_names();
  while (list($id, $name) = each($months)) {
    if (ereg($month, strtolower($name))) {
      return $id;
    }
  }
  return 0;
}

/**
 * Returns week of the year, first Sunday is first day of first week.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return int
 *   The number of the week in the year.
 */
function date_calc_week_of_year($day = 0, $month = 0, $year = 0) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $iso  = date_calc_gregorian_to_ISO($day, $month, $year);
  $parts  = explode('-', $iso);
  $week_number = intval($parts[1]);
  return $week_number;
}

/**
 * Returns quarter of the year for given date.
 *
  * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *
 * @return int
 *   The number of the quarter in the year.
 */
function date_calc_quarter_of_year($day = 0, $month = 0, $year = 0) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $year_quarter = intval(($month - 1) / 3 + 1);
  return $year_quarter;
}

/**
 * Find the number of days in the given month.
 *
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return int
 *   The number of days the month has.
 */
function date_calc_days_in_month($month = 0, $year = 0) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if ($year == 1582 && $month == 10) {
    return 21;  // October 1582 only had 1st-4th and 15th-31st
  }
  if ($month == 2) {
    if (date_is_leap_year($year)) {
      return 29;
     }
     else {
      return 28;
    }
  }
  elseif ($month == 4 or $month == 6 or $month == 9 or $month == 11) {
    return 30;
  }
  else {
    return 31;
  }
}

/**
 * Returns the number of rows on a calendar month. Useful for determining
 * the number of rows when displaying a typical month calendar.
 *
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return int
 *   The number of weeks the month has.
 */
function date_calc_weeks_in_month($month = 0, $year = 0) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  $FDOM = date_calc_first_of_month_weekday($month, $year);
  if (variable_get('date_first_day', 1)==1 && $FDOM==0) {
    $first_week_days = 7 - $FDOM + variable_get('date_first_day', 1);
    $weeks = 1;
  }
  elseif (variable_get('date_first_day', 1)==0 && $FDOM == 6) {
    $first_week_days = 7 - $FDOM + variable_get('date_first_day', 1);
    $weeks = 1;
  }
  else {
    $first_week_days = variable_get('date_first_day', 1) - $FDOM;
    $weeks = 0;
  }
  $first_week_days %= 7;
  return ceil((date_calc_days_in_month($month, $year)
    - $first_week_days) / 7) + $weeks;
}

/**
 * Return an array with days in week.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return array
 *   $week[$weekday].
 */
function date_calc_get_calendar_week($day = 0, $month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $week_array = array();
  // date for the column of week
  $curr_day = date_calc_begin_of_week($day, $month, $year, '%E');
  for ($counter = 0; $counter <= 6; $counter++) {
    $week_array[$counter] = date_calc_days_to_date($curr_day, $format);
    $curr_day++;
  }
  return $week_array;
}

/**
 * Return a set of arrays to construct a calendar month for the given date.
 *
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return array
 *   $month[$row][$col].
 */
function date_calc_get_calendar_month($month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  $month_array = array();
  // date for the first row, first column of calendar month
  if (variable_get('date_first_day', 1) == 1) {
    if (date_calc_first_of_month_weekday($month, $year) == 0) {
      $curr_day = date_calc_date_to_days('01', $month, $year) - 6;
    }
    else {
      $curr_day = date_calc_date_to_days('01', $month, $year)
        - date_calc_first_of_month_weekday($month, $year) + 1;
    }
  }
  else {
    $curr_day = (date_calc_date_to_days('01', $month, $year)
      - date_calc_first_of_month_weekday($month, $year));
  }
  // number of days in this month
  $date_calc_days_in_month = date_calc_days_in_month($month, $year);
  $date_calc_weeks_in_month = date_calc_weeks_in_month($month, $year);
  for ($row_counter = 0; $row_counter < $date_calc_weeks_in_month; $row_counter++) {
    for ($column_counter = 0; $column_counter <= 6; $column_counter++) {
      $month_array[$row_counter][$column_counter] =
        date_calc_days_to_date($curr_day , $format);
      $curr_day++;
    }
  }
  return $month_array;
}

/**
 * Return a set of arrays to construct a calendar year for the given date
 *
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return array $year[$month][$row][$col]
 */
function date_calc_get_calendar_year($year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  $year_array = array();
  for ($curr_month = 0; $curr_month <= 11; $curr_month++) {
    $year_array[$curr_month] =
      date_calc_get_calendar_month($curr_month + 1,
        $year, $format);
  }
  return $year_array;
}

/**
 * Returns date of day before given date.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_prev_day($day = 0, $month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $days = date_calc_date_to_days($day, $month, $year);
  return date_calc_days_to_date($days - 1, $format);
}

/**
 * Returns date of day after given date
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_next_day($day = 0, $month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $days = date_calc_date_to_days($day, $month, $year);
  return date_calc_days_to_date($days + 1, $format);
}

/**
 * Returns date of the previous weekday, skipping from Monday to Friday
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_prev_weekday($day = 0, $month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $days = date_calc_date_to_days($day, $month, $year);
  if (date_dow($day, $month, $year) == 1) {
    $days -= 3;
  }
  elseif (date_dow($day, $month, $year) == 0) {
    $days -= 2;
  }
  else {
    $days -= 1;
  }
  return date_calc_days_to_date($days, $format);
}

/**
 * Returns date of the next weekday of given date, skipping from
 * Friday to Monday.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_next_weekday($day = 0, $month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $days = date_calc_date_to_days($day, $month, $year);
  if (date_dow($day, $month, $year) == 5) {
    $days += 3;
  }
  elseif (date_dow($day, $month, $year) == 6) {
    $days += 2;
  }
  else {
    $days += 1;
  }
  return date_calc_days_to_date($days, $format);
}

/**
 * Returns date of the previous specific day of the week
 * from the given date.
 *
 * @param int
 *   Day of week, 0=Sunday.
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param bool   $on_or_before
 *   If true and days are same, returns current day.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_prev_day_of_week($dow, $day = 0, $month = 0, $year = 0,
  $format = DATE_CALC_FORMAT, $on_or_before = false) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $days = date_calc_date_to_days($day, $month, $year);
  $curr_weekday = date_dow($day, $month, $year);
  if ($curr_weekday == $dow) {
    if (!$on_or_before) {
      $days -= 7;
    }
  }
  elseif ($curr_weekday < $dow) {
    $days -= 7 - ($dow - $curr_weekday);
  }
  else {
    $days -= $curr_weekday - $dow;
  }
  return date_calc_days_to_date($days, $format);
}

/**
 * Returns date of the next specific day of the week
 * from the given date.
 *
 * @param int  $dow
 *   The day of the week (0 = Sunday).
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param bool   $on_or_after
 *   If true and days are same, returns current day.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string  the date in the desired format
 */
function date_calc_next_day_of_week($dow, $day = 0, $month = 0, $year = 0,
  $format = DATE_CALC_FORMAT, $on_or_after = false) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $days = date_calc_date_to_days($day, $month, $year);
  $curr_weekday = date_dow($day, $month, $year);
  if ($curr_weekday == $dow) {
    if (!$on_or_after) {
      $days += 7;
    }
  }
  elseif ($curr_weekday > $dow) {
    $days += 7 - ($curr_weekday - $dow);
  }
  else {
    $days += $dow - $curr_weekday;
  }
  return date_calc_days_to_date($days, $format);
}

/**
 * Find the month day of the beginning of week for given date,
 * using variable_get('date_first_day', 1). Can return weekday of prev month.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_begin_of_week($day = 0, $month = 0, $year = 0,
  $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $this_weekday = date_dow($day, $month, $year);
  $interval = (7 - variable_get('date_first_day', 1) + $this_weekday) % 7;
  return date_calc_days_to_date(date_calc_date_to_days($day, $month, $year)
    - $interval, $format);
}

/**
 * Find the month day of the end of week for given date, using
 * variable_get('date_first_day', 1). Can return weekday of following month.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_end_of_week($day = 0, $month = 0, $year = 0,
  $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $this_weekday = date_dow($day, $month, $year);
  $interval = (6 + variable_get('date_first_day', 1) - $this_weekday) % 7;
  return date_calc_days_to_date(date_calc_date_to_days($day, $month, $year)
    + $interval, $format);
}

/**
 * Find the month day of the beginning of week before given date, using
 * variable_get('date_first_day', 1). Can return weekday of prev month.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_begin_of_prev_week($day = 0, $month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $date = date_calc_days_to_date(date_calc_date_to_days($day-7, $month, $year), '%Y%m%d');
  $prev_week_year  = intval(substr($date, 0, 4));
  $prev_week_month = intval(substr($date, 4, 2));
  $prev_week_day   = intval(substr($date, 6, 2));
  return date_calc_begin_of_week($prev_week_day, $prev_week_month, $prev_week_year, $format);
}

/**
 * Find the month day of the beginning of week after given date, using
 * variable_get('date_first_day', 1). Can return weekday of next month.
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string  the date in the desired format
 */
function date_calc_begin_of_next_week($day = 0, $month = 0, $year = 0,
  $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if (empty($day)) {
    $day = date_calc_get_day();
  }
  $date = date_calc_days_to_date(date_calc_date_to_days($day + 7, $month, $year), '%Y%m%d');
  $next_week_year  = intval(substr($date, 0, 4));
  $next_week_month = intval(substr($date, 4, 2));
  $next_week_day   = intval(substr($date, 6, 2));
  return date_calc_begin_of_week($next_week_day, $next_week_month, $next_week_year, $format);
}

/**
 * Returns date of the first day of the month in the number of months
 * from the given date
 *
 * @param int  $months
 *   The number of months from the date provided.
 *   Positive numbers go into the future.
 *   Negative numbers go into the past.
 *   0 is the month presented in $month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_begin_of_month_by_span($months = 0, $month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if ($months > 0) {
    // future month
    $tmp_mo = $month + $months;
    $month  = $tmp_mo % 12;
    if ($month == 0) {
      $month = 12;
      $year = $year + floor(($tmp_mo - 1) / 12);
    }
    else {
      $year = $year + floor($tmp_mo / 12);
    }
  }
  else {
    // past or present month
    $tmp_mo = $month + $months;
    if ($tmp_mo > 0) {
      // same year
      $month = $tmp_mo;
    }
    elseif ($tmp_mo == 0) {
      // prior dec
      $month = 12;
      $year--;
    }
    else {
      // some time in a prior year
      $month = 12 + ($tmp_mo % 12);
      $year  = $year + floor($tmp_mo / 12);
    }
  }
  return date_calc_format(1, $month, $year, $format);
}

/**
 * Returns date of the last day of the month in the number of months
 * from the given date.
 *
 * @param int  $months
 *   The number of months from the date provided.
 *   Positive numbers go into the future.
 *   Negative numbers go into the past.
 *   0 is the month presented in $month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_end_of_month_by_span($months = 0, $month = 0, $year = 0, $format = DATE_CALC_FORMAT) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  if ($months > 0) {
    // future month
    $tmp_mo = $month + $months;
    $month  = $tmp_mo % 12;
    if ($month == 0) {
      $month = 12;
      $year = $year + floor(($tmp_mo - 1) / 12);
    }
    else {
      $year = $year + floor($tmp_mo / 12);
    }
  }
  else {
    // past or present month
    $tmp_mo = $month + $months;
    if ($tmp_mo > 0) {
      // same year
      $month = $tmp_mo;
    }
    elseif ($tmp_mo == 0) {
      // prior dec
      $month = 12;
      $year--;
    }
    else {
      // some time in a prior year
      $month = 12 + ($tmp_mo % 12);
      $year  = $year + floor($tmp_mo / 12);
    }
  }
  return date_calc_format(date_calc_days_in_month($month, $year), $month, $year, $format);
}

/**
 * Find the day of the week for the first of the month of given date
 *
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return int
 *   Number of weekday for the first day, 0=Sunday.
 */
function date_calc_first_of_month_weekday($month = 0, $year = 0) {
  if (empty($year)) {
    $year = date_calc_get_year();
  }
  if (empty($month)) {
    $month = date_calc_get_month();
  }
  return date_dow('01', $month, $year);
}

/**
 * Calculates the date of the Nth weekday of the month,
 * such as the second Saturday of January 2000
 *
 * @param int  $week
 *   The number of the week to get (1 = first, etc.  Also can be 'last'.)
 * @param int  $dow
 *   The day of the week (0 = Sunday).
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param string $format
 *   The string indicating how to format the output.
 *
 * @return string
 *   The date in the desired format.
 */
function date_calc_n_weekday_of_month($week, $dow, $month, $year, $format = DATE_CALC_FORMAT) {
  if (is_numeric($week)) {
    $DOW1day = ($week - 1) * 7 + 1;
    $DOW1  = date_dow($DOW1day, $month, $year);
    $wdate   = ($week - 1) * 7 + 1 + (7 + $dow - $DOW1) % 7;
    if ($wdate > date_calc_days_in_month($month, $year)) {
      return -1;
    }
    else {
      return date_calc_format($wdate, $month, $year, $format);
    }
  }
  elseif ($week == 'last' && $dow < 7) {
    $lastday = date_calc_days_in_month($month, $year);
    $lastdow = date_dow($lastday, $month, $year);
    $diff  = $dow - $lastdow;
    if ($diff > 0) {
      return date_calc_format($lastday - (7 - $diff), $month, $year, $format);
    }
    else {
      return date_calc_format($lastday + $diff, $month, $year, $format);
    }
  }
  else {
    return -1;
  }
}

/**
 * Determines if given date is a future date from now
 *
 * @param int  $day
 *   The day of the month.
 * @param int  $month
 *   The month.
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return boolean
 */
function date_calc_is_future_date($day, $month, $year) {
  $this_year  = date_calc_get_year();
  $this_month = date_calc_get_month();
  $this_day   = date_calc_get_day();
  if ($year > $this_year) {
    return true;
  }
  elseif ($year == $this_year) {
    if ($month > $this_month) {
      return true;
    }
    elseif ($month == $this_month) {
      if ($day > $this_day) {
        return true;
      }
    }
  }
  return false;
}

/**
 * Determines if given date is a past date from now
 *
 * @param int  $day
 *   the day of the month
 * @param int  $month
 *   the month
 * @param int  $year
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return boolean
 */
function date_calc_is_past_date($day, $month, $year) {
  $this_year  = date_calc_get_year();
  $this_month = date_calc_get_month();
  $this_day   = date_calc_get_day();
  if ($year < $this_year) {
    return true;
  }
  elseif ($year == $this_year) {
    if ($month < $this_month) {
      return true;
    }
    elseif ($month == $this_month) {
      if ($day < $this_day) {
        return true;
      }
    }
  }
  return false;
}

/**
 * Returns number of days between two given dates
 *
 * @param int  $day1
 *   the day of the month
 * @param int  $month1
 *   the month
 * @param int  $year1
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param int  $day2
 *   the day of the month
 * @param int  $month2
 *   the month
 * @param int  $year2
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 *
 * @return int
 *   the absolute number of days between the two dates.
 *   If an error occurs, -1 is returned.
 */
function date_calc_date_diff($day1, $month1, $year1, $day2, $month2, $year2) {
  if (!date_calc_is_valid($day1, $month1, $year1)) {
    return -1;
  }
  if (!date_calc_is_valid($day2, $month2, $year2)) {
    return -1;
  }
  return abs(date_calc_date_to_days($day1, $month1, $year1)
    - date_calc_date_to_days($day2, $month2, $year2));
}

/**
 * Compares two dates
 *
 * @param int  $day1
 *   the day of the month
 * @param int  $month1
 *   the month
 * @param int  $year1
 *   The 4 digit year. Do not add leading 0's for years prior to 1000.
 * @param int  $day2
 *   the day of the month
 * @param int  $month2
 *   the month
 * @param int  $year2
 *   the year.  Use the complete year instead of the abbreviated version.
 *   E.g. use 2005, not 05. Do not add leading 0's for years prior to 1000.
 *
 * @return int
 *   0 if the dates are equal. 1 if date 1 is later, -1 if date 1 is earlier.
 */
function date_calc_compare_dates($day1, $month1, $year1, $day2, $month2, $year2) {
  $ndays1 = date_calc_date_to_days($day1, $month1, $year1);
  $ndays2 = date_calc_date_to_days($day2, $month2, $year2);
  if ($ndays1 == $ndays2) {
    return 0;
  }
  return ($ndays1 > $ndays2) ? 1 : -1;
}