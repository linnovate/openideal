<?php
// $Id: voting_rules.rules.inc,v 1.1 2010/07/27 08:53:36 heydemo Exp $

/**
 * @file
 * Provides Rules integration for the Votingapi module 
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implementation of hook_rules_data_type_info().
 */
function vote_rules_data_type_info() {
  return array(
    'vote' => array(
      'label' => t('Vote'),
      'class' => 'rules_data_type_vote',
      'savable' => FALSE,
      'identifiable' => FALSE,
    ),
    'vote' => array(
      'label' => t('Vote Results'),
      'class' => 'rules_data_type_vote_results',
      'savable' => FALSE,
      'identifiable' => FALSE,
    ),
  );
}

/**
 * Implementation of hook_rules_event_info().
 */
function votingapi_rules_event_info() {
  $events = array();
  $content_types = array('node', 'user', 'comment'); 

  foreach ($content_types as $content_type) { 
    $events['voting_rules_insert_'. $content_type] = array(
      'label' => t('User votes on a @type', array('@type' => $content_type)),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote' => array('type' => 'vote', 'label' => t('The vote')),
        $content_type => array('type' => $content_type, 'label' => t("The @type being voted on", array('@type' => $content_type))),
      ),
      'redirect' => FALSE,
    );

    $events['voting_rules_delete_'. $content_type] = array(
      'label' => t('User deletes a vote on a @type', array('@type' => $content_type)),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote' => array('type' => 'vote', 'label' => t('The vote')),
        $content_type => array('type' => $content_type, 'label' => t("The @type being voted on", array('@type' => $content_type))),
      ),
      'redirect' => FALSE,
    );
    
    $events['voting_rules_results_'. $content_type] = array(
      'label' => t('Votes are calculated for a @type', array('@type' => $content_type)),
      'module' => 'Votingapi',
      'arguments' => array(
        'vote_results' => array('type' => 'vote_results', 'label' => t('The results of the vote')),
        $content_type => array('type' => $content_type, 'label' => t("The @type being voted on", array('@type' => $content_type))),
      ),
      'redirect' => FALSE,
    );
  }
  return $events;
}

/**
 * Implementation of hook_rules_condition_info().
 */
function voting_rules_rules_condition_info() {
  return array(
    'voting_rules_condition_check_vote_value' => array(
      'label' => t('Check the value of the vote'),
      'arguments' => array(
        'vote' => array('type' => 'vote', 'label' => t('Vote')),
      ),
      'module' => 'Votingapi',
    ),
    'voting_rules_condition_check_results' => array(
      'label' => t('Evaluate the results of a vote'),
      'arguments' => array(
        'vote_results' => array('type' => 'vote_results', 'label' => t('Results of the Vote')),
      ),
      'module' => 'Votingapi',
    ),
  );
}

/**
 *  Condition: Evaluate the results of the vote
 */
function voting_rules_condition_check_results($vote_results, $settings) {
  foreach ($vote_results as $data) {
    if ($data['function'] == $settings['function']) {
      return eval('return '. $data['value'] . $settings['operator'] . $settings['value'] .';'); 
    }
  }
}

/**
 * Condition: Check the value of an individual vote 
 */
function voting_rules_condition_check_vote_value($vote, $settings) {
  $string = 'return '. $vote['value'] . $settings['operator'] . $settings['value'] .';';
  return eval($string);
}

/**
 * Voting Rules condition check vote value configuration form
 */
function voting_rules_condition_check_vote_value_form($settings, &$form) {
  $form['settings']['vote'] = array(
    '#value' => '<strong>'. t("Vote value") .'</strong>',
  );
  $form['settings']['operator'] = voting_rules_operator_form_element($settings);
  $form['settings']['value'] = array(
    '#type' => 'textfield',
    '#default_value' => $settings['value'],
    '#required' => TRUE,
  );
}

/**
 * Voting Rules condition check vote results configuration form
 */
function voting_rules_condition_check_results_form($settings, &$form) {
  $form['settings']['function'] = array(
    '#title' => t("Results"),
    '#type' => 'select',
    '#options' => array('average' => t("Average vote"), 'sum' => t("Sum of all votes"), 'count' => t("Number of votes")),
    '#default_value' => $settings['function'],
  );
  $form['settings']['operator'] = voting_rules_operator_form_element($settings);
  $form['settings']['value'] = array(
    '#title' => t("Value"),
    '#type' => 'textfield',
    '#default_value' => $settings['value'],
  );
}

/**
 * Helper function that returns the >,>=,<,<=,=== select form element
 */
function voting_rules_operator_form_element($settings) {
  return array(
    '#title' => t("Operation"),
    '#type' => 'select',
    '#default_value' => $settings['operator'],
    '#required' => TRUE,
    '#options' => array(
      '' => t("Select"), '>' => t("Greater than"), '>=' => t("Greater than / equal to"), '<' => t("Less than"),
      '<=' => t("Less than / equal to"), '===' => t("Equal to"),  '!=' => t("Not equal to")
    ),
  );
}

/**
 * Voting Rules check vote value configuration form validation function
 */
function voting_rules_condition_check_vote_value_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['settings']['value'])) {
    form_set_error('settings][value', 'Value must be numeric.');
  }
}

/**
 * Voting Rules check vote result configuration form validation function
 */
function voting_rules_condition_check_results_validate($form, $form_state) {
  if (!is_numeric($form_state['values']['settings']['value'])) {
    form_set_error('settings][value', 'Value must be numeric.');
  }
}

/**
 * @}
 */
