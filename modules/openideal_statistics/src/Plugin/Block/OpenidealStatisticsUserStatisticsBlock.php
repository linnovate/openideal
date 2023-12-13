<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\openideal_challenge\OpenidealContextEntityTrait;

/**
 * Provides a 'OpenidealStatisticsUserStatisticsBlock' block.
 *
 * @Block(
 *  id = "openideal_statistics_user_statistics",
 *  admin_label = @Translation("User statistics block"),
 *   context_definitions = {
 *      "node" = @ContextDefinition(
 *       "entity:user",
 *       label = @Translation("Current user"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealStatisticsUserStatisticsBlock extends OpenidealStatisticsBaseStatisticsBlock {

  use OpenidealContextEntityTrait;

  /**
   * {@inheritdoc}
   */
  public function build() {
    $id = NULL;

    if ($user = $this->getEntity($this->getContexts())) {
      $id = $user->id();
    }
    else {
      return [];
    }

    $items = [
      'points' => [
        '#lazy_element' => [
          '#markup' => (int) $user->field_points->value,
        ],
        '#item_title' => $this->t('@user points', ['@user' => $user->getDisplayName()]),
        '#img_class' => 'score_tag',
      ],
      'ideas' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getUserIdeas',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('@user ideas', ['@user' => $user->getDisplayName()]),
        '#img_class' => 'public_stream_idea',
      ],
      'votes' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getUserVotes',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('@user votes', ['@user' => $user->getDisplayName()]),
        '#img_class' => 'public_stream_like',
      ],
      'comments' => [
        '#lazy_element' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getUserComments',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('@user comments', ['@user' => $user->getDisplayName()]),
        '#img_class' => 'public_stream_comment',
      ],
    ];

    return $this->buildItems($items, 'idea-statistics-block', $user, FALSE);
  }

}
