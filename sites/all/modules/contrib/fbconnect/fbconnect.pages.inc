<?php
// $Id: fbconnect.pages.inc,v 1.22.4.9 2010/11/02 22:17:15 vectoroc Exp $

/** 
 * @file 
 * User pages callbacks for the fbconnect module. 
 */ 

/**
 * Menu callback.
 * Called when user perform facebook registration
 */
function fbconnect_prompt_page() {
  $links = fbconnect_prompt_page_links();
  $output = array();
  foreach ($links as $link) {
    array_push($output, theme('box', $link));
  }

  return join("\n", $output);   
}

function fbconnect_prompt_page_links() {
  $conf = fbconnect_get_config();
  $params = array(
    '!site_name' => $conf['invite_name'],
  );
  $reg_msg  = t('Click here to create a new !site_name account with Facebook', $params);
  $link_msg = t('Click here to connect your existing !site_name account with Facebook', $params);

  $reg_attr = array();
  if ($conf['fast_reg_mode']) {
    $reg_attr = array('attributes' => array(
      'onclick' => 'Drupal.fbconnect.DoFastRegistration(this); return false;'
    ));
  }
  
  $reg_choice  = l($reg_msg, 'fbconnect/register/create', $reg_attr);
  $link_choice = l($link_msg, 'user', array('query' => 'destination=fbconnect/link'));
  
  return array($reg_choice, $link_choice);
}

/**
 * Menu callback.
 * Called when user perform facebook registration
 */
function fbconnect_register_page($form_state = array()) {
  $conf  = fbconnect_get_config();
  $data  = fbconnect_get_user_info('name, email');    
  $form  = drupal_retrieve_form('user_register', $form_state);
  $site  = $conf['invite_name'];
  
  // #758918 : prevent users registration if admin has blocked free registration
  $user_reg_mode = variable_get('user_register', 1);
  if ($user_reg_mode == 0) {
    drupal_access_denied();
    return;
  }
  
  drupal_prepare_form('user_register', $form, $form_state);   
  
  $form['name']['#default_value'] = $data['name'];

  $form['fbconnect'] = array(
    '#type' => 'fieldset',
    '#title' => t('Facebook Connect'),
  );

  $form['fbconnect']['fb_visible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Let my Facebook friends see me on @sitename', array('@sitename' => $site)),
    '#default_value' => 1,
  );
  
  if (variable_get('user_pictures', 0) && $conf['user_pictures'] == 'allow') {
    $form['fbconnect']['fb_avatar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use my Facebook picture as user picture'),
      '#description' => t('Your picture will be loaded from facebook.'),
      '#default_value' => 1,
    );  
  }
  $form['#redirect'] = isset($_REQUEST['destination']) ? $_REQUEST['destination'] : '';
  $form['#submit'][] = 'fbconnect_register_form_submit';  

  // Fast registration mode, we by pass std drupal reg form.
  if ($conf['fast_reg_mode']) {
    $form_state['values']['name']       = $data['name'];
    $form_state['values']['mail']       = $data['email'];
    $form_state['values']['pass']       = user_password();
    $form_state['values']['status']     = 1;
    $form_state['values']['fb_visible'] = 1;
    $form_state['values']['fb_avatar']  = 1;
    
    drupal_validate_form('user_register', $form, $form_state);
    $success = !form_get_errors();
    if (!$success) {
      $dest = array('query' => 'destination=fbconnect/link');
      drupal_set_message(t('Facebook connect registration failed for the reasons listed. You may register now, or if you already have an account you can <a href="@login">log in</a> now and link your account', array('@login' => url('user/login', $dest))), 'warning');
      // Append form validation errors below the above warning.
      $messages = drupal_get_messages('error');
      foreach ($messages['error'] as $message) {
        drupal_set_message( $message, 'error');
      }
      
      drupal_goto('user/register', $dest['query']);
    }
    
    fbconnect_register_form_submit($form, $form_state);
    drupal_goto();
  }
    
  return $form;
}

function fbconnect_register_form_submit($form, &$form_state) {
  $fbuid = fbconnect_get_fbuid();
  if (_is_fbconnect_user($fbuid)) {
    $msg = t("Retrying registration. Your Facebook account already assigned to one account on our site");
    drupal_set_message($msg, 'error');
    watchdog('fbconnect', 'Error creating account for @fbuid', array('@fbuid' => $fbuid), WATCHDOG_ALERT);
    drupal_goto();
  }
   
  if (variable_get('fbconnect_fast_reg', NULL)) {
    $user = user_save(NULL, $form_state['values']);
  }
  else {
    $user = $form_state['user'];    
  }
  
  if (!$user) {
    drupal_set_message(t("Error saving user account."), 'error');
    drupal_goto();
  }
  
  fbconnect_register($user->uid, $fbuid);  
  $data['fb_avatar']  = $form_state['values']['fb_avatar'];
  $data['fb_visible'] = $form_state['values']['fb_visible'];
  
  user_save($user, $data);   
  user_external_login($user);
  $_SESSION['fbconnect_feed']['type'] = 'register';
}

/**
 * Menu callback fbconnect identities
 */
function fbconnect_user_identities($account) {
  $conf  = fbconnect_get_config();
  $fbuid = fbconnect_get_fbuid();
  $uid   = _is_fbconnect_user($fbuid);
  
  drupal_set_title(check_plain($account->name));
  
  if ($account->fbuid) { 
    return drupal_get_form('fbconnect_user_settings_form', $account);
  }
  elseif ($uid && $account->uid != $uid) {
    return theme('fbconnect_user_profile_tab_connected', $account, $conf, $uid);
  }
  else {
    return theme('fbconnect_user_profile_tab_unconnected', $account, $conf, $uid);
  }
}

/**
 *  This form allows the user to manage their facebook connect settings.
 *
 *  Some options are visible only if facebook session is active.
 */
function fbconnect_user_settings_form($form_state, $account) {
  $conf = fbconnect_get_config();
  // These fields manage the visibility of the user, 
  // if the option is enabled, user's facebook friends 
  // will be able to see his presence on this site
  $form['fb_visible'] = array(
    '#type' => 'checkbox',
    '#title' => t('Let my Facebook friends see me on @site', array('@site' => $conf['invite_name'])),
    '#description' => t('My Facebook friends will be able to see that I own an account on this website.'),
    '#default_value' => $account->fb_visible,
  );

  // Whether user picture is enabled, 
  // users can choose to use his facebook avatar as avatar
  if ($conf['user_pictures'] == 'allow') {
    $form['fb_avatar'] = array(
      '#type' => 'checkbox',
      '#title' => t('Use my Facebook picture as user picture'),
      '#default_value' => $account->fb_avatar,
    );
  }
  
  if (!variable_get('user_pictures', 0)) {
    $form['fb_avatar']['#attributes'] = array(
      'disabled' => 'disabled',
    );
    $form['fb_avatar']['#description'] = t('Enable user picture support in !link', array(
      '!link' => l(t('User settings'), 'admin/user/settings'),
    ));    
  }
  
  $form['account'] = array(
    '#type'  => 'value',
    '#value' => $account,
  ); 
  
  $form['fbconnect'] = array(
    '#title' => t('Facebook account'),
    '#type' => 'fieldset',
    '#collapsed' => TRUE,
    '#collapsible' => TRUE,
  );
  
  $form['fbconnect']['profile'] = array(
    '#type' =>  'item',
    '#title' => t('Linked to this Facebook account'),
    '#value' => '<fb:name uid="'. $account->fbuid .'" useyou="false" linked="true"></fb:name>',
  );
  
  $form['fbconnect']['unlink'] = array(
    '#type' => 'submit',
    '#description' => t('Click here to unlink this Facebook account'),
    '#value' => t('Unlink'),
    '#access' => !!facebook_client(), // don't show unlink button if fbconnect is'nt properly configured
  ); 
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save settings'),
  );
  return $form;
}

/**
 * Handle post-validation user_settingsForm submission. 
 */
function fbconnect_user_settings_form_submit($form, &$form_state) {
  $account = $form_state['values']['account']; 
  $visible = $form_state['values']['visibility'];
  $avatar  = $form_state['values']['fb_avatar'];
  $op      = $form_state['values']['op'];
  $fbuid   = $account->fbuid;
  
  $data['fb_visible'] = $form_state['values']['fb_visible'];
  $data['fb_avatar']  = $form_state['values']['fb_avatar'];
  
  if ($op == $form['fbconnect']['unlink']['#value']) {
    if (fbconnect_unregister($fbuid)) {
      $data['fb_visible'] = NULL;
      $data['fb_avatar'] = NULL;      
    }
    else {
      form_set_error('fbconnect', t("Something went wrong. Can't unlink account"));
    }
  }
  
  if ($data) {
    user_save($account, $data);
  }
  drupal_set_message(t('Your configuration options have been saved.'));
}

/**
 * Make rendering of facebook user picture themeable
 */
function theme_fbconnect_user_picture_override($fbuid = '', $account = NULL, $user_url = '', $size = 'square', $logo = TRUE) {
  $protocol = (isset($_SERVER['HTTPS']) && $_SERVER['HTTPS'] == 'on') ? 'https' : 'http';
  $output = <<<PIC
    <div class="picture">
      <a href="$user_url">
        <img src="$protocol://graph.facebook.com/$fbuid/picture?type=$size" />
      </a>
    </div>
PIC;
  return $output;
}


/**
 * Make rendering of facebook user profile tab themable
 */
function theme_fbconnect_user_profile_tab_connected($account = NULL, $conf = NULL, $uid = NULL) {
  $site_name = $conf['invite_name'] ? $conf['invite_name'] : '';
  return t(
    "Your current Facebook session is associated to another account on our site.\n"
   ."You can logout of Facebook and attempt to associate another Facebook account to your"
   ."!site_name account, or you need to disconnect the current association."
   , array('!site_name' => $site_name)
  );
}

function theme_fbconnect_user_profile_tab_unconnected($account = NULL, $conf = NULL, $uid = NULL) {
  return '<div>'
       . t('Click here to connect your Facebook account')
       . '</div><div class="fbconnect-button">'
       . fbconnect_render_button()
       . '</div>';
}

/**
 * Workaround to play nice with LoginDestination #786662 
 */
function fbconnect_link_callback() {
  global $user;
  drupal_get_messages('status');
  drupal_goto("user/{$user->uid}/edit/fbconnect");
}