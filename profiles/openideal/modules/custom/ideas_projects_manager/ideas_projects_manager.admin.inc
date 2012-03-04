<?php
function theme_ideas_projects_manager_page() {
  drupal_add_js(drupal_get_path('module', 'ideas_projects_manager') . '/ideas_projects_manager.js');
  drupal_add_css(drupal_get_path('module', 'ideas_projects_manager') . '/ideas_projects_manager.css');
  $a = drupal_get_form('ideas_projects_manager_idea_manager_exposed_form');
  $c = drupal_get_form('ideas_projects_manager_project_manager_exposed_form');
  $b = drupal_get_form('ideas_projects_manager_ideas_list_form');
  $d = drupal_get_form('ideas_projects_manager_config_project_form');
  $output = ''; //render($a) . render($c) . render($b)  . render($d);
  $output .= '<span class="ideas-projects-manager-left">';
  $output .= render($a) . render($b);
  $output .= '</span>';
  $output .= '<span class="ideas-projects-manager-right">';
  $output .= render($c) . render($d);
  $output .= '</span>';
  return $output;
}
/**
 * Display the ideas to be added to the project.
 */
function ideas_projects_manager_ideas_list_form($form) {
  $options = array();
//  $header = array (
//    'idea' => t('Idea'),
//  );
  // Get the Projects from views.
  $ideas = views_get_view_result('ideas_manager', 'block');
  // Loop through the views result to build an array.
  foreach ($ideas as $li) {
    $options[$li->nid] = $li->node_created . " | " . $li->node_title . " By - ";
  }
  
  $form['ideas'] = array (
    '#type' => 'select',
    '#title' => 'Ideas',
//    '#header' => $header,
    '#options' => $options,
    '#multiple' => TRUE,
    '#description' => "The list of ideas. \nUse the the add button under to pass ideas to project. \nFor multiselection use CTRL + click",  
    '#suffix' =>  '<a id="add">' . t('Add') . '</a>' 
  );
  
  return $form;
}

/**
 * Look for ideas title.
 */
function ideas_projects_manager_idea_manager_exposed_form($form) {
  $form = array();
  $form['title'] = array (
    '#type' => 'textfield',
    '#title' => 'title',
    '#description' => 'Filters ideas by their title'  
  );
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' =>  'Filter'
  );
  return $form;
}

/**
 * Filter the ideas by Title.
 */
function ideas_projects_manager_idea_manager_exposed_form_submit($form, &$form_state) {
  // Builde the querystring to the redirect.
  $options = array(
   'query' => array(
      'text' => $form_state['values']['title'],
      'project' => isset($_GET['project']) ? $_GET['project'] : 0
    ),
    'absolute' => true 
  );

  $url = url($_GET['q'], $options);
  $form_state['redirect'] = $url;
}

/**
 * Select the project to manipulate.
 */
function ideas_projects_manager_project_manager_exposed_form($form) {
  $form = array();
  // Get the Projects from views.
  $projects = views_get_view_result('projects_manager', 'block');
  $options = array();
  $options[0] = '--';
  // Loop through the views result to build an array.
  foreach ($projects as $li) {
    $options[$li->nid] = $li->node_title;
  }
  $form['select_project'] = array (
    '#type' => 'select',
    '#title' => 'Select a Project',
    '#options' => $options,
    '#default_value' => isset($_GET['project']) ? $_GET['project'] : 0,
    '#description' => 'For adding ideas to aproject you must select and set aproject.'
  );
  
  
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' =>  'Set Project'
  );
  return $form;
}

/**
 * Set the project to manipulate.
 */
function ideas_projects_manager_project_manager_exposed_form_submit($form, &$form_state) {
  // Builde the querystring to the redirect.
  $options = array(
   'query' => array(
      'project' => $form_state['values']['select_project'],
      'text' => $_GET['text'] ? $_GET['text'] : ''
    ),
    'absolute' => true 
  );

  $url = url($_GET['q'], $options);
  $form_state['redirect'] = $url;
}

/**
 * Show the ideas already in priject and
 * Set a conteiner to adds ideas to.
 */
function ideas_projects_manager_config_project_form($form) {
  $options = array();
  // Get the Ideas from views.
  $ideas = views_get_view_result('projects_manager', 'block_1');
  // Loop through the views result to build an array.
  foreach ($ideas as $li) {
    $options[$li->nid] = $li->node_title;
  }
  // Check if there any selected project.
  if (!isset($_GET['project']) || empty($_GET['project'])) {
    $options = array('No project selected yet.');
  }
  // Check if there any ideas in the selected project.gi
  elseif (!isset($options) || empty($options)) {
    $options = array('There no ideas in this project yet.');
  }
  
  $form['projects'] = array (
    '#type' => 'select',
    '#title' => 'Ideas Already In Project',  
    '#multiple' => TRUE,
    '#options' => $options,
    '#description' => "List of ideas which are already in this project.\nThis list is NOT editable and you can't remove any items from here."    
  );
  
  if (isset($_GET['project']) && !empty($_GET['project'])) {
    $form['ideas_projects_manager'] = array (
      '#type' => 'select',
      '#title' => 'Ideas In Project',
      '#multiple' => TRUE,
      '#description' => 'Any ideas in this list will be added to the selected project (above), when you submit this form.',  
      '#suffix' =>  '<a id="remove">' . t('Remove') . '</a><br />'
    );
        
    $form['actions']['save'] = array(
      '#type' => 'submit',
      '#value' => t('Save Project'),
      '#submit' => array('add_ideas_to_project_submit'),

    );
  }
  return $form;
}

/**
 * Adds Ideas to the selected project.
 */
function add_ideas_to_project_submit($form, &$form_state) {
  global $language;
  $lang = module_exists('local') ? $language : LANGUAGE_NONE;
  if (isset($_GET['project']) && !empty($_GET['project'])) {
    foreach ($form_state['values']['ideas_projects_manager'] as $idea) {
      $node = node_load($idea);
      $node->field_project_ref[$lang][]['target_id'] = $_GET['project'];
      node_save($node); 
      drupal_set_message(t('The idea @title added to project'), array('@title' => $node->title));
    }
  }
  else {
    form_set_error('', t('There no project set! Please selact a project and try again'));
  }
}