<?php

/**
 * @file
 * OpenideaL Challenge module tokens support.
 */

use Drupal\Core\Cache\Cache;
use Drupal\Core\Render\BubbleableMetadata;

/**
 * Implements hook_token_info().
 */
function openideal_statistics_token_info() {
  $types = [
    'name' => t('OpenideaL'),
    'description' => t('OpenideaL token types.'),
  ];
  $tokens = [
    'ideas-count' => [
      'name' => t('OpenideaL - the count of all ideas'),
      'description' => t('The count of all ideas in the site'),
      'type' => 'openideal',
    ],
    'comments-count' => [
      'name' => t('OpenideaL - the count of all comments'),
      'description' => t('The count of all comments in the site'),
      'type' => 'openideal',
    ],
    'members-count' => [
      'name' => t('OpenideaL - the count of al members'),
      'description' => t('The count of all member is the site'),
      'type' => 'openideal',
    ],
    'votes-count' => [
      'name' => t('OpenideaL - the count of all votes'),
      'description' => t('The count of all votes in the site'),
      'type' => 'openideal',
    ],
    'node-votes-count' => [
      'name' => t('Idea votes count'),
      'type' => 'openideal',
    ],
    'challenge-ideas-count' => [
      'name' => t('Challenge ideas count'),
      'type' => 'openideal',
    ],
    'user-votes-count' => [
      'name' => t('User votes count'),
      'type' => 'openideal',
    ],
    'user-comments-count' => [
      'name' => t('User comments count'),
      'type' => 'openideal',
    ],
    'user-ideas-count' => [
      'name' => t('User ideas count'),
      'type' => 'openideal',
    ],

  ];

  return [
    'types' => [
      'openideal' => $types,
    ],
    'tokens' => [
      'openideal' => $tokens,
    ],
  ];
}

/**
 * Implements hook_tokens().
 */
function openideal_statistics_tokens($type, $tokens, array $data, array $options, BubbleableMetadata $bubbleable_metadata) {
  $replacements = [];

  // Add tokens that return total count of: users, comments, votes.
  if ($type == 'openideal') {
    $entity_type_manager = \Drupal::entityTypeManager();
    foreach ($tokens as $name => $original) {

      switch ($name) {
        case 'members-count':
          $query = $entity_type_manager->getStorage('user')->getQuery();
          $result = $query
            ->condition('status', '1')
            // @Todo: Remove once we will find better way to resolve this issue,
            // and remove the same logic from community page view.
            ->condition('uid', '1', '>')
            ->count()
            ->execute();
          $replacements[$original] = $result;
          $bubbleable_metadata->addCacheTags(['user_list']);
          break;

        case 'comments-count':
          $query = $entity_type_manager->getStorage('comment')->getQuery();
          $result = $query
            ->condition('status', '1')
            ->count()
            ->execute();
          $replacements[$original] = $result;
          $bubbleable_metadata->addCacheTags(['comment_list']);
          break;

        case 'ideas-count':
          $query = $entity_type_manager->getStorage('node')->getQuery();
          $result = $query
            ->condition('status', '1')
            ->condition('type', 'idea')
            ->count()
            ->execute();
          $replacements[$original] = $result;
          $bubbleable_metadata->addCacheTags(['node_list:idea']);
          break;

        case 'votes-count':
          $query = $entity_type_manager->getStorage('vote')->getQuery();
          $result = $query
            ->count()
            ->execute();
          $replacements[$original] = $result;
          $bubbleable_metadata->addCacheTags(['vote_list']);
          break;

        case 'node-votes-count':
          if (!empty($data['node'])) {
            $count = \Drupal::service('plugin.manager.votingapi.resultfunction')
              ->getResults('node', $data['node']->id());

            // If there are not votes on node, then the array is empty.
            $result = empty($count) ? 0 : $count['reaction_like']['vote_count'];

            $replacements[$original] = $result;
          }
          break;

        case 'user-votes-count':
          if (!empty($data['user'])) {
            $storage = \Drupal::entityTypeManager()->getStorage('vote');
            $count = $storage->getQuery()
              ->condition('user_id', $data['user']->id())
              ->condition('type', 'reaction_like')
              ->count()
              ->execute();

            $replacements[$original] = $count;
            $bubbleable_metadata->addCacheTags(Cache::mergeTags($data['user']->getCacheTags(), ['vote_list']));
          }
          break;

        case 'user-comments-count':
          if (!empty($data['user'])) {
            $query = $entity_type_manager->getStorage('comment')->getQuery();
            $result = $query
              ->condition('status', '1')
              ->condition('uid', $data['user']->id())
              ->count()
              ->execute();
            $replacements[$original] = $result;
            $bubbleable_metadata->addCacheTags(Cache::mergeTags($data['user']->getCacheTags(), ['comment_list']));
          }
          break;

        case 'user-ideas-count':
          if (!empty($data['user'])) {
            $result = \Drupal::entityQuery('node')
              ->condition('status', '1')
              ->condition('type', 'idea')
              ->condition('uid', $data['user']->id())
              ->count()
              ->execute();
            $replacements[$original] = $result;
            $bubbleable_metadata->addCacheTags(Cache::mergeTags($data['user']->getCacheTags(), ['node_list:idea']));
          }
          break;

        case 'challenge-ideas-count':
          if (!empty($data['node_id'])) {
            $storage = \Drupal::entityTypeManager()->getStorage('node');
            $node_query = $storage->getQuery();
            $result = $node_query->condition('field_challenge', $data['node_id'], '=')
              ->condition('type', 'idea')
              ->condition('status', 1)
              ->count()
              ->execute();
            $replacements[$original] = $result;
          }
          break;
      }
    }
  }

  return $replacements;
}
