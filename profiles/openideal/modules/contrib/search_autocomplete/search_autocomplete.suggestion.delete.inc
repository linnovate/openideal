<?php

/**
 * @file
 * Search Autocomplete
 * Delete a form from Search Autocomplete form list.
 *
 * @authors
 * Miroslav Talenberg (Dominique CLAUSE) <http://www.axiomcafe.fr/contact>
 *
 * Sponsored by:
 * www.axiomcafe.fr
 */

/**
 * Return the filter delete form.
 */
function search_autocomplete_suggestion_delete($form_state) {
  $form = array();

  // get data from database
  $sid = arg(5);

  if (!$sid && !is_int($sid)) {
    drupal_set_message(
      t('The suggestion has not been found, or the menu callback received a wrong parameter.'),
      'error'
    );
    watchdog(
      'search_autocomplete',
      'The suggestion has not been found, or the menu callback received a wrong parameter.',
      NULL,
      WATCHDOG_ERROR
    );

    return $form;
  }

  $form['sid'] = array(
    '#type'   => 'hidden',
    '#value'  => $sid,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete this suggestion?'),
    'admin/config/search/search_autocomplete',
    NULL,
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submission callback for the filter delete form.
 */
function search_autocomplete_suggestion_delete_submit($form, &$form_state) {
  $ok = TRUE;
  $values = $form_state['values'];
  $sid = $values['sid'];

  $ok .= db_delete('search_autocomplete_suggestions')
      ->condition('sid', $sid)
      ->execute();

  // Give a return to the user
  $ok ? drupal_set_message(t("The suggestion has been successfully deleted !")) : drupal_set_message(t("An error has occured while deleting the suggestion!"), 'error');

  $form_state['redirect'] = 'admin/config/search/search_autocomplete';
}