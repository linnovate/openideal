<?php
/**
 * @file
 * fe_idea.features.field_base.inc
 */

/**
 * Implements hook_field_default_field_bases().
 */
function fe_idea_field_default_field_bases() {
  $field_bases = array();

  // Exported field_base: 'field_blast_idea'.
  $field_bases['field_blast_idea'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_blast_idea',
    'indexes' => array(
      'energy' => array(
        0 => 'radioactivity_energy',
      ),
    ),
    'locked' => 0,
    'module' => 'radioactivity',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'radioactivity',
  );

  // Exported field_base: 'field_buzz_idea'.
  $field_bases['field_buzz_idea'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_buzz_idea',
    'indexes' => array(
      'energy' => array(
        0 => 'radioactivity_energy',
      ),
    ),
    'locked' => 0,
    'module' => 'radioactivity',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'radioactivity',
  );

  // Exported field_base: 'field_challenge_ref'.
  $field_bases['field_challenge_ref'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_challenge_ref',
    'field_permissions' => array(
      'type' => 2,
    ),
    'indexes' => array(
      'target_entity' => array(
        0 => 'target_id',
      ),
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'type' => 'none',
        ),
        'target_bundles' => array(
          'challenge' => 'challenge',
        ),
      ),
      'handler_submit' => 'Change handler',
      'profile2_private' => FALSE,
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_groove_idea'.
  $field_bases['field_groove_idea'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_groove_idea',
    'indexes' => array(
      'energy' => array(
        0 => 'radioactivity_energy',
      ),
    ),
    'locked' => 0,
    'module' => 'radioactivity',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'radioactivity',
  );

  // Exported field_base: 'field_idea_image'.
  $field_bases['field_idea_image'] = array(
    'active' => 1,
    'cardinality' => 5,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_idea_image',
    'field_permissions' => array(
      'type' => 0,
    ),
    'indexes' => array(
      'fid' => array(
        0 => 'fid',
      ),
    ),
    'locked' => 0,
    'module' => 'image',
    'settings' => array(
      'default_image' => 0,
      'uri_scheme' => 'public',
    ),
    'translatable' => 0,
    'type' => 'image',
  );

  // Exported field_base: 'field_in_action'.
  $field_bases['field_in_action'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_in_action',
    'field_permissions' => array(
      'type' => 2,
    ),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'locked' => 0,
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        'Under Review' => 'Under Review',
        'Reviewed' => 'Reviewed',
        'Coming Soon' => 'Coming Soon',
        'Launched' => 'Launched',
        'Next Time' => 'Next Time',
      ),
      'allowed_values_function' => '',
      'profile2_private' => FALSE,
    ),
    'translatable' => 0,
    'type' => 'list_text',
  );

  // Exported field_base: 'field_merge_to'.
  $field_bases['field_merge_to'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_merge_to',
    'field_permissions' => array(
      'type' => 2,
    ),
    'indexes' => array(
      'target_entity' => array(
        0 => 'target_id',
      ),
      'target_id' => array(
        0 => 'target_id',
      ),
    ),
    'locked' => 0,
    'module' => 'entityreference',
    'settings' => array(
      'handler' => 'base',
      'handler_settings' => array(
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
        'sort' => array(
          'direction' => 'ASC',
          'field' => 'body:value',
          'property' => 'nid',
          'type' => 'none',
        ),
        'target_bundles' => array(
          'idea' => 'idea',
        ),
      ),
      'handler_submit' => 'Change handler',
      'target_type' => 'node',
    ),
    'translatable' => 0,
    'type' => 'entityreference',
  );

  // Exported field_base: 'field_social_buttons'.
  $field_bases['field_social_buttons'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_social_buttons',
    'field_permissions' => array(
      'type' => 0,
    ),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'social_buttons',
    'settings' => array(),
    'translatable' => 0,
    'type' => 'social_buttons',
  );

  // Exported field_base: 'field_vote'.
  $field_bases['field_vote'] = array(
    'active' => 1,
    'cardinality' => 1,
    'deleted' => 0,
    'entity_types' => array(),
    'field_name' => 'field_vote',
    'field_permissions' => array(
      'type' => 0,
    ),
    'indexes' => array(),
    'locked' => 0,
    'module' => 'vud_field',
    'settings' => array(
      'profile2_private' => FALSE,
      'votingapi_tag' => 'vote',
      'vud_widget' => 'updown',
    ),
    'translatable' => 0,
    'type' => 'vud_field',
  );

  return $field_bases;
}
