<?php

/**
 * @file
 * Implementation of hook_diff() for node.module (body and title).
 */

/**
 * Implementation of hook_diff() for node.module (body and title).
 */
function node_diff($old_node, $new_node) {

  $result = array();
  $type = node_type_get_type($new_node);
  $result['title'] = array(
    '#name' => $type->title_label,
    '#old' => array($old_node->title),
    '#new' => array($new_node->title),
    '#weight' => -5,
    '#format' => array(
      'show_header' => FALSE,
    )
  );

  // @TODO: abstract this to work with all field types and/or split this
  // integration out to be more generic.
  $instances = field_info_instances('node', field_extract_bundle('node', $type));
  foreach ($instances as $instance) {
    $field_name = $instance['field_name'];
    $langcode = field_language('node', $new_node, $field_name);
    if (isset($new_node->{$field_name}[$langcode])) {
      foreach (array_keys($new_node->{$field_name}[$langcode]) as $delta) {
        if (isset($new_node->{$field_name}[$langcode][$delta]['value']) && isset($old_node->{$field_name}[$langcode][$delta]['value'])) {
          $view_old = $old_node->{$field_name}[$langcode][$delta]['value'];
          $view_new = $new_node->{$field_name}[$langcode][$delta]['value'];
          $result["{$field_name}_{$delta}"] = array(
            '#name' => $instance['label'],
            '#old' => explode("\n", $view_old),
            '#new' => explode("\n", $view_new),
          );
        }
        elseif (isset($new_node->{$field_name}[$langcode][$delta]['value'])) {
          // We have a newly input value where there was none in the previous
          // version of the node.
          $view_new = $new_node->{$field_name}[$langcode][$delta]['value'];
          $result["{$field_name}_{$delta}"] = array(
            '#name' => $instance['label'],
            '#old' => '',
            '#new' => explode("\n", $view_new),
          );
        }
        elseif (isset($old_node->{$field_name}[$langcode][$delta]['value'])) {
          // We have a value that has been removed from the field.
          $view_old = $old_node->{$field_name}[$langcode][$delta]['value'];
          $result["{$field_name}_{$delta}"] = array(
            '#name' => $instance['label'],
            '#old' => explode("\n", $view_old),
            '#new' => '',
          );
        }
      }
    }
    elseif (isset($old_node->{$field_name}[$langcode])) {
      // We have a value that has been removed from the field.
      foreach (array_keys($old_node->{$field_name}[$langcode]) as $delta) {
        if (isset($old_node->{$field_name}[$langcode][$delta]['value'])) {
          $view_old = $old_node->{$field_name}[$langcode][$delta]['value'];
          $result["{$field_name}_{$delta}"] = array(
            '#name' => $instance['label'],
            '#old' => explode("\n", $view_old),
            '#new' => '',
          );
        }
      }
    }
  }
  return $result;
}
