<?php

/**
 * @file
 * Place holder for the message example module.
 */

use Drupal\Core\Render\BubbleableMetadata;

/**
 * Implements hook_token_info_alter().
 */
function openideal_user_token_info_alter(&$info) {
  if (Drupal::moduleHandler()->moduleExists('message') && isset($info['tokens']['message'])) {
    $info['tokens']['message']['trimmed-node-title'] = [
      'name' => t('Node trimmed title'),
    ];
    $info['tokens']['message']['trimmed-comment'] = [
      'name' => t('Trimmed comment'),
    ];
    $info['tokens']['message']['comment-url'] = [
      'name' => t('Comment url'),
    ];
  }
}

/**
 * Implements hook_tokens().
 */
function openideal_user_tokens($type, $tokens, array $data, array $options, BubbleableMetadata $bubbleable_metadata) {
  $replacements = [];
  if ($type == 'message' && !empty($data['message'])) {
    /** @var \Drupal\message\Entity\Message $message */
    $message = $data['message'];

    foreach ($tokens as $name => $original) {
      switch ($name) {
        case 'trimmed-node-title':
          if ($message->hasField('field_node_reference')) {
            $node = $message->field_node_reference[0]->entity;

            $replacements[$original] = rtrim((text_summary($node->title->value, 'basic_html', 80)));
          }
          break;

        case 'trimmed-comment':
          if ($message->hasField('field_comment_reference')) {
            $comment = $message->field_comment_reference[0]->entity;
            $body = $comment->comment_body->view(
              [
                'label' => 'hidden',
                'type' => 'text_trimmed',
                'settings' => ['trim_length' => 130],
              ]);
            $replacements[$original] = Drupal::service('renderer')->render($body);
          }
          break;

        case 'comment-url':
          if ($message->hasField('field_comment_reference')) {
            /** @var \Drupal\comment\Entity\Comment $comment */
            $comment = $message->field_comment_reference[0]->entity;
            $uri = $comment->getCommentedEntity()->toUrl();

            // Set attributes for permalink.
            $attributes = $uri->getOption('attributes') ?: [];
            $attributes += ['class' => ['permalink'], 'rel' => 'bookmark'];
            $uri->setOptions([
              'attributes' => $attributes,
              'fragment' => 'comment-' . $comment->id(),
            ]);

            $replacements[$original] = $uri->toString();
          }

          break;
      }
    }
  }
  return $replacements;
}
