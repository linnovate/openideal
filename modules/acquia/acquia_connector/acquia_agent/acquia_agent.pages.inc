<?php
// $Id: acquia_agent.pages.inc,v 1.5.2.3 2010/09/15 21:39:18 pwolanin Exp $

/**
 * @file
 *   Acquia Agent configuration page.
 */

/**
 * Helper function. Creates an authenticator for xmlrpc calls
 */
function _acquia_agent_create_authenticator($body, $pass = NULL) {
  $path = drupal_get_path('module', 'acquia_agent');
  require_once $path .'/acquia_agent_streams.inc';

  $a = array();
  $a['time'] = time();
  $nonce = md5(acquia_agent_random_bytes(55));
  $a['nonce'] = $nonce;
  // We need an hmac to authenticate to an acquia.com account.
  // This will fail if acquia.com moves to D7.
  if (isset($pass)) {
    $md5_pass = md5($pass);
    $a['hash'] = _acquia_agent_hmac($md5_pass, $a['time'], $a['nonce'], $body);
  }
  else {
    // rpc.acquia.com XML-RPC interface requires this parameter to be a string.
    // Just pass a dummy value.
    $a['hash'] = 'x';
  }
  return $a;
}

/**
 * Helper function that creates a new AN subscription via xmlrpc call. It stores id/key
 * if success and sets page error otherwise
 */
function _acquia_agent_call_provision_freetrial($body, $authenticator, $pass) {
  $md5_pass = md5($pass);
  $values = array('body' => $body, 'authenticator' => $authenticator);
  $result = xmlrpc(acquia_agent_network_address(), 'acquia.agent.provision.freetrial', $values);
  if ($errno = xmlrpc_errno()) {
    drupal_set_message(t('Error getting free trial: @message (@errno)', array('@message' => xmlrpc_error_msg(), '@errno' => xmlrpc_errno())), 'error');
    watchdog('acquia agent', '@message (@errno): %server - %method - <pre>@data</pre>', array('@message' => xmlrpc_error_msg(), '@errno' => xmlrpc_errno(), '%server' => acquia_agent_network_address(), '%method' => 'acquia.agent.provision.freetrial', '@data' => print_r($values, TRUE)), WATCHDOG_ERROR);
    $result = FALSE;
  }
  elseif (!empty($result['body']['nid'])) {
    if (!empty($result['body']['hashkey']) && !empty($md5_pass)) {
      // We have a key XOR'd with a hash.
      $hash = str_pad(_acquia_agent_hmac(
        $md5_pass,
        $authenticator['time'],
        $authenticator['nonce'],
        $result['body']['identifier']), 64, chr(0x00));
      // Repeat the XOR and remove trailing NUL bytes.
      $key = rtrim(base64_decode($result['body']['hashkey']) ^ $hash);
      // Strip off the padding marker character.
      $key = substr($key, 0, -1);
    }
    else {
      $key = $result['body']['key'];
    }
    variable_set('acquia_key', $key);
    variable_set('acquia_identifier', $result['body']['identifier']);
    drupal_set_message(t('The Acquia configuration options have been saved.'));
    // Check subscription and send a heartbeat to Acquia Network via XML-RPC.
    // Our status gets updated locally via the return data.
    acquia_agent_check_subscription();
    cache_clear_all();
  }
  return $result;
}

/**
 *  Main 30 day trial form function
 */
function acquia_agent_an_trial_form(&$form_state, $banner) {
  $cur_step = NULL;
  if (!empty($form_state['storage']['step'])) {
    $function = '_acquia_agent_an_trial_'. $form_state['storage']['step'] .'_form';
    return $function($form_state, $banner);
  }
  else {
    return _acquia_agent_an_trial_start_form($form_state, $banner);
  }
}

/**
 * 30 day trial form submit dispatch function
 */
function acquia_agent_an_trial_form_submit($form, &$form_state) {
  if (empty($form_state['storage'])) {
    $form_state['storage'] = array();
    $form_state['storage']['values'] = array();
  }

  // Store submitted form values. Overwrite old values with new.
  $form_state['storage']['values'] = $form_state['values'] + $form_state['storage']['values'];

  // Call the step-specific submit handler.
  $submit_function = '_acquia_agent_an_trial_'. $form_state['values']['this_step']. '_form_submit';
  if (function_exists($submit_function)) {
    $submit_function($form, $form_state);
  }

  // Set up next step.
  if (!empty($form_state['values']['step_next'])) {
    $form_state['storage']['step'] = $form_state['values']['step_next'];
  }
  elseif (acquia_agent_has_credentials()) {
    // The proces is complete.
    drupal_set_message(t('The site "!site" is now connected to the Acquia network. A welcome email has been sent to the email address you provided', array('!site' => $form_state['values']['subscription_name'])));
    unset($form_state['storage']);
    $form_state['redirect'] = 'admin/settings/acquia-agent';
  }
}

function _acquia_agent_provision_freetrial($form, &$form_state) {
  $val = $form_state['values'] + $form_state['storage']['values'];
  $body = array();

  $body['accept_terms_checkbox'] = 1;

  $new_user = empty($form_state['storage']['userdata']['uid']);
  if ($new_user) {
    // New user
    $body['primary_email'] = $val['email'];

    $pass = $val['pass'];
    // Send password as hash.
    $body['pass'] = md5($val['pass']);
    $body['billing_first_name'] = $val['billing_first_name'];
    $body['billing_last_name'] = $val['billing_last_name'];
  }
  else {
    // Existing user
    $body['uid'] = $form_state['storage']['userdata']['uid'];
    $pass = $val['pass'];
  }

  $body['subscription_name'] = $val['subscription_name'];
  $body['site_url'] = $val['site_url'];
  $body['billing_company'] = $val['billing_company'];
  $body['billing_phone'] = $val['billing_phone'];
  $body['billing_country_acquia'] = $val['billing_country_acquia'];
  $auth = _acquia_agent_create_authenticator($body, ($new_user ? NULL : $pass));
  return _acquia_agent_call_provision_freetrial($body, $auth, $pass);
}

/**
 * Start form page
 *
 * Where user decides if they want to enter their email and get a new subscrition or go to the settings page
 * and use existing AN subscription id/key
 */
function _acquia_agent_an_trial_start_form(&$form_state) {
  $form = array(
    '#prefix' =>  acquia_agent_an_info_header(),
    'email' => array (
      '#type' => 'textfield',
      '#title' => t('E-mail address'),
      '#size' => 32,
      '#maxlength' => 64,
      '#required' => TRUE,
    ),
    'continue' => array(
      '#type' => 'submit',
      '#value' => 'Next >'),
    'settings_link' => array(
      '#value' => '<br/>'.l(t('I already have an active Acquia subscription, and now I want to connect this site to it'), 'admin/settings/acquia-agent/connection')),
    // Special value elements.
    'this_step' => array(
      '#type' => 'value',
      '#value' => 'start',
    ),
    '#validate' => array('_acquia_agent_an_trial_start_form_validate'),
  );
  return $form;
}

function _acquia_agent_an_trial_start_form_validate($form, &$form_state) {
  if (!valid_email_address($form_state['values']['email'])) {
    form_set_error('email', t('Email is invalid'));
  }
}

function _acquia_agent_an_trial_start_form_submit($form, &$form_state) {
  $result = xmlrpc(acquia_agent_network_address(), 'acquia.agent.mail.exists', $form_state['values']['email']);

  if ($errno = xmlrpc_errno()) {
    acquia_agent_report_xmlrpc_error();
    // Set form error to prevent switching to the next page.
    form_set_error('email');
  }
  elseif ($result) {
    // Existing user.
    $form_state['values']['step_next'] = 'pass_old';
  }
  else {
    // New user.
    $form_state['values']['step_next'] = 'details_new';
  }
}

/**
 * New user form
 */
function _acquia_agent_an_trial_details_new_form(&$form_state, $banner = NULL) {
  global $user;

  $val = $form_state['storage']['values'];
  $form = array(
    '#prefix' =>  '<p>'.t('Start your subscription by providing the information below. We\'ll create a free trial subscription for you, and set up this site with the subscription keys.').'</p>',
    'email' => array (
      '#type' => 'item',
      '#title' => t('E-mail address'),
      '#value' => $val['email'],
      '#required' => TRUE,
    ),
    'subscription_name' => array(
      '#type' => 'textfield',
      '#title' => t('Subscription name'),
      '#size' => 32,
      '#maxlength' => 64,
      '#required' => TRUE,
      '#default_value' => variable_get('site_name', 'Site for ' . $user->name),
    ),
    'site_url' => array(
      '#type' => 'textfield',
      '#title' => t('Your site\'s URL'),
      '#size' => 32,
      '#maxlength' => 128,
      '#default_value' => url(NULL, array('absolute' => TRUE)),
    ),
    'billing_country_acquia' => array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#options' => _acquia_agent_countries(),
      '#required' => TRUE
    ),
    'pass' => array(
      '#type' => 'password_confirm',
      '#title' => t('Create your Acquia.com password'),
      '#size' => 32,
      '#required' => TRUE,
    ),
    'billing_first_name' => array (
      '#type' => 'textfield',
      '#title' => t('First name'),
      '#size' => 32,
      '#maxlength' => 32,
      '#required' => TRUE,
    ),
    'billing_last_name' => array (
      '#type' => 'textfield',
      '#title' => t('Last name'),
      '#size' => 32,
      '#maxlength' => 32,
      '#required' => TRUE,
    ),
    'billing_company' => array (
      '#type' => 'textfield',
      '#title' => t('Organization name'),
      '#size' => 32,
      '#maxlength' => 64,
      '#required' => TRUE,
    ),
    'billing_phone' => array (
      '#type' => 'textfield',
      '#title' => t('Phone'),
      '#size' => 32,
      '#maxlength' => 64,
      '#required' => TRUE,
    ),
    'submit' => array (
      '#type' => 'submit',
      '#value' => t('Start my new 30-day free trial for this site'),
    ),
    // Special value elements.
    'this_step' => array(
      '#type' => 'value',
      '#value' => 'details_new',
    ),
    
    'banner' => array('#value' => $banner),
    '#theme' => 'acquia_agent_banner_form',
    '#validate' => array('_acquia_agent_an_trial_details_form_validate'),
  );
  return $form;
}

function _acquia_agent_an_trial_details_form_validate($form, &$form_state) {
  if (strlen($form_state['values']['site_url']) && !valid_url($form_state['values']['site_url'], TRUE)) {
    form_set_error('site_url', 'Please enter a valid website URL or leave the field empty. A valid website URL should include http:// or https://.');
  }
  // We don't want to do the provisioning if there are any errors.
  if (!form_get_errors()) {
    // Do the provisioning at the validation stage so a form error doesn't lose form values.
    $result = _acquia_agent_provision_freetrial($form, $form_state);
    if (!$result) {
      form_set_error('', '');
    }
    else {
      $form_state['result'] = $result['body'];
    }
  }
}

/**
 * Existing user form page
 */
function _acquia_agent_an_trial_details_old_form(&$form_state, $banner = NULL) {
  $ud = $form_state['storage']['userdata'];

  $form = array(
    '#prefix' => '<p>'. t('Let\'s get started creating a new trial subscription for your account.') .'</p>',
    'name' => array(
      '#type' => 'item',
      '#title' => t('My account username'),
      '#value' => $ud['name'],
      '#required' => TRUE,
    ),
    'first_name' => array (
      '#type' => 'item',
      '#title' => t('First name'),
      '#value' => $ud['profile_firstname'],
      '#required' => TRUE,
    ),
    'last_name' => array (
      '#type' => 'item',
      '#title' => t('Last name'),
      '#value' => $ud['profile_lastname'],
      '#required' => TRUE,
    ),
    'subscription_name' => array(
      '#type' => 'textfield',
      '#title' => t('Subscription name'),
      '#size' => 32,
      '#maxlength' => 64,
      '#required' => TRUE,
      '#default_value' => variable_get('site_name', t('My new site')),
    ),
    'site_url' => array(
      '#type' => 'textfield',
      '#title' => t('Your site\'s URL'),
      '#size' => 32,
      '#maxlength' => 128,
      '#default_value' => url(NULL, array('absolute' => TRUE)),
    ),
    'billing_country_acquia' => array(
      '#type' => 'select',
      '#title' => t('Country'),
      '#options' => _acquia_agent_countries(),
      '#required' => TRUE,
      '#default_value' => $ud['profile_country'],
    ),
    'billing_company' => array (
      '#type' => 'textfield',
      '#title' => t('Company'),
      '#size' => 32,
      '#maxlength' => 64,
      '#default_value' => $ud['profile_organization'],
      '#required' => TRUE,
    ),
    'billing_phone' => array (
      '#type' => 'textfield',
      '#title' => t('Phone'),
      '#size' => 32,
      '#maxlength' => 64,
      '#default_value' => $ud['profile_officephone'],
      '#required' => TRUE,
    ),
    'submit' => array (
      '#type' => 'submit',
      '#value' => t('Start my new 30-day free trial for this site'),
    ),
    // Special value elements.
    'this_step' => array(
      '#type' => 'value',
      '#value' => 'details_old',
    ),
    
    'banner' => array('#value' => $banner),
    '#theme' => 'acquia_agent_banner_form',
    '#validate' => array('_acquia_agent_an_trial_details_form_validate'),
  );

  return $form;
}

/**
 * Existing user password page
 *
 */
function _acquia_agent_an_trial_pass_old_form(&$form_state, $banner = NULL) {
  $values = $form_state['storage']['values'];
  $form = array(
    '#prefix' =>  acquia_agent_an_info_header(),
    'form_header' => array(
      '#value' => acquia_agent_existing_account_msg(),
    ),
    'email' => array (
      '#type' => 'item',
      '#title' => t('E-mail address'),
      '#value' => $values['email'],
      '#required' => TRUE,
    ),
    'pass' => array(
      '#type' => 'password',
      '#title' => t('Acquia.com password'),
      '#description' => t('Please enter your Acquia.com password here'),
      '#size' => 32,
      '#required' => TRUE,
    ),
    'continue' => array(
      '#type' => 'submit',
      '#value' => 'Next >',
    ),
    'forgot_pass' => array(
      '#value' => '<br/>'. l(t('I forgot my password'), 'http://acquia.com/user/password', array('attributes' => array('target' => '_blank'))),
    ),
    // Special value elements.
    'this_step' => array(
      '#type' => 'value',
      '#value' => 'pass_old',
    ),
    'step_next' => array(
      '#type' => 'value',
      '#value' => 'details_old',
    ),
    '#validate' => array('_acquia_agent_an_trial_pass_old_form_validate'),
  );
  return $form;
}

function _acquia_agent_an_trial_pass_old_form_validate($form, &$form_state) {
  $val = $form_state['storage']['values'];
  if (!empty($val['email'])) {
    $params = array('email' => $val['email']);
    $args = array('body' => $params, 'authenticator' => _acquia_agent_create_authenticator($params, $form_state['values']['pass']));
    $result = xmlrpc(acquia_agent_network_address(), 'acquia.agent.provision.get_user_details', $args);
    if ($errno = xmlrpc_errno()) {
      acquia_agent_report_xmlrpc_error();
      // Set form error to prevent switching to the next page.
      form_set_error('email');
    }
    else {
      $form_state['storage']['userdata'] = $result['body'];
    }
  }
  else {
    // Paranoid check. should not happen.
    form_set_error('email', t('No email provided'));
  }
}

/**
 * Main page function
 */
function acquia_agent_settings_page($arg = NULL) {
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');
  $path = drupal_get_path('module', 'acquia_agent');
  $banner = theme('image', $path .'/acquia_vertical_banner.png');
  drupal_add_css($path .'/acquia_agent.css', 'module', 'all', FALSE);

  if (($identifier && $key) || $arg == 'connection') {
    return acquia_agent_settings_form_page($banner);
  }
  else {
    drupal_set_title(t('Get a free 30 day trial of the Acquia Network'));
    return drupal_get_form('acquia_agent_an_trial_form', $banner);
  }
}

/**
 * Menu callback for settings page.
 */
function acquia_agent_settings_form_page($banner = NULL) {
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');

  $output =  '';
  // Check $_POST so we don't send extra XML-RPC requests during form submission.
  if ($identifier && $key && empty($_POST)) {
    // Check our connection to the Acquia Network and validity of the crenditials.
    $acquia_network_address = acquia_agent_settings('acquia_network_address');
    if (acquia_agent_valid_credentials($identifier, $key, $acquia_network_address)) {
      $subscription = acquia_agent_settings('acquia_subscription_data');
    }
    else {
      $error_message = acquia_agent_connection_error_message();
      drupal_set_message($error_message, 'error', FALSE);
    }
  }
  return drupal_get_form('acquia_agent_settings_form', $banner);
}

/**
 * Settings form builder function.
 */
function acquia_agent_settings_form($form_state, $banner) {
  $identifier = acquia_agent_settings('acquia_identifier');
  $key = acquia_agent_settings('acquia_key');

  // Help for copying the identifier and key.
  $copy_help = t('Copy from <a href="@subscriptions-overview">your subscriptions overview on the Acquia Network</a>.', array('@subscriptions-overview' => 'http://acquia.com/network/dashboard/subscription'));

  $form['cs'] = array(
    '#type' => 'fieldset',
    '#title' => t('Acquia Network keys'),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
  );

  $form['cs']['acquia_identifier'] = array(
    '#type' => 'textfield',
    '#title' => t('Identifier'),
    '#default_value' => $identifier,
    '#description' => $copy_help,
    '#required' => TRUE,
  );
  $form['cs']['acquia_key'] = array(
    '#type' => 'textfield',
    '#title' => t('Key'),
    '#default_value' => $key,
    '#description' => $copy_help,
    '#required' => TRUE,
  );

  $form['cs']['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save keys'),
    '#validate' => array('acquia_agent_set_validate'),
    '#submit' => array('acquia_agent_set_submit'),
  );
  $form['cs']['buttons']['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Clear keys'),
    '#access' => (!empty($identifier) || !empty($key)),
    '#submit' => array('acquia_agent_delete_submit'),
  );
  $ssl_available = (in_array('ssl', stream_get_transports(), TRUE) && !defined('ACQUIA_DEVELOPMENT_NOSSL'));

  $form['ssl'] = array(
    '#type' => 'fieldset',
    '#title' => t('SSL settings'),
    '#collapsible' => FALSE,
  );
  $form['ssl']['acquia_agent_verify_peer'] = array(
    '#type' => 'radios',
    '#title' => t('Enhanced SSL security'),
    '#default_value' => (int) (variable_get('acquia_agent_verify_peer', 0) && $ssl_available),
    '#options' => array(0 => t('Disabled'), 1 => t('Enabled')),
    '#description' => t('If enabled the Acquia Connector will attempty to verify Acquia server identities before sending data.  May cause communication to fail, however, depending on your local configuration.'),
    '#disabled' => !$ssl_available,
  );

  if ($ssl_available) {
    $form['ssl']['acquia_agent_verify_peer']['#description'] .= ' <div class="ok">'. t('PHP has SSL support and may support this feature.') .'</div>';
    $form['ssl']['buttons']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#submit' => array('acquia_agent_ssl_submit'),
    );
  }
  else {
    $form['ssl']['acquia_agent_verify_peer']['#description'] .= ' <div class="error">'. t('PHP does not have SSL support.') .'</div>';
  }
  $form['banner'] = array('#value' => $banner);
  $form['#theme'] = 'acquia_agent_banner_form';
  return $form;
}

/**
 * Submit acquia_agent_settings ssl setting.
 */
function acquia_agent_ssl_submit($form, &$form_state) {
  variable_set('acquia_agent_verify_peer', $form_state['values']['acquia_agent_verify_peer']);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Validate acquia_agent_settings form submissions.
 */
function acquia_agent_set_validate($form, &$form_state) {
  // Trim all input to get rid of possible whitespace pasted from the website.
  foreach ($form_state['values'] as $key => $value) {
    $form_state['values'][$key] = trim($value);
  }
  $identifier = $form_state['values']['acquia_identifier'];
  $key = $form_state['values']['acquia_key'];

  // Don't make the XML-RPC call with empty values.
  if (!empty($identifier) && !empty($key)) {
    if (!acquia_agent_valid_credentials($identifier, $key)) {
      $error_message = acquia_agent_connection_error_message();
      form_error($form, $error_message);
    }
  }
}

/**
 * Save acquia_agent_settings form submissions.
 */
function acquia_agent_set_submit($form, &$form_state) {
  variable_set('acquia_key', $form_state['values']['acquia_key']);
  variable_set('acquia_identifier', $form_state['values']['acquia_identifier']);
  drupal_set_message(t('The Acquia configuration options have been saved.'));
  // Check subscription and send a heartbeat to Acquia Network via XML-RPC.
  // Our status gets updated locally via the return data.
  acquia_agent_check_subscription();
  // Redirect to the path without the suffix.
  $form_state['redirect'] = 'admin/settings/acquia-agent';
  cache_clear_all();
}

/**
 * Delete acquia_agent settings.
 */
function acquia_agent_delete_submit($form, &$form_state) {
  variable_del('acquia_key');
  variable_del('acquia_identifier');
  variable_del('acquia_network_address');
  variable_del('acquia_subscription_data');
  drupal_set_message(t('Your Acquia configuration has been deleted.'));
  // Redirect to the path with the suffix.
  $form_state['redirect'] = 'admin/settings/acquia-agent/connection';
  cache_clear_all();
}

function theme_acquia_agent_banner_form($form) {
  if (empty($form['banner'])) {
    return drupal_render($form);
  }
  $banner = drupal_render($form['banner']);
  $output = '<div id="an-pg-container"><div id="an-pg-form">';
  $output .= drupal_render($form);
  $output .= "\n</div>\n";
  $output .= '<div class="an-pg-banner" id="'. $form['#id'] .'-banner">';
  $output .= '<a href="http://acquia.com/products-services/acquia-network" target="_blank">'. $banner . '</a>';
  $output .= "\n</div>\n</div>\n";
  return $output;
}

function acquia_agent_an_info_header() {
	$l_opt = array('attributes' => array('target' => '_blank'));
  $output = '<h2 id="an-info-header">'.t('With the <a href="@acquia-network" target="_blank">Acquia Network</a> you get:', array('@acquia-network' => 'http://acquia.com/products-services/acquia-network')).'</h2>';
  $output .= '<div id="an-info-box">';
  $output .=   '<div class="cell an-left">';
  $output .=     '<h2>'. t('Drupal support') .'</h2>';
  $output .=     t('Get expert advice for tough Drupal questions.');
  $output .=     '<br/>' .l(t('Learn more'), 'http://acquia.com/drupal-support', $l_opt);
  $output .=   '</div>';
  $output .=   '<div class="cell an-center">';
  $output .=     '<h2>'. t('Acquia Search') .'</h2>';
  $output .=     t('Add faceted search and content recommendations.');
  $output .=     '<br/>'. l(t('Learn more'), 'http://acquia.com/products-services/acquia-search', $l_opt);
  $output .=   '</div>';
  $output .=   '<div class="cell an-right">';
  $output .=     '<h2>'. t('Mollom') .'</h2>';
  $output .=     t('Add automated spam blocking for all user generated content.');
  $output .=     '<br/>'. l(t('Learn more'), 'http://acquia.com/documentation/getting-started/acquia-network-services/mollom', $l_opt);
  $output .=   '</div>';
  $output .= '</div>';
  return $output;
}

function acquia_agent_existing_account_msg() {
  $output = '<div class="messages warning" style="margin-top: 0px">';
  $output .= t('An existing Acquia.com account is associated with that email address. Enter your password to associate this free trial with that account, or <a href="@an_trial">try another email address</a>', array('@an_trial' => url('admin/settings/acquia-agent')));
  $output .= '</div>';
  return $output;
}

/**
 * List of countries in the Acquia Network database.
 */
function _acquia_agent_countries() {
  return array (
    'United States' => 'United States',
    'Afghanistan' => 'Afghanistan',
    'Albania' => 'Albania',
    'Algeria' => 'Algeria',
    'American Samoa' => 'American Samoa',
    'Andorra' => 'Andorra',
    'Angola' => 'Angola',
    'Anguilla' => 'Anguilla',
    'Antigua and Barbuda' => 'Antigua and Barbuda',
    'Argentina' => 'Argentina',
    'Armenia' => 'Armenia',
    'Ascension Island' => 'Ascension Island',
    'Australia' => 'Australia',
    'Austria' => 'Austria',
    'Azerbaijan' => 'Azerbaijan',
    'Bahamas' => 'Bahamas',
    'Bahrain' => 'Bahrain',
    'Bangladesh' => 'Bangladesh',
    'Barbados' => 'Barbados',
    'Belarus' => 'Belarus',
    'Belgium' => 'Belgium',
    'Belize' => 'Belize',
    'Benin' => 'Benin',
    'Bermuda' => 'Bermuda',
    'Bhutan' => 'Bhutan',
    'Bolivia' => 'Bolivia',
    'Bosnia and Herzegovina' => 'Bosnia and Herzegovina',
    'Botswana' => 'Botswana',
    'Brazil' => 'Brazil',
    'British Indian Ocean Territory' => 'British Indian Ocean Territory',
    'Brunei Darussalam' => 'Brunei Darussalam',
    'Bulgaria' => 'Bulgaria',
    'Burkina Faso' => 'Burkina Faso',
    'Burundi' => 'Burundi',
    'Cambodia' => 'Cambodia',
    'Cameroon' => 'Cameroon',
    'Canada' => 'Canada',
    'Cape Verde' => 'Cape Verde',
    'Cayman Islands' => 'Cayman Islands',
    'Central African Republic' => 'Central African Republic',
    'Chad' => 'Chad',
    'Chile' => 'Chile',
    'China' => 'China',
    'Colombia' => 'Colombia',
    'Comoros' => 'Comoros',
    'Congo' => 'Congo',
    'Cook Islands' => 'Cook Islands',
    'Costa Rica' => 'Costa Rica',
    'Cote D Ivoire' => 'Cote D Ivoire',
    'Croatia' => 'Croatia',
    'Cuba' => 'Cuba',
    'Cyprus' => 'Cyprus',
    'Czech Republic' => 'Czech Republic',
    'Denmark' => 'Denmark',
    'Djibouti' => 'Djibouti',
    'Dominica' => 'Dominica',
    'Dominican Republic' => 'Dominican Republic',
    'Ecuador' => 'Ecuador',
    'Egypt' => 'Egypt',
    'El Salvador' => 'El Salvador',
    'Equatorial Guinea' => 'Equatorial Guinea',
    'Eritrea' => 'Eritrea',
    'Estonia' => 'Estonia',
    'Ethiopia' => 'Ethiopia',
    'Falkland Islands' => 'Falkland Islands',
    'Faroe Islands' => 'Faroe Islands',
    'Federated States of Micronesia' => 'Federated States of Micronesia',
    'Fiji' => 'Fiji',
    'Finland' => 'Finland',
    'France' => 'France',
    'French Guiana' => 'French Guiana',
    'French Polynesia' => 'French Polynesia',
    'Gabon' => 'Gabon',
    'Georgia' => 'Georgia',
    'Germany' => 'Germany',
    'Ghana' => 'Ghana',
    'Greece' => 'Greece',
    'Greenland' => 'Greenland',
    'Grenada' => 'Grenada',
    'Guadeloupe' => 'Guadeloupe',
    'Guatemala' => 'Guatemala',
    'Guam' => 'Guam',
    'Guinea' => 'Guinea',
    'Guinea Bissau' => 'Guinea Bissau',
    'Guyana' => 'Guyana',
    'Haiti' => 'Haiti',
    'Holy See (Vatican City)' => 'Holy See (Vatican City)',
    'Honduras' => 'Honduras',
    'Hong Kong' => 'Hong Kong',
    'Hungary' => 'Hungary',
    'Iceland' => 'Iceland',
    'India' => 'India',
    'Indonesia' => 'Indonesia',
    'Iran' => 'Iran',
    'Ireland' => 'Ireland',
    'Isle of Man' => 'Isle of Man',
    'Israel' => 'Israel',
    'Italy' => 'Italy',
    'Jamaica' => 'Jamaica',
    'Japan' => 'Japan',
    'Jordan' => 'Jordan',
    'Kazakhstan' => 'Kazakhstan',
    'Kenya' => 'Kenya',
    'Kiribati' => 'Kiribati',
    'Korea' => 'Korea',
    'Kuwait' => 'Kuwait',
    'Kyrgyzstan' => 'Kyrgyzstan',
    'Laos' => 'Laos',
    'Latvia' => 'Latvia',
    'Lebanon' => 'Lebanon',
    'Lesotho' => 'Lesotho',
    'Liberia' => 'Liberia',
    'Libya' => 'Libya',
    'Liechtenstein' => 'Liechtenstein',
    'Lithuania' => 'Lithuania',
    'Luxembourg' => 'Luxembourg',
    'Macau' => 'Macau',
    'Macedonia' => 'Macedonia',
    'Madagascar' => 'Madagascar',
    'Malawi' => 'Malawi',
    'Malaysia' => 'Malaysia',
    'Maldives' => 'Maldives',
    'Mali' => 'Mali',
    'Malta' => 'Malta',
    'Marshall Islands' => 'Marshall Islands',
    'Martinique' => 'Martinique',
    'Mauritius' => 'Mauritius',
    'Mayotte' => 'Mayotte',
    'Mexico' => 'Mexico',
    'Moldova' => 'Moldova',
    'Monaco' => 'Monaco',
    'Mongolia' => 'Mongolia',
    'Montenegro' => 'Montenegro',
    'Montserrat' => 'Montserrat',
    'Morocco' => 'Morocco',
    'Mozambique' => 'Mozambique',
    'Myanmar' => 'Myanmar',
    'Namibia' => 'Namibia',
    'Nauru' => 'Nauru',
    'Nepal' => 'Nepal',
    'Netherlands' => 'Netherlands',
    'Netherlands Antilles' => 'Netherlands Antilles',
    'New Caledonia' => 'New Caledonia',
    'New Zealand' => 'New Zealand',
    'Nicaragua' => 'Nicaragua',
    'Niger' => 'Niger',
    'Nigeria' => 'Nigeria',
    'Niue' => 'Niue',
    'Norfolk Island' => 'Norfolk Island',
    'Northern Mariana Islands' => 'Northern Mariana Islands',
    'Norway' => 'Norway',
    'Oman' => 'Oman',
    'Pakistan' => 'Pakistan',
    'Palau' => 'Palau',
    'Palestine' => 'Palestine',
    'Panama' => 'Panama',
    'Papua New Guinea' => 'Papua New Guinea',
    'Paraguay' => 'Paraguay',
    'Peru' => 'Peru',
    'Philippines' => 'Philippines',
    'Pitcairn' => 'Pitcairn',
    'Poland' => 'Poland',
    'Portugal' => 'Portugal',
    'Puerto Rico' => 'Puerto Rico',
    'Qatar' => 'Qatar',
    'Reunion' => 'Reunion',
    'Romania' => 'Romania',
    'Russian Federation' => 'Russian Federation',
    'Rwanda' => 'Rwanda',
    'Saint Vincent and the Grenadines' => 'Saint Vincent and the Grenadines',
    'San Marino' => 'San Marino',
    'Sao Tome and Principe' => 'Sao Tome and Principe',
    'Saudi Arabia' => 'Saudi Arabia',
    'Senegal' => 'Senegal',
    'Serbia' => 'Serbia',
    'Seychelles' => 'Seychelles',
    'Sierra Leone' => 'Sierra Leone',
    'Singapore' => 'Singapore',
    'Slovakia' => 'Slovakia',
    'Slovenia' => 'Slovenia',
    'Solomon Islands' => 'Solomon Islands',
    'Somalia' => 'Somalia',
    'South Africa' => 'South Africa',
    'South Georgia' => 'South Georgia',
    'Spain' => 'Spain',
    'Sri Lanka' => 'Sri Lanka',
    'St. Kitts and Nevis' => 'St. Kitts and Nevis',
    'St. Lucia' => 'St. Lucia',
    'St. Pierre and Miquelon' => 'St. Pierre and Miquelon',
    'Sudan' => 'Sudan',
    'Suriname' => 'Suriname',
    'Swaziland' => 'Swaziland',
    'Sweden' => 'Sweden',
    'Switzerland' => 'Switzerland',
    'Syrian Arab Republic' => 'Syrian Arab Republic',
    'Taiwan' => 'Taiwan',
    'Tajikistan' => 'Tajikistan',
    'Tanzania' => 'Tanzania',
    'Thailand' => 'Thailand',
    'The Gambia' => 'The Gambia',
    'Togo' => 'Togo',
    'Tokelau' => 'Tokelau',
    'Tonga' => 'Tonga',
    'Trinidad and Tobago' => 'Trinidad and Tobago',
    'Tunisia' => 'Tunisia',
    'Turkey' => 'Turkey',
    'Turkmenistan' => 'Turkmenistan',
    'Turks and Caicos Islands' => 'Turks and Caicos Islands',
    'Tuvalu' => 'Tuvalu',
    'Uganda' => 'Uganda',
    'Ukraine' => 'Ukraine',
    'United Arab Emirates' => 'United Arab Emirates',
    'United Kingdom' => 'United Kingdom',
    'Uruguay' => 'Uruguay',
    'Uzbekistan' => 'Uzbekistan',
    'Vanuatu' => 'Vanuatu',
    'Venezuela' => 'Venezuela',
    'Viet Nam' => 'Viet Nam',
    'Virgin Islands' => 'Virgin Islands',
    'Western Samoa' => 'Western Samoa',
    'Yemen' => 'Yemen',
    'Yugoslavia' => 'Yugoslavia',
    'Zaire' => 'Zaire',
    'Zambia' => 'Zambia',
    'Zimbabwe' => 'Zimbabwe',
  );
};
