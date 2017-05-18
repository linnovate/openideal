<?php


/**
 * Implements hook_theme() 
 * 
 * create theme to the widget 
 * 
 */
function idea_widget_theme() {
  global $theme;
  global $user;
  global $base_url;
  return array(
    'idea_widget_form' => array(
      'arguments' => array(
        'form' => NULL,
        'domain'=>$base_url
      ),
      'template' => 'templates/idea-widget-form', // set the path here if not in root theme directory
      'render element' => 'form',
    ),
  );
}


/**
 * Implements hook_form_BASE_FORM_ID_alter() for idea_node_form().
 *
 * replace the theme from node_form to idea_widget
 *
 */
function idea_widget_form_idea_node_form_alter(&$form, &$form_state, $form_id) {
  if (!(user_access('access administration pages') || variable_get('disable_idea_widget',false))) {
    $form['#theme'] = 'idea_widget_form';
  }

}