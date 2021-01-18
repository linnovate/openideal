<?php

namespace Drupal\openideal_idea\Form;

use Drupal\Component\Utility\Html;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Url;
use Drupal\field\Entity\FieldConfig;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Configure OpenideaL Idea settings for this site.
 */
class OpenidealIdeaEditFiveStarsForm extends FormBase {

  protected const ENTITY_TYPE = 'node';

  protected const FIELD_TYPE = 'voting_api_field';

  protected const BUNDLE = 'idea';

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityManager;

  /**
   * Entity Field Manager.
   *
   * @var \Drupal\Core\Entity\EntityFieldManager
   */
  protected $entityFieldManager;

  /**
   * Five stars fields.
   *
   * @var array
   */
  private $fields;

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container) {
    $instance = parent::create($container);
    $instance->entityManager = $container->get('entity_type.manager');
    $instance->entityFieldManager = $container->get('entity_field.manager');

    return $instance;
  }

  /**
   * {@inheritdoc}
   */
  public function getFormId() {
    return 'openideal_idea_openideal_idea_edit_five_stars';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $fields = $this->entityFieldManager->getFieldDefinitions('node', 'idea');
    $form['#id'] = Html::getClass($this->getFormId());
    $form['#tree'] = TRUE;
    $form['fields'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Widgets'),
    ];
    foreach ($fields as $field_definition) {
      if ($field_definition instanceof FieldConfig && $field_definition->getType() == 'voting_api_field') {
        $this->fields[] = $field_definition;
        $id = $field_definition->getUniqueIdentifier();
        $weight = $field_definition->getThirdPartySetting('openideal_idea', 'weight');
        $label = $field_definition->label();
        $form['fields'][$id] = [
          'label' => [
            '#required' => TRUE,
            '#type' => 'textfield',
            '#title' => $this->t('Label'),
            '#default_value' => $label,
          ],
          'weight' => [
            '#required' => TRUE,
            '#min' => 0,
            '#type' => 'number',
            '#default_value' => $weight ?? 0,
          ],
          'delete' => [
            '#type' => 'link',
            '#url' => Url::fromRoute(
              'entity.field_config.node_field_delete_form',
              ['node_type' => 'idea', 'field_config' => $field_definition->id()],
              [
                'query' => ['destination' => Url::fromRoute('openideal_idea.openideal_idea_edit_five_stars')->toString()],
              ]
            ),
            '#title' => $this->t('Delete'),
            '#attributes' => ['class' => ['button']],
          ],
        ];
      }
    }

    $form['add'] = [
      '#type' => 'link',
      '#title' => $this->t('Add new widget'),
      '#url' => Url::fromRoute('openideal_idea.openideal_idea_add_five_stars'),
      '#attributes' => ['class' => ['button']],
    ];

    $form['submit'] = [
      '#type' => 'submit',
      '#value' => $this->t('Save'),
    ];

    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $fields = $this->fields ?: $this->entityFieldManager->getFieldDefinitions('node', 'idea');
    $values = $form_state->getValues();

    /** @var \Drupal\Core\Field\FieldDefinition $field_definition */
    foreach ($fields as $field_definition) {
      foreach ($values['fields'] as $id => $field_configuration) {
        if ($field_definition instanceof FieldConfig
          && $field_definition->getType() == 'voting_api_field'
          && $field_definition->getUniqueIdentifier() == $id
          && ($field_definition->label() != $field_configuration['label'] || $field_configuration['weight'] != $field_definition->getThirdPartySetting('openideal_idea', 'weight'))
        ) {
          $field_definition->setLabel($field_configuration['label']);
          $field_definition->setThirdPartySetting('openideal_idea', 'weight', $field_configuration['weight']);
          $field_definition->save();
        }
      }
    }
  }

}
