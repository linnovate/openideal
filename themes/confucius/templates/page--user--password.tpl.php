<?php  
$form_p = drupal_get_form('user_pass') ;
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
            <div class='title'>
                <p>
                    <?php print t("Renew password?");?>
                </p>
            </div>
            <div class="inner">
                <?php if ($messages): ?>
                <div id="messages">
                    <?php print $messages; ?>
                </div>
                <?php endif; ?>
                <?php print drupal_render($form_p); ?>
            </div>
        </div>
        </div><!-- content-block -->        

        <div class="footer pane-panels-mini pane-confusius-logo">
           <?php print $logo_section; ?>
        </div> <!-- footer -->
       
    </div><!-- content-wrapper -->    

</div><!-- page-wrapper -->