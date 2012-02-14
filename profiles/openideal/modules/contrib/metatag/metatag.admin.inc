<?php

/**
 * @file
 * Administration page callbacks for the metatag module.
 */

function _metatag_config_sort($a, $b) {
  $return = NULL;
  $a_contexts = explode(':', $a->instance);
  $b_contexts = explode(':', $b->instance);
  for ($i = 0; $i < max(count($a_contexts), count($b_contexts)); $i++) {
    $a_context = isset($a_contexts[$i]) ? $a_contexts[$i] : '';
    $b_context = isset($b_contexts[$i]) ? $b_contexts[$i] : '';
    if ($a_context == $b_context) {
      continue;
    }
    elseif ($a_context == 'global') {
      $return = -1;
    }
    elseif ($a_context == '') {
      $return = -1;
    }
    else {
      $return = strcmp($a_context, $b_context);
    }
  }
  return $return;
}

function _metatag_config_overview_indent($text, $instance) {
  $parents = metatag_config_get_parent_instances($instance);
  array_shift($parents);

  // Add indentation to the leading cell.
  if (!empty($parents)) {
    $prefix = array_fill(0, count($parents), '<div class="indent">');
    $suffix = array_fill(0, count($parents), '</div>');
    $text = implode('', $prefix) . $text . implode('', $suffix);
  }

  return $text;
}

function metatag_config_overview() {
  ctools_include('export');

  $metatags = metatag_get_info('tags');

  $configs = ctools_export_crud_load_all('metatag_config');
  ksort($configs);
  //uasort($configs, '_metatag_config_sort');

  $rows = array();
  foreach ($configs as $config) {
    $row = array();

    // Style disabled configurations differently.
    if (!empty($config->disabled)) {
      $row['class'][] = 'disabled';
    }

    $details = '<div class="metatag-config-label collapsed"><a href="#" class="toggle-details">' . check_plain(metatag_config_instance_label($config->instance)) . '</a></div>';
    $details .= '<div class="metatag-config-details js-hide">';

    $inherits = array();
    $parents = metatag_config_get_parent_instances($config->instance);
    array_shift($parents);
    foreach (array_reverse($parents) as $parent) {
      if (!isset($configs[$parent])) {
        $rows[$parent] = array(
          _metatag_config_overview_indent('<div class="metatag-config-label">' . check_plain(metatag_config_instance_label($parent)) . '</div>', $parent),
          '',
        );
      }
      else {
        $inherits[$parent] = metatag_config_instance_label($parent);
        if (!empty($configs[$parent]->disabled)) {
          $inherits[$parent] .= ' ' . t('(disabled)');
        }
      }
    }

    // Show how this config inherits from its parents.
    if (!empty($inherits)) {
      $details .= '<div class="inheritance"><p>' . t('Inherits meta tags from: @parents', array('@parents' => implode(', ', $inherits))) . '</p></div>';
    }

    // Add a summary of the configuration's defaults.
    $summary = array();
    foreach ($config->config as $metatag => $data) {
      $summary[] = array(
        check_plain($metatags[$metatag]['label']) . ':',
        check_plain(metatag_get_value($metatag, $data, array('raw' => TRUE))),
      );
    }
    if (!empty($summary)) {
      $details .= theme('table', array(
        'rows' => $summary,
        'attributes' => array('class' => array('metatag-value-summary')),
      ));
    }
    else {
      $details .= '<p class="warning">No overridden default meta tags</p>';
      $row['class'][] = 'warning';
    }

    // Close the details div
    $details .= '</div>';

    // Add indentation to the leading cell based on how many parents the config has.
    $details = _metatag_config_overview_indent($details, $config->instance);

    $row['data']['details'] = $details;

    $operations = array();
    if (metatag_config_access('disable', $config)) {
      $operations['edit'] = array(
        'title' => ($config->export_type & EXPORT_IN_DATABASE) ? t('Edit') : t('Override'),
        'href' => 'admin/config/search/metatags/config/' . $config->instance,
      );
    }
    if (metatag_config_access('enable', $config)) {
      $operations['enable'] = array(
        'title' => t('Enable'),
        'href' => 'admin/config/search/metatags/config/' . $config->instance . '/enable',
        'query' => array(
          'token' => drupal_get_token('enable-' . $config->instance),
        ) + drupal_get_destination(),
      );
    }
    if (metatag_config_access('disable', $config)) {
      $operations['disable'] = array(
        'title' => t('Disable'),
        'href' => 'admin/config/search/metatags/config/' . $config->instance . '/disable',
        'query' => array(
          'token' => drupal_get_token('disable-' . $config->instance),
        ) + drupal_get_destination(),
      );
    }
    if (metatag_config_access('revert', $config)) {
      $operations['revert'] = array(
        'title' => t('Revert'),
        'href' => 'admin/config/search/metatags/config/' . $config->instance . '/revert',
      );
    }
    if (metatag_config_access('delete', $config)) {
      $operations['delete'] = array(
        'title' => t('Delete'),
        'href' => 'admin/config/search/metatags/config/' . $config->instance . '/delete',
      );
    }
    $operations['export'] = array(
      'title' => t('Export'),
      'href' => 'admin/config/search/metatags/config/' . $config->instance . '/export',
    );
    $row['data']['operations'] = array(
      'data' => array(
        '#theme' => 'links',
        '#links' => $operations,
        '#attributes' => array('class' => array('links', 'inline')),
      ),
    );

    $rows[$config->instance] = $row;
  }

  $build['config_table'] = array(
    '#theme' => 'table',
    '#header' => array(
      'type' => t('Type'),
      'operations' => t('Operations'),
    ),
    '#rows' => $rows,
    '#empty' => t('No meta tag defaults available yet.'),
    '#caption' => '<div class="js-show">' . t('To view a summary of the default meta tags and the inheritance, click on a meta tag type.') . '</div>',
    '#attributes' => array(
      'class' => array('metatag-config-overview'),
    ),
    '#attached' => array(
      'js' => array(
        drupal_get_path('module', 'metatag') . '/metatag.admin.js',
      ),
      'css' => array(
        drupal_get_path('module', 'metatag') . '/metatag.admin.css',
      ),
    ),
  );

  return $build;
}

/**
 * Build an FAPI #options array for the instance select field.
 */
function _metatag_config_instance_get_available_options() {
  $options = array();
  $instances = metatag_config_instance_info();

  foreach ($instances as $instance => $instance_info) {
    if (metatag_config_load($instance)) {
      continue;
    }
    $parents = metatag_config_get_parent_instances($instance, FALSE);
    array_shift($parents);
    if (!empty($parents)) {
      $parent = reset($parents);
      $parent_label = isset($instances[$parent]['label']) ? $instances[$parent]['label'] : t('Unknown');
      if (!isset($options[$parent_label])) {
        $options[$parent_label] = array();
        if (!metatag_config_load($parent)) {
          $options[$parent_label][$parent] = t('All');
        }
      }
      $options[$parent_label][$instance] = $instance_info['label'];
      unset($options[$parent]);
    }
    else {
      $options[$instance] = $instance_info['label'];
    }
  }

  return $options;
}

function metatag_config_add_form($form, &$form_state) {
  $form['instance'] = array(
    '#type' => 'select',
    '#title' => t('Type'),
    '#description' => t('Select the type of default meta tags you would like to add.'),
    '#options' => _metatag_config_instance_get_available_options(),
    '#required' => TRUE,
  );
  $form['config'] = array(
    '#type' => 'value',
    '#value' => array(),
  );

  $form['actions']['#type'] = 'actions';
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Add and configure'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/search/metatags',
  );

  return $form;
}

function metatag_config_add_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $config = (object) $form_state['values'];
  metatag_config_save($config);
  $form_state['redirect'] = 'admin/config/search/metatags/config/' . $config->instance;
}

function metatag_config_edit_form($form, &$form_state, $config) {
  $form['cid'] = array(
    '#type' => 'value',
    '#value' => !empty($config->cid) ? $config->cid : NULL,
  );
  $form['instance'] = array(
    '#type' => 'value',
    '#value' => $config->instance,
  );

  $contexts = explode(':', $config->instance);
  $options['context'] = $contexts[0];
  if ($contexts[0] != 'global') {
    $options['token types'] = array(token_get_entity_mapping('entity', $contexts[0]));
  }

  // Ensure that this configuration is properly compared to its parent 'default'
  // configuration values.
  if (count($contexts) > 1) {
    // If the config is something like 'node:article' or 'taxonomy_term:tags'
    // then the parent default config is 'node' or 'taxonomy_term'.
    $default_instance = $contexts;
    array_pop($default_instance);
    $default_instance = implode(':', $default_instance);
    $options['defaults'] = metatag_config_load_with_defaults($default_instance);
  }
  elseif ($contexts[0] != 'global') {
    // If the config is something like 'node' or 'taxonomy_term' then the
    // parent default config is 'global'.
    $options['defaults'] = metatag_config_load_with_defaults('global');
  }
  else {
    // If the config is 'global' than there are no parent defaults.
    $options['defaults'] = array();
  }

  metatag_metatags_form($form, $config->instance, $config->config, $options);
  $form['metatags']['#type'] = 'container';

  $form['actions']['#type'] = 'actions';
  $form['actions']['save'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  $form['actions']['cancel'] = array(
    '#type' => 'link',
    '#title' => t('Cancel'),
    '#href' => isset($_GET['destination']) ? $_GET['destination'] : 'admin/config/search/metatags',
  );

  $form['#submit'][] = 'metatag_config_edit_form_submit';
  return $form;
}

function metatag_config_edit_form_submit($form, &$form_state) {
  // Build the configuration object and save it.
  form_state_values_clean($form_state);
  $config = (object) $form_state['values'];
  // @todo Consider renaming the config field from 'config' to 'metatags'
  $config->config = $config->metatags;
  unset($config->metatags);
  metatag_config_save($config);

  $label = metatag_config_instance_label($config->instance);
  drupal_set_message(t('The meta tag defaults for @label have been saved.', array('@label' => $label)));

  $form_state['redirect'] = 'admin/config/search/metatags';
}

function metatag_config_enable($config) {
  if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], 'enable-' . $config->instance)) {
    return MENU_ACCESS_DENIED;
  }

  ctools_export_crud_enable('metatag_config', $config);

  $label = metatag_config_instance_label($config->instance);
  drupal_set_message(t('The meta tag defaults for @label have been enabled.', array('@label' => $label)));
  drupal_goto();
}

function metatag_config_disable($config) {
  if (!isset($_GET['token']) || !drupal_valid_token($_GET['token'], 'disable-' . $config->instance)) {
    return MENU_ACCESS_DENIED;
  }

  ctools_export_crud_disable('metatag_config', $config);

  $label = metatag_config_instance_label($config->instance);
  drupal_set_message(t('The meta tag defaults for @label have been disabed.', array('@label' => $label)));
  drupal_goto();
}

function metatag_config_delete_form($form, &$form_state, $config) {
  $form['cid'] = array('#type' => 'value', '#value' => $config->cid);
  $form['instance'] = array('#type' => 'value', '#value' => $config->instance);

  $label = metatag_config_instance_label($config->instance);
  $delete = metatag_config_access('delete', $config);
  $title = $delete ? t('Are you sure you want to delete the meta tag defaults for @label?', array('@label' => $label)) : t('Are you sure you want to revert the meta tag defaults for @label?', array('@label' => $label));

  return confirm_form(
    $form,
    $title,
    'admin/config/search/metatags',
    t('This action cannot be undone.')
  );
}

function metatag_config_delete_form_submit($form, &$form_state) {
  $config = metatag_config_load($form_state['values']['instance']);
  metatag_config_delete($config->instance);

  $label = metatag_config_instance_label($config->instance);
  $delete = metatag_config_access('delete', $config);
  $title = $delete ? t('The meta tag defaults for @label have been deleted.', array('@label' => $label)) : t('The meta tag defaults for @label have been reverted.', array('@label' => $label));
  drupal_set_message($title);

  $form_state['redirect'] = 'admin/config/search/metatags';
}

function metatag_config_export_form($config) {
  ctools_include('export');
  return drupal_get_form('ctools_export_form', ctools_export_crud_export('metatag_config', $config), t('Export'));
}
