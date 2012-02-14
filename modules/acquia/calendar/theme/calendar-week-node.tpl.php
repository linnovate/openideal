<?php
// $Id: calendar-week-node.tpl.php,v 1.2.2.2 2008/06/20 11:19:30 karens Exp $
/**
 * @file
 * Template to display a view item as a calendar week node.
 * 
 * $node 
 *   A node object for this calendar item. Note this is
 *   not a complete node object, but it will have $node->nid
 *   that you can use to load the full object, and
 *   $node->type to tell the content type of the node.
 * 
 * $fields
 *   An array of information for every field selected in the 'Fields'
 *   section of this view, formatted as requested in the View setup.
 * 
 * Calendar info for this individual calendar item is in local time --
 * the user timezone where configurable timezones are allowed and set,
 * otherwise the site timezone. If this item has extends over more than
 * one day, it has been broken apart into separate nodes for each calendar
 * date and calendar_start will be no earlier than the start of
 * the current day and calendar_end will be no later than the end
 * of the current day.
 * 
 * $calendar_start - A formatted datetime start date for this item.
 *   i.e. '2008-05-12 05:26:15'.
 * $calendar_end - A formatted datetime end date for this item,
 *   the same as the start date except for fields that have from/to
 *   fields defined, like Date module dates. 
 * $calendar_start_date - a PHP date object for the start time.
 * $calendar_end_date - a PHP date object for the end time.
 * 
 * You can use PHP date functions on the date object to display date
 * information in other ways, like:
 * 
 *   print date_format($calendar_start_date, 'l, j F Y - g:ia');
 * 
 * @see template_preprocess_calendar_week_node.
 */ 
?>
<div class="view-item view-item-<?php print $view->name ?>">
  <div class="calendar weekview" id="<?php print $node->date_id ?>">
    <?php print theme('calendar_stripe_stripe', $node); ?>
    <?php foreach ($fields as $field): ?>
      <div id="<?php print $field['id']; ?>" class="view-field view-data-<?php print $field['id'] ?>">
        <?php if ($field['label']): ?>
          <div class="view-label-<?php print $field['id'] ?>"><?php print $field['label'] ?></div>
        <?php endif; ?>  
        <?php print $field['data']; ?>
      </div>  
    <?php endforeach; ?>
  </div>    
</div>
