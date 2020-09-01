<?php

namespace Drupal\openideal_user\Event;

use Drupal\comment\Entity\Comment;
use Drupal\user\Entity\User;
use Symfony\Component\EventDispatcher\Event;

/**
 * Class OpenidealUserMentionEvent.
 *
 * Event provided by CKEditorMention module is dispatched every time
 * entity is created and even if that entity has not mentioned users inside.
 * That behaviour doesn't make sense. Created an event that will react only when
 * user where mentioned in comment.
 *
 * @see \Drupal\ckeditor_mentions\MentionEventDispatcher
 */
class OpenidealUserMentionEvent extends Event {

  /**
   * Mentioned user id.
   *
   * @var \Drupal\user\Entity\User
   */
  public $user;

  /**
   * Comment that mentions a user.
   *
   * @var \Drupal\comment\Entity\Comment
   */
  public $comment;

  /**
   * OpenideaLUserMentionEvent construct.
   *
   * @param \Drupal\comment\Entity\Comment $comment
   *   Comment that mentions a user.
   * @param \Drupal\user\Entity\User $user
   *   Mentioned user id.
   */
  public function __construct(Comment $comment, User $user) {
    $this->user = $user;
    $this->comment = $comment;
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

  /**
   * Get comment that mentions user.
   *
   * @return \Drupal\comment\Entity\Comment
   *   Comment.
   */
  public function getComment() {
    return $this->comment;
  }

}
