<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Entity\EntityInterface;

/**
 * Provides base class for openidealEntityStatistics blocks.
 */
abstract class OpenidealStatisticsBaseStatisticsBlock extends BlockBase {

  /**
   * Checks if it's a full view mode.
   *
   * @return bool
   *   True is not provided view mode.
   */
  public function isNotViewMode($mode) {
    return isset($this->getContexts()['view_mode']) && ($this->getContexts()['view_mode']->getContextValue() != $mode);
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
