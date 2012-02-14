<?php
// $Id$

/**
 * @file
 * Admin functions for fblikebutton.
 */

/**
 * Configure which node types can be "liked" by users.
 */
function fblikebutton_admin_settings() {
  global $language;
  $lang_abbr = $language->language;
  $fblikebutton_lang = $lang_abbr . '_' . drupal_strtoupper($lang_abbr);
  $fblikebutton_node_options = node_get_types('names');
  $form['fblikebutton_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Display the Like button on these content types'),
    '#options' => $fblikebutton_node_options,
    '#default_value' => variable_get('fblikebutton_node_types', array('page')),
    '#description' => t('Each of these content types will have the "like" button automatically added to them.'),
  );
  $form['fblikebutton_showonteasers'] = array(
    '#type' => 'radios',
    '#title' => t('Display on teasers'),
    '#options' => array(t('No'), t('Yes')),
    '#default_value' => variable_get('fblikebutton_showonteasers', 0),
    '#description' => t('If <em>Yes</em> is selected, the button will appear even when the node being viewed is a teaser. Otherwise it will only appear when the full node is being viewed.'),
  );
  $form['fblikebutton_iframe_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Set the iframe height (px)'),
    '#default_value' => variable_get('fblikebutton_iframe_height', '80'),
    '#description' => t('Height of the iframe, in pixels. Default is 80. <em>Note: lower values may crop the output.</em>'),
  );
  $form['fblikebutton_iframe_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Set the iframe width (px)'),
    '#default_value' => variable_get('fblikebutton_iframe_width', '450'),
    '#description' => t('Width of the iframe, in pixels. Default is 450. <em>Note: lower values may crop the output.</em>'),
  );
  $form['fblikebutton_iframe_css'] = array(
    '#type' => 'textfield',
    '#title' => t('Extra css styling needed'),
    '#default_value' => variable_get('fblikebutton_iframe_css', ''),
    '#description' => t('Extra css attributes needed to make the iframe behave for your specific requirements. Will not necessarily overwrite existing styling. To alter the dimensions of the iframe, use the height and width fields found above.<br/>Example: <em>float: right; padding: 5px;</em>'),
  );
  $form['fblikebutton_layout'] = array(
    '#type' => 'select',
    '#title' => t('Layout style'),
    '#options' => array('standard' => t('Standard'), 'box_count' => t('Box Count'), 'button_count' => t('Button Count')),
    '#default_value' => variable_get('fblikebutton_layout', 'standard'),
    '#description' => t('Determines the size and amount of social context next to the button.'),
  );
  $form['fblikebutton_show_faces'] = array(
    '#type' => 'select',
    '#title' => t('Display faces in the box'),
    '#options' => array('true' => t('Show faces'), 'false' => t('Do not show faces')),
    '#default_value' => variable_get('fblikebutton_show_faces', 'true'),
    '#description' => t('Show profile pictures below the button. Only works if <em>Layout style</em> found above is set to <em>Standard</em> (otherwise, value is ignored).'),
  );
  $form['fblikebutton_action'] = array(
    '#type' => 'select',
    '#title' => t('Verb to display'),
    '#options' => array('like' => t('Like'), 'recommend' => t('Recommend')),
    '#default_value' => variable_get('fblikebutton_action', 'like'),
    '#description' => t('The verbiage to display inside the button itself.'),
  );
  $form['fblikebutton_displaysend'] = array(
    '#type' => 'select',
    '#title' => t('Display "send" option'),
    '#options' => array('true' => t('Display'), 'false' => t('Do not display')),
    '#default_value' => variable_get('fblikebutton_displaysend', 'true'),
    '#description' => t('Optionally display the <em>Send</em> button next to the <em>Like/Recommend</em> box.'),
  );
  $form['fblikebutton_font'] = array(
    '#type' => 'select',
    '#title' => t('Font'),
    '#options' => array('arial' => 'Arial',
                'lucida+grande' => 'Lucida Grande',
                'segoe+ui' => 'Segoe UI',
                'tahoma' => 'Tahoma',
                'trebuchet+ms' => 'Trebuchet MS',
                'verdana' => 'Verdana'),
    '#default_value' => variable_get('fblikebutton_font', 'arial'),
    '#description' => t('The font with which to display the text of the button.'),
  );
  $form['fblikebutton_color_scheme'] = array(
    '#type' => 'select',
    '#title' => t('Color scheme'),
    '#options' => array('light' => t('Light'), 'dark' => t('Dark')),
    '#default_value' => variable_get('fblikebutton_color_scheme', 'light'),
    '#description' => t('The color scheme of the box environtment.'),
  );
  $form['fblikebutton_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight'),
    '#default_value' => variable_get('fblikebutton_weight', '50'),
    '#description' => t('The weight determines where on a node the like button will appear. The larger the weight, the lower it will appear on the node. For example, if you want the button to appear more toward the top of the node, choose <em>-40</em> as opposed to <em>-39, -38, 0, 1,</em> or <em>50,</em> etc.'),
  );
  $form['fblikebutton_language'] = array(
    '#type' => 'textfield',
    '#title' => t('Language'),
    '#default_value' => variable_get('fblikebutton_language', $fblikebutton_lang),
    '#description' => t('Specific language to use. Default for this site is <em>@lang</em>. Examples:<br/>French (France): <em>fr_FR</em><br/>French (Canada): <em>fr_CA</em>', array('@lang' => $fblikebutton_lang)),
  );
  return system_settings_form($form);
}
