<?php

class RadioactivityIncidentStorage {

  static $emitters;

  const REQUIRE_BOOTSTRAP = false;

  public function __construct() {
    if (!self::$emitters) {
      self::$emitters = array();
    }
  }

  /**
   * Add incident to the storage system
   * @entity_energy Energies by entity id
   */
  public function addIncident(RadioactivityIncident $incident) {
    // does nothing
  }

  /**
   * Process incidents in the storage
   * When this is run depends solely on cron timing & decay pattern granularity
   */
  public function processIncidents() {
    // nothing to do
  }

  /**
   * Default emitting for the default processEmitter function
   */
  public function addEmitter($entity_type, $bundle, $field_name, $entity_id, $energy) {

    if (self::$emitters == NULL)
      self::$emitters = array();

    if ($entity_id !== NULL) {

      if (!isset(self::$emitters[$entity_type][$bundle][$field_name][$entity_id]))
        self::$emitters[$entity_type][$bundle][$field_name][$entity_id] = 0;

      self::$emitters[$entity_type][$bundle][$field_name][$entity_id] += $energy;

    } else {

      return self::$emitters;

    }
  }


  /**
   * Default emitter processing via ajax callback path
   */
  public function processEmitters() {
    if (count(self::$emitters) > 0) {
      radioactivity_emitter_callback_data('default', self::$emitters);
    }
  }

  /**
   * Get decay profile attached to this instance
   */
  public function getDecayProfile() {
    return radioactivity_decay_profile_load($this->settings['profile']);
  }

}


