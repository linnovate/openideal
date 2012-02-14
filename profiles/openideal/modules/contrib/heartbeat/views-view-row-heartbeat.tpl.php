<?php

/**
 * @file views-view-row-heartbeat.tpl.php
 * Default simple view template to display a single activity message.
 */
?>
<?php print $heartbeat_activity; ?>
<?php if ($comments): ?>
  <?php print $comments; ?>
<?php endif; ?>
