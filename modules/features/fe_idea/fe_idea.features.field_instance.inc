<?php
/**
 * @file
 * fe_idea.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function fe_idea_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'comment-comment_node_idea-comment_body'.
  $field_instances['comment-comment_node_idea-comment_body'] = array(
    'bundle' => 'comment_node_idea',
    'default_value' => NULL,
    'deleted' => 0,
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
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 0,
    ),
  );

  // Exported field_instance: 'comment-comment_node_idea-field_vote'.
  $field_instances['comment-comment_node_idea-field_vote'] = array(
    'bundle' => 'comment_node_idea',
    'default_value' => array(
      0 => array(
        'vud_widget' => 'updown',
        'votingapi_tag' => 'vote',
      ),
    ),
    'deleted' => 0,
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
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-idea-body'.
  $field_instances['node-idea-body'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 300,
        ),
        'type' => 'text_trimmed',
        'weight' => 2,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Description',
    'required' => 0,
    'settings' => array(
      'display_summary' => 0,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 10,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 3,
    ),
  );

  // Exported field_instance: 'node-idea-field_attachments'.
  $field_instances['node-idea-field_attachments'] = array(
    'bundle' => 'idea',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'file',
        'settings' => array(),
        'type' => 'file_default',
        'weight' => 8,
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
    'label' => 'Attached Files',
    'required' => 0,
    'settings' => array(
      'description_field' => 0,
      'file_directory' => '',
      'file_extensions' => 'txt pdf doc docx ppt pptx jpg jpeg png gif xls xlsx',
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
            'attach' => 'attach',
            'clipboard' => 0,
            'imce' => 0,
            'reference' => 0,
            'remote' => 0,
            'upload' => 'upload',
          ),
          'source_attach' => array(
            'absolute' => 0,
            'attach_mode' => 'move',
            'path' => 'file_attach',
          ),
          'source_imce' => array(
            'imce_mode' => 0,
          ),
          'source_reference' => array(
            'autocomplete' => 0,
          ),
        ),
        'progress_indicator' => 'throbber',
      ),
      'type' => 'file_generic',
      'weight' => 20,
    ),
  );

  // Exported field_instance: 'node-idea-field_blast_idea'.
  $field_instances['node-idea-field_blast_idea'] = array(
    'bundle' => 'idea',
    'default_value' => array(
      0 => array(
        'radioactivity_energy' => 1000,
        'radioactivity_timestamp' => 1332403825,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 16,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 8,
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
      'weight' => 14,
    ),
  );

  // Exported field_instance: 'node-idea-field_business_name'.
  $field_instances['node-idea-field_business_name'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'i18n_taxonomy',
        'settings' => array(),
        'type' => 'i18n_taxonomy_term_reference_link',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_business_name',
    'label' => 'Business Name Old',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 13,
    ),
  );

  // Exported field_instance: 'node-idea-field_business_results'.
  $field_instances['node-idea-field_business_results'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'State in 1-2 sentences..You can express it as a strategic platform or give it a name. Ideally you can state it
as short as a Tweet, 140 characters',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 14,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_business_results',
    'label' => 'Business Results',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 1,
    ),
  );

  // Exported field_instance: 'node-idea-field_buzz_idea'.
  $field_instances['node-idea-field_buzz_idea'] = array(
    'bundle' => 'idea',
    'default_value' => array(
      0 => array(
        'radioactivity_energy' => 1000,
        'radioactivity_timestamp' => 1332403886,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 18,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 7,
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
      'weight' => 15,
    ),
  );

  // Exported field_instance: 'node-idea-field_category'.
  $field_instances['node-idea-field_category'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_category',
    'label' => 'קטגוריה',
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => 9,
    ),
  );

  // Exported field_instance: 'node-idea-field_challenge_ref'.
  $field_instances['node-idea-field_challenge_ref'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'default_value_function' => 'entityreference_prepopulate_field_default_value',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 15,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => 1,
        ),
        'type' => 'entityreference_label',
        'weight' => 4,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_challenge_ref',
    'label' => 'Business Name',
    'required' => 1,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'action' => 'hide',
          'action_on_edit' => 0,
          'fallback' => 'none',
          'providers' => array(
            'og_context' => FALSE,
            'url' => 1,
          ),
          'skip_perm' => 0,
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
      'weight' => 2,
    ),
  );

  // Exported field_instance: 'node-idea-field_groove_idea'.
  $field_instances['node-idea-field_groove_idea'] = array(
    'bundle' => 'idea',
    'default_value' => array(
      0 => array(
        'radioactivity_energy' => 1000,
        'radioactivity_timestamp' => 1332403976,
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 19,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 6,
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
      'weight' => 16,
    ),
  );

  // Exported field_instance: 'node-idea-field_hashtags'.
  $field_instances['node-idea-field_hashtags'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'module' => 'taxonomy',
        'settings' => array(),
        'type' => 'taxonomy_term_reference_link',
        'weight' => 10,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_hashtags',
    'label' => 'Hashtags',
    'required' => FALSE,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'taxonomy',
      'settings' => array(
        'autocomplete_path' => 'taxonomy/autocomplete',
        'size' => 60,
      ),
      'type' => 'taxonomy_autocomplete',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-idea-field_how_it_works'.
  $field_instances['node-idea-field_how_it_works'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'State in 1-2 sentences..You can express it as a strategic platform or give it a name. Ideally you can state it
as short as a Tweet, 140 characters',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 13,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_how_it_works',
    'label' => 'How it Works',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-idea-field_idea'.
  $field_instances['node-idea-field_idea'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'State in 1-2 sentences.You can express it as a strategic platform or give it a name. Ideally you can state it
as short as a Tweet, 140 characters',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
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
    'field_name' => 'field_idea',
    'label' => 'Idea',
    'required' => 1,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-idea-field_idea_image'.
  $field_instances['node-idea-field_idea_image'] = array(
    'bundle' => 'idea',
    'deleted' => 0,
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
        'weight' => 7,
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
      'default_image' => 30,
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
            'attach' => 'attach',
            'clipboard' => 0,
            'imce' => 0,
            'reference' => 0,
            'remote' => 0,
            'upload' => 'upload',
          ),
          'source_attach' => array(
            'absolute' => 0,
            'attach_mode' => 'move',
            'path' => 'file_attach',
          ),
          'source_imce' => array(
            'imce_mode' => 0,
          ),
          'source_reference' => array(
            'autocomplete' => 0,
          ),
        ),
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-idea-field_in_action'.
  $field_instances['node-idea-field_in_action'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
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
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-idea-field_insight'.
  $field_instances['node-idea-field_insight'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => 'State in 4-5 sentences.No more than a paragraph. It should be clear, simple, and include cultural context.
Ideally you can state it as short as a Tweet, 140 characters',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
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
    'field_name' => 'field_insight',
    'label' => 'Insight',
    'required' => 0,
    'settings' => array(
      'text_processing' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 4,
    ),
  );

  // Exported field_instance: 'node-idea-field_merge_to'.
  $field_instances['node-idea-field_merge_to'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 17,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 9,
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
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 12,
    ),
  );

  // Exported field_instance: 'node-idea-field_moderator'.
  $field_instances['node-idea-field_moderator'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'default_value_function' => '',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => 10,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_moderator',
    'label' => 'Moderator',
    'required' => 0,
    'settings' => array(
      'behaviors' => array(
        'prepopulate' => array(
          'status' => 0,
        ),
      ),
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'entityreference',
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'path' => '',
        'size' => 60,
      ),
      'type' => 'entityreference_autocomplete',
      'weight' => 21,
    ),
  );

  // Exported field_instance: 'node-idea-field_share'.
  $field_instances['node-idea-field_share'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'social_buttons',
        'settings' => array(),
        'type' => 'social_buttons_formatter_default',
        'weight' => 9,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_share',
    'label' => 'Share',
    'required' => 0,
    'settings' => array(
      'button-0' => '<a class="in-btn" href="#" onClick="MyWindow=window.open(\'http://www.linkedin.com/shareArticle?mini=true&url=[current-page:url:absolute]&title=[current-page:title]&source=[site:url]\',\'MyWindow\',width=300,height=300); return true;">X</a>',
      'button-1' => '<a class="t-btn" href="#" onClick="MyWindow=window.open(\'http://twitter.com/home?status=[current-page:title]+[current-page:url:absolute]\',\'MyWindow\',width=300,height=300); return true;">Z</a>
',
      'button-2' => '<a class="g-btn" href="#" onClick="MyWindow=window.open(\'https://plus.google.com/share?url=[current-page:url:absolute]\',\'MyWindow\',width=300,height=300); return true;">W</a>',
      'button-3' => '<a class="fb-btn" href="#" onClick="MyWindow=window.open(\'http://www.facebook.com/share.php?u=[current-page:url:absolute]&title=[current-page:title]\',\'MyWindow\',width=300,height=300); return true;">Y</a>',
      'button-4' => '',
      'init' => FALSE,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'social_buttons',
      'settings' => array(),
      'type' => 'social_buttons_exposed',
      'weight' => 18,
    ),
  );

  // Exported field_instance: 'node-idea-field_social_buttons'.
  $field_instances['node-idea-field_social_buttons'] = array(
    'bundle' => 'idea',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'social_buttons',
        'settings' => array(),
        'type' => 'social_buttons_formatter_default',
        'weight' => 6,
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
      'button-0' => '<script src="//platform.linkedin.com/in.js" type="text/javascript"></script><script type="IN/Share" data-counter="right"></script>',
      'button-1' => '<div class="g-plusone" data-size="medium"></div><script>(function() {var po = document.createElement("script"); po.type = "text/javascript"; po.async = true;po.src = "https://apis.google.com/js/plusone.js";var s = document.getElementsByTagName("script")[0]; s.parentNode.insertBefore(po, s);})();</script>',
      'button-2' => '<a href="https://twitter.com/share" class="twitter-share-button">Tweet</a><script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0];if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src="//platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>',
      'button-3' => '<div id="fb-root"></div><script> (function(d, s, id) { var js, fjs = d.getElementsByTagName(s)[0]; if (d.getElementById(id)) return; js = d.createElement(s); js.id = id; js.src = "//connect.facebook.net/en_US/all.js#xfbml=1"; fjs.parentNode.insertBefore(js, fjs);}(document, "script", "facebook-jssdk"));</script> <div class="fb-like" data-send="false" data-layout="button_count" data-width="1" data-show-faces="false"></div>
',
      'button-4' => '',
      'init' => FALSE,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 0,
      'module' => 'social_buttons',
      'settings' => array(),
      'type' => 'social_buttons_exposed',
      'weight' => 17,
    ),
  );

  // Exported field_instance: 'node-idea-field_vote'.
  $field_instances['node-idea-field_vote'] = array(
    'bundle' => 'idea',
    'default_value' => array(
      0 => array(
        'vud_widget' => 'updown',
        'votingapi_tag' => 'vote',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'vud_field',
        'settings' => array(),
        'type' => 'default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'vud_field',
        'settings' => array(),
        'type' => 'default',
        'weight' => 1,
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
      'weight' => 10,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Blast Idea');
  t('Business Name');
  t('Business Name Old');
  t('Business Results');
  t('Buzz Idea');
  t('Comment');
  t('Description');
  t('Groove Idea');
  t('Hashtags');
  t('How it Works');
  t('Idea');
  t('Idea image');
  t('In Action');
  t('Insight');
  t('Merge to');
  t('Moderator');
  t('Share');
  t('Social Buttons');
  t('State in 1-2 sentences..You can express it as a strategic platform or give it a name. Ideally you can state it
as short as a Tweet, 140 characters');
  t('State in 1-2 sentences.You can express it as a strategic platform or give it a name. Ideally you can state it
as short as a Tweet, 140 characters');
  t('State in 4-5 sentences.No more than a paragraph. It should be clear, simple, and include cultural context.
Ideally you can state it as short as a Tweet, 140 characters');
  t('Vote');
  t('קבצים מצורפים');
  t('קטגוריה');

  return $field_instances;
}
