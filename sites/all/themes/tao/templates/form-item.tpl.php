<div <?php if (!empty($attr)) print drupal_attributes($attr) ?>>
  <?php if (!empty($label_title)): ?>
    <label <?php if (!empty($label_attr)) print drupal_attributes($label_attr) ?>><?php print $label_title ?></label>
  <?php endif; ?>
  <?php if (!empty($value)) print $value ?>
  <?php if (!empty($description)): ?>
    <div class='description'><?php print $description ?></div>
  <?php endif; ?>
</div>
