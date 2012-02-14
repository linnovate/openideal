<?php
// $Id: rules_admin.render.inc,v 1.1.2.3 2009/08/03 16:42:25 fago Exp $


/**
 * @file Rules Admin UI
 *   Functions for rendering a rule.
 *   If a rule is viewed, it's passed to drupal_get_form so it is rendered with drupal_render().
 */

/**
 * Adds surrounding elements like headlines for conditions and actions
 * and renders the rule
 */
function theme_rules_admin_rule_render($element) {
  //add css
  $path = drupal_get_path('module', 'rules_admin') .'/rules_admin.css';
  drupal_add_css($path, 'module', 'all', FALSE);
  
  if (rules_admin_is_event_rule($element)) {
    // Render event headline on event-triggered rules
    $events = rules_get_event_sets();
    $event_label = $events[$element['#set']]['label'];
    $render['event_headline'] = array('#weight' => -10, '#value' => '<h3 class="event">'. t('ON event %event', array('%event' => $event_label)) .'</h3>');
  }
  else {
    // Render rule set headline
    $rulesets = rules_get_rule_sets();
    $ruleset_label = $rulesets[$element['#set']]['label'];
    $render['event_headline'] = array('#weight' => -10, '#value' => '<h3 class="event">'. t('IN rule set %ruleset', array('%ruleset' => $ruleset_label)) .'</h3>');
  }
  
  $conditions = element_children($element['#conditions']);
  if (!empty($conditions)) {
    $render['condition_headline'] = array('#weight' => -6, '#value' => '<h3 class="conditions">'. t('IF') .'</h3>');
    $render['conditions'] = $element['#conditions'];
    $render['conditions']['#weight'] = 0;
    
    /* render the conditions of the rule like an AND */
    $render['conditions'] += array('#theme' => 'rules_operation', '#label' => t('AND'), '#_root' => TRUE);
  }

  $render['actions_headline'] = array('#weight' => 10, '#value' => '<h3 class="actions">'. t('DO') .'</h3>');
  $render['actions'] = $element['#actions'];
  $render['actions']['#weight'] = 20;

  $add_img = theme('rules_icon', 'add') .'';
  $link_options = array('attributes' => array('class' => 'modify'), 'query' => drupal_get_destination());
  $render['condition_add'] = array('#weight' => 5, '#value' => $add_img . l(t('Add a condition'), RULES_ADMIN_RULE_PATH .'/'. $element['#name'] .'/add/condition/1', $link_options), '#prefix' => '<p>', '#suffix' => '</p>');
  $render['action_add'] = array('#weight' => 100, '#value' => $add_img . l(t('Add an action'), RULES_ADMIN_RULE_PATH .'/'. $element['#name'] .'/add/action/'. $element['#actions']['#id'], $link_options), '#prefix' => '<p>', '#suffix' => '</p>');

  //propagate the rule name down the tree, as the name is needed for rendering the elements
  $render['#rule'] = $element['#name'];
  rules_prepare_render($render);

  //add a surrounding div to prevent bugs with the fieldset
  $render['#prefix'] = '<div>';
  $render['#suffix'] = '</div>';

  return drupal_render($render);
}

/**
 * Propagates the #rule property down the tree
 * and set #sorted, as we have already sorted the rule.
 */
function rules_prepare_render(&$elements) {
  foreach (element_children($elements) as $key) {
    $elements[$key]['#rule'] = $elements['#rule'];
    $elements[$key]['#sorted'] = TRUE;
    rules_prepare_render($elements[$key]);
  }
}

function theme_rule($element) {
  return $element['#children'];
}

/**
 * Renders a condition
 */
function theme_condition($element) {
  $attributes = isset($element['#attributes']) ? $element['#attributes'] : array();
  $title      = theme('rules_icon', 'condition') . check_plain(rules_get_element_label($element));
  $path       = RULES_ADMIN_RULE_PATH .'/'. $element['#rule'] .'/edit/'. $element['#id'];
  $options    = array('attributes' => _rules_attributes_add_class($attributes, 'condition'), 'query' => drupal_get_destination(), 'html' => TRUE);
  $link       = l($title, $path, $options);

  $path = RULES_ADMIN_RULE_PATH .'/'. $element['#rule'] .'/add/op/'. $element['#id'];
  $options['attributes'] = _rules_attributes_add_class($attributes, 'condition_add');
  $indent_link = l(theme_rules_icon('indent', t('Indent this condition by adding a logical operation.')), $path, $options);

  $print_op = $element['#negate'] ? theme('rules_logical_operation_label', 'not', t('NOT')) .' ' : '';
  return $print_op . $link .' '. $indent_link;
}

/**
 * Renders an action
 */
function theme_action($element) {
  $attributes = isset($element['#attributes']) ? $element['#attributes'] : array();
  $title      = theme('rules_icon', 'action') . check_plain(rules_get_element_label($element));
  $path       = RULES_ADMIN_RULE_PATH .'/'. $element['#rule'] .'/edit/'. $element['#id'];
  $options    = array('attributes' => _rules_attributes_add_class($attributes, 'action'), 'query' => drupal_get_destination(), 'html' => TRUE);
  return l($title, $path, $options);
}

/**
 * Themes a icon
 */
function theme_rules_icon($name, $title = NULL) {
  $path = drupal_get_path('module', 'rules_admin') .'/icons/'. $name .'.png';
  return theme('image', $path, $name, $title ? $title : $name, array('class' => 'rules-icon'));
}

function _rules_attributes_add_class($attributes, $class) {
  if (isset($attributes['class'])) {
    $attributes['class'] .= ' '. $class;
  }
  else {
    $attributes['class'] = $class;
  }
  return $attributes;
}

/**
 * Themes the children of a logical operation. It put the operation in between each children.
 * This function is invoked through the #theme property of the logical operations.
 */
function theme_rules_operation($element) {
  if (count(element_children($element)) == 0 && !isset($element['#_root'])) {
    //no children, so drupal_render() won't render the element itself, so we do it ourself except for the condition root (AND)
    $element['#children'] = t('Empty');
    return theme('OR', $element);
  }
  //render the children and put the operation between them
  $content = array();
  foreach (element_children($element) as $key) {
    $content[] = drupal_render($element[$key]);
  }
  $print_op = theme('rules_logical_operation_label', drupal_strtolower($element['#label']), $element['#label']);
  return implode($print_op, $content);
}

/**
 * Themes the OR condition group
 */
function theme_OR($element) {
  $attributes = isset($element['#attributes']) ? $element['#attributes'] : array();
  $element['#attributes'] = _rules_attributes_add_class($attributes, 'rules_'. $element['#type']);
  $element['#collapsible'] = FALSE;

  $print_op = $element['#negate'] ? t('NOT') .' ' : '';
  $element['#title'] = t("!not%label group", array('!not' => $print_op, '%label' => $element['#label']));
  $element['#children'] .= '<p class="logical-op-add">'. theme('rules_icon', 'add') . l(t('Add another condition to this group'), RULES_ADMIN_RULE_PATH .'/'. $element['#rule'] .'/add/condition/'. $element['#id']) .'</p>';
  $element['#children'] .= '<p class="logical-op-edit">'. theme('rules_icon', 'edit') . l(t('Edit this condition group'), RULES_ADMIN_RULE_PATH .'/'. $element['#rule'] .'/edit/'. $element['#id']) .'</p>';
  return theme('fieldset', $element);
}

/**
 * Themes the AND condition group
 */
function theme_AND($element) {
  return theme('OR', $element);
}

/**
 * Themes the operation label
 */
function theme_rules_logical_operation_label($op, $label) {
  return "<div class='logical-op-$op'>$label</div>";
}

