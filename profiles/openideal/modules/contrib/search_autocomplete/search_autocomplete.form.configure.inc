<?php

/**
 * @file
 * Search Autocomplete
 * Helper functions to retrive suggestions from database
 *
 * @authors
 * Miroslav Talenberg (Dominique CLAUSE) <http://www.axiomcafe.fr/contact>
 *
 * Sponsored by:
 * www.axiomcafe.fr
 */

/**
 * MENU CALLBACK:
 * Define the form to configure the suggestions.
 * @return  A rendered form
 */
function search_autocomplete_form_configure($form, &$form_state) {
  $base = "admin/config/search/search_autocomplete";  // base URL for this module configurations
  // get data from database
  $fid = arg(4);

  $result = db_select('search_autocomplete_forms', 'f')
    ->fields('f')
    ->condition('fid', $fid)
    ->execute()
    ->fetchAllAssoc('fid');

  foreach ($result as $item) {
    $form['fid'] = array(
      '#type' => 'hidden',
      '#value' => $fid,
    );
    // ------------------------------------------------------------------
    // HOW - How to display Search Autocomplete suggestions
    $form['search_autocomplete_how'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('HOW - How to display Search Autocomplete suggestions?'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE
    );
    // Number of characters before suggestions
    $trigger = array();
    for ($i=1; $i<20; $i++) {
      $trigger[$i] = $i . ' ' . t('characters');
    }
    $form['search_autocomplete_how']['min_char'] = array(
      '#type'           => 'select',
      '#title'          => t('Minimum keyword size that uncouple autocomplete search'),
      '#default_value'  => $item->min_char,
      '#options'        => $trigger,
      '#multiple'       => FALSE,
      '#required'       => TRUE,
    );
    // Number of suggestions to display
    $limit = array();
    for ($i=1;$i<50;$i++) {
      $limit[$i] = $i . ' ' . t('results');
    }
    $form['search_autocomplete_how']['max_sug'] = array(
      '#type'           => 'select',
      '#title'          => t('Limit of the autocomplete search result'),
      '#default_value'  => $item->max_sug,
      '#options'        => $limit,
      '#multiple'       => FALSE,
      '#required'       => TRUE
    );

    // ------------------------------------------------------------------ 
    // WHAT - What to display in Search Autocomplete suggestions
    $form['search_autocomplete_what'] = array(
      '#type'           => 'fieldset',
      '#title'          => t('WHAT - What to display in Search Autocomplete suggestions?'),
      '#description'    => t('Choose which data should be added to autocompletion suggestions.'),
      '#collapsible'    => TRUE,
      '#collapsed'      => TRUE,
      '#theme'          => 'search_autocomplete_form_configuration_fieldset'
    );
    $form['search_autocomplete_what']['#tree'] = TRUE;

    // Built possible suggestions
    $result = db_select('search_autocomplete_suggestions', 's')
              ->fields('s')
              ->condition('sug_fid', $fid)
              ->orderBy('sug_weight')
              ->execute()
              ->fetchAllAssoc('sid');

    foreach ($result as $suggestion) {
      // if the module has no dependencies, or if the dependencies are enabled: activate the suggestion
      if (drupal_strlen($suggestion->sug_dependencies) == 0 || module_exists($suggestion->sug_dependencies)) {
        $activate = TRUE;
        $title = $suggestion->sug_title;
      } // else, do not activate the suggestion and has the dependencie reason
      else {
        $activate = FALSE;
        $title = $suggestion->sug_title . '  ' . t('(require @module module)', array('@module' => $suggestion->sug_dependencies));
      }
      $sid = $suggestion->sid;
      
      $form['search_autocomplete_what'][$sid]['sid'] = array(
        '#type'           => 'hidden',
        '#value'          => $sid,
        '#disabled'       => !$activate
      );
      $form['search_autocomplete_what'][$sid]['sug_title'] = array(
        '#type'           => 'item',
        '#title'          => $title,
        '#disabled'       => !$activate
      );
      $form['search_autocomplete_what'][$sid]['sug_enabled'] = array(
        '#type'           => 'checkbox',
        '#return_value'   => 1,
        '#default_value'  => $suggestion->sug_enabled,
        '#disabled'       => !$activate
      );
      $form['search_autocomplete_what'][$sid]['sug_prefix'] = array(                // -> sug_prefix
        '#type'           => 'textfield',
        '#default_value'  => $suggestion->sug_prefix,
        '#maxlength'      => 255,
        '#size'           => 35,
        '#disabled'       => !$activate
      );
      $form['search_autocomplete_what'][$sid]['sug_weight'] = array(                // -> weight of the item in hierarchy
        '#type'           => 'weight',
        '#default_value'  => $suggestion->sug_weight,
        '#disabled'       => !$activate
      );
      $form['search_autocomplete_what'][$sid]['sug_edit'] = array(                  // -> weight of the item in hierarchy
        '#type'           => 'item',
        '#title'          => l(t('edit'), "$base/suggestion/" . $suggestion->sid . "/edit")
      );
    }

    // ------------------------------------------------------------------ 
    // ADVANCED - Advanced options
    $form['search_autocomplete_advanced'] = array(
      '#type'             => 'fieldset',
      '#title'            => t('ADVANCED - Advanced options'),
      '#collapsible'      => TRUE,
      '#collapsed'        => TRUE
    );
    $form['search_autocomplete_advanced']['selector'] = array(
      '#type'             => 'textfield',
      '#title'            => t('ID selector for this form'),
      '#description'      => t('Please change only if you know what you do, read <a href="http://projects.axiomcafe.fr/search-autocomplete">documentation</a> first.'),
      '#default_value'    => $item->selector,
      '#maxlength'        => 255,
      '#size'             => 35
    );
    // Add button validation
    $form['submit'] = array(
      '#type'             => 'submit',
      '#value'            => t('Save configuration')
    );
  }
  return $form;
}

// -------------------------------------------------------------------------------------
/**
 * Implementation of hook_submit().
 * Save the changes in the database
 */
function search_autocomplete_form_configure_submit($form, &$form_state) {
  $ok_query = TRUE;  // so far so good!
  //Update the database with the new values
  $what = '';
  $sids = '';
  $weights = '';

  // ###
  // UPDATE THE FORM
  // -> update form
  $values = $form_state['values'];
  db_update('search_autocomplete_forms')
    ->fields(array(
      'min_char'  => $values['min_char'],
      'max_sug'   => $values['max_sug'],
      'selector'  => $values['selector']
    ))
    ->condition('fid', $values['fid'])
    ->execute();
  // -> update each suggestions
  foreach ($form_state['input']['search_autocomplete_what'] as $key => $item) {
    $values['search_autocomplete_what'][$key]['sug_fid'] = $values['fid'];
    drupal_write_record('search_autocomplete_suggestions', 
      $values['search_autocomplete_what'][$key], 
      array('sid','sug_fid') );
  }
  // ###
  // UPDATE CHILD LIST BUT NOT THE ADVANCED OPTIONS
  $fids = _search_autocomplete_get_all_children($values['fid']);
  // update the settings for this form + every children form
  foreach ($fids as $fid) {
    // -> update form
    db_update('search_autocomplete_forms')
      ->fields(array(
        'min_char' => $values['min_char'],
        'max_sug'   => $values['max_sug']
      ))
      ->condition('fid', $fid)
      ->execute();  
    // -> update each suggestions
    foreach ($form_state['input']['search_autocomplete_what'] as $key => $item) {
      $values['search_autocomplete_what'][$key]['sug_fid'] = $fid;
      drupal_write_record('search_autocomplete_suggestions', 
        $values['search_autocomplete_what'][$key], 
        array('sid','sug_fid') );
    }
  }
  // ###

  $form_state['redirect'] = 'admin/config/search/search_autocomplete';
  $ok_query ? drupal_set_message(t("Configuration success !")) : drupal_set_message(t("An error has occured while saving the settings. Please, double check your settings!"), 'error');
}


// -------------------------------------------------------------------------------------
/**
 * CALLBACK:
 * Theme function for this treelist form
 */
function theme_search_autocomplete_form_configuration_fieldset($variables) {
  $form     = $variables['form'];
  $header    = array('Title', 'Enabled', 'Prefix of the item in suggestions', 'Weight', 'Operation');

  $rows = array();
  // for each elements to anchor in the form
  foreach (element_children($form) as $key) {
    $element = &$form[$key];
    $element['sug_weight']['#attributes']['class'] = array('weight-group');
    
    $rows[] = array(
      'data' => array(
        drupal_render($element['sug_title']),
        drupal_render($element['sug_enabled']),
        drupal_render($element['sug_prefix']),
        drupal_render($element['sug_weight']),
        drupal_render($element['sug_edit'])
      ), 
      'class' => array('draggable'),
    );
  }
  // Themize the table and render the form
  $output  = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'draggable-table')));
  $output .= drupal_render($form['submit']);
  $output .= drupal_render_children($form);
  
  drupal_add_tabledrag('draggable-table', 'order', 'sibling', 'weight-group');
  
  return $output;
} // function theme_search_autocomplete_form_configuration()



/////////////////////////////////////////////////////////////////////////////////////////
////                             HELPER FUNCTIONS                                    ////

// -------------------------------------------------------------------------------------
/**
 * Helper function: get the array of fids every of his children of the caller but not
 * caller fid.
 */
function _search_autocomplete_get_all_children($fid, &$items = array(), $depth = 0) {
  if ($depth)
    $items[] = $fid;
  
  //$result = db_query('SELECT * FROM {search_autocomplete_forms} WHERE parent_fid=:parent_fid', array(':parent_fid' => $fid));
  $result = db_select('search_autocomplete_forms', 'f')
      ->fields('f')
      ->condition('parent_fid', $fid)
      ->execute()
      ->fetchAllAssoc('fid');
      
  foreach ($result as $item) {
    $depth++;
    _search_autocomplete_get_all_children($item->fid, $items, $depth);
  }
  return $items;
}