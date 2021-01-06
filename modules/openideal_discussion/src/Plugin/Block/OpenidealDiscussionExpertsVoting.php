<?php

namespace Drupal\openideal_discussion\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Routing\CurrentRouteMatch;
use Drupal\openideal_idea\OpenidealHelper;
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

  /**
   * Route match.
   *
   * @var \Drupal\Core\Routing\CurrentRouteMatch
   */
  protected $routeMatch;

  /**
   * Openideal helper.
   *
   * @var \Drupal\openideal_idea\OpenidealHelper
   */
  protected $openidealHelper;

  /**
   * {@inheritDoc}
   */
  public function __construct(array $configuration, $plugin_id, $plugin_definition, CurrentRouteMatch $routeMatch, OpenidealHelper $openidealHelper) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->routeMatch = $routeMatch;
    $this->openidealHelper = $openidealHelper;
  }

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static($configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('current_route_match'),
      $container->get('openideal_idea.helper')
    );
  }

  /**
   * {@inheritDoc}
   */
  public function build() {
    $build = [];
    /** @var \Drupal\node\NodeInterface $node */
    if (($node = $this->routeMatch->getParameter('node')) && $node->bundle() === 'discussion' && !$node->get('field_idea')->isEmpty()) {
      $build['#cache']['contexts'] = ['user.roles', 'user.permissions'];
      $build['#cache']['tags'] = $node->getCacheTags();
      $idea = $node->get('field_idea')->first()->get('entity')->getTarget()->getValue();
      $view = $idea->get('field_five_stars')->view(['label' => 'hidden', 'settings' => ['show_results' => '1', 'style' => 'fontawesome-stars']]);
      $build['content'] = $view;
    }
    return $build;
  }

}