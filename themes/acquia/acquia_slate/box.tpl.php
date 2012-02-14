<?php
// $Id: box.tpl.php,v 1.1 2009/02/28 23:33:58 jwolf Exp $
?>
<!-- start box.tpl.php -->
<div class="box">

<?php if ($title): ?>
  <h2 class="title"><?php print $title ?></h2>
<?php endif; ?>

  <div class="content"><?php print $content ?></div>
</div>
<!-- /end box.tpl.php -->