<?php

namespace Drupal\openideal_user\Event;

use Drupal\user\UserInterface;
use Symfony\Component\EventDispatcher\Event;

/**
 * An event to that fires when user join the site.
 */
class OpenidealUserJoinedSiteEvent extends Event {

  /**
   * User that joined the site.
   *
   * @var \Drupal\user\Entity\User
   */
  public $user;

  /**
   * OpenidealUserJoinedSiteEvent construct.
   *
   * @param \Drupal\user\UserInterface $user
   *   Group content entity.
   */
  public function __construct(UserInterface $user) {
    $this->user = $user;
  }

  /**
   * Get user.
   *
   * @return \Drupal\user\Entity\User
   *   User.
   */
  public function getUser() {
    return $this->user;
  }

}
