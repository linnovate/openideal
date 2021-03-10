<?php

namespace Drupal\openideal_user\Event;

use Drupal\content_moderation\Event\ContentModerationStateChangedEvent;
use Symfony\Component\EventDispatcher\Event;

/**
 * An event for content moderation state.
 */
class OpenidealContentModerationEvent extends Event {

  /**
   * The entity that was moderated.
   *
   * @var \Drupal\Core\Entity\ContentEntityInterface
   */
  public $moderatedEntity;

  /**
   * The state the content has changed to.
   *
   * @var string
   */
  public $newState;

  /**
   * The state the content was before, or FALSE if none existed.
   *
   * @var string|false
   */
  public $originalState;

  /**
   * The ID of the workflow which allowed the state change.
   *
   * @var string
   */
  public $workflow;

  /**
   * Create a new ContentModerationStateChangedEvent.
   *
   * @param \Drupal\content_moderation\Event\ContentModerationStateChangedEvent $event
   *   The entity that is being moderated.
   */
  public function __construct(ContentModerationStateChangedEvent $event) {
    $this->moderatedEntity = $event->getModeratedEntity();
    $this->newState = $event->getNewState();
    $this->originalState = $event->getOriginalState();
    $this->workflow = $event->getWorkflow();
  }

  /**
   * Get the entity that is being moderated.
   *
   * @return \Drupal\Core\Entity\ContentEntityInterface
   *   The entity that is being moderated.
   */
  public function getModeratedEntity() {
    return $this->moderatedEntity;
  }

  /**
   * Get the new state of the content.
   *
   * @return string
   *   The state the content has been changed to.
   */
  public function getNewState() {
    return $this->newState;
  }

  /**
   * Get the original state of the content.
   *
   * @return string
   *   The state the content was before.
   */
  public function getOriginalState() {
    return $this->originalState;
  }

  /**
   * Get the ID of the workflow which allowed this state change.
   *
   * @return string
   *   The ID of the workflow.
   */
  public function getWorkflow() {
    return $this->workflow;
  }

}
