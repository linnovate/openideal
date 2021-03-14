<?php

namespace Drupal\openideal_discussion\Plugin\Condition;

use Drupal\content_moderation\ModerationInformation;
use Drupal\Core\Condition\ConditionPluginBase;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Entity in Moderation State' condition.
 *
 * @Condition(
 *   id = "openideal_discussion_entity_in_state",
 *   label = @Translation("Entity in Moderation state"),
 *   context_definitions = {
 *     "node" = @ContextDefinition("entity:node", label = @Translation("Node"))
 *   }
 * )
 */
class EntityInModerationState extends ConditionPluginBase implements ContainerFactoryPluginInterface {

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Moderation information service.
   *
   * @var \Drupal\content_moderation\ModerationInformation
   */
  protected $moderationInformation;

  /**
   * Constructs a Route condition plugin.
   *
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entityTypeManager
   *   Entity type manager.
   * @param \Drupal\content_moderation\ModerationInformation $moderationInformation
   *   Moderation information service.
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param array $plugin_definition
   *   The plugin implementation definition.
   */
  public function __construct(EntityTypeManagerInterface $entityTypeManager, ModerationInformation $moderationInformation, array $configuration, $plugin_id, array $plugin_definition) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->moderationInformation = $moderationInformation;
    $this->entityTypeManager = $entityTypeManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $container->get('entity_type.manager'),
      $container->get('content_moderation.moderation_information'),
      $configuration,
      $plugin_id,
      $plugin_definition
    );
  }

  /**
   * {@inheritdoc}
   */
  public function buildConfigurationForm(array $form, FormStateInterface $form_state) {
    $options = [];
    /** @var \Drupal\content_moderation\Plugin\WorkflowType\ContentModeration $plugin */
    $plugin = $this->entityTypeManager->getStorage('workflow')->load('life_cycle_phases')->getTypePlugin();

    foreach ($plugin->getStates() as $state) {
      $options[$state->id()] = $state->label();
    }

    $form['states'] = [
      '#title' => $this->t('Node moderation states'),
      '#type' => 'checkboxes',
      '#options' => $options,
      '#default_value' => $this->configuration['states'],
    ];
    return parent::buildConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function submitConfigurationForm(array &$form, FormStateInterface $form_state) {
    $this->configuration['states'] = array_filter($form_state->getValue('states'));
    parent::submitConfigurationForm($form, $form_state);
  }

  /**
   * {@inheritdoc}
   */
  public function summary() {
    if (count($this->configuration['states']) > 1) {
      $states = $this->configuration['states'];
      $last = array_pop($states);
      $states = implode(', ', $states);
      return $this->t('The node moderation state is @states or @last', [
        '@states' => $states,
        '@last' => $last,
      ]);
    }

    $bundle = reset($this->configuration['states']);
    return $this->t('The node moderation state is @bundle', ['@bundle' => $bundle]);
  }

  /**
   * {@inheritdoc}
   */
  public function evaluate() {
    if (empty($this->configuration['states']) && !$this->isNegated()) {
      return TRUE;
    }
    $node = $this->getContextValue('node');
    // In case if node isn't moderated return false.
    if (!$this->moderationInformation->isModeratedEntity($node)) {
      return FALSE;
    }
    $state = $this->moderationInformation->getOriginalState($node);
    return !empty($this->configuration['states'][$state->id()]);
  }

  /**
   * {@inheritdoc}
   */
  public function defaultConfiguration() {
    return ['states' => []] + parent::defaultConfiguration();
  }

}
