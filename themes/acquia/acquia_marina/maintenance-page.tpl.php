<?php
// $Id$
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">
  
  <head>
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <!--[if IE 7]>
      <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/ie7-fixes.css" type="text/css">
    <![endif]-->
    <!--[if lte IE 6]>
      <link rel="stylesheet" href="<?php print $base_path . $directory; ?>/ie6-fixes.css" type="text/css">
    <![endif]-->
    <?php print $scripts; ?>
  </head>
  
  <body class="<?php print $body_classes; ?>">
    <div id="page" class="clearfix">
      <div id="header-wrapper" class="clearfix">
        <div id="header-first">
          <?php if ($logo): ?> 
          <div id="logo">
            <a href="<?php print $base_path ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a>
          </div>
          <?php endif; ?>
          <?php if ($site_name): ?>
          <h1><a href="<?php print $base_path ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
          <?php endif; ?>
          <?php if ($site_slogan): ?>
          <span id="slogan"><?php print $site_slogan; ?></span>
          <?php endif; ?>
        </div><!-- /header-first -->
      </div><!-- /header-wrapper -->
      <div id="main-wrapper" class="clearfix">  
        <div id="content-wrapper">
          <div id="content">
            <?php if ($tabs): ?>
            <div id="content-tabs" class="clear">
              <?php print $tabs; ?>
            </div>
            <?php endif; ?>
            <?php if ($content || $title): ?>
            <div id="content-inner" class="clear">
              <?php if ($title): ?>
              <h1 class="title"><?php print $title; ?></h1>
              <?php endif; ?>
              <?php if ($content): ?>
              <?php print $content; ?>
              <?php endif; ?>
            </div>
            <?php endif; ?>
          </div><!-- /content -->
        </div><!-- /content-wrapper -->
      </div><!-- /main-wrapper -->
    </div><!-- /page -->
  </body>
</html>
