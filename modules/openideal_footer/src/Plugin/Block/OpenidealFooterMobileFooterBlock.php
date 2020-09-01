<?php

namespace Drupal\openideal_footer\Plugin\Block;

use Drupal\openideal_idea\Plugin\Block\OpenidealIdeaFlagAndLikeBlock;

/**
 * Provides a 'MobileFooterBlock' block.
 *
 * @Block(
 *  id = "openideal_footer_mobile_footer_block",
 *  admin_label = @Translation("Mobile footer block"),
 *   context = {
 *      "node" = @ContextDefinition(
 *       "entity:node",
 *       label = @Translation("Current Node"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealFooterMobileFooterBlock extends OpenidealIdeaFlagAndLikeBlock {

  /**
   * {@inheritdoc}
   */
  public function build() {
    $contexts = $this->getContexts();
    if (isset($contexts['node']) && ($node = $contexts['node']->getContextValue()) && !$node->isNew()) {
      // We should only display share section for article and anonymous user,
      // because article has not comments, follow, and likes at all
      // and anonymous user has no access to it.
      if ($node->bundle() != 'article' || $this->currentUser->isAnonymous()) {
        $build = parent::build();
        $build['#comment'] = TRUE;
      }
      $build['#cols'] = empty($build) ? 'col-24' : 'col-6';
      $build['#main_class'] = 'site-footer-mobile-block';
      $build['#share'] = TRUE;
    }

    $build['#theme'] = 'openideal_footer_mobile_footer_block';
    return $build;
  }

}
