<?php

namespace Drupal\openideal_idea\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;

/**
 * Openideal score configuration form.
 */
class ScoreConfigForm extends ConfigFormBase {

  /**
   * {@inheritdoc}
   */
  protected function getEditableConfigNames() {
    return [
      'openideal_idea.scoreconfig',
    ];
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'openideal_idea_score_config_form';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('openideal_idea.scoreconfig');

    $form['description'] = [
      '#type' => 'html_tag',
      '#tag' => 'h5',
      '#value' => $this->t("These values control how each parameter is considered when calculating the overall score of ideas. The formula is simple: each parameter is multiplied by it's weight and then the values are summed up to give the overall score."),
    ];

    $form['comments_value'] = [
      '#type' => 'number',
      '#title' => $this->t('Comments score weight'),
      '#min' => 0,
      '#step' => 0.1,
      '#default_value' => $config->get('comments_value') ?? 10,
    ];

    $form['votes_value'] = [
      '#type' => 'number',
      '#title' => $this->t('Votes score weight'),
      '#min' => 0,
      '#step' => 0.1,
      '#default_value' => $config->get('votes_value') ?? 5,
    ];

    $form['node'] = [
      '#type' => 'number',
      '#title' => $this->t('Views weight'),
      '#min' => 0.1,
      '#step' => 0.1,
      '#default_value' => $config->get('node') ?? 0.2,
    ];

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    parent::submitForm($form, $form_state);

    $this->config('openideal_idea.scoreconfig')
      ->set('comments_value', $form_state->getValue('comments_value'))
      ->set('votes_value', $form_state->getValue('votes_value'))
      ->set('node_value', $form_state->getValue('node'))
      ->save();
  }

}
