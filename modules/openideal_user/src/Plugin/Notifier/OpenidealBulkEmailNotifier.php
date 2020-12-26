<?php

namespace Drupal\openideal_user\Plugin\Notifier;

use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\message\MessageInterface;
use Drupal\message_notify\Plugin\Notifier\MessageNotifierBase;
use Drupal\user\UserInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Email notifier.
 *
 * @Notifier(
 *   id = "openideal_user_bulk_email",
 *   title = @Translation("Send bulk emails"),
 *   description = @Translation("Send messages via email"),
 *   viewModes = {
 *     "mail_subject",
 *     "mail_body"
 *   }
 * )
 */
class OpenidealBulkEmailNotifier extends MessageNotifierBase {

  use StringTranslationTrait;

  /**
   * The mail manager service.
   *
   * @var \Drupal\Core\Mail\MailManagerInterface
   */
  protected $mailManager;

  /**
   * Flag service.
   *
   * @var \Drupal\flag\FlagService
   */
  protected $flag;

  /**
   * Openideal Helper service.
   *
   * @var \Drupal\openideal_idea\OpenidealHelper
   */
  protected $helper;

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition, MessageInterface $message = NULL) {
    $instance = parent::create($container, $configuration, $plugin_id, $plugin_definition, $message);
    $instance->mailManager = $container->get('plugin.manager.mail');
    $instance->flag = $container->get('flag');
    $instance->helper = $container->get('openideal_idea.helper');

    return $instance;
  }

  /**
   * {@inheritdoc}
   */
  public function deliver(array $output = []) {
    $recipients = $this->getRecipients();

    if (empty($recipients)) {
      return TRUE;
    }

    $language = $this->message->language()->getId();

    // The subject in an email can't be with HTML, so strip it.
    $output['mail_subject'] = trim(strip_tags($output['mail_subject']));

    // Pass the message entity along to hook_drupal_mail().
    $output['message_entity'] = $this->message;

    foreach ($recipients as $mail) {
      $result = $this->mailManager->mail(
        'openideal_user',
        $this->message->getTemplate()->id(),
        $mail,
        $language,
        $output
      );

      if ($result['result'] != TRUE) {
        $this->logger->error($this->t('Could not send message using @title to user ID @uid.',
          [
            '@title' => $this->pluginDefinition['title'],
            '@uid' => $this->message->getOwnerId(),
          ]
        ));
      }

    }

    // Doesn't matter what we will return.
    return TRUE;
  }

  /**
   * Get the recipients.
   *
   * @return array
   *   An array uid => email
   */
  protected function getRecipients() {
    $template = $this->message->getTemplate()->id();
    $recipients = [];

    if ($template === 'node_created' || $template == 'user_joined') {
      return $recipients;
    }

    // In case of mention send only for the mentioned user.
    if ($template === 'user_mention') {
      $owner = $this->message->getOwner();
      return [$owner->id() => $owner->getEmail()];
    }

    // If it's a reply to comment only need to notify the parent comment owner.
    if ($template === 'created_reply_on_comment') {
      $comment = $this->message->field_comment_reference->entity;
      $owner = $comment->getParentComment()->getOwner();
      return [$owner->id() => $owner->getEmail()];
    }

    /** @var \Drupal\node\NodeInterface $node */
    $node = $this->message->field_node_reference->entity;

    // Get all flaggings and notify users subscribed to the entity.
    /** @var \Drupal\flag\FlagService $flag */
    $flagging_users = $this->flag->getFlaggingUsers($node);
    foreach ($flagging_users as $user) {
      $recipients[$user->id()] = $user->getEmail();
    }

    // If the comment owner at the same time is follower of commented entity,
    // remove notification of new comment so we don't notify the user two times.
    if ($template == 'comment_created') {
      $comment = $this->message->field_comment_reference->entity;
      if ($comment->hasParentComment()) {
        $owner = $comment->getParentComment()->getOwner();
        $this->removeOwner($owner, $recipients);
      }
    }

    return array_unique($recipients);
  }

  /**
   * {@inheritDoc}
   */
  public function postSend($result, array $output = []) {
    // Nothing to do here.
  }

  /**
   * Remove the owner from recipients.
   *
   * @param \Drupal\user\UserInterface $owner
   *   The owner.
   * @param array $recipients
   *   The array from which remove the owner.
   */
  private function removeOwner(UserInterface $owner, array &$recipients) {
    $id = $owner->id();
    if (array_key_exists($id, $recipients)) {
      unset($recipients[$id]);
    }
  }

}
