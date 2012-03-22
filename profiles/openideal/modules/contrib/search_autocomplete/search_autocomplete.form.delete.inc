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
function search_autocomplete_form_delete($form, &$form_state, $parent_fid, $fid) {

  if (!$fid) {
    drupal_set_message(
      t('The form has not been found, or the menu callback received a wrong parameter.'),
      'error'
    );
    watchdog(
      'search_autocomplete',
      'The form has not been found, or the menu callback received a wrong parameter.',
      NULL,
      WATCHDOG_ERROR
    );

    return $form;
  }

  $form['parent_fid'] = array(
    '#type' => 'hidden',
    '#value' => $parent_fid,
  );
  $form['fid'] = array(
    '#type' => 'hidden',
    '#value' => $fid,
  );

  return confirm_form(
    $form,
    t('Are you sure you want to delete this form?'),
    'admin/config/search/search_autocomplete',
    NULL,
    t('Delete'),
    t('Cancel')
  );
}

/**
 * Submission callback for the filter delete form.
 */
function search_autocomplete_form_delete_submit($form, &$form_state) {

  $ok = TRUE;
  $values = $form_state['values'];
  $fid = $values['fid'];
  $parent_fid = $values['parent_fid'];

  db_update('search_autocomplete_forms')
            ->fields(array(
              'parent_fid'  => $parent_fid,
            ))
            ->condition('parent_fid', $fid)
            ->execute();
  db_query('DELETE FROM {search_autocomplete_forms} WHERE fid = :fid', array(':fid' => $fid));
  db_query('DELETE FROM {search_autocomplete_suggestions} WHERE sug_fid = :sug_fid', array(':sug_fid' => $fid));

  // Give a return to the user
  drupal_set_message(t("The form has been successfully deleted !"));
  
  $form_state['redirect'] = 'admin/config/search/search_autocomplete';
}