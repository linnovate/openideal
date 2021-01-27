<?php

namespace Drupal\openideal_statistics\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\openideal_challenge\OpenidealContextEntityTrait;
use Drupal\openideal_statistics\OpenidealStatisticsFivestarsTrait;

/**
 * Provides a 'OpenidealStatisticsIdeaStatisticsBlock' block.
 *
 * @Block(
 *  id = "openideal_statistics_idea_statistics",
 *  admin_label = @Translation("Idea statistics block"),
 *   context = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealStatisticsIdeaStatisticsBlock extends BlockBase {

  use OpenidealContextEntityTrait;
  use OpenidealStatisticsFivestarsTrait;

  /**
   * {@inheritdoc}
   */
  public function build($challenge = NULL) {
    $contexts = $this->getContexts();
    $is_not_full = isset($contexts['view_mode']) && $contexts['view_mode']->getContextValue() != 'full';
    $id = NULL;

    /** @var \Drupal\node\NodeInterface $node */
    if ($node = $this->getEntity($this->getContexts())) {
      $id = $node->id();
    }
    else {
      return [];
    }

    $items = [
      'overall_score' => [
        '#lazy_element' => [
          $challenge ? '' : $node->field_overall_score->first()->view(['settings' => ['scale' => 0]]),
        ],
        '#item_title' => $this->t('Overall score'),
        '#img_class' => 'score_tag',
      ],
      'votes' => [
        '#lazy_element' => [
          '#lazy_builder' => [
            'openideal_statistics.lazy_builder:getVotes',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Votes'),
        '#img_class' => $is_not_full ? 'public_stream_like' : 'like_tag',
      ],
      'comments' => [
        '#lazy_element' => [
          '#lazy_builder' => [
            'openideal_statistics.lazy_builder:getComments',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Comments'),
        '#img_class' => $is_not_full ? 'public_stream_comment' : 'comment_tag',
      ],
      'views' => [
        '#lazy_element' => [
          '#lazy_builder' => [
            'openideal_statistics.lazy_builder:getViews',
            [$id],
          ],
          '#create_placeholder' => TRUE,
        ],
        '#item_title' => $this->t('Views'),
        '#img_class' => $is_not_full ? 'public_stream_view' : 'view_tag',
      ],
    ];

    // Do not show if idea is not in below then expert review workflow phase.
    if ($this->configuration['show_five_stars'] && $node->bundle() == 'idea' &&
      !in_array($node->moderation_state->value,
        [
          'published',
          'draft_approval',
          'draft',
        ]
      )) {
      $items += $this->viewFivestars($node);
    }

    // @todo create trait or abstract class with this as method.
    foreach ($items as &$item) {
      $item['#wrapper_attributes'] = ['class' => ['idea-statistics-block--list__item']];
      $item['#type'] = 'statistics_item';
      $item['#show_title'] = !$is_not_full;
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

  /**
   * {@inheritDoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['show_five_stars'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Show five stars'),
      '#default_value' => $this->configuration['show_five_stars'],
    ];
    return $form;
  }

  /**
   * {@inheritDoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['show_five_stars'] = $form_state->getValue('show_five_stars');
  }

  /**
   * {@inheritDoc}
   */
  public function defaultConfiguration() {
    return parent::defaultConfiguration() + ['show_five_stars' => FALSE];
  }

}
