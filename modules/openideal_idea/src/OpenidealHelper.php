<?php

namespace Drupal\openideal_idea;

use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Extension\ModuleHandler;
use Drupal\Core\Session\AccountInterface;
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
   * OpenidealHelper construct.
   *
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   Entity type manager.
   * @param \Drupal\group\GroupMembershipLoader $group_membership_loader
   *   Group membership loader.
   * @param \Drupal\Core\Config\ConfigFactory $configFactory
   *   Config factory.
   * @param \Drupal\Core\Extension\ModuleHandler $moduleHandler
   *   Module handler.
   */
  public function __construct(
    EntityTypeManagerInterface $entity_type_manager,
    GroupMembershipLoader $group_membership_loader,
    ConfigFactory $configFactory,
    ModuleHandler $moduleHandler
  ) {
    $this->entityTypeManager = $entity_type_manager;
    $this->groupMembershipLoader = $group_membership_loader;
    $this->configFactory = $configFactory;
    $this->moduleHandler = $moduleHandler;
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
   * @param string|int $id
   *   Node id.
   *
   * @return float|int
   *   Score.
   */
  public function computeOverallScore($id) {
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
    if ($this->moduleHandler->moduleExists('statistics')) {
      $statistics_result = $this->nodeStatisticsDatabase->fetchView($id);
      if ($statistics_result) {
        $node_counter_value = $statistics_result->getTotalCount() * ($configuration->get('node_value') ?? 0.2);
      }
    }

    return $comments * ($configuration->get('comments_value') ?? 10)
      + $votes * ($configuration->get('votes_value') ?? 5)
      + $node_counter_value;
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
