<?php
// $Id: php.rules.inc,v 1.1.2.11 2009/05/15 13:03:12 fago Exp $


/**
 * @file rules integration for the php module
 * This provides an php input evaluator as well as a execute php code action.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_rules_evaluator().
 */
function php_rules_evaluator() {
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
 * @param $string
 *   The string to evaluate later.
 * @param $variables
 *   An array of available variables.
 *
 * @return
 *   Arbitrary data, which is passed to the evaluator on evaluation.
 *   If NULL is returned the input evaluator will be skipped later.
 */
function rules_input_evaluator_php_prepare($string, $variables) {
  if (strpos($string, '<?') !== FALSE) {
    $used_vars = array();
    foreach ($variables as $name => $info) {
      if (strpos($string, '$'. $name) !== FALSE) {
        $used_vars[] = $name;
      }
    }
    return $used_vars;
  }
}

/**
 * Apply the input evaluator.
 *
 * @param $string
 *   The string to evaluate.
 * @param $data
 *   The data as returned from the prepare() callback.
 * @param $state
 *   The current evaluation state.
 * @param $return_output
 *   Whether to return the output of the PHP code or the data the code returns, which is
 *   used by the "Custom PHP action".
 */
function rules_input_evaluator_php_apply($string, $data = NULL, &$state, $return_output = TRUE) {
  $vars = array();

  if ($data) {
    $vars = rules_get_variables(drupal_map_assoc($data), $state);
    if (!$vars) {
      rules_log(t('PHP is not evaluated as there are not all necessary variables available.'));
      return FALSE;
    }
  }
  $function = $return_output ? 'rules_php_eval' : 'rules_php_eval_return';
  return $function($string, $vars);
}

/**
 * Evalutes the given PHP code, with the given variables defined. This is like
 * drupal_eval() but allows to pass some arguments to the code.
 *
 * @param $code The PHP code to run, with <?php ?>
 * @param $arguments Array containing variables to be extracted to the code
 *
 * @return The output of the php code.
 */
function rules_php_eval($code, $arguments = array()) {
  extract($arguments);

  ob_start();
  print eval('?>'. $code);
  $output = ob_get_contents();
  ob_end_clean();

  return $output;
}

/**
 * Evalutes the given PHP code, with the given variables defined. This is like
 * rules_php_eval() but does return the returned data from the PHP code.
 *
 * @param $code The PHP code to run, without <?php ?>
 * @param $arguments Array containing variables to be extracted to the code
 *
 * @return The return value of eval
 */
function rules_php_eval_return($code, $arguments = array()) {
  extract($arguments);
  return eval($code);
}

/**
 * Implementation of hook_rules_action_info().
 */
function php_rules_action_info() {
  return array(
    'rules_action_custom_php' => array(
      'label' => t('Execute custom PHP code'),
      'module' => 'PHP',
      'eval input' => array('code'),
    ),
  );
}

/**
 * Custom php action: Fires up the input evaluator manually and returns it's return value.
 */
function rules_action_custom_php($settings, $element, &$state) {
  // Note that php code inside of <?php is executed by the input evaluator,
  // while this will execute the resulting string.

  return rules_input_evaluator_php_apply($settings['code'], $settings['vars'], $state, FALSE);
}

/**
 * Implementation of hook_rules_condition_info().
 */
function php_rules_condition_info() {
  return array(
    'rules_condition_custom_php' => array(
      'label' => t('Execute custom PHP code'),
      'module' => 'PHP',
      'eval input' => array('code'),
    ),
  );
}

/**
 * Custom php condition: Fires up the input evaluator manually and returns it's return value.
 */
function rules_condition_custom_php($settings, $element, &$state) {
  // Note that php code inside of <?php is executed by the input evaluator,
  // while this will execute the resulting string.

  return rules_input_evaluator_php_apply($settings['code'], $settings['vars'], $state, FALSE);
}

/**
 * @}
 */