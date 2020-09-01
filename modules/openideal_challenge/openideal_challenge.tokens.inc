<?php

/**
 * @file
 * OpenideaL Challenge module tokens support.
 */

use Drupal\Core\Render\BubbleableMetadata;

/**
 * Implements hook_token_info().
 */
function openideal_challenge_token_info() {
  $openideal = [
    'name' => t('OpenideaL'),
    'description' => t('OpenideaL token types.'),
  ];

  $count_of_ideas['count-of-ideas'] = [
    'name' => t('OpenideaL - count of ideas'),
    'description' => t('Count of idea for specific challenge'),
    'type' => 'openideal',
  ];

  return [
    'types' => [
      'openideal' => $openideal,
    ],
    'tokens' => [
      'openideal' => $count_of_ideas,
    ],
  ];
}

/**
 * Implements hook_tokens().
 */
function openideal_challenge_tokens($type, $tokens, array $data, array $options, BubbleableMetadata $bubbleable_metadata) {
  $replacements = [];

  if ($type == 'openideal' && !empty($data['challenge_id'])) {
    $challenge_id = $data['challenge_id']->jsonSerialize();
    $entity_type_manager = \Drupal::entityTypeManager()->getStorage('node');
    /** @var \Drupal\node\Entity\Node $challenge */
    $challenge = $entity_type_manager->load((int) $challenge_id);

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'count-of-ideas':
          /** @var \Drupal\openideal_challenge\Service\OpenidealChallengeServiceInterface $challenge_service */
          $challenge_service = \Drupal::service('openideal_challenge.challenge_service');
          $replacements[$original] = $challenge_service->getCountOfIdeas($challenge->id());
          $bubbleable_metadata->addCacheableDependency($challenge);
          break;
      }
    }
  }

  return $replacements;
}
