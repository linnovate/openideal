<?php

namespace Drupal\openideal_idea\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Logger\LoggerChannelFactoryInterface;
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
   * Logger factory.
   *
   * @var \Drupal\Core\Logger\LoggerChannelInterface
   */
  protected $logger;

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
   * @param \Drupal\Core\Logger\LoggerChannelFactoryInterface $logger
   *   Logger factory.
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    CurrentRouteMatch $current_route_match,
    LoggerChannelFactoryInterface $logger
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->currentRouteMatch = $current_route_match;
    $this->logger = $logger->get('openideal_idea');
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('current_route_match'),
      $container->get('logger.factory')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $node = $this->currentRouteMatch->getParameter('node');
    $page = '';
    $build = [];
    if ($node instanceof NodeInterface) {
      $bundle = $node->bundle();
      switch ($bundle) {
        case 'idea':
          $url = Url::fromRoute('view.ideas.all_ideas_page');
          // @todo Change to Node::getPluralLabel once
          //   https://www.drupal.org/project/drupal/issues/2765065 implemented.
          $page = $this->t('ideas');
          break;

        case 'challenge':
          $url = Url::fromRoute('view.challenges.all_challenges_page');
          $page = $this->t('challenges');
          break;

        case 'article':
          $url = Url::fromRoute('view.news.all_news_page');
          $page = $this->t('article');
          break;

        case 'discussion':
          $idea = $node->get('field_idea');
          if ($idea->isEmpty()) {
            $this->logger->error('Cannot find group for discussion, id:@id', ['@id' => $node->id()]);
            return $build;
          }

          $url = $idea->first()->get('entity')->getTarget()->getValue()->toUrl();
          $page = $this->t('Idea');

      }

      $build['link'] = [
        '#type' => 'link',
        '#title' => $this->t('Back to @page', ['@page' => $page]),
        '#url' => $url,
      ];
      $build['#cache']['tags'] = $node->getCacheTags();
    }

    return $build;
  }

}
