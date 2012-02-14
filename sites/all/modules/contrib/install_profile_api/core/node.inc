<?php

/**
 * Given a content_type array, enable the new content type.
 */
function install_add_content_type($content_type) {
  install_create_content_type($content_type['type'], $content_type['name'], $content_type);
}

/**
 * Given a content_type name, and an array of custom properties, enable
 * a new content type. This function is written to be self-documenting,
 * as it explicitly lists the properties you can set, and shows the defaults
 * if you don't set them.
 *
 * @param $content_type
 *   Required. Type id, only alpha-numeric and underscores.
 * @param $content_name
 *   Required. A descriptive name.
 * @param $properties
 *   Optional. An array of properties that describe the new node type.
 */
function install_create_content_type($content_type, $content_name, $properties = array()) {

  $default = array(
    'type' => $content_type, // No default, must pass a content type id name
    'name' => $content_name, // No default, must pass a content type display name
    'module' => 'node',
    'description' => '',
    'custom' => TRUE,
    'modified' => TRUE,
    'has_title' => TRUE,
    'has_body' => TRUE,
    'title_label' => 'Title',
    'body_label' => '',
    'locked' => FALSE,
    'min_word_count' => '0',
    'orig_type' => '',
    'help' => '',
  );
  $type = array_merge($default, $properties);

  $type = (object)_node_type_set_defaults($type);
  node_type_save($type);
}

/**
 * Given a title and body, and an array of custom properties, create
 * a node.
 *
 * @param $title
 *   Optional. The node title.
 * @param $body
 *   Optional. The node body.
 * @param $properties
 *   Optional. An array of properties that describe the node.
 */
function install_create_node($title = NULL, $body = NULL, $properties = array()) {
  $properties['nid'] = NULL; // Force creation of new node.
  $properties['title'] = $title;
  $properties['body'] = $body;
  $node = install_save_node($properties);
  return $node;
}

/**
 * Given an array of custom properties, save a node. This function is
 * written to be self-documenting, as it explicitly lists the properties
 * you can set, and shows the defaults if you don't set them.
 *
 * @param $properties
 *   Optional. An array of properties that describe the node.
 */
function install_save_node($properties = array()) {

  // If name has been passed as a property, convert it to a uid.
  if (isset($properties['name'])) {
    if (!($account = user_load(array('name' => $properties['name'])))) {
      drupal_set_message(t('Failed to load Drupal user %user -- node %title not saved.', array('%name' => $properties['name'], '%title' => $properties['title'])), 'error', FALSE);
      return;
    }
  }

  $default = array(
    'nid' => isset($properties['nid']) ? $properties['nid'] : NULL, // Defaults to NULL, which creates a new node.
    'title' => !empty($properties['title']) ? $properties['title'] : '-- no title--', // Defaults to '-- no title --'.
    'body' => NULL, // Defaults to no body.
    'type' => 'page',
    'teaser' => NULL,
    'log' => '',
    'created' => '',
    'format' => FILTER_FORMAT_DEFAULT,
    'uid' => isset($account) ? $account->uid : 0,

  );
  $node = array_merge($default, $properties);

  $node = (object) $node;
  node_save($node);

  return $node;
}
