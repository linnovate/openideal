<?php
/**
 * Plugin info 
 */
$plugin = array(
  'title' => t('Social Links'),
  'single' => TRUE,
  'category' => array(t('Miscellaneous'), -9),
);

/**
 * Run-time rendering of the body of the block.
 */
function ideal_panes_social_links_content_type_render($subtype, $conf, $args, $context) {
  $output = '';
  $output .= '<a class="social_link facebook" href="'.$conf['facebook'].'">facebook</a>';
  $output .= '<a class="social_link twitter"  href="'.$conf['twitter'].'">twitter</a>';
  $output .= '<a class="social_link google" href="'.$conf['google'].'">google</a>';
  $output .= '<a class="social_link linkedin" href="'.$conf['linkedin'].'">linkedin</a>';
  $block = new stdClass();
  $block->title = "";
  $block->content = $output;
  
  return $block;
}

function ideal_panes_social_links_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['facebook'] = array(
    '#type' => 'textfield',
    '#title' => t('Link for facebook'),
    '#default_value' => !empty($conf['facebook']) ? $conf['facebook'] : '',
    '#size' => '20'
  );
    $form['twitter'] = array(
    '#type' => 'textfield',
    '#title' => t('Link for twitter'),
    '#default_value' => !empty($conf['twitter']) ? $conf['twitter'] : '',
    '#size' => '20'
  );
      $form['google'] = array(
    '#type' => 'textfield',
    '#title' => t('Link for google+'),
    '#default_value' => !empty($conf['google']) ? $conf['google'] : '',
    '#size' => '20'
  );
    $form['linkedin'] = array(
    '#type' => 'textfield',
    '#title' => t('Link for linkedin'),
    '#default_value' => !empty($conf['linkedin']) ? $conf['linkedin'] : '',
    '#size' => '20'
  ); 

  return $form;
  
  
}

function ideal_panes_social_links_content_type_edit_form_submit($form, &$form_state) {
 $form_state['conf']['facebook'] = $form_state['values']['facebook'];
 $form_state['conf']['twitter'] = $form_state['values']['twitter'];
 $form_state['conf']['google'] = $form_state['values']['google'];
 $form_state['conf']['linkedin'] = $form_state['values']['linkedin'];

}
