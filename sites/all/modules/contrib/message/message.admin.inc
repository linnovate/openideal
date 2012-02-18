<?php

/**
 * @file
 * Message type editing UI.
 */

/**
 * UI controller.
 */
class MessageTypeUIController extends EntityDefaultUIController {

  /**
   * Overrides hook_menu() defaults.
   */
  public function hook_menu() {
    $items = parent::hook_menu();
    $items[$this->path]['description'] = 'Manage message types, including fields.';
    return $items;
  }

  /**
   * Overridden to customize the field location.
   */
  public function entityFormSubmitBuildEntity($form, &$form_state) {
    // We cannot use entity_form_submit_build_entity() any more.
    $entity = $form_state['message_type'];

    // Extract form values.
    form_state_values_clean($form_state);
    foreach ($form_state['values'] as $key => $value) {
      if ($key != 'message_type_fields') {
        $entity->$key = $value;
      }
    }
    // Invoke all specified builders for copying form values to entity
    // properties.
    // @see entity_form_submit_build_entity()
    if (isset($form['#entity_builders'])) {
      foreach ($form['#entity_builders'] as $function) {
        $function('message_type', $entity, $form, $form_state);
      }
    }
    field_attach_submit('message_type', $entity, $form['message_type_fields'], $form_state);
    return $entity;
  }
}

/**
 * Generates the message type editing form.
 */
function message_type_form($form, &$form_state, $message_type, $op = 'edit') {
  if ($op == 'clone') {
    $message_type->label .= ' (cloned)';
    $message_type->name .= '_clone';
  }

  $form['description'] = array(
    '#title' => t('Description'),
    '#type' => 'textfield',
    '#default_value' => $message_type->description,
    '#description' => t('The human-readable description of this message type.'),
    '#required' => TRUE,
    '#weight' => -5,
  );
  // Machine-readable type name.
  $form['name'] = array(
    '#type' => 'machine_name',
    '#default_value' => isset($message_type->name) ? $message_type->name : '',
    '#disabled' => $message_type->hasStatus(ENTITY_IN_CODE),
    '#machine_name' => array(
      'exists' => 'message_type_load',
      'source' => array('description'),
    ),
    '#description' => t('A unique machine-readable name for this message type. It must only contain lowercase letters, numbers, and underscores.'),
    '#weight' => -5,
  );

  // We might have gotten the message type category via ajax, so set it in the
  // message type entity.
  if (!empty($form_state['values']['message_type_category'])) {
    $message_type->category = $form_state['values']['message_type_category'];
  }

  if ($op == 'add') {
    // Get all the message type category, and allow user to choose one using ajax.
    $options = array();
    $options['message_type'] = t('Default message type category');
    foreach (message_type_category_load() as $message_category) {
      $options[$message_category->category] = !empty($message_category->description) ? $message_category->description : $message_type->category;
    }

    $form['message_type_category'] = array(
      '#title' => t('Message type category'),
      '#type' => 'select',
      '#options' => $options,
      '#default_value' => $message_type->category,
      '#description' => t('Select the message type category.'),
      '#required' => TRUE,
      '#ajax' => array(
        'callback' => 'message_type_fields_ajax_callback',
        'wrapper' => 'message-type-wrapper',
      ),
    );
  }
  else {
    if ($message_category = message_type_category_load($message_type->category)) {
      $value = !empty($message_category->description) ? check_plain($message_category->description) : check_plain($message_type->category);
    }
    else {
      $value = t('Default message type category');
    }
    $form['message_type_category'] = array(
      '#title' => t('Message type category'),
      '#type' => 'item',
      '#markup' => $value,
    );
  }

  $form['message_type_fields'] = array(
    '#prefix' => '<div id="message-type-wrapper">',
    '#suffix' => '</div>',
    '#tree' => TRUE,
    '#parents' => array('message_type_fields'),
  );
  field_attach_form('message_type', $message_type, $form['message_type_fields'], $form_state);

  $form['argument_keys'] = array(
    '#title' => t('Replacement tokens'),
    '#type' => 'textfield',
    '#default_value' => implode(', ', (array) $message_type->argument_keys),
    '#description' => t('A comma-separated list of replacement tokens, e.g. %title or !url, of which the message text makes use of. Each replacement token has to start with one of the special characters "@", "%" or "!". This character controls the sanitization method used, analogously to the <a href="!link">t()</a> function.', array('!link' => 'http://api.drupal.org/api/drupal/includes--bootstrap.inc/function/t/7')),
  );

  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save message type'),
    '#weight' => 40,
  );

  if (!$message_type->hasStatus(ENTITY_IN_CODE) && $op != 'add') {
    $form['actions']['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete message type'),
      '#weight' => 45,
      '#limit_validation_errors' => array(),
      '#submit' => array('message_type_form_submit_delete')
    );
  }
  return $form;
}

/**
 * AJAX callback to attach the message type fields to the form.
 *
 * Since the controlling logic for populating the form is in the form builder
 * function, all we do here is select the element and return it to be updated.
 */
function message_type_fields_ajax_callback(array $form, array &$form_state) {
  return $form['message_type_fields'];
}

/**
 * Form API validation callback for the type form.
 */
function message_type_form_validate($form, $form_state) {
  field_attach_form_validate('message_type', $form_state['message_type'], $form['message_type_fields'], $form_state);
}

/**
 * Form API submit callback for the type form.
 */
function message_type_form_submit(&$form, &$form_state) {
  $message_type = entity_ui_form_submit_build_entity($form, $form_state);
  $message_type->argument_keys = array_filter(array_map('trim', explode(',', $message_type->argument_keys)));
  // Save and go back.
  $message_type->save();
  $form_state['redirect'] = 'admin/structure/messages';
}

/**
 * Form API submit callback for the delete button.
 */
function message_type_form_submit_delete(&$form, &$form_state) {
  $form_state['redirect'] = 'admin/structure/messages/manage/' . $form_state['message_type']->name . '/delete';
}
