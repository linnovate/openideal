<?php

namespace Drupal\openideal_idea\Plugin\RabbitHoleBehaviorPlugin;

use Drupal\Core\Entity\EntityInterface;
use Drupal\rabbit_hole\Plugin\RabbitHoleBehaviorPlugin\PageRedirect;

/**
 * Redirects to another page.
 *
 * @RabbitHoleBehaviorPlugin(
 *   id = "openideal_group_page_redirect",
 *   label = @Translation("Group page redirect")
 * )
 */
class GroupPageRedirect extends PageRedirect {

  /**
   * {@inheritdoc}
   */
  public function getActionTarget(EntityInterface $entity) {
    /** @var \Drupal\Core\Entity\EntityInterface $group_content */
    foreach ($entity->getContentEntities() as $group_content) {
      if ($group_content->getEntityTypeId() === 'node' && $group_content->bundle() === 'idea') {
        return $group_content->toUrl()->toString();
      }
    }

    return parent::getActionTarget($entity);
  }

}
