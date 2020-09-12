<?php

namespace Drupal\openideal_user\Plugin\Field\FieldWidget;

use Drupal\entity_browser\Plugin\Field\FieldWidget\FileBrowserWidget;

/**
 * Entity browser single file widget.
 *
 * @FieldWidget(
 *   id = "openideal_user_entity_browser_single_file",
 *   label = @Translation("Single file entity browser"),
 *   provider = "entity_browser",
 *   multiple_values = TRUE,
 *   field_types = {
 *     "file",
 *     "image"
 *   }
 * )
 */
class SingleFileBrowserWidget extends FileBrowserWidget {

  /**
   * {@inheritdoc}
   *
   * Removed table implementation for multiple values,
   * because we need this only for single image.
   */
  protected function displayCurrentSelection($details_id, array $field_parents, array $entities) {
    $current = parent::displayCurrentSelection($details_id, $field_parents, $entities);
    $current['#type'] = 'container';
    $current['#attributes']['class'][] = 'single-entity-browser-widget';
    if (!empty($entities)) {
      $id = (reset($entities))->id();
      $current[$id]['filename']['#access'] = FALSE;
      $current[$id]['meta']['#access'] = FALSE;
      $current[$id]['_weight']['#access'] = FALSE;
    }
    return $current;
  }

}
