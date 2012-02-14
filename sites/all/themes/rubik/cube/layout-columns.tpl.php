<?php include('header.inc'); ?>

<?php if ($header): ?><div id='header' class='limiter clear-block'><?php print $header ?></div><?php endif; ?>

<div id ='page' class='clear-block limiter page-content'>
  <?php if ($show_messages && $messages): ?>
    <div id='console' class='clear-block'><?php print $messages; ?></div>
  <?php endif; ?>
  <div id='left' class='clear-block'>
    <?php print $left ?>
  </div>
  <div id='content'>
    <?php if (!empty($content)): ?>
      <div class='content-wrapper clear-block'><?php print $content ?></div>
    <?php endif; ?>
    <?php print $content_region ?>
  </div>
  <div id='right' class='clear-block'>
    <?php print $right ?>
  </div>
</div>

<?php include('footer.inc'); ?>
