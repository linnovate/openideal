<?php
// $Id: image-view-image-gallery.tpl.php,v 1.2.2.1 2010/08/03 17:43:00 sun Exp $


/**
 * @file
 * Template for a list of gallery nodes.
 *
 * - $title : The title of this group of rows.  May be empty.
 * - $options['type'] will either be ul or ol.
 * @ingroup views_templates
 */
?>
<div class="item-list image-gallery-nodes clear-block">
  <?php if (!empty($title)): ?>
    <h3><?php print $title; ?></h3>
  <?php endif; ?>
  <<?php print $options['type']; ?> class="images">
    <?php foreach ($rows as $id => $row): ?>
      <li class="<?php print $classes[$id]; ?>"><?php print $row; ?></li>
    <?php endforeach; ?>
  </<?php print $options['type']; ?>>
</div>

