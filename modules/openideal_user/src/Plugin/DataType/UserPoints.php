<?php

namespace Drupal\openideal_user\Plugin\DataType;

use Drupal\Core\TypedData\Plugin\DataType\Map;
use Drupal\rules\TypedData\Type\SiteInterface;

/**
 * The "user points" data type.
 *
 * @ingroup typed_data
 *
 * @DataType(
 *   id = "user_points",
 *   label = @Translation("User points information"),
 *   description = @Translation("User points information"),
 *   definition_class = "\Drupal\openideal_user\TypedData\Type\UserPointsDefinition"
 * )
 */
class UserPoints extends Map implements SiteInterface {

}
