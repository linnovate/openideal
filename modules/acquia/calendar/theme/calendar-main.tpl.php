<?php
// $Id: calendar-main.tpl.php,v 1.2.2.4 2009/01/10 20:04:18 karens Exp $
/**
 * @file
 * Template to display calendar navigation and links.
 * 
 * @see template_preprocess_calendar_main.
 *
 * $view: The view.
 * $calendar_links: Array of formatted links to other calendar displays - year, month, week, day.
 * $calendar_popup: The popup calendar date selector.
 * $display_type: year, month, day, or week.
 * $mini: Whether this is a mini view.
 * $min_date_formatted: The minimum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * $max_date_formatted: The maximum date for this calendar in the format YYYY-MM-DD HH:MM:SS.
 * 
 */
//dsm('Display: '. $display_type .': '. $min_date_formatted .' to '. $max_date_formatted);
?>

<div class="calendar-calendar">
  <?php if (!empty($calendar_popup)) print $calendar_popup;?>
  <?php if (!empty($calendar_add_date)) print $calendar_add_date; ?>
  <?php if (empty($block)) print theme('links', $calendar_links);?>
  <?php print theme('date_navigation', $view) ?>
</div>