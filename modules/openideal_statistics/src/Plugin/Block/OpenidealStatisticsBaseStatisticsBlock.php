<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides base class for openidealEntityStatistics blocks.
 */
abstract class OpenidealStatisticsBaseStatisticsBlock extends BlockBase implements ContainerFactoryPluginInterface {

  /**
   * Route match.
   *
   * @var \Drupal\Core\Routing\RouteMatch
   */
  protected $routeMatch;

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    $instance = new static($configuration, $plugin_id, $plugin_definition);
    $instance->routeMatch = $container->get('current_route_match');
    return $instance;
  }

  /**
   * Checks if provided view mode not active.
   *
   * @return bool
   *   True is not provided view mode.
   */
  public function isViewMode($mode) {
    // Can't check view mode of blocks that are placed as plain structure
    // blocks, so need to check if there is a node and if so then it's
    // full view mode.
    return (($mode == 'full' && $this->routeMatch->getParameter('node')) || (isset($this->getContexts()['view_mode'])) && ($this->getContexts()['view_mode']->getContextValue() == $mode));
  }

  /**
   * Build items.
   *
   * @param array $items
   *   Items to build.
   * @param string $class
   *   Class to apply.
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   Entity to add cache tags.
   * @param bool $show_title
   *   Show title?
   *
   * @return array
   *   Rendarable array.
   */
  public function buildItems(array $items, $class, EntityInterface $entity, bool $show_title) {
    foreach ($items as &$item) {
      $item['#wrapper_attributes'] = ['class' => [$class . '--list__item']];
      $item['#type'] = 'statistics_item';
      $item['#show_title'] = $show_title;
    }

    return [
      'content' => [
        '#theme' => 'item_list',
        '#cache' => ['tags' => $entity->getCacheTags()],
        '#items' => $items,
        '#attributes' => ['class' => [$class . '--list']],
        '#wrapper_attributes' => ['class' => [$class . '']],
      ],
    ];
  }

}
