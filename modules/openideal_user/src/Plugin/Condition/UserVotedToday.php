<?php

namespace Drupal\openideal_user\Plugin\Condition;

use Drupal\Core\Database\Connection;
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
   * Current DB connection.
   *
   * @var \Drupal\Core\Database\Connection
   */
  protected $database;

  /**
   * {@inheritdoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, EntityTypeManager $entityTypeManager, Connection $database, Time $time) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entityTypeManager;
    $this->time = $time;
    $this->database = $database;
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
      $container->get('database'),
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
    $user = $vote->getOwner();
    $select = $this->database->select('message__field_node_reference', 'm');
    $select->condition('m.bundle', 'create_like_on_node');
    $select->join('message_field_data', 'md', 'md.mid = m.entity_id AND md.uid = :id', [':id' => $user->id()]);
    $select->join('node_field_data', 'n', 'n.nid = m.field_node_reference_target_id');
    $select->condition('n.type', $vote->entity_id->entity->bundle());
    $select->condition('m.field_node_reference_target_id', $vote->getVotedEntityId());
    $result = $select->countQuery()
      ->execute()->fetchField();
    return $result == 1;
  }

}
