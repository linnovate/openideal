<?php 
// $Id: page.tpl.php,v 1.1.4.1 2009/05/26 06:21:30 jwolf Exp $ 
?>

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language; ?>" xml:lang="<?php print $language->language; ?>">

  <head>
    <title><?php print $head_title; ?></title>
    <?php print $head; ?>
    <?php print $styles; ?>
    <!--[if IE 7]>
      <?php print $ie7_styles; ?>
    <![endif]-->
    <!--[if lte IE 6]>
      <?php print $ie6_styles; ?>
    <![endif]-->
    <?php if ($local_styles): ?>
    <?php print $local_styles; ?>
    <?php endif; ?>
    <?php print $scripts; ?>
  </head>

  <body class="<?php print $body_classes; ?>">
    <div id="page">
      <?php if ($content): ?>
      <div id="skip">
        <a href="#main-content"><?php print t('Skip to Main Content'); ?></a>
      </div>
      <?php endif; ?>
      <?php if ($banner): ?>
      <div id="banner" class="clearfix">
        <?php print $banner; ?>
      </div><!-- /banner -->
      <?php endif; ?> 
      <?php if ($header_top): ?>
      <div id="header-top" class="clearfix">
        <?php print $header_top; ?>
      </div><!-- /header-top -->
      <?php endif; ?>
      <?php if ($primary_links): ?>
        <div id="primary-menu">
          <?php print theme('links', $primary_links); ?>
        </div><!-- /primary_menu -->
      <?php endif; ?>
      <div id="header-wrapper" class="clearfix">
        <div id="header-first">
          <?php if ($logo): ?> 
          <div id="logo">
            <a href="<?php print check_url($front_page) ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a>
          </div>
          <?php endif; ?>
          <?php if ($site_name): ?>
          <h1><a href="<?php print check_url($front_page) ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
          <?php endif; ?>
        </div><!-- /header-first -->
        <div id="header-middle">
          <?php if ($site_slogan): ?>
          <div id="slogan">
            <?php print $site_slogan; ?>
          </div>
          <?php endif; ?>
          <?php if ($header_middle): ?>
          <?php print $header_middle; ?>
          <?php endif; ?>
        </div><!-- /header-middle -->
        <div id="search-box">
          <?php print $search_box; ?>
        </div><!-- /search-box -->
      </div><!-- /header-wrapper -->
      <div id="main-wrapper" class="clearfix">  
        <?php if ($sidebar_first || $secondary_links): ?>
        <div id="sidebar-first">
          <?php if ($secondary_links): ?>
          <div id="secondary-menu">
            <?php print theme('links', $secondary_links); ?>
          </div><!-- /secondary_menu -->
          <?php endif; ?>
          <?php if ($sidebar_first): ?>
          <?php print $sidebar_first; ?>
          <?php endif; ?>
        </div><!-- /sidebar-first -->
        <?php endif; ?>
        <div id="content-wrapper">
          <?php if ($messages): ?>
          <?php print $messages; ?>
          <?php endif; ?>
          <?php if ($help): ?>
          <?php print $help; ?>
          <?php endif; ?>
          <?php if ($breadcrumb): ?>
          <div id="breadcrumb">
            <?php print $breadcrumb; ?>
          </div><!-- /breadcrumb -->
          <?php endif; ?>
          <?php if ($content_top):?>
          <div id="content-top" <?php print $banner_image; ?>>
            <?php print $content_top; ?>  
          </div><!-- /content-top -->
          <?php endif; ?>
          <div id="content">
            <a name="main-content" id="main-content"></a>
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
            <?php if ($content_bottom): ?>
            <div id="content-bottom">
              <div class="corner top-right"></div>
              <div class="corner top-left"></div>
              <div class="inner">
              <?php print $content_bottom; ?>
              </div>
              <div class="corner bottom-right"></div>
              <div class="corner bottom-left"></div>
            </div><!-- /content-bottom -->
            <?php endif; ?>
          </div><!-- /content -->
        </div><!-- /content-wrapper -->
        <?php if ($sidebar_last): ?>
        <div id="sidebar-last">
          <?php print $sidebar_last; ?>
        </div><!-- /sidebar_last -->
        <?php endif; ?>
      </div><!-- /main-wrapper -->
    </div><!-- /page -->
    <?php if ($postscript_first || $postscript_middle || $postscript_last): ?>
    <div id="postscripts">
      <div id="postscript-wrapper" class="<?php print $postscripts; ?> clearfix">
        <?php if ($postscript_first): ?>
        <div id="postscript-first" class="column">
          <?php print $postscript_first; ?>
        </div><!-- /postscript-first -->
        <?php endif; ?>
        <?php if ($postscript_middle): ?>
        <div id="postscript-middle" class="column">
          <?php print $postscript_middle; ?>
        </div><!-- /postscript-middle -->
        <?php endif; ?>
        <?php if ($postscript_last): ?>
        <div id="postscript-last" class="column">
          <?php print $postscript_last; ?>
        </div><!-- /postscript-last -->
        <?php endif; ?>
      </div><!-- /postscript-wrapper -->
    </div><!-- /postscripts -->
    <?php endif; ?>
    <?php if ($footer || $footer_message): ?>
    <div id="footer" class="clearfix">
      <div id="footer-wrapper">
        <?php if ($footer_message): ?>
        <div id="footer-message">
          <?php print $footer_message; ?>
        </div>
        <?php endif; ?>
        <?php if ($footer) : ?>
          <div id="footer-region">
            <?php print $footer; ?>
          </div>
        <?php endif; ?>
      </div><!-- /footer-wrapper -->
    </div><!-- /footer -->
    <?php endif; ?>
    <?php print $closure; ?>
  </body>
</html>