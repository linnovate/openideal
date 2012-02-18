<?php

/**
 * @file
 * Provides Entity metadata integration.
 */

/**
 * Extend the defaults.
 */
class MessageMetadataController extends EntityDefaultMetadataController {

  public function entityPropertyInfo() {
    $info = parent::entityPropertyInfo();
    $properties = &$info[$this->type]['properties'];

    $properties['type'] = array(
      'setter callback' => 'entity_property_verbatim_set',
      'required' => TRUE,
      'description' => t('The message type.'),
      'type' => 'message_type',
    ) + $properties['type'];

    unset($properties['arguments']);
    // Unset the uid property, as it is available via the user anyway.
    unset($properties['uid']);

    $properties['user'] = array(
      'label' => t("User"),
      'type' => 'user',
      'description' => t("The user for which to log the message."),
      'getter callback' => 'entity_property_getter_method',
      'setter callback' => 'entity_property_setter_method',
      'schema field' => 'uid',
      'required' => TRUE,
    );

    $properties['timestamp'] = array(
      'type' => 'date',
      'setter callback' => 'entity_property_verbatim_set',
      'description' => t('The time the message has been logged.'),
    ) + $properties['timestamp'];

    // Add a property providing the final text for use with entity tokens.
    $properties['text'] = array(
      'type' => 'text',
      'label' => t('The message text'),
      'getter callback' => 'message_property_get_text',
      'description' => t('The message text with all replacement arguments applied.'),
      // The tokens are already sanitized if needed.
      'sanitize' => TRUE,
    );

    // Add in the arguments specific to the bundles.
    foreach (message_type_load() as $name => $type) {
      $info['message']['bundles'][$name]['properties']['arguments'] = array(
        'label' => t('Message arguments'),
        'type' => 'struct',
        'description' => t("The arguments to associate with the message."),
        'getter callback' => 'entity_property_verbatim_get',
        'setter callback' => 'entity_property_verbatim_set',
        'property info' => array(),
      );
      $keys = array_filter($type->argument_keys);
      foreach ($keys as $key) {
        // Add the argument keys so they can later be referenced by
        // entity-metadata. For example if the message is "hello @foo", then
        // that key will be available via message:argument:foo.
        $key_name = substr($key, 1);
        $info['message']['bundles'][$name]['properties']['arguments']['property info'][$key_name] = array(
          'type' => 'text',
          'label' => t('Argument @key', array('@key' => $key)),
          'description' => t('The replacement value for @key', array('@key' => $key)),
          'getter callback' => 'message_property_get_argument',
          'setter callback' => 'message_property_set_argument',
          // Get the first character of the replacement key.
          'sanitized' => ($key[0] == '!') ? TRUE : FALSE,
          'message_replace_char' => $key[0],
        );
      }
    }

    return $info;
  }
}
