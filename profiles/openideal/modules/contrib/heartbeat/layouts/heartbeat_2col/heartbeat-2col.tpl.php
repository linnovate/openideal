<?php

/**
 * @file
 * Display Suite 2 column template for heartbeat activity.
 */
?>
<div class="<?php print $classes;?> clearfix">

  <?php if (isset($title_suffix['contextual_links'])): ?>
  <?php print render($title_suffix['contextual_links']); ?>
  <?php endif; ?>

  <div class="heartbeat-group-left<?php print $heartbeat_left_classes; ?>">
  <?php if ($heartbeat_left): ?>
      <?php print $heartbeat_left; ?>
  <?php endif; ?>
  </div>

  <div class="heartbeat-group-content">
    <?php if ($heartbeat_content): ?>
      <div class="group-content<?php print $heartbeat_content_classes; ?>">
        <?php print $heartbeat_content; ?>
      </div>
    <?php endif; ?>

    <?php if ($heartbeat_footer): ?>
      <div class="group-footer<?php print $heartbeat_footer_classes; ?>">
        <?php print $heartbeat_footer; ?>
      </div>
    <?php endif; ?>
  </div>

</div>