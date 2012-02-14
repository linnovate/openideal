<?php
// $Id: calendar-month-multiple-node.tpl.php,v 1.1.2.5 2009/02/14 16:51:50 karens Exp $
/**
 * @file
 * Template to display a summary of the days items as a calendar month node.
 * 
 * 
 * @see template_preprocess_calendar_month_multiple_node.
 */
?>
<div class="view-item view-item-<?php print $view->name ?>">
  <div class="calendar monthview" id="<?php print $curday ?>">
    <?php foreach ($types as $type): ?>
      <?php if ($view->date_info->style_max_items_behavior != 'more'): ?>
        <?php print theme('calendar_stripe_stripe', $type); ?>
      <?php endif; ?>
    <?php endforeach; ?>
    <div class="view-item <?php print views_css_safe('view-item-'. $view->name) ?>">
      <?php if ($view->date_info->style_max_items_behavior != 'more'): ?>
        <div class="multiple-events"> 
          <?php print l(t('Click to see all @count events', array('@count' => $count)), $link) ?>
        </div>    
    </div>
    <?php else: ?>
      <div class="calendar-more"><?php print l(t('more'), $link) ?>»</div>
    <?php endif; ?>
  </div>    
</div>
