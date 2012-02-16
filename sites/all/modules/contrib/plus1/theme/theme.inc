<?php

/**
 * @file
 * Theme file, contains implementations of theme functions and preprocessors.
 */

/**
 * Preprocess function for widget template file. Provides variables for default widget.
 * @param  $variables
 * @return void
 */
function template_preprocess_plus1_widget(&$variables) {
  $entity_type = $variables['entity_type'];
  $entity_id = $variables['entity_id'];
  $tag = $variables['tag'];
  $voted = $variables['voted'];
  $logged_in = $variables['logged_in'];
  $score = $variables['score'];
  $vote_link = $variables['vote_link'];
  $undo_vote_link = $variables['undo_vote_link'];
  $link_query = $variables['link_query'];
  $can_vote = $variables['can_vote'];
  $can_undo_vote = $variables['can_undo_vote'];
  $vote_text = $variables['vote_text'];
  $undo_vote_text = $variables['undo_vote_text'];
  $voted_text = $variables['voted_text'];

  if (!$logged_in && !$can_vote) {
    $variables['widget_message'] =  l(t('Log in<br />to vote'), 'user', array('html' => TRUE));
  }
  elseif ($variables['voted']) { // User already voted.
    // is the user can undo his vote then provide him with link
    if ($can_undo_vote) {
      if ($undo_vote_text != "") {
        $variables['widget_message'] = l($undo_vote_text, $undo_vote_link, array('query' => $link_query, 'attributes' => array('class' => array('plus1-link'))));
      }
      else {
        // if we don't have text for undo action, add "arrow-down" link after score.
        $variables['use_arrow_down'] = TRUE;
        $variables['widget_message'] = l(t('Undo vote'), $undo_vote_link, array('query' => $link_query, 'attributes' => array('class' => array('plus1-link'))));
      }
    }
    else {
      $variables['widget_message'] = $voted_text;
    }
  }
  elseif ($can_vote) {
    // User is eligible to vote.
    $variables['widget_message'] = l($vote_text, $vote_link, array('query' => $link_query, 'attributes' => array('class' => array('plus1-link'))));
  }
}

/**
 * Theming function for json response.
 * @param  $variables
 * @return array
 *
 * Returns an array of variables which will be send back to browser, after ajax request.
 * @see plus1_vote(), plus1_undo_vote()
 */
function theme_plus1_json_response($variables) {
  switch ($variables['entity_type']) {
    case 'node':
      return array('widget' => drupal_render(plus1_build_node_jquery_widget($variables['entity_id'], $variables['tag'])));
      break;
    case 'taxonomy_term':
      return array('widget' => drupal_render(plus1_build_taxonomy_term_jquery_widget($variables['entity_id'], $variables['tag'])));
      break;
    case 'comment':
      return array('widget' => drupal_render(plus1_build_comment_jquery_widget($variables['entity_id'], $variables['tag'])));
      break;
  }
}
