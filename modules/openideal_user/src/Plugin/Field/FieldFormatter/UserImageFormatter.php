<?php

namespace Drupal\openideal_user\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\EntityReferenceFieldItemListInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\image\Plugin\Field\FieldFormatter\ImageFormatter;

/**
 * Plugin implementation of the 'user image' formatter.
 *
 * @FieldFormatter(
 *   id = "openideal_user_user_image",
 *   label = @Translation("User image formatter"),
 *   field_types = {
 *     "image"
 *   },
 *   quickedit = {
 *     "editor" = "image"
 *   }
 * )
 */
class UserImageFormatter extends ImageFormatter implements ContainerFactoryPluginInterface {

  /**
   * {@inheritDoc}
   */
  protected function getEntitiesToView(EntityReferenceFieldItemListInterface $items, $langcode) {
    // Set user name as a title.
    if (!$items->isEmpty()) {
      $items[0]->set('title', $items->getEntity()->getDisplayName());
    }
    return parent::getEntitiesToView($items, $langcode);
  }

}
