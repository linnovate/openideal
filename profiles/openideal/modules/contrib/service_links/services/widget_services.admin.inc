<?php

/**
 * @file
 * Administrative page callbacks for Widget Services module.
 */

/**
 * Menu callback administration settings for widget options.
 */
function  widget_services_admin() {
  $form['twitter_widget'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Twitter Widget'),
  );
  $form['twitter_widget']['service_links_tw_data_count'] = array(
    '#type' => 'select',
    '#title' => t('Counter'),
    '#default_value' => variable_get('service_links_tw_data_count', 'horizontal'),
    '#options' => array(
      'horizontal' => t('Horizontal'),
      'vertical' => t('Vertical'),
      'none' => t('None'),
    ),
  );
  $form['twitter_widget']['service_links_tw_data_via'] = array(
    '#type' => 'textfield',
    '#field_prefix' => '@',
    '#title' => t('Via user'),
    '#description' => t('Add here your screen name.'),
    '#default_value' => variable_get('service_links_tw_data_via', ''),
    '#size' => 40,
  );
  //data-related
  //lang
  //counturl

  $form['facebook_like'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Facebook Like'),
  );
  $form['facebook_like']['service_links_fl_layout'] = array(
    '#type' => 'select',
    '#title' => t('Layout'),
    '#default_value' => variable_get('service_links_fl_layout', 'button_count'),
    '#options' => array(
      'standard' => t('Standard'),
      'button_count' => t('Button Count'),
      'box_count' => t('Box Count'),
    ),
  );
  $form['facebook_like']['service_links_fl_width'] = array(
    '#title' => t('Width'),
    '#type' => 'textfield',
    '#default_value' => variable_get('service_links_fl_width', 100),
    '#description' => t('A good value for button layout is 100, for box layout is 80 and for standard layout 450'),
    '#size' => 10,
  );
  $form['facebook_like']['service_links_fl_font'] = array(
    '#title' => t('Font'),
    '#type' => 'select',
    '#default_value' => variable_get('service_links_fl_font', ''),
    '#options' => array(
      '' => t('None'),
      'arial' => t('Arial'),
      'lucida grande' => t('Lucida Grande'),
      'segoe ui' => t('Segoe Ui'),
      'tahoma' => t('Tahoma'),
      'trebuchet ms' => t('Trebuchet Ms'),
      'verdana' => t('Verdana'),
    ),
  );
  $form['facebook_like']['service_links_fl_show_faces'] = array(
    '#title' => t('Show Faces'),
    '#type' => 'radios',
    '#default_value' => variable_get('service_links_fl_show_faces', 'false'),
    '#options' => array(
      'true' => t('Yes'),
      'false' => t('No'),
    ),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['facebook_like']['service_links_fl_colorscheme'] = array(
    '#title' => t('Color Scheme'),
    '#type' => 'radios',
    '#default_value' => variable_get('service_links_fl_colorscheme', 'light'),
    '#options' => array(
      'light' => t('Light'),
      'dark' => t('Dark'),
    ),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['facebook_like']['service_links_fl_action'] = array(
    '#title' => t('Action'),
    '#type' => 'radios',
    '#default_value' => variable_get('service_links_fl_action', 'like'),
    '#options' => array(
      'like' => t('Like'),
      'recommended' => t('Recommended'),
    ),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['facebook_like']['service_links_fl_locale'] = array(
    '#title' => t('Locale'),
    '#type' => 'textfield',
    '#default_value' => variable_get('service_links_fl_locale', ''),
    '#description' => t('Enter the language code and the country code separated by an underscore (i.e. es_ES, en_EN, en_US, it_IT, fr_FR, pt_BR, pt_PT)'),
    '#size' => 5,
    '#attributes' => array('class' => array('container-inline')),
  );
 
  $form['facebook_widget'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Facebook Share'),
  );
  $form['facebook_widget']['service_links_fs_type'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#default_value' => variable_get('service_links_fs_type', 'button_count'),
    '#options' => array(
      'button_count' => t('Button Count'),
      'box_count' => t('Box Count'),
      'icon_link' => t('Icon Link'),
    ),
  );
  $form['facebook_widget']['service_links_fs_fix_count'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the counter also if zero'),
    '#default_value' => variable_get('service_links_fs_fix_count', FALSE),
  );

  $form['digg_smart_button'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Digg Button'),
  );
  $form['digg_smart_button']['service_links_dsb_type'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#default_value' => variable_get('service_links_dsb_type', 'DiggCompact'),
    '#options' => array(
      'DiggWide' => t('Wide'),
      'DiggMedium' => t('Medium'),
      'DiggCompact' => t('Compact'),
      'DiggIcon' => t('Icon'),
    ),
  );
  $form['digg_smart_button']['service_links_dsb_related'] = array(
    '#title' => t('Related'),
    '#type' => 'radios',
    '#default_value' => variable_get('service_links_dsb_related', 'yes'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
    '#attributes' => array('class' => array('container-inline')),
  );
  $form['digg_smart_button']['service_links_dsb_style'] = array(
    '#title' => t('Default style'),
    '#type' => 'radios',
    '#default_value' => variable_get('service_links_dsb_style', 'yes'),
    '#options' => array(
      'yes' => t('Yes'),
      'no' => t('No'),
    ),
    '#attributes' => array('class' => array('container-inline')),
  );

  $form['google_plus_one'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Google Plus One'),
  );
  $form['google_plus_one']['service_links_gpo_type'] = array(
    '#title' => t('Type'),
    '#type' => 'select',
    '#default_value' => variable_get('service_links_gpo_type', ''),
    '#options' => array(
      '' => t('Standard'),
      'small' => t('Small'),
      'medium' => t('Medium'),
      'tall' => t('Tall'),
    ),
  );
  $form['google_plus_one']['service_links_gpo_counter'] = array(
    '#title' => t('Show counter'),
    '#type' => 'checkbox',
    '#default_value' => variable_get('service_links_gpo_counter', TRUE),
  );

  $form['linkedin_share_button'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Linkedin Share Button'),
  );
  $form['linkedin_share_button']['service_links_lsb_countmode'] = array(
    '#title' => t('Count Mode'),
    '#type' => 'select',
    '#default_value' => variable_get('service_links_lsb_countmode', 'right'),
    '#options' => array(
      'top' => t('Vertical'),
      'right' => t('Horizontal'),
      '' => t('No Count'),
    ),
  );

  return system_settings_form($form);
}
