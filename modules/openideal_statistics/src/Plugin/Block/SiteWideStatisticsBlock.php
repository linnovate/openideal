<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Core\Block\BlockBase;

/**
 * Provides a 'SiteWideStatisticsBlock' block.
 *
 * @Block(
 *  id = "site_wide_statistics_block",
 *  admin_label = @Translation("Site wide statistics block"),
 * )
 */
class SiteWideStatisticsBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $items = [
      'ideas' => [
        '#img_class' => 'statistics_tag',
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getIdeas', []],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Ideas'),
      ],
      'members' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getMembers', []],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Members'),
        '#img_class' => 'members_tag',
      ],
      'comments' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getComments', []],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Comments'),
        '#img_class' => 'comment_tag',
      ],
      'votes' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getVotes', []],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Votes'),
        '#img_class' => 'like_tag',
      ],
    ];

    foreach ($items as &$item) {
      $item['#wrapper_attributes'] = ['class' => ['idea-statistics-block--list__item']];
      $item['#type'] = 'statistics_item';
    }

    return [
      'content' => [
        '#theme' => 'item_list',
        '#items' => $items,
        '#attributes' => ['class' => ['idea-statistics-block--list']],
        '#wrapper_attributes' => ['class' => ['idea-statistics-block']],
      ],
    ];
  }

}
