<?php

/**
 * @file
 * Hooks provided by the Message module.
 *
 */

/**
 * @addtogroup hooks
 * @{
 */

/**
 * Act on a message that is being assembled before rendering.
 *
 * @param $message
 *   The message entity.
 * @param $view_mode
 *   The view mode the message is rendered in.
 * @param $langcode
 *   The language code used for rendering.
 *
 * The module may add elements to $message->content prior to rendering. The
 * structure of $message->content is a renderable array as expected by
 * drupal_render().
 *
 * @see hook_entity_prepare_view()
 * @see hook_entity_view()
 */
function hook_message_view($message, $view_mode, $langcode) {
  $message->content['my_additional_field'] = array(
    '#markup' => $additional_field,
    '#weight' => 10,
    '#theme' => 'mymodule_my_additional_field',
  );
}

/**
 * Alter the results of entity_view() for messages.
 *
 * @param $build
 *   A renderable array representing the message content.
 *
 * This hook is called after the content has been assembled in a structured
 * array and may be used for doing processing which requires that the complete
 * message content structure has been built.
 *
 * If the module wishes to act on the rendered HTML of the message rather than
 * the structured content array, it may use this hook to add a #post_render
 * callback. Alternatively, it could also implement hook_preprocess_message().
 * See drupal_render() and theme() documentation respectively for details.
 *
 * @see hook_entity_view_alter()
 */
function hook_message_view_alter($build) {
  if ($build['#view_mode'] == 'full' && isset($build['an_additional_field'])) {
    // Change its weight.
    $build['an_additional_field']['#weight'] = -10;

    // Add a #post_render callback to act on the rendered HTML of the entity.
    $build['#post_render'][] = 'my_module_post_render';
  }
}

/**
 * Define default message type configurations.
 *
 * @return
 *   An array of default message types, keyed by machine names.
 *
 * @see hook_default_message_type_alter()
 */
function hook_default_message_type() {
  $defaults['main'] = entity_create('message_type', array(
    // É
  ));
  return $defaults;
}

/**
 * Alter default message type configurations.
 *
 * @param array $defaults
 *   An array of default message types, keyed by machine names.
 *
 * @see hook_default_message_type()
 */
function hook_default_message_type_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}

/**
 * Alter message type forms.
 *
 * Modules may alter the message type entity form by making use of this hook or
 * the entity bundle specific hook_form_message_type_edit_BUNDLE_form_alter().
 * #entity_builders may be used in order to copy the values of added form
 * elements to the entity, just as documented for
 * entity_form_submit_build_entity().
 *
 * @param $form
 *   Nested array of form elements that comprise the form.
 * @param $form_state
 *   A keyed array containing the current state of the form.
 */
function hook_form_message_type_form_alter(&$form, &$form_state) {
  // Your alterations.
}

/**
 * Define default message type category configurations.
 *
 * @return
 *   An array of default message type categories, keyed by machine names.
 *
 * @see hook_default_message_type_category_alter()
 */
function hook_default_message_type_category() {
  $defaults['main'] = entity_create('message_type_category', array(
    // É
  ));
  return $defaults;
}

/**
 * Alter default message type category configurations.
 *
 * @param array $defaults
 *   An array of default message type categories, keyed by machine names.
 *
 * @see hook_default_message_type_category()
 */
function hook_default_message_type_category_alter(array &$defaults) {
  $defaults['main']->name = 'custom name';
}

/**
 * @} End of "addtogroup hooks".
 */