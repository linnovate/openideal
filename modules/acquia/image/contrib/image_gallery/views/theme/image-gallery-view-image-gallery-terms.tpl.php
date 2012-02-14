<?php
// $Id: image-gallery-view-image-gallery-terms.tpl.php,v 1.1 2009/08/27 12:10:57 joachim Exp $
/**
 * @file
 * Template for a list of gallery terms.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 * @ingroup views_templates
 */
?>
<div class="item-list image-gallery-terms clear-block">
  <?php if (!empty($title)) : ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <<?php print $options['type']; ?> class="galleries">
    <?php foreach ($rows as $id => $row): ?>
      <li class="<?php print $classes[$id]; ?> clear-block"><?php print $row; ?></li>
    <?php endforeach; ?>
  </<?php print $options['type']; ?>>
</div>
