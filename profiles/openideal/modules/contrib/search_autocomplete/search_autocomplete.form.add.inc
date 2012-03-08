<?php

/**
 * @file
 * Search Autocomplete
 * Add a new form to Search Autocomplete form list.
 *
 * @authors
 * Miroslav Talenberg (Dominique CLAUSE) <http://www.axiomcafe.fr/contact>
 *
 * Sponsored by:
 * www.axiomcafe.fr
 */

/**
 * MENU CALLBACK:
 * Define the page to add a form.
 * @return  A rendered form
 */
function search_autocomplete_form_add() {
  $form = array();
  /* ------------------------------------------------------------------ */
  $form['title'] = array(
    '#title'          => t('Title'),
    '#description'    => 'Please enter a title for this form',
    '#type'           => 'textfield',
    '#default_value'  => '',
    '#maxlength'      => 255,
    '#required'       => TRUE,
  );
  $descr = t('Enter a valid query selector for the form. This should be an id or a class surrounding the input box.') . '<br/>' . t('Do not include input. In case of a doubt, refer to the <a href="http://projects.axiomcafe.fr/search-autocomplete">documentation</a>');
  $form['selector'] = array(
    '#title'          => t('Selector'),
    '#description'    => $descr,
    '#type'           => 'textfield',
    '#default_value'  => '',
    '#maxlength'      => 255,
    '#required'       => TRUE,
  );
  // submit buton
  $form['submit'] = array(
    '#type'           => 'submit',
    '#value'          => t('Save'),
  );
  return $form;
}

// -------------------------------------------------------------------------------------
/**
 * Implements hook_submit().
 * Save the new form in database
 */
function search_autocomplete_form_add_submit($form, &$form_state) {
  $ok_query = TRUE;  // so far so good!

  // Update the database with the new values
  $what = '';
  $sids = '';
  $weights = '';

  // Get the form values
  $values = $form_state['values'];

  // Check if aready existing records
  $result = db_select('search_autocomplete_forms', 'f')
    ->fields('f')
    ->condition('title',    $values['title'])
    ->condition('selector', $values['selector'])
    ->execute()
    ->fetchAll();

  foreach ($result as $obj) {
    drupal_set_message(t("The title or the selector already exists. Please choose another one."), 'error');
    return;
  }

  // Insert the new form in database
  $fid = db_insert('search_autocomplete_forms')
          ->fields(array(
            'title' => $values['title'],
            'selector' => $values['selector']
          ))
          ->execute();

  // Connect default suggestions to this form
  $result = db_select('search_autocomplete_suggestions', 's')
    ->fields('s')
    ->condition('sug_fid', 0)
    ->execute()
    ->fetchAllAssoc('sid');

  foreach ($result as $match) {
    db_insert('search_autocomplete_suggestions')
      ->fields(array(
        'sid'               => $match->sid,
        'sug_fid'           => $fid,
        'sug_enabled'       => $match->sug_enabled,
        'sug_prefix'        => $match->sug_prefix,
        'sug_title'         => $match->sug_title,
        'sug_name'          => $match->sug_name,
        'sug_dependencies'  => $match->sug_dependencies,
        'sug_weight'        => $match->sug_weight,
        'sug_query'         => $match->sug_query
      ))
      ->execute();
  }

  // redirect to configuration page
  $form_state['redirect'] = 'admin/config/search/search_autocomplete/' . $fid . '/configure';

  // Give a return to the user
  $ok_query ? drupal_set_message(t('The form has been created successfully !') . '<br/>' . t('Please check its configuration.')) : drupal_set_message(t("An error has occured while creating the form. Please, double check your settings!"), 'error');
}