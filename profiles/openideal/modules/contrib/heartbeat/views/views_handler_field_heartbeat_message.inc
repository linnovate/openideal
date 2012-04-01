<?php
/**
 * Field handler to present a message for the activity.
 */
class views_handler_field_heartbeat_message extends views_handler_field {
  function construct() {
    parent::construct();
    $this->additional_fields['variables'] = 'variables';
    $this->additional_fields['message_id'] = 'message_id';
  }

  function option_definition() {
    $options = parent::option_definition();

    $options['text'] = array('default' => '', 'translatable' => TRUE);

    return $options;
  }

  function options_form(&$form, &$form_state) {
    $form['text'] = array(
      '#type' => 'textfield',
      '#title' => t('Text to display'),
      '#default_value' => $this->options['text'],
    );
    parent::options_form($form, $form_state);
  }

  function query() {
    $this->ensure_my_table();
    $this->add_additional_fields();
  }

  function render($values) {
    $variables = heartbeat_decode_message_variables($this->get_value($values, 'variables'));
    $template_id = $this->get_value($values, 'message_id');
    $template = heartbeat_message_template_load($template_id);
    return $this->render_message($template->message, $variables);
  }

  function render_message($message, $values) {
    return strtr($message, $values);
  }
}