<?php
// $Id: page.tpl.php,v 1.2.2.1 2009/05/25 09:33:09 jwolf Exp $
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
    <div id="page" class="clearfix">
      
      <?php if ($content): ?>
        <div id="skip">
          <a href="#main-content"><?php print t('Skip to Main Content'); ?></a>
        </div>
      <?php endif; ?>
      <div id="header-wrapper">
        <div id="header" class="clearfix">
          
          <?php if ($search_box): ?>
          <div id="search-box">
            <?php print $search_box; ?>
          </div><!-- /search-box -->
          <?php endif; ?>
      
          <div id="header-first">
            <?php if ($logo): ?> 
            <div id="logo">
              <a href="<?php print check_url($front_page) ?>" title="<?php print t('Home') ?>"><img src="<?php print $logo ?>" alt="<?php print t('Home') ?>" /></a>
            </div>
            <?php endif; ?>
            <?php if ($site_name): ?>
            <h1><a href="<?php print check_url($front_page) ?>" title="<?php print t('Home'); ?>"><?php print $site_name; ?></a></h1>
            <?php endif; ?>
            <?php if ($site_slogan): ?>
            <span id="slogan"><?php print $site_slogan; ?></span>
            <?php endif; ?>
          </div><!-- /header-first -->
  
          <div id="header-middle">
            <?php if ($header_middle): ?>
            <?php print $header_middle; ?>
            <?php endif; ?>
          </div><!-- /header-middle -->
      
          <div id="header-last">
            <?php if ($header_last): ?>
            <?php print $header_last; ?>
            <?php endif; ?>
          </div><!-- /header-last -->
      
        </div><!-- /header -->
      </div><!-- /header-wrapper -->
      
      <div id="primary-menu-wrapper" class="clearfix">
        <?php if ($primary_links): ?>
        <div id="primary-menu">
          <?php print $primary_links_tree; ?>
        </div><!-- /primary_menu -->
        <?php endif; ?>
      </div><!-- /primary-menu-wrapper -->

      <div id="preface">
        <?php if ($preface_first || $preface_middle || $preface_last || $mission): ?>
        <div id="preface-wrapper" class="<?php print $prefaces; ?> clearfix">
          <?php if ($mission): ?>
          <div id="mission"> 
            <?php print $mission; ?>
          </div>
          <?php endif; ?>
        
          <?php if ($preface_first): ?>
          <div id="preface-first" class="column">
            <?php print $preface_first; ?>
          </div><!-- /preface-first -->
          <?php endif; ?>

          <?php if ($preface_middle): ?>
          <div id="preface-middle" class="column">
            <?php print $preface_middle; ?>
          </div><!-- /preface-middle -->
          <?php endif; ?>

          <?php if ($preface_last): ?>
          <div id="preface-last" class="column">
            <?php print $preface_last; ?>
          </div><!-- /preface-last -->
          <?php endif; ?>
        </div><!-- /preface-wrapper -->
        <?php endif; ?>
      </div><!-- /preface -->

      <div id="main-wrapper">
        <div id="main" class="clearfix">
          
          <?php if ($breadcrumb): ?>
          <div id="breadcrumb">
            <?php print $breadcrumb; ?>
          </div><!-- /breadcrumb -->
          <?php endif; ?>
        
          <?php if ($sidebar_first): ?>
          <div id="sidebar-first">
            <?php print $sidebar_first; ?>
          </div><!-- /sidebar-first -->
          <?php endif; ?>

          <div id="content-wrapper">

            <?php if ($messages): ?>
              <?php print $messages; ?>
            <?php endif; ?>

            <?php if ($content_top): ?>
            <div id="content-top">
              <?php print $content_top; ?>
            </div><!-- /content-top -->
            <?php endif; ?>
            
            <div id="content">
              <a name="main-content" id="main-content"></a>
              <?php if ($tabs): ?>
              <div id="content-tabs">
                <?php print $tabs; ?>
              </div>
              <?php endif; ?>

              <?php if (($sidebar_first) && ($sidebar_last)) : ?>
                <?php if ($sidebar_last): ?>
                <div id="sidebar-last">
                  <?php print $sidebar_last; ?>
                </div><!-- /sidebar_last -->
                <?php endif; ?>
              <?php endif; ?>

              <div id="content-inner">
                
              <?php if ($help): ?>
                <div id="help">
                  <?php print $help; ?>
                </div>
              <?php endif; ?>
                
                <?php if ($title): ?>
                <h1 class="title"><?php print $title; ?></h1>
                <?php endif; ?>
                <div id="content-content">
                  <?php print $content; ?>
                </div>
              </div><!-- /content-inner -->
            </div><!-- /content -->

            <?php if ($content_bottom): ?>
            <div id="content-bottom">
              <?php print $content_bottom; ?>
            </div><!-- /content-bottom -->
            <?php endif; ?>
          </div><!-- /content-wrapper -->
          
          <?php if ((!$sidebar_first) && ($sidebar_last)) : ?>
            <?php if ($sidebar_last): ?>
            <div id="sidebar-last">
              <?php print $sidebar_last; ?>
            </div><!-- /sidebar_last -->
            <?php endif; ?>
          <?php endif; ?>

          <?php if ($postscript_first || $postscript_middle || $postscript_last): ?>
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
          <?php endif; ?>
          
          <?php print $feed_icons; ?>

          <?php if ($footer_top || $footer || $footer_message): ?>
          <div id="footer" class="clearfix">
            <?php if ($footer_top): ?>
            <?php print $footer_top; ?>
            <?php endif; ?>
            <?php if ($footer): ?>
            <?php print $footer; ?>
            <?php endif; ?>
            <?php if ($footer_message): ?>
            <?php print $footer_message; ?>
            <?php endif; ?>
          </div><!-- /footer -->
          <?php endif; ?>
          
        </div><!-- /main -->
      </div><!-- /main-wrapper -->
    </div><!-- /page -->
    <?php print $closure; ?>
  </body>
</html>
