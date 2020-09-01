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
    $build['#theme'] = 'site_wide_statistics_block';
    $build['#main_class'] = 'site-wide-statistics-block';
    $build['#show_title'] = TRUE;
    $build['#content'] = [
      'ideas' => [
        'bottom' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getIdeas', []],
          '#create_placeholder' => TRUE,
        ],
        'title' => $this->t('Ideas'),
        'img_class' => 'statistics_tag',
      ],
      'members' => [
        'bottom' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getMembers', []],
          '#create_placeholder' => TRUE,
        ],
        'title' => $this->t('Members'),
        'img_class' => 'members_tag',
      ],
      'comments' => [
        'bottom' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getComments', []],
          '#create_placeholder' => TRUE,
        ],
        'title' => $this->t('Comments'),
        'img_class' => 'comment_tag',
      ],
      'votes' => [
        'bottom' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getVotes', []],
          '#create_placeholder' => TRUE,
        ],
        'title' => $this->t('Votes'),
        'img_class' => 'like_tag',
      ],
    ];
    return $build;
  }

}
