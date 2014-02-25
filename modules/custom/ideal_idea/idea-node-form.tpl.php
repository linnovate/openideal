<div class="node-add-wrapper clear-block">
  <div class="node-column-sidebar">
    <?php if($sidebar): ?>
      <?php print render($sidebar); ?>
    <?php endif; ?>
  </div>
  <div class="node-column-main">
    <?php if($form): ?>
      <?php print drupal_render_children($form); ?>
    <?php endif; ?>
    <?php if($buttons): ?>
      <div class="node-buttons">
        <?php print render($buttons); ?>
    </div>
  <?php endif; ?>
  </div>
  <div class="clear"></div>
</div>