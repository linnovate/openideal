<?php
function ideal_main_config_page() {
  $a = drupal_get_form('ideal_main_idea_manager_exposed_form');
  $b = drupal_get_form('ideal_main_config_form');
  $output = render($a) . render($b);
 
  return $output;
}
/**
 * Form for configurations.
 */
function ideal_main_config_form($form) {
  drupal_add_js(drupal_get_path('module', 'ideal_main') . '/ideal_main.js');
  $ideas = views_get_view_result('ideas_manager', 'block');

//  $header = array (
//    'ideas' => t('Ideas'),
//  );
  foreach ($ideas as $li) {
//    $options[$li->nid] = array('ideas' => $li->node_title);
    $options[$li->nid] = $li->node_title;
  }
//  
//  $form = array();
//  $form['table'] = array (
//    '#type' => 'tableselect',
//    '#header' => $header,
//    '#options' => $options,
//    '#empty' => t('No users found'),
//  );
  
  $form['ideas'] = array (
    '#type' => 'select',
    '#title' => 'Ideas',
    '#options' => $options,
    '#multiple' => TRUE,
    '#suffix' =>  '<a id="add">' . t('Add') . '</a>' 
  );
  
  $projects = views_get_view_result('projects_manager', 'block');
  foreach ($projects as $li) {
    $p_options[$li->nid] = $li->node_title;
  }
  $form['projects'] = array (
    '#type' => 'select',
    '#title' => 'Projects',
    '#options' => $p_options,
  );
  $form['ideas_in_project'] = array (
    '#type' => 'select',
    '#title' => 'Ideas',
    '#options' => array(),
    '#multiple' => TRUE,
    '#suffix' =>  '<a id="remove">' . t('Remove') . '</a>' 
  );
  
//  $form['projects'] = array (
//    '#type' => 'markup',
//    '#title' => 'projects',
//    '#markup' => views_embed_view('projects_manager', 'block'),
//  );
  return system_settings_form($form);
}

/**
 * Form for configurations.
 */
function ideal_main_idea_manager_exposed_form($form) {
  $form = array();
  $form['title'] = array (
    '#type' => 'textfield',
    '#title' => 'title',
  );
  
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' =>  'Filter'
  );
  return $form;
}
/**
 * Form for configurations.
 */
function ideal_main_idea_manager_exposed_form_submit($form, &$form_state) {
  $options = array(
   'query' => array(
      'text' => $form_state['values']['title']
    ),
    'absolute' => true 
  );

  $url = url($_GET['q'], $options);
  $form_state['redirect'] = $url;
}
