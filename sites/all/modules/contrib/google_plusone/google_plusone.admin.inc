<?php

/**
 * @file
 * Administration page for the Google +1 Button.
 */

/**
 * Menu callback: displays the Google Plus One module settings page.
 *
 * @ingroup forms
 */
function google_plusone_admin_settings() {

  // Custom javascript code to preview in real-time the button
  drupal_add_js(drupal_get_path('module', 'google_plusone') . '/google_plusone.admin.js');
  // not good performance.. but anyway, it's only for admin page and can help for debugging.
  $script = '<script type="text/javascript" src="http://apis.google.com/js/plusone.js">';
  drupal_set_html_head('<script type="text/javascript" src="http://apis.google.com/js/plusone.js"></script>');

  $locations = array(
    'full' => t('Full node'),
    'teaser' => t('Teaser'),
    'link' => t('Node links'),
  );
  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Settings'),
    '#description' => t('These are settings for nodes: the button will be created dynamically for each node using its URL. <br />In the other hand, if you only need a block with a fixed URL like your homepage, go to the <a href="@blocks">blocks</a> page to enable and configure the <em>Google +1 Button</em> block.', array('@blocks' => url('admin/build/block'))),
  );
  $form['display']['google_plusone_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display the button on these content types:'),
    '#options' => node_get_types('names'),
    '#default_value' => variable_get('google_plusone_node_types', array()),
  );
  $form['display']['google_plusone_node_location'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display the button in these view modes:'),
    '#options' => $locations,
    '#default_value' => variable_get('google_plusone_node_location', array('full')),
  );
  $form['display']['google_plusone_weight'] = array(
     '#type' => 'weight',
     '#title' => t('Weight'),
     '#delta' => 50,
     '#default_value' => variable_get('google_plusone_weight', '-10'),
     '#description' => t('Heavier items will sink. The default weight -10 will show the button at the top of the node content.'),
  );


  $available_sizes = array(
    'small' => t('Small (15px)'),
    'medium' => t('Medium (20px)'),
    '' => t('Standard (24px)'),
    'tall' => t('Tall (60px)'),
  );
  $default = array(
    'count' => 0,
    'size' => '',  // standard
    'css' => 'margin: 0 1em 1em 1em;float:right',
    'syntax' => 'g:plusone',
  );
  $button_settings = variable_get('google_plusone_button_settings', $default);
  $form['google_plusone_button_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Button Settings'),
    '#tree' => TRUE, // All the options in this fieldset will be grouped in 1 single variable.
  );
  $form['google_plusone_button_settings']['count'] = array(
    '#type' => 'radios',
    '#title' => t('Include count?'),
    '#options' => array(t('Yes'), t('No')),
    '#default_value' => $button_settings['count'],
    '#description' => google_plusone_build_preview_button($available_sizes),
  );
  $form['google_plusone_button_settings']['size'] = array(
    '#type' => 'radios',
    '#title' => t('Size'),
    '#options' => $available_sizes,
    '#default_value' => $button_settings['size'],
  );
  $form['google_plusone_button_settings']['css'] = array(
    '#type' => 'textfield',
    '#title' => t('Optional wrapper with CSS'),
    '#maxlength' => 256,
    '#default_value' => $button_settings['css'],
    '#description' => t('To help with the layout and placement of the button, it will be rendered by default inside a wrapper: <br />	&lt;div class="g-plusone-wrapper"&gt;	&lt;/div&gt;<br/>You can enter CSS rules to style this wrapper. By default <em>margin: 0 1em 1em 1em;float:right</em><br />To disable totally the wrapper, input the word <em>nowrapper</em>'),
  );
  $form['google_plusone_button_settings']['syntax'] = array(
    '#type' => 'radios',
    '#title' => t('Tag syntax'),
    '#options' => array('HTML5' => t('HTML5'), 'g:plusone' => t('g:plusone')),
    '#default_value' => $button_settings['syntax'],
    '#description' => t('Two different but equivalent formats are allowed for the +1 tag. By default: g:plusone'),
  );


  $default_advanced = array(
      'lang' => '',
      'scope' => 'page_bottom_docwrite',
      'parsetags' => 'onload',
  );
  $config = variable_get('google_plusone_advanced_settings', $default_advanced);
  $form['google_plusone_advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
    '#collapsed' => TRUE,
    '#tree' => TRUE, // All the options in this fieldset will be grouped in 1 single variable.
    '#collapsible' => TRUE,
    '#description' => t('Get more information about these options from the documentation of the <a href="@api-doc">Google +1 API</a>.', array('@api-doc' => 'http://code.google.com/apis/+1button/')),
  );
  $form['google_plusone_advanced_settings']['lang'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#default_value' => $config['lang'],
    '#description' => t('Input the language code you want from <a href="@api-doc">this list</a>. Leave empty to use the default <em>en-Us</em> language.<br/>If your Drupal site has multiple languages enabled (using Locale module), then input <strong>the Google +1 language codes</strong> separated by commas. For example: <em>pt-br,fr,ru</em>', array('@api-doc' => 'http://code.google.com/apis/+1button/#languages')),
  );
  $form['google_plusone_advanced_settings']['scope'] = array(
    '#type' => 'radios',
    '#title' => t('Scope'),
    '#options' => array('page_bottom_docwrite' => t('Footer'), 'header' => t('Header')),
    '#default_value' => $config['scope'],
    '#description' => t('Location of the script file. By default: <em>Footer</em>, for performance reasons.'),
  );
  $form['google_plusone_advanced_settings']['parsetags'] = array(
    '#type' => 'radios',
    '#title' => t('Parse tags'),
    '#options' => array('explicit' => t('explicit'), 'onload' => t('onload')),
    '#default_value' => $config['parsetags'],
    '#description' => t('Buttons will be rendered immediately upon page load, or explicitly called. By default <em>onload</em>.'),
  );
  return system_settings_form($form);
}


/**
 * Returns HTML for a preview button in the admin settings page.
 */
function google_plusone_build_preview_button($sizes) {
  $output = '';
  $output .= '<div id="google_plusone_preview" style="display:none;">';
  $output .= '<div style="float: right;padding:2em;margin: 0 3em 0 3em;font-size:1.2em;border: thin groove #999;">' . t('Preview of the Google +1 button:') . '<br /><br />';
  foreach ($sizes as $size => $value) {
    $output .= '<div id="google_plusone_' . $size . '" style="display:none">';
    $output .= '<div class="g-plusone" href="http://drupal.org" data-size="' . $size . '" data-count="false"></div>';
    $output .= '<br /><br clear="all" />' . t('With counting included:') . '<br /><br />';
    $output .= '<div class="g-plusone" href="http://drupal.org" data-size="' . $size . '" data-count="true"></div>';
    $output .= '</div>';
  }
  $output .= '</div></div>';
  return $output;
}

/*
 * Trim and polish text input fields
 */
function google_plusone_admin_settings_validate($form, &$form_state) {
  $form_state['values']['google_plusone_button_settings']['css'] = google_plusone_trim($form_state['values']['google_plusone_button_settings']['css'], ';');
  $form_state['values']['google_plusone_advanced_settings']['lang'] = drupal_strtolower(trim($form_state['values']['google_plusone_advanced_settings']['lang']));
}

/**
 * Helper function to trim whitespace and make sure to remove
 * the last character if it is a delimiter.
 *
 * @param $list
 *   A string representing a list of items
 * @param $delimiter
 *   A string that contains a delimiter.
 * @return
 *   A whitespace-trimmed string
 *
 */
function google_plusone_trim($list, $delimiter) {
  $list = trim(check_plain($list));
  if (!empty($list)) {
    if (drupal_substr($list, -1) === $delimiter) {
      $list = drupal_substr($list, 0, -1);
    }
  }
  return $list;
}
