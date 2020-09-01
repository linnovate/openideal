<?php

namespace Drupal\openideal_user\Plugin\Condition;

use Drupal\content_moderation\ModerationInformationInterface;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\rules\Core\RulesConditionBase;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides an 'Entity is moderated' condition.
 *
 * @Todo: remove if not needed.
 *
 * @Condition(
 *   id = "openideal_entity_is_moderated",
 *   label = @Translation("Entity is moderated"),
 *   category = @Translation("Entity"),
 *   context_definitions = {
 *     "entity" = @ContextDefinition("entity",
 *       label = @Translation("Entity"),
 *       description = @Translation("Specifies the entity for which to evaluate the condition."),
 *       assignment_restriction = "selector"
 *     ),
 *   }
 * )
 */
class EntityIsModerated extends RulesConditionBase implements ContainerFactoryPluginInterface {

  /**
   * The Moderation Information service.
   *
   * @var \Drupal\content_moderation\ModerationInformationInterface
   */
  protected $moderationInfo;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, ModerationInformationInterface $moderation_info) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->moderationInfo = $moderation_info;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('content_moderation.moderation_information')
    );
  }

  /**
   * Check if the provided entity is moderated.
   *
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   The entity to check.
   *
   * @return bool
   *   TRUE if the provided entity is moderated.
   */
  protected function doEvaluate(EntityInterface $entity) {
    return $this->moderationInfo->isModeratedEntity($entity);
  }

}
