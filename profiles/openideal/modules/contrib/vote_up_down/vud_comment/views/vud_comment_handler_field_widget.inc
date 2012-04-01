<?php

/**
 * @file
 * Provide a handler for Vote Up/down widget field for comments.
 */

/**
 * A handler that provides a Vote Up/Down widget field for comments.
 */
class vud_comment_handler_field_widget extends views_handler_field {
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
    $query = 'SELECT n.type FROM {comment} c LEFT JOIN {node} n ON c.nid = n.nid WHERE c.cid = :cid';
    $type = db_query($query, array(':cid' => $values->cid))->fetchField();
    $comment_allow = in_array($type, variable_get('vud_comment_node_types', array()), TRUE);
    $can_edit = user_access('use vote up/down on comments');
    if ($comment_allow && ($can_edit || user_access('view vote up/down count on comments'))) {
      $tag = variable_get('vud_tag', 'vote');
      $widget_theme = variable_get('vud_comment_widget', 'plain');

      $variables = array(
        'entity_id' => $values->cid,
        'type' => 'comment',
        'tag' => $tag,
        'widget_theme' => $widget_theme,
        'readonly' => !$can_edit,
      );
      $widget = theme('vud_widget', $variables);
    }

    return $widget;
  }
}
