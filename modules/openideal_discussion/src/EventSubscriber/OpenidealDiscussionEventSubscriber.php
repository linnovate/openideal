<?php

namespace Drupal\openideal_discussion\EventSubscriber;

use Drupal\Core\Messenger\MessengerTrait;
use Drupal\layout_builder\Event\SectionComponentBuildRenderArrayEvent;
use Drupal\layout_builder\LayoutBuilderEvents;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

/**
 * Openideal Discussion Event subscriber.
 */
class OpenidealDiscussionEventSubscriber implements EventSubscriberInterface {

  use MessengerTrait;

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    // Make sure to react on event before layout_builder.
    // @see BlockComponentRenderArray
    // @codingStandardsIgnoreLine
    $events[LayoutBuilderEvents::SECTION_COMPONENT_BUILD_RENDER_ARRAY] = ['onBuildRender', 20];
    return $events;
  }

  /**
   * Add to links field "Read review" link.
   *
   * @param \Drupal\layout_builder\Event\SectionComponentBuildRenderArrayEvent $event
   *   Event.
   */
  public function onBuildRender(SectionComponentBuildRenderArrayEvent $event) {
    $context = $event->getContexts();
    $plugin = $event->getPlugin();
    if ($plugin->getPLuginId() == 'extra_field_block:node:discussion:links'
      && isset($context['entity'])) {
      $build = $event->getBuild();
      unset($build['content']['#markup']);

      $build['content'] = [
        '#type' => 'link',
        '#title' => t('Read review →'),
        '#url' => $context['entity']->getContextValue()->toUrl(),
      ];

      $event->setBuild($build);
    }
  }

}
