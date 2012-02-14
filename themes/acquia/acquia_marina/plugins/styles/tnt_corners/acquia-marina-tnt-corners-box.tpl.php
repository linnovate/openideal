<?php
// $Id: acquia-marina-tnt-corners-box.tpl.php,v 1.1.2.2 2009/12/27 23:39:42 jwolf Exp $

/**
 *
 * Markup for Acquia Marina rounded corners.
 *
 */
?>
<div class="acquia_marina-corner-wrapper">
  <div class="acquia_marina-corner-corner">
    <div class="acquia_marina-corner-top-left"></div>
    <div class="acquia_marina-corner-top-right"></div>
    <div class="acquia_marina-corner-outside">
      <div class="acquia_marina-corner-inside">
        <p class="acquia_marina-corner-topspace"></p>
          <?php print $content; ?>
        <p class="acquia_marina-corner-bottomspace"></p>
      </div><!-- /acquia_marina-corner-inside -->
    </div>
    <div class="acquia_marina-corner-bottom-left"></div>
    <div class="acquia_marina-corner-bottom-right"></div>
  </div><!-- /acquia_marina-corner -->
</div><!-- /acquia_marina-corner-wrapper -->
