<?php

namespace Drupal\openideal_challenge\Event;

use Drupal\Core\Entity\EntityInterface;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class OpenidealChallengeEvent.
 *
 * @package Drupal\openideal_challenge\Event
 */
class OpenidealChallengeEvent extends Event {

  /**
   * The event triggered after opening a challenge node via cron.
   *
   * @Event
   *
   * @var string
   */
  const CHALLENGE_OPEN = 'openideal_challenge.open';

  /**
   * The event triggered after closing a challenge node via cron.
   *
   * @Event
   *
   * @var string
   */
  const CHALLENGE_CLOSE = 'openideal_challenge.close';

  /**
   * Node object.
   *
   * @var \Drupal\Core\Entity\EntityInterface
   */
  public $entity;

  /**
   * OpenidealChallengeEvent constructor.
   *
   * @param \Drupal\Core\Entity\EntityInterface $node
   *   The node object that caused the event to fire.
   */
  public function __construct(EntityInterface $node) {
    $this->entity = $node;
  }

  /**
   * Gets node object.
   *
   * @return \Drupal\Core\Entity\EntityInterface
   *   The node object that caused the event to fire.
   */
  public function getEntity() {
    return $this->entity;
  }

}
