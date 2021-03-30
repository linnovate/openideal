<?php

namespace Drupal\openideal_webhooks\Plugin\RulesAction;

use Drupal\Component\Plugin\Derivative\DeriverBase;
use Drupal\Core\Plugin\Discovery\ContainerDeriverInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;
use Drupal\rules\Context\ContextDefinition;
use Drupal\rules\Context\ContextDefinitionInterface;
use Symfony\Component\DependencyInjection\ContainerInterface;

/**
 * Derives Webhooks type of plugin.
 *
 * @see \Drupal\openideal_webhooks\Plugin\RulesAction\WebhookAction
 */
class WebhookActionDeriver extends DeriverBase implements ContainerDeriverInterface {

  use StringTranslationTrait;

  /**
   * Config Factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory
   */
  protected $configFactory;

  /**
   * {@inheritDoc}
   */
  public static function create(ContainerInterface $container, $base_plugin_id) {
    $instance = new static();
    $instance->configFactory = $container->get('config.factory');
    return $instance;
  }

  /**
   * {@inheritDoc}
   */
  public function getDerivativeDefinitions($base_plugin_definition) {
    $plugins = $this->configFactory->get('webhooks.settings')->get('plugins') ?? [];

    foreach ($plugins as $plugin_id => $plugin) {
      // Add the derivative.
      $this->derivatives[$plugin_id] = [
        'label' => $this->t('Webhook of type @type', ['@type' => $plugin]),
        'category' => $this->t('Webhooks'),
        'provides' => [],
        'context_definitions' => [
          'entity' => ContextDefinition::create('entity')
            ->setRequired(TRUE)
            ->setAssignmentRestriction(ContextDefinitionInterface::ASSIGNMENT_RESTRICTION_SELECTOR)
            ->setLabel($this->t('Data or entity to send via webhook')),
          'event' => ContextDefinition::create('string')
            ->setLabel($this->t('Event to React'))
            ->setRequired(TRUE),
        ],
      ] + $base_plugin_definition;
    }

    return $this->derivatives;
  }

}
