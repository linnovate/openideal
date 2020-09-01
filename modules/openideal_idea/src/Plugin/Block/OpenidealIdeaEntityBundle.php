<?php

namespace Drupal\openideal_idea\Plugin\Block;

use Drupal\Core\Block\BlockBase;

/**
 * Provides a OpenidealIdeaEntityBundle class.
 *
 * @Block(
 *   id = "openidel_idea_node_bundle",
 *   admin_label = @Translation("Entity bundle"),
 *   category = @Translation("Openideal"),
 *   context = {
 *      "entity" = @ContextDefinition(
 *       "entity",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealIdeaEntityBundle extends BlockBase {

  /**
   * {@inheritDoc}
   */
  public function build() {
    $build = [];
    $contexts = $this->getContexts();
    // If displayed in layout builder node isn't presented.
    if (isset($contexts['entity']) && ($entity = $contexts['entity']->getContextValue()) && !$entity->isNew()) {
      $build['content_type'] = [
        '#type' => 'html_tag',
        '#tag' => 'div',
        '#attributes' => ['class' => ['entity_bundle_label', 'entity_bundle_label--' . $entity->bundle()]],
        '#value' => $entity->bundle(),
      ];
    }

    return $build;
  }

}
