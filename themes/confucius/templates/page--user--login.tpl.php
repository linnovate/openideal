<div class="page-wrapper">
    <?php if (!empty($page['sidebar_first'])): ?>
    <aside class="sidebar_first">
        <?php  print render($page['sidebar_first']);?>
    </aside>
    <?php endif;?>
    <aside class="sidebar_first">
        <?php
          $block = module_invoke('panels_mini', 'block_view', 'sidebar_first');
          print $block['content'];    
        ?>
    </aside>

    <div class="content-wrapper">
        <div class="content-block">
            <a class= "logo" href="/front"><img alt=" Home page " height="64" src="/profiles/idea/themes/confucius/images/confucius-logo.png" title=" Home page " width="164"></a>
        <div>            
            <!-- login screen -->
            <div class='title'>
                <p>
                    <?php print t("Welcome to @sitename", array('@sitename' => variable_get('site_name', 'OpenideaL'))); ?>
                </p>
            </div>
            <div class="inner">
            <!-- to register -->
                <?php if ($messages): ?>
                <div id="messages">
                    <?php print $messages; ?>
                </div>
                <?php endif; ?>
                <?php print drupal_render($page['content']['system_main']); ?>     
               
                <div class="more-options">
                    <!-- renew password -->
                  <!--   <div class="to-renew">
                        <?php print l(t('Forgot your password'),'/user/password');?>
                    </div>

                    <div class="to-register">
                        <?php print l(t('New User'),'/user/register');?>
                    </div> -->
            </div>
            </div>
             </div>
         </div>
        <!-- content-block -->

    </div><!-- content-wrapper -->    

</div><!-- page-wrapper -->
