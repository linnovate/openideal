<?php

namespace Drupal\openideal_user\ContextProvider;

use Drupal\Core\Cache\CacheableMetadata;
use Drupal\Core\Config\ConfigFactory;
use Drupal\Core\Plugin\Context\Context;
use Drupal\Core\Plugin\Context\ContextDefinition;
use Drupal\Core\Plugin\Context\ContextProviderInterface;
use Drupal\Core\StringTranslation\StringTranslationTrait;

/**
 * Makes the current path available as a context variable.
 */
class UserPointsContext implements ContextProviderInterface {

  use StringTranslationTrait;

  /**
   * Config factory.
   *
   * @var \Drupal\Core\Config\ConfigFactory
   */
  protected $configFactory;

  /**
   * Constructs a new CurrentPathContext.
   *
   * @param \Drupal\Core\Config\ConfigFactory $config_factory
   *   Config factory.
   */
  public function __construct(ConfigFactory $config_factory) {
    $this->configFactory = $config_factory;
  }

  /**
   * {@inheritdoc}
   */
  public function getRuntimeContexts(array $unqualified_context_ids) {
    $config = $this->configFactory->get('openideal_user.user_points_configuration');
    $values = [
      'vote' => $config->get('vote'),
      'comment' => $config->get('comment'),
      'idea' => $config->get('idea'),
    ];

    $context_definition = new ContextDefinition('user_points', $this->t('User points'));
    $context = new Context($context_definition, $values);
    $cacheability = new CacheableMetadata();
    $cacheability->setCacheTags($config->getCacheTags());
    $context->addCacheableDependency($cacheability);

    $result = [
      'points' => $context,
    ];

    return $result;
  }

  /**
   * {@inheritdoc}
   */
  public function getAvailableContexts() {
    return $this->getRuntimeContexts([]);
  }

}
