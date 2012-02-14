<?php
// $Id: taxonomy.rules_forms.inc,v 1.1.2.10 2009/09/10 10:57:54 fago Exp $


/**
 * @file Rules configuration forms for the taxonomy module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Action: Load a term configuration form.
 *
 * Multistep form.
 */
function rules_action_taxonomy_load_term_form($settings, &$form, $form_state) {
  $settings += array('vocabulary' => 0);
  if (empty($settings['vocabulary'])) {
    // Get existing taxonomy vocabularies.
    $vocab = $options = array();
    $options = _rules_action_taxonomy_get_vocab();
    $form['settings']['vocabulary'] = array(
      '#type' => 'select',
      '#title' => t('Vocabulary'),
      '#options' => $options,
      '#description' => !empty($options) ? t('Select the vocabulary.') : t('There are no existing vocabularies, you should <a href="@add-url">add</a> one.', array('@add-url' => url('/admin/content/taxonomy/add/vocabulary'))),
      '#required' => TRUE,
      '#disabled' => empty($options),
    );

    // Hide some form elements in the first step.
    $form['new']['#access'] = FALSE;
    $form['input_help']['#access'] = FALSE;
    $form['weight']['#access'] = FALSE;

    // Replace the usual submit handlers with our own handler.
    $form['submit']['#submit'] = array('rules_action_taxonomy_load_term_form_step_submit');
    $form['submit']['#value'] = t('Continue');
  }
  else {
    $options = array();
    $vocabulary = taxonomy_vocabulary_load($settings['vocabulary']);
    $terms = taxonomy_get_tree($vocabulary->vid);
    foreach ($terms as $term) {
      $options[$term->tid] = check_plain($term->name);
    }
    $form['settings']['term'] = array(
      '#type' => 'fieldset',
      '#title' => t("Select a term"),
      '#description' => empty($options) ? t('There are no terms in the vocabulary, you should <a href="@add-url">add</a> one.', array('@add-url' => url('admin/content/taxonomy/'. $vocabulary->vid .'/add/term'))) : t('Select an existing term from vocabulary !vocab or manually enter the name of the term that should be added or removed from the content.', array('!vocab' => $vocabulary->name)),
    );
    $form['settings']['term']['term_select'] = rules_taxonomy_form($vocabulary->vid, !empty($settings['term']['term_select']) ? $settings['term']['term_select'] : 0);

    $form['settings']['term']['term_text'] = array(
      '#type' => 'textarea',
      '#title' => t('Select by term id'),
      '#default_value' => !empty($settings['term']['term_text']) ? $settings['term']['term_text'] : '',
      '#disabled' => empty($options),
      '#description' => t('Optional: enter the term id (<em>not the term name</em>) that should be loaded . If this field is used "Select a term" field will be ignored.'),
    );
  }
}

function rules_action_taxonomy_load_term_form_step_submit($form, &$form_state) {
  $form_state['element']['#settings']['vocabulary'] = $form_state['values']['settings']['vocabulary'];
}

/**
 * Own version of taxonomy_form(), which forces the form to be not multiple.
 */
function rules_taxonomy_form($vid, $value = 0, $help = NULL, $name = 'taxonomy') {
  $vocabulary = taxonomy_vocabulary_load($vid);
  $help = ($help) ? $help : $vocabulary->help;
  return _taxonomy_term_select(check_plain($vocabulary->name), $name, $value, $vid, $help, FALSE, t('- None selected -'));
}

/**
 * Action: Add a new term to vocabulary configuration form.
 *
 * As we allow adding terms to vocabularies that are created on the fly, we
 * can't present the term's advanced settings.
 */
function rules_action_taxonomy_add_term_form($settings, &$form, $form_state) {
  // Fields definition taken from taxonomy_form_term().
  $form['settings']['term'] = array(
    '#type' => 'fieldset',
    '#title' => t('Term Identification'),
    '#collapsible' => TRUE,
  );
  $form['settings']['term']['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Term name'),
    '#default_value' => !empty($settings['term']['name']) ? $settings['term']['name'] : '',
    '#maxlength' => 255,
    '#description' => t('The name of this term.'),
    '#required' => TRUE,
  );

  $form['settings']['term']['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#default_value' => !empty($settings['term']['description']) ? $settings['term']['description'] : '',
    '#description' => t('A description of the term. To be displayed on taxonomy/term pages and RSS feeds.'),
  );
}

/**
 * Action: Load a vocabulary configuration form.
 */
function rules_action_taxonomy_load_vocab_form($settings, &$form, $form_state) {
  $form['settings']['vocabulary'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vocabulary selection'),
  );
  $options = _rules_action_taxonomy_get_vocab();
  $form['settings']['vocabulary']['vocab_select'] = array(
    '#type' => 'select',
    '#title' => t('Select a vocabulary'),
    '#options' => $options,
    '#default_value' => !empty($settings['vocabulary']['vocab_select']) ? $settings['vocabulary']['vocab_select'] : '',
    '#description' => !empty($options) ? t('Select the vocabulary.') : t('There are no existing vocabularies, you should <a href="@add-url">add</a> one.', array('@add-url' => url('admin/content/taxonomy/add/vocabulary'))),
    '#required' => TRUE,
    '#disabled' => empty($options),
  );
  $form['settings']['vocabulary']['vocab_text'] = array(
    '#type' => 'textarea',
    '#title' => t('Select by vocabulary id'),
    '#default_value' => !empty($settings['vocabulary']['vocab_text']) ? $settings['vocabulary']['vocab_text'] : '',
    '#disabled' => empty($options),
    '#description' => t('Optional: Enter the vocabulary id (<em>not the vocabulary name</em>) that should be loaded. If this field is used, the "Select a vocabulary" field will be ignored.'),
  );
}

/**
 * Action: Create a new vocabulary configuration form.
 *
 * @see rules_action_taxonomy_add_vocab_submit().
 */
function rules_action_taxonomy_add_vocab_form($settings, &$form, $form_state) {
  module_load_include('inc', 'taxonomy', 'taxonomy.admin');
  $form['settings']['vocab'] = array(
    '#type' => 'fieldset',
    '#title' => t('Vocabulary settings'),
  );

  $form['settings']['vocab']['vocab'] = taxonomy_form_vocabulary($form_state, !empty($settings) ? $settings : array());
  // Remove the 'Save' button.
  unset($form['settings']['vocab']['vocab']['submit']);
}

/**
 * Action: Create a new vocabulary submit handler.
 *
 * Flatten the vocabulary settings so they can be passed to
 * taxonomy_form_vocabulary().
 */
function rules_action_taxonomy_add_vocab_submit(&$settings, $form, $form_state) {
  $vocab = array();
  if (!empty($settings['vocab'])) {
    foreach ($settings['vocab']['vocab'] as $key => $values) {
      if (is_array($values)) {
        if (!empty($values['nodes'])) {
          // User array_filter for the nodes, otherwise all content types will
          // be chosen.
          $values['nodes'] = array_filter($values['nodes']);
        }
        $vocab += $values;
      }
      else {
        $vocab[$key] = $values;
      }
    }
  }
  $settings = $vocab;
}

/**
 * Helper function; Return all existing vocabularies.
 */
function _rules_action_taxonomy_get_vocab() {
  $vocabs = taxonomy_get_vocabularies();
  $options = array();
  foreach ($vocabs as $vocab) {
    $options[$vocab->vid] = check_plain($vocab->name);
  }
  return $options;
}

/**
 * @}
 */
