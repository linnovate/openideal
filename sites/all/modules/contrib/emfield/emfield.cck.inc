<?php
// $Id: emfield.cck.inc,v 1.1.2.30.2.6 2010/07/21 20:12:01 aaron Exp $

/**
 * @file
 * Helper functions to implement our various cck-required functions,
 * such as hook_field and hook_widget.
 */

/**
 *  Helper function to consistantly define field columns.
 */
function _emfield_field_columns($field) {
  $columns = array(
    // This contains the original URL or embed code pre-parsing,
    // as entered by the user/editor.
    'embed' => array('type' => 'text', 'size' => 'big', 'not null' => TRUE, 'not null' => FALSE, 'sortable' => TRUE),
    // This contains the code used by the provider to identify the media.
    'value' => array('type' => 'varchar', 'length' => 255, 'not null' => TRUE, 'not null' => FALSE, 'sortable' => TRUE),
    // This is the actual provider used;
    // matches up with the specific provider.inc file.
    'provider' => array('type' => 'varchar', 'length' => 255, 'not null' => TRUE, 'not null' => FALSE, 'sortable' => TRUE),
    // An array for any extra data required by this media,
    // such as unique thumbnail requirements or rss feed data.
    'data' => array('type' => 'text', 'size' => 'big', 'not null' => TRUE, 'not null' => FALSE, 'sortable' => FALSE),
    // Some providers may have media that appears in results, but may be
    // unavailable or private.
    'status' => array('description' => "The availability status of this media.", 'type' => 'int', 'unsigned' => 'TRUE', 'not null' => TRUE, 'default' => EMFIELD_STATUS_AVAILABLE),
    // The version of the provider's data, as an integer. In general, this
    // number will be increased incrementally as changes to the API are
    // introduced, or the module introduces or requires new data.
    // Emfield uses this periodically in its updates.
    'version' => array('description' => "The version of the provider's data.", 'type' => 'int', 'unsigned' => TRUE, 'not null' => TRUE, 'default' => 0),
  );

  // Allow other modules to add new columns.
  $columns = array_merge($columns, module_invoke_all('emfield_field_columns_extra', $field));

  return $columns;
}

/**
 * Implementation of hook_emfield_field().
 * This in turn is a callback for hook_field().
 *
 * This private function is returned directly by emfield_emfield_field().
 */
function _emfield_emfield_field($op, &$node, $field, &$items, $teaser, $page, $module) {
  $return = array();
  switch ($op) {
    case 'validate':
      foreach ($items as $delta => $item) {
        $error_field = $field['multiple'] ? $field['field_name'] .']['. $delta .'][embed' : $field['field_name'];
        _emfield_field_validate_id($field, $item, $error_field, $module, $delta);
      }
      break;

    case 'sanitize':
      // Get the field ready for Previews.
      if (!empty($node->preview)) {
        foreach ($items as $delta => $item) {
          $items[$delta] = _emfield_field_submit_id($field, $item, $module);
        }
      }
      break;

    case 'presave':
      foreach ($items as $delta => $item) {
        $items[$delta] = _emfield_field_submit_id($field, $item, $module);
        // Remove empty fields
        if (empty($item['embed'])) {
          // Remove thumbnail files
          if (module_exists('emthumb')) {
            // see http://drupal.org/node/393490#comment-1328368
            if (is_array($item) && !empty($item['emthumb']['emthumb']['emthumb']['emthumb']['file'])) {
              _emthumb_file_delete($item['emthumb']['emthumb']['emthumb']['emthumb']['file'], $field['field_name']);
            }
          }
          unset($items[$delta]);
        }
      }
      break;

    case 'load':
      // We need to unserialize the 'data' column manually.
      $field_name = $field['field_name'];
      foreach ($items as $delta => $item) {
        $data = (array)unserialize($items[$delta]['data']);
        $items[$delta]['data'] = $data;
        $node->{$field_name}[$delta]['data'] = $data;
      }

      $return[$field_name] = $items;
      break;

    case 'delete':
      break;
  }
  // Allow other modules to modify the data before storing.
  if (in_array($op, array('insert', 'update'))) {
    foreach ($items as $delta => $item) {
      // Invoke all modules implementing hook_emfield_data_alter(&$data),
      // with the following parameters also sent.
      drupal_alter('emfield_data', $items[$delta]['data'], $module, $delta, $node, $field, $items);
    }
  }
  // Allow modules (such as emthumb) to alter our data.
  foreach (module_implements('emfield_field_extra') as $module) {
    $args = array($op, &$node, $field, &$items, $teaser, $page, $module);
    $ret = call_user_func_array($module .'_emfield_field_extra', $args);
    if (is_array($ret)) {
      $return = array_merge($return, $ret);
    }
  }
  if (in_array($op, array('insert', 'update'))) {
    foreach ($items as $delta => $item) {
      // We need to manually serialize the 'data' array.
      if(is_array($items[$delta]['data'])){
        $items[$delta]['data'] = serialize($items[$delta]['data']);
      }
    }
  }
  return $return;
}

function _emfield_emfield_widget_settings($op, $widget, $module) {
  switch ($op) {
    case 'form':
      // Make sure to register the new type as supported by this module.
      emfield_implement_types(FALSE);

      $form = array();
      $options = array();
      $providers = emfield_system_list($module);
      foreach ($providers as $provider) {
        if (variable_get('emfield_'. $module .'_allow_'. $provider->name, TRUE)) {
          $info = emfield_include_invoke($module, $provider->name, 'info');
          $options[$provider->name] = $info['name'];
        }
      }
      $form['provider_list'] = array(
        '#type' => 'fieldset',
        '#title' => t('Providers Supported'),
        '#description' => t('Select which third party providers you wish to allow for this content type from the list below. If no checkboxes are checked, then all providers will be supported. When a user submits new content, the URL they enter will be matched to the provider, assuming that provider is allowed here.'),
        '#collapsible' => TRUE,
        '#collapsed' => FALSE,
      );
      $form['provider_list']['providers'] = array(
        '#type' => 'checkboxes',
        '#title' => t('Providers'),
        '#default_value' => empty($widget['providers']) ? array() : $widget['providers'],
        '#options' => $options,
      );

      foreach (module_implements('emfield_widget_settings_extra') as $module) {
        $form[$module] = module_invoke($module, 'emfield_widget_settings_extra', 'form', $widget);
      }

      return $form;

    case 'save':
      $columns = array('providers'); //, 'helper_module', );
      foreach (module_implements('emfield_widget_settings_extra') as $module) {
        $columns = array_merge($columns, module_invoke($module, 'emfield_widget_settings_extra', 'save', $widget));
      }

      return $columns;
  }
}


/**
 * Helper function for emfield_emfield_widget, which in turn is a help function
 * for all emfields  that implement hook_widget().
 * This creates default widget handling for all the Embedded Media Fields.
 * Helper modules are expected to call this function to create the widget,
 * which will include a list of all providers as well as handle data parsing.
 */
function _emfield_emfield_widget(&$form, &$form_state, $field, $items, $delta = 0, $module) {
  // Our form element will need to be processed as a tree,
  // collapsing any children elements.
  $tree = array('#tree' => TRUE);

  $providers = emfield_allowed_providers($field, $module);
  $urls = array();
  $additional_form_elements = array();
  foreach ($providers as $provider) {
    // Only list providers allowed for this field. Honor global settings first.
    if (variable_get('emfield_'. $module .'_allow_'. $provider->name, TRUE)) {
      // Get the provider info.
      $info = emfield_include_invoke($module, $provider->name, 'info');
      // Grab the provider's URL.
      $urls[] = $info['url'] ? l($info['name'], $info['url'], array('attributes' => array('target' => '_blank'))) : $info['name'];
      // Allow the module to add any additional elements to the form,
      // based on individual provider needs.
      $item = isset($items[$delta]) ? $items[$delta] : NULL;
      $additional_element = emfield_include_invoke($module, $provider->name, 'form', $field, $item);
      if ($additional_element) {
        $additional_form_elements[$provider->name] = $additional_element;
      }
    }
  }

  // Set the widget description, but allow the field to override this.
  if (!(empty($field['widget']['description']))) {
    $textfield_description = t('!description', array('!description' => content_filter_xss($field['widget']['description'])));
  }
  else {
    $textfield_description = t('Enter the URL or Embed Code here. The embedded third party content will be parsed and displayed appropriately from this.');
  }

  // Add a list of all supported third party providers.
  $textfield_description .= '<br />'. t('The following services are provided: !urls', array('!urls' => implode(', ', $urls)));
  // Get the value of our data, if it's been set for this node.
  $embed = isset($items[$delta]['embed']) ? $items[$delta]['embed'] : '';
  $value = isset($items[$delta]['value']) ? $items[$delta]['value'] : '';
  $tree['embed'] = array(
    '#type' => 'textfield',
    '#title' => t('@label', array('@label' => $field['widget']['label'])),
    '#description' => $textfield_description,
    '#default_value' => $embed,
    '#required' => $delta == 0 ? $field['required'] : FALSE,
    '#maxlength' => 4096,
  );

  $tree['value'] = array(
    '#type' => 'value',
    '#value' => $embed,
  );

  if (!empty($additional_form_elements)) {
    foreach ($additional_form_elements as $key => $element) {
      $tree[$key] = $element;
    }
  }

  if ($value) {
    $info = emfield_include_invoke($module, $items[$delta]['provider'], 'info');
    $tree['value_markup'] = array(
      '#type' => 'item',
      '#value' => t('(@provider ID: !value)', array('@provider' => $info['provider'], '!value' => l($value, emfield_include_invoke($module, $info['provider'], 'embedded_link', $value, $items[$delta]['data']), array('target' => '_blank')))),
    );
  }

  // Allow other modules, such as Embedded Media Thumbnail,
  // to add additional elements to the widget.
  foreach (module_implements('emfield_widget_extra') as $module_extra) {
    // Use call_user_func_array() rather than module module_invoke() to
    // correctly pass the arguments as references.
    $function = $module_extra .'_emfield_widget_extra';
    $args = array(&$form, &$form_state, $field, $items, $delta, $module);
    $tree[$module_extra] = call_user_func_array($function, $args);

    // In Drupal 6, we need to build multipart/form-data forms manually.
    if (function_exists($module_extra. '_emfield_widget_extra_file_included')) {
      if (call_user_func($module_extra. '_emfield_widget_extra_file_included')) {
        $form['#attributes'] = array('enctype' => "multipart/form-data");
      }
    }
  }
  return $tree;
}

function _emfield_emfield_field_settings($op, $field) {
  switch ($op) {
    case 'views data':
      $table_alias = content_views_tablename($field);
      $data = content_views_field_views_data($field);

      // Add provider views support.
      $field_provider = $field['field_name'] .'_provider';
      $data[$table_alias][$field_provider] = $data[$table_alias][$field['field_name'] .'_embed'];
      $data[$table_alias][$field_provider]['title'] .= t(' - Provider');
      $data[$table_alias][$field_provider]['field']['title'] = $data[$table_alias][$field_provider]['title'];
      $data[$table_alias][$field_provider]['title short'] = t('Provider');
      $data[$table_alias][$field_provider]['field']['title short'] = $data[$table_alias][$field_provider]['title short'];
      $data[$table_alias][$field_provider]['field']['field'] = $field_provider;
      $data[$table_alias][$field_provider]['field']['handler'] = 'emfield_handler_field_provider';
      $data[$table_alias][$field_provider]['argument']['field'] = $field_provider;
      $data[$table_alias][$field_provider]['argument']['handler'] = 'emfield_handler_argument_provider';
      $data[$table_alias][$field_provider]['filter']['field'] = $field_provider;
      $data[$table_alias][$field_provider]['sort']['field'] = $field_provider;

      // Add status views support.
      $field_status = $field['field_name'] .'_status';
      $data[$table_alias][$field_status] = $data[$table_alias][$field['field_name'] .'_embed'];
      $data[$table_alias][$field_status]['title'] .= t(' - Status');
      $data[$table_alias][$field_status]['field']['title'] = $data[$table_alias][$field_status]['title'];
      $data[$table_alias][$field_status]['title short'] = t('Status');
      $data[$table_alias][$field_status]['field']['title short'] = $data[$table_alias][$field_status]['title short'];
      $data[$table_alias][$field_status]['field']['field'] = $field_status;
      $data[$table_alias][$field_status]['field']['handler'] = 'emfield_handler_field_status';
      $data[$table_alias][$field_status]['argument']['field'] = $field_status;
      $data[$table_alias][$field_status]['argument']['handler'] = 'content_handler_argument_number';
      $data[$table_alias][$field_status]['filter']['field'] = $field_status;
      $data[$table_alias][$field_status]['sort']['field'] = $field_status;
      return $data;
    case 'database columns':
      return emfield_field_columns($field);
  }
}
