<?php
/**
 * Callback file for incident storages POSTs
 */

include "radioactivity-bootstrap.inc";

$payload = _radioactivity_validate_payload($_POST);

if (!$payload) {
  header('HTTP/1.1 400 Bad Request');
  print("Invalid post data - check configuration.");
  return;
}

$data = $payload['data'];
$data = unserialize($data);

require_once "includes/RadioactivityIncident.inc";
require_once "includes/RadioactivityIncidentStorage.inc";

if (is_array($data)) {
  foreach ($data as $storage => $entity_types) {
    $class = "Radioactivity" . ucfirst($storage) . "IncidentStorage";
    require_once "includes/" . $class . ".inc";
    if (!class_exists($class)) {
      // FIXME: throw an error?
      continue;
    }
    $storage = new $class();
    if ($class::REQUIRE_BOOTSTRAP) {
      // verify boostrapping
      _radioactivity_require_bootstrapping();
    }
    foreach ($entity_types as $entity_type => $entities) {
      foreach ($entities as $bundle => $fields) {
        foreach ($fields as $field_name => $languages) {
          foreach ($languages as $language => $ids) {
            foreach ($ids as $entity_id => $energy) {
              $incident = new RadioactivityIncident(
                  $entity_type,
                  $bundle,
                  $field_name,
                  $language,     
                  $entity_id,
                  $energy,
                  time()
              );
              $storage->addIncident($incident);
            }
          }
        }
      }
    }
  }
}


if (defined('DRUPAL_ROOT')) {
  // Clear field cache to reflect changes
  db_delete("cache_field")->execute();
}


