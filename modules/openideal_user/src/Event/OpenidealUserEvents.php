<?php

namespace Drupal\openideal_user\Event;

/**
 * Contains all events provided by openideal_user module.
 */
final class OpenidealUserEvents {

  /**
   * Name of the event fired when the user is mentioned in comment.
   *
   * This event allows modules to perform an action whenever someone mention
   * user in comments.
   *
   * @var string
   */
  const OPENIDEAL_USER_MENTION = 'openideal_user.user_mention';

  /**
   * Name of the event fired when the user joined the group.
   *
   * @var string
   */
  const OPENIDEA_USER_JOINED_GROUP = 'openideal_user.user_joined_group';

  /**
   * Name of the event fired when the user joined the group.
   *
   * @var string
   */
  const OPENIDEA_USER_LEFT_GROUP = 'openideal_user.user_left_group';

  /**
   * Name of the event fired when the user joined the site.
   *
   * @var string
   */
  const OPENIDEA_USER_JOINED_THE_SITE = 'openideal_user.user_joined_site';

  /**
   * Name of the event fired when content changes state.
   *
   * Rules module can't get private&public object properties and
   * that event is a "decorator" for STATE_CHANGED event.
   *
   * @see \Drupal\content_moderation\Event\ContentModerationStateChangedEvent
   * @see \Drupal\content_moderation\Entity\ContentModerationState::realSave()
   *
   * @var string
   */
  const WORKFLOW_STATE_CHANGED = 'openideal_user.content_moderation.state_changed';

}
