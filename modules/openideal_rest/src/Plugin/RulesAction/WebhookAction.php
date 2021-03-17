<?php

namespace Drupal\openideal_rest\Plugin\RulesAction;

use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\openideal_rest\Event\WebhookEvent;
use Drupal\rules\Core\RulesActionBase;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\EventDispatcher\EventDispatcherInterface;

/**
 * Provides a Webhook action.
 *
 * @RulesAction(
 *   id = "openideal_rest_webhook",
 *   label = @Translation("Send webhook on OI events"),
 *   category = @Translation("Webhook"),
 *   context_definitions = {
 *     "type" = @ContextDefinition("string",
 *       label = @Translation("Webhook type"),
 *       required = TRUE
 *     ),
 *   }
 * )
 */
class WebhookAction extends RulesActionBase implements ContainerFactoryPluginInterface {

  use StringTranslationTrait;

  /**
   * Event dispatcher.
   *
   * @var \Symfony\Component\EventDispatcher\EventDispatcherInterface
   */
  protected $eventDispatcher;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    EventDispatcherInterface $eventDispatcher
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->eventDispatcher = $eventDispatcher;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('event_dispatcher')
    );
  }

  /**
   * Dispatch webhooks event.
   *
   * @param string $type
   *   Webhook type.
   */
  protected function doExecute(string $type) {

  }

}
