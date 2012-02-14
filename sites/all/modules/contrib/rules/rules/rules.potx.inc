<?php
// $Id: rules.potx.inc,v 1.1.2.1 2009/04/19 18:19:06 fago Exp $


/**
 * @file This is just a dummy file for the potx translation extractor.
 *   It contains additional calls to t(), so those strings are properly extracted.
 */

function rules_potx() {
  // Calls to rules_handle_error_msg()
  t('An error occured during rule evaluation. It follows the evaluation log: !log');
}