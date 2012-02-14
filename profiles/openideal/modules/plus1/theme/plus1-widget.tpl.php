<?php
/**
 * @file
 * Template implementation to display the node widget.
 */
?>
<div class="plus1-widget">
  <?php if (!isset($use_arrow_down)) : ?>
    <div class="plus1-msg">
      <?php if($can_vote) : ?>
        <div class="<?php if (!$voted) print 'plus1-vote';?>">
          <?php print $widget_message; ?>
        </div>
      <?php else: ?>
        <small><?php print $widget_message; ?></small>
      <?php endif; ?>
    </div>
  <?php endif;?>

  <div class="plus1-score">
    <?php print $score; ?>
  </div>

  <?php if (isset($use_arrow_down)) : ?>
    <div class="plus1-msg">
      <div class="plus1-undo-vote">
        <?php print $widget_message; ?>
      </div>
    </div><?php endif;?>
</div>
