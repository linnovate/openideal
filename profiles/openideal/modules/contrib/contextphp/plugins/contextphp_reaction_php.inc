<?php

/**
 * @file contextphp_reaction_php.inc
 *    Adds php reaction to Context
 */

/**
 * PHP code as a Context reaction.
 */
class contextphp_reaction_php extends context_reaction {
  /**
   * Editor form.
   */
  function editor_form($context) {
    $form['#type'] = 'value';
    $form['#value'] = TRUE;
    return $form;
  }

  /**
   * Submit handler for editor form.
   */
  function editor_form_submit($context, $values) {
    return array($values);
  }

  function options_form($context) {
    $defaults = $this->fetch_from_context($context, 'options');
    return array(
      'phpcode' => array(
        '#type' => 'textarea',
        '#title' => t('PHP code'),
        '#description' => t('Enter PHP code that will react on the given condition. Do not use &lt;?php ?&gt;.'),
        '#default_value' => isset($defaults['phpcode']) ? $defaults['phpcode'] : '',
      ),
    );
  }

  function execute() {
    foreach (context_active_contexts() as $context) {
      $options = $this->fetch_from_context($context, 'options');
      if (!empty($options['phpcode'])) {
        $code = '<?php ' . $options['phpcode'] . ' ?>';
        if (module_exists('php')) {
          php_eval($code);
        }
        else {
          drupal_set_message(t("Please enable 'PHP filter' to allow the 'Context PHP' module to evaluate your code."), 'warning', TRUE);
          return FALSE;
        }
      }
    }
  }
}
