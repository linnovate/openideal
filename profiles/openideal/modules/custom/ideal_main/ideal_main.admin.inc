<?php 
function ideal_main_config_page() {
//  drupal_add_js(drupal_get_path('module', 'ideal_main') . '/ideal_main.js');
//  $a = drupal_get_form('ideal_main_idea_manager_exposed_form');
//  $c = drupal_get_form('ideal_main_project_manager_exposed_form');
//  $b = drupal_get_form('ideal_main_config_form');
//  $d = drupal_get_form('ideal_main_config_project_form');
//  $output = ''; //render($a) . render($c) . render($b)  . render($d);
//  $output .= '<span class="ideas-in-project-left>';
//  $output .= render($a) . render($b);
//  $output .= '</div>';
//  $output .= '<span class="ideas-in-project-right>';
//  $output .= render($c) . render($d);
//  $output .= '</div>';
//  return $output;
  return '';
}
///**
// * Display the ideas to be added to the project.
// */
//function ideal_main_config_form($form) {
//  $ideas = views_get_view_result('ideas_manager', 'block');
//  foreach ($ideas as $li) {
//    $options[$li->nid] = $li->node_title;
//  }
//  
//  $form['ideas'] = array (
//    '#type' => 'select',
//    '#title' => 'Ideas',
//    '#options' => $options,
//    '#multiple' => TRUE,
//    '#suffix' =>  '<a id="add">' . t('Add') . '</a>' 
//  );
//  
//  return $form;
//}
//
///**
// * Look for ideas title.
// */
//function ideal_main_idea_manager_exposed_form($form) {
//  $form = array();
//  $form['title'] = array (
//    '#type' => 'textfield',
//    '#title' => 'title',
//  );
//  
//  $form['actions']['submit'] = array(
//    '#type' => 'submit',
//    '#value' =>  'Filter'
//  );
//  return $form;
//}
//
///**
// * Filter the ideas by Title.
// */
//function ideal_main_idea_manager_exposed_form_submit($form, &$form_state) {
//  $options = array(
//   'query' => array(
//      'text' => $form_state['values']['title'],
//      'project' => isset($_GET['project']) ? $_GET['project'] : 0
//    ),
//    'absolute' => true 
//  );
//
//  $url = url($_GET['q'], $options);
//  $form_state['redirect'] = $url;
//}
//
///**
// * Select the project to manipulate.
// */
//function ideal_main_project_manager_exposed_form($form) {
//  $form = array();
//  $projects = views_get_view_result('projects_manager', 'block');
//  $options = array();
//  $options[0] = '--';
//  foreach ($projects as $li) {
//    $options[$li->nid] = $li->node_title;
//  }
//  $form['select_project'] = array (
//    '#type' => 'select',
//    '#title' => 'Select Project',
//    '#options' => $options,
//    '#default_value' => isset($_GET['project']) ? $_GET['project'] : 0,
//  );
//  
//  
//  $form['actions']['submit'] = array(
//    '#type' => 'submit',
//    '#value' =>  'Set Project'
//  );
//  return $form;
//}
//
///**
// * Set the project to manipulate.
// */
//function ideal_main_project_manager_exposed_form_submit($form, &$form_state) {
//  $options = array(
//   'query' => array(
//      'project' => $form_state['values']['select_project'],
//      'text' => $_GET['text'] ? $_GET['text'] : ''
//    ),
//    'absolute' => true 
//  );
//
//  $url = url($_GET['q'], $options);
//  $form_state['redirect'] = $url;
//}
//
///**
// * Show the ideas already in priject and
// * Set a conteiner to adds ideas to.
// */
//function ideal_main_config_project_form($form) {
//  $projects = views_get_view_result('projects_manager', 'block_1');
//  foreach ($projects as $li) {
//    $options[$li->nid] = $li->node_title;
//  }
//  $form['projects'] = array (
//    '#type' => 'select',
//    '#title' => 'Ideas Already In Project',  
//    '#multiple' => TRUE,
//    '#options' => (isset($_GET['project']) && !empty($_GET['project'])) ? $options : 0,  
//  );
//  
//  $form['ideas_in_project'] = array (
//    '#type' => 'select',
//    '#title' => 'Ideas In Project',
//    '#multiple' => TRUE,
//    '#suffix' =>  '<a id="remove">' . t('Remove') . '</a>' 
//  );
//  
//  $projects1 = views_get_view_result('projects_manager', 'block');
//  $options = array();
//  $options[0] = '--';
//  foreach ($projects1 as $li) {
//    $options1[$li->nid] = $li->node_title;
//  }
//    
//  $form['actions']['save'] = array(
//    '#type' => 'submit',
//    '#value' => t('Save Project'),
//    '#submit' => array('add_ideas_to_project_submit'),
//
//   );
//  return $form;
//}
//
///**
// * Adds Ideas to the selected project.
// */
//function add_ideas_to_project_submit($form, &$form_state) {
//  global $language;
//  $lang = module_exists('local') ? $language : LANGUAGE_NONE;
//  if (isset($_GET['project']) && !empty($_GET['project'])) {
//    foreach ($form_state['values']['ideas_in_project'] as $idea) {
//      $node = node_load($idea);
//      $node->field_project_ref[$lang][]['target_id'] = $_GET['project'];
//      node_save($node); 
//      drupal_set_message(t('The idea @title added to project'), array('@title' => $node->title));
//    }
//  }
//  else {
//    form_set_error('', t('There no project set! Please selact a project and try again'));
//  }
//}