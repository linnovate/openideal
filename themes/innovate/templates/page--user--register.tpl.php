<?php  
$form_r = drupal_get_form('user_register_form') ;
?>
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
    
      <!-- footer -->
   </div><!-- content-wrapper -->
</div><!-- page-wrapper -->
