<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="<?php print $language->language ?>" lang="<?php print $language->language ?>" dir="<?php print $language->dir ?>">
  <head>
    <?php print $head ?>
    <?php print $styles ?>
    <?php print $scripts ?>
    <title><?php print $head_title ?></title>
  </head>
  <body <?php print drupal_attributes($attr) ?>>

  <?php if ($help): ?>
  <div id='page-help'><div class='limiter clear-block'>
    <?php if ($help_toggler) print $help_toggler ?>
    <?php if ($help) print $help ?>
  </div></div>
  <?php endif; ?>

  <div id='top'>
    <div id='page-title' class='limiter clear-block'>
      <!--<?php if ($help_toggler) print $help_toggler ?>-->
      <?php if ($primary_links) print theme('links', $primary_links) ?>
      <h1 class='page-title <?php print $page_icon_class ?>'>
        <?php if (!empty($page_icon_class)): ?><span class='icon'></span><?php endif; ?>
        <?php if ($title) print $title ?>
      </h1>
    </div>
    <div id='branding'><div class='limiter clear-block'>
      <?php if ($user_links) print theme('links', $user_links) ?>
      <div class='breadcrumb clear-block'><?php print $breadcrumb ?></div>
    </div></div>
  </div>

  <?php if ($tabs): ?>
    <div id='tabs'>
      <div class='page-tabs limiter clear-block'><?php print $tabs ?></div>
      <?php if ($tabs2): ?><div class='page-tabs limiter clear-block'><?php print $tabs2 ?></div><?php endif; ?>
    </div>
  <?php endif; ?>
