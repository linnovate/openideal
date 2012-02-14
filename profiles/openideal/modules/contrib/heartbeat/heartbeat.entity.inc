<?php

/**
 * Implement hook_entity_info().
 */
function heartbeat_entity_info() {
  $return = array(
    'heartbeat_activity' => array(
      'label' => t('Heartbeat Activity Entity'),
      'controller class' => 'HeartbeatActivityController',
      'base table' => 'heartbeat_activity',
      'uri callback' => 'heartbeat_activity_uri',
      'fieldable' => TRUE,
      'entity keys' => array(
        'id' => 'uaid',
        'bundle' => 'message_id',
        'label' => 'message_id',
      ),
      'bundle keys' => array(
        'bundle' => 'message_id',
      ),
      'bundles' => array(),
      'view modes' => array(
        'full' => array(
          'label' => t('Full activity message'),
          'custom settings' => FALSE,
        ),
      ),
    ),
  );
  // Bundles must provide a human readable name so we can create help and error
  // messages, and the path to attach Field admin pages to.
  foreach (heartbeat_templates_names() as $name => $description) {
    $return['heartbeat_activity']['bundles'][$name] = array(
      'label' => $description,
      'admin' => array(
        'path' => 'admin/structure/heartbeat/templates/list/%heartbeat_message_template',
        'real path' => 'admin/structure/heartbeat/templates/list/' . $name,
        'bundle argument' => 5,
        'access arguments' => array('admin heartbeat templates'),
      ),
    );
  }
  $return['heartbeat_activity_template'] = array(
    'label' => t('Heartbeat activity entity type'),
    'controller class' => 'HeartbeatTemplateController',
    'base table' => 'heartbeat_messages',
    'entity keys' => array(
      'id' => 'hid',
      'label' => 'message_id',
    ),
    'fieldable' => FALSE,
  );

  return $return;
}

/**
 * Entity uri callback.
 */
function heartbeat_activity_uri($heartbeatActivity) {
  return array(
    'path' => 'heartbeat/message/' . $heartbeatActivity->uaid,
  );
}

/**
 * Implements hook_field_extra_fields().
 */
function heartbeat_field_extra_fields() {

  $return = array();
  $info = entity_get_info('heartbeat_activity');
  foreach (array_keys($info['bundles']) as $bundle) {
    $return['heartbeat_activity'][$bundle] = array(
      'display' => array(
        'avatar' => array(
          'label' => 'Avatar',
          'description' => t('Heartbeat activity avatar element'),
          'weight' => 0,
        ),
        'message' => array(
          'label' => 'Message',
          'description' => t('Heartbeat activity message element'),
          'weight' => 1,
        ),
        'time' => array(
          'label' => 'Time',
          'description' => t('Heartbeat activity time element'),
          'weight' => 2,
        ),
        'buttons' => array(
          'label' => 'Buttons',
          'description' => t('Heartbeat activity buttons'),
          'weight' => 3,
        ),
        'attachments' => array(
          'label' => 'Attachments',
          'description' => t('Heartbeat activity attachments'),
          'weight' => 4,
        ),
      ),
    );
  }

  return $return;
}

/**
 * Controller class for heartbeat activity.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for heartbeat activity objects.
 */
class HeartbeatActivityController extends DrupalDefaultEntityController {

  protected function buildQuery($ids, $conditions = array(), $revision_id = FALSE) {
    $query = parent::buildQuery($ids, $conditions, $revision_id);
    $query->addTag('translatable');
    $query->addTag('heartbeat_activity_access');
    return $query;
  }

}

/**
 * Controller class for heartbeat templates.
 *
 * This extends the DrupalDefaultEntityController class, adding required
 * special handling for heartbeat template objects.
 */
class HeartbeatTemplateController extends DrupalDefaultEntityController {

  /**
   * Implements DrupalEntityControllerInterface::load().
   *
   * We make sure there is no query run, but ctools does this job for heartbeat.
   */
  public function load($ids = array(), $conditions = array()) {
    ctools_include('export');
    return ctools_export_load_object('heartbeat_templates', 'conditions', $conditions);
  }

}