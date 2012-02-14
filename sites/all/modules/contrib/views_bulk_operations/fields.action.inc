<?php
// $Id: fields.action.inc,v 1.1.2.32 2010/07/17 15:30:47 kratib Exp $
/**
 * @file Drupal action to set individual field values. 
 * 
 * Heavily "inspired" by fago's work in CCK on 'Populate a field' action (cck/includes/content.rules.inc).
 */

define('VBO_ACTION_FIELDS_ALL', '_all_');

function views_bulk_operations_fields_action_info() {
  if (!module_exists('content')) return array();
  return array('views_bulk_operations_fields_action' => array(
    'type' => 'node',
    'description' => t('Modify node fields'),
    'configurable' => TRUE,
    'behavior' => array('changes_node_property'),
    'form properties' => array('#field_info'),
  ));
}

function views_bulk_operations_fields_action_theme() {
  return array(
    'views_bulk_operations_fields_action_form' => array(
      'arguments' => array('form' => NULL),
    ),
  );
}

function views_bulk_operations_fields_action_form($context) {
  module_load_include('inc', 'content', 'includes/content.node_form');

  // This action form uses static-time settings. If they were not set, pull the defaults now.
  if (!isset($context['settings'])) $context['settings'] = views_bulk_operations_fields_action_views_bulk_operations_form_options();
  $form['#settings'] = $context['settings'];
  $form['#theme'] = 'views_bulk_operations_fields_action_form';
  $form['#node'] = (object)array('type' => NULL);
  $form_state = array();
  $weight = -100;
  if (isset($context['selection']) && isset($context['view'])) {
    $nids = array_map('_views_bulk_operations_get_oid', $context['selection'], array_fill(0, count($context['selection']), $context['view']->base_field));
    $result = db_query("SELECT DISTINCT type FROM {node} WHERE nid IN (%s)", implode(',', $nids));
  }
  else {
    $result = db_query("SELECT type from {node_type}");
  }
  $fields = array();
  while ($type = db_result($result)) {
    $type_info = content_types($type);
    $fields += $type_info['fields'];
    //$fields += _views_bulk_operations_fields_action_non_cck($type);
  }

  if (empty($fields)) {
    form_set_error('', t('The selected nodes do not have any editable fields. Please select other nodes and try again.'));
    return array();
  }

  foreach ($fields as $field) {
    if (module_exists('content_permissions') && 
        in_array('edit ' . $field['field_name'], module_invoke('content_permissions', 'perm')) && 
        !user_access('edit ' . $field['field_name'])) continue;
    if (!empty($context['settings']['display_fields']) && 
        !in_array($field['field_name'], $context['settings']['display_fields']) && 
        !in_array(VBO_ACTION_FIELDS_ALL, $context['settings']['display_fields'])) continue;

    $form['#node']->{$field['field_name']} = NULL;

    // The field info and widget.
    if (isset($field['field_form'])) { // is it our hacked definition?
      $form += (array)$field['field_form'];
    }
    else { // no, it's CCK
      $field['required'] = FALSE;
      $form += (array)content_field_form($form, $form_state, $field);
    }
    if (!isset($form[$field['field_name']])) continue;
    $form['#field_info'][$field['field_name']] = $field;
    
    // Adjust some settings on the field itself.
    $form[$field['field_name']]['#weight'] = $weight++;
    $form[$field['field_name']]['#prefix'] = '<div class="fields-action-togglable">' . @$form[$field['field_name']]['#prefix'];
    $form[$field['field_name']]['#suffix'] = @$form[$field['field_name']]['#suffix'] . '</div>';

    // Checkbox to enable/disable this field.
    $form[$field['field_name'] . '_check'] = array('#type' => 'checkbox', '#attributes' => array('class' => 'fields-action-toggler'));

    // Checkbox to add/overwrite values in this field.
    if ($field['multiple']) {
      $form[$field['field_name'] . '_add'] = array(
        '#type' => 'checkbox', 
        '#prefix' => '<div class="fields-action-togglable">', 
        '#suffix' => '</div>',
        '#attributes' => array(
          'title' => t('Check this box to add new values to this multi-valued field, instead of overwriting existing values.'),
        ),
      );
    }
    else {
      $form[$field['field_name'] . '_add'] = array('#type' => 'value', '#value' => FALSE);
    }

    // Default value.
    if (@$context[$field['field_name'] . '_check']) {
      $form[$field['field_name']]['#default_value'] = $context[$field['field_name']];
      $form[$field['field_name'] . '_check']['#default_value'] = TRUE;
    }

    // PHP code to program the value.
    if (user_access('Use PHP input for field settings (dangerous - grant with care)') && $context['settings']['php_code']) {
      if (isset($field['field_form'])) {
        $sample = t('return value;');
      }
      else {
        $db_info = content_database_info($field);
        $columns = array_keys($db_info['columns']);
        foreach ($columns as $key => $column) {
          $columns[$key] = t("'@column' => value for @column", array('@column' => $column));
        }
        $sample = t("return array(\n".
                    "  0 => array(\n    @columns\n  ),\n".
                    "  // You'll usually want to stop here. Provide more values\n".
                    "  // if you want your 'default value' to be multi-valued:\n".
                    "  1 => array(\n    @columns\n  ),\n".
                    "  2 => ...\n);", 
                    array('@columns' => implode(",\n    ", $columns))
        );
      }
      $form[$field['field_name'] . '_code'] = array(
        '#type' => 'textarea',
        '#default_value' => '',
        '#rows' => 6,
        '#description' => t('Expected format: <pre>!sample</pre>', array(
          '!sample' => $sample,
        )),
        '#prefix' => '<div class="fields-action-togglable">',
        '#suffix' => '</div>',
      );
    } 
  }

  // Special case for only one field: convert the checkbox into a value that's TRUE by default.
  if (count($form['#field_info']) == 1) {
    $field_name = key($form['#field_info']);
    $form[$field_name . '_check'] = array('#type' => 'value', '#value' => TRUE);
  }

  return $form;
}

function theme_views_bulk_operations_fields_action_form($form) {
  $output = '';
  if (module_exists('token')) {
    $output .= t('<h3>Using tokens</h3>
                  In text fields, you can use the following tokens:
                  <fieldset class="collapsed collapsible"><legend>Available tokens</legend>!tokens</fieldset>', array('!tokens' => theme('token_help', 'node'))
    );
    drupal_add_js('misc/collapse.js');
  }
  $access = user_access('Use PHP input for field settings (dangerous - grant with care)');
  if ($access && $form['#settings']['php_code']) {
    $output .= t('<h3>Using the Code widget</h3>
                  <ul>
                  <li>Will override the value specified in the Field widget.</li>
                  <li>Should include &lt;?php ?&gt; delimiters.</li>
                  <li>If in doubt, refer to <a target="_blank" href="@link_devel">devel.module</a> \'Dev load\' tab on a content page to figure out the expected format.</li>
                  </ul>', array('@link_devel' => 'http://www.drupal.org/project/devel')
    );
  }

  if (count($form['#field_info']) == 1) { // Special case for just one field: make the table more usable
    $field_name = key($form['#field_info']);
    $header = array();
    if ($form[$field_name . '_add']['#type'] == 'checkbox') {
      $row[] = drupal_render($form[$field_name . '_add']);
      $header[] = array('data' => t('Add?'), 'title' => t('Checkboxes in this column allow you to add new values to multi-valued fields, instead of overwriting existing values.'));
    }
    $row[] = drupal_render($form[$field_name]);
    $header[] = t('Field');
    if ($access && $form['#settings']['php_code']) {
      $row[] = drupal_render($form[$field_name . '_code']);
      $header[] = t('Code');
    }
    if (count($header) == 1) {
      $header = NULL;
    }
    $output .= theme('table', $header, array(array('class' => 'fields-action-row', 'id' => 'fields-action-row' . str_replace('_', '-', $field_name), 'data' => $row)));
  }
  else { // Many fields
    drupal_add_js(drupal_get_path('module', 'views_bulk_operations') . '/fields.action.js');

    $header = array(theme('table_select_header_cell'), t('Add?'), t('Field'));
    if ($access && $form['#settings']['php_code']) {
      $header[] = t('Code');
    }
    if (!empty($form['#field_info'])) foreach ($form['#field_info'] as $field_name => $field) {
      $row = array(
        'class' => 'fields-action-row',
        'id' => 'fields-action-row-' . str_replace('_', '-', $field_name), 
        'data' => array(
          drupal_render($form[$field_name . '_check']),
          drupal_render($form[$field_name . '_add']),
          drupal_render($form[$field_name]),
        ),
      );
      if ($access && $form['#settings']['php_code']) {
        $row['data'][] = drupal_render($form[$field_name . '_code']);
      }
      $rows[] = $row;
    }
    $output .= theme('table', $header, $rows); 
  }
  $output .= drupal_render($form);
  return $output;
}

function views_bulk_operations_fields_action_validate($form, $form_state) {
  $chosen = 0;
  foreach ($form['#field_info'] as $field_name => $field) {
    if ($form_state['values'][$field_name . '_check'] && isset($field['type'])) {
      $chosen++;
      $function = $field['module'] .'_field';
      if (function_exists($function)) {
        $form['#node']->$field_name = $form_state['values'][$field_name];
        $items = isset($form['#node']->$field_name) ? $form['#node']->$field_name : array();
        $function('validate', $form['#node'], $field, $items, $form, NULL);
        content_field('validate', $form['#node'], $field, $items, $form, NULL);
      }
    }
  }
  if (!$chosen) {
    form_set_error('', t('You must select at least one field to modify.'));
  }
}

function views_bulk_operations_fields_action_submit($form, $form_state) {
  $values = array();
  foreach ($form['#field_info'] as $field_name => $field) {
    $values[$field_name] = $form_state['values'][$field_name];
    $values[$field_name . '_check'] = $form_state['values'][$field_name . '_check'];
    $values[$field_name . '_add'] = $form_state['values'][$field_name . '_add'];
    if (isset($form_state['values'][$field_name . '_code'])) {
      $values[$field_name . '_code'] = $form_state['values'][$field_name . '_code'];
    }
  }
  $values['#field_info'] = $form['#field_info'];
  return $values;
}

function _views_bulk_operations_fields_action_non_cck($type) {
  module_load_include('inc', 'node', 'node.pages');

  global $user;
  $form_state = array('storage' => NULL, 'submitted' => FALSE);
  $form_id = $type . '_node_form';
  $node = array('uid' => $user->uid, 'name' => (isset($user->name) ? $user->name : ''), 'type' => $type, 'language' => '');
  $form = drupal_retrieve_form($form_id, $form_state, $node);
  drupal_prepare_form($form_id, $form, $form_state);

  $fields = array();
  foreach ($form['#content_extra_fields'] as $field_name => $value) {
    if (isset($form[$field_name]) && !in_array($form[$field_name]['#type'], array('hidden', 'value'))) {
      $fields[$field_name] = array(
        'field_name' => $field_name,
        'field_form' => array($field_name => $form[$field_name]),
        'widget' => array('label' => $value['label']),
        'multiple' => FALSE,
      );
    }
  }
  return $fields;
}

function views_bulk_operations_fields_action_views_bulk_operations_form_options() {
  $options['php_code'] = FALSE;
  $options['display_fields'] = array(VBO_ACTION_FIELDS_ALL);
  return $options;
}

function views_bulk_operations_fields_action_views_bulk_operations_form($options) {
  $form['php_code'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show PHP code area'),
    '#description' => t('Check this ON if you want to show a textarea for each field to allow the user to write a PHP script that will populate the value of this field.'),
    '#default_value' => $options['php_code'],
  );
  $fields = array(VBO_ACTION_FIELDS_ALL => t('- All fields -'));
  foreach (content_fields() as $field) {
    $fields[$field['field_name']] = $field['widget']['label'] .' ('. $field['field_name'] .')';
  }
  /*
  foreach (node_get_types() as $type => $info) {
    foreach (_views_bulk_operations_fields_action_non_cck($type) as $field) {
      if (!isset($fields[$field['field_name']])) {
        $fields[$field['field_name']] = $field['widget']['label'] .' ('. $field['field_name'] .')';
      }
    }
  }
  */
  if (empty($options['display_fields'])) {
    $options['display_fields'] = array(VBO_ACTION_FIELDS_ALL);
  }
  $form['display_fields'] = array(
    '#type' => 'select',
    '#title' => t('Display fields'),
    '#options' => $fields,
    '#multiple' => TRUE,
    '#description' => t('Select which field(s) the action form should present to the user.'),
    '#default_value' => $options['display_fields'],
  );
  return $form;
}

function views_bulk_operations_fields_action_views_bulk_operations_form_validate($form, $form_state) {
  if (empty($form_state['values']['display_fields'])) {
    form_set_error($form_state['values']['_error_element_base'] . 'display_fields', t('You must select at least one field to be shown to the user.'));
  }
}

function views_bulk_operations_fields_action(&$node, $context) {
  foreach ($context['#field_info'] as $field_name => $field) {
    if ($context[$field_name . '_check']) { // use the value for this field
      if (!empty($context[$field_name . '_code'])) {
        $value = eval('?>' . $context[$field_name . '_code']);
      }
      else {
        $value = $context[$field_name];
      }
      if ($context[$field_name . '_add']) {
        if (!is_array($node->$field_name) || empty($node->$field_name)) {
          $node->$field_name = array();
        }
        else {
          $function = $field['module'] .'_content_is_empty';
          foreach ($node->$field_name as $delta => $item) {
            if (call_user_func($function, $item, $field)) {
              unset($node->$field_name[$delta]);
            }
          }
          $node->$field_name = array_values($node->$field_name);
        }
        foreach ($value as $v_delta => $v_item) {
          if (is_string($v_delta)) {
            $node->$field_name[$v_delta] = _views_bulk_operations_fields_action_token($v_item, $node, $field, $v_delta);
          }
          else {
            $node->{$field_name}[] = _views_bulk_operations_fields_action_token($v_item, $node, $field, $v_delta);
          }
        }
/*
        $function = $field['module'] .'_content_is_empty';
        foreach ($node->$field_name as $delta => $item) {
          if (call_user_func($function, $item, $field)) {
            foreach ($value as $v_delta => $v_item) {
              if (!call_user_func($function, $v_item, $field)) {
                $node->{$field_name}[$delta] = $v_item;
                unset($value[$v_delta]);
                break;
              }
            }
          }
        }
*/
      }
      else {
        $node->$field_name = _views_bulk_operations_fields_action_token($value, $node, $field);
      }
      if ($field_name == 'name') { // special case: fix uid when given author name
        if ($account = user_load(array('name' => $node->name))) {
          $node->uid = $account->uid;
        }
        else {
          $node->uid = 0;
        }
      }
    }
  }
}

function _views_bulk_operations_fields_action_token($value, $node, $field, $delta = NULL) {
  if (module_exists('token')) {
    if (isset($delta) && isset($field['columns'])) {
      if (is_array($value)) foreach (array_keys($field['columns']) as $column) {
        if (isset($value[$column])) {
          $value[$column] = token_replace($value[$column], 'node', $node);
        }
      }
    }
    else if (is_array($value) && isset($field['columns'])) {
      foreach ($value as $v_delta => $v_item) {
        if (is_array($v_item)) foreach (array_keys($field['columns']) as $column) {
          if (isset($v_item[$column])) {
            $value[$v_delta][$column] = token_replace($v_item[$column], 'node', $node);
          }
        }
      }
    }
    else {
      $value = token_replace($value, 'node', $node);
    }
  }
  return $value;
}

