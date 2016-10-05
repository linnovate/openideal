<?php  
$form_r = drupal_get_form('user_register_form') ;
?>
<div class="page-wrapper">
    <?php if (!empty($page['sidebar_first'])): ?>
    <aside class="sidebar_first">
        <?php  print render($page['sidebar_first']);?>
    </aside>
    <?php endif;?>
    <div class="content-wrapper">
        <div class="content-block">
        <div>           
            <!-- Register screen -->
            <div class='title'>
                <p>
                    <?php print t("New to the circle?");?>
                </p>
            </div>
            <div class="inner">
            <?php if ($messages): ?>
            <div id="messages">
                <?php print $messages; ?>
            </div>
            <?php endif; ?>
            <?php print drupal_render($form_r); ?>                
            </div><!-- inner -->
        </div>
            
        </div><!-- content-block -->   
    
    <div class="footer pane-panels-mini pane-innovate-logo">
       <?php print $logo_section; ?>
    </div><!-- footer -->
   </div><!-- content-wrapper -->
</div><!-- page-wrapper -->
