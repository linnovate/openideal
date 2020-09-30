<?php

namespace Drupal\openideal_idea\Routing;

use Drupal\Core\Routing\RouteSubscriberBase;
use Symfony\Component\Routing\RouteCollection;

/**
 * Class OpenidealIdeaRouteSubscriber.
 *
 * Listens to the dynamic route events.
 */
class OpenidealIdeaRouteSubscriber extends RouteSubscriberBase {

  /**
   * {@inheritdoc}
   */
  protected function alterRoutes(RouteCollection $collection) {
    if ($route = $collection->get('content_moderation.admin_moderated_content')) {
      $route->setPath('/admin/content/ideas');
    }
  }

}
