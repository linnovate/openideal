<?php

namespace Drupal\openideal_user\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\openideal_challenge\OpenidealContextEntityTrait;

/**
 * Provides a 'OpenidealUserName' block.
 *
 * @Block(
 *  id = "openideal_user_user_name_block",
 *  admin_label = @Translation("User name"),
 *   context = {
 *      "user" = @ContextDefinition(
 *       "entity:user",
 *       label = @Translation("Current user"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealUserName extends BlockBase {

  use OpenidealContextEntityTrait;

  /**
   * {@inheritdoc}
   */
  public function build() {
    $build = [];
    if ($user = $this->getEntity($this->getContexts(), 'user')) {
      $build['container'] = [
        '#type' => 'container',
        '#attributes' => ['class' => ['user-compact--name']],
        'description' => [
          '#type' => 'html_tag',
          '#tag' => 'span',
          '#value' => $this->t('Username:'),
        ],
        'name' => [
          '#title' => $user->getDisplayName(),
          '#type' => 'link',
          '#url' => $user->toUrl(),
        ],
      ];
      $build['#cache']['tags'] = $user->getCacheTags();

      if ($this->configuration['hide_label']) {
        $build['container']['description']['#attributes']['class'][] = 'hidden';
      }
    }

    return $build;
  }

  /**
   * {@inheritdoc}
   */
  public function blockForm($form, FormStateInterface $form_state) {
    $form['hide_label'] = [
      '#type' => 'checkbox',
      '#title' => $this->t('Hide label'),
      '#default_value' => $this->configuration['hide_label'],
    ];
    return $form;
  }

  /**
   * {@inheritdoc}
   */
  public function blockSubmit($form, FormStateInterface $form_state) {
    $this->configuration['hide_label'] = $form_state->getValue('hide_label');
  }

}
