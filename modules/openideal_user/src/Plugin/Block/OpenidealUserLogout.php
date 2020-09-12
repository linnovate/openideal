<?php

namespace Drupal\openideal_user\Plugin\Block;

use Drupal\Core\Block\BlockBase;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\Session\AccountProxy;
use Drupal\Core\Url;
use Drupal\openideal_challenge\OpenidealContextEntityTrait;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Provides a 'OpenidealUserLogout' block.
 *
 * @Block(
 *  id = "openideal_user_logout_block",
 *  admin_label = @Translation("User logout"),
 *   context = {
 *      "user" = @ContextDefinition(
 *       "entity:user",
 *       label = @Translation("Current user"),
 *       required = FALSE,
 *     )
 *   }
 * )
 */
class OpenidealUserLogout extends BlockBase implements ContainerFactoryPluginInterface {

  use OpenidealContextEntityTrait;

  /**
   * Route match.
   *
   * @var \Drupal\Core\Session\AccountProxy
   */
  protected $currentUser;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    AccountProxy $accountProxy
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->currentUser = $accountProxy;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('current_user')
    );
  }

  use OpenidealContextEntityTrait;

  /**
   * {@inheritdoc}
   */
  public function build() {
    $build = [];
    if (($user = $this->getEntity($this->getContexts(), 'user'))
      && $this->currentUser->id() == $user->id()
      && !$this->currentUser->isAnonymous()
    ) {
      $build = [
        'logout' => [
          '#type' => 'link',
          '#title' => $this->t('Log out'),
          '#url' => Url::fromRoute('user.logout', ['user' => $user->id()]),
        ],
      ];

      $build['#cache']['contexts'][] = 'user.roles:authenticated';
    }

    return $build;
  }

}
