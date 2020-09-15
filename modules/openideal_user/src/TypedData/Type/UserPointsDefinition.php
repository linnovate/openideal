<?php

namespace Drupal\openideal_user\TypedData\Type;

use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\Core\TypedData\ComplexDataDefinitionBase;
use Drupal\Core\TypedData\DataDefinition;

/**
 * A typed data definition class for describing user points information.
 */
class UserPointsDefinition extends ComplexDataDefinitionBase {

  use StringTranslationTrait;

  /**
   * {@inheritdoc}
   */
  public function getPropertyDefinitions() {
    if (!isset($this->propertyDefinitions)) {
      $this->propertyDefinitions['vote'] = DataDefinition::create('string')
        ->setLabel($this->t('Vote'))
        ->setDescription($this->t('Vote weight'))
        ->setRequired(TRUE);
      $this->propertyDefinitions['comment'] = DataDefinition::create('string')
        ->setLabel($this->t('Comment'))
        ->setDescription($this->t('Comments weight'))
        ->setRequired(TRUE);
      $this->propertyDefinitions['idea'] = DataDefinition::create('string')
        ->setLabel($this->t('Idea'))
        ->setDescription($this->t('Idea weight'))
        ->setRequired(TRUE);
    }
    return $this->propertyDefinitions;
  }

}
