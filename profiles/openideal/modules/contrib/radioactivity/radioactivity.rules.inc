<?php

/**
 * Implements hook_rules_action_info()
 */
function radioactivity_rules_action_info() {
  $defaults = array(
    'group' => t('Radioactivity'),
    'access callback' => 'rules_user_integration_access',
  );

  // Emit energy
  $items['radioactivity_emit'] = $defaults + array(
    'label' => t('Add an incident to a field'),
    'base' => 'radioactivity_rules_action_emit',
    'parameter' => array(
      'data' => array(
        'type' => '*',
        'label' => t('Radioactivity field'),
        'description' => t('The radioactivity field in which you want to add an incident to'),
        'restriction' => 'selector',
        'wrapped' => TRUE,
        'allow null' => FALSE,
      ),
      'value' => array(
        'type' => 'decimal',
        'label' => t('Energy amount'),
        'description' => t('The variable which contains the amount of energy to be added to or substracted from the field'),
        'allow null' => TRUE,
        'optional' => TRUE,
      ),
    ),
  );

  // Set energy
  $items['radioactivity_set'] = $defaults + array(
    'label' => t('Set the energy of a field'),
    'base' => 'radioactivity_rules_action_set',
    'parameter' => array(
      'data' => array(
        'type' => '*',
        'label' => t('Radioactivity field'),
        'description' => t('The radioactivity field which you wish to set'),
        'restriction' => 'selector',
        'wrapped' => TRUE,
        'allow null' => FALSE,
      ),
      'value' => array(
        'type' => 'decimal',
        'label' => t('Energy level'),
        'description' => t('The variable which contains the new energy level for the field'),
        'allow null' => TRUE,
        'optional' => TRUE,
      ),
    ),
  );

  // Get maximum
  $items['radioactivity_maximum'] = $defaults + array(
    'label' => t('Get the maximum energy from a field'),
    'base' => 'radioactivity_rules_action_maximum',
    'parameter' => array(
      'data' => array(
        'type' => '*',
        'label' => t('Radioactivity field'),
        'description' => t('The radioactivity field from which you want the maximum energy to be retrieved from'),
        'restriction' => 'selector',
        'wrapped' => TRUE,
        'allow null' => FALSE,
      ),
    ),
    'provides' => array(
      'maximum_energy' => array(
        'type' => 'decimal',
        'label' => t('Maximum energy'),
      ),
    ),
  );

  return $items;
}


/**
 * Action: Emit energy
 */
function radioactivity_rules_action_emit($field, $energy) {
 
  $info = $field->info();

  $field_name = $info['name']; 
  $entity_type = $info['parent']->type();
  $bundle = $info['parent']->getBundle(); 
  $id = $info['parent']->getIdentifier();
  $lang = LANGUAGE_NONE;
 
  if ($info['translatable']) {
    global $language;
    $lang = $language->language;
  } 

  $profile = radioctivity_get_field_profile($entity_type, $bundle, $field_name);
  if ($profile && $profile->storageObject) {
    $profile->storageObject->addIncident(new RadioactivityIncident($entity_type, $bundle, $field_name, $lang, $id, $energy, time()));
  }
  field_cache_clear();
}


/**
 * Action: Set energy
 */
function radioactivity_rules_action_set($field, $energy) { 
  $info = $field->info();

  $field_name = $info['name'];
  $entity_type = $info['parent']->type();
  $bundle = $info['parent']->getBundle();
  $id = $info['parent']->getIdentifier();
  $lang = LANGUAGE_NONE;

  if ($info['translatable']) {
    global $language;
    $lang = $language->language;
  }

  _radioactivity_update_energy(
    $entity_type,
    $bundle,
    $field_name,
    $lang,
    $id,
    $energy,
    time(),
    true
  );

  field_cache_clear();
}



/**
 * Action: Get maximum energy
 */
function radioactivity_rules_action_maximum($field) {
  $info = $field->info();
  $field_id = $info['name']; 
  $entity_type = $info['parent']->type();
  $max = _radioactivity_get_field_maximum($field_id, $entity_type);
  return array('maximum_energy' => $max);
}


/**
 * User integration access callback.
 */
function radioactivity_rules_integration_access($type, $name) {
  return user_access('administer radioactivity');
}


