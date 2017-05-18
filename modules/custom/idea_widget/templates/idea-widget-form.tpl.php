<?php global $base_url;?>
  <?php if($form['#action'] == '/node/add/idea')
          $action = 'add';
        elseif ($form['#action'] == '/node/edit/idea' || $form['#node']->nid){
          $action = 'edit';
          $nid = $form['#node']->nid;
        }
  ?>
  <div class="">
    <script type="text/javascript" src="<?php print $base_url ?>/sites/all/modules/idea_widget/idea_widget.js?action=<?php print $action?>&&nid=<?php print  $nid ?>"></script>
  </div>
