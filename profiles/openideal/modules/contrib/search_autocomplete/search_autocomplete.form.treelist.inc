<?php

/**
 * @file
 * Search Autocomplete
 * Display the list of form to autocomplete and themis it as a draggable table.
 *
 * @authors
 * Miroslav Talenberg (Dominique CLAUSE) <http://www.axiomcafe.fr/contact>
 *
 * Sponsored by:
 * www.axiomcafe.fr
 */

//-------------------------------------------------------------------------------------
/**
 * Menu Callback: create the form to list the searchforms
 * @return     the form
 */
function search_autocomplete_treelist_form($form, &$form_state) {
  $base = "admin/config/search/search_autocomplete";  // base URL for this module configurations

  $data = _search_autocomplete_get_items();  // get all forms ordered as a tree;
  $form['my_items'] = array();
  $form['my_items']['#tree'] = TRUE;

  // for each items to render in the form
  foreach ($data as $values) {
    $fid        = $values->fid;
    $title      = $values->title;
    $weight     = $values->weight;
    $enabled    = $values->enabled;
    $parent_fid = $values->parent_fid;

    $form['my_items'][$fid] = array(       // element for this item
      'title' => array(                      // -> human readeable title
        '#type' => 'item',
        '#title' => check_plain($title),
      ),
      'enabled' => array(                    // -> defines if the autocompletion is enabled for this item
        '#type' => 'checkbox',
        '#default_value' => $enabled,
      ),
      'operations' => array(
        'configure' => array(
          '#type' => 'item',
          '#title' => filter_xss(l(t('configure'), "$base/$fid/configure")),
        ),
        'delete' => array(
          '#type' => 'item',
          '#title' => filter_xss(l(t('delete'), "$base/$parent_fid/delete/$fid")),
        ),
      ),
      'weight' => array(                     // -> weight of the item in hierarchy
        '#type' => 'weight',
        '#delta' => count($data),
        '#default_value' => $weight,
      ),
      'fid' => array(                        // -> the individual id if the item
        '#type' => 'hidden',
        '#value' => $fid,
      ),
      'parent_fid' => array(                 // -> id of the parent item in hierarchy
        '#type' => 'textfield',
        '#default_value' => $parent_fid
      ),
      '#depth' => $values->depth,          // -> depth of the item
    );
  }

  // submit buton
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
} // function search_autocomplete_treelist_form()

//-------------------------------------------------------------------------------------
/**
 * Implementation of hook_submit().
 * Save the changes in the database
 */
function search_autocomplete_treelist_form_submit($form, &$form_state) {
  //Update the database with the new values
  foreach ($form_state['values']['my_items'] as $item) {
    db_update('search_autocomplete_forms')
      ->fields(array(
        'weight' => $item['weight'],
        'parent_fid' => $item['parent_fid'],
        'enabled' => $item['enabled'],
      ))
      ->condition('fid', $item['fid'])
      ->execute();
  }
  drupal_set_message(t('Configuration success'));
} //function search_autocomplete_treelist_form_submit()

/**
 * This function transforms the choices into a string.
 */
function array2str($myarray, &$output = '', $parentkey = "---") {
  foreach ($myarray as $key => $value) {
    if (is_array($value)) {
      $parentkey .= "---";
      array2str($value, $output, $parentkey);
      $parentkey = substr($parentkey, 0, -3);
    } 
    else {
      $output .= $parentkey . "[" . $key . "] => " . $value . "<br/>";
    }
  }
  return $output;
}
//-------------------------------------------------------------------------------------
/**
 * CALLBACK:
 * Theme function for this treelist form
 */
function theme_search_autocomplete_treelist_form($variables) {
  $form = $variables['form'];

  drupal_add_tabledrag('my-draggable-table', 'order', 'sibling', 'weight-group');
  drupal_add_tabledrag('my-draggable-table', 'match', 'parent', 'parent-group', 'parent-group', 'id-group'); 

  $rows = array();
  // for each elements to anchor in the form
  foreach (element_children($form['my_items']) as $key) {
  
    $element                                        = &$form['my_items'][$key];
    $element['weight']['#attributes']['class']      = array('weight-group');
    $element['fid']['#attributes']['class']         = array('id-group');
    $element['parent_fid']['#attributes']['class']  = array('parent-group');

    $rows[] = array(
      'data' => array(
        theme('indentation', array('size' => $element['#depth'])) . drupal_render($element['title']),
        drupal_render($element['enabled']),
        drupal_render($element['weight']) .
        drupal_render($element['fid']) .
        drupal_render($element['parent_fid']),
        drupal_render($element['operations']['configure']),
        drupal_render($element['operations']['delete']),
      ), 
      'class' => array('draggable'),
    );
  }
  // create table headers
  $header = array(t('Form nam'), t('Enabled'), t('Weight'), array(
    'data' => t('Operations'),
    'colspan' => 2,
  ));
  // Themize the table and render the form
  $output  = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'my-draggable-table')));
  $output .= drupal_render($form['submit']);
  $output .= drupal_render_children($form);
  
  return $output;
} //function theme_search_autocomplete_treelist_form()



/////////////////////////////////////////////////////////////////////////////////////////
////                             HELPER FUNCTIONS                                    ////

//-------------------------------------------------------------------------------------
/**
 * Helper function: get the forms from database and render them hierarchically
 * return the items sorted
 */
function _search_autocomplete_get_items() {
  $items = array();
  // get data in database and fetch it
  $result = db_select('search_autocomplete_forms', 'f')
    ->fields('f')
    ->orderBy('weight')  //ORDER BY created
    ->execute()
    ->fetchAllAssoc('fid');
    
  // order the list
  return _search_autocomplete_get_ordered_list(0, $result);
} //function _search_autocomplete_get_items()

//-------------------------------------------------------------------------------------
/**
 * HELPER:
 * Returns a tree list of all items in the $items array that are children
 * of the supplied parent, ordered appropriately
 * @return the ordered tree
 */
function _search_autocomplete_get_ordered_list($parent, $items, $depth = 0) {

  $remnant = array(); $children = array();
  // Insert direct children in $children
  // and remaining in $remnant
  foreach ($items as $item) {
    if ($item->parent_fid == $parent) {
      $item->depth = $depth;
      $children[] = $item;
    }
    else
      $remnant[] = $item;
  }

  // Sort the direct children by weight
  usort($children, '_search_autocomplete_sort_by_weight');

  $ancestors = array();

  foreach ($children as $child) {
    // Order the child ancestors iteratively
    $child_children = _search_autocomplete_get_ordered_list($child->fid, $remnant, $depth + 1);
    // Push the results into the main array below the child
    $ancestors[] = $child;
    // Merge it if needed
    if (count($child_children)) {
      $ancestors = array_merge($ancestors, $child_children);
    }
  }
  return $ancestors;
}

//-------------------------------------------------------------------------------------
/**
 * HELPER:
 * Usort function for sorting arrays by weight
 * @return   1: if ($a < $b),
 *      0 if equal,
 *      -1 otherwise
 */
function _search_autocomplete_sort_by_weight($a, $b) {
  if ($a->weight == $b->weight)
    return 0;
  return ($a->weight < $b->weight) ? -1 : 1;
}

