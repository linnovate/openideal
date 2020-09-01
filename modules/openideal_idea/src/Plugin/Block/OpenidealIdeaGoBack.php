<?php

namespace Drupal\openideal_idea\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Routing\CurrentRouteMatch;
use Drupal\Core\Url;
use Drupal\node\NodeInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'Go back' block.
 *
 * @Block(
 *  id = "openideal_idea_go_back_block",
 *  admin_label = @Translation("Go back"),
 * )
 */
class OpenidealIdeaGoBack extends BlockBase implements ContainerFactoryPluginInterface {

  /**
   * Current route match service.
   *
   * @var \Drupal\Core\Routing\CurrentRouteMatch
   */
  protected $currentRouteMatch;

  /**
   * Constructs a new OpenidealIdeaGoBack object.
   *
   * @param array $configuration
   *   A configuration array containing information about the plugin instance.
   * @param string $plugin_id
   *   The plugin_id for the plugin instance.
   * @param string $plugin_definition
   *   The plugin implementation definition.
   * @param \Drupal\Core\Routing\CurrentRouteMatch $current_route_match
   *   Current route match.
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    CurrentRouteMatch $current_route_match
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->currentRouteMatch = $current_route_match;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('current_route_match')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $node = $this->currentRouteMatch->getParameter('node');
    $build = [];
    if ($node instanceof NodeInterface) {
      $bundle = $node->bundle();

      switch ($bundle) {
        case 'idea':
          $url = Url::fromRoute('view.ideas.all_ideas_page');
          break;

        case 'challenge':
          $url = Url::fromRoute('view.challenges.all_challenges_page');
          break;

        case 'article':
          $url = Url::fromRoute('view.news.all_news_page');
          break;

      }

      $build['link'] = [
        '#type' => 'link',
        '#title' => $this->t('Back to @page', ['@page' => $bundle . 's']),
        '#url' => $url,
      ];
      $build['#cache']['tags'] = $node->getCacheTags();
    }

    return $build;
  }

}
