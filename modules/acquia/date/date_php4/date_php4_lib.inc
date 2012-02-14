<?php
// $Id: date_php4_lib.inc,v 1.4.4.7 2009/02/24 17:14:06 karens Exp $

/**
 * Date Library, extended date functions.
 * File is included only when needed.
 */

/**
 * @ingroup adodb
 * @{
 */
/**
 * The following functions are low level functions that implements pre 1970
 * to post 2038 versions of native php date functions. Will handle dates from
 * the year 100 to the year 3000. Uses native php date functions when possible,
 * alterate methods when native functions won't work.
 *
 * Altered the original ADODB code to split it between the high level
 * functions which are used when pre-1970 and post-2038 dates are not needed
 * and this large file which is only parsed for dates that are out of range
 * for native php date handling.
 *
 * Replace native php functions:
 *   getdate() with date_getdate()
 *   date() with date_date()
 *   gmdate() with date_gmdate()
 *   mktime() with date_mktime()
 *   gmmktime() with gmdate_mktime()
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
 * Low-level function that returns the getdate() array for pre-1970
 * and post-2038 dates.
 *
 * We have a special$fast flag, which if set to true, will return fewer
 * array values, and is much faster as it does not calculate dow, etc.
 *
 * @param $timestamp a unix timestamp
 * @param $timezone name
 *   Use 'UTC' to avoid timezone conversion.
 */
function _date_getdate($timestamp = false, $timezone = false) {
  static $YRS;
  if ($timezone === FALSE) {
    $timezone = date_default_timezone_name();
  }

  $timestamp_in = $timestamp;
    
  $_day_power = 86400;
  $_hour_power = 3600;
  $_min_power = 60;
  if ($timestamp < -12219321600) $timestamp -= 86400*10; // if 15 Oct 1582 or earlier, gregorian correction
  $_month_table_normal = array("", 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
  $_month_table_leap = array("", 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
  $d366 = $_day_power * 366;
  $d365 = $_day_power * 365;
  if ($timestamp < 0) {
    if (empty($YRS)) $YRS = array(
      1970 => 0,
      1960 => -315619200,
      1950 => -631152000,
      1940 => -946771200,
      1930 => -1262304000,
      1920 => -1577923200,
      1910 => -1893456000,
      1900 => -2208988800,
      1890 => -2524521600,
      1880 => -2840140800,
      1870 => -3155673600,
      1860 => -3471292800,
      1850 => -3786825600,
      1840 => -4102444800,
      1830 => -4417977600,
      1820 => -4733596800,
      1810 => -5049129600,
      1800 => -5364662400,
      1790 => -5680195200,
      1780 => -5995814400,
      1770 => -6311347200,
      1760 => -6626966400,
      1750 => -6942499200,
      1740 => -7258118400,
      1730 => -7573651200,
      1720 => -7889270400,
      1710 => -8204803200,
      1700 => -8520336000,
      1690 => -8835868800,
      1680 => -9151488000,
      1670 => -9467020800,
      1660 => -9782640000,
      1650 => -10098172800,
      1640 => -10413792000,
      1630 => -10729324800,
      1620 => -11044944000,
      1610 => -11360476800,
      1600 => -11676096000);
    
    // The valid range of a 32bit signed timestamp is typically from
    // Fri, 13 Dec 1901 20:45:54 GMT to Tue, 19 Jan 2038 03:14:07 GMT
    //
    $lastsecs = 0;
    $lastyear = 1970;
    foreach ($YRS as $year => $secs) {
      if ($timestamp >= $secs) {
        $a = $lastyear;
        break;
      }
      $lastsecs = $secs;
      $lastyear = $year;
    }
    $timestamp -= $lastsecs;
    if (!isset($a)) $a = $lastyear;
    for (; --$a >= 0;) {
      $lastd = $timestamp;
      if ($leap = date_is_leap_year($a)) $timestamp += $d366;
      else $timestamp += $d365;

      if ($timestamp >= 0) {
        $year = $a;
        break;
      }
    }
    $secs_in_year = 86400 * ($leap ? 366 : 365) + $lastd;
    $timestamp = $lastd;
    $mtab = ($leap) ? $_month_table_leap : $_month_table_normal;
    for ($a = 13 ; --$a > 0;) {
      $lastd = $timestamp;
      $timestamp += $mtab[$a] * $_day_power;
      if ($timestamp >= 0) {
        $month = $a;
        $ndays = $mtab[$a];
        break;
      }
    }
    $timestamp = $lastd;
    $day = $ndays + ceil(($timestamp+1) / ($_day_power));
    $timestamp += ($ndays - $day+1)* $_day_power;
    $hour = floor($timestamp/$_hour_power);
  }
  else {
    
    if ($timezone != 'UTC') {
      $timestamp += date_get_gmt_diff_ts($timestamp, $timezone);
    }
    
    for ($a = 1970 ;; $a++) {
      $lastd = $timestamp;
      if ($leap = date_is_leap_year($a)) $timestamp -= $d366;
      else $timestamp -= $d365;
      if ($timestamp < 0) {
        $year = $a;
        break;
      }
    }
    $secs_in_year = $lastd;
    $timestamp = $lastd;
    $mtab = ($leap) ? $_month_table_leap : $_month_table_normal;
    for ($a = 1 ; $a <= 12; $a++) {
      $lastd = $timestamp;
      $timestamp -= $mtab[$a] * $_day_power;
      if ($timestamp < 0) {
        $month = $a;
        $ndays = $mtab[$a];
        break;
      }
    }
    $timestamp = $lastd;
    $day = ceil(($timestamp + 1) / $_day_power);
    $timestamp = $timestamp - ($day - 1) * $_day_power;
    $hour = floor($timestamp / $_hour_power);
  }
  $timestamp -= $hour * $_hour_power;
  $min = floor($timestamp / $_min_power);
  $secs = $timestamp - $min * $_min_power;
  $dow = date_dow($day, $month, $year);
  return array(
    'second' => $secs,
    'minute' => $min,
    'hour' => $hour,
    'day' => $day,
    'wday' => $dow,
    'month' => $month,
    'year' => $year,
    'yday' => floor($secs_in_year / $_day_power),
    'weekday' => gmdate('l', ($_day_power * (3 + $dow))),
    'leap' => $leap,
    'ndays' => $ndays,
    'month_name' => gmdate('F', mktime(0, 0, 0, $month, 2, 1971)),
    0 => $timestamp_in
  );
}

/**
 * Low level function to create date() for pre-1970 and post-2038 dates.
 *
 * @param $format a format string for the result
 * @param $timestamp a unix timestamp
 * @param $timezone name
 *   Use 'UTC' to avoid timezone conversion.
 */
function _date_date($format, $timestamp = false, $timezone = false) {
  if ($timezone === FALSE) {
    $timezone = date_default_timezone_name();
  }

  $_day_power = 86400;
  $arr = _date_getdate($timestamp, $timezone);
  $year = $arr['year'];
  $month = $arr['month'];
  $day = $arr['day'];
  $hour = $arr['hour'];
  $min = $arr['minute'];
  $secs = $arr['second'];
  $max = strlen($format);
  $dates = '';
  
  /*
    at this point, we have the following integer vars to manipulate:
    $year, $month, $day, $hour, $min, $secs
  */
  for ($i = 0; $i < $max; $i++) {
    switch ($format[$i]) {
    case 'T': $dates .= date('T');break;
    // YEAR
    case 'L': $dates .= $arr['leap'] ? '1' : '0'; break;
    case 'r': // Thu, 21 Dec 2000 16:01:07 +0200
      // 4.3.11 uses '04 Jun 2004'
      // 4.3.8 uses  ' 4 Jun 2004'
      $dates .= gmdate('D', $_day_power*(3 + date_dow($day, $month, $year))) .', '.
      ($day < 10 ? '0'. $day : $day) .' '. date('M', mktime(0, 0, 0, $month, 2, 1971)) .' '. $year .' ';
      if ($hour < 10) $dates .= '0'. $hour; else $dates .= $hour;
      if ($min < 10) $dates .= ':0'. $min; else $dates .= ':'. $min;
      if ($secs < 10) $dates .= ':0'. $secs; else $dates .= ':'. $secs;
      $gmt = date_get_gmt_diff_ts($timestamp, $timezone);
      $dates .= sprintf(' %s%04d', ($gmt >= 0) ? '+' : '-', abs($gmt) / 36); break;
    case 'Y': $dates .= date_pad($year, 4); break;
    case 'y': $dates .= substr($year, strlen($year)-2, 2); break;
    // MONTH
    case 'm': if ($month<10) $dates .= '0'. $month; else $dates .= $month; break;
    case 'Q': $dates .= ($month + 3)>>2; break;
    case 'n': $dates .= $month; break;
    case 'M': $dates .= date('M', mktime(0, 0, 0, $month, 2, 1971)); break;
    case 'F': $dates .= date('F', mktime(0, 0, 0, $month, 2, 1971)); break;
    // DAY
    case 't': $dates .= $arr['ndays']; break;
    case 'z': $dates .= $arr['yday']; break;
    case 'w': $dates .= date_dow($day, $month, $year); break;
    case 'l': $dates .= gmdate('l', $_day_power*(3 + date_dow($day, $month, $year))); break;
    case 'D': $dates .= gmdate('D', $_day_power*(3 + date_dow($day, $month, $year))); break;
    case 'j': $dates .= $day; break;
    case 'd': if ($day<10) $dates .= '0'. $day; else $dates .= $day; break;
    case 'S':
      $d10 = $day % 10;
      if ($d10 == 1) $dates .= 'st';
      else if ($d10 == 2 && $day != 12) $dates .= 'nd';
      else if ($d10 == 3) $dates .= 'rd';
      else $dates .= 'th';
      break;
    // HOUR
    case 'Z':
      $dates .= -date_get_gmt_diff_ts($timestamp, $timezone);
      break;
    case 'O':
      $gmt = date_get_gmt_diff_ts($timestamp, $timezone);
      $dates .= sprintf('%s%04d', ($gmt<0)?'+':'-', abs($gmt)/36); 
      break;
    case 'H':
      if ($hour < 10) $dates .= '0'. $hour;
      else $dates .= $hour;
      break;
    case 'h':
      if ($hour > 12) $hh = $hour - 12;
      else {
        if ($hour == 0) $hh = '12';
        else $hh = $hour;
      }
      if ($hh < 10) $dates .= '0'. $hh;
      else $dates .= $hh;
      break;
    case 'G':
      $dates .= $hour;
      break;
    case 'g':
      if ($hour > 12) $hh = $hour - 12;
      else {
        if ($hour == 0) $hh = '12';
        else $hh = $hour;
      }
      $dates .= $hh;
      break;
    // MINUTES
    case 'i': if ($min < 10) $dates .= '0'. $min; else $dates .= $min; break;
    // SECONDS
    case 'U': $dates .= $timestamp; break;
    case 's': if ($secs < 10) $dates .= '0'. $secs; else $dates .= $secs; break;
    // AM/PM
    // Note 00:00 to 11:59 is AM, while 12:00 to 23:59 is PM
    case 'a':
      if ($hour>=12) $dates .= 'pm';
      else $dates .= 'am';
      break;
    case 'A':
      if ($hour>=12) $dates .= 'PM';
      else $dates .= 'AM';
      break;
    default:
      $dates .= $format[$i]; break;
    // ESCAPE
    case "\\":
      $i++;
      if ($i < $max) $dates .= $format[$i];
      break;
    }
  }
  return $dates;
}

/**
 * Low level function to create mktime() for pre-1970 and post 2038 dates.
 *
 * @param $hr the hour
 * @param $min the minute
 * @param $sec the second
 * @param $mon the month
 * @param $day the day
 * @param $year the year
 * @param $timezone name
 *   Use 'UTC' to avoid timezone conversion.
 */
function _date_mktime($hr, $min, $sec, $mon = false, $day = false, $year = false, $timezone = false) {
  if ($timezone === FALSE) {
    $timezone = date_default_timezone_name();
  }

  /*
  # disabled because some people place large values in $sec.
  # however we need it for $mon because we use an array...
  $hr = intval($hr);
  $min = intval($min);
  $sec = intval($sec);
  */
  $mon = intval($mon);
  $day = intval($day);
  $year = intval($year);
  $year = date_year_digit_check($year);
  if ($mon > 12) {
    $y = floor(($mon-1) / 12);
    $year += $y;
    $mon -= $y * 12;
  }
  else if ($mon < 1) {
    $y = ceil((1-$mon) / 12);
    $year -= $y;
    $mon += $y * 12;
  }
  $_day_power = 86400;
  $_hour_power = 3600;
  $_min_power = 60;
  $_month_table_normal = array("", 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
  $_month_table_leap = array("", 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31);
  $_total_date = 0;
  if ($year >= 1970) {
    $year_in = $year;
    for ($a = 1970 ; $a <= $year; $a++) {
      $leap = date_is_leap_year($a);
      if ($leap == true) {
        $loop_table = $_month_table_leap;
        $_add_date = 366;
      }
      else {
        $loop_table = $_month_table_normal;
        $_add_date = 365;
      }
      if ($a < $year) {
        $_total_date += $_add_date;
      }
      else {
        for ($b=1; $b<$mon; $b++) {
          $_total_date += $loop_table[$b];
        }
      }
    }
    $_total_date +=$day-1;
    $ret = ($_total_date * $_day_power) + ($hr * $_hour_power) + ($min * $_min_power) + $sec;
    $ret -= date_get_gmt_diff_ts($ret, $timezone);
    
  }
  else {
    for ($a = 1969 ; $a >= $year; $a--) {
      $leap = date_is_leap_year($a);
      if ($leap == true) {
        $loop_table = $_month_table_leap;
        $_add_date = 366;
      }
      else {
        $loop_table = $_month_table_normal;
        $_add_date = 365;
      }
      if ($a > $year) { $_total_date += $_add_date;
      }
      else {
        for ($b = 12;$b>$mon;$b--) {
          $_total_date += $loop_table[$b];
        }
      }
    }
    $_total_date += $loop_table[$mon] - $day;
    $_day_time = $hr * $_hour_power + $min * $_min_power + $sec;
    $_day_time = $_day_power - $_day_time;
    $ret = -( $_total_date * $_day_power + $_day_time);
    if ($ret < -12220185600) $ret += 10*86400; // if earlier than 5 Oct 1582 - gregorian correction
    else if ($ret < -12219321600) $ret = -12219321600; // if in limbo, reset to 15 Oct 1582.
  }
  return $ret;
}

/**
 * @} End of ingroup "adodb".
 */