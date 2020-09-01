<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Core\Block\BlockBase;

/**
 * Provides a 'OpenidealStatisticsChallengeStatisticsBlock' block.
 *
 * @Block(
 *  id = "openideal_statistics_challenge_statistics",
 *  admin_label = @Translation("Challenge statistics block"),
 *   context = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealStatisticsChallengeStatisticsBlock extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build($challenge = NULL) {
    // @Todo: create a trait that will handle getting the node from context.
    $build = [];
    $contexts = $this->getContexts();
    $is_not_full = isset($contexts['view_mode']) && $contexts['view_mode']->getContextValue() != 'full';
    $id = NULL;

    if (isset($contexts['node']) && !$contexts['node']->getContextValue()->isNew()) {
      $node = $contexts['node']->getContextValue();
      $id = $node->id();
    }
    else {
      return [];
    }

    $build['#theme'] = 'site_wide_statistics_block';
    $build['#main_class'] = 'idea-statistics-block';
    $build['#show_title'] = !$is_not_full;
    $build['#content'] = [
      'ideas' => [
        'bottom' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getChallengeIdeas', [$id]],
          '#create_placeholder' => TRUE,
        ],
        'title' => $this->t('Challenge ideas'),
        'img_class' => $is_not_full ? 'public_stream_idea' : 'statistics_tag',
      ],
      'votes' => [
        'bottom' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getVotes', [$id]],
          '#create_placeholder' => TRUE,
        ],
        'title' => $this->t('Votes'),
        'img_class' => $is_not_full ? 'public_stream_like' : 'like_tag',
      ],
      'comments' => [
        'bottom' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getComments', [$id]],
          '#create_placeholder' => TRUE,
        ],
        'title' => $this->t('Comments'),
        'img_class' => $is_not_full ? 'public_stream_comment' : 'comment_tag',
      ],
      'views' => [
        'bottom' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getViews', [$id]],
          '#create_placeholder' => TRUE,
        ],
        'title' => $this->t('Views'),
        'img_class' => $is_not_full ? 'public_stream_view' : 'view_tag',
      ],
    ];

    return $build;
  }

}
