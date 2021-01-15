<?php

namespace Drupal\openideal_idea\Form;

use Drupal\Component\Utility\Html;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\field\Entity\FieldStorageConfig;
use Exception;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Configure OpenideaL Idea settings for this site.
 */
class OpenidealIdeaAddFiveStarsForm extends FormBase {

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
    return 'openideal_idea_openideal_idea_add_five_stars_widget';
  }

  /**
   * {@inheritdoc}
   */
  public function buildForm(array $form, FormStateInterface $form_state) {
    $form['#id'] = Html::getClass($this->getFormId());
    $field_prefix = $this->config('field_ui.settings')->get('field_prefix');
    $form['#tree'] = TRUE;
    $form['field'] = [
      '#type' => 'fieldset',
      '#title' => $this->t('Widget'),
      'label' => [
        '#required' => TRUE,
        '#type' => 'textfield',
        '#title' => $this->t('Label'),
        '#default_value' => '',
      ],
      'weight' => [
        '#title' => $this->t('Weight'),
        '#description' => $this->t('The widget weight directly affect overall idea score'),
        '#type' => 'textfield',
        '#default_value' => 0,
      ],
      'field_name' => [
        '#type' => 'machine_name',
        // This field should stay LTR even for RTL languages.
        '#field_prefix' => '<span dir="ltr">' . $field_prefix,
        '#field_suffix' => '</span>&lrm;',
        '#size' => 15,
        '#description' => $this->t('A unique machine-readable name containing letters, numbers, and underscores.'),
        // Calculate characters depending on the length of the field prefix
        // setting. Maximum length is 32.
        '#maxlength' => FieldStorageConfig::NAME_MAX_LENGTH - strlen($field_prefix),
        '#machine_name' => [
          'source' => ['field', 'label'],
          'exists' => [$this, 'fieldNameExists'],
        ],
        '#required' => FALSE,
      ],
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
  public function validateForm(array &$form, FormStateInterface $form_state) {
    parent::validateForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $values = $form_state->getValue('field');
    $field_name = $this->configFactory()->get('field_ui.settings')->get('field_prefix') . $values['field_name'];
    $field_storage_values = [
      'field_name' => $field_name,
      'entity_type' => self::ENTITY_TYPE,
      'type' => self::FIELD_TYPE,
      'translatable' => TRUE,
      'settings' =>
        [
          'vote_type' => 'vote',
          'vote_plugin' => 'oi_fivestar',
        ],
      'cardinality' => '1',
      'cardinality_number' => '1',
    ];
    $field_values = [
      'description' => '',
      'required' => 0,
      'settings' =>
        [
          'anonymous_window' => '0',
          'user_window' => '0',
        ],
      'field_name' => $field_name,
      'entity_type' => self::ENTITY_TYPE,
      'bundle' => self::BUNDLE,
      'label' => $values['label'],
      // Field translatability should be explicitly enabled by the users.
      'translatable' => FALSE,
    ];

    // Create the field storage and field.
    try {
      $this->entityManager->getStorage('field_storage_config')->create($field_storage_values)->save();
      $field = $this->entityManager->getStorage('field_config')->create($field_values);
      $field->setThirdPartySetting('openideal_idea', 'weight', $values['weight']);
      $field->save();
    } catch (Exception $e) {
      $this->messenger()->addError($this->t('There was a problem creating field %label: @message', ['%label' => $values['label'], '@message' => $e->getMessage()]));
      return;
    }

    // Helps to five stars work correctly.
    // @todo Make further investigation.
    drupal_flush_all_caches();

    $form_state->setRedirect('openideal_idea.openideal_idea_edit_five_stars');
  }

  /**
   * Checks if a field machine name is taken.
   *
   * @param string $value
   *   The machine name, not prefixed.*
   *
   * @return bool
   *   Whether or not the field machine name is taken.
   */
  public function fieldNameExists($value) {
    // Add the field prefix.
    $field_name = $this->configFactory->get('field_ui.settings')->get('field_prefix') . $value;

    $field_storage_definitions = $this->entityFieldManager->getFieldStorageDefinitions(self::ENTITY_TYPE);
    return isset($field_storage_definitions[$field_name]);
  }

}
