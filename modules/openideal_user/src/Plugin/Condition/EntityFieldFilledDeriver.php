<?php

namespace Drupal\openideal_user\Plugin\Condition;

use Drupal\Component\Plugin\Derivative\DeriverBase;
use Drupal\Core\Entity\EntityFieldManagerInterface;
use Drupal\Core\Plugin\Discovery\ContainerDeriverInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\field\Entity\FieldConfig;
use Drupal\rules\Context\ContextDefinition;
use Drupal\rules\Context\ContextDefinitionInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Derives condition plugin definitions based on entity field is filled.
 *
 * @see \Drupal\openideal_user\Plugin\Condition\IdeaIsInState
 */
class EntityFieldFilledDeriver extends DeriverBase implements ContainerDeriverInterface {

  use StringTranslationTrait;

  /**
   * Entity field manager.
   *
   * @var \Drupal\Core\Entity\EntityFieldManagerInterface
   */
  protected $entityFieldManager;

  /**
   * Creates a new TransactionCreateDeriver object.
   *
   * @param \Drupal\Core\Entity\EntityFieldManagerInterface $entity_field_manager
   *   Entity field manager.
   */
  public function __construct(EntityFieldManagerInterface $entity_field_manager) {
    $this->entityFieldManager = $entity_field_manager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, $base_plugin_id) {
    return new static(
      $container->get('entity_field.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function getDerivativeDefinitions($base_plugin_definition) {
    $entity_types = [
      'idea' => 'node',
      'challenge' => 'node',
      'user' => 'user',
    ];
    // Loop through needed entities and set plugin field definitions for them.
    foreach ($entity_types as $bundle => $entity_type_id) {
      $entity_fields = $this->entityFieldManager->getFieldDefinitions($entity_type_id, $bundle);
      foreach ($entity_fields as $field_name => $fieldDefinition) {
        // Add the derivative.
        if ($fieldDefinition instanceof FieldConfig) {
          // Save field + bundle, to avoid problems with repeated fields.
          $this->derivatives["${entity_type_id}.${bundle}.${field_name}"] = [
            'label' => $this->t('@bundle @field field was filled', [
              '@field' => $fieldDefinition->getLabel(),
              '@bundle' => ucfirst($bundle),
            ]),
            'category' => $this->t('@bundle', ['@bundle' => ucfirst($bundle)]),
            'provides' => [],
            'context_definitions' => [
              'idea' => ContextDefinition::create('entity:node')
                ->setLabel($this->t('@bundle entity.', ['@bundle' => ucfirst($bundle)]))
                ->setAssignmentRestriction(ContextDefinitionInterface::ASSIGNMENT_RESTRICTION_SELECTOR)
                ->setRequired(TRUE),
            ],
          ] + $base_plugin_definition;
        }
      }
    }

    return $this->derivatives;
  }

}
