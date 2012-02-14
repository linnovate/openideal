<?php
// $Id: rules.api.php,v 1.1.2.10 2010/03/08 18:22:24 fago Exp $

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */


/**
 * @defgroup rules Rules module integrations.
 *
 * Module integrations with the rules module.
 *
 * The Rules developer documentation describes how modules can integrate with
 * rules: http://drupal.org/node/298486.
 */

/**
 * @defgroup rules_hooks Rules' hooks
 * @{
 * Hooks that can be implemented by other modules in order to extend rules.
 */


/**
 * Define rules compatible actions.
 *
 * This hook is required in order to add a new rules action. It should be
 * placed into the file MODULENAME.rules.inc.
 *
 * @return
 *   An array of information about the module's provided rules actions. The array
 *   contains a sub-array for each action, with the action name as the key.
 *
 *   Possible attributes for each sub-array are:
 *
 *    - 'label'
 *         The label of the action. Start capitalized. Required.
 *    - 'module'
 *         The providing module's user readable name. Used for grouping the
 *         actions in the interface. Should start with a capital letter.
 *         Required.
 *    - 'arguments'
 *         An array describing the arguments needed by the action with the
 *         argument's name as key. Optional. Each argument has to be described
 *         by a sub-array with possible attributes as described afterwards.
 *   - 'new variables'
 *         An array describing the new variables the action adds to the rules
 *         evaluation state with the variable name as key. Optional. Each
 *         variable has to be described by a sub-array with possible
 *         attributes as described afterwards.
 *   - 'eval input'
 *         Optional; An array containing form element names of elements contained in the
 *         actions settings form ($form['settings']) to which input evaluators
 *         should be attached.
 *         For settings in a nested array the array keys may be separated by '|'
 *         in the name.
 *   - 'label callback'
 *         A callback to improve the action's label once it has been configured.
 *         Optional (Defaults to {ACTION_NAME}_label).
 *         @see rules_action_callback_label().
 *   - 'base'
 *         The base for action implementation callbacks to use instead of the
 *         action's name. This is useful for having a single implementation for
 *         a couple of (probably somehow similar) actions. Optional (defaults
 *         to the name).
 *   - 'help'
 *         A help text to assist the user during action configuration. Optional.
 *         As an alternative one can implement rules_action_callback_help().
 *
 *
 *  Each 'arguments' array may contain the following properties:
 *   -  'label'
 *         The label of the argument. Start capitalized. Required.
 *   - 'type'
 *         The rules data type of the variable, which is to be passed to
 *         the action. See http://drupal.org/node/298633 for a list of
 *         known types. Required.
 *   - 'description'
 *         If necessary, further description of the argument. The usage
 *         of this attribute depends on the data type. Optional.
 *   - 'default value'
 *         The value to pass to the action, when there is no specified
 *         value. Optional.
 *         It's main usage is in conjunction with the data type 'value'
 *         to pass some information from this hook to an actions base
 *         implementation.
 *
 *  Each 'new variables' array may contain the following properties:
 *   - 'label'
 *         The default label of the new variable. Start capitalized.
 *         Required.
 *   - 'type'
 *         The rules data type of the variable. See
 *         http://drupal.org/node/298633 for a list of known types.
 *         Required.
 *   - 'save'
 *         If this is set to TRUE, the new variable is saved by rules
 *         when the rules evaluation ends. Only possible for data types
 *         marked as 'savable'. Optional (defaults to FALSE).
 *   - 'label callback'
 *         A callback to improve the variables label using the action's
 *         configuration settings. Optional.
 *
 *  The module has to provide an implementation for each action, for which the
 *  function name has to equal the action's name or if specified, the action's
 *  base attribute. The other callbacks are optional.
 *
 *  @see rules_action_callback(), rules_action_callback_form(), rules_action_callback_validate(),
 *    rules_action_callback_submit(), rules_action_callback_help().
 */
function hook_rules_action_info() {
  return array(
    'rules_action_mail_to_user' => array(
      'label' => t('Send a mail to a user'),
      'arguments' => array(
        'user' => array('type' => 'user', 'label' => t('Recipient')),
      ),
      'module' => 'System',
      'eval input' => array('subject', 'message', 'from'),
    ),
  );
}


/**
 * The implementation callback for an action.
 *
 * It should be placed into the file MODULENAME.rules.inc.
 *
 * @param
 *   The callback gets the arguments passed as described in
 *   hook_rules_action_info() as well as an array containing
 *   the action's configuration settings, if there are any.
 *
 * @return
 *   The action may return an array containg variables and their
 *   names as key.
 *   This is used to let rules save a variable having a savable
 *   data type. Or also, if the action has specified to provide new
 *   variables it can do so by returning the variables. For
 *   an example adding a new variable see rules_action_load_node().
 *
 *   Conditions have to return a boolean value.
 *
 * @see hook_rules_action_info().
 */
function rules_action_callback($node, $title, $settings) {
  $node->title = $title;
  return array('node' => $node);
}

/**
 * The configuration form callback for an action.
 *
 * It should be placed into the file MODULENAME.rules_forms.inc or into
 * MODULENAME.rules.inc.
 *
 * This callback can be used to alter the automatically generated
 * configuration form. New form elements should be put in $form['settings']
 * as its form values are used to populate $settings automatically. If some
 * postprocessing of the values is necessary the action may implement
 * rules_action_callback_submit().
 *
 * @param $settings
 *   The array of configuration settings to edit. This array is going to be
 *   passed to the action implementation once executed.
 * @param $form
 *   The configuration form as generated by rules. The modify it, has to be
 *   taken by reference. Additional form elements should go into
 *   $form['settings']. To let rules know about additional textual form
 *   elements use the 'eval input' attribute of hook_rules_action_info() so
 *   rules adds input evaluation support to them.
 * @param $form_state
 *   The form's form state.
 *
 *
 * @see rules_action_callback_validate(), rules_action_callback_submit()
 *
 */
function rules_action_callback_form($settings, &$form) {
  $settings += array('type' => array());
  $form['settings']['type'] = array(
    '#type' => 'select',
    '#title' => t('Content types'),
    '#options' => node_get_types('names'),
    '#multiple' => TRUE,
    '#default_value' => $settings['type'],
    '#required' => TRUE,
  );
}

/**
 * The configuration form validation callback for an action.
 *
 * It should be placed into the file MODULENAME.rules_forms.inc or into
 * MODULENAME.rules.inc.
 *
 * This callback can be implemented to validate the action's configuration
 * form.
 *
 * @param $form
 *   The configuration form.
 * @param $form_state
 *   The form's form state.
 *
 *
 * @see rules_action_callback_form(), rules_action_callback_submit()
 */
function rules_action_callback_validate($form, $form_state) {
  if (!$form_state['values']['settings']['username'] && !$form_state['values']['settings']['userid']) {
    form_set_error('username', t('You have to enter the user name or the user id.'));
  }
}

/**
 * The configuration form submit callback for an action.
 *
 * It should be placed into the file MODULENAME.rules_forms.inc or into
 * MODULENAME.rules.inc.
 *
 * This callback can be implemented to post process the action's
 * configuration form values before they are stored.
 *
 * @param $settings
 *   The configuration settings to store.
 * @param $form
 *   The configuration form.
 * @param $form_state
 *   The form's form state.
 *
 *
 * @see rules_action_callback_validate(), rules_action_callback_submit()
 */
function rules_action_callback_submit(&$settings, $form, $form_state) {
  $settings['roles'] = array_filter(array_keys(array_filter($settings['roles'])));
}

/**
 * The configuration help callback for an action.
 *
 * It should be placed into the file MODULENAME.rules_forms.inc or into
 * MODULENAME.rules.inc.
 *
 * Any help text returned by this callback is shown during the action
 * configuration.
 *
 * @return The translated string to show.
 *
 * @see hook_rules_action_info()
 */
function rules_action_callback_help() {
  return t('This help text is going to be displayed during action configuration.');
}

/**
 * Features module integration callback.
 *
 * It should be placed into the file MODULENAME.rules_forms.inc or into
 * MODULENAME.rules.inc.
 *
 * Allows actions or conditions to add further needed feature components.
 *
 * @see hook_rules_action_info()
 */
function rules_action_callback_features_export(&$export, &$pipe, $settings) {
  
}


/**
 * Define rules conditions.
 *
 * This hook is required in order to add a new rules condition. It should be
 * placed into the file MODULENAME.rules.inc.
 *
 * Adding conditions works exactly the same way as adding actions, with the
 * exception that conditions can't add new variables. Thus the 'new variables'
 * attribute is not supported. Furthermore the condition implementation callback
 * has to return a boolean value.
 *
 * @see hook_rules_action_info().
 */
function hook_rules_condition_info() {
  return array(
    'rules_condition_text_compare' => array(
      'label' => t('Textual comparison'),
      'arguments' => array(
        'text1' => array('label' => t('Text 1'), 'type' => 'string'),
        'text2' => array('label' => t('Text 2'), 'type' => 'string'),
      ),
      'help' => t('TRUE is returned, if both texts are equal.'),
      'module' => 'Rules',
    ),
  );
}

/**
 * Define rules events.
 *
 * This hook is required in order to add a new rules event. It should be
 * placed into the file MODULENAME.rules.inc.
 *
 * @return
 *   An array of information about the module's provided rules events. The array
 *   contains a sub-array for each event, with the event name as the key.
 *
 *   Possible attributes for each sub-array are:
 *
 *    - 'label'
 *         The label of the event. Start capitalized. Required.
 *    - 'module'
 *         The providing module's user readable name. Used for grouping the
 *         events in the interface. Should start with a capital letter.
 *         Required.
 *    - 'arguments'
 *         An array describing the arguments provided by the event with the
 *         argument's name as key. Optional. Each argument has to be described
 *         by a sub-array with possible attributes as described afterwards.
 *   - 'help'
 *         A help text for rules associated with this event. Optional.
 *
 *
 *  Each 'arguments' array may contain the following properties:
 *   - 'label'
 *         The label of the argument. Start capitalized. Required.
 *   - 'type'
 *         The rules data type of the variable. See http://drupal.org/node/298633
 *         for a list of known types. Required.
 *   - 'handler'
 *         A handler to load the actual argument value. This is useful for lazy
 *         loading variables. The handler gets all available variables passed in the
 *         order as defined. Optional. Also see http://drupal.org/node/298554.
 *   - 'saved'
 *         If the variable is saved afterwards, set this to TRUE. So rules knows
 *         about it and won't save the variable a second time. Optional (defaults
 *         to FALSE).
 *
 *
 *  The module has to invoke the event when it occurs using rules_invoke_event().
 *  This function call has to happen outside of MODULENAME.rules.inc, usually it's
 *  invoked directly from the providing module but wrapped by a
 *  module_exists('rules') check.
 *
 *  @see rules_invoke_event().
 */
function hook_rules_event_info() {
  return array(
    'user_insert' => array(
      'label' => t('User account has been created'),
      'module' => 'User',
      'arguments' => rules_events_hook_user_arguments(t('registered user')),
    ),
    'user_update' => array(
      'label' => t('User account details have been updated'),
      'module' => 'User',
      'arguments' => rules_events_hook_user_arguments(t('updated user')) + array(
        'account_unchanged' => array('type' => 'user', 'label' => t('unchanged user')),
      ),
    ),
    'user_view' => array(
      'label' => t('User page has been viewed'),
      'module' => 'User',
      'arguments' => rules_events_hook_user_arguments(t('viewed user')),
      'help' => t("Note that if drupal's page cache is enabled, this event won't be generated for pages served from cache."),
    ),
    'user_delete' => array(
      'label' => t('User has been deleted'),
      'module' => 'User',
      'arguments' => rules_events_hook_user_arguments(t('deleted user')),
    ),
    'user_login' => array(
      'label' => t('User has logged in'),
      'module' => 'User',
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('logged in user')),
      ),
    ),
    'user_logout' => array(
      'label' => t('User has logged out'),
      'module' => 'User',
      'arguments' => array(
        'account' => array('type' => 'user', 'label' => t('logged out user')),
      ),
    ),
  );
}


/**
 * Define rules data types.
 *
 * This hook is required in order to add a new rules data type. It should be
 * placed into the file MODULENAME.rules.inc.
 *
 * @return
 *   An array of information about the module's provided data types. The array
 *   contains a sub-array for each data type, with the data type name as the key.
 *   See http://drupal.org/node/298633.
 *
 *   Possible attributes for each sub-array are:
 *
 *    - 'label'
 *         The label of the data type. Start uncapitalized. Required.
 *    - 'class'
 *         The implementing class of the data type. It has to extend the class
 *         'rules_data_type' and override methods depending on the other
 *         attributes 'identifiable', 'savable' and 'uses_input_form'. Thus this
 *         attribute is optional (defaulting to 'rules_data_type'), but in fact
 *         required for most data types.
 *    - 'identifiable'
 *         Whether data of this type can be identified, e.g. for loading from
 *         the database. Optional (defaults to TRUE).
 *    - 'savable'
 *         Whether data of this type can be saved automatically, so the save()
 *         method has to be implemented. Optional (defaults to FALSE).
 *    - 'uses_input_form'
 *         Whether the data type provides an input form for specifying a data
 *         value. Optional (defaults to not identifiable).
 *    - 'eval input'
 *         If the data type uses an input form, this can be used to enable input
 *         evaluation for it. Optional (defaults to FALSE).
 *    - 'token type'
 *         The type name as used by the token module. Defaults to the type name
 *         as used by rules. Use FALSE to let token ignore this type. Optional.
 *    - 'hidden'
 *         Whether the data type should be hidden from the UI. Optional
 *         (defaults to FALSE).
 *    - 'module'
 *         The providing module's user readable name. Rules generates actions to
 *         save 'savable' data types, thus this attribute is used for those
 *         generated actions. Should start with a capital letter. Optional.
 *
 *  @see class rules_data_type.
 */
function hook_rules_data_type_info() {
  return array(
    'node' => array(
      'label' => t('content'),
      'class' => 'rules_data_type_node',
      'savable' => TRUE,
      'identifiable' => TRUE,
      'module' => 'Node',
    ),
  );
}

/**
 * Define rules input evaluators.
 *
 * This hook is required in order to add a new input evaluator. It should be
 * placed into the file MODULENAME.rules.inc.
 *
 * @return
 *   An array of information about the module's provided input evaluators. The
 *   array contains a sub-array for each evaluator, with the name as the key.
 *
 *   Possible attributes for each sub-array are:
 *
 *    - 'label'
 *         The label of the data type. Start capitalized. Required.
 *    - 'weight'
 *         A weight for controlling the evaluation order of multiple evaluators.
 *         Required.
 *
 *  @see rules_input_evaluator_callback_prepare(), rules_input_evaluator_callback_apply(), rules_input_evaluator_callback_help().
 */
function hook_rules_evaluator() {
  return array(
    'rules_input_evaluator_php' => array(
      'label' => t('PHP Evaluation'),
      'weight' => 0,
    ),
  );
}

/**
 * Prepares the evalution.
 *
 * It is used to determine whether the input evaluator has been used.
 * If so arbitrary data may be passed to application callback - e.g. a list of
 * used variables. It should be placed into the file MODULENAME.rules.inc.
 *
 * @param $string
 *     The string to evaluate later on.
 * @param $variables
 *     An array of info about available variables.
 *
 * @return
 *     Arbitrary data, which is passed to the evaluator on evaluation.
 *     If NULL is returned the input evaluator will be skipped later.
 * @see hook_rules_evaluator(), rules_input_evaluator_php_prepare().
 */
function rules_input_evaluator_callback_prepare($string, $variables) {
  // Scan the string for evaluator usage and
  return TRUE;
}

/**
 * Apply the input evaluator.
 *
 * The implementation should be placed into the file MODULENAME.rules.inc.
 *
 * @param $string
 *     The string to evaluate.
 * @param $data
 *     The data as returned from rules_input_evaluator_callback_prepare().
 * @param $state
 *     The current evaluation state.
 * @param $return_output
 *     The evaluated string.
 * @see hook_rules_evaluator(), rules_input_evaluator_php_apply().
 */
function rules_input_evaluator_callback_apply($string, $data = NULL, &$state, $return_output = TRUE) {
  // Evaluate string.
  return $string;
}


/**
 * Provide some usage help for the evaluator.
 *
 * Usually this uses some theme function to provide themable help. It should
 * be placed into the file MODULENAME.rules_forms.inc or into
 * MODULENAME.rules.inc.
 *
 * @param $variables
 *     An array of info about available variables.
 *
 * @return
 *     Either a rendered string or an array as used by drupal_render().
 * @see hook_rules_evaluator(), rules_input_evaluator_php_help().
 */
function rules_input_evaluator_callback_help($variables) {
  
  foreach ($variables as $name => $info) {
    $form[$name] = array(
      '#type' => 'fieldset',
      '#title' => t('Replacement patterns for @name', array('@name' => $info['label'])),
      '#collapsible' => TRUE,
      '#collapsed' => TRUE,
    );
    $form[$name]['content'] = array(
      '#value' => theme('token_help', $info['type'], '['. $name . ':', ']'),
    );
  }
  return $form;
}

/**
 * Alter rules compatible actions.
 *
 * The implementation should be placed into the file MODULENAME.rules.inc.
 *
 * @param $actions
 *   The items of all modules as returned from hook_rules_action_info().
 *
 * @see hook_rules_action_info().
 */
function hook_rules_action_info_alter(&$actions) {
  // The rules action is more powerful, so hide the core action
  unset($actions['rules_core_node_assign_owner_action']);
  // We prefer handling saving by rules - not by the user.
  unset($actions['rules_core_node_save_action']);
}

/**
 * Alter rules conditions.
 *
 * The implementation should be placed into the file MODULENAME.rules.inc.
 *
 * @param $conditions
 *   The items of all modules as returned from hook_rules_condition_info().
 *
 * @see hook_rules_condition_info().
 */
function hook_rules_condition_info_alter(&$conditions) {
  // Change conditions
}

/**
 * Alter rules events.
 *
 * The implementation should be placed into the file MODULENAME.rules.inc.
 *
 * @param $events
 *   The items of all modules as returned from hook_rules_event_info().
 *
 * @see hook_rules_event_info().
 */
function hook_rules_event_info_alter(&$events) {
  // Change events
}

/**
 * Map core action types to rules actions.
 *
 * Core actions of a special type usually work with one argument and a fixed
 * data type. Mappings from this hook are used to automatically generate
 * rules compatible actions for core actions. To further customize the action
 * info of a specific action use hook_rules_action_info_alter(). Also see
 * http://drupal.org/node/299055.
 *
 * The implementation should be placed into the file MODULENAME.rules.inc.
 *
 *
 * @return
 *   An array of mappings, where the key is the core action type. The provided
 *   mapping values are used for generating hook_rules_action_info() entries
 *   for core action of the given type. Thus all attributes supported by
 *   hook_rules_action_info() can be used here.
 */
function hook_rules_action_type_map() {
  return array(
    'node' => array(
      'module' => 'Node',
      'arguments' => array(
        'node' => array(
          'label' => t('Content'),
          'type' => 'node',
        ),
      ),
    ),
  );
}

/**
 * React on an import of a rule.
 *
 * This hook is called if a rule is imported through the import/export admin
 * interface or if a default rule is provided by a module via
 * hook_rules_defaults().
 *
 * @param $rule
 *   An array representing the rule with its properties.
 */
function hook_rules_import(&$rule) {
  // Examine the rule, e.g. check if it is event-triggered.
  if ($rule['#set'] === 'event_my_module') {
    // Initiate post-processing that is needed to make to rule work.
  }
}

/**
 * Provides a default rule.
 *
 * Modules that implement this hook have to provide a configuration array that
 * contains rules and/or rule sets. You can use the output from the export
 * functionality, but you have to change rules and rule sets names and you need
 * to set the status property to 'default' or 'fixed'.
 *
 * @return
 *   An array containing the configuration.
 * 
 * @see http://drupal.org/node/298634
 */
function hook_rules_defaults() {
  $config = array (
    'rules' =>
    array (
       // A default rule.
      'yourmodule_default_rule_1' => array(
        '#status' => 'default',
        // Your other rule properties here.
      ),
      // A fixed rule (hidden from the Rules admin user interface).
      'yourmodule_default_rule_2' => array(
        '#status' => 'fixed',
        // Your other rule properties here.
      ),
    ),
    'rule_sets' =>
    array(
      // A default rule set.
      'yourmodule_default_set_1' => array(
        'status' => 'default',
        // Your other rule set properties here.
      ),
      // A fixed rule set (hidden from the Rules admin user interface).
      'yourmodule_default_set_2' => array(
        'status' => 'fixed',
        // Your other rule properties here.
      ),
    ),
  );
  return $config;
}

/**
 * @}
 */
