<?php

namespace Drupal\openideal_rest\EventSubscriber;

use Drupal\openideal_rest\Event\OpenidealRestEvents;
use Drupal\openideal_rest\Event\WebhookEvent;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

/**
 *
 */
class OpenidealRestEventSubscriber implements EventSubscriberInterface {

  /**
   * {@inheritDoc}
   */
  public static function getSubscribedEvents() {
    $events[OpenidealRestEvents::OUT_COMING_WEBHOOK][] = [
      'onOutComingWebhook', 255,
    ];
    return $events;
  }

  /**
   * Send the message on webhooks.
   *
   * @param \Drupal\openideal_rest\Event\WebhookEvent $event
   *   Event.
   */
  public function onOutComingWebhook(WebhookEvent $event) {
    // @todo Implement.
  }

}
