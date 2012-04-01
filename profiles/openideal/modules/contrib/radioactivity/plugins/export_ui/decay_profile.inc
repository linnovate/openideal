<?php

/**
 * Define this Export UI plugin.
 */
$plugin = array(
  'schema' => 'radioactivity_decay_profile',
  'access' => 'administer radioactivity',  // Define a permission users must have to access these pages.

  // Define the menu item.
  'menu' => array(
    'menu item' => 'radioactivity',
    'menu title' => 'Radioactivity',
    'menu description' => 'Administer Radioactivity decay profiles.',
  ),

  // Define user interface texts.
  'title singular' => t('decay profile'),
  'title plural' => t('decay profiles'),
  'title singular proper' => t('Radioactivity decay profile'),
  'title plural proper' => t('Radioactivity decay profiles'),

  // Define the names of the functions that provide the add/edit forms.
  'form' => array(
    'settings' => 'radioactivity_admin_edit_profile_form',
    // 'submit' and 'validate' are also valid callbacks.
  ),
);



function radioactivity_admin_edit_profile_form(&$form, &$form_state) {

  $profile = $form_state['item'];

  unset($form['info']['machine_name']);

  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#required' => TRUE,
    '#maxlength' => 128,
    '#default_value' => $profile->name,
  );

  $form['machine_name'] = array(
    '#type' => 'machine_name',
    '#title' => t('Machine name'),
    '#default_value' => $profile->machine_name,
    '#maxlength' => 21,
    '#machine_name' => array(
      'exists' => 'radioactivity_decay_profile_exists',
    ),
  );

  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => $profile->description,
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'radioactivity') . '/js/point.js',
        drupal_get_path('module', 'radioactivity') . '/js/viewport.js',
        drupal_get_path('module', 'radioactivity') . '/js/graph.js',
      ),
    ),
  );

  $default = array(
    '#states' => array(
      'invisible' => array(
        'input[name="enable_decay"]' => array('checked' => false),
      ),
    ),
  );

  $form['enable_decay'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable energy decaying'),
    '#description' => t('Check to enable the decaying of energy.'),
    '#default_value' => $profile->enable_decay,
  );

  $form['half_life'] = array(
    '#type' => 'textfield',
    '#title' => t('Half life'),
    '#default_value' => $profile->half_life,
    '#required' => TRUE,
    '#description' => t('Determines the decay rate of the radioactivity. For exaple, if the decay rate is '.
            '3600 (one hour), the radioactivity halves once an hour. If it is now 1000, it will '.
            'be 500 after an hour, 250 after two hours, and so on. The default is 6 hours.'),
    '#element_validate' => array('_element_validate_integer_positive'),
  ) + $default;

  $form['granularity'] = array(
    '#type' => 'textfield',
    '#title' => t('Decay granularity'),
    '#default_value' => $profile->granularity,
    '#required' => TRUE,
    '#description' => t('This setting determines how often at most the radioactivity is decreased by the decay formula. '.
            'The shorter the time, the more accurate the modeling will be, but the more database '.
            'activity is required. The default (1 minute) should be a good starting point.'),
    '#element_validate' => array('_element_validate_integer_positive'),
  ) + $default;

  $form['cut_off'] = array(
    '#type' => 'textfield',
    '#title' => t('Energy cut off'),
    '#default_value' => $profile->cut_off,
    '#required' => TRUE,
    '#description' => t('Defines the energy lever under which the energy is assumed nonexistent.'),
    '#element_validate' => array('_element_validate_number'),
  ) + $default;

  $opts = array(
    'Live' => 'Live storage - write directly to the field database.',
    'Deferred' => 'Deferred storage - write to a database table for scheduled processing.',
    'File' => 'File storage - write incidents to a file in php temporary directory',
  );

  if (class_exists('Memcache')) {
    $opts['Memcached'] = 'Memcached storage - write incidents to memcached';
  }

  $form['storage'] = array(
    '#type' => 'select',
    '#title' => t('Incident storage'),
    '#default_value' => $profile->storage,
    '#required' => TRUE,
    '#description' => t('Select the storage where all of the incidents are stored before processing them.'),
    '#options' => $opts,
  );
}







