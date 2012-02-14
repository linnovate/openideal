<?php
// $Id: context_condition_sitewide.inc,v 1.1.2.2 2010/08/02 19:54:03 yhahn Exp $

/**
 * Simple sitewide context, always present.
 */
class ideal_challenge_context_condition extends context_condition {
  function condition_values() {
    return array(1 => t('Active challenge page'));
  }

  function editor_form($context = NULL) {
    $form = parent::editor_form($context);
    $form[1]['#title'] = t('Active challenge page');
    $form['#weight'] = -10;
    return $form;
  }

  function execute($value) {
    foreach ($this->get_contexts($value) as $context) {
      context_condition_met($context, $this->plugin);
    }
  }
}
