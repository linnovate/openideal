<?php

namespace Drupal\openideal_idea\Plugin\Block;

use Drupal\Core\Block\BlockBase;

/**
 * Provides a 'Node tags' block.
 *
 * @Block(
 *  id = "openideal_idea_tags_block",
 *  admin_label = @Translation("Node tags"),
 *   context = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealIdeaTags extends BlockBase {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $build = [];
    $contexts = $this->getContexts();
    if (isset($contexts['node'])) {
      $node = $contexts['node']->getContextValue();
      $build = [
        '#theme' => 'item_list',
        '#title' => $this->t('Tags'),
        '#attributes' => ['class' => ['idea-tags']],
      ];
      $items = [];
      foreach ($node->field_idea_tags as $tag) {
        $items[] = $tag->entity->label();
      }
      $build['#items'] = $items;
      $build['#cache']['tags'] = $node->getCacheTags();
    }

    return $build;
  }

}
