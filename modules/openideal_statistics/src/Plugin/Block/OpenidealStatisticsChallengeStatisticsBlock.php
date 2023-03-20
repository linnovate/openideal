<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\openideal_challenge\OpenidealContextEntityTrait;

/**
 * Provides a 'OpenidealStatisticsChallengeStatisticsBlock' block.
 *
 * @Block(
 *  id = "openideal_statistics_challenge_statistics",
 *  admin_label = @Translation("Challenge statistics block"),
 *   context_definitions = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealStatisticsChallengeStatisticsBlock extends OpenidealStatisticsBaseStatisticsBlock {

  use OpenidealContextEntityTrait;

  /**
   * {@inheritdoc}
   */
  public function build($challenge = NULL) {
    $is_not_full = !$this->isViewMode('full');
    $id = NULL;

    if ($node = $this->getEntity($this->getContexts())) {
      $id = $node->id();
    }
    else {
      return [];
    }

    $items = [
      'ideas' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getChallengeIdeas',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Challenge ideas'),
        '#img_class' => $is_not_full ? 'public_stream_idea' : 'statistics_tag',
      ],
      'votes' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getVotes',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Votes'),
        '#img_class' => $is_not_full ? 'public_stream_like' : 'like_tag',
      ],
      'comments' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getComments',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Comments'),
        '#img_class' => $is_not_full ? 'public_stream_comment' : 'comment_tag',
      ],
      'views' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getViews',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Views'),
        '#img_class' => $is_not_full ? 'public_stream_view' : 'view_tag',
      ],
    ];

    return $this->buildItems($items, 'idea-statistics-block', $node, !$is_not_full);
  }

}
