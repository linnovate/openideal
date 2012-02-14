<?php
// $Id: emvideo_handler_field_duration.inc,v 1.1.2.1 2009/09/30 15:56:48 aaron Exp $

/**
 * @file
 * Field handler for emvideo providers.
 *
 * An extended subclass for field handling that adds multiple field grouping.
 *
 * Fields that want multiple value grouping options in addition to basic
 * field and formatter handling can extend this class.
 */
class emvideo_handler_field_duration extends content_handler_field_multiple {

  /**
   * Provide 'group multiple values' option.
   */
  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);

    $form['format']['#options'] = array(
      'duration_time' => t('Duration (mm:ss)'),
      'duration_seconds' => t('Duration (seconds)'),
    );
  }

  function render_provider($values) {
    // We're down to a single node here, so we can retrieve the actual field
    // definition for the node type being considered.
    $field = content_fields($this->content_field['field_name'], $values->{$this->aliases['type']});
    $options = $this->options;
    $db_info = content_database_info($field);

    // Build a pseudo-node from the retrieved values.
    $node = drupal_clone($values);
    $node->type = $values->{$this->aliases['type']};
    $node->nid = $values->{$this->aliases['nid']};
    $node->vid = $values->{$this->aliases['vid']};
    // Some formatters need to behave differently depending on the build_mode
    // (for instance: preview), so we provide one.
    $node->build_mode = NODE_BUILD_NORMAL;

    $item = array();
    foreach ($db_info['columns'] as $column => $attributes) {
      $item[$column] = $values->{$this->aliases[$attributes['column']]};
    }

    $item['#delta'] = $field['multiple'] ?  $values->{$this->aliases['delta']} : 0;

    // Render items.
    $output = theme('emvideo_handler_field_duration', $item, $field, $node, $values, $options['format']);
    return $this->render_link($output, $values);
  }

  function render($values) {
    // If this is not a grouped field, use content_handler_field::render().
    if (!$this->defer_query) {
      return $this->render_provider($values);
    }

    // We're down to a single node here, so we can retrieve the actual field
    // definition for the node type being considered.
    $field = content_fields($this->content_field['field_name'], $values->{$this->aliases['type']});
    $options = $this->options;

    $vid = $values->{$this->field_alias};
    if (isset($this->field_values[$vid])) {
      // Gather items, respecting the 'Display n values starting from m' settings.
      $count_skipped = 0;
      $items = array();
      foreach ($this->field_values[$vid] as $item) {
        if (empty($options['multiple']['multiple_from']) || ($count_skipped >= $options['multiple']['multiple_from'])) {
          if (empty($options['multiple']['multiple_number']) || (count($items) < $options['multiple']['multiple_number'])) {
            // Grab the nid - needed for render_link().
            $nid = $item['_nid'];
            unset($item['_nid']);
            $items[] = $item;
          }
          else {
            break;
          }
        }
        $count_skipped++;
      }

      // Build a pseudo-node from the retrieved values.
      $node = drupal_clone($values);
      // content_format and formatters will need a 'type'.
      $node->type = $values->{$this->aliases['type']};
      $node->nid = $values->{$this->aliases['nid']};
      $node->vid = $values->{$this->aliases['vid']};

      // Some formatters need to behave differently depending on the build_mode
      // (for instance: preview), so we provide one.
      $node->build_mode = NODE_BUILD_NORMAL;

      // Render items.
      if ($items) {
        $rendered = array();
        // Single-value formatter.
        foreach ($items as $item) {
          $output = theme('emvideo_handler_field_duration', $item, $field, $node, $values, $options['format']);
          if (!empty($output)) {
            $rendered[] = $this->render_link($output, (object) array('nid' => $nid));
          }
        }

        if (count($rendered) > 1) {
          // TODO: could we use generic field display ?
          return theme('content_view_multiple_field', $rendered, $field, $values);
        }
        elseif ($rendered) {
          return $rendered[0];
        }
      }
    }

    return '';
  }

}
