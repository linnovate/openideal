<div class='form form-layout-simple clear-block'>
  <?php print drupal_render($form) ?>
  <?php if ($buttons): ?>
    <div class='buttons'><?php print drupal_render($buttons) ?></div>
  <?php endif; ?>
</div>
