<?php

namespace Drupal\openideal_user\Plugin\Condition;

use Drupal\comment\Entity\Comment;
use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\node\Entity\Node;
use Drupal\rules\Core\RulesConditionBase;
use Drupal\votingapi\VoteInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides an 'Is voted entity published' condition.
 *
 * @Condition(
 *   id = "openideal_voted_entity_is_published",
 *   label = @Translation("Voted entity is published"),
 *   category = @Translation("Vote"),
 *   context_definitions = {
 *     "entity" = @ContextDefinition("entity:vote",
 *       label = @Translation("Vote entity"),
 *       description = @Translation("Specifies the entity for which to evaluate the condition."),
 *       assignment_restriction = "selector"
 *     ),
 *   }
 * )
 */
class VoteEntityPublished extends RulesConditionBase implements ContainerFactoryPluginInterface {

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManager
   */
  protected $entityTypeManager;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, EntityTypeManager $entityTypeManager) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entityTypeManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('entity_type.manager'),
    );
  }

  /**
   * Check if the voted entity is Published.
   *
   * @param \Drupal\votingapi\VoteInterface $vote
   *   The entity to check.
   *
   * @return bool
   *   TRUE if user voted today, false otherwise.
   */
  protected function doEvaluate(VoteInterface $vote) {
    $entity = $this->entityTypeManager->getStorage($vote->getVotedEntityType())->load($vote->getVotedEntityId());
    if ($entity instanceof Comment) {
      return $entity->getCommentedEntity()->isPublished();
    }
    elseif ($entity instanceof Node) {
      return $entity->isPublished();
    }
    return FALSE;
  }

}
