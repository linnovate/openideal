<fieldset <?php if (!empty($attr)) print drupal_attributes($attr) ?>>
  <?php if (!empty($title)): ?>
    <legend><span class='<?php print $hook ?>-title'><?php print $title ?></span></legend>
  <?php endif; ?>
  <?php if (!empty($content)): ?>
    <div class='<?php print $hook ?>-content clear-block <?php if (!empty($is_prose)) print 'prose' ?>'>
      <?php print $content ?>
    </div>
  <?php endif; ?>
</fieldset>
