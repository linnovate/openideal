<?php

namespace Drupal\openideal_statistics;

use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Utility\Token;
use Drupal\statistics\NodeStatisticsDatabaseStorage;

/**
 * LazyBuilder object.
 */
class OpenidealStatisticsLazyBuilder {

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Token service.
   *
   * @var \Drupal\Core\Utility\Token
   */
  protected $token;

  /**
   * Node statistics service.
   *
   * @var \Drupal\statistics\NodeStatisticsDatabaseStorage
   */
  protected $nodeStatistics;

  /**
   * LazyBuilder constructor.
   *
   * @param \Drupal\Core\Entity\EntityTypeManagerInterface $entity_type_manager
   *   Entity type manager.
   * @param \Drupal\Core\Utility\Token $token
   *   Token service.
   * @param \Drupal\statistics\NodeStatisticsDatabaseStorage $node_statistics_database_storage
   *   Node statistics service.
   */
  public function __construct(EntityTypeManagerInterface $entity_type_manager, Token $token, NodeStatisticsDatabaseStorage $node_statistics_database_storage) {
    $this->entityTypeManager = $entity_type_manager;
    $this->token = $token;
    $this->nodeStatistics = $node_statistics_database_storage;
  }

  /**
   * Build element that return idea count.
   *
   * @return array
   *   Renderable array.
   */
  public function getIdeas() {
    return [
      '#markup' => $this->token->replace('[openideal:ideas-count]'),
      '#cache' => [
        'tags' => ['node_list:idea'],
      ],
    ];
  }

  /**
   * Build element that return members count.
   *
   * @return array
   *   Renderable array.
   */
  public function getMembers() {
    return [
      '#markup' => $this->token->replace('[openideal:members-count]'),
      '#cache' => [
        'tags' => ['user_list'],
      ],
    ];
  }

  /**
   * Build element that return comments count.
   *
   * @param int $id
   *   Node id.
   *
   * @return array
   *   Renderable array.
   */
  public function getComments($id = NULL) {
    $node = $id ? $this->entityTypeManager->getStorage('node')->load($id) : NULL;
    $markup = $node
      ? $this->token->replace('[node:comment-count]', ['node' => $node])
      : $this->token->replace('[openideal:comments-count]');
    return [
      '#markup' => $markup,
      '#cache' => [
        'tags' => $node ? $node->getCacheTags() : ['comment_list'],
      ],
    ];
  }

  /**
   * Get the node count of the views.
   *
   * @param int $id
   *   The node id.
   *
   * @return array
   *   Renderable array.
   */
  public function getViews($id) {
    $node = $this->entityTypeManager->getStorage('node')->load($id);
    $statistics_result = $this->nodeStatistics->fetchView($node->id());
    return [
      // In case the node created for the first time result will be false,
      // so need to check it.
      '#markup' => $statistics_result ? $statistics_result->getTotalCount() : 0,
      '#cache' => ['max-age' => 3600],
    ];
  }

  /**
   * Build element that return votes count.
   *
   * @param int $id
   *   Node id.
   *
   * @return array
   *   Renderable array.
   */
  public function getVotes($id = NULL) {
    $node = $id ? $this->entityTypeManager->getStorage('node')->load($id) : NULL;
    $markup = $node
      ? $this->token->replace('[openideal:node-votes-count]', ['node' => $node])
      : $this->token->replace('[openideal:votes-count]');
    return [
      '#markup' => $markup,
      '#cache' => [
        'tags' => $node ? $node->getCacheTags() : ['vote_list'],
      ],
    ];
  }

  /**
   * Build element that return Challenge ideas count.
   *
   * @param int $id
   *   Node id.
   *
   * @return array
   *   Renderable array.
   */
  public function getChallengeIdeas($id) {
    $markup = $this->token->replace('[openideal:challenge-ideas-count]', ['node_id' => $id]);

    return [
      '#markup' => $markup,
      '#cache' => [
        'tags' => ['node_list:idea'],
      ],
    ];
  }

}
