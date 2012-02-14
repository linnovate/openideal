<?php
// $Id: nodewords_admin.admin.inc,v 1.1.2.4 2010/04/18 10:18:15 kiam Exp $

/**
 * @file
* Administration interface for nodewords.module.
 */

/**
 * Menu callback: settings form.
 */
function nodewords_admin_settings_form() {
  $admin_pages = array(
    NODEWORDS_TYPE_ERRORPAGE,
    NODEWORDS_TYPE_PAGE,
    NODEWORDS_TYPE_TERM,
    NODEWORDS_TYPE_VOCABULARY,
  );
  $form = array();
  $enabled_options = array();
  $metatags_options = array(
    'admin' => TRUE,
    'fieldset' => TRUE,
    'last fields' => array(
      'operations' => array(
        'reset_metatags' => array(
          '#type' => 'submit',
          '#value' => t('Reset default meta tag values'),
          '#submit' => array('nodewords_admin_settings_form_reset_metatags'),
        ),
      ),
    ),
    'title' => t('Default meta tags'),
    'weight' => 0,
  );
  $options = array();
  $tags_info = nodewords_get_possible_tags();

  nodewords_load_all_includes('nodewords.tags.inc');

  if (!empty($tags_info)) {
    foreach ($tags_info as $name => $info) {
      $options[$name] = '';

      $bool = (
        (
          !empty($info['context']['allowed']) &&
          is_array($info['context']['allowed']) &&
          count(array_intersect($admin_pages, $info['context']['allowed']))
        ) ||
        (
          !empty($info['context']['denied']) &&
          is_array($info['context']['denied']) &&
          (count(array_intersect($admin_pages, $info['context']['denied'])) != count($admin_pages))
        )
      );
      if ($bool || empty($info['context']['allowed']) || empty($info['context']['allowed'])) {
        $enabled_options[$name] = '';
      }
    }

    $form['enabled'] = array(
      '#type' => 'fieldset',
      '#title' => t('Enabled meta tags'),
      '#description' => t('These settings change the meta tags that will be output, and shown in edit forms.'),
      '#collapsible' => TRUE,
      '#weight' => 0,
      '#group' => 'nodewords',
    );

    $form['enabled']['output'] = array(
      '#type' => 'fieldset',
      '#title' => t('Meta tags to output'),
      '#description' => t('These settings change the meta tags that will be output. Meta tags are used in search indexes only if there are modules implementing the necessary code.'),
      '#collapsible' => TRUE,
      '#theme' => 'nodewords_admin_output_metatags',
    );

    $form['enabled']['output']['nodewords_head'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => variable_get('nodewords_head', array()),
      '#checkall' => variable_get('nodewords_enable_checkall', FALSE) ? 'nodewords-head-output' : FALSE,
    );

    $form['enabled']['output']['nodewords_search_index'] = array(
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => variable_get('nodewords_search_index', array()),
      '#checkall' => variable_get('nodewords_enable_checkall', FALSE) ? 'nodewords-search-index-output' : FALSE,
    );

    $form['enabled']['admin'] = array(
      '#type' => 'fieldset',
      '#title' => t('Meta tags enabled on administrative settings pages'),
      '#theme' => 'nodewords_admin_enabled_metatags',
      '#weight' => 0,
    );

    $select_options = array(
      'never' => t('Never'),
      'empty' => t('When the meta tag is empty'),
    );

    $form['enabled']['admin']['nodewords_admin_edit'] = array(
      '#type' => 'checkboxes',
      '#options' => $enabled_options,
      '#default_value' => variable_get('nodewords_admin_edit', array()),
      '#checkall' => variable_get('nodewords_enable_checkall', FALSE) ? 'nodewords-enabled-metatags-admin-edit-form' : FALSE,
    );

    foreach ($tags_info as $name => $info) {
      if (isset($enabled_options[$name])) {
        $form['enabled']['admin']['nodewords_admin_use_default_value_' . $name] = array(
          '#type' => 'select',
          '#default_value' => variable_get('nodewords_admin_use_default_value_' . $name, 'empty'),
          '#options' => $select_options,
          '#multiple' => FALSE,
        );
      }
    }
  }
  else {
    $form['nodewords_head'] = array(
      '#type' => 'value',
      '#value' => variable_get('nodewords_head', array()),
    );

    $form['nodewords_search_index'] = array(
      '#type' => 'value',
      '#value' => variable_get('nodewords_search_index', array()),
    );

    $form['nodewords_admin_edit'] = array(
      '#type' => 'value',
      '#value' => variable_get('nodewords_admin_edit', array()),
    );
  }

  $form['metatags_creation'] = array(
    '#type' => 'fieldset',
    '#title' => t('Meta tags creation options'),
    '#description' => t('These options change the way the meta tags are generated.'),
    '#collapsible' => TRUE,
    '#weight' => 4,
    '#group' => 'nodewords',
  );

  $form['metatags_creation']['nodewords_list_repeat'] = array(
    '#type' => 'checkbox',
    '#title' => t('Repeat meta tags for lists'),
    '#description' => t('Some search engines punish sites that use the same meta tags on different pages. Uncheck this option if you want to suppress the repetition of the same meta tags on pages that use the pager - if unchecked, Drupal will only display the meta tags for the first page and not for subsequent pages. If unsure, select this option.'),
     '#default_value' => variable_get('nodewords_list_repeat', FALSE),
  );

  $default_size = variable_get('nodewords_max_size', 350);

  $form['metatags_creation']['nodewords_max_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Maximum meta tags length'),
    '#description' => t('The maximum length to use for the meta tags form fields.'),
    '#default_value' => $default_size,
    '#required' => TRUE,
    '#element_validate' => array('nodewords_admin_max_size_validate'),
    '#size' => 6,
    '#maxlength' => 6,
  );

  $form['metatags_creation']['nodewords_base_url'] = array(
    '#type' => 'textfield',
    '#title' => t('Base URL'),
    '#description' => t('Enter the base URL that will be used for any URL generated by the module.'),
    '#default_value' => variable_get('nodewords_base_url', ''),
    '#element_validate' => array('nodewords_admin_base_url_validate'),
  );

  $form['third_party_integration'] = array(
    '#type' => 'fieldset',
    '#title' => t('Third party modules integration'),
    '#description' => t('These options enable the integration between Nodewords and other modules.'),
    '#collapsible' => TRUE,
    '#weight' => 8,
    '#group' => 'nodewords',
  );

  $form['third_party_integration']['nodewords_enable_checkall'] = array(
    '#type' => 'checkbox',
    '#title' => t(
      'Enable the integration with <a href="@checkall">checkall.module</a>',
      array('@checkall' => 'http://drupal.org/project/checkall')
    ),
    '#description' => t(
      'The integration with checkall.module allows to check/uncheck all the checkboxes of a set with a click.'
    ),
    '#default_value' => variable_get('nodewords_enable_checkall', FALSE),
  );

  if (!module_exists('checkall')) {
    $form['third_party_integration']['nodewords_enable_checkall']['#description'] .= ' ' .t(
      'The module checkall.module is not enabled, or installed.'
    );

    if (!variable_get('nodewords_enable_checkall', FALSE)) {
      $form['third_party_integration']['nodewords_enable_checkall']['#disabled'] = TRUE;
    }
  }

  $form['third_party_integration']['nodewords_enable_tokens'] = array(
    '#type' => 'checkbox',
    '#title' => t(
      'Enable the integration with <a href="@token">token.module</a>',
      array('@token' => 'http://drupal.org/project/token')
    ),
    '#description' => t(
      'The integration with token.module allows to use tokens in meta tags content.'
    ),
    '#default_value' => variable_get('nodewords_enable_tokens', TRUE),
  );

  if (!module_exists('token')) {
    $form['third_party_integration']['nodewords_enable_tokens']['#description'] .=  ' ' . t('The module token.module is not enabled, or installed.');

    if (!variable_get('nodewords_enable_tokens', TRUE)) {
      $form['third_party_integration']['nodewords_enable_tokens']['#disabled'] = TRUE;
    }
  }

  foreach ($tags_info as $name => $info) {
    $function = $info['callback'] . '_settings_form';
    $options = array(
      'parameters' => !empty($info['callback arguments']) ? $info['callback arguments'] : array(),
    );

    if (function_exists($function)) {
      $function($form, 'nodewords_admin_settings_form', $options);
    }
  }

  $form['buttons']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save configuration'),
  );
  $form['buttons']['reset'] = array(
    '#type' => 'submit',
    '#value' => t('Reset to defaults'),
    '#submit' => array('nodewords_admin_settings_form_reset'),
  );
  $form['buttons']['#weight'] = 100;

  $form['#pre_render'][] = 'vertical_tabs_form_pre_render';
  $form['#submit'][] = 'nodewords_admin_settings_form_submit';
  $form['#theme'] = 'nodewords_admin_settings_form';

  return $form;
}

/**
 * Function to validate the base URL.
 */
function nodewords_admin_base_url_validate($element, &$form_state) {
  if (!empty($element['#value']) && !($base_url = rtrim($element['#value'], '/'))) {
    form_error($element, t('Invalid base URL.'));
  }
  elseif (!empty($base_url) && !valid_url($base_url, TRUE)) {
    form_error($element, t('Invalid base URL.'));
  }
}

/**
 * Function to validate the value entered for the maximum meta tags length.
 *
 */
function nodewords_admin_max_size_validate($element, &$form_state) {
  if (!empty($element['#value'])) {
    $value = trim($element['#value']);

    if (empty($value) || (!empty($value) && (!is_numeric($value) || $value <= 0))) {
      form_error($element, t('The maximum length must be a number greater than zero.'));
    }
  }
}

/**
 * Submission function for the settings page.
 */
function nodewords_admin_settings_form_submit($form, &$form_state) {
  // Exclude unnecessary elements.
  unset(
    $form_state['values']['form_build_id'],
    $form_state['values']['form_id'],
    $form_state['values']['form_token'],
    $form_state['values']['op'],
    $form_state['values']['reset'],
    $form_state['values']['submit']
  );

  foreach ($form_state['values'] as $key => $value) {
    if (is_array($value) && isset($form_state['values']['array_filter'])) {
      $value = array_keys(array_filter($value));
    }

    variable_set($key, $value);
  }

  drupal_set_message(t('The configuration options have been saved.'));
  cache_clear_all();
  drupal_rebuild_theme_registry();
}

/**
 * Submission function for the reset button of the settings page.
 */
function nodewords_admin_settings_form_reset($form, &$form_state) {
  nodewords_delete_tags();

  // Exclude unnecessary elements.
  unset(
    $form_state['values']['form_build_id'],
    $form_state['values']['form_id'],
    $form_state['values']['form_token'],
    $form_state['values']['op'],
    $form_state['values']['reset'],
    $form_state['values']['submit']
  );

  foreach ($form_state['values'] as $key => $value) {
    variable_del($key);
  }

  drupal_set_message(t('The configuration options have been reset to their default values.'));
  cache_clear_all();
  drupal_rebuild_theme_registry();
}

/**
 * Front page settings form.
 */
function nodewords_admin_tags_edit_form(&$form_state, $options = array()) {
  $form = array();

  $options += _nodewords_get_default_metatags_type();

  $form['#nodewords_options'] = $options;
  $form['nodewords'] = nodewords_tags_edit_fields(
    $options,
    nodewords_load_tags($options),
    array(
      'admin' => TRUE,
      'fieldset' => FALSE,
    )
  );

  if (!empty($form['nodewords'])) {
    $form['nodewords']['#tree'] = TRUE;

    $form['submit'] = array(
      '#type' => 'submit',
      '#value' => t('Save configuration'),
      '#submit' => array('nodewords_admin_tags_edit_form_submit'),
      '#weight' => 40,
    );

    $form['reset'] = array(
      '#type' => 'submit',
      '#value' => t('Reset to defaults'),
      '#submit' => array('nodewords_admin_tags_edit_form_reset'),
      '#weight' => 42,
    );
  }

  return $form;
}

/**
 * Submission function for the meta tags edit page (reset button).
 */
function nodewords_admin_tags_edit_form_reset($form, &$form_state) {
  nodewords_delete_tags($form['#nodewords_options']);
  drupal_set_message(t('The configuration options have been reset to their default values.'));
}

/**
 * Submission function for the meta tags edit page.
 */
function nodewords_admin_tags_edit_form_submit($form, &$form_state) {
  nodewords_save_tags($form_state['values']['nodewords']['metatags'], $form['#nodewords_options']);
  drupal_set_message(t('The configuration options have been saved.'));
}

/**
 * Render the list of enabled meta tags in administration forms.
 */
function theme_nodewords_admin_enabled_metatags($form) {
  foreach (nodewords_get_possible_tags() as $name => $info) {
    $row = array();

    if (isset($form['nodewords_admin_edit'][$name])) {
      $row[] = $info['label'];
      $row[] = drupal_render($form['nodewords_admin_edit'][$name]);
      $row[] = drupal_render($form['nodewords_admin_use_default_value_' . $name]);

      $rows[] = $row;
    }
  }

  $header[] = t('Meta tag');
  $header[] = array(
    'data' => t('Show in edit form'),
    'class' => (variable_get('nodewords_enable_checkall', FALSE) && count($rows) > 1) ? 'nodewords-enabled-metatags-admin-edit-form' : '',
  );
  $header[] = t('Use the default value');

  return theme('table', $header, $rows) . drupal_render($form);
}

/**
 * Render the output settings fieldset.
 */
function theme_nodewords_admin_output_metatags($form) {
  foreach (nodewords_get_possible_tags() as $name => $info) {
    $row = array();

    $row[] = $info['label'];
    $row[] = drupal_render($form['nodewords_head'][$name]);
    $row[] = drupal_render($form['nodewords_search_index'][$name]);

    $rows[] = $row;
  }

  $header[] = t('Meta tag');
  $header[] = array(
    'data' => t('In HTML tag HEAD'),
    'class' => (variable_get('nodewords_enable_checkall', FALSE) && count($rows) > 1) ? 'nodewords-head-output' : '',
  );
  $header[] = array(
    'data' => t('In search index'),
    'class' => (variable_get('nodewords_enable_checkall', FALSE) && count($rows) > 1) ? 'nodewords-search-index-output' : '',
  );

  return theme('table', $header, $rows) . drupal_render($form);
}

function _nodewords_admin_mass_delete_batch_process($ids, $type, &$context) {
  if (!isset($context['sandbox']['progress'])) {
    $context['sandbox']['progress'] = 0;
    $context['sandbox']['max'] = count($ids);
    $context['sandbox']['ids'] = $ids;
  }

  $count = min(5, count($context['sandbox']['ids']));

  if (!isset($context['results']['count'])) {
    $context['results']['count'] = 0;
  }

  for ($i = 1; $i <= $count; $i++) {
    $id = array_shift($context['sandbox']['ids']);

    db_query("DELETE FROM {nodewords} WHERE type = %d AND id = %d", $type, $id);

    module_invoke_all('nodewords_delete_tags', array('type' => $type, 'id' => $id));

    $context['results']['count']++;
    $context['sandbox']['progress']++;
  }

  if ($context['sandbox']['progress'] != $context['sandbox']['max']) {
    $context['finished'] = $context['sandbox']['progress'] / $context['sandbox']['max'];
  }
}

function _nodewords_admin_mass_update_batch_finished($success, $results, $operations) {
  if ($success) {
    drupal_set_message(t('The update has been performed.'));
  }
  else {
    drupal_set_message(t('An error occurred and processing did not complete.'), 'error');

    $message = format_plural(
      $results['count'],
      '1 item successfully processed.',
      '@count items successfully processed.'
    );

    drupal_set_message($message);
  }
}
