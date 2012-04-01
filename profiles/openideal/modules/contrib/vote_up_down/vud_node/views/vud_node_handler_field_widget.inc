<?php

/**
 * @file
 * Provide a handler for Vote Up/down widget field for nodes.
 */

/**
 * A handler that provides a Vote Up/Down widget field for nodes.
 */
class vud_node_handler_field_widget extends views_handler_field {
  function query() {
    // We have to override the parent here, because we don't want
    // to do anything.
  }

  function option_definition() {
    $options = parent::option_definition();
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    // It doesn't make sense to have the ability to alter the output.
    $form['alter']['#access'] = FALSE;
  }

  function render($values) {
    $widget = '';
    if (($can_edit = user_access('use vote up/down on nodes')) || user_access('view vote up/down count on nodes')) {
      $nid = $values->nid;
      $type = db_query("SELECT type FROM {node} WHERE nid = :nid", array(':nid' => $nid))->fetchField();
      $valid_type = in_array($type, variable_get('vud_node_types', array()), TRUE);

      if ($valid_type) {
        $tag = variable_get('vud_tag', 'vote');
        $widget_type = variable_get('vud_node_widget', 'plain');
        $widget_message_code = (!$can_edit) ? VUD_WIDGET_MESSAGE_DENIED : VUD_WIDGET_MESSAGE_ERROR;

        $variables = array(
          'entity_id' => $nid,
          'type' => 'node',
          'tag' => $tag,
          'widget_type' => $widget_type,
          'readonly' => !$can_edit,
          'widget_message_code' => $widget_message_code,
        );
        $widget = theme('vud_widget', $variables);
      }
    }

    return $widget;
  }
}
