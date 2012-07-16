<?php

/**
 * Implements hook_form_FORMID_alter()
 */
function plato_form_comment_form_alter(&$form, &$form_state) {
  // Add placeholder to comment textarea
  $form['comment_body'][LANGUAGE_NONE][0]['#attributes']['placeholder'] = t('Write a comment...');
  // Hide label
  $form['comment_body'][LANGUAGE_NONE][0]['#title_display'] = 'invisible';
  // Modify label for logged-in
  if ($form['is_anonymous']['#value'] == FALSE) {
    $form['author']['_author']['#title'] = t('Posting as');
  }
}
