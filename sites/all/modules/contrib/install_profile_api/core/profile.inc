<?php

/**
 * Given a funky array of profile fields, create them.
 */
function install_profile_field_add($data) {
  if (!is_array($data) || !isset($data['title']) || !isset($data['name'])) {
    return false;
  }
  $data['category'] = ($data['category']) ? $data['category'] : '';
  $data['type'] = ($data['type']) ? $data['type'] : 'textfield';
  $data['required'] = ($data['required']) ? $data['required'] : '0';
  $data['register'] = ($data['register']) ? $data['register'] : '0';
  $data['visibility'] = ($data['visibility']) ? $data['visibility'] : '0';
  $data['explanation'] = ($data['explanation']) ? $data['explanation'] : '';
  $data['options'] = ($data['options']) ? implode("\n", $data['options']) : ''; 
 
  $fields = array_keys($data);

  // Prepare the query:
  foreach ($data as $key => $value) {
    if (in_array((string) $key, $fields)) {
      $k[] = db_escape_string($key);
      $v[] = $value;
      $s[] = "'%s'";
    }
  }
  db_query("INSERT INTO {profile_fields} (". implode(", ", $k) .") VALUES (". implode(", ", $s) .")", $v);

  return db_last_insert_id('profile_fields', 'fid');
}
