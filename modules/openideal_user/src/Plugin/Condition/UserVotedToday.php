<?php

namespace Drupal\openideal_user\Plugin\Condition;

use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Component\Datetime\Time;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\rules\Core\RulesConditionBase;
use Drupal\votingapi\VoteInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides an 'User is voted' condition.
 *
 * @Condition(
 *   id = "openideal_user_is_voted",
 *   label = @Translation("User voted today"),
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
class UserVotedToday extends RulesConditionBase implements ContainerFactoryPluginInterface {

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManager
   */
  protected $entityTypeManager;

  /**
   * Time service.
   *
   * @var \Drupal\Component\Datetime\Time
   */
  protected $time;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, EntityTypeManager $entityTypeManager, Time $time) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entityTypeManager;
    $this->time = $time;
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
      $container->get('datetime.time')
    );
  }

  /**
   * Check if the user voted today.
   *
   * @param \Drupal\votingapi\VoteInterface $vote
   *   The entity to check.
   *
   * @return bool
   *   TRUE if user voted today, false otherwise.
   */
  protected function doEvaluate(VoteInterface $vote) {
    // @Todo: Because of the vote is deleted every
    // time and new one is created when user re-voted
    // need to create additional logic to check that user
    // already voted on that entity.
    $storage = $this->entityTypeManager->getStorage('vote');
    $user = $vote->getOwner();
    // Check if user voted.
    $count = $storage->getQuery()
      ->condition('user_id', $user->id(), '=')
      ->condition('timestamp', $this->time->getRequestTime() - 86400, '>')
      ->count()
      ->execute();
    return $count == 1;
  }

}
