<?php

namespace Drupal\openideal_challenge\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\openideal_challenge\OpenidealContextEntityTrait;

/**
 * Class ChallengeIdeasTitleWithCount.
 *
 * @Block(
 *   id = "openidel_challenge_challenge_ideas_title",
 *   admin_label = @Translation("Challenge ideas title with ideas count"),
 *   category = @Translation("Openideal"),
 *   context = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class ChallengeIdeasTitleWithCount extends BlockBase {

  use OpenidealContextEntityTrait;

  /**
   * {@inheritDoc}
   */
  public function build() {
    $build = [];
    if ($node = $this->getEntity($this->getContexts())) {
      $build['content'] = [
        '#type' => 'container',
        '#attributes' => ['class' => ['challenges-ideas-title']],
        'title' => [
          '#markup' => $this->t('Challenge ideas'),
        ],
        // The lazy builder element do not supports the prefix and suffix,
        // so add them like this.
        'prefix' => [
          '#markup' => ' (',
        ],
        'count' => [
          '#lazy_builder' => ['openideal_statistics.lazy_builder:getChallengeIdeas', [$node->id()]],
          '#create_placeholder' => TRUE,
        ],
        'suffix' => [
          '#markup' => ')',
        ],
      ];
      $build['#cache']['tags'] = ['node_list:idea'];
    }

    return $build;
  }

}
