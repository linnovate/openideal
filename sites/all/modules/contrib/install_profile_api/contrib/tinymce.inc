<?php

/**
 * Add roles to an existing TinyMCE profile identified by the profile name.
 */
function install_tinymce_add_roles($name, $roles) {
  foreach ($roles as $role) {
    db_query("INSERT INTO {tinymce_role} (name, rid) VALUES ('%s', %d)", $name, $role);
  }
}

/**
 * Create a new TinyMCE profile and set the settings.
 *
 * @param $name
 *   A text string identifying the profile.
 * @param $settings
 *   An associative array containing key value pairs.
 */
function install_tinymce_create_profile($name, $settings) {
  db_query("INSERT INTO {tinymce_settings} (name, settings) VALUES ('%s', '%s')", $name, serialize($settings));
}
