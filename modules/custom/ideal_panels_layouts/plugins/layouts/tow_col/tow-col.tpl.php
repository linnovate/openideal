<?php
/**
 * @brief  Template for the sidebar mini-panel panels layout.
 */
?>

<div class="panel-display panel-tow-col clear-block" <?php if (!empty($css_id)) { print "id=\"$css_id\""; } ?>>

  <?php if (!empty($content['region_first'])): ?>
    <div class="panel-panel unit pane-region-first">
      <div class="inside">
        <?php print $content['region_first']; ?>
      </div>
    </div>
  <?php endif; ?>

  <?php if (!empty($content['region_second'])): ?>
    <div class="panel-panel unit pane-region-second" >
      <div class="inside">
        <?php print $content['region_second']; ?>
      </div>
    </div>
  <?php endif; ?>

</div>