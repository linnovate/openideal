<?php

namespace Drupal\openideal_challenge;

/**
 * Provide trait to get contextual entity and verify it for layout_builder view.
 *
 * @package Drupal\openideal_challenge
 */
trait OpenidealContextEntityTrait {

  /**
   * Get the entity from contexts.
   *
   * @param array $contexts
   *   Contexts.
   * @param string $entity_type
   *   The entity type.
   *
   * @Todo: create an interface with constants those describe $entity_type's?
   *    or always call the entity variable as "entity".
   *
   * @return \Drupal\Core\Entity\EntityInterface|false
   *   Entity.
   */
  protected function getEntity(array $contexts, $entity_type = 'node') {
    if (isset($contexts[$entity_type])
      && ($entity = $contexts[$entity_type]->getContextValue())
      && !$entity->isNew()) {
      return $entity;
    }
    return FALSE;
  }

}
