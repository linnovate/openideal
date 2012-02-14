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
function google_plusone_admin_settings($form) {

  // Custom javascript code to preview in real-time the button
  drupal_add_js(drupal_get_path('module', 'google_plusone') . '/google_plusone.admin.js');
  drupal_add_js('https://apis.google.com/js/plusone.js', 'external');

  $locations = array();
  $entity_info = entity_get_info('node');
  $view_modes = $entity_info['view modes'];
  foreach ($view_modes as $name => $info) {
    $locations[$name] = $info['label'];
  }

  $form['display'] = array(
    '#type' => 'fieldset',
    '#title' => t('Display Settings for nodes'),
    '#description' => t('These settings will render a button <strong>for each node</strong>, using its URL. If you only need a block with a fixed URL like your homepage, or dynamic URL for generic pages, better go to the <a href="@blocks">blocks</a> page to enable and configure the <em>Google +1 Button</em> block.', array('@blocks' => url('admin/structure/block'))),
  );
  $form['display']['google_plusone_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display the button on these content types:'),
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('google_plusone_node_types', array()),
  );
  $form['display']['google_plusone_node_location'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display the button in these view modes:'),
    '#options' => $locations + array('link' => t('Node links')),
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
  $available_annotations = array(
    'none' => t('None'),
    'bubble' => t('Bubble (by default)'),
    'inline' => t('Inline'),
  );
  $default = array(
    'annotation' => 'bubble',
    'width' => '250',
    'size' => '',  // standard
    'css' => 'margin: 0 1em 1em 1em;float:right',
    'syntax' => 'g:plusone',
    'alias' => 'aliased'
  );
  $button_settings = array_merge($default, variable_get('google_plusone_button_settings', array()));
  $form['google_plusone_button_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Button Settings for nodes'),
    '#tree' => TRUE, // All the options in this fieldset will be grouped in 1 single variable.
    '#description' => google_plusone_build_preview_button($available_sizes),
  );
  $form['google_plusone_button_settings']['annotation'] = array(
    '#type' => 'radios',
    '#title' => t('Annotation: How to show the counting?'),
    '#options' => $available_annotations,
    '#default_value' => $button_settings['annotation'],
  );
  $form['google_plusone_button_settings']['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width (only affects to inline annotation)'),
    '#default_value' => $button_settings['width'],
    '#size' => 20,
    '#description' => t('By default recommended 250 (pixels). Minimum 120'),
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
    '#description' => t('The button is wrapped in a div	&lt;div class="g-plusone-wrapper"&gt;[button]&lt;/div&gt;<br/>. Here you can enter CSS rules to style this wrapper. By default <em>margin: 0 1em 1em 1em;float:right</em><br />To disable totally the wrapper, input the word <em>nowrapper</em>'),
  );
  $form['google_plusone_button_settings']['syntax'] = array(
    '#type' => 'radios',
    '#title' => t('Tag syntax'),
    '#options' => array('HTML5' => t('HTML5'), 'g:plusone' => t('g:plusone')),
    '#default_value' => $button_settings['syntax'],
    '#description' => t('Two different but equivalent formats are allowed for the +1 tag. By default: g:plusone'),
  );
  $form['google_plusone_button_settings']['alias'] = array(
    '#type' => 'radios',
    '#title' => t('Aliased path'),
    '#default_value' => $button_settings['alias'],
    '#options' => array('not_aliased' => t('Not aliased'), 'aliased' => t('Aliased')),
    '#description' => t('By default <em>Aliased</em>. If you change this setting, <strong>be aware</strong> that Google+ will see them as different URLs, so the button will not keep the previous counting.'),
  );

  $default_advanced = array(
   'lang' => '',
   'scope' => 'page_bottom',
   'parsetags' => 'onload',
   'async' => 1,
  );
  $config = array_merge($default_advanced, variable_get('google_plusone_advanced_settings', $default_advanced));
  $form['google_plusone_advanced_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced Settings'),
    '#collapsed' => TRUE,
    '#tree' => TRUE, // All the options in this fieldset will be grouped in 1 single variable.
    '#collapsible' => TRUE,
    '#description' => t('Get more information about these options from the documentation of the <a href="@api-doc">Google +1 API</a>.', array('@api-doc' => 'https://developers.google.com/+/plugins/+1button/')),
  );
  $form['google_plusone_advanced_settings']['lang'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#default_value' => $config['lang'],
    '#description' => t('Input the language code you want from <a href="@api-doc">this list</a>. Leave empty to use the default <em>en-Us</em> language.<br/>If, and only if, you have a multi-lingual site using Locale module, then you can input a list of <strong>Google +1 language codes</strong> separated by commas. For example: <em>pt-br,fr,ru</em>', array('@api-doc' => 'https://developers.google.com/+/plugins/+1button/#available-languages')),
  );
  $form['google_plusone_advanced_settings']['scope'] = array(
    '#type' => 'radios',
    '#title' => t('Scope'),
    '#options' => array('page_bottom' => t('page_bottom'), 'header' => t('header')),
    '#default_value' => $config['scope'],
    '#description' => t('Location of the script file. By default: <em>page_bottom</em>, for performance reasons. Your theme <strong>must</strong> have the same region name.'),
  );
  $form['google_plusone_advanced_settings']['async'] = array(
    '#type' => 'checkbox',
    '#title' => t('Load JavaScript asynchronously'),
    '#default_value' => $config['async'],
    '#description' => t('By default enabled. If enabled, the Google+1 JavaScript will be loaded asynchronously, preventing that the browser gets blocked (waiting for the JavaScript load) in the pages where Google+1 button is present.'),
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
  $output = '<div id="google_plusone_preview" style="display:none;">';
  $output .= '<div style="float: right;padding:2em;margin: 0 3em 0 3em;font-size:1.2em;border: thin groove #999;">' . t('Real time preview (annotation, width and size) for http://drupal.org') . '<br /><br />';
  foreach ($sizes as $size => $value) {
    $output .= '<div id="google_plusone_' . $size . '" style="display:none">';
    $output .= '<br clear="all" />' . t('With no annotation:') . '<br /><br />';
    $output .= '<div class="g-plusone" data-href="http://drupal.org" data-size="' . $size . '" data-annotation="none"></div>';
    $output .= '<br /><br clear="all" />' . t('With bubble annotation:') . '<br /><br />';
    $output .= '<div class="g-plusone" data-href="http://drupal.org" data-size="' . $size . '" data-annotation="bubble"></div>';
    $output .= '<br /><br clear="all" />' . t('With inline annotation:') . '<br /><br /><div class="g-inline">';
    $output .= '<div class="g-plusone" data-href="http://drupal.org" data-size="' . $size . '" data-annotation="inline"></div>';
    $output .= '</div></div>';
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
