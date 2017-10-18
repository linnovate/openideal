<?php
/**
 * @file
 * fe_idea.features.field.inc
 */

/**
 * Implements hook_field_default_fields().
 */
function fe_idea_field_default_fields() {
  $fields = array();

  // Exported field: 'comment-comment_node_idea-comment_body'.
  $fields['comment-comment_node_idea-comment_body'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'comment',
      ),
      'field_name' => 'comment_body',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'text_long',
    ),
    'field_instance' => array(
      'bundle' => 'comment_node_idea',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'comment',
      'field_name' => 'comment_body',
      'label' => 'Comment',
      'required' => 1,
      'settings' => array(
        'text_processing' => '0',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '5',
        ),
        'type' => 'text_textarea',
        'weight' => 0,
      ),
    ),
  );

  // Exported field: 'comment-comment_node_idea-field_vote'.
  $fields['comment-comment_node_idea-field_vote'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_vote',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'vud_field',
      'settings' => array(
        'profile2_private' => FALSE,
        'votingapi_tag' => 'vote',
        'vud_widget' => 'updown',
      ),
      'translatable' => '0',
      'type' => 'vud_field',
    ),
    'field_instance' => array(
      'bundle' => 'comment_node_idea',
      'default_value' => array(
        0 => array(
          'vud_widget' => 'updown',
          'votingapi_tag' => 'vote',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'vud_field',
          'settings' => array(),
          'type' => 'default',
          'weight' => 1,
        ),
      ),
      'entity_type' => 'comment',
      'field_name' => 'field_vote',
      'label' => 'Vote',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
        'votingapi_tag' => 'vote',
        'vud_widget' => 'alternate',
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'vud_field',
        'settings' => array(),
        'type' => 'vud_field_default_widget',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'node-idea-body'.
  $fields['node-idea-body'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(
        0 => 'node',
      ),
      'field_name' => 'body',
      'foreign keys' => array(
        'format' => array(
          'columns' => array(
            'format' => 'format',
          ),
          'table' => 'filter_format',
        ),
      ),
      'indexes' => array(
        'format' => array(
          0 => 'format',
        ),
      ),
      'locked' => '0',
      'module' => 'text',
      'settings' => array(
        'profile2_private' => FALSE,
      ),
      'translatable' => '0',
      'type' => 'text_with_summary',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => 'Please share you idea.',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(),
          'type' => 'text_default',
          'weight' => '2',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'text',
          'settings' => array(
            'trim_length' => '300',
          ),
          'type' => 'text_trimmed',
          'weight' => '2',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'body',
      'label' => 'Describe your idea',
      'required' => 0,
      'settings' => array(
        'display_summary' => 0,
        'text_processing' => '1',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'text',
        'settings' => array(
          'rows' => '10',
          'summary_rows' => 5,
        ),
        'type' => 'text_textarea_with_summary',
        'weight' => '4',
      ),
    ),
  );

  // Exported field: 'node-idea-field_attachments'.
  $fields['node-idea-field_attachments'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '10',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_attachments',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'file',
      'settings' => array(
        'display_default' => 0,
        'display_field' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'file',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'file',
          'settings' => array(),
          'type' => 'file_default',
          'weight' => 12,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_attachments',
      'label' => 'Attachments',
      'required' => 0,
      'settings' => array(
        'description_field' => 0,
        'file_directory' => '',
        'file_extensions' => 'txt pdf doc docx ppt pptx',
        'max_filesize' => '',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'file',
        'settings' => array(
          'dragdropfile_enabled' => 1,
          'filefield_sources' => array(
            'filefield_sources' => array(
              'attach' => 0,
              'clipboard' => 0,
              'imce' => 0,
              'reference' => 0,
              'remote' => 0,
              'upload' => 'upload',
            ),
            'source_attach' => array(
              'absolute' => '0',
              'attach_mode' => 'move',
              'path' => 'file_attach',
            ),
            'source_imce' => array(
              'imce_mode' => '0',
            ),
            'source_reference' => array(
              'autocomplete' => '0',
            ),
          ),
          'progress_indicator' => 'throbber',
        ),
        'type' => 'file_generic',
        'weight' => '15',
      ),
    ),
  );

  // Exported field: 'node-idea-field_blast_idea'.
  $fields['node-idea-field_blast_idea'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_blast_idea',
      'foreign keys' => array(),
      'indexes' => array(
        'energy' => array(
          0 => 'radioactivity_energy',
        ),
      ),
      'locked' => '0',
      'module' => 'radioactivity',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'radioactivity',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => array(
        0 => array(
          'radioactivity_energy' => '1000',
          'radioactivity_timestamp' => '1332403825',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '6',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '8',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_blast_idea',
      'label' => 'Blast Idea',
      'required' => 0,
      'settings' => array(
        'history' => 0,
        'history_limit' => 8,
        'profile' => 'blast_idea',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'radioactivity',
        'settings' => array(),
        'type' => 'radioactivity_basic_widget',
        'weight' => '9',
      ),
    ),
  );

  // Exported field: 'node-idea-field_buzz_idea'.
  $fields['node-idea-field_buzz_idea'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_buzz_idea',
      'foreign keys' => array(),
      'indexes' => array(
        'energy' => array(
          0 => 'radioactivity_energy',
        ),
      ),
      'locked' => '0',
      'module' => 'radioactivity',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'radioactivity',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => array(
        0 => array(
          'radioactivity_energy' => '1000',
          'radioactivity_timestamp' => '1332403886',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '8',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '7',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_buzz_idea',
      'label' => 'Buzz Idea',
      'required' => 0,
      'settings' => array(
        'history' => 0,
        'history_limit' => 8,
        'profile' => 'buzz_idea',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'radioactivity',
        'settings' => array(),
        'type' => 'radioactivity_basic_widget',
        'weight' => '10',
      ),
    ),
  );

  // Exported field: 'node-idea-field_category'.
  $fields['node-idea-field_category'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_category',
      'foreign keys' => array(
        'tid' => array(
          'columns' => array(
            'tid' => 'tid',
          ),
          'table' => 'taxonomy_term_data',
        ),
      ),
      'indexes' => array(
        'tid' => array(
          0 => 'tid',
        ),
      ),
      'locked' => '0',
      'module' => 'taxonomy',
      'settings' => array(
        'allowed_values' => array(
          0 => array(
            'vocabulary' => 'categories',
            'parent' => '0',
          ),
        ),
        'profile2_private' => FALSE,
      ),
      'translatable' => '0',
      'type' => 'taxonomy_term_reference',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '0',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'taxonomy',
          'settings' => array(),
          'type' => 'taxonomy_term_reference_link',
          'weight' => '0',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_category',
      'label' => 'Category',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '2',
      ),
    ),
  );

  // Exported field: 'node-idea-field_challenge_ref'.
  $fields['node-idea-field_challenge_ref'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_challenge_ref',
      'field_permissions' => array(
        'type' => '2',
      ),
      'foreign keys' => array(
        'node' => array(
          'columns' => array(
            'target_id' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'target_entity' => array(
          0 => 'target_id',
        ),
        'target_id' => array(
          0 => 'target_id',
        ),
      ),
      'locked' => '0',
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
      'translatable' => '0',
      'type' => 'entityreference',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => NULL,
      'default_value_function' => 'entityreference_prepopulate_field_default_value',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '5',
        ),
        'teaser' => array(
          'label' => 'above',
          'module' => 'entityreference',
          'settings' => array(
            'link' => 1,
          ),
          'type' => 'entityreference_label',
          'weight' => '4',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_challenge_ref',
      'label' => 'Is Part Of A Challenge',
      'required' => 0,
      'settings' => array(
        'behaviors' => array(
          'prepopulate' => array(
            'action' => 'hide',
            'action_on_edit' => 0,
            'fallback' => 'none',
            'og_context' => 0,
            'skip_perm' => '0',
            'status' => 1,
          ),
        ),
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '8',
      ),
    ),
  );

  // Exported field: 'node-idea-field_groove_idea'.
  $fields['node-idea-field_groove_idea'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_groove_idea',
      'foreign keys' => array(),
      'indexes' => array(
        'energy' => array(
          0 => 'radioactivity_energy',
        ),
      ),
      'locked' => '0',
      'module' => 'radioactivity',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'radioactivity',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => array(
        0 => array(
          'radioactivity_energy' => '1000',
          'radioactivity_timestamp' => '1332403976',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '9',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '6',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_groove_idea',
      'label' => 'Groove Idea',
      'required' => 0,
      'settings' => array(
        'history' => 0,
        'history_limit' => 8,
        'profile' => 'groove_idea',
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'radioactivity',
        'settings' => array(),
        'type' => 'radioactivity_basic_widget',
        'weight' => '11',
      ),
    ),
  );

  // Exported field: 'node-idea-field_idea_image'.
  $fields['node-idea-field_idea_image'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '5',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_idea_image',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(
        'fid' => array(
          'columns' => array(
            'fid' => 'fid',
          ),
          'table' => 'file_managed',
        ),
      ),
      'indexes' => array(
        'fid' => array(
          0 => 'fid',
        ),
      ),
      'locked' => '0',
      'module' => 'image',
      'settings' => array(
        'default_image' => 0,
        'uri_scheme' => 'public',
      ),
      'translatable' => '0',
      'type' => 'image',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'above',
          'module' => 'image',
          'settings' => array(
            'image_link' => '',
            'image_style' => '',
          ),
          'type' => 'image',
          'weight' => 11,
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_idea_image',
      'label' => 'Idea image',
      'required' => 0,
      'settings' => array(
        'alt_field' => 0,
        'default_image' => 0,
        'file_directory' => '',
        'file_extensions' => 'png gif jpg jpeg',
        'max_filesize' => '',
        'max_resolution' => '',
        'min_resolution' => '',
        'title_field' => 0,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'image',
        'settings' => array(
          'dragdropfile_enabled' => 1,
          'filefield_sources' => array(
            'filefield_sources' => array(
              'attach' => 0,
              'clipboard' => 0,
              'imce' => 0,
              'reference' => 0,
              'remote' => 0,
              'upload' => 'upload',
            ),
            'source_attach' => array(
              'absolute' => '0',
              'attach_mode' => 'move',
              'path' => 'file_attach',
            ),
            'source_imce' => array(
              'imce_mode' => '0',
            ),
            'source_reference' => array(
              'autocomplete' => '0',
            ),
          ),
          'preview_image_style' => 'thumbnail',
          'progress_indicator' => 'throbber',
        ),
        'type' => 'image_image',
        'weight' => '5',
      ),
    ),
  );

  // Exported field: 'node-idea-field_in_action'.
  $fields['node-idea-field_in_action'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_in_action',
      'field_permissions' => array(
        'type' => '2',
      ),
      'foreign keys' => array(),
      'indexes' => array(
        'value' => array(
          0 => 'value',
        ),
      ),
      'locked' => '0',
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
      'translatable' => '0',
      'type' => 'list_text',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '3',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'list',
          'settings' => array(),
          'type' => 'list_default',
          'weight' => '3',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_in_action',
      'label' => 'In Action',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'options',
        'settings' => array(),
        'type' => 'options_select',
        'weight' => '6',
      ),
    ),
  );

  // Exported field: 'node-idea-field_merge_to'.
  $fields['node-idea-field_merge_to'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_merge_to',
      'field_permissions' => array(
        'type' => '2',
      ),
      'foreign keys' => array(
        'node' => array(
          'columns' => array(
            'target_id' => 'nid',
          ),
          'table' => 'node',
        ),
      ),
      'indexes' => array(
        'target_entity' => array(
          0 => 'target_id',
        ),
        'target_id' => array(
          0 => 'target_id',
        ),
      ),
      'locked' => '0',
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
      'translatable' => '0',
      'type' => 'entityreference',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '7',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => '9',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_merge_to',
      'label' => 'Merge to',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 1,
        'module' => 'entityreference',
        'settings' => array(
          'match_operator' => 'CONTAINS',
          'path' => '',
          'size' => '60',
        ),
        'type' => 'entityreference_autocomplete',
        'weight' => '7',
      ),
    ),
  );

  // Exported field: 'node-idea-field_social_buttons'.
  $fields['node-idea-field_social_buttons'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_social_buttons',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'social_buttons',
      'settings' => array(),
      'translatable' => '0',
      'type' => 'social_buttons',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => NULL,
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'social_buttons',
          'settings' => array(),
          'type' => 'social_buttons_formatter_default',
          'weight' => '10',
        ),
        'teaser' => array(
          'label' => 'above',
          'settings' => array(),
          'type' => 'hidden',
          'weight' => 0,
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_social_buttons',
      'label' => 'Social Buttons',
      'required' => 0,
      'settings' => array(
        'button-0' => '<div id="fb-root"></div><script> (function(d, s, id) { var js, fjs = d.getElementsByTagName(s)[0]; if (d.getElementById(id)) return; js = d.createElement(s); js.id = id; js.src = "//connect.facebook.net/en_US/all.js#xfbml=1"; fjs.parentNode.insertBefore(js, fjs);}(document, "script", "facebook-jssdk"));</script> <div class="fb-like" data-send="false" data-layout="button_count" data-width="1" data-show-faces="false"></div>',
        'button-1' => '<a href="https://twitter.com/share" class="twitter-share-button">Tweet</a><script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0];if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src="//platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>',
        'button-2' => '<div class="g-plusone" data-size="medium"></div><script>(function() {var po = document.createElement("script"); po.type = "text/javascript"; po.async = true;po.src = "https://apis.google.com/js/plusone.js";var s = document.getElementsByTagName("script")[0]; s.parentNode.insertBefore(po, s);})();</script>',
        'button-3' => '<script src="//platform.linkedin.com/in.js" type="text/javascript"></script><script type="IN/Share" data-counter="right"></script>',
        'init' => FALSE,
        'user_register_form' => FALSE,
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'social_buttons',
        'settings' => array(),
        'type' => 'social_buttons_exposed',
        'weight' => '12',
      ),
    ),
  );

  // Exported field: 'node-idea-field_vote'.
  $fields['node-idea-field_vote'] = array(
    'field_config' => array(
      'active' => '1',
      'cardinality' => '1',
      'deleted' => '0',
      'entity_types' => array(),
      'field_name' => 'field_vote',
      'field_permissions' => array(
        'type' => '0',
      ),
      'foreign keys' => array(),
      'indexes' => array(),
      'locked' => '0',
      'module' => 'vud_field',
      'settings' => array(
        'profile2_private' => FALSE,
        'votingapi_tag' => 'vote',
        'vud_widget' => 'updown',
      ),
      'translatable' => '0',
      'type' => 'vud_field',
    ),
    'field_instance' => array(
      'bundle' => 'idea',
      'default_value' => array(
        0 => array(
          'vud_widget' => 'updown',
          'votingapi_tag' => 'vote',
        ),
      ),
      'deleted' => '0',
      'description' => '',
      'display' => array(
        'default' => array(
          'label' => 'hidden',
          'module' => 'vud_field',
          'settings' => array(),
          'type' => 'default',
          'weight' => '1',
        ),
        'teaser' => array(
          'label' => 'hidden',
          'module' => 'vud_field',
          'settings' => array(),
          'type' => 'default',
          'weight' => '1',
        ),
      ),
      'entity_type' => 'node',
      'field_name' => 'field_vote',
      'label' => 'Vote',
      'required' => 0,
      'settings' => array(
        'user_register_form' => FALSE,
        'votingapi_tag' => 'vote',
        'vud_widget' => 'alternate',
      ),
      'widget' => array(
        'active' => 0,
        'module' => 'vud_field',
        'settings' => array(),
        'type' => 'vud_field_default_widget',
        'weight' => '3',
      ),
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Attachments');
  t('Blast Idea');
  t('Buzz Idea');
  t('Category');
  t('Comment');
  t('Describe your idea');
  t('Groove Idea');
  t('Hashtags');
  t('Idea image');
  t('In Action');
  t('Is Part Of A Challenge');
  t('Merge to');
  t('Please share you idea.');
  t('Social Buttons');
  t('Vote');

  return $fields;
}
