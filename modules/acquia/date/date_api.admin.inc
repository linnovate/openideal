<?php
// $Id: date_api.admin.inc,v 1.1.2.2 2009/03/19 23:22:32 karens Exp $

/**
 * @file
 * Administrative page callbacks for the date_api module.
 */

/**
 * Allow users to configure additional date format types.
 */
function date_api_date_formats_form($form_state) {
  // Add date_api.js and js settings.
  date_api_add_system_javascript();

  $form = array();

  // Display existing format types and their formats.
  date_api_date_format_form_elements($form);

  $form = system_settings_form($form);
  return $form;
}

/**
 * Allow users to configure custom date formats.
 */
function date_api_configure_custom_date_formats() {
  // Add date_api.js and js settings.
  date_api_add_system_javascript();
  $output = '';

  // Get list of custom date formats.
  $formats = date_get_formats('custom', TRUE);

  if (!empty($formats)) {
    $rows = array();
    foreach ($formats as $format => $format_info) {
      $display_text = date_format_date(date_now(), 'custom', $format);
      $delete_link = l(t('remove'), 'admin/settings/date-time/formats/delete/' . $format_info['dfid']);
      $row = array($display_text, $delete_link);
      $rows[] = $row;
    }
    $output = theme('table', array(), $rows);

  }
  else {
    $output = t('No custom formats configured. Please <a href="@link">add</a> some.', array('@link' => base_path() . 'admin/settings/date-time/formats/add'));
  }

  return $output;
}


/**
 * Allow users to add additional date formats.
 */
function date_api_add_date_formats_form($form_state) {
  // Add date_api.js and js settings.
  date_api_add_system_javascript();

  $form['add_date_format'] = array(
    '#type' => 'textfield',
    '#title' => t('Format string'),
    '#attributes' => array('class' => 'custom-format'),
    '#description' => t('A user-defined date format. See the <a href="@url">PHP manual</a> for available options. This format is currently set to display as <span>%date</span>.', array('@url' => 'http://php.net/manual/function.date.php', '%date' => date_format_date(date_now(), 'custom', '-'))),
  );


  $form['update'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
    '#weight' => 3,
    '#submit' => array('date_api_date_time_settings_submit'),
  );

  $form['#validate'][] = 'date_api_date_time_settings_validate';

  return $form;
}

/**
 * Add drop down selects for date format types.
 *
 * @param &$form
 *   The form being altered.
 */
function date_api_date_format_form_elements(&$form) {
  $form['date_formats'] = array(
    '#type' => 'fieldset',
    '#title' => t('Date formats'),
  );

  // Get list of all available date format types.
  $format_types = date_get_format_types('', TRUE);

  // Get list of all available date formats.
  $all_formats = array();
  $date_formats = date_get_formats('', TRUE); // Call this to rebuild the list, and to have default list.
  foreach ($date_formats as $type => $format_info) {
    $all_formats = array_merge($all_formats, $format_info);
  }
  $custom_formats = date_get_formats('custom');
  foreach ($format_types as $type => $type_info) {
    // If a system type, only show the available formats for that type and
    // custom ones.
    if ($type_info['locked'] == 1) {
      $formats = date_get_formats($type);
      if (empty($formats)) {
        $formats = $all_formats;
      }
      elseif (!empty($custom_formats)) {
        $formats = array_merge($formats, $custom_formats);
      }
    }
    // If a user configured type, show all available date formats.
    else {
      $formats = $all_formats;
    }

    $choices = array();
    foreach ($formats as $f => $format) {
      $choices[$f] = date_format_date(date_now(), 'custom', $f);
    }
    $default = variable_get('date_format_' . $type, array_shift(array_keys($formats)));

    // Get format type info for this format type.
    $type_info = date_get_format_types($type);

    date_api_date_format_select_field($form, $type, $type_info, $default, $choices, 1);
  }
}

function date_api_date_time_settings_validate($form, &$form_state) {
  $formats = date_get_formats('custom');
  if (!empty($formats) && in_array($form_state['values']['add_date_format'], array_keys($formats))) {
    form_set_error('add_date_format', t('This format already exists.  Please enter a unique format string.'));
  }
}

function date_api_date_time_settings_submit($form, &$form_state) {
  if (!empty($form_state['values']['add_date_format'])) {
    $format = array();
    $format['format'] = $form_state['values']['add_date_format'];
    $format['type'] = 'custom';
    $format['locked'] = 0;
    $format['is_new'] = 1;
    date_format_save($format);
  }

  // Unset, to prevent this getting saved as a variables.
  unset($form_state['values']['add_date_format']);

  drupal_set_message(t('Configuration saved.'));
}

/**
 * Menu callback; present a form for deleting a date format.
 */
function date_api_delete_format_form(&$form_state, $dfid) {
  $form = array();
  $form['dfid'] = array(
    '#type' => 'value',
    '#value' => $dfid,
  );
  $format = date_get_format($dfid);

  $output = confirm_form($form,
    t('Are you sure you want to remove the format %format?', array('%format' => date_format_date(date_now(), 'custom', $format['format']))),
    'admin/settings/date-time/formats/custom',
    t('This action cannot be undone.'),
    t('Remove'), t('Cancel'),
    'confirm'
  );

  return $output;
}

/**
 * Delete a configured date format.
 */
function date_api_delete_format_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $format = date_get_format($form_state['values']['dfid']);
    date_format_delete($form_state['values']['dfid']);
    drupal_set_message(t('Removed date format %format.', array('%format' => date_format_date(date_now(), 'custom', $format['format']))));
    $form_state['redirect'] = 'admin/settings/date-time/formats/custom';
  }
}



/**
 * Menu callback; present a form for deleting a date format type.
 */
function date_api_delete_format_type_form(&$form_state, $format_type) {
  $form = array();
  $form['format_type'] = array(
    '#type' => 'value',
    '#value' => $format_type,
  );
  $type_info = date_get_format_types($format_type);

  $output = confirm_form($form,
    t('Are you sure you want to remove the format type %format?', array('%format' => $type_info['title'])),
    'admin/settings/date-time/formats',
    t('This action cannot be undone.'),
    t('Remove'), t('Cancel'),
    'confirm'
  );

  return $output;
}

/**
 * Delete a configured date format type.
 */
function date_api_delete_format_type_form_submit($form, &$form_state) {
  if ($form_state['values']['confirm']) {
    $type_info = date_get_format_types($form_state['values']['format_type']);
    date_format_type_delete($form_state['values']['format_type']);
    drupal_set_message(t('Removed date format type %format.', array('%format' => $type_info['title'])));
    $form_state['redirect'] = 'admin/settings/date-time/formats';
  }
}

/**
 * Helper function; return form fields for date format selects.
 */
function date_api_date_format_select_field(&$form, $type, $type_info, $default, $choices, $show_remove = 0) {
  // Show date format select list.
  $form['date_formats']['date_format_' . $type] = array(
    '#prefix' => '<div class="date-container"><div class="select-container">',
    // Leave the date-container div open if we are going to be adding to and
    // then closing it below.
    '#suffix' => ($show_remove == 1 && $type_info['locked'] == 0) ? '</div>' : '</div></div>',
    '#type' => 'select',
    '#title' => t('!type date format', array('!type' => $type_info['title'])),
    '#attributes' => array('class' => 'date-format'),
    '#default_value' => (isset($choices[$default]) ? $default : 'custom'),
    '#options' => $choices,
  );

  // If this isn't a system provided type, allow the user to remove it from
  // the system.
  if ($show_remove == 1 && $type_info['locked'] == 0) {
    $form['date_formats']['date_format_' . $type . '_delete'] = array(
      '#prefix' => '<div class="date-format-delete">',
      '#suffix' => '</div></div>',
      '#value' => l(t('remove'), 'admin/settings/date-time/delete/' . $type),
    );
  }
}


