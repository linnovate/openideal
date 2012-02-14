<?php
// $Id: fbconnect.admin.inc,v 1.19.2.3 2010/10/30 12:18:35 vectoroc Exp $

/** 
 * @file 
 * Administration page callbacks for the fbconnect module. 
 */ 

function fbconnect_api_keys_settings(&$form_state) {
  $form['fbconnect_appid'] = array( 
    '#type' => 'textfield', 
    '#required' => TRUE,
    '#title' => t('Application ID'), 
    '#default_value' => variable_get('fbconnect_appid', NULL),
    '#description' => t('Your Application ID, <strong> not API Key </strong>'),
  );
  
  $form['fbconnect_skey'] = array(
    '#type' => 'textfield',
    '#required' => TRUE,
    '#title' => t('Application Secret'),
    '#default_value' => variable_get('fbconnect_skey', NULL),
    '#description' => t('Do not share your secret key with anyone'),
  );
  
  $form['fbconnect_language_code'] = array(
    '#type' => 'textfield',
    '#title' => (module_exists('i18n')) ? t('Default language code') : t('Language code'),
    '#description' => t('Enter your country code here to get translated versions of facebook connect. (e.g. en_US or de_DE)'),
    '#default_value' => variable_get('fbconnect_language_code', 'en_US'),
  );
  
  if (module_exists('i18n')) {
    $form['language_codes'] = array(
      '#type'          => 'fieldset',
      '#title'         => t('Automatically Switch Language Code'),
      '#description'   => t('Enable the fbconnect module to use a different language code for each of your site languages.'),
      '#collapsible'   => TRUE,
      '#collapsed'     => TRUE,
    );

    foreach (language_list() as $langcode => $language) {
      $form['language_codes']['fbconnect_language_code_' . $langcode] = array(
        '#type'          => 'select',
        '#title'         => t('@name (@native) Language code', array('@name' => $language->name, '@native' => $language->native)),
        '#default_value' => variable_get('fbconnect_language_code_' . $langcode, ''),
        '#options'       => array('' => t('- Use default -')) + _fbconnect_language_codes(),
      );
    }
  }  
  
  $form['fbconnect_debug'] = array(
    '#type' => 'checkbox',
    '#title' => t('Debug mode'),      
    '#default_value' => variable_get('fbconnect_debug', FALSE),
  );  
  
  $form['fbconnect_connect_url'] = array(
    '#type' => 'item',
    '#title' => t('Connect url'),
    '#description' => t('Copy this value into Facebook Applications on Connect settings tab'),
    '#value' => variable_get('fbconnect_connect_url', $GLOBALS['base_url'] . '/'),
  );
    
//  locale
//  connect_logo_url
//  about_url
//  base_domain
//  tos_url

  return system_settings_form($form);
}

function fbconnect_appearance_settings(&$form_state) {
  $form['site'] = array(
    '#type' => 'fieldset',
    '#title' => t('Settings for !site', array('!site' => variable_get('site_name', t('Your Site')))),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['site']['fbconnect_fast_reg'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fast registration mode'),
    '#default_value' => variable_get('fbconnect_fast_reg', NULL),
    '#description' => t('Enable this mode will bypass drupal\'s standard registration form.'),
  );
  
  $form['site']['fbconnect_loginout_mode'] = array(
    '#type' => 'select',
    '#title' => t('Login/logout mode'),
    '#options' => array(
      'manual' => t('Manual'),
      'ask'    => t('Ask user to logout from FB'),
      'auto'   => t('Auto login/logout'),
    ),
    '#default_value' => variable_get('fbconnect_loginout_mode', FALSE),
    '#description' => t('Login to drupal/logout from facebook. Enable this if you don\'t want to logout automatically from facebook.'),
  );
  
  // TODO: rename to more clear name 
  $form['site']['fbconnect_reg_options'] = array(
    '#type' => 'checkbox',
    '#title' => t('Disable linking accounts during registration'),
    '#default_value' => variable_get('fbconnect_reg_options', 0),
    '#description' => t('Disables the option to link local account to a FB account on registration'),
  );
  
  $form['display'] = array(
    '#title' => t('Display Settings'),
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  
  if (fbconnect_get_config()) {  
    $button = array(
      'small_short'  => fbconnect_render_button(array('size' => 'small' , 'onclick' => 'return false;')),
      'medium_short' => fbconnect_render_button(array('size' => 'medium', 'onclick' => 'return false;')),
      'large_short'  => fbconnect_render_button(array('size' => 'large' , 'onclick' => 'return false;')),
    );

    $form['display']['fbconnect_button'] = array(
      '#type' => 'radios',
      '#default_value' => variable_get('fbconnect_button', 'medium_short'),
      '#options' => $button,
    );
    $form['display']['fbconnect_button_text'] = array(
      '#title' => t('Button text'),
      '#type' => 'textfield',
      '#description' => t('Enter the text you want to display on the Facebook Connect Button.'),
      '#default_value' => variable_get('fbconnect_button_text', 'Connect'),
    );  
  }
  
  $form['display']['fbconnect_pic_allow'] = array(
    '#type' => 'select',
    '#title' => t('Facebook picture settings'),
    '#description' => t('Here you can deside whether you want to allow the use of the facebook picture - or not.'),
    '#default_value' => variable_get('fbconnect_pic_allow', 'allow'),
    '#options' => array(
      'allow' => t('Let the user choose'),
      'disallow' => t('Do not allow the use of facebook user picture at your site'),
    ),
  );
  
  $options_pic_size = array(
    'thumb' => t('Thumb (50px wide)'),
    'small' => t('Small (100px wide)'),
    'normal' => t('Normal (200px wide)'),
    'square' => t('Square (50px by 50px)'),
  );
  
  $form['display']['fbconnect_pic_size'] = array(
    '#type' => 'select',
    '#title' => t('Default Facebook picture size'),
    '#description' => t('This will set the default user picture size throughout the site. If your user decides to use his facebook picture, choose its size here.'),
    '#default_value' => variable_get('fbconnect_pic_size', 'square'),
    '#options' => $options_pic_size,
  );
  
  $form['display']['fbconnect_pic_size_nodes'] = array(
    '#type' => 'select',
    '#title' => t('Node Facebook picture size'),
    '#description' => t('This will set the picture size when viewing a node post. If your user decides to use his facebook picture, choose its size here.'),
    '#default_value' => variable_get('fbconnect_pic_size_nodes', 'square'),
    '#options' => $options_pic_size,
  );
  $form['display']['fbconnect_pic_size_comments'] = array(
    '#type' => 'select',
    '#title' => t('Comments Facebook picture size'),
    '#description' => t('This will set the picture size when viewing a comment post. If your user decides to use his facebook picture, choose its size here.'),
    '#default_value' => variable_get('fbconnect_pic_size_comments', 'square'),
    '#options' => $options_pic_size,
  );
  $form['display']['fbconnect_pic_size_profile'] = array(
    '#type' => 'select',
    '#title' => t('Profile Facebook picture size'),
    '#description' => t('This will set the picture size when viewing a user\'s profile page. If your user decides to use his facebook picture, choose its size here.'),
    '#default_value' => variable_get('fbconnect_pic_size_profile', 'small'),
    '#options' => $options_pic_size,
  );  

  $form['display']['fbconnect_pic_logo'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Facebook Logo'),
    '#description' => t('Uncheck this if you don\'t want to display the facebook logo on the user picture.'),
    '#default_value' => variable_get('fbconnect_pic_logo', TRUE),
  );

  return system_settings_form($form);
}


function fbconnect_fbapp_settings(&$form_state) {
  if (!facebook_client()) {
    drupal_set_message(t('Ensure that you entered valid api keys.'), 'error');
    drupal_goto('admin/settings/fbconnect');
  }
  
  if (!fbconnect_get_fbuid()) {
    $button_attr = array('text' => t('Connect with your Facebook account'));
    $form['fbconnect'] = array(
      '#value' => fbconnect_render_button($button_attr),
    );
    
    return $form;
  }
  
  $form['fbconnect_base_domaine'] = array(
    '#type' => 'textfield',
    '#title' => t('Base Domain Url'),
    '#description' => t('Use this to enable your Facebook Connect implementation to span multiple subdomains (e.g., using example.com would enable  www.example.com, foo.example.com or bar.example.com,).'),
    '#default_value' => variable_get('fbconnect_base_domaine', NULL),
  );
  
  $form['fbconnect_connect_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Connect Url'),
    '#description' => t('Your site\'s main URL.'),
    '#default_value' => variable_get('fbconnect_connect_url', $GLOBALS['base_url'] . '/'),
  );
  
  $form['fbconnect_uninstall_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Post-Remove Callback Url'),
    '#description' => t('Facebook pings this URL when a user removes your application.'),
    '#default_value' => variable_get(
      'fbconnect_uninstall_url',
      url('fbconnect/post-remove', array('absolute' => TRUE))
    ),
  );
  
  $form = system_settings_form($form);
  $form['#submit'][] = 'fbconnect_fbapp_settings_submit';
  return $form;
}

function fbconnect_fbapp_settings_submit($form, &$form_state) {
  $properties = array(
    'connect_url'   => $form_state['values']['fbconnect_connect_url'],
    'base_domain'   => $form_state['values']['fbconnect_base_domaine'],
    'uninstall_url' => $form_state['values']['fbconnect_uninstall_url'],
  );
  
  if (!facebook_client()) {
    drupal_set_message(t('Unable to get a facebook client, check your api key'), 'error');
    return FALSE;  
  }
  if (!fbconnect_get_fbuid()) {
    drupal_set_message(t('Unable to autoconfigure your application settings, make sure you are logged into Facebook'), 'warning');
    return FALSE;
  }
  
  try {
    $res = facebook_client()->api(array(
      'method' => 'admin.setAppProperties',
      'properties' => array_filter($properties),
    ));
  }
  catch (Exception $e) {
    drupal_set_message(t('Exception thrown while using admin_setAppProperties : @code', array('@code' => $e->getMessage())), 'error');
  }
  if (!$res) {
    drupal_set_message(t('Checking your facebook session, you are not allowed to change this application settings'), 'error');
  }
  else {
    drupal_set_message(t('Your facebook app settings has been updated'));      
  }
}

/**
 * Returns a list of valid Facebook language codes.
 *
 * see: http://wiki.developers.facebook.com/index.php/Facebook_Locales
 */
function _fbconnect_language_codes() {
  return array(
    'af_ZA' => 'Afrikaans',
    'ar_AR' => 'Arabic',
    'ay_BO' => 'Aymara',
    'az_AZ' => 'Azeri',
    'be_BY' => 'Belarusian',
    'bg_BG' => 'Bulgarian',
    'bn_IN' => 'Bengali',
    'bs_BA' => 'Bosnian',
    'ca_ES' => 'Catalan',
    'ck_US' => 'Cherokee',
    'cs_CZ' => 'Czech',
    'cy_GB' => 'Welsh',
    'da_DK' => 'Danish',
    'de_DE' => 'German',
    'el_GR' => 'Greek',
    'en_GB' => 'English (UK)',
    'en_PI' => 'English (Pirate)',
    'en_UD' => 'English (Upside Down)',
    'en_US' => 'English (US)',
    'eo_EO' => 'Esperanto',
    'es_CL' => 'Spanish (Chile)',
    'es_CO' => 'Spanish (Colombia)',
    'es_ES' => 'Spanish (Spain)',
    'es_LA' => 'Spanish',
    'es_MX' => 'Spanish (Mexico)',
    'es_VE' => 'Spanish (Venezuela)',
    'et_EE' => 'Estonian',
    'eu_ES' => 'Basque',
    'fa_IR' => 'Persian',
    'fb_FI' => 'Finnish (test)',
    'fb_LT' => 'Leet Speak',
    'fi_FI' => 'Finnish',
    'fo_FO' => 'Faroese',
    'fr_CA' => 'French (Canada)',
    'fr_FR' => 'French (France)',
    'ga_IE' => 'Irish',
    'gl_ES' => 'Galician',
    'gn_PY' => 'Guaraní',
    'gu_IN' => 'Gujarati',
    'he_IL' => 'Hebrew',
    'hi_IN' => 'Hindi',
    'hr_HR' => 'Croatian',
    'hu_HU' => 'Hungarian',
    'hy_AM' => 'Armenian',
    'id_ID' => 'Indonesian',
    'is_IS' => 'Icelandic',
    'it_IT' => 'Italian',
    'ja_JP' => 'Japanese',
    'jv_ID' => 'Javanese',
    'ka_GE' => 'Georgian',
    'kk_KZ' => 'Kazakh',
    'km_KH' => 'Khmer',
    'kn_IN' => 'Kannada',
    'ko_KR' => 'Korean',
    'ku_TR' => 'Kurdish',
    'la_VA' => 'Latin',
    'li_NL' => 'Limburgish',
    'lt_LT' => 'Lithuanian',
    'lv_LV' => 'Latvian',
    'mg_MG' => 'Malagasy',
    'mk_MK' => 'Macedonian',
    'ml_IN' => 'Malayalam',
    'mn_MN' => 'Mongolian',
    'mr_IN' => 'Marathi',
    'ms_MY' => 'Malay',
    'mt_MT' => 'Maltese',
    'nb_NO' => 'Norwegian (bokmal)',
    'ne_NP' => 'Nepali',
    'nl_BE' => 'Dutch (België)',
    'nl_NL' => 'Dutch',
    'nn_NO' => 'Norwegian (nynorsk)',
    'pa_IN' => 'Punjabi',
    'pl_PL' => 'Polish',
    'ps_AF' => 'Pashto',
    'pt_BR' => 'Portuguese (Brazil)',
    'pt_PT' => 'Portuguese (Portugal)',
    'qu_PE' => 'Quechua',
    'rm_CH' => 'Romansh',
    'ro_RO' => 'Romanian',
    'ru_RU' => 'Russian',
    'sa_IN' => 'Sanskrit',
    'se_NO' => 'Northern Sámi',
    'sk_SK' => 'Slovak',
    'sl_SI' => 'Slovenian',
    'so_SO' => 'Somali',
    'sq_AL' => 'Albanian',
    'sr_RS' => 'Serbian',
    'sv_SE' => 'Swedish',
    'sw_KE' => 'Swahili',
    'sy_SY' => 'Syriac',
    'ta_IN' => 'Tamil',
    'te_IN' => 'Telugu',
    'tg_TJ' => 'Tajik',
    'th_TH' => 'Thai',
    'tl_PH' => 'Filipino',
    'tl_ST' => 'Klingon',
    'tr_TR' => 'Turkish',
    'tt_RU' => 'Tatar',
    'uk_UA' => 'Ukrainian',
    'ur_PK' => 'Urdu',
    'uz_UZ' => 'Uzbek',
    'vi_VN' => 'Vietnamese',
    'xh_ZA' => 'Xhosa',
    'yi_DE' => 'Yiddish',
    'zh_CN' => 'Simplified Chinese (China)',
    'zh_HK' => 'Traditional Chinese (Hong Kong)',
    'zh_TW' => 'Traditional Chinese (Taiwan)',
    'zu_ZA' => 'Zulu',
  );
}