<?php
// $Id: php.rules_forms.inc,v 1.1.2.8 2009/05/15 13:03:12 fago Exp $


/**
 * @file Rules configuration forms for the php module
 *
 * @addtogroup rules
 * @{
 */

/**
 * Returns some help for the admin using the evaluator
 */
function rules_input_evaluator_php_help($variables, $save_info = FALSE) {
  $output = '<p>'. t('PHP code inside of &lt;?php ?&gt; delimiters will be evaluated and replaced by its output. E.g. &lt;? echo 1+1?&gt; will be replaced by 2.');
  $output .= ' '. t('Furthermore you can make use of the following variables:') .'</p>';
  $headers = array(t('Variable'), t('Type'), t('Description'),
  );
  if ($save_info) {
    $headers[] = t('Intelligent saving');
  }
  $rows = array();
  $data_types = rules_get_data_types();
  foreach ($variables as $name => $info) {
    $row   = array();
    $row[] = '$'. check_plain($name);
    $row[] = check_plain($data_types[$info['type']]['label']);
    $row[] = check_plain($info['label']);
    if ($save_info) {
      $data_type = rules_get_data_object($info);
      if ($data_type->is_savable()) {
        $row[] = t('Yes');
      }
      else {
        $row[] = t('No');
      }
    }
    $rows[] = $row;
  }

  $output .= theme('table', $headers, $rows, array('class' => 'rules-php-help'));
  if ($save_info) {
    $output .= '<p>'. t('Note that variables are passed by reference, so you can change them.') .' ';
    $output .= t("If you want to make the changes permanent, you can let rules intelligently save the changes when the variable's data type supports it.") .' '. t('To make use of "intelligent saving" just return an array of variables to save, e.g.: !code So variables are saved only once, even if modified multiple times.', array('!code' => '<pre>return array("node" => $node);</pre>')) .'</p>';
  }
  return $output;
}

/**
 * Execute custom php code action configuration form.
 */
function rules_action_custom_php_form($settings, &$form, $form_state) {
  $form['settings']['code'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP Code'),
    '#default_value' => isset($settings['code']) ? $settings['code'] : '',
    '#description' => t("The code that should be executed. Don't include &lt;?php ?&gt; delimiters."),
    '#required' => TRUE,
  );
  // add help including some info for saving the variables
  $variables = $form_state['proxy']->get_available_variables($form_state['element']['#id']);
  $help = rules_input_evaluator_php_help($variables, TRUE);
  $form['input_help']['rules_input_evaluator_php']['#collapsed'] = FALSE;
  $form['input_help']['rules_input_evaluator_php']['content'] = array('#value' => $help);
}

/**
 * Execute custom php code action configuration form.
 */
function rules_action_custom_php_submit(&$settings, $form, $form_state) {
  $variables = $form_state['proxy']->get_available_variables($form_state['element']['#id']);
  $settings['vars'] = rules_input_evaluator_php_prepare('<?'. $settings['code'], $variables);
}

/**
 * Execute custom php code condition configuration form.
 */
function rules_condition_custom_php_form($settings, &$form, $form_state) {
  $form['settings']['code'] = array(
    '#type' => 'textarea',
    '#title' => t('PHP Code'),
    '#default_value' => isset($settings['code']) ? $settings['code'] : '',
    '#description' => t("The code that should be executed. Don't include &lt;?php ?&gt; delimiters.") .' '. t('Be sure to always return a boolean value, e.g.: !code', array('!code' => '<pre>return $author->name != "bot";</pre>')),
    '#required' => TRUE,
  );
  $form['input_help']['rules_input_evaluator_php']['#collapsed'] = FALSE;
}

/**
 * Custom PHP condition configuration form validation
 * Checks for at least one return statement
 */
function rules_condition_custom_php_validate($form, $form_state) {
  if (strpos($form_state['values']['settings']['code'], 'return') === FALSE) {
    form_set_error('code', t('The code has to always return a boolean value.'));
  }
}

/**
 * Execute custom php code action configuration form.
 */
function rules_condition_custom_php_submit(&$settings, $form, $form_state) {
  $variables = $form_state['proxy']->get_available_variables($form_state['element']['#id']);
  $settings['vars'] = rules_input_evaluator_php_prepare('<?'. $settings['code'], $variables);
}

/**
 * The following functions help converting the workflow-ng php condition/actions
 * when upgrading from workflow-ng
 */
function workflow_ng_action_custom_php_upgrade(&$element) {
  $element['#name'] = 'rules_action_custom_php';
  $old_settings = $element['#settings'];
  $element['#settings'] = array('code' => $old_settings['php']);
  $element['#settings']['code_args'] = $old_settings['used_arguments'];
  $element['#settings']['vars'] = $old_settings['used_php_arguments'];
}

function workflow_ng_condition_custom_php_upgrade(&$element) {
  workflow_ng_action_custom_php_upgrade($element);
  $element['#name'] = 'rules_condition_custom_php';
}

/**
 * @}
 */
