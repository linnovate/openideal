<?php

namespace Drupal\openideal_idea\EventSubscriber;

use Drupal\content_moderation\Event\ContentModerationEvents;
use Drupal\content_moderation\Event\ContentModerationStateChangedEvent;
use Drupal\Core\Messenger\MessengerTrait;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

/**
 * Class OpenidealIdeaEventSubscriber.
 */
class OpenidealIdeaEventSubscriber implements EventSubscriberInterface {

  use MessengerTrait;

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    $events[ContentModerationEvents::STATE_CHANGED] = ['onContentStateChange'];
    return $events;
  }

  /**
   * Notify users about approval process once it's published to review.
   *
   * @param \Drupal\content_moderation\Event\ContentModerationStateChangedEvent $event
   *   The dispatched event.
   */
  public function onContentStateChange(ContentModerationStateChangedEvent $event) {
    // Need to compare original state with FALSE,
    // because most initial transition is unset.
    if (($event->getOriginalState() === 'draft' || $event->getOriginalState() === FALSE)
      && $event->getNewState() === 'draft_approval') {
      $openideal_config = config_pages_config('openideal_configurations');
      $message = $openideal_config->field_idea_approval_message->view('default');
      if (!empty($message)) {
        $this->messenger()->addMessage($message);
      }
    }
  }

}
