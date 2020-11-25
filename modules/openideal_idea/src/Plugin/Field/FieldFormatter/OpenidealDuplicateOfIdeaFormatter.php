<?php

namespace Drupal\openideal_idea\Plugin\Field\FieldFormatter;

use Drupal\Core\Field\FieldDefinitionInterface;
use Drupal\Core\Field\Plugin\Field\FieldFormatter\EntityReferenceFormatterBase;
use Drupal\Core\Field\FieldItemListInterface;

/**
 * Plugin implementation of the 'openideal_duplicate_of_idea' formatter.
 *
 * @FieldFormatter(
 *   id = "openideal_duplicate_of_idea",
 *   label = @Translation("Openideal Duplicate Idea"),
 *   description = @Translation("Display the link for a duplicate idea."),
 *   field_types = {
 *     "entity_reference"
 *   }
 * )
 */
class OpenidealDuplicateOfIdeaFormatter extends EntityReferenceFormatterBase {

  /**
   * {@inheritdoc}
   */
  public function viewElements(FieldItemListInterface $items, $langcode) {
    $elements = [];
    // @codingStandardsIgnoreLine
    if (\Drupal::routeMatch()->getRouteName() != 'layout_builder.defaults.node.view') {
      foreach ($this->getEntitiesToView($items, $langcode) as $delta => $entity) {
        if ($entity) {
          $field_text = $this->t('This idea was merged with <a href="@link">@title</a> idea.', [
            '@link' => $entity->toUrl()->toString(),
            '@title' => $entity->label(),
          ]);
          // @todo add css class to the markup.
          $elements[$delta] = [
            '#type' => 'markup',
            '#markup' => '<div>' . $field_text . '</div>',
          ];

          $elements[$delta]['#cache']['tags'] = $entity->getCacheTags();
        }
      }
    }

    return $elements;
  }

  /**
   * {@inheritdoc}
   */
  public static function isApplicable(FieldDefinitionInterface $field_definition) {
    // This formatter is only available for idea content type nodes.
    $target_type = $field_definition->getFieldStorageDefinition()
      ->getSetting('target_type');
    return $target_type == 'node' && $field_definition->get('bundle') == 'idea';
  }

}
