<?php

/**
 * @file
 * Ctools export UI for a heartbeat template
 * @author stalski
 *
 */
class ctools_export_ui_heartbeat_template extends ctools_export_ui {

  /**
   * Page callback to delete an exportable item.
   */
  function delete_page($js, $input, $item) {
    $form_state = array(
      'plugin' => $this->plugin,
      'object' => &$this,
      'ajax' => $js,
      'item' => $item,
      'op' => $item->export_type & EXPORT_IN_CODE ? 'revert' : 'delete',
      'rerender' => TRUE,
      'no_redirect' => TRUE,
    );

    $output = drupal_build_form('ctools_export_ui_delete_confirm_form', $form_state);
    if (!empty($form_state['executed'])) {
      ctools_export_crud_delete($this->plugin['schema'], $item);
      $export_key = $this->plugin['export']['key'];
      $message = str_replace('%title', check_plain($item->{$export_key}), $this->plugin['strings']['confirmation'][$form_state['op']]['success']);
      drupal_set_message($message);

      // Perform a deletion of the activity for this template.
      db_delete('heartbeat_activity')->condition('message_id', $item->message_id)->execute();

      drupal_goto(ctools_export_ui_plugin_base_path($this->plugin));
    }

    return $output;
  }

}