<?php

namespace Drupal\openideal_statistics\EventSubscriber;

use Drupal\Core\Messenger\MessengerTrait;
use Drupal\layout_builder\Event\SectionComponentBuildRenderArrayEvent;
use Drupal\layout_builder\LayoutBuilderEvents;
use Symfony\Component\EventDispatcher\EventSubscriberInterface;

/**
 * Class OpenidealIdeaEventSubscriber.
 */
class OpenidealStatisticsEventSubscriber implements EventSubscriberInterface {

  use MessengerTrait;

  /**
   * {@inheritdoc}
   */
  public static function getSubscribedEvents() {
    // Make sure to react on event before layout_builder.
    // @see BlockComponentRenderArray
    $events[LayoutBuilderEvents::SECTION_COMPONENT_BUILD_RENDER_ARRAY] = ['onBuildRender', 101];
    return $events;
  }

  /**
   * Adds to the statistics and status block context current node.
   *
   * @param \Drupal\layout_builder\Event\SectionComponentBuildRenderArrayEvent $event
   *   Event.
   */
  public function onBuildRender(SectionComponentBuildRenderArrayEvent $event) {
    $context = $event->getContexts();
    $plugin = $event->getPlugin();
    if (($plugin->getPLuginId() == 'openideal_statistics_idea_statistics'
        || $plugin->getPluginId() == 'openideal_statistics_and_status'
        || $plugin->getPluginId() == 'openideal_statistics_challenge_statistics')
      && isset($context['entity'])) {
      $plugin->setContext('view_mode', $context['view_mode']);
    }
  }

}
