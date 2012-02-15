<?php
/**
 * @file
 *   Template for thumbs widget.
 */
?>
<div class="vud-widget vud-widget-thumbs" id="<?php print $id; ?>">
  <div class="up-score clear-block">
    <?php if ($show_links): ?>
      <?php if ($show_up_as_link): ?>
        <a href="<?php print $link_up; ?>" rel="nofollow" class="<?php print "$link_class_up"; ?>" title="<?php print t('Vote up!'); ?>">
      <?php endif; ?>
      <div class="vote-thumb <?php print $class_up; ?>" title="<?php print t('Vote up!'); ?>"></div>
      <div class="element-invisible"><?php print t('Vote up!'); ?></div>
      <?php if ($show_up_as_link): ?>
        </a>
      <?php endif; ?>
    <?php endif; ?>
  </div>

  <div class="down-score clear-block">
    <?php if ($show_links): ?>
      <?php if ($show_down_as_link): ?>
        <a href="<?php print $link_down; ?>" rel="nofollow" class="<?php print "$link_class_down"; ?>" title="<?php print t('Vote down!'); ?>">
      <?php endif; ?>
      <div class="vote-thumb <?php print $class_down; ?>" title="<?php print t('Vote down!'); ?>"></div>
      <div class="element-invisible"><?php print t('Vote down!'); ?></div>
      <?php if ($show_down_as_link): ?>
        </a>
      <?php endif; ?>
    <?php endif; ?>
  </div>

  <?php if ($show_reset): ?>
    <a href="<?php print $link_reset; ?>" rel="nofollow" class="element-invisible <?php print $link_class_reset; ?>" title="<?php print $reset_long_text; ?>">
      <div class="<?php print $class_reset; ?>">
        <?php print $reset_short_text; ?>
      </div>
    </a>
  <?php endif; ?>
  <p class="vote-current-score"><?php print t('Points'); ?>: <strong><?php print $vote_sum; ?></strong></p>

  <?php if ($class_up == 'up-active'): ?>
    <p class="voted-how"><?php print t('You voted &lsquo;up&rsquo;'); ?></p>
  <?php elseif ($class_down == 'down-active'): ?>
    <p class="voted-how"><?php print t('You voted &lsquo;down&rsquo;'); ?></p>
  <?php endif; ?>

</div>
