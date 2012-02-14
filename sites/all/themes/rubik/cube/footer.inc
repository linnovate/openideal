  <?php if ($feed_icons || $footer_message): ?>
  <div id='footer' class='limiter clear-block'>
    <?php if ($feed_icons): ?>
      <div class='feed-icons clear-block'>
        <label><?php print t('Feeds') ?></label>
        <?php print $feed_icons ?>
      </div>
    <?php endif; ?>
    <?php if ($footer_message): ?><div class='footer-message'><?php print $footer_message ?></div><?php endif; ?>
  </div>
  <?php endif; ?>

  <?php print $closure ?>

  </body>
</html>
