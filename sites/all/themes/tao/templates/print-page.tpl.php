<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>">
<head>
  <?php print $head ?>
  <?php print $styles ?>

  <title><?php print $head_title ?></title>
</head>

<body <?php print drupal_attributes($attr) ?>>
  <div class='limiter clear-block'>
    <div id='content' class='clear-block'>
      <?php print $print_header ?>
      <?php print $content ?>
    </div>
    <?php if ($footer_message): ?>
      <div id='footer' class='clear-block'><?php print $footer_message ?></div>
    <?php endif; ?>
  </div>
</body>

</html>
