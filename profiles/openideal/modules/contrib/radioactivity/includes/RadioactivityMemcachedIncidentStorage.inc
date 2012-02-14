<?php


class RadioactivityMemcachedIncidentStorage extends RadioactivityIncidentStorage {

  const REQUIRE_BOOTSTRAP = FALSE;
  static $memcachedEmitters;  

  /**
   * Constructor
   * @param unknown_type $field_info
   */
  public function __construct() {
  }

  public function connect() {
    $mc = new Memcache();
    $mc->addServer(VAR_RADIOACTIVITY_MEMCACHED_HOST, VAR_RADIOACTIVITY_MEMCACHED_PORT);
    if (@$mc->connect(VAR_RADIOACTIVITY_MEMCACHED_HOST, VAR_RADIOACTIVITY_MEMCACHED_PORT))
      return $mc;
    return FALSE;
  }

  /**
   * Add incident to cache
   */
  public function addIncident($incident) {
    $mc = $this->connect();
    if (!$mc) {
      return;
    }
    $i = $mc->get("radioactivity_top");
    if (!$i) $i = 0;
    $mc->set("radioactivity_top", $i + 1);
    $mc->set("radioactivity_incident_" . $i, $incident); 
  }

  /**
   * Process incidents in the deferred queue 
   */
  public function processIncidents() {
    $mc = $this->connect();
    $i = $mc->get("radioactivity_bottom");
    if (!$i) $i = 0;
    // get and reset top
    $to = $mc->get("radioactivity_top");
    $mc->set("radioactivity_top", 0);
    if (!$to) $to = 0;
    while ($i < $to) {
      $incident = $mc->get("radioactivity_incident_" . $i);
      if ($incident->floodCheck()) {
        $incident->updateEnergy();
      }
      $i++;
    }
    $mc->set("radioactivity_bottom", 0);  
  }


  /**
   * Default emitting for the default processEmitter function
   */
  public function addEmitter($entity_id, $energy) {
    if (self::$memcachedEmitters == NULL)
      self::$memcachedEmitters = array();
    if ($entity_id !== NULL) {
      if (!isset(self::$memcachedEmitters[$this->entityType][$this->bundle][$this->fieldName][$entity_id]))
        self::$memcachedEmitters[$this->entityType][$this->bundle][$this->fieldName][$entity_id] = 0;
      self::$memcachedEmitters[$this->entityType][$this->bundle][$this->fieldName][$entity_id] += $energy;
    } else {
      return self::$memcachedEmitters;
    }
  }

  /**
   * Default emitter processing via ajax callback path
   */
  public function processEmitters() {
    if (count(self::$memcachedEmitters) > 0) {
      radioactivity_emitter_callback_data('memcache', self::$memcachedEmitters);
    }
  }

}


