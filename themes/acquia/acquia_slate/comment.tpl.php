<?php
// $Id: comment.tpl.php,v 1.1 2009/02/28 23:33:58 jwolf Exp $
?>
<!-- start comment.tpl.php -->
<div class="comment <?php print $comment_classes;?> clear-block">
  <?php print $picture ?>
  <?php if ($comment->new): ?>
  <a id="new"></a>
  <span class="new"><?php print $new ?></span>
  <?php endif; ?>
  <h3 class="title"><?php print $title ?></h3>
  <div class="submitted">
    <?php print $submitted ?>
  </div>
  <div class="content">
    <?php print $content ?>
    <?php if ($signature): ?>
    <div class="signature">
      <?php print $signature ?>
    </div>
    <?php endif; ?>
  </div>
  <?php if ($links): ?>
  <div class="links">
    <?php print $links ?>
  </div>
  <?php endif; ?>
</div>
<!-- /end comment.tpl.php -->