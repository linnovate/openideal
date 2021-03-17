<?php

namespace Drupal\openideal_rest\Event;

use Drupal\message\MessageInterface;
use Symfony\Component\EventDispatcher\Event;

/**
 * Represent various webhooks events.
 */
class WebhookEvent extends Event {

  /**
   * Rule id.
   *
   * @var string
   */
  protected $ruleId;

  /**
   * Message.
   *
   * @var \Drupal\message\MessageInterface
   */
  protected $message;

  /**
   * WebhookEvent constructor.
   *
   * @param string $rule_id
   *   Rule id.
   * @param \Drupal\message\MessageInterface $message
   *   Message.
   */
  public function __construct(string $rule_id, MessageInterface $message) {
    $this->ruleId = $rule_id;
    $this->message = $message;
  }

  /**
   * Get rule id.
   *
   * @return string
   *   Rule id.
   */
  public function getRuleId() {
    return $this->ruleId;
  }

  /**
   * Set rule id.
   *
   * @param string $ruleId
   *   Rule id.
   */
  public function setRuleId(string $ruleId) {
    $this->ruleId = $ruleId;
  }

  /**
   * Get message.
   *
   * @return \Drupal\message\MessageInterface
   *   Message.
   */
  public function getMessage() {
    return $this->message;
  }

  /**
   * Set message.
   *
   * @param \Drupal\message\MessageInterface $message
   *   Message.
   */
  public function setMessage(MessageInterface $message) {
    $this->message = $message;
  }

}
