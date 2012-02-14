<?php
// $Id: emfield.admin.inc,v 1.1.2.16.2.2 2009/12/08 21:19:21 aaron Exp $

/**
 * @file
 * The administrative settings page.
 */

/**
 * Callback for admin/content/emfield.
 */
function emfield_settings() {
  if (!module_exists('emvideo') && !module_exists('emimage') && !module_exists('emaudio')) {
    drupal_set_message(t('The Embedded Media Field module does nothing on its own. You should also install the Embedded Video Field, Embedded Image Field, and/or Embedded Audio Field modules from the <a href="@modules">modules administration page</a>. (If you do not see them listed there, under the CCK section, you may need to <a href="@download">download the module</a> from its project page. They are all in the same package.)', array('@download' => 'http://drupal.org/project/emfield', '@modules' => url('admin/build/modules'))), 'error');
  }
  $form = array();

  $form = array(
    '#type' => 'fieldset',
    '#title' => t('General Settings'),
    '#description' => t('These features will be generally available for use by related modules as needed.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  // Add vertical tabs display if available.
  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';

  $form['flvmediaplayer'] = array(
    '#type' => 'fieldset',
    '#title' => t('JW FLV Media Player options'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  if (module_exists('flvmediaplayer')) {
    $url = emfield_flvmediaplayer_url(TRUE);
    $flvmediaplayer_configs = flvmediaplayer_profile_get_configs();
    $flvmediaplayer_configs = (!empty($flvmediaplayer_configs)) ? $flvmediaplayer_configs : array();
    $form['flvmediaplayer']['emfield_flvmediaplayer_url'] = array(
      '#type' => 'item',
      '#title' => t('JW FLV Media Player'),
      '#description' => t('Some providers may support the !player, and may be configured to use that player for display of its media. You may set that path (%path) at the !settings.', array('%path' => url($url), '!settings' => l(t('FLV Media Player settings page'), 'admin/settings/flvmediaplayer'), '!player' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/'))),
    );
    $form['flvmediaplayer']['emfield_flv_profile'] = array(
      '#type' => 'select',
      '#title' => t('FLV Media Player Profile'),
      '#options' => array('' => t('-----')) + $flvmediaplayer_configs,
      '#description' => t('You may choose to display the FLV Media Player with a !profile, if configured.', array('!profile' => l(t('profile'), 'admin/settings/flvmediaplayer/profiles'))),
      '#default_value' => variable_get('emfield_flv_profile', ''),
    );
  }
  else {
    $form['flvmediaplayer']['emfield_flvmediaplayer_url'] = array(
      '#type' => 'textfield',
      '#title' => t('JW FLV Media Player'),
      '#default_value' => emfield_flvmediaplayer_url(TRUE),
      '#description' => t("Some providers may support the !player, and may be configured to use that player for display of its media. In that case, please enter the path to this player here, without a leading slash. If you are certain you have the player installed on this server, the path doesn't show up here, and you don't know the path, you can blank out this field and save the configuration, and an attempt will be made to autodiscover the player. For best success, you should put the file somewhere in the /sites/all/plugins folder.", array('!player' => l(t('JW FLV Media Player'), 'http://www.longtailvideo.com/players/jw-flv-player/'))),
    );
  }
  $form['flvmediaplayer']['emfield_imagerotator_url'] = array(
    '#type' => 'textfield',
    '#title' => t('JW Image Rotator'),
    '#default_value' => emfield_imagerotator_url(TRUE),
    '#description' => t("Some providers may support the !player, and may be configured to use that player for display of its media. In that case, please enter the path to this player here, without a leading slash. If you are certain you have the player installed on this server, the path doesn't show up here, and you don't know the path, you can blank out this field and save the configuration, and an attempt will be made to autodiscover the player. For best success, you should put the file somewhere in the /sites/all/plugins folder.", array('!player' => l(t('JW Image Rotator'), 'http://www.longtailvideo.com/players/jw-image-rotator/'))),
  );

  if (module_exists('swfobject_api')) {
    $swfobject_desc = t('As you have the <a href="@swfobject_api" target="_blank">SWFObject API</a> module installed, Embedded Media Field will use those settings, assuming you have configured them properly. Visit its <a href="@settings">settings page</a> for more information.', array('@swfobject_api' => 'http://drupal.org/project/swfobject_api', '@settings' => url('admin/settings/swfobject_api')));
  }
  else {
    $swfobject_desc = t('If you have the swfobject.js file installed on your system, you can make it available to Embedded Media Field and its related modules by entering the information here. You can download and find out more about <a href="@here" target="_blank">SWFObject here</a>. You may also choose to install the <a href="@swfobject_api" target="_blank">SWFObject API</a> module, which will integrate automatically with this module..', array('@here' => 'http://code.google.com/p/swfobject/', '@swfobject_api' => 'http://drupal.org/project/swfobject_api'));
  }
  $form['swfobject'] = array(
    '#type' => 'fieldset',
    '#title' => t('SWF Object'),
    '#description' => $swfobject_desc,
    '#collapsible' => TRUE,
  );
  $form['swfobject']['emfield_swfobject'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use SWFObject'),
    '#default_value' => variable_get('emfield_swfobject', FALSE),
    '#description' => t('When checked, then Embedded Media Field will use the SWFObject javascript library when it is able.'),
  );

  if (!module_exists('swfobject_api')) {
    $form['swfobject']['emfield_swfobject_location'] = array(
      '#type' => 'textfield',
      '#title' => t('SWFObject location'),
      '#default_value' => variable_get('emfield_swfobject_location', ''),
      '#description' => t('Enter the path to the swfobject.js file, relative to the web root, without the preceding slash (/).'),
    );
  }

  $form['thumbnails'] = array(
    '#type' => 'fieldset',
    '#title' => t('Thumbnails'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['thumbnails']['emfield_thumbnail_overlay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Thumbnail play button overlay'),
    '#description' => t('If a thumbnail replacement or modal display formatter (such as Shadowbox) is selected, then a play button will be overlayed on the image if this box is checked.'),
    '#default_value' => variable_get('emfield_thumbnail_overlay', TRUE),
  );

  $form = system_settings_form($form);

  // Custom valiation callback so we can validate the SWFObject path.
  $form['#validate'][] = 'emfield_settings_validate';

  return $form;
}

function emfield_module_settings(&$form_state, $module) {
  $form = array();

  $header = array(t('Feature'), t('Supported'), t('Notes'));

  $emfield_info = module_invoke($module, 'emfield_info');
  $providers = emfield_system_list($module);
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('@module', array('@module' => $emfield_info['#name'])),
    '#description' => $emfield_info['#settings_description'],
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form = array_merge($form, module_invoke($module, 'emfield_settings'));
  $form['providers'] = array(
    '#type' => 'fieldset',
    '#title' => t('Providers'),
    '#description' => t('The following settings determine what providers are allowed, and what provider-specific options, if any, are set.'),
//     '#collapsible' => TRUE,
//     '#collapsed' => FALSE,
  );
    // Add vertical tabs display if available.
  $form['providers']['#pre_render'][] = 'vertical_tabs_form_pre_render';
  foreach ($providers as $provider) {
    $info = emfield_include_invoke($module, $provider->name, 'info');
    $form['providers'][$provider->name] = array(
      '#type' => 'fieldset',
      '#title' => t('@provider configuration', array('@provider' => $info['name'])),
      '#description' => $info['settings_description'],
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    if (is_array($info['supported_features']) && !empty($info['supported_features'])) {
      $form['providers'][$provider->name]['supported_features'] = array(
        '#type' => 'fieldset',
        '#title' => t('Supported features'),
        '#description' => t('This is a list of the current state of support for the following features by %provider:', array('%provider' => $info['name'])),
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#weight' => 7,
      );
      $form['providers'][$provider->name]['supported_features']['features'] = array(
        '#type' => 'markup',
        '#value' => theme('table', $header, $info['supported_features']),
      );
    }
    $form['providers'][$provider->name]['emfield_'. $module .'_allow_'. $provider->name] = array( '#type' => 'checkbox',
      '#title' => t('Allow content from %provider', array('%provider' => $info['name'])),
      '#description' => t('If checked, then content types may be created that allow content to be provided by %provider.', array('%provider' => $info['name'])),
      '#weight' => -10,
      '#default_value' => variable_get('emfield_'. $module .'_allow_'. $provider->name, TRUE),
    );
    $form['providers'][$provider->name][] = emfield_include_invoke($module, $provider->name, 'settings');
  }

  $form = system_settings_form($form);

  return $form;
}

/**
 *  Validation for emfield_settings form, callback for /admin/content/emfield.
 *  Ensure we have a valid SWFObject path.
 */
function emfield_settings_validate($form, $form_state) {
  if ($form_state['values']['emfield_swfobject'] && $form_state['values']['emfield_swfobject_location']) {
    if (!file_exists($form_state['values']['emfield_swfobject_location'])) {
      form_set_error('emfield_swfobject_location', t('The SWFObject %file file was not found at that location. Please specify a valid location.', array('%file' => $form_state['values']['emfield_swfobject_location'])));
    }
  }
}

/**
 * Menu callback settings form.
 * If job_queue module is installed, admins can reload embedded media data in bulk
 */
function emfield_settings_jobqueue() {
  $form = array();
  $form['job_queue'] = array(
    '#type' => 'fieldset',
    '#title' => t('Reload Embedded Media Data'),
    '#description' => t('You can reload the embedded media data on all nodes on your site if necessary.  This action will be queued to run on cron via the job queue module.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $types = content_types();
  $options = array();
  foreach ($types as $type) {
    foreach ($type['fields'] as $field) {
      if (module_hook($field['type'], 'emfield_info')) {
        $options[$type['type']] = $type['name'];
      }
    }
  }
  if (count($options)) {
    $form['job_queue']['types'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Types to reload'),
      '#options' => $options,
      '#description' => t('Choose the content types to reload'),
    );

    $form['job_queue']['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Submit'),
    );
  }

  return $form;
}

/**
 * For each selected content type, add all of its nodes to the job queue for reloading.
 */
function emfield_settings_jobqueue_submit($form, &$form_state) {
  foreach($form_state['values']['types'] as $type => $value) {
    if ($value) {
      $result = db_query("SELECT nid FROM {node} WHERE type = '%s'", $type);
      while ($nid = db_result($result)) {
        job_queue_add('emfield_reload', 'refresh emfield info', array($nid), drupal_get_path('module', 'emfield') .'/emfield.admin.inc', TRUE);
      }
      drupal_set_message(t('The %type nodes have been queued to reload emfield data upon cron.', array('%type' => $type)));
    }
  }
}

/**
 *  This reloads and saves the data for a single node.
 */
function emfield_reload($nid) {
  if ($node = node_load($nid)) {
    $type = content_types($node->type);
    foreach ($type['fields'] as $field) {
      if (module_hook($field['type'], 'emfield_info')) {
        $items = $node->{$field['field_name']};
        emfield_emfield_field('submit', $node, $field, $items, FALSE, FALSE, $field['type']);
        $node->{$field['field_name']} = $items;
        node_save($node);
      }
    }
  }
}

function emfield_disable_obsolete_provider($module = NULL, $provider = NULL) {
  if ($module && $provider) {
    $providers = emfield_system_list($module, $provider, array('suppress_errors' => TRUE));
    if (!isset($providers[$provider])) {
      variable_set('emfield_'. $module .'_allow_'. $provider, FALSE);
      drupal_set_message(t('The message regarding the missing or obsolete Embedded Media Field %provider provider file has been disabled. Note that if you decide in the future to install the provider, you will need to re-enable it at the !admin page.', array('%provider' => $provider, '!admin' => l(t('Embedded Media Field configuration'), 'admin/content/emfield'))));
    }
  }
  drupal_goto($_GET['destination']);
}
