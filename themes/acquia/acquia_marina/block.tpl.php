<?php
// $Id$
?>

<!-- start block.tpl.php -->
<div class="block-wrapper <?php print $block_zebra; ?>">
  <?php if ($rounded_block): ?>   <!-- see preprocess_block() -->
  <div class="rounded-block">
    <div class="rounded-block-top-left"></div>
    <div class="rounded-block-top-right"></div>
    <div class="rounded-outside">
      <div class="rounded-inside">
        <p class="rounded-topspace"></p>
  <?php endif; ?>
      
        <div id="block-<?php print $block->module .'-'. $block->delta; ?>" class="block block-<?php print $block->module ?>">
          <?php if ($block->subject): ?>
            <?php if ($rounded_block): ?>
          <div class="block-icon pngfix"></div>
            <?php endif; ?>
          <h2 class="title block-title pngfix"><?php print $block->subject ?></h2>
          <?php endif; ?>
          <div class="content">
            <?php print $block->content ?>
          </div>
        </div>
  
  <?php if ($rounded_block): ?>
        <p class="rounded-bottomspace"></p>
      </div><!-- /rounded-inside -->
    </div>
    <div class="rounded-block-bottom-left"></div>
    <div class="rounded-block-bottom-right"></div>
  </div><!-- /rounded-block -->
  <?php endif; ?>
  
</div>
<!-- /end block.tpl.php -->
