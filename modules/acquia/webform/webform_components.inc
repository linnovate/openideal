<?php
// $Id: webform_components.inc,v 1.9.2.32 2010/02/09 06:31:36 quicksketch Exp $

/**
 * @file
 *   Webform module components handling.
 */

/**
 * Provides interface and database handling for editing components of a webform.
 *
 * @author Nathan Haug <nate@lullabot.com>
 */

/**
 * Overview form of all components for this webform.
 */
function webform_components_form($form_state, $node) {
  $form = array(
    '#tree' => TRUE,
    '#node' => $node,
    'components' => array(),
  );

  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );

  $options = array();
  foreach ($node->webform['components'] as $cid => $component) {
    $options[$cid] = check_plain($component['name']);
    $form['components'][$cid]['cid'] = array(
      '#type' => 'hidden',
      '#default_value' => $component['cid'],
    );
    $form['components'][$cid]['pid'] = array(
      '#type' => 'hidden',
      '#default_value' => $component['pid'],
    );
    $form['components'][$cid]['weight'] = array(
      '#type' => 'textfield',
      '#size' => 4,
      '#title' => t('Weight'),
      '#default_value' => $component['weight'],
    );
    $form['components'][$cid]['mandatory'] = array(
      '#type' => 'checkbox',
      '#title' => t('Mandatory'),
      '#default_value' => $component['mandatory'],
      '#access' => !in_array($component['type'], array('markup', 'fieldset', 'pagebreak')),
    );
    $form['components'][$cid]['email'] = array(
      '#type' => 'checkbox',
      '#title' => t('E-mail'),
      '#default_value' => $component['email'],
      '#access' => !in_array($component['type'], array('markup', 'fieldset', 'pagebreak')),
    );
  }

  $form['add']['name'] = array(
    '#type' => 'textfield',
    '#size' => 24,
  );

  $component_types = webform_load_components();
  natcasesort($component_types);
  $form['add']['type'] = array(
    '#type' => 'select',
    '#options' => $component_types,
    '#weight' => 3,
    '#default_value' => (isset($_GET['cid']) && isset($node->webform['components'][$_GET['cid']])) ? $node->webform['components'][$_GET['cid']]['type'] : 'textfield',
  );
  $form['add']['mandatory'] = array(
    '#type' => 'checkbox',
  );
  $form['add']['email'] = array(
    '#type' => 'checkbox',
    '#default_value' => 1,
  );
  $form['add']['cid'] = array(
    '#type' => 'hidden',
    '#default_value' => '',
  );
  $form['add']['pid'] = array(
    '#type' => 'hidden',
    '#default_value' => (isset($_GET['cid']) && isset($node->webform['components'][$_GET['cid']])) ? $node->webform['components'][$_GET['cid']]['pid'] : 0,
  );
  $form['add']['weight'] = array(
    '#type' => 'weight',
    '#delta' => count($node->webform['components']) > 10 ? count($node->webform['components']) : 10,
    '#default_value' => (isset($_GET['cid']) && isset($node->webform['components'][$_GET['cid']])) ? $node->webform['components'][$_GET['cid']]['weight'] + 1 : count($node->webform['components']),
  );

  $form['add']['add'] = array(
    '#type' => 'submit',
    '#value' => t('Add'),
    '#weight' => 45,
  );

  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 45,
  );

  $form['publish'] = array(
    '#type' => 'submit',
    '#value' => t('Publish'),
    '#weight' => 50,
    '#access' => !$node->status,
  );

  return $form;
}

/**
 * Theme the node components form. Use a table to organize the components.
 *
 * @param $form
 *   The form array.
 * @return
 *   Formatted HTML form, ready for display.
 */
function theme_webform_components_form($form) {
  // Add CSS to display submission info. Don't preprocess because this CSS file is used rarely.
  drupal_add_css(drupal_get_path('module', 'webform') .'/webform.css', 'module', 'all', FALSE);
  drupal_add_js(drupal_get_path('module', 'webform') .'/webform.js', 'module', 'header', FALSE, TRUE, FALSE);

  drupal_add_tabledrag('webform-components', 'order', 'sibling', 'webform-weight');
  drupal_add_tabledrag('webform-components', 'match', 'parent', 'webform-pid', 'webform-pid', 'webform-cid');

  $node = $form['#node'];

  $headers = array(t('Name'), t('Type'), t('Value'), t('Mandatory'), t('E-mail'), t('Weight'), array('data' => t('Operations'), 'colspan' => 3));
  $rows = array();

  // Add a row containing form elements for a new item.
  unset($form['add']['name']['#title'], $form['add_type']['#description']);
  $form['add']['name']['#value'] = t('New component name');
  $form['add']['name']['#attributes']['class'] = 'webform-default-value';
  $form['add']['cid']['#attributes']['class'] = 'webform-cid';
  $form['add']['pid']['#attributes']['class'] = 'webform-pid';
  $form['add']['weight']['#attributes']['class'] = 'webform-weight';
  $row_data = array(
    drupal_render($form['add']['name']),
    drupal_render($form['add']['type']),
    '',
    drupal_render($form['add']['mandatory']),
    drupal_render($form['add']['email']),
    drupal_render($form['add']['cid']) . drupal_render($form['add']['pid']) . drupal_render($form['add']['weight']),
    array('colspan' => 3, 'data' => drupal_render($form['add']['add'])),
  );
  $add_form = array('data' => $row_data, 'class' => 'draggable webform-add-form');
  $form_rendered = FALSE;

  if (!empty($node->webform['components'])) {
    $component_tree = array();
    $page_count = 1;
    _webform_components_tree_build($node->webform['components'], $component_tree, 0, $page_count);
    $component_tree = _webform_components_tree_sort($component_tree);
    // Build the table rows.
    function _webform_components_form_rows($node, $cid, $component, $level, &$form, &$rows, &$add_form) {
      // Create presentable values.
      if (drupal_strlen($component['value']) > 30) {
        $component['value'] = drupal_substr($component['value'], 0, 30);
        $component['value'] .= '...';
      }
      $component['value'] = check_plain($component['value']);

      // Remove individual titles from the mandatory and weight fields.
      unset($form['components'][$cid]['mandatory']['#title']);
      unset($form['components'][$cid]['pid']['#title']);
      unset($form['components'][$cid]['weight']['#title']);
      unset($form['components'][$cid]['email']['#title']);

      // Add special classes for weight and parent fields.
      $form['components'][$cid]['cid']['#attributes']['class'] = 'webform-cid';
      $form['components'][$cid]['pid']['#attributes']['class'] = 'webform-pid';
      $form['components'][$cid]['weight']['#attributes']['class'] = 'webform-weight';

      // Build indentation for this row.
      $indents = '';
      for ($n = 1; $n <= $level; $n++) {
        $indents .= '<div class="indentation">&nbsp;</div>';
      }

      // Add each component to a table row.
      $row_data = array(
        $indents . filter_xss($component['name']),
        t($component['type']),
        ($component['value'] == '') ? '-' : $component['value'],
        drupal_render($form['components'][$cid]['mandatory']),
        drupal_render($form['components'][$cid]['email']),
        drupal_render($form['components'][$cid]['cid']) . drupal_render($form['components'][$cid]['pid']) . drupal_render($form['components'][$cid]['weight']),
        l(t('Edit'), 'node/'. $node->nid .'/edit/components/'. $cid),
        l(t('Clone'), 'node/'. $node->nid .'/edit/components/'. $cid .'/clone'),
        l(t('Delete'), 'node/'. $node->nid .'/edit/components/'. $cid .'/delete'),
      );
      $row_class = 'draggable'. ($component['type'] != 'fieldset' ? ' tabledrag-leaf' : '');
      $rows[] = array('data' => $row_data, 'class' => $row_class);
      if (isset($component['children']) && is_array($component['children'])) {
        foreach ($component['children'] as $cid => $component) {
          _webform_components_form_rows($node, $cid, $component, $level + 1, $form, $rows, $add_form);
        }
      }

      // Add the add form if this was the last edited component.
      if (isset($_GET['cid']) && $component['cid'] == $_GET['cid'] && $add_form) {
        $add_form['data'][0] = $indents . $add_form['data'][0];
        $rows[] = $add_form;
        $add_form = FALSE;
      }
    }
    foreach ($component_tree['children'] as $cid => $component) {
      _webform_components_form_rows($node, $cid, $component, 0, $form, $rows, $add_form);
    }
  }
  else {
    $rows[] = array(array('data' => t('No Components, add a component below.'), 'colspan' => 9));
  }

  // Append the add form if not already printed.
  if ($add_form) {
    $rows[] = $add_form;
  }

  $output = '';
  $output .= theme('table', $headers, $rows, array('id' => 'webform-components'));
  $output .= drupal_render($form);
  return $output;
}

function webform_components_form_validate($form, &$form_state) {
  if (isset($_POST['op']) && $_POST['op'] == t('Add') && drupal_strlen(trim($form_state['values']['add']['name'])) <= 0) {
    form_set_error('add][name', t('When adding a new component, the name field is required.'));
  }
}

function webform_components_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  // Update all mandatory and weight values.
  foreach ($node->webform['components'] as $cid => $component) {
    if ($component['pid'] != $form_state['values']['components'][$cid]['pid'] || $component['weight'] != $form_state['values']['components'][$cid]['weight'] || $component['mandatory'] != $form_state['values']['components'][$cid]['mandatory'] || $component['email'] != $form_state['values']['components'][$cid]['email']) {
      $component['weight'] = $form_state['values']['components'][$cid]['weight'];
      $component['mandatory'] = $form_state['values']['components'][$cid]['mandatory'];
      $component['email'] = $form_state['values']['components'][$cid]['email'];
      $component['pid'] = $form_state['values']['components'][$cid]['pid'];
      $component['nid'] = $node->nid;
      webform_component_update($component);
    }
  }

  if (isset($_POST['op']) && $_POST['op'] == t('Publish')) {
    $node->status = 1;
    node_save($node);
    drupal_set_message(t('Your webform has been published.'));
    return 'node/'. $node->nid;
  }
  elseif (isset($_POST['op']) && $_POST['op'] == t('Add')) {
    $component = $form_state['values']['add'];
    $form_state['redirect'] = array('node/'. $node->nid .'/edit/components/new/'. $component['type'], 'name='. urlencode($component['name']) .'&mandatory='. $component['mandatory'] .'&email='. $component['email'] .'&pid='. $component['pid'] .'&weight='. $component['weight']);
  }
  else {
    drupal_set_message(t('The component positions and mandatory values have been updated.'));

    // Since Webform components have been updated but the node itself has not
    // been saved, it is necessary to explicitly clear the cache to make sure
    // the updated webform is visible to anonymous users.
    cache_clear_all();
  }
}

function webform_component_edit_form(&$form_state, $node, $component, $clone = FALSE) {
  drupal_set_title(t('Edit component: @name (@type)', array('@name' => $component['name'], '@type' => t($component['type']))));

  // Print the correct field type specification.
  // We always need: name and description.
  $form = array(
    '#tree' => TRUE,
  );
  $form['type'] = array(
    '#type' => 'value',
    '#value' => $component['type'],
  );
  $form['nid'] = array(
    '#type' => 'value',
    '#value' => $node->nid,
  );
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => isset($component['cid']) ? $component['cid'] : NULL,
  );
  $form['clone'] = array(
    '#type' => 'value',
    '#value' => $clone,
  );
  $form['name'] = array(
    '#type' => 'textfield',
    '#default_value' => $component['name'],
    '#title' => t('Label'),
    '#description' => t('This is used as a descriptive label when displaying this form element.'),
    '#required' => TRUE,
    '#weight' => -2,
    '#maxlength' => 255,
  );
  $form['extra']['description'] = array(
    '#type' => 'textarea',
    '#default_value' => isset($component['extra']['description']) ? $component['extra']['description'] : '',
    '#title' => t('Description'),
    '#description' => t('A short description of the field used as help for the user when he/she uses the form.') . theme('webform_token_help'),
    '#weight' => -1,
  );

  $form['advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#tree' => FALSE,
    'weight' => 20,
  );
  $form['advanced']['form_key'] = array(
    '#type' => 'textfield',
    '#default_value' => empty($component['form_key']) ? _webform_safe_name($component['name']) : $component['form_key'],
    '#title' => t('Field Key'),
    '#description' => t('Enter a machine readable key for this form element. May contain only lowercase alphanumeric characters and underscores. This key will be used as the name attribute of the form element. This value has no effect on the way data is saved, but may be helpful if using Additional Processing or Validation code.'),
    '#required' => TRUE,
    '#weight' => -1,
  );
  $form['advanced']['mandatory'] = array(
    '#type' => 'checkbox',
    '#title' => t('Mandatory'),
    '#default_value' => ($component['mandatory'] == '1' ? TRUE : FALSE),
    '#description' => t('Check this option if the user must enter a value.'),
    '#weight' => 2,
    '#access' => !in_array($component['type'], array('pagebreak', 'fieldset')),
  );
  $form['advanced']['email'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include in e-mails'),
    '#default_value' => ($component['email'] == '1' ? TRUE : FALSE),
    '#description' => t('If checked, submitted values from this component will be included in e-mails.'),
    '#weight' => 2,
    '#access' => !in_array($component['type'], array('pagebreak', 'fieldset', 'markup')),
  );

  if (variable_get('webform_enable_fieldset', TRUE) && is_array($node->webform['components'])) {
    $options = array('0' => t('Root'));
    foreach ($node->webform['components'] as $existing_cid => $value) {
      if ($value['type'] == 'fieldset' && (!isset($component['cid']) || $existing_cid != $component['cid'])) {
        $options[$existing_cid] = $value['name'];
      }
    }
    $form['advanced']['pid'] = array(
      '#type' => 'select',
      '#title' => t('Parent Fieldset'),
      '#default_value' => $component['pid'],
      '#description' => t('Optional. You may organize your form by placing this component inside another fieldset.'),
      '#options' => $options,
      '#weight' => 3,
    );
  }

  $form['advanced']['weight'] = array(
    '#type' => 'textfield',
    '#size' => 4,
    '#title' => t('Weight'),
    '#default_value' => $component['weight'],
    '#description' => t('Optional. In the menu, the heavier items will sink and the lighter items will be positioned nearer the top.'),
    '#weight' => 4,
  );

  // Add the fields specific to this component type:
  webform_load_components(); // Load all component types.
  $edit_function = '_webform_edit_'. $component['type'];
  $additional_form_elements = array();
  if (function_exists($edit_function)) {
    $additional_form_elements = $edit_function($component); // Call the component render function.
  }
  else {
    drupal_set_message(t('The webform component of type @type does not have an edit function defined.', array('@type' => $component['type'])));
  }

  // Merge the additional fields with the current fields:
  if (isset($additional_form_elements['extra'])) {
    $extra_fields_copy = $form['extra'];
    $form['extra'] = array_merge($extra_fields_copy, $additional_form_elements['extra']);
    unset($additional_form_elements['extra']);
  }
  if (isset($additional_form_elements['advanced'])) {
    $advanced_fields_copy = $form['advanced'];
    $form['advanced'] = array_merge($advanced_fields_copy, $additional_form_elements['advanced']);
    unset($additional_form_elements['advanced']);
  }
  $form = array_merge($form, $additional_form_elements);

  // Add the submit button.
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#weight' => 5,
  );

  return $form;
}

/**
 * Field name validation for the webform unique key. Must be alphanumeric.
 */
function webform_component_edit_form_validate($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  if (!preg_match('!^[a-z0-9_]+$!', $form_state['values']['form_key'])) {
    form_set_error('form_key', t('The field key %field_key is invalid. Please include only lowercase alphanumeric characters and underscores.', array('%field_key' => $form_state['values']['form_key'])));
  }

  foreach ($node->webform['components'] as $cid => $component) {
    if (($component['cid'] != $form_state['values']['cid'] || $form_state['values']['clone']) && ($component['pid'] == $form_state['values']['pid']) && (strcasecmp($component['form_key'], $form_state['values']['form_key']) == 0)) {
      form_set_error('form_key', t('The field key %field_key is already in use by the field labeled %existing_field. Please use a unique key.', array('%field_key' => $form_state['values']['form_key'], '%existing_field' => $component['name'])));
    }
  }

  // Let the field do any additional validation.
  webform_load_components($form_state['values']['type']);
  $validate_function = '_webform_edit_validate_'. $form_state['values']['type'];
  if (function_exists($validate_function)) {
    $validate_function($form, $form_state);
  }
}

function webform_component_edit_form_submit($form, &$form_state) {
  $node = node_load($form_state['values']['nid']);

  // Remove empty extra values.
  if (isset($form_state['values']['extra'])) {
    foreach ($form_state['values']['extra'] as $key => $value) {
      if ($value  === '') {
        unset($form_state['values']['extra'][$key]);
      }
    }
  }

  // Remove empty attribute values.
  if (isset($form_state['values']['extra']['attributes'])) {
    foreach ($form_state['values']['extra']['attributes'] as $key => $value) {
      if ($value === '') {
        unset($form_state['values']['extra']['attributes'][$key]);
      }
    }
  }

  if ($form_state['values']['clone']) {
    drupal_set_message(t('Component %name cloned.', array('%name' => $form_state['values']['name'])));
    webform_component_clone($node, $form_state['values']);
  }
  elseif (!empty($form_state['values']['cid'])) {
    drupal_set_message(t('Component %name updated.', array('%name' => $form_state['values']['name'])));
    webform_component_update($form_state['values']);
  }
  else {
    drupal_set_message(t('New component %name added.', array('%name' => $form_state['values']['name'])));
    $cid = webform_component_insert($form_state['values']);
  }

  // Since Webform components have been updated but the node itself has not
  // been saved, it is necessary to explicitly clear the cache to make sure
  // the updated webform is visible to anonymous users.
  cache_clear_all();

  $form_state['redirect'] = array('node/'. $form_state['values']['nid'] .'/edit/components', isset($cid) ? 'cid='. $cid : '');
}

function webform_component_delete_form($form_state, $node, $component) {
  $cid = $component['cid'];

  $form = array();
  $form['node'] = array(
    '#type' => 'value',
    '#value' => $node,
  );
  $form['component'] = array(
    '#type' => 'value',
    '#value' => $component,
  );

  if ($node->webform['components'][$cid]['type'] == 'fieldset') {
    $question = t('Delete the %name fieldset?', array('%name' => $node->webform['components'][$cid]['name']));
    $description = t('This will immediately delete the %name fieldset and all children elements within %name from the %webform webform. This cannot be undone.', array('%name' => $node->webform['components'][$cid]['name'], '%webform' => $node->title));
  }
  else {
    $question = t('Delete the %name component?', array('%name' => $node->webform['components'][$cid]['name']));
    $description = t('This will immediately delete the %name component from the %webform webform. This cannot be undone.', array('%name' => $node->webform['components'][$cid]['name'], '%webform' => $node->title));
  }

  return confirm_form($form, $question, 'node/'. $node->nid .'/edit/components', $description, t('Delete'));
}

function webform_component_delete_form_submit($form, &$form_state) {
  drupal_set_message(t('Component %name deleted.', array('%name' => $form_state['values']['component']['name'])));
  webform_component_delete($form_state['values']['node'], $form_state['values']['component']);
  $form_state['redirect'] = 'node/'. $form_state['values']['node']->nid .'/edit/components';
}

/**
 * Insert a new component into the database.
 *
 * @param $component
 *   A full component containing fields from the component form.
 */
function webform_component_insert(&$component) {
  db_lock_table('webform_component');
  $component['cid'] = isset($component['cid']) ? $component['cid'] : db_result(db_query('SELECT MAX(cid) FROM {webform_component} WHERE nid = %d', $component['nid'])) + 1;
  $component['value'] = isset($component['value']) ? $component['value'] : NULL;
  $component['mandatory'] = isset($component['mandatory']) ? $component['mandatory'] : 0;
  db_query("INSERT INTO {webform_component} (nid, cid, pid, form_key, name, type, value, extra, mandatory, weight, email) VALUES (%d, %d, %d, '%s', '%s', '%s', '%s', '%s', %d, %d, %d)", $component['nid'], $component['cid'], $component['pid'], $component['form_key'], $component['name'], $component['type'], $component['value'], serialize($component['extra']), $component['mandatory'], $component['weight'], $component['email']);
  db_unlock_tables();
  return $component['cid'];
}

/**
 * Update an existing component with new values.
 *
 * @param $component
 *   A full component containing a nid, cid, and all other fields from the
 *   component form. Additional properties are stored in the extra array.
 */
function webform_component_update($component) {
  $component['value'] = isset($component['value']) ? $component['value'] : NULL;
  $component['mandatory'] = isset($component['mandatory']) ? $component['mandatory'] : 0;
  return db_query("UPDATE {webform_component} SET pid = %d, form_key = '%s', name = '%s', type = '%s', value = '%s', extra = '%s', mandatory = %d, weight = %d, email = %d WHERE nid = %d AND cid = %d", $component['pid'], $component['form_key'], $component['name'], $component['type'], $component['value'], serialize($component['extra']), $component['mandatory'], $component['weight'], $component['email'], $component['nid'], $component['cid']);
}

function webform_component_delete($node, $component) {
  // Check if a delete function is available for this component. If so,
  // load all submissions and allow the component to delete each one.
  $delete_function = '_webform_delete_'. $component['type'];
  if (function_exists($delete_function)) {
    module_load_include('inc', 'webform', 'webform_submissions');
    $submissions = webform_get_submissions($node->nid);
    foreach ($submissions as $submission) {
      if (isset($submission->data[$component['cid']])) {
        $delete_function($submission->data[$component['cid']], $component);
      }
    }
  }

  // Remove database entries.
  db_query('DELETE FROM {webform_component} WHERE nid = %d AND cid = %d', $node->nid, $component['cid']);
  db_query('DELETE FROM {webform_submitted_data} WHERE nid = %d AND cid = %d', $node->nid, $component['cid']);

  // Delete all elements under this element.
  $result = db_query('SELECT cid FROM {webform_component} WHERE nid = %d AND pid = %d', $node->nid, $component['cid']);
  while ($row = db_fetch_object($result)) {
    $component = $node->webform['components'][$row->cid];
    webform_component_delete($node, $component);
  }
}

/**
 * Recursively insert components into the database.
 * @param $node
 *   The node object containing the current webform.
 * @param $component
 *   A full component containing fields from the component form.
 */
function webform_component_clone(&$node, &$component) {
  $original_cid = $component['cid'];
  unset($component['cid']);
  $new_cid = webform_component_insert($component);
  if ($component['type'] == 'fieldset') {
    foreach ($node->webform['components'] as $cid => $child_component) {
      if ($child_component['pid'] == $original_cid) {
        $child_component['pid'] = $new_cid;
        webform_component_clone($node, $child_component);
      }
    }
  }
  return $new_cid;
}

/**
 * Populate a component with the defaults for that type.
 */
function webform_component_defaults(&$component) {
  webform_load_components();

  $function = '_webform_defaults_'. $component['type'];
  if (function_exists($function)) {
    $defaults = $function();
    foreach ($defaults as $key => $default) {
      if (!isset($component[$key])) {
        $component[$key] = $default;
      }
    }
    foreach ($defaults['extra'] as $extra => $default) {
      if (!isset($component['extra'][$extra])) {
        $component['extra'][$extra] = $default;
      }
    }
  }
}
