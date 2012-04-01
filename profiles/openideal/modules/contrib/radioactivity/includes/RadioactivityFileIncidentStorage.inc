<?php


class RadioactivityFileIncidentStorage extends RadioactivityIncidentStorage {

  const REQUIRE_BOOTSTRAP=false;

  static $fileEmitters;  
  static $cronDone;

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
    $row = serialize($incident);
    $tmp = sys_get_temp_dir(); 
    $file = $tmp . "/radioactivity-payload.txt"; 
    $fh = fopen($file, "a+"); 
    fwrite($fh, $row . PHP_EOL); 
    fclose($fh);
  }

  /**
   * Process incidents in the deferred queue 
   */
  public function processIncidents() {

    // file incidents are all in the same storage so we'll do this only once
    // even though there might be many fields
    if (self::$cronDone) {
      return;
    }
    self::$cronDone = true;

    $tmp = sys_get_temp_dir(); 
   
    $file = $tmp . "/radioactivity-payload.txt"; 
     
    if (file_exists($file)) { 
      // get file contents and clear file 
      $fh = fopen($file, "r"); 
      $data = fread($fh, filesize($file)); 
      fclose($fh); 
      unlink($file); 
       
      $data = explode(PHP_EOL, $data); 
      foreach ($data as $row) { 
        if (empty($row)) 
          continue; 
        $incident = unserialize($row); 
        $incident->updateEnergy();
      } 
    }
  }

  /**
   * Default emitting for the default processEmitter function
   */
  public function addEmitter($entity_type, $bundle, $field_name, $entity_id, $energy) {
    if (self::$fileEmitters == NULL)
      self::$fileEmitters = array();

    if ($entity_id !== NULL) {

      if (!isset(self::$fileEmitters[$entity_type][$bundle][$field_name][$entity_id]))
        self::$fileEmitters[$entity_type][$bundle][$field_name][$entity_id] = 0;

      self::$fileEmitters[$entity_type][$bundle][$field_name][$entity_id] += $energy;

    } else {

      return self::$fileEmitters;

    }
  }

  /**
   * Default emitter processing via ajax callback path
   */
  public function processEmitters() {
    if (count(self::$fileEmitters) > 0) {
      radioactivity_emitter_callback_data('file', self::$fileEmitters);
    }
  }

}


