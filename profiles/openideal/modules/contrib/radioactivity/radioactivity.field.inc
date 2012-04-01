<?php


/**
 * Implementation of hook_field_info
 */
function radioactivity_field_info() {
  return array(
    RADIOACTIVITY_FIELD_TYPE => array(
      'label' => t('Radioactivity'),
      'description' => t('This field stores radioactivity in the database.'),
      'settings' => array(
      ),
      'instance_settings' => array(
        'profile' => 'default',
      ),
      'default_widget' => RADIOACTIVITY_BASIC_WIDGET,
      'default_formatter' => RADIOACTIVITY_COMBO_FORMATTER,
      'property_type' => 'decimal',
    ),
  );
}



/**
 * Implements hook_field_instance_settings_form().
 */
function radioactivity_field_instance_settings_form($field, $instance) {
  $settings = $instance['settings'];

  $form = array();

  if ($field['type'] == RADIOACTIVITY_FIELD_TYPE) {

    $profiles = radioactivity_get_decay_profile_options_list();

    $form['profile'] = array(
      '#type' => 'select',
      '#title' => t('Decay profile'),
      '#default_value' => $settings['profile'],
      '#required' => TRUE,
      '#description' => t('Select the decay profile for this instance. You can create and edit decay profiles !here.', array(
        "!here" => l("here", "admin/structure/radioactivity/profiles"))
      ),
      '#options' => $profiles,
    );

    if (count($profiles) == 0) {
      $form['profile']['#disabled'] = true;
    }

  }

  return $form;
}






/**
 * Implement hook_field_widget_info().
 */
function radioactivity_field_widget_info() {
  return array(
    RADIOACTIVITY_BASIC_WIDGET => array(
      'label' => t('Basic text field to edit radioactivity'),
      'field types' => array(RADIOACTIVITY_FIELD_TYPE),
    ),
  );
}



function radioactivity_field_formatter_info() {
  return array(
    RADIOACTIVITY_COMBO_FORMATTER => array(
      'label' => t('Radioactivity combo emitter + display'),
      'field types' => array(RADIOACTIVITY_FIELD_TYPE),
      'settings' => array(
        'energy' => 10,
        'type' => 'none',
      ),
    ),
  );
}



function _radioactivity_register_emitter($storage = null, $entity_type = null, $bundle = null, $field_name = null, $language = null, $entity_id = null, $energy = null) {
  static $cache = array(); 
  if ($storage == null)
    return $cache;
  @$cache[$storage][$entity_type][$bundle][$field_name][$language][$entity_id] = $energy;
}

/**
 * Implements hook_field_formatter_view().
 */
function radioactivity_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {

  $element = array();

  switch ($display['type']) {

    case RADIOACTIVITY_COMBO_FORMATTER:

      $entity_id = 0;

      $info = entity_get_info($entity_type);

      $id = $info['entity keys']['id'];

      $entity_id = $entity->{$id};
      
      $language = LANGUAGE_NONE;
      if ($field['translatable']) {
        $language = field_language($entity_type, $entity, $field['field_name']);
      } 

      if ($entity_id > 0) {
        $settings = $display['settings'];
        if ($settings['energy'] != 0) {

          $profile = radioctivity_get_field_profile($entity_type, $instance['bundle'], $field['field_name']);
          if ($profile && $profile->storageObject) {
            _radioactivity_register_emitter($profile->storage, $entity_type, $instance['bundle'], $field['field_name'], $language, $entity_id, $settings['energy']);
          } else {
            watchdog(
              "radioactivity",
              "Storage for field @field not found for @bundle. Please change it in the field configuration.",
              array("@field" => $field['field_name'], "@bundle" => $instance['bundle']),
              WATCHDOG_ERROR
            );
          }
        }
      }

      drupal_add_css(drupal_get_path('module', 'radioactivity') . '/css/radioactivity.css');
      switch ($settings['type']) {
        case 'none':
          break;

        case 'energy':
          foreach ($items as $delta => $item) {
            $element[$delta] = array(
              '#markup' => $item[RADIOACTIVITY_FIELD_ENERGY],
            );
          }
        break;

        case 'popularity':
          $maximum = ceil(_radioactivity_get_field_maximum($field['id'], $entity_type));
          foreach ($items as $delta => $item) {
            $energy = $item[RADIOACTIVITY_FIELD_ENERGY];
            $energy = $energy > 0 ? $energy : 1;
            $element[$delta] = array(
              '#markup' => round(($energy / $maximum) * 100),
            );
          }
        break;

        case 'gauge':
          $maximum = ceil(_radioactivity_get_field_maximum($field['id'], $entity_type));
          foreach ($items as $delta => $item) {
            $energy = $item[RADIOACTIVITY_FIELD_ENERGY];
            $energy = $energy > 0 ? $energy : 1;
            $element[$delta] = array(
              '#theme' => 'radioactivity_gauge',
              '#maximum' => $maximum,
              '#energy' => $energy,
            );
          }
        break;

      }

    break;
  }

  return $element;
}




/**
 * Implements hook_field_instance_settings_form().
 */
function radioactivity_field_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {

  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $form = array();

  if ($field['type'] == RADIOACTIVITY_FIELD_TYPE) {

    switch ($display['type']) {
      case RADIOACTIVITY_COMBO_FORMATTER:

        $form['energy'] = array(
          '#type' => 'textfield',
          '#title' => t('View incident energy'),
          '#default_value' => $settings['energy'],
          '#required' => TRUE,
          '#description' => t('Defines the energy emitted by viewing this field. Set to 0 to disable emit.'),
          '#element_validate' => array('_element_validate_number'),
        );

        $form['type'] = array(
          '#type' => 'select',
          '#title' => t('Type'),
          '#default_value' => $settings['type'],
          '#options' => radioactivity_combo_field_types(),
          '#required' => TRUE,
        );
      break;
    }

  }

  return $form;
}



function radioactivity_combo_field_types() {
  return array(
    'none' => t('Do not display energy'),
    'energy' => t('Raw numeric value'),
    'popularity' => t('Percentage'),
    'gauge' => t('Hotness gauge'),
  );
}



/**
 * Implements hook_field_formatter_settings_summary().
 */
function radioactivity_field_formatter_settings_summary($field, $instance, $view_mode) {
  $display = $instance['display'][$view_mode];
  $settings = $display['settings'];

  $types = radioactivity_combo_field_types();

  if ($settings['type'] != 'none') {
    $summary = t("Energy display: @type.", array("@type" => $types[$settings['type']]));
  } else {
    $summary = t("Energy display is disabled.");
  }

  if ($settings['energy'] != 0) {
    $summary .= " " . t('Emit @energy when visible.', array(
      '@energy' => $settings['energy'],
    ));
  } else {
    $summary .= " " . t('Emitter is disabled');
  }

  return $summary;
}



function radioactivity_page_alter($page) {

  // foreach emitters emitter->emit();
  $register = _radioactivity_register_emitter();
  module_load_include("inc", "radioactivity", "radioactivity-bootstrap");

  $path = base_path() . drupal_get_path('module', 'radioactivity') . '/emit.php';

  $fp = array(
    'enabled' => variable_get('radioactivity_flood_protection', false),
    'timeout' => variable_get('radioactivity_flood_timeout', 15),
  );

  if (count($register) > 0) {
    $register = serialize($register);
    $translated[$path] = _radioactivity_prepare_payload($register);
    drupal_add_library('system', 'jquery.cookie');
    drupal_add_js(drupal_get_path('module', 'radioactivity') .'/js/radioactivity.js');
    drupal_add_js(array('radioactivity' => array('emitters' => $translated)), 'setting');
    drupal_add_js(array('radioactivity' => array('flood_protection' => $fp)), 'setting');
  }
}


/**
 * Implement hook_field_validate().
 */
function radioactivity_field_validate($obj_type, $object, $field, $instance, $langcode, &$items, &$errors) {
  foreach($items as $delta => $item) {
    if (isset($item[RADIOACTIVITY_FIELD_ENERGY]) < 0) {
      $errors[$field['field_name']][$langcode][$delta][] = array(
        'error' => 'radioactivity_invalid',
        'message' => t('Value must be greater than 0'),
      );
    }
  }
}




/**
 * Implement hoo_field_is_empty().
 */
function radioactivity_field_is_empty($item, $field) {
  return strlen($item[RADIOACTIVITY_FIELD_ENERGY]) == 0;
}



function radioactivity_field_presave($entity_type, $entity, $field, $instance, $langcode, &$items) {
  if ($field['type'] == RADIOACTIVITY_FIELD_TYPE) {

    foreach ($items as $delta => $item) {

      // ensure values
      $items[$delta][RADIOACTIVITY_FIELD_TIMESTAMP] = time();

    }
  }
}



/**
 * Implement hook_field_widget().
 *
 * This widget displayed three text fields\, one each for red, green,
 * and blue. However, the field type defines a single text column,
 * rgb, which needs an HTML color spec. Define an element validate
 * handler that converts our r, g, and b fields into a simulaed single
 * 'rgb' form element.
 */
function radioactivity_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta = 0, $element) {

  if (!user_access("edit radioactivity")) {
    return $element;
  }

  if (isset($items[$delta]) && $items[$delta][RADIOACTIVITY_FIELD_TIMESTAMP] > 0) {
    $seconds = time() - $items[$delta][RADIOACTIVITY_FIELD_TIMESTAMP];
    $minutes = floor($seconds / 60);
    $hours   = floor($minutes / 60);
    $seconds = $seconds - ($minutes * 60);
    $minutes = $minutes - ($hours * 60);

    $time_ago = t(
    	"Radioactivity energy. Last emission @hour hours @min minutes and @sec seconds ago",
      array(
      	"@hour" => $hours,
      	"@min" => $minutes,
      	"@sec" => $seconds,
      )
    );
  } else {
    $time_ago = t("Radioactivity energy.");
  }

  $title = $instance['label'];
  $energy = isset($items[$delta][RADIOACTIVITY_FIELD_ENERGY]) ? $items[$delta][RADIOACTIVITY_FIELD_ENERGY] : 0;
  $timestamp = isset($items[$delta][RADIOACTIVITY_FIELD_TIMESTAMP]) ? $items[$delta][RADIOACTIVITY_FIELD_TIMESTAMP] : time();
  $element += array(

    '#type' => 'fieldset',
    '#group' => 'Radioactivity',
    '#title' => $title,

    RADIOACTIVITY_FIELD_ENERGY => array(
      '#type' => 'textfield',
      '#title' => t("Energy"),
      '#default_value' => $energy,
      '#description' => $time_ago,
    ),

    RADIOACTIVITY_FIELD_TIMESTAMP => array(
      '#type' => 'hidden',
      '#title' => t("Last emission"),
      '#default_value' => $timestamp,
    ),
  );

  return $element;
}




