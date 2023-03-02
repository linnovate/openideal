<?php

namespace Drupal\openideal_user\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Entity\EntityTypeManager;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Url;
use Drupal\openideal_challenge\OpenidealContextEntityTrait;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'OpenidealUserAccount' block.
 *
 * @Block(
 *  id = "openideal_user_user_account_block",
 *  admin_label = @Translation("User account"),
 *   context_definitions = {
 *      "user" = @ContextDefinition(
 *       "entity:user",
 *       label = @Translation("Current user"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealUserAccount extends BlockBase implements ContainerFactoryPluginInterface {

  use OpenidealContextEntityTrait;

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManager
   */
  protected $entityTypeManager;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    EntityTypeManager $entityTypeManager
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->entityTypeManager = $entityTypeManager;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('entity_type.manager')
    );
  }

  /**
   * {@inheritdoc}
   */
  public function build() {
    $build = [
      '#type' => 'container',
      '#attributes' => ['class' => ['site-navigation--user-account']],
    ];
    if (($user = $this->getEntity($this->getContexts(), 'user')) && !$user->isAnonymous()) {
      $view_builder = $this->entityTypeManager->getViewBuilder('user');
      $author = $view_builder->view($user, 'author');
      $build += [
        'author' => $author,
        'personal_settings' => [
          '#type' => 'link',
          '#title' => $this->t('personal settings'),
          '#url' => Url::fromRoute('entity.user.edit_form', ['user' => $user->id()]),
        ],
      ];

      $build['#cache']['tags'] = $user->getCacheTags();

    }
    else {
      $build += [
        'login' => [
          '#type' => 'link',
          '#attributes' => ['class' => ['login-link']],
          '#title' => $this->t('Log in'),
          '#url' => Url::fromRoute('user.login'),
        ],
      ];
    }

    return $build;
  }

}
