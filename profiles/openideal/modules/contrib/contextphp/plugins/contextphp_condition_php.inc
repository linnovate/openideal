<?php

/**
 * @file contextphp_condition_php.inc
 *    Adds php argument condition to Context
 */

/**
 * PHP code as a Context condition.
 */
class contextphp_condition_php extends context_condition {
  function condition_values() {
    return array(1 => t('True'));
  }

  /**
   * Our value is always TRUE. The PHP code in the options form descides if it
   * validates or not on runtime.
   */
  function condition_form($context) {
    $form['#type'] = 'value';
    $form['#value'] = TRUE;
    return $form;
  }

  function condition_form_submit($values) {
    return array($values);
  }

  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    return array(
      'phpcode' => array(
        '#type' => 'textarea',
        '#title' => t('PHP code'),
        '#description' => t('Enter PHP code that returns TRUE if the condition shall be met. Do not use &lt;?php ?&gt;.'),
        '#default_value' => isset($defaults['phpcode']) ? $defaults['phpcode'] : '',
      ),
    );
  }

  function execute() {
    foreach (context_enabled_contexts() as $context) {
      $options = $this->fetch_from_context($context, 'options');
      if (!empty($options['phpcode'])) {
        $code = '<?php ' . $options['phpcode'] . ' ?>';
        if (module_exists('php')) {
          $return = php_eval($code);
        }
        else {
          drupal_set_message(t("Please enable 'PHP filter' to allow the 'Context PHP' module to evaluate your code."), 'warning', TRUE);
          return FALSE;
        }

        if ($return == TRUE) {
          $this->condition_met($context, $return);
        }
      }
    }
  }
}
