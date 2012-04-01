<?php

class RadioactivityLiveIncidentStorage extends RadioactivityIncidentStorage {

  const REQUIRE_BOOTSTRAP = true;

  /**
   * Constructor
   * @param unknown_type $field_info
   */
  public function __construct() {
    parent::__construct();
  }

  /**
   * Add incident directly to the db
   * @entity_energy Energy by entity id
   */
  public function addIncident(RadioactivityIncident $incident) {
    $incident->updateEnergy();
  }

}
