<?php

/**
 * Form for configurations.
 */
function add_idea_autocomplete_config_form($form) {
  
  $form['idea_add_autocomplete'] = array(
    '#type' => 'checkbox',
    '#title' => t('Idea Add Autocomplete'),
    '#description' => 'Enable an autocomplete to add idea title.',  
    '#default_value' => variable_get('idea_add_autocomplete', TRUE),
  );

  return system_settings_form($form);
}
