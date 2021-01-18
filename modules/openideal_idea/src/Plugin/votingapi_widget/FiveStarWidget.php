<?php

namespace Drupal\openideal_idea\Plugin\votingapi_widget;

use Drupal\votingapi_widgets\Plugin\votingapi_widget\FiveStarWidget as BaseFiveStarWidget;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Assigns ownership of a node to a user.
 *
 * Copy of FiveStartWidget but with changed permissions.
 *
 * @VotingApiWidget(
 *   id = "oi_fivestar",
 *   label = @Translation("OpenideaL Fivestar rating"),
 *   values = {
 *    1 = @Translation("Poor"),
 *    2 = @Translation("Not so poor"),
 *    3 = @Translation("average"),
 *    4 = @Translation("good"),
 *    5 = @Translation("very good"),
 *   },
 * )
 */
class FiveStarWidget extends BaseFiveStarWidget {

  /**
   * Route match.
   *
   * @var \Drupal\Core\Routing\RouteMatch
   */
  protected $routeMatch;

  /**
   * Permission checker.
   *
   * @var \Drupal\group\Access\GroupPermissionChecker
   */
  protected $permissionChecker;

  /**
   * OpenideaL helper.
   *
   * @var \Drupal\openideal_idea\OpenidealHelper
   */
  protected $helper;

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    $instance = parent::create($container, $configuration, $plugin_id, $plugin_definition);
    $instance->routeMatch = $container->get('current_route_match');
    $instance->permissionChecker = $container->get('group_permission.checker');
    $instance->helper = $container->get('openideal_idea.helper');
    return $instance;
  }

  /**
   * {@inheritDoc}
   */
  public function canVote($vote, $account = FALSE) {
    /** @var \Drupal\node\NodeInterface $node */
    if (($node = $this->routeMatch->getParameter('node')) && $node->bundle() == 'discussion' && ($group = $this->helper->getGroupFromNode($node))) {
      return $this->permissionChecker->hasPermissionInGroup('vote on five stars', $account ?: $this->account, $group);
    }
    return parent::canVote($vote, $account);
  }

}
