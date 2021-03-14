<?php

namespace Drupal\openideal_idea;

use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Database\Connection;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Session\AccountInterface;
use Drupal\field\Entity\FieldConfig;
use Drupal\group\GroupMembershipLoader;
use Drupal\node\NodeInterface;
use Drupal\statistics\NodeStatisticsDatabaseStorage;

/**
 * Provides openideal helper service.
 */
class OpenidealHelper {

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Group member ship loader.
   *
   * @var \Drupal\group\GroupMembershipLoader
   */
  protected $groupMembershipLoader;

  /**
   * Config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory
   */
  protected $configFactory;

  /**
   * Module handler.
   *
   * @var \Drupal\Core\Extension\ModuleHandler
   */
  protected $moduleHandler;

  /**
   * Node statistics service.
   *
   * @var \Drupal\statistics\NodeStatisticsDatabaseStorage
   */
  protected $nodeStatisticsDatabase;

  /**
   * Database connection.
   *
   * @var \Drupal\Core\Database\Connection
   */
  protected $database;

  /**
   * OpenidealHelper construct.
   *
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   Entity type manager.
   * @param \Drupal\group\GroupMembershipLoader $group_membership_loader
   *   Group membership loader.
   * @param \Drupal\Core\Config\ConfigFactory $configFactory
   *   Config factory.
   * @param \Drupal\Core\Database\Connection $database
   *   Database.
   */
  public function __construct(
    EntityTypeManagerInterface $entity_type_manager,
    GroupMembershipLoader $group_membership_loader,
    ConfigFactory $configFactory,
    Connection $database
  ) {
    $this->entityTypeManager = $entity_type_manager;
    $this->groupMembershipLoader = $group_membership_loader;
    $this->configFactory = $configFactory;
    $this->database = $database;
  }

  /**
   * Get the group from "group" module.
   *
   * @param \Drupal\node\NodeInterface $node
   *   Node.
   *
   * @return \Drupal\group\Entity\Group|false
   *   Group or false in case if group couldn't be found.
   */
  public function getGroupFromNode(NodeInterface $node) {
    // Get the group_content - gnode.
    $group_contents = $this->entityTypeManager
      ->getStorage('group_content')
      ->loadByEntity($node);

    if (!empty($group_contents)) {
      // Don't need to check all of group contents,
      // such as they all from one group.
      $group_content = reset($group_contents);
      return $group_content->getGroup();
    }

    return FALSE;
  }

  /**
   * Get the group membership.
   *
   * @param \Drupal\Core\Session\AccountInterface $account
   *   Account to fetch.
   * @param \Drupal\node\NodeInterface $node
   *   Node to check in.
   *
   * @return \Drupal\group\GroupMembership|false
   *   Return group member or false.
   */
  public function getGroupMember(AccountInterface $account, NodeInterface $node) {
    if ($group = $this->getGroupFromNode($node)) {
      return $this->groupMembershipLoader->load($group, $account);
    }
    return FALSE;
  }

  /**
   * Compute overall score.
   *
   * @param \Drupal\node\NodeInterface $idea
   *   Node id.
   *
   * @return float|int
   *   Score.
   */
  public function computeOverallScore(NodeInterface $idea) {
    $id = $idea->id();
    $configuration = $this->configFactory->get('openideal_idea.scoreconfig');
    // Get node comments.
    $comments = $this->entityTypeManager->getStorage('comment')->getQuery()
      ->condition('entity_id', $id)
      ->condition('entity_type', 'node')
      ->count()
      ->execute();

    // Get node votes.
    $votes = $this->entityTypeManager->getStorage('vote')->getQuery()
      ->condition('entity_id', $id)
      ->condition('entity_type', 'node')
      ->count()
      ->execute();

    // Compute the score.
    $node_counter_value = 0;

    // If statistics module is enabled then add node view count to score.
    if ($this->nodeStatisticsDatabase) {
      $statistics_result = $this->nodeStatisticsDatabase->fetchView($id);
      if ($statistics_result) {
        $node_counter_value = $statistics_result->getTotalCount() * ($configuration->get('node_value') ?? 0.2);
      }
    }

    $five_stars = $this->retrieveFivestarsScore($idea);

    return $five_stars + $comments * ($configuration->get('comments_value') ?? 10)
      + $votes * ($configuration->get('votes_value') ?? 5)
      + $node_counter_value;
  }

  /**
   * Retrieve voting api results for an idea.
   *
   * Formula: (average_score - 3) * weight.
   *
   * @param \Drupal\node\NodeInterface $idea
   *   Idea.
   *
   * @return int
   *   Results.
   */
  private function retrieveFivestarsScore(NodeInterface $idea) {
    $fields = $idea->getFieldDefinitions();
    $query = $this->database->select('votingapi_result', 'v')
      ->fields('v', ['function', 'value'])
      ->condition('entity_type', $idea->getEntityTypeId())
      ->condition('type', 'vote')
      ->condition('entity_id', $idea->id());
    $condition = $query->orConditionGroup();
    $weights = [];
    foreach ($fields as $field_name => $field_definition) {
      if (static::isVotingAPIField($field_definition)) {
        $weight = (int) $idea->get($field_name)->getFieldDefinition()->getThirdPartySetting('openideal_idea', 'weight');
        $condition->condition('function', 'vote_field_average:node.' . $field_name);
        $weights['vote_field_average:node.' . $field_name] = $weight;
      }
    }
    $scores = $query
      ->condition($condition)
      ->execute()
      ->fetchAllKeyed(1, 0);

    $fivestars_score = 0;
    array_walk($scores,
      function ($function, $value) use ($weights, &$fivestars_score) {
        $fivestars_score += ((int) $value - 3) * $weights[$function];
      }
    );

    return $fivestars_score;
  }

  /**
   * Checks if the field of voting api type.
   *
   * @param \Drupal\Core\Field\FieldDefinitionInterface $field_definition
   *   Field to check.
   *
   * @return bool
   *   TRUE if it is voting api field.
   */
  // @codingStandardsIgnoreLine
  public static function isVotingAPIField(FieldDefinitionInterface $field_definition) {
    return $field_definition instanceof FieldConfig && $field_definition->getType() == 'voting_api_field';
  }

  /**
   * Set the group membership manager service.
   *
   * @param \Drupal\statistics\NodeStatisticsDatabaseStorage $nodeStatisticsDatabaseStorage
   *   The group membership manager service.
   */
  public function setStatisticsStorage(NodeStatisticsDatabaseStorage $nodeStatisticsDatabaseStorage) {
    $this->nodeStatisticsDatabase = $nodeStatisticsDatabaseStorage;
  }

}
