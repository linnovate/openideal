<?php

namespace Drupal\openideal_user;

use Drupal\Core\Cache\CacheableMetadata;
use Drupal\Core\Cache\Context\CacheContextInterface;
use Drupal\Core\Cache\Context\UserCacheContextBase;
use Drupal\Core\Routing\CurrentRouteMatch;
use Drupal\Core\Session\AccountInterface;

/**
 * Defines the IsUserProfilePage service, for "is user profile page" caching.
 *
 * Cache context ID: 'user.is_user_profile_page'.
 */
class IsUserProfilePageCacheContext extends UserCacheContextBase implements CacheContextInterface {

  /**
   * Route match.
   *
   * @var \Drupal\Core\Routing\CurrentRouteMatch
   */
  protected $routeMatch;

  /**
   * {@inheritDoc}
   */
  public function __construct(AccountInterface $user, CurrentRouteMatch $routeMatch) {
    parent::__construct($user);
    $this->routeMatch = $routeMatch;
  }

  /**
   * {@inheritdoc}
   */
  public static function getLabel() {
    return t('Is user page');
  }

  /**
   * {@inheritdoc}
   */
  public function getContext() {
    $result = '0';
    if ($this->routeMatch->getRouteName() == 'entity.user.canonical') {
      $user = $this->routeMatch->getParameter('user');
      $result = $user->id() == $this->user->id() ? '1' : '0';
    }
    return $result;
  }

  /**
   * {@inheritdoc}
   */
  public function getCacheableMetadata() {
    return new CacheableMetadata();
  }

}
