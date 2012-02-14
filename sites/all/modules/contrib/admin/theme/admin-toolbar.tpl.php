<div id='admin-toolbar' class='<?php print $position ?> <?php print $layout ?> <?php print $behavior ?>'>
  <span class='admin-toggle'><?php print t('Admin') ?></span>

  <div class='admin-blocks admin-blocks-<?php print count($blocks) ?>'>
    <div class='admin-tabs clear-block'>
      <?php foreach ($tabs as $bid => $tab): ?>
        <?php print theme('admin_tab', $tab, $bid); ?>
      <?php endforeach; ?>
    </div>

    <?php foreach ($blocks as $bid => $block): ?>
      <div class='admin-block <?php if (isset($block->class)) print $block->class ?>' id='block-<?php print $bid ?>'>
        <div class='block-content clear-block'><?php print $block->content ?></div>
      </div>
    <?php endforeach; ?>
  </div>

</div>
