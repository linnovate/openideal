<?php

namespace Drupal\openideal_idea\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\openideal_idea\OpenidealHelper;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Openideal score configuration form.
 */
class ScoreConfigForm extends ConfigFormBase {

  /**
   * Entity field manager.
   *
   * @var \Drupal\Core\Entity\EntityFieldManagerInterface
   */
  protected $entityFieldManager;

  /**
   * Fields.
   *
   * @var array
   */
  protected $fields;

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container) {
    $instance = parent::create($container);

    $instance->entityFieldManager = $container->get('entity_field.manager');
    return $instance;
  }

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

    $fields = $this->entityFieldManager->getFieldDefinitions('node', 'idea');

    $form['weights'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Fivestars widgets'),
      '#description' => $this->t('Configure weights of fivestar widgets'),
    ];

    foreach ($fields as $name => $field_definition) {
      if (OpenidealHelper::isVotingAPIField($field_definition)) {
        $weight = $field_definition->getThirdPartySetting('openideal_idea', 'weight');
        $this->fields['weight' . $name] = [
          'weight' => $weight,
          'field_definition' => $field_definition,
        ];
        $form['weights']['weight' . $name] = [
          '#title' => $field_definition->getLabel(),
          '#required' => TRUE,
          '#min' => 0,
          '#type' => 'number',
          '#default_value' => $weight ?? 0,
        ];
      }
    }

    return parent::buildForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $values = $form_state->cleanValues()->getValues();

    // Save configuration only in case if field was changed.
    foreach ($values as $name => $value) {
      if (strpos($name, 'weight') === 0 && $this->fields[$name]['weight'] !== $value) {
        $this->fields[$name]['field_definition']->setThirdPartySetting('openideal_idea', 'weight', $value);
        $this->fields[$name]['field_definition']->save();
      }
    }

    $this->config('openideal_idea.scoreconfig')
      ->set('comments_value', $form_state->getValue('comments_value'))
      ->set('votes_value', $form_state->getValue('votes_value'))
      ->set('node_value', $form_state->getValue('node'))
      ->save();

    parent::submitForm($form, $form_state);
  }

}
