<?php

namespace Drupal\openideal_webhooks\Plugin\RulesAction;

use Drupal\Component\Serialization\Json;
use Drupal\Core\Entity\EntityInterface;
use Drupal\Core\Entity\EntityTypeManagerInterface;
use Drupal\Core\Plugin\ContainerFactoryPluginInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\message\MessageInterface;
use Drupal\rules\Core\RulesActionBase;
use Drupal\webhooks\Webhook;
use Drupal\webhooks\WebhooksService;
use Symfony\Component\DependencyInjection\ContainerInterface;
use Symfony\Component\Serializer\SerializerInterface;

/**
 * Provides a Webhook action.
 *
 * @RulesAction(
 *   id = "openideal_webhook",
 *   deriver = "Drupal\openideal_webhooks\Plugin\RulesAction\WebhookActionDeriver",
 * )
 */
class WebhookAction extends RulesActionBase implements ContainerFactoryPluginInterface {

  use StringTranslationTrait;

  /**
   * Outgoing webhook types.
   */
  const SLACK = 'slack';
  const OTHER = 'other';

  /**
   * Event dispatcher.
   *
   * @var \Symfony\Component\EventDispatcher\EventDispatcherInterface
   */
  protected $webhookService;

  /**
   * Entity type manager.
   *
   * @var \Drupal\Core\Entity\EntityTypeManagerInterface
   */
  protected $entityTypeManager;

  /**
   * Serializer.
   *
   * @var \Symfony\Component\Serializer\SerializerInterface
   */
  protected $serializer;

  /**
   * {@inheritDoc}
   */
  public function __construct(
    array $configuration,
    $plugin_id,
    $plugin_definition,
    WebhooksService $webhookService,
    EntityTypeManagerInterface $entityTypeManager,
    SerializerInterface $serializer
  ) {
    parent::__construct($configuration, $plugin_id, $plugin_definition);
    $this->webhookService = $webhookService;
    $this->entityTypeManager = $entityTypeManager;
    $this->serializer = $serializer;
  }

  /**
   * {@inheritdoc}
   */
  public static function create(ContainerInterface $container, array $configuration, $plugin_id, $plugin_definition) {
    return new static(
      $configuration,
      $plugin_id,
      $plugin_definition,
      $container->get('webhooks.service'),
      $container->get('entity_type.manager'),
      $container->get('serializer')
    );
  }

  /**
   * Dispatch webhooks event.
   *
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   Entity from fetch data.
   * @param string $event
   *   Event id.
   */
  protected function doExecute(EntityInterface $entity, $event) {
    // Remove default part of id.
    $id = preg_replace('/[a-z0-9_]+:/', '', $this->getPluginId());

    switch (strtolower($id)) {
      case self::SLACK:
        $this->reactOnSlack($entity, $event, $id);
        return;

      default:
        $this->reactOnDefault($entity, $event, $id);
    }
  }

  /**
   * Send slack webhooks.
   *
   * @param \Drupal\message\MessageInterface $entity
   *   Entity.
   * @param string $event
   *   Event.
   * @param string $id
   *   String plugin id.
   */
  protected function reactOnSlack(MessageInterface $entity, $event, $id) {
    $webhook_configs = $this->getWebConfigByEventAndPlugin($event, $id);

    $template_id = $entity->getTemplate()->id();
    // Only user_joined event don't have message partial field for
    // email because we don't have ability to follow the user.
    $delta = $template_id == 'user_joined' ? 1 : 3;
    $delta = $template_id == 'user_mention' ? 4 : $delta;

    // @todo Implement logic to identify message texts inside partial fields
    //   or create another field.
    $text = $entity->getText(NULL, $delta);
    $text = Json::decode(reset($text));
    foreach ($webhook_configs as $webhook_config) {
      $webhook = new Webhook(
        ['event' => $event] + $text,
        [],
        $event,
        $webhook_config->getContentType()
      );
      $this->webhookService->send($webhook_config, $webhook);
    }
  }

  /**
   * Default handler.
   *
   * @param \Drupal\Core\Entity\EntityInterface $entity
   *   Entity.
   * @param string $event
   *   Event.
   * @param string $id
   *   String plugin id.
   */
  protected function reactOnDefault(EntityInterface $entity, $event, $id) {
    $webhook_configs = $this->getWebConfigByEventAndPlugin($event, $id);

    $entity = $this->serializer->normalize($entity);

    foreach ($webhook_configs as $webhook_config) {
      $webhook = new Webhook(
        [
          'event' => $event,
          'entity' => $entity,
        ],
        [],
        $event,
        $webhook_config->getContentType()
      );
      $this->webhookService->send($webhook_config, $webhook);
    }
  }

  /**
   * Load webconfig by event and plugin.
   *
   * @param string $event
   *   Event type.
   * @param string $plugin
   *   Plugin type.
   *
   * @return \Drupal\webhooks\Entity\WebhookConfig[]
   *   Webconfig entities.
   */
  protected function getWebConfigByEventAndPlugin($event, $plugin) {
    $storage = $this->entityTypeManager->getStorage('webhook_config');
    $query = $storage->getQuery()
      ->condition('status', 1)
      ->condition('events', '"' . $event . '"', 'CONTAINS')
      ->condition('type', 'outgoing', '=')
      ->condition('third_party_settings.openideal_webhooks.plugin', $plugin, '=')
      ->accessCheck(FALSE);
    $ids = $query->execute();

    return $storage->loadMultiple($ids);
  }

}
