<?php

namespace Drupal\openideal_discussion\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Entity\EntityFieldManager;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Routing\CurrentRouteMatch;
use Drupal\openideal_statistics\OpenidealStatisticsFivestarsTrait;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides experts voting block for discussion page.
 *
 * @Block(
 *   id = "openideal_discussion_experts_voting",
 *   admin_label = @Translation("Experts voting widget")
 * )
 */
class OpenidealDiscussionExpertsVoting extends BlockBase implements ContainerFactoryPluginInterface {

  use OpenidealStatisticsFivestarsTrait;

  /**
   * Route match.
   *
   * @var \Drupal\Core\Routing\CurrentRouteMatch
   */
  protected $routeMatch;

  /**
   * Entity field manager.
   *
   * @var \Drupal\Core\Entity\EntityFieldManager
   */
  protected $fieldManager;

  /**
   * {@inheritDoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, CurrentRouteMatch $routeMatch, EntityFieldManager $fieldManager) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->routeMatch = $routeMatch;
    $this->fieldManager = $fieldManager;
  }

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('current_route_match'),
      $container->get('entity_field.manager')
    );
  }

  /**
   * {@inheritDoc}
   */
  public function build() {
    $build = [];
    /** @var \Drupal\node\NodeInterface $node */
    if (($node = $this->routeMatch->getParameter('node')) && $node->bundle() === 'discussion' && !$node->get('field_idea')->isEmpty()) {
      $build['#cache']['contexts'] = ['user.roles'];
      $build['#cache']['tags'] = $node->getCacheTags();
      /** @var \Drupal\node\Entity\Node $idea */
      $idea = $node->get('field_idea')->entity;

      $build['content'] = $this->viewFivestars($idea);
    }
    return $build;
  }

}
