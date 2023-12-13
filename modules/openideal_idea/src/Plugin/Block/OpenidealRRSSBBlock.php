<?php

namespace Drupal\openideal_idea\Plugin\Block;

use Drupal\openideal_challenge\OpenidealContextEntityTrait;
use Drupal\rrssb\Plugin\Block\RRSSBBlock;

/**
 * Provides a 'OpenidealRRSSBBlock' block.
 *
 * @Block(
 *   id = "openideal_rrssb_block",
 *   admin_label = @Translation("Openideal RRSSB block with ability to set contextual entity"),
 *   category = @Translation("RRSSB"),
 *   context = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealRRSSBBlock extends RRSSBBlock {

  use OpenidealContextEntityTrait;

  /**
   * {@inheritdoc}
   */
  public function build() {
    if ($node = $this->getEntity($this->getContexts())) {
      $config = $this->getConfiguration();
      return rrssb_get_buttons($config['button_set'], $node, 'url.path');
    }
    return [];
  }

}
