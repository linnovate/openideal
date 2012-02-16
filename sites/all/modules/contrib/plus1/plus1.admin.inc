<?php

/**
 * @file
 * This file contains declartion for admin form.
 */

/**
 * Menu callback to configure module settings.
 */
function plus1_settings() {
  $form['plus1_global_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Plus1 global settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['plus1_global_settings']['plus1_add_js'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include standard Plus1 js file.'),
    '#default_value' => variable_get('plus1_add_js', 1),
  );

  $form['plus1_global_settings']['plus1_add_css'] = array(
    '#type' => 'checkbox',
    '#title' => t('Include standard Plus1 css file.'),
    '#default_value' => variable_get('plus1_add_css', 1),
  );

  $form['plus1_node_widget_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Node widget settings'),
    '#description' => t('You may select none, one or both options.'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );

  $form['plus1_node_widget_settings']['plus1_content_types'] = array(
    '#type' => 'fieldset',
    '#title' => t('Content types'),
    '#collapsible' => FALSE,
  );

  $form['plus1_node_widget_settings']['plus1_content_types']['plus1_node_types'] = array(
    '#type' => 'checkboxes',
    '#options' => node_type_get_names(),
    '#default_value' => variable_get('plus1_node_types', array('story')),
  );

  $form['plus1_node_widget_settings']['plus1_node_in_teaser'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a Plus 1 voting widget to the node in teaser view.'),
    '#default_value' => variable_get('plus1_node_in_teaser', 0),
  );

  $form['plus1_node_widget_settings']['plus1_node_in_full_view'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add a Plus 1 voting widget to the node in full view.'),
    '#default_value' => variable_get('plus1_node_in_full_view', 1),
  );

  $form['plus1_node_widget_settings']['plus1_node_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Vote text"),
    '#default_value' => variable_get('plus1_node_vote_text', t('Vote')),
    '#description' => t('Enter, for example, <em>Vote</em> or <em>Digg</em>.'),
  );

  $form['plus1_node_widget_settings']['plus1_node_voted_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Feedback provided to voter when he already voted"),
    '#default_value' => variable_get('plus1_node_voted_text', t('You voted')),
    '#description' => t('Enter, for example, <em>Dugg</em>, <em>You voted</em>, or <em>Voted</em>.'),
  );

  $form['plus1_node_widget_settings']['plus1_node_undo_vote'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to undo their node votes"),
    '#default_value' => variable_get('plus1_node_undo_vote', 0),
  );

  $form['plus1_node_widget_settings']['plus1_node_undo_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Feedback provided to voter when he already voted and user CAN undo his vote"),
    '#default_value' => variable_get('plus1_node_undo_vote_text', ''),
    '#description' => t('Enter, for example, <em>Undugg</em>, <em>Undo vote</em>, or <strong>leave it blank to provide user with "arrow down" icon.</strong>'),
    '#states' => array(
      'visible' => array(
        'input[name="plus1_node_undo_vote"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['plus1_node_widget_settings']['plus1_node_widget_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight of node voting widget'),
    '#default_value' => variable_get('plus1_node_widget_weight', '100'),
    '#description' => t('The higher the weight the lower in the node the widget will be added. Can be a negative number.'),
  );

  $form['plus1_comment_widget_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Comment widget settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['plus1_comment_widget_settings']['plus1_comment_widget_show'] = array(
    '#type' => 'checkbox',
    '#title' => t("Add Plus 1 voting widget to comments"),
    '#default_value' => variable_get('plus1_comment_widget_show', 0),
  );

  $form['plus1_comment_widget_settings']['plus1_comment_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Vote text"),
    '#default_value' => variable_get('plus1_comment_vote_text', t('Vote')),
    '#description' => t('Enter, for example, <em>Vote</em> or <em>Digg</em>.'),
  );

  $form['plus1_comment_widget_settings']['plus1_comment_voted_text'] = array(
    '#type' => 'textfield',
    '#title' => t('Feedback provided to voter when he already voted'),
    '#default_value' => variable_get('plus1_comment_voted_text'),
    '#description' => t('Enter, for example, <em>Dugg</em>, <em>You voted</em>, or <em>Voted</em>.'),
  );

  $form['plus1_comment_widget_settings']['plus1_comment_undo_vote'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to undo their comment votes"),
    '#default_value' => variable_get('plus1_comment_undo_vote', 0),
  );

  $form['plus1_comment_widget_settings']['plus1_comment_undo_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Feedback provided to voter when he already voted and user CAN undo his vote"),
    '#default_value' => variable_get('plus1_comment_undo_vote_text', ''),
    '#description' => t('Enter, for example, <em>Undugg</em>, <em>Undo vote</em>, or <strong>leave it blank to provide user with "arrow down" icon.</strong>'),
    '#states' => array(
      'visible' => array(
        'input[name="plus1_comment_undo_vote"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['plus1_comment_widget_settings']['plus1_comment_widget_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight of comment voting widget'),
    '#default_value' => variable_get('plus1_comment_widget_weight', '100'),
    '#description' => t('The higher the weight the lower in the comment the widget will be added. Can be a negative number.'),
  );

  $form['plus1_taxonomy_term_widget_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Taxonomy term widget settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['plus1_taxonomy_term_widget_settings']['plus1_taxonomy_term_widget_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Add Plus 1 voting widget to taxonomy terms'),
    '#default_value' => variable_get('plus1_taxonomy_term_widget_show', 0),
  );

  $vocabularies = array();
  foreach (taxonomy_get_vocabularies() as $voc) {
    $vocabularies[$voc->vid] = $voc->name;
  }

  $form['plus1_taxonomy_term_widget_settings']['plus1_taxonomy_vocabularies'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Add voting widget to following vocabularies'),
    '#options' => $vocabularies,
    '#default_value' => variable_get('plus1_taxonomy_vocabularies', array())
  );

  $form['plus1_taxonomy_term_widget_settings']['plus1_taxonomy_term_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Vote text"),
    '#default_value' => variable_get('plus1_taxonomy_term_vote_text', t('Vote')),
    '#description' => t('Enter, for example, <em>Vote</em> or <em>Digg</em>.'),
  );

  $form['plus1_taxonomy_term_widget_settings']['plus1_taxonomy_term_voted_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Feedback provided to voter when he already voted"),
    '#default_value' => variable_get('plus1_taxonomy_term_voted_text', t('You voted')),
    '#description' => t('Enter, for example, <em>Dugg</em>, <em>You voted</em>, or <em>Voted</em>.'),
  );

  $form['plus1_taxonomy_term_widget_settings']['plus1_taxonomy_term_undo_vote'] = array(
    '#type' => 'checkbox',
    '#title' => t("Allow users to undo their taxonomy term votes"),
    '#default_value' => variable_get('plus1_taxonomy_term_undo_vote', 0),
  );

  $form['plus1_taxonomy_term_widget_settings']['plus1_taxonomy_term_undo_vote_text'] = array(
    '#type' => 'textfield',
    '#title' => t("Feedback provided to voter when he already voted and user CAN undo his vote"),
    '#default_value' => variable_get('plus1_taxonomy_term_undo_vote_text', ''),
    '#description' => t('Enter, for example, <em>Undugg</em>, <em>Undo vote</em>, or <strong>leave it blank to provide user with "arrow down" icon.</strong>'),
    '#states' => array(
      'visible' => array(
        'input[name="plus1_taxonomy_term_undo_vote"]' => array('checked' => TRUE),
      ),
    ),
  );

  $form['plus1_taxonomy_term_widget_settings']['plus1_taxonomy_term_widget_weight'] = array(
    '#type' => 'textfield',
    '#title' => t('Weight of taxonomy term voting widget'),
    '#default_value' => variable_get('plus1_taxonomy_term_widget_weight', '100'),
    '#description' => t('The higher the weight the lower in the comment the widget will be added. Can be a negative number.'),
  );


  $form['array_filter'] = array('#type' => 'hidden');

  return system_settings_form($form);
}
