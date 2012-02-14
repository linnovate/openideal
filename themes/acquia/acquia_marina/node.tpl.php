<?php 
// $Id$ 
?>

<!-- start node.tpl.php -->
<div id="node-<?php print $node->nid; ?>" class="node <?php print $node_classes; ?>">
  <?php print $picture ?>

  <?php if ($page == 0): ?>
  <h2 class="title"><a href="<?php print $node_url ?>" title="<?php print $title ?>"><?php print $title ?></a></h2>
  <?php endif; ?>

  <div class="meta">
    <?php if ($submitted): ?>
    <span class="submitted"><?php print $submitted ?></span>
    <?php endif; ?>
  </div>

  <div class="content">
    <?php print $content ?>
  </div>

  <?php if ($terms): ?>
  <div class="terms">
    <?php print $terms; ?>
  </div>
  <?php endif;?>
  
  <?php if ($links): ?>
  <div class="links">
    <?php print $links; ?>
  </div>
  <?php endif; ?>

  <?php if ($node_bottom && !$teaser): ?>
  <div id="node-bottom">
    <?php print $node_bottom; ?>
  </div>
  <?php endif; ?>
</div>
<!-- /#node-<?php print $node->nid; ?> -->
